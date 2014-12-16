/*
 * Copyright (C) 2014 Project Hatohol
 *
 * This file is part of Hatohol.
 *
 * Hatohol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License, version 3
 * as published by the Free Software Foundation.
 *
 * Hatohol is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Hatohol. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef ZabbixAPI_h
#define ZabbixAPI_h

#include <string>
#include <libsoup/soup.h>
#include "Monitoring.h"
#include "MonitoringServerInfo.h"
#include "ItemTablePtr.h"
#include "JSONBuilder.h"
#include "JSONParser.h"
#include "HatoholError.h"


class ZabbixAPI
{
public:
	typedef enum {
		VALUE_TYPE_UNKNOWN = -1,
		VALUE_TYPE_FLOAT   = 0,
		VALUE_TYPE_STRING  = 1,
		VALUE_TYPE_LOG     = 2,
		VALUE_TYPE_INTEGER = 3,
		VALUE_TYPE_TEXT    = 4
	} ValueType;

	ZabbixAPI(void);
	virtual ~ZabbixAPI();

	static const uint64_t EVENT_ID_NOT_FOUND;

	static ItemInfoValueType toItemValueType(
	  const ZabbixAPI::ValueType &valueType);
	static ZabbixAPI::ValueType fromItemValueType(
	  const ItemInfoValueType &valueType);

protected:
	const static uint64_t UNLIMITED = -1;
	void setMonitoringServerInfo(const MonitoringServerInfo &serverInfo);

	/**
	 * Called when the authtoken is updated.
	 */
	virtual void onUpdatedAuthToken(const std::string &authToken);

	/**
	 * Get the API version of the target ZABBIX server.
	 * Note that this method is NOT MT-safe.
	 *
	 * @retrun An API version.
	 */
	const std::string &getAPIVersion(void);

	/**
	 * Compare the API version of the target Zabbix server
	 * with the specified version.
	 *
	 * @param major A major version. Ex. 1 if version is 1.2.3.
	 * @param minor A minor version. Ex. 2 if version is 1.2.3.
	 * @param micro A micro version. Ex. 3 if version is 1.2.3.
	 *
	 * @return
	 * true if the API version of the server is equal to or greater than
	 * that of specified version. Otherwise false is returned.
	 */
	bool checkAPIVersion(int major, int minor, int micro);

	/**
	 * Open a session with with Zabbix API server
	 *
	 * @param msgPtr
	 * An address of a SoupMessage object pointer. If this parameter is
	 * not NULL, a SoupMessage object pointer is copied to this parameter.
	 * Otherwise, the object is freeed internally. And the parameter is
	 * not changed.
	 *
	 * @return
	 * true if a session is oppned successfully. Otherwise, false is
	 * returned.
	 */
	bool openSession(SoupMessage **msgPtr = NULL);

	SoupSession *getSession(void);
	bool updateAuthTokenIfNeeded(void);
	std::string getAuthToken(void);
	void clearAuthToken(void);

	/**
	 * Get the triggers.
	 *
	 * @param requestSince
	 * Triggers with timestamp after this parameter will be returned.
	 *
	 * @return The obtained triggers as an ItemTable format.
	 */
	ItemTablePtr getTrigger(int requestSince = 0);

	/**
	 * Get the items.
	 *
	 * @return The obtained items as an ItemTable format.
	 */
	ItemTablePtr getItems(void);


	/**
	 * Get the history.
	 *
	 * @return The obtained history as an ItemTable format.
	 */
	ItemTablePtr getHistory(const ItemIdType &itemId,
				const ZabbixAPI::ValueType &valueType,
				const time_t &beginTime,
				const time_t &endTime);

	/**
	 * Get the hosts and the host groups.
	 *
	 * @param hostsTablePtr
	 * A ItemTablePtr the obtained hosts are stored in.
	 *
	 * @param hostsGroupsTablePtr
	 * A ItemTablePtr the obtained host groups are stored in.
	 */
	void getHosts(ItemTablePtr &hostsTablePtr,
	              ItemTablePtr &hostsGroupsTablePtr);

	/**
	 * Get the groups.
	 *
	 * @param groupsTablePtr
	 * A ItemTablePtr the obtained groups are stored in.
	 */
	void getGroups(ItemTablePtr &groupsTablePtr);

	/**
	 * Get the applications
	 *
	 * @param appIdVector
	 * A vector filled with required application IDs.
	 *
	 * @return The obtained triggers as an ItemTable format.
	 */
	ItemTablePtr getApplications(const std::vector<uint64_t> &appIdVector);
	ItemTablePtr getApplications(ItemTablePtr items);

	/**
	 * Get the applications
	 *
	 * @param eventIdFrom
	 * The first event ID to be obtained.
	 *
	 * @param eventIdTill
	 * The last event ID to be obtained.
	 *
	 * @return The obtained events as an ItemTable format.
	 */
	ItemTablePtr getEvents(uint64_t eventIdFrom, uint64_t eventIdTill);

	/**
	 * Get the first or last event ID the target Zabbix server has.
	 *
	 * @param isFirst
	 * true if you want first event ID.
	 * false if you want last event ID.
	 *
	 * @return The first or last event ID.
	 */
	uint64_t getEndEventId(const bool &isFirst);

	/**
	 * Get the triggers.
	 *
	 * @param requestSince
	 * Triggers with timestamp after this parameter will be returned.
	 *
	 * @return
	 * A SoupMessage object with the raw Zabbix servers's response.
	 */
	SoupMessage *queryTrigger(HatoholError &queryRet, int requestSince = 0);

	/**
	 * Get the items.
	 *
	 * @return
	 * A SoupMessage object with the raw Zabbix servers's response.
	 */
	SoupMessage *queryItem(HatoholError &queryRet);

	/**
	 * Get the history.
	 *
	 * @return
	 * A SoupMessage object with the raw Zabbix servers's response.
	 */
	SoupMessage *queryHistory(HatoholError &queryRet,
				  const ItemIdType &itemId,
				  const ZabbixAPI::ValueType &valueType,
				  const time_t &beginTime,
				  const time_t &endTime);

	/**
	 * Get the hosts.
	 *
	 * @return
	 * A SoupMessage object with the raw Zabbix servers's response.
	 */
	SoupMessage *queryHost(HatoholError &queryRet);

	/**
	 * Get the groups.
	 *
	 * @return
	 * A SoupMessage object with the raw Zabbix servers's response.
	 */
	SoupMessage *queryGroup(HatoholError &queryRet);

	/**
	 * Get the applications.
	 *
	 * @return
	 * A SoupMessage object with the raw Zabbix servers's response.
	 */
	SoupMessage *queryApplication(const std::vector<uint64_t> &appIdVector,
				      HatoholError &queryRet);

	/**
	 * Get the events.
	 *
	 * @return
	 * A SoupMessage object with the raw Zabbix servers's response.
	 */
	SoupMessage *queryEvent(uint64_t eventIdFrom, uint64_t eventIdTill,
				HatoholError &queryRet);

	/**
	 * Get the first or last event ID the target Zabbix server has.
	 *
	 * @param isFirst
	 * true if you want first event ID.
	 * false if you want last event ID.
	 *
	 * @return
	 * A SoupMessage object with the raw Zabbix servers's response.
	 */
	SoupMessage *queryEndEventId(const bool &isFirst, HatoholError &queryRet);

	/**
	 * Get the functions.
	 * Actually, the body of 'functions' is objtained in the prior call of
	 * getTrigger(). So the caller must be call getTrigger() before this
	 * method.
	 *
	 * @return The obtained functions as an ItemTable format.
	 */
	ItemTablePtr getFunctions(void);

	SoupMessage *queryCommon(JSONBuilder &agent, HatoholError &queryRet);
	SoupMessage *queryAPIVersion(HatoholError &queryRet);
	std::string getInitialJSONRequest(void);
	bool parseInitialResponse(SoupMessage *msg);
	void startObject(JSONParser &parser, const std::string &name);
	void startElement(JSONParser &parser, const int &index);

	void getString(JSONParser &parser, const std::string &name,
	               std::string &value);
	int pushInt(JSONParser &parser, ItemGroup *itemGroup,
	            const std::string &name, const ItemId &itemId);
	uint64_t pushUint64(JSONParser &parser, ItemGroup *itemGroup,
	                    const std::string &name, const ItemId &itemId);
	std::string pushString(JSONParser &parser, ItemGroup *itemGroup,
	                       const std::string &name, const ItemId &itemId);
	void parseAndPushTriggerData(
	  JSONParser &parser,
	  VariableItemTablePtr &tablePtr, const int &index);
	void parseAndPushItemsData(
	  JSONParser &parser,
	  VariableItemTablePtr &tablePtr, const int &index);
	void parseAndPushHostsData(
	  JSONParser &parser,
	  VariableItemTablePtr &tablePtr, const int &index);
	void parseAndPushHostsGroupsData(
	  JSONParser &parser,
	  VariableItemTablePtr &tablePtr, const int &index);
	void parseAndPushGroupsData(
	  JSONParser &parser,
	  VariableItemTablePtr &tablePtr, const int &index);
	void parseAndPushApplicationsData(
	  JSONParser &parser,
	  VariableItemTablePtr &tablePtr, const int &index);
	void parseAndPushEventsData(
	  JSONParser &parser,
	  VariableItemTablePtr &tablePtr, const int &index);

	void pushTriggersHostid(JSONParser &parser, ItemGroup *itemGroup);
	void pushApplicationid(JSONParser &parser, ItemGroup *itemGroup);

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};

#endif // ZabbixAPI_h
