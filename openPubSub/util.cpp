#include "util.h"
#include <string>
#include <open62541/types_generated_handling.h>


    openPubSub::string::string()
    {
        String = UA_String_new();
        UA_String_init(String);
    }

    openPubSub::string::string(const char *str)
    : string()
    {
        *String = UA_STRING_ALLOC(str);
    }

    openPubSub::string::string(const std::string &str)
    : string(str.c_str())
    {    }

    openPubSub::string::string(const ::UA_String *str)
    : string()
    {
        UA_String_copy(str, String);
    }

    openPubSub::string::~string() {
        UA_String_deleteMembers(String);
        UA_String_delete(String);
    }

    openPubSub::string::operator std::string() const {
        if (String->data == nullptr)
            return std::string();
        else
            return std::string(String->data, String->data + String->length);
    }

    bool openPubSub::string::operator==(const string &rhs) const
    {
        return UA_String_equal(String, rhs.String);
    }

    bool openPubSub::string::operator!=(const string &rhs) const {
        return !(rhs == *this);
    }


