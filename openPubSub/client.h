#ifndef OPENPUBSUB_CLIENT_H
#define OPENPUBSUB_CLIENT_H

#include <open62541/client.h>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/plugin/pubsub.h>
#include <open62541/plugin/pubsub_ethernet.h>
#include <open62541/plugin/pubsub_udp.h>

// openPubSub
#include "util.h"

namespace openPubSub
{

class Client
{
private:

    UA_PubSubConnectionConfig mp_pubSubConfig;
    UA_NetworkAddressDataType mp_networkAddressURL;
    UA_PubSubChannel *mp_pubSubChannel;
    UA_Boolean _running;

public:

    // should mp_client and mp_config be private?
    UA_Client *mp_client;
    UA_ClientConfig mp_config;

    explicit Client();
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

    /* Provides default values for a new subscription.
     * RequestedPublishingInterval:  500.0 [ms]
     * RequestedLifetimeCount: 10000
     * RequestedMaxKeepAliveCount: 10
     * MaxNotificationsPerPublish: 0 (unlimited)
     * PublishingEnabled: true
     * Priority: 0 */
    static UA_INLINE UA_CreateSubscriptionRequest
    UA_CreateSubscriptionRequest_default(void) {
        UA_CreateSubscriptionRequest request;
        UA_CreateSubscriptionRequest_init(&request);

        request.requestedPublishingInterval = 500.0;
        request.requestedLifetimeCount = 10000;
        request.requestedMaxKeepAliveCount = 10;
        request.maxNotificationsPerPublish = 0;
        request.publishingEnabled = true;
        request.priority = 0;
        return request;
    }
    UA_CreateSubscriptionResponse
    UA_Client_Subscriptions_create(UA_Client *client,
                                   const UA_CreateSubscriptionRequest request,
                                   void *subscriptionContext,
                                   UA_Client_StatusChangeNotificationCallback statusChangeCallback,
                                   UA_Client_DeleteSubscriptionCallback deleteCallback);

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
#endif // OPENPUBSUB_CLIENT_H