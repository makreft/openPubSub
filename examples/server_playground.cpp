#include "../openPubSub/server.h"
#include <vector>

int main()
{
    openPubSub::string teststr = "test";
    openPubSub::Server serv("UDP");
    openPubSub::init(serv);
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
    serv.run();
}
