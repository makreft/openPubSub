#include "subscriber.h"
namespace openPubSub
{

    Client *_client;
    UA_Boolean _running;
    static void stopHandler()
    {
        _client->stopClient();
    }
    Client::Client()
    {
        _running = UA_TRUE;
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
    void init(Client &client)
    {
        _client = &client;
        signal(SIGINT, reinterpret_cast<__sighandler_t>(stopHandler));
        signal(SIGTERM, reinterpret_cast<__sighandler_t>(stopHandler));
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


}

