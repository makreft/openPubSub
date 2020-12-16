#include "util.h"
#include <string>
#include <open62541/types_generated_handling.h>
#include <open62541/plugin/log.h>
#include <open62541/plugin/log_stdout.h>

 // hidden from the user should only be accessed by appropriate methods
 void initialize::stopHandler()
 {
    initialize().stop();
 }
 void initialize::init()
 {
     //_i = this;
     signal(SIGINT, reinterpret_cast<__sighandler_t>(stopHandler));
     signal(SIGTERM, reinterpret_cast<__sighandler_t>(stopHandler));
 }
 void initialize::stop()
 {
     UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
     _running = false;
 }
openPubSub::string::string()
{
    value = UA_String_new();
    UA_String_init(value);
}
openPubSub::string::string(const char *str)
: string()
{
    *value = UA_STRING_ALLOC(str);
}
openPubSub::string::string(const std::string &str)
: string(str.c_str())
{}
openPubSub::string::string(const ::UA_String *str)
: string()
{
    UA_String_copy(str, value);
}

openPubSub::string::~string()
{
    UA_String_deleteMembers(value);
    UA_String_delete(value);
}

openPubSub::string::operator std::string() const
{
        if (value->data == nullptr)
            return std::string();
        else
            return std::string(value->data, value->data + value->length);
}

bool openPubSub::string::operator==(const string &rhs) const
{
    return UA_String_equal(value, rhs.value);
}

bool openPubSub::string::operator!=(const string &rhs) const
{
    return !(rhs == *this);
}
