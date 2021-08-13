/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Cpu.cpp: Part of sspender
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

void Cpu::initDevice()
{
	isDeviceInitialized(true);
}

//cpu stats are stored in /proc/stat
string Cpu::getStatesFileName()
{
	stringstream ss;
	ss << "/proc/stat";

	return ss.str();
}

//make sure this disk is initialized and spawn a thread
//that will monitor its usage in the background
void Cpu::monitorUsage()
{
	if(!isDeviceInitialized())
	{
		initDevice();
	}

	std::thread cpuMonitorThread (monitorDeviceUsage, this, getWatchDogCopy());

	cpuMonitorThread.detach();
}

bool Cpu::shouldMonitorUsage()
{
	return shouldSuspendIfIdle();
}

void Cpu::setIdle(bool state)
{
	setIdleState(state);
}

void Cpu::calculateUsage(ifstream &statesFile, DeviceUsage *cpuUsage)
{
	vector<string> fileOutput1;
	parseFile(statesFile, &fileOutput1);

	int prevWorkJiffies, prevTotalJiffies;

	//get work and total jiffies
	sumCpuJiffies(fileOutput1, &prevWorkJiffies, &prevTotalJiffies);

	//wait for MONITORING_THREAD_FREQUENCY ms
	std::this_thread::sleep_for(std::chrono::milliseconds(MONITORING_THREAD_FREQUENCY));

	vector<string> fileOutput2;
	parseFile(statesFile, &fileOutput2);

	int newWorkJiffies, newTotalJiffies;

	//get new work and total jiffies
	sumCpuJiffies(fileOutput2, &newWorkJiffies, &newTotalJiffies);

	//get the work and total period
	double workOverPeriod = newWorkJiffies - prevWorkJiffies;
	double totalOverPeriod = newTotalJiffies - prevTotalJiffies;

	//get the percentage of work over total
	cpuUsage->load = (workOverPeriod / totalOverPeriod) * 100;
}

//example stat line in /proc/stat:
//cpu  204776 32504 122058 6943011 4853 20 1402 0 0 0
//    |-------------------|
//          work jiffies
//    |----------------------------------------------|
//                       all jiffies
void Cpu::sumCpuJiffies(const vector<string> &fileOutput, int *workJiffies, int *totalJiffies)
{
	for(size_t i = 0, len = fileOutput.size(); i < len; ++i)
	{
		//find the line that contains the cpu jiffies
		size_t found = fileOutput[i].find("cpu", 0);

		if ((found != string::npos) && (i < (len - 1)) )
		{
			vector<string> tokens;

			splitByEmptySpace(fileOutput[i], &tokens);

			*workJiffies = 0;
			*totalJiffies = 0;

			//will start from 1 as the first element is 'cpu'
			for(size_t j = 1; j < tokens.size(); ++j)
			{
				int jiffiValue = atoi(tokens[j].c_str());

				//everything goes in total
				*totalJiffies += jiffiValue;

				//the first 3 jiffies are work
				if(j <= NUM_OF_CPU_WORK_JIFFIES)
				{
					*workJiffies += jiffiValue;
				}
			}
		}
	}
}

ostream & operator<<(ostream &os, Cpu &cpu)
{
	DeviceUsage deviceUsage = {0, 0, 0};
	cpu.getAvrgUsage(&deviceUsage);

	os << cpu.getDeviceName() << " - Current state: " << (cpu.getIdleState() ? "idle" : "busy") << " - ";
	os << "Current load: " << deviceUsage.load <<  "%\n";

	return os;
}
