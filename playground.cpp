#include <iostream>
#include <string>
#include "openPubSub.h"
#include <memory>

UA_Boolean running = true;
static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}
int main()
{
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);


    openPubSub::Server(serv);
    std::cout << "doin sth..." << "\n";
    UA_StatusCode retVal;
    retVal = UA_Server_run(serv.p_server, &running);
    UA_Server_delete(serv.p_server);
    return (int) retVal;
}