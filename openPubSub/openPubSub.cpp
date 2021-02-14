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

// left here to build the future
//#include "VariableNode.h"

namespace openPubSub
{
//In order avoid the signal initialization.
Server *__server;
static void stopHandler(void)
{
    __server->stopServer();
}
//implementation details are hidden for high-level usage
struct Server::Impl
{
    //UA_NodeId m_connectionID;
    //UA_NodeId m_publishedDataSetID;
    //UA_NodeId m_dataSetWriterID;
    //UA_NodeId m_writerGroupID;
    UA_StatusCode m_status = UA_STATUSCODE_GOOD;
    int m_defaultTcpAddressValue = 4840;
    UA_Boolean m_running;
    UA_NodeId m_connectionIdent, m_publishedDataSetIdent, m_writerGroupIdent;
    UA_NodeId m_dataSetWriterIdent;
    /*
     * TODO: Think of a data structure so that
     * multiple DataSets, Fields and so on can be
     * saved and called.
     */

    UA_WriterGroupConfig m_writerGroupConfig;
    UA_DataSetWriterConfig m_dataSetWriterConfig;
    UA_PubSubConnectionConfig m_pubSubConnectionConfig;
    UA_NetworkAddressUrlDataType transportProfile = {UA_STRING_NULL, \
       UA_STRING("http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp")};
    UA_NodeId sampleTypeId = {1, UA_NODEIDTYPE_NUMERIC, {1001}};
    UA_Server *m_server;
    UA_ServerConfig *m_config;

    UA_UInt64 callbackId = 0;
};
//stopServer is triggered by ctrl-c stop signal
void Server::stopServer()
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    mImpl->m_running = false;
}
void init(Server &server)

{
    __server = &server;
    signal(SIGINT, reinterpret_cast<__sighandler_t>(stopHandler));
    signal(SIGTERM, reinterpret_cast<__sighandler_t>(stopHandler));
}

bool Server::isRunning(void)
{
    if (mImpl->m_running)
        return true;
    else
        return false;
}
Server::Server()
    : mImpl(std::make_unique<Impl>())
{
    mImpl->m_running = UA_TRUE;
    mImpl->m_server = UA_Server_new();
    mImpl->m_config = UA_Server_getConfig(mImpl->m_server);
    mImpl->m_status  = UA_ServerConfig_setMinimal(mImpl->m_config,
                                                 mImpl->m_defaultTcpAddressValue,
                                                 NULL);
    //mImpl->m_status = UA_ServerConfig_setDefault(mImpl->m_config);
    mImpl->m_config->pubsubTransportLayers = \
            (UA_PubSubTransportLayer *)UA_calloc(2, sizeof(UA_PubSubTransportLayer));
    if(!mImpl->m_config->pubsubTransportLayers)
    {
        UA_Server_delete(mImpl->m_server);
        throw transportlayerNotFound();
    }
    mImpl -> m_config -> pubsubTransportLayers[0] = UA_PubSubTransportLayerUDPMP();
    mImpl -> m_config -> pubsubTransportLayersSize++;
#ifdef UA_ENABLE_PUBSUB_ETH_UADP
        mImpl -> m_config ->pubsubTransportLayers[1] = UA_PubSubTransportLayerEthernet();
        mImpl -> m_config ->pubsubTransportLayersSize++;
#endif
}

Server::~Server(void)
{
    UA_Server_delete(mImpl->m_server);
}

void Server::run(void)
{
    UA_StatusCode retVal = UA_Server_run(mImpl->m_server, &mImpl->m_running);
    if (retVal != UA_STATUSCODE_GOOD)
        throw ua_exception(retVal);
}
//==============================================================================
//============PubSub Callback Config=============================
//==============================================================================




//==============================================================================
//============PubSub Connection Handling========================================
//==============================================================================

UA_StatusCode Server::addRepeatedCallback(UA_ServerCallback callback)
{
    UA_StatusCode code = UA_Server_addRepeatedCallback(mImpl->m_server, (UA_ServerCallback)callback, NULL, 1000, &mImpl->callbackId);
    return code;
}
void Server::addPubSubConnection(UA_NetworkAddressUrlDataType *networkAddressUrl,
                            char *connectionName, const int pubId) {
    /* Details about the connection configuration and handling are located
     * in the pubsub connection tutorial */
    UA_PubSubConnectionConfig connectionConfig;
    memset(&connectionConfig, 0, sizeof(connectionConfig));
    connectionConfig.name = UA_STRING(connectionName);
    connectionConfig.transportProfileUri = mImpl->transportProfile.url;
    connectionConfig.enabled = UA_TRUE;
    UA_Variant_setScalar(&connectionConfig.address, networkAddressUrl,
                         &UA_TYPES[UA_TYPES_NETWORKADDRESSURLDATATYPE]);
    /* Changed to static publisherId from random generation to identify
     * the publisher on Subscriber side */
    connectionConfig.publisherId.numeric = pubId;
    UA_Server_addPubSubConnection(mImpl->m_server, &connectionConfig, &mImpl->m_connectionIdent);
}

void Server::addPublishedDataSet(char *pdsName) {
    /* The PublishedDataSetConfig contains all necessary public
    * informations for the creation of a new PublishedDataSet */
    UA_PublishedDataSetConfig publishedDataSetConfig;
    memset(&publishedDataSetConfig, 0, sizeof(UA_PublishedDataSetConfig));
    publishedDataSetConfig.publishedDataSetType = UA_PUBSUB_DATASET_PUBLISHEDITEMS;
    publishedDataSetConfig.name = UA_STRING(pdsName);
    /* Create new PublishedDataSet based on the PublishedDataSetConfig. */
    UA_Server_addPublishedDataSet(mImpl->m_server, &publishedDataSetConfig,
                                  &mImpl->m_publishedDataSetIdent);
}
// DataSetField
// * The DataSetField (DSF) is part of the PDS and describes exactly one published
// * field. */

void Server::addDataSetField(char *fieldName) {
    /* Add a field to the previous created PublishedDataSet */

    UA_NodeId dataSetFieldIdent;
    UA_DataSetFieldConfig dataSetFieldConfig;
    memset(&dataSetFieldConfig, 0, sizeof(UA_DataSetFieldConfig));
    dataSetFieldConfig.dataSetFieldType = UA_PUBSUB_DATASETFIELD_VARIABLE;
    dataSetFieldConfig.field.variable.fieldNameAlias = UA_STRING(fieldName);
    dataSetFieldConfig.field.variable.promotedField = UA_FALSE;
    dataSetFieldConfig.field.variable.publishParameters.publishedVariable =
        UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
    dataSetFieldConfig.field.variable.publishParameters.attributeId = UA_ATTRIBUTEID_VALUE;
    UA_Server_addDataSetField(mImpl->m_server, mImpl->m_publishedDataSetIdent,
                              &dataSetFieldConfig, &dataSetFieldIdent);
}

void Server::addInt32DataSetField(UA_NodeId publishedVariable)
{
    /* Add a field to the previous created PublishedDataSet */
    UA_DataSetFieldConfig dataSetFieldConfig;
    UA_NodeId fInt32;
    memset(&dataSetFieldConfig, 0, sizeof(UA_DataSetFieldConfig));
    //dataSetFieldConfig.dataSetFieldType = UA_PUBSUB_DATASETFIELD_VARIABLE;
    dataSetFieldConfig.field.variable.fieldNameAlias = UA_STRING("Int32");
    dataSetFieldConfig.field.variable.promotedField = UA_FALSE;
    dataSetFieldConfig.field.variable.publishParameters.publishedVariable = publishedVariable;
    dataSetFieldConfig.field.variable.publishParameters.attributeId = UA_ATTRIBUTEID_VALUE;
    UA_Server_addDataSetField(mImpl->m_server, mImpl->m_publishedDataSetIdent,
                              &dataSetFieldConfig, &fInt32);
}

void Server::addWriterGroup(void)
{
    /* Now we create a new WriterGroupConfig and add the group to the existing
     * PubSubConnection. */
    UA_WriterGroupConfig writerGroupConfig;
    memset(&writerGroupConfig, 0, sizeof(UA_WriterGroupConfig));
    writerGroupConfig.name = UA_STRING("Int32 WriterGroup");
    writerGroupConfig.publishingInterval = 500;
    writerGroupConfig.enabled = UA_FALSE;
    writerGroupConfig.writerGroupId = 100;
    writerGroupConfig.maxEncapsulatedDataSetMessageCount = 3;
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
    UA_Server_addWriterGroup(mImpl->m_server, mImpl->m_connectionIdent, &writerGroupConfig,
                             &mImpl->m_writerGroupIdent);
    UA_Server_setWriterGroupOperational(mImpl->m_server, mImpl->m_writerGroupIdent);
    UA_UadpWriterGroupMessageDataType_delete(writerGroupMessage);
}

// * DataSetWriter
// * -------------
// * The DataSetWriters are the glue between the WriterGroups and the
// * PublishedDataSets. The DataSetWriter contain configuration parameters and
// * flags which influence the creation of DataSet messages. These messages are
// * encapsulated inside the network message. The DataSetWriter must be linked
// * with an existing PublishedDataSet and be contained within a WriterGroup. */
void Server::addDataSetWriter(void) {
    /* We need now a DataSetWriter within the WriterGroup. This means we must
     * create a new DataSetWriterConfig and call the addWriterGroup function. */
    //UA_NodeId m_dataSetWriterIdent;
    UA_DataSetWriterConfig dataSetWriterConfig;
    memset(&dataSetWriterConfig, 0, sizeof(UA_DataSetWriterConfig));
    dataSetWriterConfig.name = UA_STRING("Demo DataSetWriter");
    dataSetWriterConfig.dataSetWriterId = 62541;
    dataSetWriterConfig.keyFrameCount = 10;
    UA_Server_addDataSetWriter(mImpl->m_server, mImpl->m_writerGroupIdent,
                               mImpl->m_publishedDataSetIdent, &dataSetWriterConfig,
                               &mImpl->m_dataSetWriterIdent);
}

//==============================================================================
//===============INFORMATION MODELLING==========================================
//==============================================================================

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
    UA_Server_addVariableNode(mImpl->m_server, UA_NODEID_STRING(1, browseName),
                              folderId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, nodeDisplayName),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                              varNodeAttr, NULL, &variableNodeId);
}

void Server::addInt32VariableNode(UA_NodeId variableNodeId, UA_NodeId folderId,
                                    UA_Int32 dsInt32Val)
{
    UA_NodeId_init(&variableNodeId);
    UA_VariableAttributes varNodeAttr = UA_VariableAttributes_default;
    varNodeAttr.valueRank = -1;
    UA_NodeId_copy(&UA_TYPES[UA_TYPES_INT32].typeId, &varNodeAttr.dataType);
    varNodeAttr.accessLevel = UA_ACCESSLEVELMASK_READ ^ UA_ACCESSLEVELMASK_WRITE;
    UA_Variant_setScalar(&varNodeAttr.value, &dsInt32Val, &UA_TYPES[UA_TYPES_INT32]);
    varNodeAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Int32");
    UA_Server_addVariableNode(mImpl->m_server, UA_NODEID_STRING(1, "Publisher1.Int32"),
                              folderId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "Int32"),
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
    UA_Server_addVariableNode(mImpl->m_server, UA_NODEID_STRING(1, browseName),
                              folderId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, nodeDisplayName),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                              varNodeAttr, NULL, &variableNodeId);
}

void Server::addObjectNode(char *publisherName, UA_NodeId *folderId)
{
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", publisherName);
    UA_Server_addObjectNode(mImpl->m_server, UA_NODEID_NULL,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, publisherName), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            oAttr, NULL, folderId);
}

UA_StatusCode Server::addReaderGroup(void)
{
    UA_NodeId m_connectionIdentifier;
    UA_NodeId readerGroupIdentifier;
    if (mImpl->m_server == NULL)
        return UA_STATUSCODE_BADINTERNALERROR;

    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_ReaderGroupConfig readerGroupConfig;
    memset(&readerGroupConfig, 0, sizeof(UA_ReaderGroupConfig));
    readerGroupConfig.name = UA_STRING("ReaderGroup1");
    //readerGroupConfig.rtLevel = UA_PUBSUB_RT_FIXED_SIZE;
    retval |= UA_Server_addReaderGroup(mImpl->m_server, m_connectionIdentifier,
                                       &readerGroupConfig,
                                       &readerGroupIdentifier);
    return retval;
}

//==============================================================================
//=================Interface to Internals=======================================
//==============================================================================

UA_ServerConfig * Server::getUAServerConfig(void)
{
   return mImpl->m_config;
}
void Server::setCustomServerConfig(UA_ServerConfig* serverConfig)
{
    mImpl->m_config=serverConfig;
}
UA_Server *Server::getUAServer(void)
{
    return mImpl->m_server;
}

UA_StatusCode Server::getPubSubConnectionConfig(UA_PubSubConnectionConfig * config)
{
    return (UA_Server_getPubSubConnectionConfig(
        mImpl->m_server,
        mImpl->m_connectionIdent,
        config));
}

UA_NodeId Server::getConnectionIdent(void)
{
    return mImpl->m_connectionIdent;
}

UA_StatusCode Server::removePubSubConnection(void)
{
    return UA_Server_removePubSubConnection(mImpl->m_server,
                                    mImpl->m_connectionIdent);
}

UA_StatusCode Server::getDataSetWriterConfig(UA_DataSetWriterConfig *config)
{
    return UA_Server_getDataSetWriterConfig(mImpl->m_server,
                                            mImpl->m_dataSetWriterIdent, config);
}

UA_StatusCode Server::removeDataSetWriter(void)
{
    return UA_Server_removeDataSetWriter(mImpl->m_server,
                                         mImpl->m_dataSetWriterIdent);
}
}// namespace openPubSub
