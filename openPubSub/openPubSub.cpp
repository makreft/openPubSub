#include "openPubSub.h"
#include <signal.h>
#if defined (__cplusplus)
extern "C" {
#endif
#include <open62541/plugin/log_stdout.h>
#include <open62541/plugin/pubsub_udp.h>
#include <open62541/plugin/pubsub_ethernet.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#if defined (__cplusplus)
}
#endif


namespace openPubSub
{
    //In order avoid the signal initialization.
    Server *__server;
    static void stopHandler()
    {
        __server->stopServer();
    }
    void init(Server &server)
    {
        __server = &server;
        signal(SIGINT, reinterpret_cast<__sighandler_t>(stopHandler));
        signal(SIGTERM, reinterpret_cast<__sighandler_t>(stopHandler));
    }
    struct Server::Impl
    {
        //UA_NodeId m_connectionID;
        //UA_NodeId m_publishedDataSetID;
        //UA_NodeId m_dataSetWriterID;
        //UA_NodeId m_writerGroupID;
        UA_StatusCode m_status = UA_STATUSCODE_GOOD;
        int m_defaultTcpAddressValue = 4840;
        UA_Boolean m_running;
        UA_NodeId connectionIdent, publishedDataSetIdent, writerGroupIdent;

        UA_WriterGroupConfig m_writerGroupConfig;
        UA_DataSetWriterConfig m_dataSetWriterConfig;
        UA_PubSubConnectionConfig m_pubSubConnectionConfig;

        //string m_transportUri = "http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp";
        //UA_String transportProfile =
        //    UA_STRING("http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp");
        //UA_NetworkAddressUrlDataType networkAddressUrl =
        //    {UA_STRING_NULL , UA_STRING("opc.udp://224.0.0.22:4840/")};
        UA_NetworkAddressUrlDataType transportProfile = {UA_STRING_NULL, UA_STRING("http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp")};
        ////multicast: opc.udp://224.0.0.22:4840/
        ////unicast: opc.udp://127.0.0.1:4840/

        //UA_NetworkAddressUrlDataType networkAddressUrl = {UA_STRING_NULL, UA_STRING("opc.udp://224.0.0.22:4840/")};

        UA_Server *mp_server;
        UA_ServerConfig *mp_config;
    };



    void Server::addVariableNode(UA_NodeId variableNodeId, const UA_NodeId folderId,
                                 char* nodeDisplayName, char* browseName,
                                 UA_Boolean UA_DataType, bool setAccessLevel)
    {
        UA_NodeId_init(&variableNodeId);
        UA_VariableAttributes varNodeAttr = UA_VariableAttributes_default;
        varNodeAttr.valueRank = -1;
        UA_NodeId_copy(&UA_TYPES[UA_TYPES_BOOLEAN].typeId, &varNodeAttr.dataType);
        if(setAccessLevel)
            varNodeAttr.accessLevel = UA_ACCESSLEVELMASK_READ ^ UA_ACCESSLEVELMASK_WRITE;
        UA_Variant_setScalar(&varNodeAttr.value, &UA_DataType, &UA_TYPES[UA_TYPES_BOOLEAN]);
        varNodeAttr.displayName = UA_LOCALIZEDTEXT("en-US", nodeDisplayName);
        UA_Server_addVariableNode(mImpl->mp_server, UA_NODEID_STRING(1, browseName),
                                  folderId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                                  UA_QUALIFIEDNAME(1, nodeDisplayName),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  varNodeAttr, NULL, &variableNodeId);
    }
    void Server::addVariableNode(UA_NodeId variableNodeId, const UA_NodeId folderId,
                                 char* nodeDisplayName, char* browseName,
                                 UA_NodeId UA_DataType, bool setAccessLevel)
    {
        UA_NodeId_init(&variableNodeId);
        UA_VariableAttributes varNodeAttr = UA_VariableAttributes_default;
        varNodeAttr.valueRank = -1;
        UA_NodeId_copy(&UA_TYPES[UA_TYPES_INT32].typeId, &varNodeAttr.dataType);
        if(setAccessLevel)
            varNodeAttr.accessLevel = UA_ACCESSLEVELMASK_READ ^ UA_ACCESSLEVELMASK_WRITE;
        UA_Variant_setScalar(&varNodeAttr.value, &UA_DataType, &UA_TYPES[UA_TYPES_INT32]);
        varNodeAttr.displayName = UA_LOCALIZEDTEXT("en-US", nodeDisplayName);
        UA_Server_addVariableNode(mImpl->mp_server, UA_NODEID_STRING(1, browseName),
                                  folderId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                                  UA_QUALIFIEDNAME(1, nodeDisplayName),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  varNodeAttr, NULL, &variableNodeId);


    }
    void Server::addVariableNode(UA_NodeId variableNodeId, const UA_NodeId folderId,
                                 char* nodeDisplayName, char* browseName,
                                 UA_DateTime UA_DataType, bool setAccessLevelMask)
    {
        UA_NodeId_init(&variableNodeId);
        UA_VariableAttributes varNodeAttr = UA_VariableAttributes_default;
        varNodeAttr.valueRank = -1;
        UA_NodeId_copy(&UA_TYPES[UA_TYPES_DATETIME].typeId, &varNodeAttr.dataType);
        if(setAccessLevelMask)
            varNodeAttr.accessLevel = UA_ACCESSLEVELMASK_READ ^ UA_ACCESSLEVELMASK_WRITE;
        UA_Variant_setScalar(&varNodeAttr.value, &UA_DataType, &UA_TYPES[UA_TYPES_DATETIME]);
        varNodeAttr.displayName = UA_LOCALIZEDTEXT("en-US", nodeDisplayName);
        UA_Server_addVariableNode(mImpl->mp_server, UA_NODEID_STRING(1, browseName),
                                  folderId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                                  UA_QUALIFIEDNAME(1, nodeDisplayName),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  varNodeAttr, NULL, &variableNodeId);


    }
    void Server::addObjectNode(char * publisherName, UA_NodeId folderId)
    {
        UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
        UA_Server_addObjectNode(mImpl->mp_server, UA_NODEID_NULL,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                UA_QUALIFIEDNAME(1, publisherName),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                oAttr, NULL, &folderId);
    }

    UA_StatusCode Server::addReaderGroup()
    {
        UA_NodeId connectionIdentifier;
        UA_NodeId readerGroupIdentifier;
        if (mImpl->mp_server == NULL)
            return UA_STATUSCODE_BADINTERNALERROR;

        UA_StatusCode retval = UA_STATUSCODE_GOOD;
        UA_ReaderGroupConfig readerGroupConfig;
        memset(&readerGroupConfig, 0, sizeof(UA_ReaderGroupConfig));
        readerGroupConfig.name = UA_STRING("ReaderGroup1");
        //readerGroupConfig.rtLevel = UA_PUBSUB_RT_FIXED_SIZE;
        retval |= UA_Server_addReaderGroup(mImpl->mp_server, connectionIdentifier,
                                           &readerGroupConfig,
                                           &readerGroupIdentifier);
        return retval;
    }


    void Server::stopServer()
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
        mImpl->m_running = false;
    }

    Server::Server()
    : mImpl(std::make_unique<Impl>())
{
        mImpl->m_running=UA_TRUE;
        mImpl->mp_server = UA_Server_new();
        mImpl->mp_config = UA_Server_getConfig(mImpl->mp_server);
        mImpl->m_status = UA_ServerConfig_setMinimal(mImpl->mp_config,
                                                     mImpl->m_defaultTcpAddressValue,
                                                     NULL);
        //mImpl->m_status = UA_ServerConfig_setDefault(mImpl->mp_config);
        mImpl->mp_config->pubsubTransportLayers = \
                (UA_PubSubTransportLayer *)UA_calloc(2, sizeof(UA_PubSubTransportLayer));
        if(!mImpl->mp_config->pubsubTransportLayers)
        {
            UA_Server_delete(mImpl->mp_server);
            throw transportlayerNotFound();
        }
        mImpl -> mp_config -> pubsubTransportLayers[0] = UA_PubSubTransportLayerUDPMP();
        mImpl -> mp_config -> pubsubTransportLayersSize++;
#ifdef UA_ENABLE_PUBSUB_ETH_UADP
        mImpl -> mp_config ->pubsubTransportLayers[1] = UA_PubSubTransportLayerEthernet();
        mImpl -> mp_config ->pubsubTransportLayersSize++;
#endif
}

    Server::~Server()
    {
        UA_Server_delete(mImpl->mp_server);
    }
    void Server::run()
    {
        UA_StatusCode retVal = UA_Server_run(mImpl->mp_server, &mImpl->m_running);
        if (retVal != UA_STATUSCODE_GOOD)
            throw ua_exception(retVal);
    }

   void Server::addPubSubConnection(UA_NetworkAddressUrlDataType* networkAddressUrl){
       /* Details about the connection configuration and handling are located
        * in the pubsub connection tutorial */
       UA_PubSubConnectionConfig connectionConfig;
       memset(&connectionConfig, 0, sizeof(connectionConfig));
       connectionConfig.name = UA_STRING("UADP Connection 1");
       connectionConfig.transportProfileUri = mImpl->transportProfile.url;
       connectionConfig.enabled = UA_TRUE;
       UA_Variant_setScalar(&connectionConfig.address, networkAddressUrl,
                            &UA_TYPES[UA_TYPES_NETWORKADDRESSURLDATATYPE]);
       /* Changed to static publisherId from random generation to identify
        * the publisher on Subscriber side */
       connectionConfig.publisherId.numeric = 2234;
       UA_Server_addPubSubConnection(mImpl->mp_server, &connectionConfig, &mImpl->connectionIdent);
   }

    void Server::addPublishedDataSet(){
        /* The PublishedDataSetConfig contains all necessary public
        * informations for the creation of a new PublishedDataSet */
        UA_PublishedDataSetConfig publishedDataSetConfig;
        memset(&publishedDataSetConfig, 0, sizeof(UA_PublishedDataSetConfig));
        publishedDataSetConfig.publishedDataSetType = UA_PUBSUB_DATASET_PUBLISHEDITEMS;
        publishedDataSetConfig.name = UA_STRING("Demo PDS");
        /* Create new PublishedDataSet based on the PublishedDataSetConfig. */
        UA_Server_addPublishedDataSet(mImpl->mp_server, &publishedDataSetConfig,
                                      &mImpl->publishedDataSetIdent);
    }
    // DataSetField
    // * The DataSetField (DSF) is part of the PDS and describes exactly one published
    // * field. */
    void Server::addDataSetField() {
        /* Add a field to the previous created PublishedDataSet */
        UA_NodeId dataSetFieldIdent;
        UA_DataSetFieldConfig dataSetFieldConfig;
        memset(&dataSetFieldConfig, 0, sizeof(UA_DataSetFieldConfig));
        dataSetFieldConfig.dataSetFieldType = UA_PUBSUB_DATASETFIELD_VARIABLE;
        dataSetFieldConfig.field.variable.fieldNameAlias = UA_STRING("Server localtime");
        dataSetFieldConfig.field.variable.promotedField = UA_FALSE;
        dataSetFieldConfig.field.variable.publishParameters.publishedVariable =
            UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
        dataSetFieldConfig.field.variable.publishParameters.attributeId = UA_ATTRIBUTEID_VALUE;
        UA_Server_addDataSetField(mImpl->mp_server, mImpl->publishedDataSetIdent,
                                  &dataSetFieldConfig, &dataSetFieldIdent);
    }
    void Server::addWriterGroup() {
        /* Now we create a new WriterGroupConfig and add the group to the existing
         * PubSubConnection. */
        UA_WriterGroupConfig writerGroupConfig;
        memset(&writerGroupConfig, 0, sizeof(UA_WriterGroupConfig));
        writerGroupConfig.name = UA_STRING("Demo WriterGroup");
        writerGroupConfig.publishingInterval = 100;
        writerGroupConfig.enabled = UA_FALSE;
        writerGroupConfig.writerGroupId = 100;
        writerGroupConfig.encodingMimeType = UA_PUBSUB_ENCODING_UADP;
        writerGroupConfig.messageSettings.encoding = UA_EXTENSIONOBJECT_DECODED;
        writerGroupConfig.messageSettings.content.decoded.type = &UA_TYPES[UA_TYPES_UADPWRITERGROUPMESSAGEDATATYPE];
        /* The configuration flags for the messages are encapsulated inside the
         * message- and transport settings extension objects. These extension
         * objects are defined by the standard. e.g.
         * UadpWriterGroupMessageDataType */
        UA_UadpWriterGroupMessageDataType *writerGroupMessage = UA_UadpWriterGroupMessageDataType_new();
        /* Change message settings of writerGroup to send PublisherId,
         * WriterGroupId in GroupHeader and DataSetWriterId in PayloadHeader
         * of NetworkMessage */
        writerGroupMessage->networkMessageContentMask = (UA_UadpNetworkMessageContentMask) (
            UA_UADPNETWORKMESSAGECONTENTMASK_PUBLISHERID |
            (UA_UadpNetworkMessageContentMask) UA_UADPNETWORKMESSAGECONTENTMASK_GROUPHEADER |
            (UA_UadpNetworkMessageContentMask) UA_UADPNETWORKMESSAGECONTENTMASK_WRITERGROUPID |
            (UA_UadpNetworkMessageContentMask) UA_UADPNETWORKMESSAGECONTENTMASK_PAYLOADHEADER);
        writerGroupConfig.messageSettings.content.decoded.data = writerGroupMessage;
        UA_Server_addWriterGroup(mImpl->mp_server, mImpl->connectionIdent, &writerGroupConfig,
                                 &mImpl->writerGroupIdent);
        UA_Server_setWriterGroupOperational(mImpl->mp_server, mImpl->writerGroupIdent);
        UA_UadpWriterGroupMessageDataType_delete(writerGroupMessage);
    }
        // * DataSetWriter
    // * -------------
    // * The DataSetWriters are the glue between the WriterGroups and the
    // * PublishedDataSets. The DataSetWriter contain configuration parameters and
    // * flags which influence the creation of DataSet messages. These messages are
    // * encapsulated inside the network message. The DataSetWriter must be linked
    // * with an existing PublishedDataSet and be contained within a WriterGroup. */
    void Server::addDataSetWriter() {
        /* We need now a DataSetWriter within the WriterGroup. This means we must
         * create a new DataSetWriterConfig and add call the addWriterGroup function. */
        UA_NodeId dataSetWriterIdent;
        UA_DataSetWriterConfig dataSetWriterConfig;
        memset(&dataSetWriterConfig, 0, sizeof(UA_DataSetWriterConfig));
        dataSetWriterConfig.name = UA_STRING("Demo DataSetWriter");
        dataSetWriterConfig.dataSetWriterId = 62541;
        dataSetWriterConfig.keyFrameCount = 10;
        UA_Server_addDataSetWriter(mImpl->mp_server, mImpl->writerGroupIdent,
                                   mImpl->publishedDataSetIdent, &dataSetWriterConfig,
                                   &dataSetWriterIdent);
    }
    bool Server::isRunning()
    {
        if (mImpl->m_running)
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
