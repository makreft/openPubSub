#include <gtest/gtest.h>
#include "../include/openPubSub.h"

TEST(openPubSubTests, makePubSubServerObject)
{
    openPubSub::Server serv;
    openPubSub::init(serv);
    serv.run();
}