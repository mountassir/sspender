/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Device.cpp: Part of sspender
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

#include "Device.h"

Device::Device(const string &deviceName, bool suspendIfIdle)
{
	m_deviceName = deviceName;
	m_currentUsage.load = 0;
	m_watchDog.reset(new WatchDog(true));
	m_initialized = false;
	m_deviceIsIdle = false;
	m_shouldSuspendIfIdle = suspendIfIdle;

	resetUsage();
}

Device::~Device()
{
	m_watchDog->shouldStillMonitor(false);
}

//this is a heart beat that will be called the detached thread,
//when this object is out of scope, the detached thread will terminate
bool Device::shouldStillMonitor()
{
	return m_watchDog->shouldStillMonitor();
}

//this is a kill switch for the detached thread that will be monitoring this device
void Device::setMonitoringState(bool monitoringState)
{
	m_watchDog->shouldStillMonitor(monitoringState);
}

//get a pointer this device's watch dog, will be used by the detached thread
shared_ptr<WatchDog> Device::getWatchDogCopy()
{
	return m_watchDog;
}

bool Device::shouldSuspendIfIdle()
{
	return m_shouldSuspendIfIdle;
}

string& Device::getDeviceName()
{
	return m_deviceName;
}

bool Device::isDeviceInitialized()
{
	return m_initialized;
}

void Device::isDeviceInitialized(bool initialized)
{
	m_initialized = initialized;
}

void Device::getCurrentUsage(DeviceUsage *deviceUsage)
{
	std::lock_guard<mutex> locker(m_mutex);

	copyDeviceUsage(m_currentUsage, deviceUsage);
}

void Device::getAvrgUsage(DeviceUsage *deviceUsage)
{
	std::lock_guard<mutex> locker(m_mutex);

	copyDeviceUsage(m_avrgUsage, deviceUsage);
}

bool Device::getIdleState()
{
	std::lock_guard<mutex> locker(m_mutex);

	return m_deviceIsIdle;
}

void Device::setIdleState(bool state)
{
	std::lock_guard<mutex> locker(m_mutex);

	m_deviceIsIdle = state;
}

void Device::copyDeviceUsage(const DeviceUsage &input, DeviceUsage *output)
{
	output->load = input.load;
	output->totalRead = input.totalRead;
	output->totalWritten = input.totalWritten;
}

void Device::resetUsage()
{
	resetUsage(&m_avrgUsage);
	resetUsage(&m_currentUsage);
}

void Device::resetUsage(DeviceUsage *deviceUsage)
{
	std::lock_guard<mutex> locker(m_mutex);

	deviceUsage->reset();
}

void Device::setUsage(const DeviceUsage &deviceUsage)
{
	updateAverageUsage(deviceUsage);

	std::lock_guard<mutex> locker(m_mutex);

	m_currentUsage = deviceUsage;
}

void Device::updateAverageUsage(const DeviceUsage &deviceUsage)
{
	std::lock_guard<mutex> locker(m_mutex);

	m_avrgUsage.load         = updateAverageValue(m_avrgUsage.load,         deviceUsage.load);
	m_avrgUsage.totalRead    = updateAverageValue(m_avrgUsage.totalRead,    deviceUsage.totalRead);
	m_avrgUsage.totalWritten = updateAverageValue(m_avrgUsage.totalWritten, deviceUsage.totalWritten);
}

double Device::updateAverageValue(double currentAverageValue, double currentValue)
{
	double newAverageValue =0 ;

	if(currentAverageValue == 0)
	{
		//if the current average value is = 0 (got the first value),
		//average is the new value.
		newAverageValue = currentValue;
	}
	else
	{
		//if the current average value is != 0 (already have an average),
		//average is (oldAverage + newValue) / 2
		newAverageValue += (currentAverageValue + currentValue) / 2;
	}

	return newAverageValue;
}

void Device::monitorDeviceUsage(Device *deviceToMonitor, shared_ptr<WatchDog> watchDog)
{
	//we only need to open the file once
	ifstream statesFile (deviceToMonitor->getStatesFileName());

	auto startTime = Clock::now();

	//while the device object is still in scope
	//call it's functions to calculate and update the usage
	while(watchDog->shouldStillMonitor())
	{
		DeviceUsage diskUsage = {0, 0, 0};

		deviceToMonitor->calculateUsage(statesFile, &diskUsage);

		deviceToMonitor->setUsage(diskUsage);

		if(diskUsage.load > 10)
		{
			deviceToMonitor->setIdle(false);
			startTime = Clock::now();
		}
		else
		{
			double duration = getMinutesDuration(startTime);

			if(duration > 1)
			{
				deviceToMonitor->setIdle(true);
			}
		}
	}

	statesFile.close();
}
