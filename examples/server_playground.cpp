#include "../openPubSub/server.h"


int main()
{
    openPubSub::string test = "test";
    openPubSub::Server serv("UDP");
    serv.init();
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
