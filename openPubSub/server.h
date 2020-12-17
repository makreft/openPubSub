#ifndef OPENPUBSUB_SERVER_H
#define OPENPUBSUB_SERVER_H

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

// openPubSub
#include "util.h"

namespace openPubSub
{
    /// The Server expects that all configurations are done during initialization.
    /// As soon as the run() method is called, the server cannot be modified.
    /// Steps for configuration (taken from the open62541 documentation):
    /// 1. Create a Server configuration with default settings as starting point
    /// 2. Modify the configuration, e.g. by adding a server certificate.
    /// 3. Instantiate a server with it.
    /// 4. After shutdown of the server, clean up the configuration (free memory)
    class Server
    {
    private:

        UA_NodeId m_connectionID;
        UA_NodeId m_publishedDataSetID;
        UA_NodeId m_dataSetFieldID;
        UA_NodeId m_dataSetWriterID;
        UA_NodeId m_writerGroupID;

        UA_WriterGroupConfig m_writerGroupConfig;
        UA_DataSetWriterConfig m_dataSetWriterConfig;

        std::string m_transportUri;
        std::string m_networkUrl;
    public:
        // should mp_server and mp_config be private?
        /// Member pointer of the server.
        UA_Server *mp_server;
        /// Member pointer of the server config.
        UA_ServerConfig *mp_config;
        /// @param transportLayer
        /// define what kind of transport layer is
        /// supposed to be used by the server. Currently only UDP over UADP is
        /// supported but MQTT should be a better choice and thus will be
        /// implemented in a future version.
        explicit Server(string transportLayer="UDP");
        /// calls UA_Server_delete and frees the memory allocated on the heap
        /// for m_transportUri, m_networkUrl.
        ~Server();
        void stopServer();
        void addPubSubConnection(string nameOfPubSubConnection);
        /// @param nameOfPublishedDS
        /// Name of the Published DataSet.
        void addPublishedDataSet(string nameOfPublishedDS);
        void addDataSetField(string nameOfDSField);
        void addWriterGroup(string nameOfWriterGroup);
        void addDataSetWriter(string nameOfDSWriter);
        /// @param transportLayer
        /// define what kind of transport layer is
        /// supposed to be used by the server
        /// @param T
        /// The method addPubSubTransportLayer is supposed to be used only
        /// when constructing the openPubSub::Server object
        template<class T>
        void addPubSubTransportLayer(const T transportLayer)
        {
            mp_config->pubsubTransportLayers = \
                    (UA_PubSubTransportLayer *)UA_malloc(sizeof(UA_PubSubTransportLayer));
            mp_config -> pubsubTransportLayers[0] = transportLayer;
            mp_config -> pubsubTransportLayersSize++;
        }
        /// @param transportProfileUri the passed std::string will be
        /// converted to char * on the heap. Memory of m_ will be
        /// freed in the Server destructor.
        void setTransportProfileUri(const std::string &transportProfileUri = \
                "http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp");

        /// @param networkAddressUrl the passed std::string will be
        /// converted to char * on the heap. Memory of m_networkUrl will be
        /// freed in the Server destructor.
        void setNetworkAddressUrl(const std::string &networkAddressUrl = \
                "opc.udp://127.0.0.1:4840/");

        /// Call run() after everything is configured.
        void run();
        bool isRunning();
    };
    void init(Server &server);
}

#endif // OPENPUBSUB_SERVER_H
