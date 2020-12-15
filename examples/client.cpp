#include "../openPubSub/subscriber.cpp"
#include <iostream>

int main()
{
    openPubSub::Client client;
    openPubSub::init(client);
    if(client.isRunning())
        client.configPSConnection();
    std::cout << "debuggggeing \n";
}