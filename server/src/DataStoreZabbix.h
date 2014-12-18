/*
 * Copyright (C) 2013 Project Hatohol
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

#ifndef DataStoreZabbix_h
#define DataStoreZabbix_h

#include "ArmZabbixAPI.h"
#include "DataStore.h"

class DataStoreZabbix : public DataStore {
public:
	DataStoreZabbix(const MonitoringServerInfo &serverInfo,
	                const bool &autoStart = true);
	virtual ~DataStoreZabbix();

	virtual ArmBase &getArmBase(void);
	void setCopyOnDemandEnable(bool enable);
private:
	ArmZabbixAPI	m_armApi;
};

#endif // DataStoreZabbix_h
