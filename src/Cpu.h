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

#ifndef CPU_H_
#define CPU_H_

#include "Device.h"
#include "utils.h"

using namespace std;

class Cpu : public Device {

private:

public:
	Cpu(const string &cpuName) :
		Device(cpuName, true) { };

	virtual void initDevice();
	virtual void monitorUsage();
	virtual string getStatesFileName();
	virtual void calculateUsage(ifstream &statesFile, DeviceUsage *cpukUsage);

protected:
	void sumCpuJiffies(const vector<string> &fileOutput, int *workJiffies, int *totalJiffies);
};

#endif
