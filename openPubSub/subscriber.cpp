#include "subscriber.h"

openPubSub::Client::Client()
:m_running(true){}

bool openPubSub::Client::isRunning()
{
    if (m_running)
        return true;
    else
        return false;
}
