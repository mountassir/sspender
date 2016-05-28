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
	m_shouldSuspendIfIdle = suspendIfIdle;

	resetUsage();
}

Device::~Device()
{
	m_watchDog->shouldStillMonitor(false);
}

bool Device::shouldStillMonitor()
{
	return m_watchDog->shouldStillMonitor();
}

void Device::setMonitoringState(bool monitoringState)
{
	m_watchDog->shouldStillMonitor(monitoringState);
}

shared_ptr<WatchDog> Device::getWatchDogCopy()
{
	return m_watchDog;
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

	deviceUsage->totalRead = 0;
	deviceUsage->totalRead = 0;
	deviceUsage->totalWritten = 0;
}

void Device::setUsage(const DeviceUsage &deviceUsage)
{
	std::lock_guard<mutex> locker(m_mutex);

	updateAverageUsage(deviceUsage);

	m_currentUsage = deviceUsage;
}

void Device::updateAverageUsage(const DeviceUsage &deviceUsage)
{
	m_avrgUsage.load = updateAverageValue(m_avrgUsage.load, deviceUsage.load);
	m_avrgUsage.totalRead = updateAverageValue(m_avrgUsage.totalRead, deviceUsage.totalRead);
	m_avrgUsage.totalWritten = updateAverageValue(m_avrgUsage.totalWritten, deviceUsage.totalWritten);
}

double Device::updateAverageValue(double currentAverageValue, double currentValue)
{
	double newAverageValue =0 ;

	if(currentAverageValue == 0)
	{
		newAverageValue = currentValue;
	}
	else
	{
		newAverageValue += (currentAverageValue + currentValue) / 2;
	}

	return newAverageValue;
}
