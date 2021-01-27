//
// Created by sba on 27.02.20.
//

#ifndef SMARTFACTORYLIB_UATYPES_H
#define SMARTFACTORYLIB_UATYPES_H

#include <open62541/types.h>
#include "Errors.h"

template<class U>
const UA_DataType * get_UA_Type() {
    throw not_implemented_exception<U>();
}

#endif //SMARTFACTORYLIB_UATYPES_H
