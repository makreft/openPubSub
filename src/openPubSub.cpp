#include "include/openPubSub.h"

namespace openPubSub
{
     Server::Server()
    {
        p_server = UA_Server_new();
        p_config = UA_Server_getConfig(p_server);
        UA_ServerConfig_setDefault(p_config);
        p_config->pubsubTransportLayers = (UA_PubSubTransportLayer *)
            UA_malloc(sizeof(UA_PubSubTransportLayer));
        p_config -> pubsubTransportLayers[0] = UA_PubSubTransportLayerUDPMP();
        p_config -> pubsubTransportLayersSize++;

        addPubSubConnection();
        addPublishedDataSet();
        addDataSetField();
        addWriterGroup();
        addDataSetWriter();
    }
    Server::~Server()
    {
        //UA_Server_delete(p_server);
    }

    void Server::addPubSubConnection()
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
        UA_Server_addPubSubConnection(p_server, &connectionConfig,
                                      &m_connectionID);
    }

    void Server::addPublishedDataSet() {
        UA_PublishedDataSetConfig publishedDataSetConfig;
        memset(&publishedDataSetConfig, 0, sizeof(UA_PublishedDataSetConfig));
        publishedDataSetConfig.publishedDataSetType =
            UA_PUBSUB_DATASET_PUBLISHEDITEMS;
        publishedDataSetConfig.name = UA_STRING("Demo PDS");
        UA_Server_addPublishedDataSet(p_server, &publishedDataSetConfig,
                                      &m_publishedDataSetID);
    }

    void Server::addDataSetField()
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
        UA_Server_addDataSetField(p_server, m_publishedDataSetID,
                                  &dataSetFieldConfig,
                                  &m_dataSetFieldID);
    }

    void Server::addWriterGroup()
    {
        memset(&m_writerGroupConfig, 0, sizeof(UA_WriterGroupConfig));
        m_writerGroupConfig.name = UA_STRING("Demo WriterGroup");
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
        UA_Server_addWriterGroup(p_server, m_connectionID,
                                 &m_writerGroupConfig, &m_writerGroupID);
        UA_Server_setWriterGroupOperational(p_server, m_writerGroupID);
        UA_UadpWriterGroupMessageDataType_delete(writerGroupMessage);
    }

    void Server::addDataSetWriter()
    {
        memset(&m_dataSetWriterConfig, 0, sizeof(UA_DataSetWriterConfig));
        m_dataSetWriterConfig.name = UA_STRING("Demo DataSetWriter");
        m_dataSetWriterConfig.dataSetWriterId = 62541;
        m_dataSetWriterConfig.keyFrameCount = 10;
        UA_Server_addDataSetWriter(p_server, m_writerGroupID, m_publishedDataSetID,
                                   &m_dataSetWriterConfig, &m_dataSetWriterID);
    }
}

