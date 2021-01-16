#include "../openPubSub/server.h"
#include "../openPubSub/ua_pubsub/ua_pubsub.h"
#include <vector>

int main()
{
    openPubSub::string teststr = "test";
    openPubSub::Server serv("UDP");
    openPubSub::initServer(serv);
    //obs::init<obs::Server> server;
    std::vector<openPubSub::Server> test;

    serv.addPubSubConnection("my PubSub Connection");
    serv.addPublishedDataSet("My PDS");
    serv.addDataSetField("test");
    serv.addWriterGroup("test1");
    serv.addWriterGroup("test1");
    serv.addWriterGroup("test1");
    serv.addDataSetWriter("test2");
    serv.addDataSetWriter("test2");
    serv.addPubSubConnection("my other connection");
    serv.addPublishedDataSet("My other PDS");
    serv.addDataSetField("My DSF");
    //====================
    //cyclisch bind data to topic
    //serv.publish("mytopic", nodePub);
    ////=====
    //serv.subscribe("mytopic", nodeSub)
    serv.run();
}
