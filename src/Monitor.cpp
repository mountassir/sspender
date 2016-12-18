/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Monitor.cpp: Part of sspender
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
		int numberOfMonitoredDevices = 0;

		for(size_t i = 0; i < numberOfDevices; ++i)
		{
			if(devices[i]->shouldMonitorUsage())
			{
				DeviceUsage deviceUsage = {0, 0, 0};

				devices[i]->getAvrgUsage(&deviceUsage);

				totalUsage->load         += deviceUsage.load;
				totalUsage->totalRead    += deviceUsage.totalRead;
				totalUsage->totalWritten += deviceUsage.totalWritten;

				numberOfMonitoredDevices++;
			}
		}

		averageUsage->load         += totalUsage->load / numberOfMonitoredDevices;
		averageUsage->totalRead    += totalUsage->totalRead / numberOfMonitoredDevices;
		averageUsage->totalWritten += totalUsage->totalWritten / numberOfMonitoredDevices;
	}

	template <typename T>
	inline void sumDevicesUsage (const T &devices)
	{
		devices->resetUsage();
	}
}

Monitor::Monitor()
{

}

Monitor::~Monitor()
{
	if(m_disksToMonitor.size() > 0)
	{
		delete[] &m_disksToMonitor;
	}

	if(m_cpusToMonitor.size() > 0)
	{
		delete[] &m_cpusToMonitor;
	}
}

void Monitor::monitorSystemUsage(const vector<DiskCfg> &disks,
		                             const vector<CpuCfg> &cpus)
{
	for(size_t i = 0, len = disks.size(); i < len; ++i)
	{
		Disk *newDisk= new Disk(disks[i].diskName,
								disks[i].diskUUID,
								disks[i].idle_time_threshold,
								disks[i].idle_load_threshold,
								disks[i].spinDown,
								disks[i].suspendIfIdle);

		newDisk->monitorUsage();

		m_disksToMonitor.push_back(newDisk);
	}

	for(size_t i = 0, len = cpus.size(); i < len; ++i)
	{
		Cpu *newCpu= new Cpu(cpus[i].cpuName,
				             cpus[i].idle_time_threshold,
							 cpus[i].idle_load_threshold,
							 cpus[i].suspendIfIdle);

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

void Monitor::printTheMachineUsage()
{
	for(size_t i = 0, len = m_cpusToMonitor.size(); i < len; ++i)
	{
		cout << *(m_cpusToMonitor[i]) << "\n";
	}

	for(size_t i = 0, len = m_disksToMonitor.size(); i < len; ++i)
	{
		cout << *(m_disksToMonitor[i]) << "\n";
	}
}

bool Monitor::isTheMachineIdle()
{
	for(size_t i = 0, len = m_cpusToMonitor.size(); i < len; ++i)
	{
		if(m_cpusToMonitor[i]->shouldSuspendIfIdle() && !m_cpusToMonitor[i]->getIdleState())
		{
			return false;
		}
	}

	for(size_t i = 0, len = m_disksToMonitor.size(); i < len; ++i)
	{
		if(m_disksToMonitor[i]->shouldSuspendIfIdle() && !m_disksToMonitor[i]->getIdleState())
		{
			return false;
		}
	}

	return true;
}

bool Monitor::canBeSuspended()
{
	for(size_t i = 0, len = m_cpusToMonitor.size(); i < len; ++i)
	{
		if(m_cpusToMonitor[i]->shouldSuspendIfIdle())
		{
			return true;
		}
	}

	for(size_t i = 0, len = m_disksToMonitor.size(); i < len; ++i)
	{
		if(m_disksToMonitor[i]->shouldSuspendIfIdle())
		{
			return true;
		}
	}

	return false;
}

bool Monitor::areClientsConnected(const vector<string> &clients)
{
	bool isAnyClientOnline = false;

	for(size_t i = 0, len = clients.size(); i < len; ++i)
	{
		if(isIpAddressOnline(clients[i]))
		{
			cout << "IP " << clients[i] << " is online." << "\n";

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
			cout << ipAddress << " is online." << "\n";
			return true;
		}
	}

	cout << ipAddress << " is not online." << "\n";

	return false;
}
