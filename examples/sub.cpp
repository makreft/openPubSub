/* Callbacks defined for Subscriptions
*typedef void (*UA_Client_DeleteSubscriptionCallback)
*    (UA_Client *client, UA_UInt32 subId, void *subContext);
*
*typedef void (*UA_Client_StatusChangeNotificationCallback)
*    (UA_Client *client, UA_UInt32 subId, void *subContext,
*     UA_StatusChangeNotification *notification);
*
* static UA_INLINE UA_CreateSubscriptionRequest
* UA_CreateSubscriptionRequest_default(void) {
*     UA_CreateSubscriptionRequest request;
*     UA_CreateSubscriptionRequest_init(&request);
*
*     request.requestedPublishingInterval = 500.0;
*     request.requestedLifetimeCount = 10000;
*     request.requestedMaxKeepAliveCount = 10;
*     request.maxNotificationsPerPublish = 0;
*     request.publishingEnabled = true;
*     request.priority = 0;
*     return request;
* }
*/
/* Provides default values for a new subscription.
 *
 * RequestedPublishingInterval:  500.0 [ms]
 * RequestedLifetimeCount: 10000
 * RequestedMaxKeepAliveCount: 10
 * MaxNotificationsPerPublish: 0 (unlimited)
 * PublishingEnabled: true
 * Priority: 0 */

#include <open62541/util.h>
#include <open62541/client_subscriptions.h>



