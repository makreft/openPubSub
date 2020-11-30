[![Build Status](https://travis-ci.com/makreft/openPubSub.svg?branch=main)](https://travis-ci.com/makreft/openPubSub)
[![code quality score](https://www.code-inspector.com/project/16691/score/svg)](https://www.code-inspector.com)
[![Code Grade](https://www.code-inspector.com/project/16691/status/svg)](https://www.code-inspector.com)

# Goal of this Repo
the goal is to write a wrapper of open62541 PubSub implementation in C++

# Documentation

You can find a more complete documentation [here](https://makreft.github.io/openPubSub/)


# openPubSub initialization
```bash
> clone this project
> git submodule update --init --recursive
> cd open62541
> git submodule update --init --recursive
> mkdir build && cd build
> cmake ..
> cmake -DUA_ENABLE_DISCOVERY=ON -DUA_ENABLE_DISCOVERY_MULTICAST=ON -DUA_ENABLE_HARDENING=ON -DUA_ENABLE_METHODCALLS=ON -DUA_ENABLE_NODEMANAGEMENT=ON -DUA_ENABLE_PARSING=ON -DUA_ENABLE_PUBSUB=ON -DUA_ENABLE_PUBSUB_ETH_UADP=ON -DUA_NAMESPACE_ZERO=FULL ../
> make install # by default this will install open62541 in your system library.  \
# Configure install path for best practice. But than also root level CMake has \
# to be adjusted apropriately.
```

### Optional if using the cmake gui:
* check the Advanced Option
* Toggle the following Options (if not already ON):
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

# dependencies
* G++_VERSION >= g++-7
* open62541 v.1.1

# example
```cpp
#include "include/openPubSub.h"

int main()
{
    openPubSub::Server serv;
    openPubSub::init(serv);
    serv.run();
}
```

# Building the documentation
```console
> cd docs_sphinx
> make html
> cd _build/html
> # open index.html with your browser of choice
```
