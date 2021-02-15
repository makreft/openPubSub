#ifndef OPENPUBSUB_H
#define OPENPUBSUB_H

// open62541
#include <open62541/server_pubsub.h>

// C standard lib

// C++ standard lib
#include <memory>

// ops
#include "util/util.h"


namespace openPubSub
{
class Server
{
private:
    /// @Impl: only for implementation purposes High-Level usage is provided by
    /// getter and setter functionality
    struct Impl;
    std::unique_ptr<Impl> mImpl;

public:
    /// The standard constructor will create everything that is UA_default
    /// for custom handling, interface functions are provided (get* or set*).
    Server(void);
    ~Server(void);
    void stopServer(void);
    int m_pubSubConfigPublishCycleMS = 100;
    int m_pubSubConfigFieldCount = 1;
    UA_UInt32 * m_valueStore[1];

    //==========================================================================
    //============Connection Handling===========================================
    //==========================================================================
    ///The server that creates the PubSub Connection with addPubSubConnection
    ///is the Host --> Publisher of the data.
    ///@param networkAddressUrl:
    ////usually the default is multicast: opc.udp://224.0.0.22:4840/
    void addPubSubConnection(char *networkAddressUrl,
                             char *connectionName);
    /// The PublishedDataSet (PDS) and PubSubConnection are the toplevel entities and
    /// can exist alone. The PDS contains the collection of the published fields. All
    /// other PubSub elements are directly or indirectly linked with the PDS or
    /// connection. Returns the publishedDataSetId, that is needed for adding DS fields
    /// to the PublishedDataSet.
    void addPublishedDataSet(char *pdsName);
    /// The DataSetField (DSF) is part of the PDS and describes exactly one published
    /// field. all fields are added to the beginning of the list.

    /// Add one DataSetField with static value source to PDS
    void addDataSetField(char *fieldName);
    void addInt32DataSetField(UA_NodeId publishedVariable);
    void addWriterGroup(char* writerGroupName);
    /// A DataSetWriter (DSW) is the glue between the WG and the PDS. The DSW is
    /// linked to exactly one PDS and contains additional information for the
    /// message generation.
    void addDataSetWriter(char * dataSetWriterName);
    /// before calling the freeze method the writerGroup can be updated e.g. with
    /// a different publishing interval.
    void updatePublishingInterval(int publishingInterval);
    /// freeze: don't allow changes to writerGroup after call
    void freezeWriterGroupConfiguration();
    UA_StatusCode addRepeatedCallback(UA_ServerCallback callback);
    void run(void);
    bool isRunning(void);

    //==========================================================================
    //=================InformationSpace modelling===============================
    //==========================================================================

    /// @param UA_DataType: This interface has been implemented for some
    /// UA_DataTypes, should be more readable with a template for the future.
    void addVariableNode(UA_NodeId variableNodeId, const UA_NodeId folderId,
                         char *nodeDisplayName, char *browseName,
                         UA_Boolean UA_DataType, bool setAccessLevel);
    void addInt32VariableNode(UA_NodeId variableNodeId, UA_NodeId folderId,
                                        UA_Int32 dsInt32Val);
    void addVariableNode(UA_NodeId variableNodeId, const UA_NodeId folderId,
                         char *nodeDisplayName, char *browseName,
                         UA_DateTime UA_DataType, bool setAccessLevelMask);
    void addObjectNode(char * publisherName, UA_NodeId *folderId);
    UA_StatusCode addReaderGroup(void);

    //==========================================================================
    //================Interface to Internals====================================
    //==========================================================================

    UA_ServerConfig * getUAServerConfig(void);
    UA_Server * getUAServer(void);
    void setCustomServerConfig(UA_ServerConfig *serverConfig);
    UA_NodeId getConnectionIdent(void);
    /// check connection, 0 if successful
    ///@param config: the PSCConfig will be copied to this value
    UA_StatusCode getPubSubConnectionConfig(UA_PubSubConnectionConfig * config);
    /// remove connection, 0 if successful
    UA_StatusCode removePubSubConnection(void);
    ///@param config: the DSWConfig will be copied to this value
    UA_StatusCode getDataSetWriterConfig(UA_DataSetWriterConfig * config);

    /// removeDataSetWriter
    UA_StatusCode removeDataSetWriter(void);
};
    ///initializes the signal stuff
    void init(Server &server);
}
#endif // OPENPUBSUB_H
