#ifndef OPENPUBSUB_H
#define OPENPUBSUB_H
#if defined (__cplusplus)
extern "C" {
#endif
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <open62541/plugin/pubsub.h>
#include <open62541/plugin/pubsub_udp.h>
#include <open62541/plugin/log.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server_pubsub.h>
#include <open62541/types_generated.h>
#include <open62541/types.h>
#if defined (__cplusplus)
}
#endif
#include <signal.h>
#include <stdio.h>
// C++ standard lib
#include <string>
#include <boost/scoped_array.hpp>
#include <memory>
#include <cstring>
//using the Hungarian notation:
//"m_" means member variable
//"p_" means pointer

namespace openPubSub
{
    struct ua_exception : public std::exception
    {

        UA_StatusCode code;

        explicit ua_exception(UA_StatusCode returnCode)
        {
            code = returnCode;
        }

        const char * what() const throw() override
        {
            return UA_StatusCode_name(code);
        }
    };

    class Server
    {
    public:
        /// Member pointer of the server.
        UA_Server *mp_server;
        /// Member pointer of the server config.
        UA_ServerConfig *mp_config;
        /// Member pointer holding the state, if the server is running.
        UA_Boolean mp_running;

        /// @param transportLayer define what kind of transport layer is
        /// supposed to be used by the server. Currently only UDP over UADP is
        /// supported but MQTT should be a better choice and thus will be
        /// implemented in a future version.
        explicit Server(std::string transportLayer="UDP");
        /// calls UA_Server_delete and frees the memory allocated on the heap
        /// for m_transportUri, m_networkUrl.
        ~Server();
        void stopServer();
        void addPubSubConnection(std::string nameOfPubSubConnection);
        /// @param nameOfPublishedDS Name of the Published DataSet.
        void addPublishedDataSet(std::string nameOfPublishedDS);
        void addDataSetField(std::string nameOfDSField);
        void addWriterGroup(std::string nameOfWriterGroup);
        void addDataSetWriter(std::string nameOfDSWriter);

        /// @param transportLayer divine what kind of transport layer is
        /// supposed to be used by the server
        /// @param T
        /// The method addPubSubTransportLayer is supposed to be used only
        /// when constructing the openPubSub::Server object
        template<class T>
        void addPubSubTransportLayer(const T TransportLayer)
        {
            mp_config->pubsubTransportLayers = (UA_PubSubTransportLayer *) \
                                UA_malloc(sizeof(UA_PubSubTransportLayer));
            mp_config -> pubsubTransportLayers[0] = TransportLayer;
            mp_config -> pubsubTransportLayersSize++;
        }
        /// @param transportProfileUri the passed std::string will be
        /// converted to char * on the heap. Memory of m_ will be
        /// freed in the Server destructor.
        void setTransportProfileUri(std::string transportProfileUri = \
                "http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp");

        /// @param networkAddressUrl the passed std::string will be
        /// converted to char * on the heap. Memory of m_networkUrl will be
        /// freed in the Server destructor.
        void setNetworkAddressUrl(std::string networkAddressUrl = \
                                                   "opc.udp://127.0.0.1:4840/");

        /// Call run() at after everything is configured.
        void run();

    private:
        void setNameOfPubSubConnection(std::string name);
        void setNameOfPublishedDataSet(std::string name);
        UA_NodeId m_connectionID;
        UA_NodeId m_publishedDataSetID;
        UA_NodeId m_dataSetFieldID;
        UA_NodeId m_dataSetWriterID;
        UA_NodeId m_writerGroupID;

        UA_WriterGroupConfig m_writerGroupConfig;
        UA_DataSetWriterConfig m_dataSetWriterConfig;
        /// the transport uri can be set with setTransportProfileUri()
        std::string m_transportUri;
        /// the transport uri can be set with setNetworkAddressUrl()
        std::string m_networkUrl;
        std::string m_namePubSubConnection;
        std::string m_namePublishedDS;
    };

    void init(Server &server);
}

#endif // OPENPUBSUB_H