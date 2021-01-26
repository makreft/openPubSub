
#include "subscription.h"
    namespace openPubSub
    {


    Subscriber *__subscriber;
    static void stopHandler()
    {
        __subscriber->stopSubcription();
    }
    void init(Subscriber &subscriber)
    {
        __subscriber = &subscriber;
        signal(SIGINT, reinterpret_cast<__sighandler_t>(stopHandler));
        signal(SIGTERM, reinterpret_cast<__sighandler_t>(stopHandler));
    }
    struct Subscriber::Impl
    {
        UA_NetworkMessage networkMessage;
        UA_PubSubChannel *psc;
        UA_Boolean m_running = true;
        UA_ByteString buffer;
        UA_PubSubTransportLayer udpLayer = UA_PubSubTransportLayerUDPMP();
        UA_PubSubConnectionConfig connectionConfig;
        UA_NetworkAddressUrlDataType networkAddressUrl =
            {UA_STRING_NULL , UA_STRING("opc.udp://224.0.0.22:4840/")};
    };
    Subscriber::Subscriber()
        : mImpl(std::make_unique<Impl>())
    {
        memset(&mImpl->connectionConfig, 0, sizeof(mImpl->connectionConfig));
        mImpl->connectionConfig.name = UA_STRING("UADP Connection 1");
        mImpl->connectionConfig.transportProfileUri =
            UA_STRING("http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp");
        mImpl->connectionConfig.enabled = UA_TRUE;
        UA_Variant_setScalar(&mImpl->connectionConfig.address,
                             &mImpl->networkAddressUrl,
                             &UA_TYPES[UA_TYPES_NETWORKADDRESSURLDATATYPE]);
        mImpl->psc =
            mImpl->udpLayer.createPubSubChannel(&mImpl->connectionConfig);
        mImpl->psc->regist(mImpl->psc, NULL, NULL);
    }

    UA_StatusCode Subscriber::listen()
    {
        UA_StatusCode retval = UA_ByteString_allocBuffer(&mImpl->buffer, 512);
        if(retval != UA_STATUSCODE_GOOD)
        {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
        "Message buffer allocation failed!");
        return retval;
        }

        /* Receive the message. Blocks for 100ms */
        retval = mImpl->psc->receive(mImpl->psc, &mImpl->buffer, NULL, 100);
        if(retval != UA_STATUSCODE_GOOD || mImpl->buffer.length == 0)
        {
        /* Workaround!! Reset buffer length. Receive can set the length to zero.
         * Then the buffer is not deleted because no memory allocation is
         * assumed.
         * TODO: Return an error code in 'receive' instead of setting the buf
         * length to zero. */
        mImpl->buffer.length = 512;
        UA_ByteString_clear(&mImpl->buffer);
        return UA_STATUSCODE_GOOD;
        }

        /* Decode the message */
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
        "Message length: %lu", (unsigned long) mImpl->buffer.length);
        memset(&mImpl->networkMessage, 0, sizeof(UA_NetworkMessage));
        size_t currentPosition = 0;
        UA_NetworkMessage_decodeBinary(&mImpl->buffer, &currentPosition, &mImpl->networkMessage);
        UA_ByteString_clear(&mImpl->buffer);

        /* Is this the correct message type? */
        if(mImpl->networkMessage.networkMessageType != UA_NETWORKMESSAGE_DATASET)
        goto cleanup;

        /* At least one DataSetMessage in the NetworkMessage? */
        if(mImpl->networkMessage.payloadHeaderEnabled &&
            mImpl->networkMessage.payloadHeader.dataSetPayloadHeader.count < 1)
        goto cleanup;

        /* Is this a KeyFrame-DataSetMessage? */
        for(size_t j = 0; j < mImpl->networkMessage.payloadHeader.dataSetPayloadHeader.count; j++)
        {
        UA_DataSetMessage *dsm = &mImpl->networkMessage.payload.dataSetPayload.dataSetMessages[j];
        if(dsm->header.dataSetMessageType != UA_DATASETMESSAGE_DATAKEYFRAME)
        continue;

        /* Loop over the fields and print well-known content types */
        for(int i = 0; i < dsm->data.keyFrameData.fieldCount; i++)
        {
            const UA_DataType *currentType = dsm->data.keyFrameData.dataSetFields[i].value.type;
            if(currentType == &UA_TYPES[UA_TYPES_BYTE])
            {
            UA_Byte value = *(UA_Byte *)dsm->data.keyFrameData.dataSetFields[i].value.data;
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
            "Message content: [Byte] \tReceived data: %i", value);
            } else if (currentType == &UA_TYPES[UA_TYPES_UINT32])
            {
            UA_UInt32 value = *(UA_UInt32 *)dsm->data.keyFrameData.dataSetFields[i].value.data;
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
            "Message content: [UInt32] \tReceived data: %u", value);
            } else if (currentType == &UA_TYPES[UA_TYPES_DATETIME])
            {
            UA_DateTime value = *(UA_DateTime *)dsm->data.keyFrameData.dataSetFields[i].value.data;
            UA_DateTimeStruct receivedTime = UA_DateTime_toStruct(value);
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
            "Message content: [DateTime] \t"
            "Received date: %02i-%02i-%02i Received time: %02i:%02i:%02i",
            receivedTime.year, receivedTime.month, receivedTime.day,
            receivedTime.hour, receivedTime.min, receivedTime.sec);
            }
        }
    }
    cleanup:
    UA_NetworkMessage_clear(&mImpl->networkMessage);
    return retval;
    }

    void Subscriber::stopSubcription()
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
        mImpl->m_running = false;
    }

    UA_StatusCode Subscriber::allocCustomBuffer(int bufferSize)
    {
        UA_StatusCode retval =UA_ByteString_allocBuffer(&mImpl->buffer, bufferSize);
        return retval;
    }

Subscriber::~Subscriber()
{
    mImpl->psc->close(mImpl->psc);
}

//TODO: while run not only listen but also write data to a node
//run(UA_Callback_LifeCycle callback)
void Subscriber::run()
{
    mImpl->m_running = UA_TRUE;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    while(mImpl->m_running && retval == UA_STATUSCODE_GOOD)
        retval = listen();
}
}
