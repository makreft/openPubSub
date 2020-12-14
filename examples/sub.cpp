//
// Created by sba on 16.09.19.
//

/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */

/**
 * IMPORTANT ANNOUNCEMENT
 * The PubSub subscriber API is currently not finished. This examples can be used to receive
 * and print the values, which are published by the tutorial_pubsub_publish example.
 * The following code uses internal API which will be later replaced by the higher-level
 * PubSub subscriber API.
 */

#include <open62541/plugin/log_stdout.h>
#include <open62541/plugin/pubsub.h>
#include <open62541/plugin/pubsub_udp.h>

#include <open62541/server.h>
#include <open62541/server_config_default.h>

//#include "openPubSub/ua_pubsub_networkmessage.h"

#include <signal.h>
#include <open62541/types.h>

UA_Boolean running = true;
static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                "received ctrl-c");
    running = false;
}

static UA_StatusCode
subscriberListen(UA_PubSubChannel *psc) {
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
         * length to zero. */
        buffer.length = 512;
        UA_ByteString_clear(&buffer);
        return UA_STATUSCODE_GOOD;
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_PubSubTransportLayer udpLayer = UA_PubSubTransportLayerUDPMP();

    UA_PubSubConnectionConfig connectionConfig;
    memset(&connectionConfig, 0, sizeof(connectionConfig));
    connectionConfig.name = UA_STRING("UADP Connection 1");
    connectionConfig.transportProfileUri =
            UA_STRING("http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp");
    connectionConfig.enabled = UA_TRUE;

    UA_NetworkAddressUrlDataType networkAddressUrl =
            {UA_STRING_NULL , UA_STRING("opc.udp://127.0.0.1:4840/")};
    UA_Variant_setScalar(&connectionConfig.address, &networkAddressUrl,
                         &UA_TYPES[UA_TYPES_NETWORKADDRESSURLDATATYPE]);

    UA_PubSubChannel *psc =
            udpLayer.createPubSubChannel(&connectionConfig);
    psc->regist(psc, NULL, NULL);

    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    while(running && retval == UA_STATUSCODE_GOOD)
        retval = subscriberListen(psc);

    psc->close(psc);

    return 0;
}