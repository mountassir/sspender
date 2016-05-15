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

#ifndef SYSTEMUSAGE_H_
#define SYSTEMUSAGE_H_

#include <mutex>

using namespace std;

class WatchDog
{
private:
	mutex m_mutex;
	bool   m_souldStillMonitor;
	double m_cpuUsage;
	double m_storageUsage;

public:
	WatchDog(bool shouldMonitor) : m_souldStillMonitor(shouldMonitor), m_cpuUsage(0), m_storageUsage(0) {};
	void shouldStillMonitor(bool keppMonitoring);
	bool shouldStillMonitor();
};

#endif
