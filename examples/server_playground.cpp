#include "../openPubSub/openPubSub.h"
#include "../openPubSub/ua_pubsub/ua_pubsub.h"
#include <open62541/server_config_default.h>
#include <time.h>

static UA_NodeId ds1BoolToggleId;
static int ds1BoolToggleCount = 0;
static UA_Boolean ds1BoolToggleVal = false;
static UA_NodeId ds1Int32Id;
static UA_Int32 ds1Int32Val = 0;
static UA_NodeId ds1Int32FastId;
static UA_Int32 ds1Int32FastVal = 0;
static UA_NodeId ds1DateTimeId;

static UA_NodeId ds2BoolToggleId;
static UA_Boolean ds2BoolToggleVal = false;
static UA_NodeId ds2ByteId;
static UA_Byte ds2ByteVal = 0;
static UA_NodeId ds2Int16Id;
static UA_Int16 ds2Int16Val = 0;
static UA_NodeId ds2Int32Id;
static UA_Int32 ds2Int32Val = 0;
static UA_NodeId ds2Int64Id;
static UA_Int64 ds2Int64Val = 0;
static UA_NodeId ds2SByteId;
static UA_SByte ds2SByteVal = 0;
static UA_NodeId ds2UInt16Id;
static UA_UInt16 ds2UInt16Val = 0;
static UA_NodeId ds2UInt32Id;
static UA_UInt32 ds2UInt32Val = 0;
static UA_NodeId ds2UInt64Id;
static UA_UInt64 ds2UInt64Val = 0;
static UA_NodeId ds2FloatId;
static UA_Float ds2FloatVal = 0;
static UA_NodeId ds2DoubleId;
static UA_Double ds2DoubleVal = 0;
static UA_String *ds2StringArray = NULL;
static size_t ds2StringArrayLen = 0;
static size_t ds2StringIndex = 0;
static UA_NodeId ds2StringId;
static UA_NodeId ds2ByteStringId;
static UA_NodeId ds2GuidId;
static UA_NodeId ds2DateTimeId;
static UA_NodeId ds2UInt32ArrId;
static UA_UInt32 ds2UInt32ArrValue[10] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 };


int main()
{

    openPubSub::Server serv;
    openPubSub::init(serv);
    UA_NodeId writerGroupId, pubsubConnectionId, publishedDSId;
    UA_NodeId publishThisVar = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVERSTATUSTYPE_CURRENTTIME);
    serv.addPubSubConnection("MyPUBSUB", pubsubConnectionId);
    serv.addPublishedDataSet("MyPDS", publishedDSId);
    serv.addDataSetField("Server localtime", publishedDSId, publishThisVar);
    serv.addWriterGroup("MyWriterGroup",pubsubConnectionId,writerGroupId );
    serv.addDataSetWriter("MyDataSetWriter", publishedDSId, writerGroupId);


    serv.run();
}
void
timerCallback(UA_Server *server, void *data) {
// DataSet 1
// BoolToggle
UA_Variant tmpVari;
if (ds1BoolToggleCount >= 3) {
if (ds1BoolToggleVal) {
ds1BoolToggleVal = false;
} else {
ds1BoolToggleVal = true;
}

UA_Variant_init(&tmpVari);
UA_Variant_setScalar(&tmpVari, &ds1BoolToggleVal, &UA_TYPES[UA_TYPES_BOOLEAN]);
UA_Server_writeValue(server, ds1BoolToggleId, tmpVari);
ds1BoolToggleCount = 0;
}

// Int32
UA_Variant_init(&tmpVari);
ds1Int32Val++;
if(ds1Int32Val > 10000)
ds1Int32Val = 0;
UA_Variant_setScalar(&tmpVari, &ds1Int32Val, &UA_TYPES[UA_TYPES_INT32]);
UA_Server_writeValue(server, ds1Int32Id, tmpVari);

// Int32Fast
UA_Variant_init(&tmpVari);
ds1Int32FastVal += 100;
if(ds1Int32FastVal > 10000)
ds1Int32FastVal = 0;
UA_Variant_setScalar(&tmpVari, &ds1Int32FastVal, &UA_TYPES[UA_TYPES_INT32]);
UA_Server_writeValue(server, ds1Int32FastId, tmpVari);

// DateTime
UA_Variant_init(&tmpVari);
UA_DateTime tmpTime = UA_DateTime_now();
UA_Variant_setScalar(&tmpVari, &tmpTime, &UA_TYPES[UA_TYPES_DATETIME]);
UA_Server_writeValue(server, ds1DateTimeId, tmpVari);

// DataSet 2
UA_Server_writeValue(server, ds2DateTimeId, tmpVari);

// BoolToggle
if (ds2BoolToggleVal)
ds2BoolToggleVal = false;
else
ds2BoolToggleVal = true;

// Write new value
UA_Variant_init(&tmpVari);
UA_Variant_setScalar(&tmpVari, &ds2BoolToggleVal, &UA_TYPES[UA_TYPES_BOOLEAN]);
UA_Server_writeValue(server, ds2BoolToggleId, tmpVari);

// Byte
UA_Variant_init(&tmpVari);
ds2ByteVal++;
UA_Variant_setScalar(&tmpVari, &ds2ByteVal, &UA_TYPES[UA_TYPES_BYTE]);
UA_Server_writeValue(server, ds2ByteId, tmpVari);

// Int16
UA_Variant_init(&tmpVari);
ds2Int16Val++;
UA_Variant_setScalar(&tmpVari, &ds2Int16Val, &UA_TYPES[UA_TYPES_INT16]);
UA_Server_writeValue(server, ds2Int16Id, tmpVari);

// Int32
UA_Variant_init(&tmpVari);
ds2Int32Val++;
UA_Variant_setScalar(&tmpVari, &ds2Int32Val, &UA_TYPES[UA_TYPES_INT32]);
UA_Server_writeValue(server, ds2Int32Id, tmpVari);

// Int64
UA_Variant_init(&tmpVari);
ds2Int64Val++;
UA_Variant_setScalar(&tmpVari, &ds2Int64Val, &UA_TYPES[UA_TYPES_INT64]);
UA_Server_writeValue(server, ds2Int64Id, tmpVari);

// SByte
UA_Variant_init(&tmpVari);
ds2SByteVal++;
UA_Variant_setScalar(&tmpVari, &ds2SByteVal, &UA_TYPES[UA_TYPES_SBYTE]);
UA_Server_writeValue(server, ds2SByteId, tmpVari);

// UInt16
UA_Variant_init(&tmpVari);
ds2UInt16Val++;
UA_Variant_setScalar(&tmpVari, &ds2UInt16Val, &UA_TYPES[UA_TYPES_UINT16]);
UA_Server_writeValue(server, ds2UInt16Id, tmpVari);

// UInt32
UA_Variant_init(&tmpVari);
ds2UInt32Val++;
UA_Variant_setScalar(&tmpVari, &ds2UInt32Val, &UA_TYPES[UA_TYPES_UINT32]);
UA_Server_writeValue(server, ds2UInt32Id, tmpVari);

// UInt64
UA_Variant_init(&tmpVari);
ds2UInt64Val++;
UA_Variant_setScalar(&tmpVari, &ds2UInt64Val, &UA_TYPES[UA_TYPES_UINT64]);
UA_Server_writeValue(server, ds2UInt64Id, tmpVari);

// Float
UA_Variant_init(&tmpVari);
ds2FloatVal++;
UA_Variant_setScalar(&tmpVari, &ds2FloatVal, &UA_TYPES[UA_TYPES_FLOAT]);
UA_Server_writeValue(server, ds2FloatId, tmpVari);

// Double
UA_Variant_init(&tmpVari);
ds2DoubleVal++;
UA_Variant_setScalar(&tmpVari, &ds2DoubleVal, &UA_TYPES[UA_TYPES_DOUBLE]);
UA_Server_writeValue(server, ds2DoubleId, tmpVari);

// String
UA_Variant_init(&tmpVari);
ds2StringIndex++;
if(ds2StringIndex >= ds2StringArrayLen)
ds2StringIndex = 0;

UA_Variant_setScalar(&tmpVari, &ds2StringArray[ds2StringIndex], &UA_TYPES[UA_TYPES_STRING]);
UA_Server_writeValue(server, ds2StringId, tmpVari);

// ByteString
UA_Variant_init(&tmpVari);
UA_ByteString bs;
UA_ByteString_init(&bs);
UA_ByteString_allocBuffer(&bs, 4);
UA_UInt32 ui2 = UA_UInt32_random();
memcpy(bs.data, &ui2, 4);

UA_Variant_setScalar(&tmpVari, &bs, &UA_TYPES[UA_TYPES_BYTESTRING]);
UA_Server_writeValue(server, ds2ByteStringId, tmpVari);
UA_ByteString_clear(&bs);

// Guid
UA_Variant_init(&tmpVari);
UA_Guid g = UA_Guid_random();
UA_Variant_setScalar(&tmpVari, &g, &UA_TYPES[UA_TYPES_GUID]);
UA_Server_writeValue(server, ds2GuidId, tmpVari);

// UInt32Array
for(size_t i = 0; i < 10; i++)
ds2UInt32ArrValue[i]++;

UA_Variant_init(&tmpVari);
UA_Variant_setArray(&tmpVari, ds2UInt32ArrValue, 10, &UA_TYPES[UA_TYPES_UINT32]);
UA_Server_writeValue(server, ds2UInt32ArrId, tmpVari);
}

