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

/*
 * This class will create and initialize all the devices that
 * need to be monitored, this can also be called to check if
 * the system is idle, cpu load, storage load...
 */
class Monitor
{
private:
	vector<Disk*> m_disksToMonitor;  //all disks that need to be monitored
	vector<Cpu*> m_cpusToMonitor;    //all the cpus that need to be monitored

public:
	Monitor();
	~Monitor();

	//get the current cpu load
	void getCpuLoad(double *cpuUsage);

	//get the storage load
	void getStorageLoad(double *storageLoad, double *storageRead, double *storageWritten);

	//check if any of the clients is online
	bool areClientsConnected(const vector<string> &clients);

	//ping and check if this ip is online
	bool isIpAddressOnline(const string &ipAddress);

	//start monitoring all the following devices
	void monitorSystemUsage(const vector<DiskCfg> &disks,
                               const vector<CpuCfg> &cpus);

	void printTheMachineUsage();

	bool isTheMachineIdle();
};

#endif
