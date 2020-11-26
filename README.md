[![Build Status](https://travis-ci.com/makreft/openPubSub.svg?branch=main)](https://travis-ci.com/makreft/openPubSub)

# Goal of this Repo
the goal is to write a wrapper of open62541 PubSub implementation in C++

# openPubSub initialization
```console
> clone this project
> git submodule update --init --recursive
> cd lib/open62541
> mkdir build && cd build
> cmake-gui ..
```
* check the Advanced Option
* Toggle the following Options:
    * UA_BUILD_EXAMPLES
    * UA_BUILD_TOOLS
    * UA_ENABLE_DA
    * UA_ENABLE_DISCOVERY
    * UA_ENABLE_DISCOVERY_MULTICAST
    * UA_ENABLE_HARDENING
    * UA_ENABLE_METHODCALLS
    * UA_ENABLE_NODEMANAGEMENT
    * UA_ENABLE_NODESET_COMPILER_DESC..
    * UA_ENABLE_PARSING
    * UA_ENABLE_PUBSUB
    * UA_ENABLE_PUBSUB_ETH_UADP
    * UA_ENABLE_PUBSUB_INFORMATIONMODEL
    * UA_ENABLE_PUBSUB_INFORMATIONMODEL_METHODS
    * UA_ENABLE_PUBSUB_MQTT
    * UA_ENABLE_STATUSCODE_DESCRIPTIONS
    * UA_NAMESPACE_ZERO=FULL

```console
> make -j8
```



# Building the documentation
```console
> cd docs_sphinx
> make html
```

go to the _build/html folder and open the "index.html"
