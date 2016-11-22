/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Device.h: Part of sspender
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

/*
 * This class represents a single device that will be monitored,
 * stores, updates and exports all available statistics;
 */
class Device {
private:
	mutex m_mutex;                            //mutex to make reads/writes atomic
	shared_ptr<WatchDog> m_watchDog;          //used to shutdown detached threads
	string m_deviceName;                      //name of this device
	DeviceUsage m_currentUsage, m_avrgUsage;  //current and average usage
	bool m_initialized;                       //to check if the device is initialized
	bool m_shouldSuspendIfIdle;               //if true, the machine will only suspend if this device
	                                          //(and any other device where this is true) is idle

public:
	Device(const string &deviceName, bool suspendIfIdle);
	virtual ~Device();

	//getters
	void getCurrentUsage(DeviceUsage *deviceUsage);
	void getAvrgUsage(DeviceUsage *deviceUsage);

	//setters
	void setUsage(const DeviceUsage &deviceUsage);

	//should the machine be suspended if this device is idle(and any other monitored devices)
	bool shouldSuspendIfIdle();

	//this will reset both current and average usage to 0
	void resetUsage();

	/* defined by derived classes */
	//initialize this device
	virtual void initDevice() = 0;

	//start monitoring this device's usage
	virtual void monitorUsage() = 0;

	//returns the file containing the stats information
	virtual string getStatesFileName() = 0;

	//reads the stats from the file and calculate this device's usage
	virtual void calculateUsage(ifstream &statesFile, DeviceUsage *deviceUsage) = 0;

	virtual bool shouldMonitorUsage() = 0;

protected:
	//returns this device's name
	string& getDeviceName();

	//check if the device is initialized
	bool isDeviceInitialized();

	//set initialization state
	void isDeviceInitialized(bool initialized);

	//copy one usage struct to another (pointer)
	void copyDeviceUsage(const DeviceUsage &input, DeviceUsage *output);

	//reset a usage struct
	void resetUsage(DeviceUsage *deviceUsage);

	//update the average usage
	void updateAverageUsage(const DeviceUsage &deviceUsage);

	//calculate the new average usage
	double updateAverageValue(double currentAverageValue, double currentValue);

	//this is a heart beat that will be called the detached thread,
	//when this object is out of scope, the detached thread will terminate
	bool shouldStillMonitor();

	//this is a kill switch for the detached thread that will be monitoring this device
	void setMonitoringState(bool monitoringState);

	//get a pointer this device's watch dog, will be used by the detached thread
	shared_ptr<WatchDog> getWatchDogCopy();
};

#endif
