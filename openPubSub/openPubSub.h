#ifndef OPENPUBSUB_H
#define OPENPUBSUB_H

// open62541
#include <open62541/plugin/log.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/plugin/pubsub.h>
#include <open62541/plugin/pubsub_udp.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <open62541/server_pubsub.h>

// C standard lib
#include <signal.h>
#include <stdio.h>

// C++ standard lib
#include <cstring>
#include <memory>
#include <vector>
#include <exception>

// ops
#include "util/util.h"

// ops = openPubSub?
namespace openPubSub
{
class Server
{
private:
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
