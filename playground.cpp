#include "openPubSub/openPubSub.h"

int main()
{
    openPubSub::Server serv;
    openPubSub::init(serv);
    serv.addPubSubConnection("myPubSubConnection");
    serv.addPublishedDataSet("Demo PublishedDataSet");
    serv.addDataSetField("test");
    serv.addWriterGroup("test1");
    serv.addDataSetWriter("test2");
    serv.run();
}