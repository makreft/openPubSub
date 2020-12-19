#include <gtest/gtest.h>
#include "../openPubSub/server.h"

openPubSub::Server server("UDP");

TEST(openPubSubTests, checkIfServerIsRunning)
{
    EXPECT_TRUE(server.isRunning());
}

TEST(openPubSubTests, checkIfServerIsConfigured)
{
    UA_ServerConfig *a = NULL;
    EXPECT_NE(a, server.mp_config);
}

TEST(openPubSubTests, checkIfServerIsConfiguredWithDefault)
{
    // if UA_ServerConfig_setDefault(p_config) is called,
    // endpointsSize must be greater than 0.
    EXPECT_TRUE(server.mp_config->endpointsSize > 0);
}

TEST(openPubSubTests, checkIfServerShutsDown)
{
    server.stopServer();
    EXPECT_FALSE(server.isRunning());
}