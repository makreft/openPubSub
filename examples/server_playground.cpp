#include "../openPubSub/openPubSub.h"
#include "../openPubSub/ua_pubsub/ua_pubsub.h"
#include <open62541/server_config_default.h>

UA_UInt32 *valueStore[1];
static int callback()
{
    return 100;
}

static void valueUpdateCallback(UA_Server *server, void *data)
{
    u_int32_t sensorVal;
    for (int i = 0; i < 1; ++i)
    {
        sensorVal = callback();
        //std::cout << "Inside valueUpdateCallback, sensorVal: " << sensorVal << "\n";
        if (sensorVal == -1)
            printf("Issue with sensor.");
        *valueStore[i] = sensorVal;
    }
}

int main()
{

    openPubSub::Server serv;
    openPubSub::init(serv);

    serv.addPubSubConnection("opc.udp://224.0.0.22:4840/", "UADP Connection 1");
    serv.addPublishedDataSet("Server Playground PDS");
    serv.addWriterGroup("Server Playground WriterGroup");
    serv.addDataSetWriter("Server Playground DataSetWriter");
    serv.addDataSetField("Server Playground DataSetField");
    UA_Server* ua_server = serv.getUAServer();
    UA_UInt64 callbackId;
    printf("here");
    UA_Server_addRepeatedCallback(ua_server, valueUpdateCallback, NULL, 100,&callbackId);

    serv.run();
}

