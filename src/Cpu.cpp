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

#include "Cpu.h"

namespace
{
	void monitorCpuUsage(Cpu *deviceToMonitor, shared_ptr<WatchDog> watchDog)
	{
		ifstream statesFile (deviceToMonitor->getStatesFileName());

		while(watchDog->shouldStillMonitor())
		{
			DeviceUsage cpuUsage = {0, 0, 0};

			deviceToMonitor->calculateUsage(statesFile, &cpuUsage);

			deviceToMonitor->setUsage(cpuUsage);
		}

		statesFile.close();
	}
}

void Cpu::monitorUsage()
{
	if(!isDeviceInitialized())
	{
		initDevice();
	}

	std::thread cpuMonitorThread (monitorCpuUsage, this, getWatchDogCopy());
	cpuMonitorThread.detach();
}

void Cpu::calculateUsage(ifstream &statesFile, DeviceUsage *cpuUsage)
{
	vector<string> fileOutput1;
	parseFile(statesFile, &fileOutput1);

	int workJiffies1, totalJiffies1;

	sumCpuJiffies(fileOutput1, &workJiffies1, &totalJiffies1);

	std::this_thread::sleep_for(std::chrono::milliseconds(MONITORING_THREAD_FREQUENCY));

	vector<string> fileOutput2;
	parseFile(statesFile, &fileOutput2);

	int workJiffies2, totalJiffies2;

	sumCpuJiffies(fileOutput2, &workJiffies2, &totalJiffies2);

	double workOverPeriod = workJiffies2 - workJiffies1;
	double totalOverPeriod = totalJiffies2 - totalJiffies1;

	cpuUsage->load = (workOverPeriod / totalOverPeriod) * 100;
}

string Cpu::getStatesFileName()
{
	stringstream ss;
	ss << "/proc/stat";

	return ss.str();
}

void Cpu::initDevice()
{
	isDeviceInitialized(true);
}

void Cpu::sumCpuJiffies(const vector<string> &fileOutput, int *workJiffies, int *totalJiffies)
{
	size_t len = fileOutput.size();

	for(size_t i = 0; i < len; ++i)
	{
		size_t found = fileOutput[i].find("cpu", 0);

		if ((found != string::npos) && (i < (len - 1)) )
		{
			vector<string> tokens;

			splitByEmptySpace(fileOutput[i], &tokens);

			*workJiffies = 0;
			*totalJiffies = 0;

			for(size_t j = 1; j < tokens.size(); ++j)
			{
				int jiffiValue = atoi(tokens[j].c_str());

				*totalJiffies += jiffiValue;

				if(j < 4)
				{
					*workJiffies += jiffiValue;
				}
			}
		}
	}
}
