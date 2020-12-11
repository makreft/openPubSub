#include "openPubSub.h"
#include <iostream>

namespace openPubSub
{
    Server *p_server;

    static void stopHandler()
    {
        p_server->stopServer();
    }

    void init(Server &server)
    {
        p_server = &server;
        signal(SIGINT, reinterpret_cast<__sighandler_t>(stopHandler));
        signal(SIGTERM, reinterpret_cast<__sighandler_t>(stopHandler));
    }

    Server::Server(std::string transportLayer)
    {
        mp_running=true;
        mp_server = UA_Server_new();
        mp_config = UA_Server_getConfig(mp_server);
        UA_ServerConfig_setDefault(mp_config);
        setNetworkAddressUrl();
        setTransportProfileUri();
        if (transportLayer == "UDP")
        {
            this->addPubSubTransportLayer(UA_PubSubTransportLayerUDPMP());
        }
        else if (transportLayer == "MQTT")
        {
            //TODO: implement MQTT
            this->addPubSubTransportLayer(UA_PubSubTransportLayerUDPMP());
        }
    }

    Server::~Server()
    {
        UA_Server_delete(mp_server);
    }

    void Server::run()
    {
        UA_StatusCode retVal = UA_Server_run(mp_server, &mp_running);
        if (retVal != UA_STATUSCODE_GOOD)
            throw ua_exception(retVal);
    }

    void Server::stopServer()
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
        mp_running = false;
    }

    void Server::addPubSubConnection(std::string nameOfPubSubConnection)
    {
        setNameOfPubSubConnection(nameOfPubSubConnection);
        UA_PubSubConnectionConfig connectionConfig;
        memset(&connectionConfig, 0, sizeof(connectionConfig));
        connectionConfig.name = UA_STRING(strdup(m_namePubSubConnection.c_str()));
        connectionConfig.transportProfileUri = UA_STRING(strdup(m_transportUri.c_str()));
        UA_NetworkAddressUrlDataType networkAddressUrl =
            {UA_STRING_NULL , UA_STRING(strdup(m_networkUrl.c_str()))};
        connectionConfig.enabled = UA_TRUE;
        UA_Variant_setScalar(&connectionConfig.address, &networkAddressUrl,
                             &UA_TYPES[UA_TYPES_NETWORKADDRESSURLDATATYPE]);
        connectionConfig.publisherId.numeric = 2234;
        UA_Server_addPubSubConnection(mp_server, &connectionConfig,
                                      &m_connectionID);
    }

    void Server::addPublishedDataSet(std::string nameOfPublishedDS)
    {
        setNameOfPublishedDataSet(nameOfPublishedDS);
        UA_PublishedDataSetConfig publishedDataSetConfig;
        memset(&publishedDataSetConfig, 0, sizeof(UA_PublishedDataSetConfig));
        publishedDataSetConfig.publishedDataSetType =
                                            UA_PUBSUB_DATASET_PUBLISHEDITEMS;
        publishedDataSetConfig.name = UA_STRING(strdup(m_namePublishedDS.c_str()));
        UA_Server_addPublishedDataSet(mp_server, &publishedDataSetConfig,
                                      &m_publishedDataSetID);
    }

    void Server::addDataSetField(std::string nameOfDSField)
    {
        UA_DataSetFieldConfig dataSetFieldConfig;
        memset(&dataSetFieldConfig, 0, sizeof(UA_DataSetFieldConfig));
        dataSetFieldConfig.dataSetFieldType = UA_PUBSUB_DATASETFIELD_VARIABLE;
        dataSetFieldConfig.field.variable.fieldNameAlias =
            UA_STRING(strdup(nameOfDSField.c_str()));
        dataSetFieldConfig.field.variable.promotedField = UA_FALSE;
        dataSetFieldConfig.field.variable.publishParameters.publishedVariable =
            UA_NODEID_NUMERIC(0, UA_NS0ID_SERVERSTATUSTYPE_CURRENTTIME);
        dataSetFieldConfig.field.variable.publishParameters.attributeId =
            UA_ATTRIBUTEID_VALUE;
        UA_Server_addDataSetField(mp_server, m_publishedDataSetID,
                                  &dataSetFieldConfig,
                                  &m_dataSetFieldID);
    }

    void Server::addWriterGroup(std::string nameOfWriterGroup)
    {
        //setNameOfWriterGroup();
        memset(&m_writerGroupConfig, 0, sizeof(UA_WriterGroupConfig));
        m_writerGroupConfig.name = UA_STRING(strdup(nameOfWriterGroup.c_str()));
        m_writerGroupConfig.publishingInterval = 100;
        m_writerGroupConfig.enabled = UA_FALSE;
        m_writerGroupConfig.writerGroupId = 100;
        m_writerGroupConfig.encodingMimeType = UA_PUBSUB_ENCODING_UADP;
        m_writerGroupConfig.messageSettings.encoding = UA_EXTENSIONOBJECT_DECODED;
        m_writerGroupConfig.messageSettings.content.decoded.type =
            &UA_TYPES[UA_TYPES_UADPWRITERGROUPMESSAGEDATATYPE];
        UA_UadpWriterGroupMessageDataType *writerGroupMessage =
            UA_UadpWriterGroupMessageDataType_new();
        writerGroupMessage->networkMessageContentMask =
            ( UA_UadpNetworkMessageContentMask )( UA_UADPNETWORKMESSAGECONTENTMASK_PUBLISHERID   |
            ( UA_UadpNetworkMessageContentMask )  UA_UADPNETWORKMESSAGECONTENTMASK_GROUPHEADER   |
            ( UA_UadpNetworkMessageContentMask )  UA_UADPNETWORKMESSAGECONTENTMASK_WRITERGROUPID |
            ( UA_UadpNetworkMessageContentMask )  UA_UADPNETWORKMESSAGECONTENTMASK_PAYLOADHEADER );
        m_writerGroupConfig.messageSettings.content.decoded.data = writerGroupMessage;
        UA_Server_addWriterGroup(mp_server, m_connectionID,
                                 &m_writerGroupConfig, &m_writerGroupID);
        UA_Server_setWriterGroupOperational(mp_server, m_writerGroupID);
        UA_UadpWriterGroupMessageDataType_delete(writerGroupMessage);
    }

    void Server::addDataSetWriter(std::string nameOfDSWriter)
    {
        memset(&m_dataSetWriterConfig, 0, sizeof(UA_DataSetWriterConfig));
        m_dataSetWriterConfig.name = UA_STRING(strdup(nameOfDSWriter.c_str()));
        m_dataSetWriterConfig.dataSetWriterId = 62541;
        m_dataSetWriterConfig.keyFrameCount = 10;
        UA_Server_addDataSetWriter(mp_server, m_writerGroupID, m_publishedDataSetID,
                                   &m_dataSetWriterConfig, &m_dataSetWriterID);
    }

    void Server::setNameOfPubSubConnection(std::string namePSC)
    {
        m_namePubSubConnection = namePSC;
    }
    void Server::setTransportProfileUri(std::string transportProfileUri)
    {
        m_transportUri = transportProfileUri;
    }
    void Server::setNetworkAddressUrl(std::string networkAddressUrl)
    {
        m_networkUrl = networkAddressUrl;
    }
    void Server::setNameOfPublishedDataSet(std::string namePDS)
    {
        m_namePublishedDS = namePDS;
    }
}
