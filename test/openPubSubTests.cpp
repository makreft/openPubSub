#include <gtest/gtest.h>
#include "../include/openPubSub.h"
#include <thread>
#include <open62541/types.h>

openPubSub::Server server;

TEST(openPubSubTests, checkIfServerIsRunning)
{
    EXPECT_TRUE(server.m_running);
}

TEST(openPubSubTests, checkIfServerIsConfigured)
{
    UA_ServerConfig *a = NULL;
    EXPECT_NE(a, server.p_config);
}

TEST(openPubSubTests, checkIfServerIsConfiguredWithDefault)
{
    //if UA_ServerConfig_setDefault(p_config) is called,
    // endpointsSize must be greater than 0.
    EXPECT_TRUE(server.p_config->endpointsSize > 0);
}

//TEST(openPubSubTests, checkIfServerHasPubSubTransportLayer)
//{
//
//
//}