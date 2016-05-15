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

#include "Monitor.h"

namespace
{
	template <typename T>
	inline void sumDevicesUsage (const vector<T> &devices,
			                     DeviceUsage *totalUsage,
			                     DeviceUsage *averageUsage)
	{
		size_t numberOfDevices = devices.size();

		for(size_t i = 0; i < numberOfDevices; ++i)
		{
			DeviceUsage deviceUsage = {0, 0, 0};

			devices[i]->getAvrgUsage(&deviceUsage);
			devices[i]->resetUsage();

			totalUsage->load         += deviceUsage.load;
			totalUsage->totalRead    += deviceUsage.totalRead;
			totalUsage->totalWritten += deviceUsage.totalWritten;
		}

		averageUsage->load         += totalUsage->load / numberOfDevices;
		averageUsage->totalRead    += totalUsage->totalRead / numberOfDevices;
		averageUsage->totalWritten += totalUsage->totalWritten / numberOfDevices;
	}
}

Monitor::Monitor()
{

}

Monitor::~Monitor()
{
	delete[] &m_disksToMonitor;

	delete[] &m_cpusToMonitor;
}

void Monitor::monitorSystemUsage(const vector<string> &disks,
		                         const vector<string> &cpus)
{
	for(size_t i = 0, len = disks.size(); i < len; ++i)
	{
		Disk *newDisk= new Disk(disks[i]);

		newDisk->monitorUsage();

		m_disksToMonitor.push_back(newDisk);
	}

	for(size_t i = 0, len = cpus.size(); i < len; ++i)
	{
		Cpu *newCpu= new Cpu(cpus[i]);

		newCpu->monitorUsage();

		m_cpusToMonitor.push_back(newCpu);
	}
}

void Monitor::getCpuLoad(double *cpuUsage)
{
	DeviceUsage totalUsage = {0, 0, 0};
	DeviceUsage averageUsage = {0, 0, 0};

	sumDevicesUsage(m_cpusToMonitor, &totalUsage, &averageUsage);

	*cpuUsage = roundValue(averageUsage.load);
}

void Monitor::getStorageLoad(double *storageLoad, double *storageRead, double *storageWritten)
{
	DeviceUsage totalUsage = {0, 0, 0};
	DeviceUsage averageUsage = {0, 0, 0};

	sumDevicesUsage(m_disksToMonitor, &totalUsage, &averageUsage);

	*storageLoad    = roundValue(averageUsage.load);
	*storageRead    = roundValue(averageUsage.totalRead);
	*storageWritten = roundValue(averageUsage.totalWritten);
}

bool Monitor::areClientsConnected(const vector<string> &clients)
{
	bool isAnyClientOnline = false;

	for(size_t i = 0, len = clients.size(); i < len; ++i)
	{
		if(isIpAddressOnline(clients[i]))
		{
			cout << "IP " << clients[i] << " is online." << endl;

			isAnyClientOnline = true;
		}
	}

	return isAnyClientOnline;
}

bool Monitor::isIpAddressOnline(const string &ipAddress)
{
	vector<string> pingOutput;
	runSystemCommand("ping -c 1 -W 1 " + ipAddress, &pingOutput);

	for(size_t i = 0, len = pingOutput.size(); i < len; ++i)
	{
		size_t found = pingOutput[i].find(PING_IDENTIFIER, 0);

		if (found != string::npos)
		{
			cout << ipAddress << " is online." << endl;
			return true;
		}
	}

	cout << ipAddress << " is not online." << endl;

	return false;
}
