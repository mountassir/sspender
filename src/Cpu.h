/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Cpu.h: Part of sspender
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

#ifndef CPU_H_
#define CPU_H_

#include "Device.h"
#include "utils.h"
#include "WatchDog.h"

using namespace std;

/*
 * This class represents a single cpu that will be monitored,
 * stores, updates and exports all available statistics.
 * This extends the class Device
 */
class Cpu : public Device {

private:

public:
	Cpu(const string &cpuName, int idleTimeThreshold, int idleLoadThreshold) :
		Device(cpuName, idleTimeThreshold, idleLoadThreshold, true) { };

	//initialize this cpu
	virtual void initDevice();

	//start monitoring this cpu's usage
	virtual void monitorUsage();

	//reads the stats from the file and calculate this cpu's usage
	virtual void calculateUsage(ifstream &statesFile, DeviceUsage *cpukUsage);

	//returns the file containing the stats information for this cpu
	virtual string getStatesFileName();

	virtual bool shouldMonitorUsage();

	virtual void setIdle(bool state);

protected:
	//sum the jiffies to get the cpu load
	void sumCpuJiffies(const vector<string> &fileOutput, int *workJiffies, int *totalJiffies);
};

ostream & operator<<(ostream &os, Cpu &disk);

#endif
