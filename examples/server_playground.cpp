#include "../openPubSub/openPubSub.h"
#include "../openPubSub/ua_pubsub/ua_pubsub.h"
#include <open62541/server_config_default.h>
#include <time.h>


int main()
{

    openPubSub::Server serv;
    openPubSub::init(serv);

    UA_NodeId writerGroupId, pubsubConnectionId, publishedDSId;
    UA_NetworkAddressUrlDataType networkAddressUrl =
        {UA_STRING_NULL , UA_STRING("opc.udp://224.0.0.22:4840/")};

    UA_NodeId publishThisVar = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVERSTATUSTYPE_CURRENTTIME);
    serv.addPubSubConnection(&networkAddressUrl, "UADP Connection 1", 2234);
    serv.addPublishedDataSet( "Default PDS");
    serv.addDataSetField("Server localtime");
    serv.addWriterGroup();
    serv.addDataSetWriter();
    serv.run();
}

