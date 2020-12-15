#include <gtest/gtest.h>
#include "../openPubSub/subscriber.h"

openPubSub::Client client;

TEST(openPubSubTests, checkClient)
{
EXPECT_TRUE(client.isRunning());
}
