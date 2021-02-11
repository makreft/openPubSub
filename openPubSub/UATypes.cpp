//
// Created by sba on 14.03.20.
//

#include "UATypes.h"
#include <open62541/types_generated.h>

#define GEN_DEFAULT_UA_TYPE_TRAIT(TYPE_NAME,UA_TYPE_NAME) \
template<> \
const UA_DataType * get_UA_Type<TYPE_NAME>() { \
    return &UA_TYPES[UA_TYPE_NAME]; \
}

GEN_DEFAULT_UA_TYPE_TRAIT(UA_Boolean, UA_TYPES_BOOLEAN) // Also bool
GEN_DEFAULT_UA_TYPE_TRAIT(UA_SByte, UA_TYPES_SBYTE)
GEN_DEFAULT_UA_TYPE_TRAIT(UA_Byte, UA_TYPES_BYTE)
GEN_DEFAULT_UA_TYPE_TRAIT(UA_Int16, UA_TYPES_INT16) // Also short
GEN_DEFAULT_UA_TYPE_TRAIT(UA_UInt16, UA_TYPES_UINT16) // Also unsigned short
GEN_DEFAULT_UA_TYPE_TRAIT(UA_Int32, UA_TYPES_INT32) // Also int
GEN_DEFAULT_UA_TYPE_TRAIT(UA_UInt32, UA_TYPES_UINT32) // Also unsigned int
GEN_DEFAULT_UA_TYPE_TRAIT(UA_Int64, UA_TYPES_UINT32) // Also long and UA_DateTime!
GEN_DEFAULT_UA_TYPE_TRAIT(UA_UInt64, UA_TYPES_UINT32) // Also unsigned long
GEN_DEFAULT_UA_TYPE_TRAIT(UA_Float, UA_TYPES_FLOAT) // Also float
GEN_DEFAULT_UA_TYPE_TRAIT(UA_Double, UA_TYPES_DOUBLE) // Also double
GEN_DEFAULT_UA_TYPE_TRAIT(UA_String, UA_TYPES_STRING) // NOT THE SAME AS STD::STRING!
//GEN_DEFAULT_UA_TYPE_TRAIT(UA_DateTime, UA_TYPES_DATETIME) // Same as UA_Int64!

GEN_DEFAULT_UA_TYPE_TRAIT(UA_Argument, UA_TYPES_ARGUMENT)
GEN_DEFAULT_UA_TYPE_TRAIT(UA_NodeId, UA_TYPES_NODEID)
GEN_DEFAULT_UA_TYPE_TRAIT(UA_LocalizedText, UA_TYPES_LOCALIZEDTEXT)