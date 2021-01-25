#include <gtest/gtest.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include "../openPubSub/openPubSub.h"

openPubSub::Server server;

TEST(openPubSubTests, checkIfServerIsRunning)
{
    EXPECT_TRUE(server.isRunning());
}

TEST(openPubSubTests, checkIfServerIsConfigured)
{
    UA_ServerConfig *a = NULL;
    EXPECT_NE(a, server.getUAServerConfig());
}

TEST(openPubSubTests, checkIfServerIsConfiguredWithDefault)
{
    // if UA_ServerConfig_setDefault(p_config) is called,
    // endpointsSize must be greater than 0.
    UA_Server *ua_server = UA_Server_new();
    UA_ServerConfig *ua_conf = UA_Server_getConfig(ua_server);
    UA_ServerConfig_setDefault(ua_conf);
    EXPECT_EQ(*server.getUAServerConfig()->applicationDescription.applicationUri.data,
              *ua_conf->applicationDescription.applicationUri.data);
    EXPECT_EQ(*server.getUAServerConfig()->applicationDescription.productUri.data,
              *ua_conf->applicationDescription.productUri.data);
    EXPECT_EQ(*server.getUAServerConfig()->applicationDescription.applicationName.text.data,
              *ua_conf->applicationDescription.applicationName.text.data);
}

TEST(openPubSubTests, checkIfServerShutsDown)
{
    server.stopServer();
    EXPECT_FALSE(server.isRunning());
}