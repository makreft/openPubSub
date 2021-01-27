//
// Created by sba on 28.02.20.
//

#ifndef SMARTFACTORYLIB_VARIABLENODE_H
#define SMARTFACTORYLIB_VARIABLENODE_H

#include <vector>

#include <open62541/server.h>
#include "UATypes.h"
#include "Errors.h"

class NodeDataBase{
public:
    bool isInit(){
        return _init;
    }

    virtual void init(UA_Server *server, UA_NodeId id) = 0;
protected:
    bool _init = false;
    UA_NodeId _id;
    UA_Server * _server;
    const UA_DataType * _type;
};

template<typename T>
class NodeData : public NodeDataBase{

public:
    NodeData() = default;

    void init(UA_Server *server, UA_NodeId id) override {

        this->_id = id;
        this->_server = server;

        // TODO Typecheck
        this->_type = get_UA_Type<T>();

        this->_init = true;
    }

    NodeData<T> & operator=(T rhs) {
        if (!_init) {
            throw not_initialized();
        }

        UA_Variant val;
        UA_Variant_setScalar(&val, &rhs, this->_type);
        UA_Server_writeValue(this->_server, this->_id, val);
        return *this;
    }

    operator T() const { // NOLINT
        UA_Variant val;
        UA_Server_readValue(this->_server, this->_id, &val);
        T data = *(T *) val.data;
        UA_Variant_clear(&val);
        return data;
    }
};

template<typename T>
class NodeData<std::vector<T>> : public NodeDataBase{

public:
    NodeData() = default;

    void init(UA_Server *server, UA_NodeId id) override {

        this->_id = id;
        this->_server = server;

        // TODO Typecheck
        this->_type = get_UA_Type<T>();

        this->_init = true;
    }

    NodeData<std::vector<T>> & operator=(std::vector<T> rhs) {
        if (!_init) {
            throw not_initialized();
        }
        UA_Variant val;
        UA_Variant_setArray(&val, rhs.data(), rhs.size(), this->_type);
        UA_Server_writeValue(this->_server, this->_id, val);
        return *this;
    }

    operator std::vector<T>() const { // NOLINT
        UA_Variant val;
        UA_Server_readValue(this->_server, this->_id, &val);
        std::vector<T> ret((T *) val.data, ((T *)val.data) + val.arrayLength);
        return ret;
    }
};



#endif //SMARTFACTORYLIB_VARIABLENODE_H
