#include "../openPubSub/openPubSub.h"
#include <iostream>

int main()
{
    openPubSub::Server server;
    openPubSub::init(server);
    server.addReaderGroup();
    //UA_DataSet server.subscribe("topic");
    server.run();
    std::cout << "test\n";
}