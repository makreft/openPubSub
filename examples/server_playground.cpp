#include "../openPubSub/openPubSub.h"
#include "../openPubSub/ua_pubsub/ua_pubsub.h"
#include <vector>

int main()
{
    openPubSub::Server serv;
    openPubSub::initServer(serv);
    //obs::init<obs::Server> server;
    int pubsubID1 = 888;
    int pubsubID2 = 999;
    UA_ServerConfig* conf = serv.getUAServerConfig();

    serv.addPubSubConnection("my PubSub Connection", pubsubID1);
    serv.addPublishedDataSet("My PDS");
    serv.addDataSetField("test");
    serv.addWriterGroup("test1", 100, 6);
    serv.addDataSetWriter("test2");
    serv.addPubSubConnection("my other connection", pubsubID2);
    serv.addPublishedDataSet("My other PDS");
    serv.addDataSetField("My DSF");
    //====================
    //cyclisch bind data to topic
    //serv.publish("mytopic", nodePub);
    ////=====
    //serv.subscribe("mytopic", nodeSub)
    serv.run();
}
