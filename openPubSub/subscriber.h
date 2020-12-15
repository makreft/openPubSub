#ifndef OPENPUBSUB_SUBSCRIBERTESTS_H
#define OPENPUBSUB_SUBSCRIBERTESTS_H
//#include <open62541/server.h>
#include "util.h"

namespace openPubSub
{
class Client
{
private:
    UA_Boolean m_running;
public:
    Client();
    bool isRunning();
};

}



#endif //OPENPUBSUB_SUBSCRIBERTESTS_H
