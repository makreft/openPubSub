#include "client.h"

namespace openPubSub
{
    Client::Client()
    {
        //_running = UA_TRUE;
        mp_client = UA_Client_new();
        UA_ClientConfig_setDefault(UA_Client_getConfig(mp_client));
    }
    Client::~Client()
    {
        UA_Client_delete(mp_client);
    }

    void Client::stopClient()
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
        _running = false;
    }


    bool Client::isRunning()
    {
        if (_running)
            return true;
        else
            return false;
    }

    void Client::setTransportLayer()
    {


    }

    UA_CreateSubscriptionResponse
    Client::UA_Client_Subscriptions_create(UA_Client *client,
                                           const UA_CreateSubscriptionRequest request,
                                           void *subscriptionContext,
                                           UA_Client_StatusChangeNotificationCallback statusChangeCallback,
                                           UA_Client_DeleteSubscriptionCallback deleteCallback) {
        return UA_CreateSubscriptionResponse();
    }
}

