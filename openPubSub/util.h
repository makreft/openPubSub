#include <string>
#include <open62541/types_generated.h>
#include <open62541/types.h>

#ifndef OPENPUBSUB_UTIL_H
#define OPENPUBSUB_UTIL_H

namespace openPubSub
{

    class ua_exception : public std::exception
    {
    public:
        UA_StatusCode code;

        explicit ua_exception(UA_StatusCode returnCode)
        {
            code = returnCode;
        }

        const char * what() const throw() override
        {
            return UA_StatusCode_name(code);
        }
    };

    struct string
    {
        string();
        string(const char *str);
        string(const std::string &str);
        explicit string(const ::UA_String *str);
        ~string();

        ::UA_String *String;

        explicit operator std::string() const;
        bool operator==(const string &rhs) const;
        bool operator!=(const string &rhs) const;
    };

}


#endif //OPENPUBSUB_UTIL_H
