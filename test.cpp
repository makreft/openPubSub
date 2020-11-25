#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <open62541/plugin/pubsub.h>
#include <open62541/plugin/pubsub_udp.h>
#include <open62541/plugin/log.h>
#include <open62541/plugin/log_stdout.h>
#include <signal.h>
#include <stdio.h>
#include <open62541/server_pubsub.h>
#include <open62541/types_generated.h>
#include <open62541/types.h>

UA_Boolean running = true;


static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                "received ctrl-c");
    running = false;
}

static void addPubSubConnection(UA_Server *server, UA_NodeId *connectionID)
{
    UA_PubSubConnectionConfig connectionConfig;
    memset(&connectionConfig, 0, sizeof(connectionConfig));
    connectionConfig.name = UA_STRING("UADP Connection 1");
    connectionConfig.transportProfileUri = UA_STRING(
            "http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp");
    UA_NetworkAddressUrlDataType networkAddressUrl =
        {UA_STRING_NULL , UA_STRING("opc.udp://127.0.0.1:4840/")};
    connectionConfig.enabled = UA_TRUE;
    UA_Variant_setScalar(&connectionConfig.address, &networkAddressUrl,
            &UA_TYPES[UA_TYPES_NETWORKADDRESSURLDATATYPE]);
    connectionConfig.publisherId.numeric = 2234;
    UA_Server_addPubSubConnection(server, &connectionConfig, connectionID);
}

static void addPublishedDataSet(UA_Server *server, UA_NodeId * pdsID )
{
    UA_PublishedDataSetConfig publishedDataSetConfig;
    memset(&publishedDataSetConfig, 0, sizeof(UA_PublishedDataSetConfig));
    publishedDataSetConfig.publishedDataSetType =
            UA_PUBSUB_DATASET_PUBLISHEDITEMS;
    publishedDataSetConfig.name = UA_STRING("Demo PDS");
    UA_Server_addPublishedDataSet(server, &publishedDataSetConfig, pdsID);
}
static void addDataSetField(UA_Server * server, UA_NodeId *pdsID,
                            UA_NodeId *dsfID)
{
    UA_DataSetFieldConfig dataSetFieldConfig;
    memset(&dataSetFieldConfig, 0, sizeof(UA_DataSetFieldConfig));
    dataSetFieldConfig.dataSetFieldType = UA_PUBSUB_DATASETFIELD_VARIABLE;
    dataSetFieldConfig.field.variable.fieldNameAlias =
            UA_STRING("Server localtime");
    dataSetFieldConfig.field.variable.promotedField = UA_FALSE;
    dataSetFieldConfig.field.variable.publishParameters.publishedVariable =
            UA_NODEID_NUMERIC(0, UA_NS0ID_SERVERSTATUSTYPE_CURRENTTIME);
    dataSetFieldConfig.field.variable.publishParameters.attributeId =
            UA_ATTRIBUTEID_VALUE;
    UA_Server_addDataSetField(server, *pdsID, &dataSetFieldConfig, dsfID);
}

static void addWriterGroup(UA_Server *server, UA_NodeId *connectionID,
                           UA_NodeId *writerGroupID)
{
    UA_WriterGroupConfig writerGroupConfig;
    memset(&writerGroupConfig, 0, sizeof(UA_WriterGroupConfig));
    writerGroupConfig.name = UA_STRING("Demo WriterGroup");
    writerGroupConfig.publishingInterval = 100;
    writerGroupConfig.enabled = UA_FALSE;
    writerGroupConfig.writerGroupId = 100;
    writerGroupConfig.encodingMimeType = UA_PUBSUB_ENCODING_UADP;
    writerGroupConfig.messageSettings.encoding = UA_EXTENSIONOBJECT_DECODED;
    writerGroupConfig.messageSettings.content.decoded.type =
            &UA_TYPES[UA_TYPES_UADPWRITERGROUPMESSAGEDATATYPE];
    UA_UadpWriterGroupMessageDataType *writerGroupMessage =
            UA_UadpWriterGroupMessageDataType_new();
    writerGroupMessage->networkMessageContentMask =
            ( UA_UadpNetworkMessageContentMask )( UA_UADPNETWORKMESSAGECONTENTMASK_PUBLISHERID   |
            ( UA_UadpNetworkMessageContentMask )  UA_UADPNETWORKMESSAGECONTENTMASK_GROUPHEADER   |
            ( UA_UadpNetworkMessageContentMask )  UA_UADPNETWORKMESSAGECONTENTMASK_WRITERGROUPID |
            ( UA_UadpNetworkMessageContentMask )  UA_UADPNETWORKMESSAGECONTENTMASK_PAYLOADHEADER );
    writerGroupConfig.messageSettings.content.decoded.data = writerGroupMessage;
    UA_Server_addWriterGroup(server, *connectionID, &writerGroupConfig,
                             writerGroupID);
    UA_Server_setWriterGroupOperational(server, *writerGroupID);
    UA_UadpWriterGroupMessageDataType_delete(writerGroupMessage);
}
static void addDataSetWriter(UA_Server *server, UA_NodeId *writerGroupID,
                             UA_NodeId *publishedDataSetID,
                             UA_NodeId *dataSetWriterID)
{
    UA_DataSetWriterConfig dataSetWriterConfig;
    memset(&dataSetWriterConfig, 0, sizeof(UA_DataSetWriterConfig));
    dataSetWriterConfig.name = UA_STRING("Demo DataSetWriter");
    dataSetWriterConfig.dataSetWriterId = 62541;
    dataSetWriterConfig.keyFrameCount = 10;
    UA_Server_addDataSetWriter(server, *writerGroupID, *publishedDataSetID,
                               &dataSetWriterConfig, dataSetWriterID);
}
static void addVariable(UA_Server *server)
{
    /* Define the attribute of the myInteger variable node */
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Int32 myInteger = 42;
    UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
    attr.description = UA_LOCALIZEDTEXT("en-US","the answer");
    attr.displayName = UA_LOCALIZEDTEXT("en-US","the answer");
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    /* Add the variable node to the information model */
    UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "the answer");
    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, "the answer");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_Server_addVariableNode(server, myIntegerNodeId, parentNodeId,
                          parentReferenceNodeId, myIntegerName,
                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                          attr, NULL, NULL);
}

int main()
{
    UA_NodeId connectionID;
    UA_NodeId publishedDataSetID;
    UA_NodeId dataSetFieldID;
    UA_NodeId dataSetWriterID;
    UA_NodeId writerGroupID;

    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);


    UA_Server *server = UA_Server_new();
    UA_ServerConfig *config = UA_Server_getConfig(server);
    UA_ServerConfig_setDefault(config);

    config->pubsubTransportLayers = (UA_PubSubTransportLayer *)
        UA_malloc(sizeof(UA_PubSubTransportLayer));
    config->pubsubTransportLayers[0] = UA_PubSubTransportLayerUDPMP();
    config->pubsubTransportLayersSize++;

    addPubSubConnection(server, &connectionID);
    addPublishedDataSet(server, &publishedDataSetID);
    addDataSetField(server, &publishedDataSetID, &dataSetFieldID);
    addWriterGroup(server, &connectionID, &writerGroupID);
    addDataSetWriter(server, &writerGroupID, &publishedDataSetID, &dataSetFieldID);
    addVariable(server);

    UA_StatusCode retVal;
    retVal = UA_Server_run(server, &running);
    UA_Server_delete(server);
    return (int) retVal;
}
