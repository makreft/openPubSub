#include "../openPubSub/openPubSub.h"
#include "../openPubSub/ua_pubsub/ua_pubsub.h"
#include <open62541/server_config_default.h>
#include <time.h>
#include <fstream>
#include <iostream>

#define CLK 5
#define DBIT 6 //SO
#define CS 7

#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

int Thermal_Couple_Read()
{
    int value = 0;
    //init sensor
    digitalWrite(CS, LOW);
    delay(2);
    digitalWrite(CS, HIGH);
    delay(200);
    /* Read the chip and return the raw temperature value
     * Bring CS pin low to allow us to read the data from
     * the conversion process
     */
    digitalWrite(CS, LOW);
    /* Cycle the clock for dummy bit 15 */
    digitalWrite(CLK, HIGH);
    // delay(1);
    digitalWrite(CLK, LOW);
    
    /* Read bits 14-3 from MAX6675 for the Temp.
     * Loop for each bit reading the value and storing
     * the final value in 'temp'
     */
    int i;
    for(i = 14; i >= 0; i--)
    {
    digitalWrite(CLK, HIGH);
    //delay(1);
    value += digitalRead(DBIT) << i;
    digitalWrite(CLK, LOW);
    }
    // check bit D2 if HIGH no sensor
    if ((value & 0x04) == 0x04) return -1;
    // shift rigth three places
    return value >> 3;
}
UA_NodeId SENSOR_VALUE_ID;
UA_Int32 SENSOR_VALUE = 0;
float Ctemp, Ftemp;


void max6675_callback(UA_Server *server, void *data)
{
    SENSOR_VALUE = Thermal_Couple_Read();
    if (SENSOR_VALUE == -1)
    {
        printf("No sensor connected.");
    }
    //else
    //{
    //    Ctemp = SENSOR_VALUE * 0.25;
    //    printf("Temp in °C: %4.2f \n", Ctemp);
    //}
    UA_Variant tmpVari; //Temperature Variant
    UA_Variant_init(&tmpVari);
    UA_Variant_setScalar(&tmpVari, &SENSOR_VALUE, &UA_TYPES[UA_TYPES_FLOAT]);
    UA_Server_writeValue(server, SENSOR_VALUE_ID, tmpVari);
}

int main()
{
    if(wiringPiSetup() == -1)
    {
        printf("wiringpi setup failed \n");
        exit(1);
    }
    pinMode(CLK, OUTPUT);
    pinMode(DBIT, INPUT);
    pinMode(CS, OUTPUT);

    digitalWrite(CS, HIGH);
    digitalWrite(CLK, LOW);


    openPubSub::Server serv;
    openPubSub::init(serv);

    UA_NodeId writerGroupId, pubsubConnectionId, publishedDSId;
    UA_NetworkAddressUrlDataType networkAddressUrl =
        {UA_STRING_NULL , UA_STRING("opc.udp://224.0.0.22:4840/")};

    //1. Adding PubSub Connection onto the Server
    serv.addPubSubConnection(&networkAddressUrl, "UADP Connection 1", 2234);

    //2. Adding a DataSet for the Temperature
    serv.addPublishedDataSet("DataSet Temperature");

    //3. Adding an Object Node
    UA_NodeId folderId;
    serv.addObjectNode("Publisher1", &folderId);
    //4. Adding a UA_TYPES_INT32 Variable Node
    serv.addInt32VariableNode(SENSOR_VALUE_ID, folderId, SENSOR_VALUE);
    //5. Adding a Int32 DataSetField
    serv.addInt32DataSetField(SENSOR_VALUE_ID);
    //serv.addDataSetField("Server localtime");
    serv.addWriterGroup();
    serv.addDataSetWriter();
    UA_StatusCode code;
    UA_Server *ua_server = serv.getUAServer();
    UA_UInt64 callbackId = 0;
    code = UA_Server_addRepeatedCallback(ua_server, (UA_ServerCallback)max6675_callback,
                                    NULL, 1000, &callbackId);
    std::cout << "status code: " << code << "\n" << std::endl;
    serv.run();
    //if(code)
    //{    
    //    serv.run();
    //}
    //else
    //{
    //     exit(3);
    //}
    return 0;
}
//void
//timerCallback(UA_Server *server, void *data) {
//// DataSet 1
//// BoolToggle
//UA_Variant tmpVari;
//if (ds1BoolToggleCount >= 3) {
//if (ds1BoolToggleVal) {
//ds1BoolToggleVal = false;
//} else {
//ds1BoolToggleVal = true;
//}
//
//UA_Variant_init(&tmpVari);
//UA_Variant_setScalar(&tmpVari, &ds1BoolToggleVal, &UA_TYPES[UA_TYPES_BOOLEAN]);
//UA_Server_writeValue(server, ds1BoolToggleId, tmpVari);
//ds1BoolToggleCount = 0;
//}
//
//// Int32
//UA_Variant_init(&tmpVari);
//ds1Int32Val++;
//if(ds1Int32Val > 10000)
//ds1Int32Val = 0;
//UA_Variant_setScalar(&tmpVari, &ds1Int32Val, &UA_TYPES[UA_TYPES_INT32]);
//UA_Server_writeValue(server, ds1Int32Id, tmpVari);
//
//// Int32Fast
//UA_Variant_init(&tmpVari);
//ds1Int32FastVal += 100;
//if(ds1Int32FastVal > 10000)
//ds1Int32FastVal = 0;
//UA_Variant_setScalar(&tmpVari, &ds1Int32FastVal, &UA_TYPES[UA_TYPES_INT32]);
//UA_Server_writeValue(server, ds1Int32FastId, tmpVari);
//
//// DateTime
//UA_Variant_init(&tmpVari);
//UA_DateTime tmpTime = UA_DateTime_now();
//UA_Variant_setScalar(&tmpVari, &tmpTime, &UA_TYPES[UA_TYPES_DATETIME]);
//UA_Server_writeValue(server, ds1DateTimeId, tmpVari);
//
//// DataSet 2
//UA_Server_writeValue(server, ds2DateTimeId, tmpVari);
//
//// BoolToggle
//if (ds2BoolToggleVal)
//ds2BoolToggleVal = false;
//else
//ds2BoolToggleVal = true;
//
//// Write new value
//UA_Variant_init(&tmpVari);
//UA_Variant_setScalar(&tmpVari, &ds2BoolToggleVal, &UA_TYPES[UA_TYPES_BOOLEAN]);
//UA_Server_writeValue(server, ds2BoolToggleId, tmpVari);
//
//// Byte
//UA_Variant_init(&tmpVari);
//ds2ByteVal++;
//UA_Variant_setScalar(&tmpVari, &ds2ByteVal, &UA_TYPES[UA_TYPES_BYTE]);
//UA_Server_writeValue(server, ds2ByteId, tmpVari);
//
//// Int16
//UA_Variant_init(&tmpVari);
//ds2Int16Val++;
//UA_Variant_setScalar(&tmpVari, &ds2Int16Val, &UA_TYPES[UA_TYPES_INT16]);
//UA_Server_writeValue(server, ds2Int16Id, tmpVari);
//
//// Int32
//UA_Variant_init(&tmpVari);
//ds2Int32Val++;
//UA_Variant_setScalar(&tmpVari, &ds2Int32Val, &UA_TYPES[UA_TYPES_INT32]);
//UA_Server_writeValue(server, ds2Int32Id, tmpVari);
//
//// Int64
//UA_Variant_init(&tmpVari);
//ds2Int64Val++;
//UA_Variant_setScalar(&tmpVari, &ds2Int64Val, &UA_TYPES[UA_TYPES_INT64]);
//UA_Server_writeValue(server, ds2Int64Id, tmpVari);
//
//// SByte
//UA_Variant_init(&tmpVari);
//ds2SByteVal++;
//UA_Variant_setScalar(&tmpVari, &ds2SByteVal, &UA_TYPES[UA_TYPES_SBYTE]);
//UA_Server_writeValue(server, ds2SByteId, tmpVari);
//
//// UInt16
//UA_Variant_init(&tmpVari);
//ds2UInt16Val++;
//UA_Variant_setScalar(&tmpVari, &ds2UInt16Val, &UA_TYPES[UA_TYPES_UINT16]);
//UA_Server_writeValue(server, ds2UInt16Id, tmpVari);
//
//// UInt32
//UA_Variant_init(&tmpVari);
//ds2UInt32Val++;
//UA_Variant_setScalar(&tmpVari, &ds2UInt32Val, &UA_TYPES[UA_TYPES_UINT32]);
//UA_Server_writeValue(server, ds2UInt32Id, tmpVari);
//
//// UInt64
//UA_Variant_init(&tmpVari);
//ds2UInt64Val++;
//UA_Variant_setScalar(&tmpVari, &ds2UInt64Val, &UA_TYPES[UA_TYPES_UINT64]);
//UA_Server_writeValue(server, ds2UInt64Id, tmpVari);
//
//// Float
//UA_Variant_init(&tmpVari);
//ds2FloatVal++;
//UA_Variant_setScalar(&tmpVari, &ds2FloatVal, &UA_TYPES[UA_TYPES_FLOAT]);
//UA_Server_writeValue(server, ds2FloatId, tmpVari);
//
//// Double
//UA_Variant_init(&tmpVari);
//ds2DoubleVal++;
//UA_Variant_setScalar(&tmpVari, &ds2DoubleVal, &UA_TYPES[UA_TYPES_DOUBLE]);
//UA_Server_writeValue(server, ds2DoubleId, tmpVari);
//
//// String
//UA_Variant_init(&tmpVari);
//ds2StringIndex++;
//if(ds2StringIndex >= ds2StringArrayLen)
//ds2StringIndex = 0;
//
//UA_Variant_setScalar(&tmpVari, &ds2StringArray[ds2StringIndex], &UA_TYPES[UA_TYPES_STRING]);
//UA_Server_writeValue(server, ds2StringId, tmpVari);
//
//// ByteString
//UA_Variant_init(&tmpVari);
//UA_ByteString bs;
//UA_ByteString_init(&bs);
//UA_ByteString_allocBuffer(&bs, 4);
//UA_UInt32 ui2 = UA_UInt32_random();
//memcpy(bs.data, &ui2, 4);
//
//UA_Variant_setScalar(&tmpVari, &bs, &UA_TYPES[UA_TYPES_BYTESTRING]);
//UA_Server_writeValue(server, ds2ByteStringId, tmpVari);
//UA_ByteString_clear(&bs);
//
//// Guid
//UA_Variant_init(&tmpVari);
//UA_Guid g = UA_Guid_random();
//UA_Variant_setScalar(&tmpVari, &g, &UA_TYPES[UA_TYPES_GUID]);
//UA_Server_writeValue(server, ds2GuidId, tmpVari);
//
//// UInt32Array
//for(size_t i = 0; i < 10; i++)
//ds2UInt32ArrValue[i]++;
//
//UA_Variant_init(&tmpVari);
//UA_Variant_setArray(&tmpVari, ds2UInt32ArrValue, 10, &UA_TYPES[UA_TYPES_UINT32]);
//UA_Server_writeValue(server, ds2UInt32ArrId, tmpVari);
//}
//
