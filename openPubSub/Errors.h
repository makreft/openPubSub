//
// Created by sba on 27.02.20.
//

#ifndef SMARTFACTORYLIB_ERRORS_H
#define SMARTFACTORYLIB_ERRORS_H

#include <cxxabi.h>
#include <iostream>
#include <open62541/types.h>

void checkUaStatusCode(UA_StatusCode returnCode);

struct ua_exception : public std::exception {

    UA_StatusCode code;

    explicit ua_exception(UA_StatusCode returnCode) {
        code = returnCode;
    }

    const char * what() const throw() override {
        return UA_StatusCode_name(code);
    }

};

struct type_missmatch_exception : public std::exception {
    const char * what () const throw () override {
        return "Types does not match";
    }
};

struct not_initialized: public std::exception {
    const char * what () const throw () override {
        return "Object is not initialized";
    }
};

// TODO make it compile time? boost::typeindex:type_id
template<class U>
struct not_implemented_exception : public std::exception {
    const char * what () const throw () override {
        char * raw = abi::__cxa_demangle(typeid(U).name(), NULL, NULL, NULL);
        std::string typeName(raw);
        free(raw);
        return typeName.c_str();
    }
};

#endif //SMARTFACTORYLIB_ERRORS_H
