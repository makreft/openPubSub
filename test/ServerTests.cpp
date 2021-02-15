#include <gtest/gtest.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include "../openPubSub/openPubSub.h"


TEST(openPubSubTests, checkIfServerIsRunning)
{
    openPubSub::Server server;
    openPubSub::init(server);
    EXPECT_TRUE(server.isRunning());
}

TEST(openPubSubTests, checkIfServerIsConfigured)
{
    openPubSub::Server server;
    openPubSub::init(server);
    UA_ServerConfig *a = NULL;
    EXPECT_NE(a, server.getUAServerConfig());
}

TEST(openPubSubTests, checkIfServerIsConfiguredWithDefault)
{
    // if UA_ServerConfig_setDefault(p_config) is called,
    // endpointsSize must be greater than 0.
    openPubSub::Server server;
    openPubSub::init(server);
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
    openPubSub::Server server;
    openPubSub::init(server);
    server.stopServer();
    EXPECT_FALSE(server.isRunning());
}

TEST(openPubSubTests, checkServerPubSubConnection)
{
    UA_PubSubConnectionConfig pubsubconconf;
    openPubSub::Server server;
    openPubSub::init(server);
    server.addPubSubConnection("opc.udp://224.0.0.22:4840/", "UADP Connection 1");
    EXPECT_EQ(0, server.getPubSubConnectionConfig(&pubsubconconf));
    EXPECT_EQ(0, server.removePubSubConnection());
}
TEST(openPubSubTests, checkServerPublishedDataSet)
{
    UA_DataSetWriterConfig config;
    openPubSub::Server server;
    openPubSub::init(server);
    server.addPubSubConnection("opc.udp://224.0.0.22:4840/", "UADP Connection 1");
    server.addPublishedDataSet("Default PDS");
    server.addDataSetField("Server localtime");
    server.addWriterGroup("some");
    server.addDataSetWriter("someelse");
    UA_StatusCode code =server.getDataSetWriterConfig(&config);
    EXPECT_EQ(0, code);
    EXPECT_EQ(0, server.removeDataSetWriter());
}
