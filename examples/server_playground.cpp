#include "../openPubSub/openPubSub.h"
#include "../openPubSub/ua_pubsub/ua_pubsub.h"

int main()
{

    openPubSub::Server serv;
    openPubSub::init(serv);


    serv.addPubSubConnection("opc.udp://224.0.0.22:4840/", "UADP Connection 1");
    serv.addPublishedDataSet("Server Playground PDS");
    serv.addWriterGroup("Server Playground WriterGroup");
    serv.addDataSetWriter("Server Playground DataSetWriter");
    serv.addDataSetField("Server Playground DataSetField");
    serv.freezeWriterGroupConfiguration();
    //serv.addRepeatedCallback();
    serv.run();
}

