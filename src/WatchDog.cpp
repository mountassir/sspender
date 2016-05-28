/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * WatchDog.cpp: Part of sspender
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

