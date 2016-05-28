/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Monitor.h: Part of sspender
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

#ifndef MONITOR_H_
#define MONITOR_H_

#include <iostream>
#include <fstream>

#include "constants.h"
#include "Device.h"
#include "Disk.h"
#include "Cpu.h"
#include "utils.h"

using namespace std;

class Monitor
{
private:
	vector<Disk*> m_disksToMonitor;
	vector<Cpu*> m_cpusToMonitor;

public:
	Monitor();
	~Monitor();
	void getCpuLoad(double *cpuUsage);
	void getStorageLoad(double *storageLoad, double *storageRead, double *storageWritten);
	bool areClientsConnected(const vector<string> &clients);
	bool isIpAddressOnline(const string &ipAddress);
	void monitorSystemUsage(const vector<DiskCfg> &disks,
                            const vector<string> &cpus);
};

#endif
