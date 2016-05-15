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

#ifndef DEVICE_H_
#define DEVICE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <memory>

#include "WatchDog.h"
#include "constants.h"
#include "utils.h"

using namespace std;

struct DeviceUsage
{
	double load;
	double totalRead;
	double totalWritten;
};

class Device {
private:
	mutex m_mutex;
	shared_ptr<WatchDog> m_watchDog;
	string m_deviceName;
	DeviceUsage m_currentUsage, m_avrgUsage;
	bool m_initialized;

public:
	Device(const string &deviceName);
	virtual ~Device();

	void getCurrentUsage(DeviceUsage *deviceUsage);
	void getAvrgUsage(DeviceUsage *deviceUsage);
	void resetUsage();
	void setUsage(const DeviceUsage &deviceUsage);

	//defined by derived classes
	virtual void initDevice() = 0;
	virtual void monitorUsage() = 0;
	virtual string getStatesFileName() = 0;
	virtual void calculateUsage(ifstream &statesFile, DeviceUsage *deviceUsage) = 0;

protected:
	string& getDeviceName();
	bool isDeviceInitialized();
	void isDeviceInitialized(bool initialized);
	bool shouldStillMonitor();
	void setMonitoringState(bool monitoringState);
	void copyDeviceUsage(const DeviceUsage &input, DeviceUsage *output);
	void resetUsage(DeviceUsage *deviceUsage);
	double updateAverageValue(double currentAverageValue, double currentValue);
	void updateAverageUsage(const DeviceUsage &deviceUsage);
	shared_ptr<WatchDog> getWatchDogCopy();
};

#endif
