/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Writer.cpp: Part of sspender
 *
 * sspender is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * sspender is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with sspender.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include <unistd.h>
#include <algorithm>
#include <iomanip>

#include "Monitor.h"
#include "constants.h"
#include "utils.h"

using namespace std;

namespace
{
	bool sortVector(int i,int j)
	{
		return (i<j);
	}
}

class Manager
{
private:
	Monitor m_monitor;
	vector<string> m_ipsToWatch;
	vector<string> m_disksToMonitor;
	vector<string> m_cpusToMonitor;
	vector<string> m_timesToWakeAt;
	SLEEP_MODE m_sleepMode;
	int m_checkIfIdleEvery, m_stopMonitoringFor, m_resetMonitoringAfter, m_suspendAfter;
	bool m_suspendIfCpuIdle, m_suspendIfStorageIdle;

public:
	Manager() : m_sleepMode(DISK),
	            m_checkIfIdleEvery(CHECK_IF_IDLE_EVERY),
	            m_stopMonitoringFor(STOP_MONITORING_FOR),
	            m_resetMonitoringAfter(RESET_MONITORING_IF_BUSY_FOR),
	            m_suspendAfter(SUSPEND_AFTER),
	            m_suspendIfCpuIdle(true), m_suspendIfStorageIdle(true) { };

	void watchSystem();
	void setWhatToMonitor(bool suspendIfCpuIdle, bool suspendIfStorageIdle);
	void setIpsToWatch(const vector<string> &ipToWatch);
	void setDisksToMonitor(const vector<string> &disksToMonitor);
	void setCpusToMonitor();
	void setTimesToWakeAt(const vector<string> &wakeAt);
	void setSleepMode(SLEEP_MODE sleepMode);
	void setTimers(int check_if_idle_every,
				   int stop_monitoring_for,
				   int reset_monitoring_after,
				   int suspend_after);

protected:
	bool isSystemIdle();
	void suspendServer();
	void suspendUntil(double currentTimeInMinutes, double until);
	void rtcWakeSuspend(double secondsToSleep, vector<string> *output);
	void pmUtilSuspend(double secondsToSleep, vector<string> *output);
	string getSleepMode();
	string getPmUtilCommand();
};

#endif
