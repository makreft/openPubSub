#include "include/openPubSub.h"

int main()
{
    openPubSub::Server serv;
    openPubSub::init(serv);
    serv.run();
}