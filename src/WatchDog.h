#ifndef SYSTEMUSAGE_H_
#define SYSTEMUSAGE_H_

#include <mutex>

using namespace std;

class WatchDog
{
private:
	mutex m_mutex;
	bool   m_souldStillMonitor;
	double m_cpuUsage;
	double m_storageUsage;

public:
	WatchDog(bool shouldMonitor) : m_souldStillMonitor(shouldMonitor), m_cpuUsage(0), m_storageUsage(0) {};
	void shouldStillMonitor(bool keppMonitoring);
	bool shouldStillMonitor();
};

#endif
