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
        UA_Server *p_server;
        UA_ServerConfig *p_config;
        UA_Boolean m_running;

        explicit Server();
        template<class T>
        void addPubSubTransportLayer(T TransportLayer);
        void run();
        void stopServer();
        ~Server();
        void addPubSubConnection();
        void addPublishedDataSet();
        void addDataSetField();
        void addWriterGroup();
        void addDataSetWriter();

    private:
        UA_NodeId m_connectionID;
        UA_NodeId m_publishedDataSetID;
        UA_NodeId m_dataSetFieldID;
        UA_NodeId m_dataSetWriterID;
        UA_NodeId m_writerGroupID;

        UA_WriterGroupConfig m_writerGroupConfig;
        UA_DataSetWriterConfig m_dataSetWriterConfig;
        //m_p for member pointer
    };

    void init(Server &server);
}

#endif // OPENPUBSUB_H
