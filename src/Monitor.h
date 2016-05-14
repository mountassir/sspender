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

class Monitor
{
private:
	vector<Disk*> m_disksToMonitor;
	vector<Cpu*> m_cpusToMonitor;

public:
	Monitor();
	~Monitor();
	void getCpuLoad(double *cpuUsage);
	void getStorageLoad(double *storageLoad, double *storageRead, double *storageWritten);
	bool areClientsConnected(const vector<string> &clients);
	bool isIpAddressOnline(const string &ipAddress);
	void monitorSystemUsage(const vector<string> &disks,
                            const vector<string> &nics);
};

#endif
