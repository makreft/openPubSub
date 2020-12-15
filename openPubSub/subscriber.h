#ifndef OPENPUBSUB_SUBSCRIBERTESTS_H
#define OPENPUBSUB_SUBSCRIBERTESTS_H
//#include <open62541/server.h>
#include "util.h"
#include <open62541/plugin/log_stdout.h>
#include <open62541/plugin/pubsub.h>
#include <open62541/plugin/pubsub_udp.h>
#include <open62541/client.h>

namespace openPubSub
{

class Client
{
private:

    UA_Client *mp_client;
    UA_ClientConfig mp_config;
    UA_PubSubConnectionConfig mp_pubSubConfig;
    UA_NetworkAddressDataType mp_networkAddressURL;
    UA_PubSubChannel *mp_pubSubChannel;

public:

    // should mp_client and mp_config be private?

    Client();
    ~Client();
    bool isRunning();

    void stopClient();


    void configPSConnection(const string &nameOfConnection = "0815-Connection",
                            const string &url ="opc.udp://127.0.0.1:4840/")
    {
        memset(&mp_pubSubConfig, 0, sizeof(mp_pubSubConfig));
        mp_pubSubConfig.enabled = UA_TRUE;
        mp_pubSubConfig.name = *nameOfConnection.value;
        // TODO: make transportProfileUri more dynamically
        mp_pubSubConfig.transportProfileUri = \
                (*string("http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp").value);
        mp_networkAddressURL = \
                {url.value->length, url.value->data};
        UA_Variant_setScalar(&mp_pubSubConfig.address, &mp_networkAddressURL, \
                             &UA_TYPES[UA_TYPES_NETWORKADDRESSDATATYPE]);
        addPubSubTransportLayer(UA_PubSubTransportLayerUDPMP());
    }
    void addPubSubTransportLayer(UA_PubSubTransportLayer transportLayer)
    {
        mp_pubSubChannel = transportLayer.createPubSubChannel(&mp_pubSubConfig);
        mp_pubSubChannel->regist(mp_pubSubChannel, NULL, NULL);
    }


    static UA_StatusCode subscriberListens(UA_PubSubChannel *psc) {
        UA_ByteString buffer;
        UA_StatusCode retval = UA_ByteString_allocBuffer(&buffer, 512);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                         "Message buffer allocation failed!");
            return retval;
        }
        /* Receive the message. Blocks for 100ms */
        retval = psc->receive(psc, &buffer, NULL, 100);
        if(retval != UA_STATUSCODE_GOOD || buffer.length == 0) {
            /* Workaround!! Reset buffer length. Receive can set the length to zero.
             * Then the buffer is not deleted because no memory allocation is
             * assumed.
             * TODO: Return an error code in 'receive' instead of setting the buf
             * length to zero.*/
            buffer.length = 512;
            UA_ByteString_clear(&buffer);
            return UA_STATUSCODE_GOOD;
        }
    }

    void setTransportLayer();
};

}



#endif //OPENPUBSUB_SUBSCRIBERTESTS_H
