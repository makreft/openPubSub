#include "openPubSub.h"
#include <signal.h>
#include <open62541/plugin/log.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/plugin/pubsub.h>
#include <open62541/plugin/pubsub_udp.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

namespace openPubSub
{
    namespace {
        Server *_server;
        UA_Boolean _running;
    }
    struct Server::Impl
    {
        UA_NodeId m_connectionID;
        UA_NodeId m_publishedDataSetID;
        UA_NodeId m_dataSetWriterID;
        UA_NodeId m_writerGroupID;

        UA_WriterGroupConfig m_writerGroupConfig;
        UA_DataSetWriterConfig m_dataSetWriterConfig;

        std::string m_transportUri = "http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp";
        //multicast: opc.udp://224.0.0.22:4840/
        //unicast: opc.udp://127.0.0.1:4840/

        std::string m_networkUrl = "opc.udp://224.0.5.1:4840";

        UA_Server *mp_server;
        UA_ServerConfig *mp_config;
    };

    void Server::addPubSubTransportLayer()
    {
        mImpl->mp_config->pubsubTransportLayers = \
                (UA_PubSubTransportLayer *)UA_calloc(2, sizeof(UA_PubSubTransportLayer));
        if(!mImpl->mp_config->pubsubTransportLayers)
        {
            UA_Server_delete(mImpl->mp_server);
            throw transportlayerNotFound();
        }
        mImpl -> mp_config -> pubsubTransportLayers[0] = UA_PubSubTransportLayerUDPMP();
        mImpl -> mp_config -> pubsubTransportLayersSize++;
    }

    static void stopHandler()
    {
        _server->stopServer();
    }

    void initServer(Server &server)
    {
        _server = &server;
        signal(SIGINT, reinterpret_cast<__sighandler_t>(stopHandler));
        signal(SIGTERM, reinterpret_cast<__sighandler_t>(stopHandler));
    }
    void Server::stopServer()
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
        _running = false;
    }

    Server::Server()
    : mImpl(std::make_unique<Impl>())
    {
        _running=UA_TRUE;
        mImpl->mp_server = UA_Server_new();
        mImpl->mp_config = UA_Server_getConfig(mImpl->mp_server);
        UA_ServerConfig_setDefault(mImpl->mp_config);
    }
    Server::~Server()
    {
        UA_Server_delete(mImpl->mp_server);
    }
    void Server::run()
    {
        UA_StatusCode retVal = UA_Server_run(mImpl->mp_server, &_running);
        if (retVal != UA_STATUSCODE_GOOD)
            throw ua_exception(retVal);
    }
    void Server::addPubSubConnection(string nameOfPubSubConnection, int publisherID)
    {
        this->addPubSubTransportLayer();
        UA_PubSubConnectionConfig connectionConfig;
        memset(&connectionConfig, 0, sizeof(connectionConfig));
        connectionConfig.name = *nameOfPubSubConnection.value;
        connectionConfig.transportProfileUri = (UA_STRING(strdup(mImpl->m_transportUri.c_str())));

        UA_NetworkAddressUrlDataType networkAddressUrl = \
                {UA_STRING_NULL , (UA_STRING(strdup(mImpl->m_networkUrl.c_str())))};
        connectionConfig.enabled = UA_TRUE;
        UA_Variant_setScalar(&connectionConfig.address, &networkAddressUrl,
                             &UA_TYPES[UA_TYPES_NETWORKADDRESSURLDATATYPE]);
        connectionConfig.publisherId.numeric = publisherID;
        UA_Server_addPubSubConnection(mImpl->mp_server, &connectionConfig,
                                      &mImpl->m_connectionID);
    }
    void Server::addPublishedDataSet(string nameOfPublishedDS)
    {
        UA_PublishedDataSetConfig publishedDataSetConfig;
        memset(&publishedDataSetConfig, 0, sizeof(UA_PublishedDataSetConfig));
        publishedDataSetConfig.publishedDataSetType = \
                UA_PUBSUB_DATASET_PUBLISHEDITEMS;
        publishedDataSetConfig.name = *nameOfPublishedDS.value;
        UA_Server_addPublishedDataSet(mImpl->mp_server, &publishedDataSetConfig,
                                      &mImpl->m_publishedDataSetID);
    }
    void Server::addDataSetField(string nameOfDSField)
    {
        UA_NodeId dataSetField;
        UA_DataSetFieldConfig dataSetFieldConfig;
        memset(&dataSetFieldConfig, 0, sizeof(UA_DataSetFieldConfig));
        dataSetFieldConfig.dataSetFieldType = UA_PUBSUB_DATASETFIELD_VARIABLE;
        dataSetFieldConfig.field.variable.fieldNameAlias = *nameOfDSField.value;
        dataSetFieldConfig.field.variable.promotedField = UA_FALSE;
        dataSetFieldConfig.field.variable.publishParameters.publishedVariable = \
                UA_NODEID_NUMERIC(0, UA_NS0ID_SERVERSTATUSTYPE_CURRENTTIME);
        dataSetFieldConfig.field.variable.publishParameters.attributeId = \
                UA_ATTRIBUTEID_VALUE;
        UA_Server_addDataSetField(mImpl->mp_server, mImpl->m_publishedDataSetID,
                                  &dataSetFieldConfig,
                                  &dataSetField);
    }
    void Server::addWriterGroup(string nameOfWriterGroup, int publishingInterval,
                                int writerGroupId)
    {
        memset(&mImpl->m_writerGroupConfig, 0, sizeof(UA_WriterGroupConfig));
        mImpl->m_writerGroupConfig.name = *nameOfWriterGroup.value;
        mImpl->m_writerGroupConfig.publishingInterval = publishingInterval;
        mImpl->m_writerGroupConfig.enabled = UA_FALSE;
        mImpl->m_writerGroupConfig.writerGroupId = writerGroupId;
        mImpl->m_writerGroupConfig.encodingMimeType = UA_PUBSUB_ENCODING_UADP;
        mImpl->m_writerGroupConfig.messageSettings.encoding = UA_EXTENSIONOBJECT_DECODED;
        mImpl->m_writerGroupConfig.messageSettings.content.decoded.type = \
                &UA_TYPES[UA_TYPES_UADPWRITERGROUPMESSAGEDATATYPE];
        UA_UadpWriterGroupMessageDataType *writerGroupMessage = \
                UA_UadpWriterGroupMessageDataType_new();
        writerGroupMessage->networkMessageContentMask = \
            (UA_UadpNetworkMessageContentMask)(UA_UADPNETWORKMESSAGECONTENTMASK_PUBLISHERID  |
            (UA_UadpNetworkMessageContentMask) UA_UADPNETWORKMESSAGECONTENTMASK_GROUPHEADER  |
            (UA_UadpNetworkMessageContentMask) UA_UADPNETWORKMESSAGECONTENTMASK_WRITERGROUPID|
            (UA_UadpNetworkMessageContentMask) UA_UADPNETWORKMESSAGECONTENTMASK_PAYLOADHEADER);
        mImpl->m_writerGroupConfig.messageSettings.content.decoded.data = writerGroupMessage;
        UA_Server_addWriterGroup(mImpl->mp_server, mImpl->m_connectionID,
                                 &mImpl->m_writerGroupConfig, &mImpl->m_writerGroupID);
        UA_Server_setWriterGroupOperational(mImpl->mp_server, mImpl->m_writerGroupID);
        UA_UadpWriterGroupMessageDataType_delete(writerGroupMessage);
    }
    void Server::addDataSetWriter(string nameOfDSWriter)
    {
        memset(&mImpl->m_dataSetWriterConfig, 0, sizeof(UA_DataSetWriterConfig));
        mImpl->m_dataSetWriterConfig.name = *nameOfDSWriter.value;
        mImpl->m_dataSetWriterConfig.dataSetWriterId = 62541;
        mImpl->m_dataSetWriterConfig.keyFrameCount = 10;
        UA_Server_addDataSetWriter(mImpl->mp_server, mImpl->m_writerGroupID,
                                   mImpl->m_publishedDataSetID, &mImpl->m_dataSetWriterConfig,
                                   &mImpl->m_dataSetWriterID);
    }
    void Server::setTransportProfileUri(const std::string &transportProfileUri)
    {
        mImpl->m_transportUri = transportProfileUri;
    }
    void Server::setNetworkAddressUrl(const std::string &networkAddressUrl)
    {
        mImpl->m_networkUrl = networkAddressUrl;
    }
    bool Server::isRunning()
    {
        if (_running)
            return true;
        else
            return false;
    }
    UA_ServerConfig * Server::getUAServerConfig() {
       return mImpl->mp_config;
    }
    void Server::setCustomServerConfig(UA_ServerConfig* serverConfig) {
        mImpl->mp_config=serverConfig;
    }

    UA_Server *Server::getUAServer() {
        return mImpl->mp_server;
    }
}
