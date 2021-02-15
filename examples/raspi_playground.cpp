#include "../openPubSub/openPubSub.h"
#include "../openPubSub/ua_pubsub/ua_pubsub.h"


#include <open62541/plugin/log_stdout.h>
#include <open62541/plugin/pubsub_udp.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <signal.h>
#include <stdlib.h>
#include <open62541/server_pubsub.h>


#define CLK 5
#define DBIT 6 //SO
#define CS 7

#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

UA_UInt32 *valueStore[1];
static int Thermal_Couple_Read()
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

static void valueUpdateCallback(UA_Server *server, void *data)
{
    u_int32_t sensorVal;
    for (int i = 0; i < 1; ++i)
    {
        sensorVal = Thermal_Couple_Read();
        //std::cout << "Inside valueUpdateCallback, sensorVal: " << sensorVal << "\n";
        if (sensorVal == -1)
            printf("Issue with sensor.");
        *valueStore[i] = sensorVal;
    }
}

int main()
{
    // WiringPi setup
    int setup = wiringPiSetup();
    if(setup == -1)
    {
        std::cout << "setup error" << "\n";
        exit(1);
    }
    pinMode(CLK, OUTPUT);
    pinMode(DBIT, INPUT);
    pinMode(CS, OUTPUT);

    digitalWrite(CS, HIGH);
    digitalWrite(CLK, LOW);

    // openPubSub init
    openPubSub::Server serv;
    openPubSub::init(serv);


    serv.addPubSubConnection("opc.udp://224.0.0.22:4840/", "UADP Connection 1");
    serv.addPublishedDataSet("Server Playground PDS");
    serv.addWriterGroup("Server Playground WriterGroup");
    serv.addDataSetWriter("Server Playground DataSetWriter");
    serv.addDataSetField("Server Playground DataSetField");
    serv.freezeWriterGroupConfiguration();
    UA_Server* ua_server = serv.getUAServer();
    UA_UInt64 callbackId;
    UA_Server_addRepeatedCallback(ua_server, valueUpdateCallback, NULL, 100,&callbackId);

    serv.run();
}

