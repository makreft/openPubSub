#ifndef OPENPUBSUB_H
#define OPENPUBSUB_H

// open62541
#include <open62541/server_pubsub.h>

// C standard lib

// C++ standard lib
#include <memory>

// ops
#include "util/util.h"

// ops = openPubSub?
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
    Server();
    ~Server();
    /// @param UA_DataType: This interface has been implemented for some
    /// UA_DataTypes, should be more readable with a template for the future.
    void addVariableNode(UA_NodeId variableNodeId, const UA_NodeId folderId,
                         char *nodeDisplayName, char *browseName,
                         UA_Boolean UA_DataType, bool setAccessLevel);
    void addVariableNode(UA_NodeId variableNodeId, const UA_NodeId folderId,
                         char *nodeDisplayName, char *browseName,
                         UA_NodeId UA_DataType, bool setAccessLevel);
    void addVariableNode(UA_NodeId variableNodeId, const UA_NodeId folderId,
                         char *nodeDisplayName, char *browseName,
                         UA_DateTime UA_DataType, bool setAccessLevelMask);
    void addObjectNode(char * publisherName, UA_NodeId folderId);
    void stopServer();
    //TODO:
    //void writeDataToTopic(char *);
    //void timerCallback(UA_Server *server, void *data);
    UA_StatusCode addReaderGroup();
    ///The server that creates the PubSub Connection with addPubSubConnection
    ///is the Host --> Publisher of the data.
    void addPubSubConnection(UA_NetworkAddressUrlDataType* networkAddressUrl);
    /// The PublishedDataSet (PDS) and PubSubConnection are the toplevel entities and
    /// can exist alone. The PDS contains the collection of the published fields. All
    /// other PubSub elements are directly or indirectly linked with the PDS or
    /// connection. Returns the publishedDataSetId, that is needed for adding DS fields
    /// to the PublishedDataSet.
    void addPublishedDataSet();
    /// The DataSetField (DSF) is part of the PDS and describes exactly one published
    /// field. all fields are added to the beginning of the list.
    void addDataSetField();
    void addWriterGroup();
    /// A DataSetWriter (DSW) is the glue between the WG and the PDS. The DSW is
    /// linked to exactly one PDS and contains additional information for the
    /// message generation.
    void addDataSetWriter();
    void run();
    bool isRunning();
    UA_ServerConfig * getUAServerConfig();
    UA_Server * getUAServer();
    void setCustomServerConfig(UA_ServerConfig *serverConfig);

    void adddReaderGroup();
};
    void init(Server &server);
}
#endif // OPENPUBSUB_H
