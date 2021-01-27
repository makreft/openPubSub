//
// Created by sba on 14.03.20.
//

#include "Errors.h"

void checkUaStatusCode(UA_StatusCode returnCode) {
    if (returnCode != UA_STATUSCODE_GOOD) {
        throw ua_exception(returnCode);
    }
}