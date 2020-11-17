#ifndef OPCUAGENERICCLIENT_HH_
#define OPCUAGENERICCLIENT_HH_

#include <map>
#include <vector>
#include <string>

// C++11 interface
#include <mutex>
#include <chrono>
#include <condition_variable>

#ifdef HAS_OPCUA
	#include <open62541/client.h>
#endif

namespace OPCUA {

class GenericClient {
#ifdef HAS_OPCUA
private:
	// the OPC UA client pointer (is initialized in connect() and destroyed in disconnect() methods)
	UA_Client *client;

	// the OPC UA Object ID (for the connected root object)
	NodeId rootObjectId;

	// the general client mutex (prevents from concurrent access of the internal UA Client, which is not thread safe by itself)
	mutable std::recursive_mutex clientMutex;

	// make the GenericClient non-copyable
	GenericClient(const GenericClient&) = delete;
	GenericClient& operator=(const GenericClient&) = delete;

	/** the internal registry of all the used entities (i.e. typically OPC UA variables)
	 *
	 *  Please overload and implement the method createClientSpace() in derived classes
	 *  such that all the needed entities are stored within this registry so that
	 *  they do not need to be searched on the fly each time the get/set entity method is called.
	 */
	std::map<std::string, NodeId> entityRegistry;

	/// internal registry to manage subscriptions (key=SubscriptionId, value=entityBrowseName)
	std::map<UA_UInt32, std::string> subscriptionsRegistry;
	std::chrono::steady_clock::duration minSubscriptionInterval;

	struct EventEntry {
		UA_MonitoredItemCreateResult result;
		UA_EventFilter filter;
	};

	// internal registry to store the root-node ID for activated events
	std::map<UA_UInt32, EventEntry> eventRegistry;

	// entityUpdate handling
	mutable std::mutex entityUpdateMutex;
	// entity update registry (key=EntityName, value=CondVar)
	std::map<std::string, std::condition_variable> entityUpdateSignalRegistry;
	// entity update value registry (key=EntityName, value=UpdateValue)
	std::map<std::string, OPCUA::Variant> entityUpdateValueRegistry;

	/// simple discovery service checks if the given address has any endpoints (not really needed)
	bool hasEndpoints(const std::string &address, const bool &display=true);

	/** method for finding an element by browsing a path within the server space
	 *
	 *  This method internally uses the OPC UA Browsing service to get the NodeId
	 *  of the element specified by the objectPath. The objectPath is assumed to have
	 *  the following structure:
	 *  <ObjectName>/element/subelement/subsubelement/...
	 *  The first segment is assumed to be the object located under the default UA_NS0ID_OBJECTSFOLDER.
	 *  The following segments specify a hierarchy of subelements below the root object.
	 *  Please note, that the individual elements are identified by their BrowseName (i.e. the qualified name)
	 *  and the namespaceIndex (the second, optional attribute).
	 *
	 *  @param objectPath:		the element location within an object specified as a path (linux like path syntax)
	 *
	 *  @return the NodeId of the first matching element
	 *  	- NodeId is set to UA_NODEID_NULL if the element could not be found (use UA_NodeId_isNill(result) to check for nill)
	 */
	NodeId browseObjectPath(const std::string &objectPath) const;

	/** method to find a sub-element of the given parentNodeId
	 *
	 *  This method browses the direct children of the given parentNodeId. Please note that the element
	 *  is identified by its browse-name and only the first match is returned (in case of possible multiple
	 *  matches with the same browse-name).
	 *
	 *  @param elementBrowseName	the browse-name (not the display name) of the element to find
	 *  @param parentNodeId			the parent under which the element is searched
	 *  @param filterNodeType		optional filter for node types (default is a filter for OPC UA variables)
	 *
	 *  @return the NodeId of the first matching element
	 *  	- NodeId is set to NULL if the element could not be found
	 */
	NodeId findElement(const std::string &elementBrowseName, const NodeId &parentNodeId, const UA_NodeClass &filterNodeType=UA_NODECLASS_VARIABLE) const;

	/** this helper method is used to check if the client is connected and an entity with the provided name is available in the server object
	 *
	 *  The method implements the generic check that is is used from within the methods getEntity(), setEntity() and callMethod()
	 *  to assure that the client is connected and that the requested entity is actually existing in the server space.
	 *  In case the derived client has implemented the method createClientSpace() properly, then this method
	 *  uses the internal entityRegistry instead of accessing the server remotely (to make this check a lot faster).
	 *
	 *  @param entityBrowseName the entoty's browse name
	 *  @param entityId the output argument where the matched entity ID is returned
	 *  @param filterNodeType an optional filter (default value is a OPC UA Variable)
	 */
	OPCUA::StatusCode checkAndGetEntityId(const std::string &entityBrowseName, NodeId &entityId, const UA_NodeClass &filterNodeType=UA_NODECLASS_VARIABLE) const;

	// these two helper methods might be useful to create fast access to OPC UA variables (not yet tested)
	UA_StatusCode registerNode(const NodeId &nodeId);
	UA_StatusCode unregisterNode(const NodeId &nodeId);

	// these two helper methods should be used to implement the client async upcalls (not yet fully tested)
	UA_StatusCode createSubscription(const std::string &entityBrowseName, const std::chrono::steady_clock::duration &subscriptionInterval = std::chrono::milliseconds(100) );
	UA_StatusCode deleteSubscription(const std::string &entityBrowseName);

	/// internal upcall called from within the internal OPC UA upcall method, it propagates the call to the 
	void handleEntity(const UA_UInt32 &subscriptionId, UA_DataValue *data);

	// internal upcall to handle incoming events
	void handleEventUpcall(const UA_UInt32 &eventId, const size_t &nEventFields, UA_Variant *eventFields);
#endif

protected:
	/** this method creates a fast-access registry to an OPC UA variable identified by its browse-name
	 *
	 *  This method searches for an OPC UA variable within the server space, and if found:
	 *  (i) creates a registry entry to speed-up read/write access and (ii) sets up a
	 *  subscription infrastructure to get notified about incoming value updates for the given variable.
	 *  This method is supposed to be used from within the custom implementation of the createClientSpace()
	 *  method.
	 *
	 *  @param variableBrowseName is the browse-name of the OPC UA variable within the server space
	 *  @param activateUpdateUpcall optional attribute allows activation/deactivation of the upcall interface for data-updates
	 *  @param samplingInterval optional internal sampling interval for checking of the value-updates (default interval is 100 ms)
	 *  @return true on SUCCESS or false on ERROR
	 *
	 *  @sa createClientSpace()
	 */
	bool addVariableNode(const std::string &variableBrowseName, const bool activateUpdateUpcall=true, const std::chrono::steady_clock::duration &samplingInterval = std::chrono::milliseconds(100) );

	/** this method creates a fast-access registry to an OPC UA method identified by its browse-name
	 *
	 *  This method searches for an OPC UA method within the server space, and if found creates a registry
	 *  entry to speed-up calling access. This method is supposed to be used from within the custom
	 *  implementation of the createClientSpace() method.
	 *
	 *  @param methodBrowseName is the browse-name of the OPC UA method within the server space
	 *  @return true on SUCCESS or false on ERROR
	 *
	 *  @sa createClientSpace()
	 */
	bool addMethodNode(const std::string &methodBrowseName);

	/** this method shall be implemented in derived classes to create the client-space (i.e. to fill the entityRegistry)
	 *
	 *  Please overload and implement this method in derived classes to specify the client's specific variables.
	 *  You can use the helper method findElement() to get the OPC UA node ids for the object and its variables.
	 *  The mapping of variable's browse name and node-id should be stored within the entityRegistry.
	 *  An example implementation is like this:
	 *
	 *	// for each entity create a registry entry like this:
	 *	if(this->addVariableNode("MyVariable") != true) {
	 *		return false;
	 *	}
	 *	// for each method create a registry entry like this:
	 *	if(this->addMethodNode("MyMethod") != true) {
	 *		return false;
	 *	}
	 *
	 *
	 *  @param activateUpcalls parameter that is propagated from the connect() method to indicate whether the the upcall interface should be activated or not (pass this value to the addMethodNode() method)
	 *  @return true if everything worked well, or false in case of problems
	 *
	 *  @sa addVariableNode()
	 *  @sa addMethodNode()
	 */
    };
} /* namespace OPCUA */
#endif /* OPCUAGENERICCLIENT_HH_ */