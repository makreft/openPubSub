#include <gtest/gtest.h>
#include "../openPubSub/client.h"

openPubSub::Client client;

TEST(openPubSubTests, checkClientIsRunning)
{
    EXPECT_TRUE(client.isRunning());
}
TEST(openPubSubTests, checkIfClientShutsDown)
{
    client.stopClient();
}