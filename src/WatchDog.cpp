#include "WatchDog.h"

void WatchDog::shouldStillMonitor(bool keppMonitoring)
{
	std::lock_guard<mutex> locker(m_mutex);

	m_souldStillMonitor = keppMonitoring;
}

bool WatchDog::shouldStillMonitor()
{
	std::lock_guard<mutex> locker(m_mutex);

	return m_souldStillMonitor;
}

