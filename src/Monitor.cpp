#include "Monitor.h"

namespace
{
	template <typename T>
	inline double sumDevicesUsage (const vector<T> &devices)
	{
		double totalUsage = 0;

		for(size_t i = 0, len = devices.size(); i < len; ++i)
		{
			DeviceUsage deviceUsage (0, 0, 0);

			totalUsage += devices[i]->getAvrgUsage();
			devices[i]->resetUsage();
		}

		return totalUsage;
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

double Monitor::getCpuLoad()
{
	double totalDiskUsage = sumDevicesUsage(m_cpusToMonitor);

	return totalDiskUsage;
}

double Monitor::getStorageLoad()
{
	double totalCpuUsage = sumDevicesUsage(m_disksToMonitor);

	return totalCpuUsage;
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
