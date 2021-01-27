/* This is free and unencumbered software released into the public domain.
 * See LICENSE file for more information */


#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include "../openPubSub/openPubSub.h"
UA_StatusCode iterateNoWaitInternal(openPubSub::Server *server) {
    UA_StatusCode retval = UA_Server_run_startup(UA_Server);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    while(running) {
        UA_Server_run_iterate(server, false);
    }
    retval = UA_Server_run(server, &running);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    return UA_STATUSCODE_GOOD;
}
int main()
{
    openPubSub::Server server;
    openPubSub::init(server);

}

