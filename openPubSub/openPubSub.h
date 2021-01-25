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
    /// PIMPL: only for implementation purposes High-Level usage is provided by
    /// getter and setter functionality
    struct Impl;
    std::unique_ptr<Impl> mImpl;

public:
    Server();
    ~Server();
    void stopServer();
    void addPubSubConnection(string nameOfPubSubConnection, int publisherID);
    void addPublishedDataSet(string nameOfPublishedDS);
    void addDataSetField(string nameOfDSField);
    void addWriterGroup(string nameOfWriterGroup, int publishingInterval,
                        int writerGroupId);
    void addDataSetWriter(string nameOfDSWriter);
    void addPubSubTransportLayer();
    void setTransportProfileUri(const std::string &transportProfileUri);
    void setNetworkAddressUrl(const std::string &networkAddressUrl);
    void run();
    bool isRunning();
    UA_ServerConfig * getUAServerConfig();
    UA_Server * getUAServer();
    void setCustomServerConfig(UA_ServerConfig *serverConfig);
};
    void initServer(Server &server);
}

#endif // OPENPUBSUB_H
