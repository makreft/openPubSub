#ifndef OPS_SUBSCRIPTION_H
#define OPS_SUBSCRIPTION_H


#include <open62541/plugin/log_stdout.h>
#include <open62541/plugin/pubsub.h>
#include <open62541/plugin/pubsub_udp.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include "ua_pubsub/ua_pubsub_networkmessage.h"

#include <signal.h>
#ifdef UA_ENABLE_PUBSUB_ETH_UADP
#include <open62541/plugin/pubsub_ethernet.h>
#endif
#include <memory>
#include <experimental/propagate_const>


    class Subscriber
    {
    private:
        /// @Impl: only for implementation purposes High-Level usage is provided by
        /// getter and setter functionality
        struct Impl;
        std::unique_ptr<Impl> mImpl;
        UA_StatusCode allocCustomBuffer(int bufferSize);

    public:
        Subscriber();
        ~Subscriber();
        void stopSubscription();
        UA_StatusCode listen();
        void run();

        void stopSubcription();
    };



#endif //OPS_SUBSCRIPTION_H
