[![Codacy Badge](https://api.codacy.com/project/badge/Grade/00517f54bfcc42938d11df09cd700ebd)](https://app.codacy.com/gh/makreft/openPubSub?utm_source=github.com&utm_medium=referral&utm_content=makreft/openPubSub&utm_campaign=Badge_Grade)
[![Build Status](https://travis-ci.com/makreft/openPubSub.svg?branch=main)](https://travis-ci.com/makreft/openPubSub)

# Goal of this Repo
the goal is to write a wrapper of open62541 PubSub implementation in C++

# Documentation (in development)

You can find a more complete documentation [here](https://makreft.github.io/openPubSub/)


# installing open62541 locally
```bash
> clone https://github.com/makreft/openPubSub.git
> cd openPubSub
> git submodule update --init --recursive
> cd external/open62541
> mkdir build; cd build
> cmake ..
> cmake -DCMAKE_INSTALL_PREFIX=$HOME/install/open62541 -DUA_BUILD_TOOLS=ON \
        -DUA_ENABLE_DISCOVERY_MULTICAST=ON -DUA_ENABLE_HARDENING=ON \
        -DUA_ENABLE_NODESET_COMPILER_DESCRIPTIONS=ON -DUA_ENABLE_PUBSUB=ON \
        -DUA_ENABLE_PUBSUB_ETH_UADP=ON -DUA_ENABLE_SUBSCRIPTIONS=ON \
        -DUA_LOGLEVEL=300 -DUA_NAMESPACE_ZERO=FULL
> make install
```

## Using the cmake gui:
* check the Advanced Option
* Toggled Options:
  * UA_BUILD_EXAMPLES=OFF
  * UA_BUILD_FUZZING=OFF
  * UA_BUILD_FUZZING_CORPUS=OFF
  * UA_BUILD_OSS_FUZZ=OFF
  * UA_BUILD_TOOLS=ON
  * UA_BUILD_UNIT_TESTS=OFF
  * UA_DEBUG=OFF
  * UA_DEBUG_DUMP_PKGS=OFF
  * UA_ENABLE_COVERAGE=OFF
  * UA_ENABLE_DETERMINISTIC_RNG=OFF
  * UA_ENABLE_DISCOVERY_MULTICAST=ON
  * UA_ENABLE_ENCRYPTION=OFF
  * UA_ENABLE_ENCRYPTION_OPENSSL=OFF
  * UA_ENABLE_HARDENING=ON
  * UA_ENABLE_IMMUTABLE_NODES=OFF
  * UA_ENABLE_MICRO_EMB_DEV_PROFILE=OFF
  * UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS=ON
  * UA_ENABLE_PUBSUB=ON
  * UA_ENABLE_PUBSUB_CUSTOM_PUBLISH_HANDLING=OFF
  * UA_ENABLE_PUBSUB_ETH_UADP=ON
  * UA_ENABLE_PUBSUB_ETH_UADP_XDP=OFF
  * UA_ENABLE_PUBSUB_INFORMATIONMODEL_METHODS=OFF
  * UA_ENABLE_QUERY=OFF
  * UA_ENABLE_STATIC_ANALYZER=OFF
  * UA_ENABLE_SUBSCRIPTIONS=ON
  * UA_ENABLE_SUBSCRIPTIONS_EVENTS=OFF
  * UA_ENABLE_UNIT_TESTS_MEMCHECK=OFF
  * UA_ENABLE_UNIT_TEST_FAILURE_HOOKS=OFF
  * UA_ENABLE_VALGRIND_INTERACTIVE=OFF
  * UA_FILE_NS0=OFF
  * UA_FILE_NS0_BLACKLIST=OFF
  * UA_LOGLEVEL=300
  * UA_MULTITHREADING=0
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
