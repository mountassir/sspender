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

#include "Manager.h"
#include "Config.h"

using namespace std;

int main(int argc, char *argv[])
{
	string filePath;

	if (argc > 1)
	{
		filePath = argv[1];
	}

	vector<string> ipToWatch, disksToMonitor, disksToSpinDown, wakeAt;
	SLEEP_MODE sleepMode;
	int check_if_idle_every;
    int stop_monitoring_for;
    int reset_monitoring_after;
    int suspend_after;
    bool suspend_if_cpu_idle;
    bool suspend_if_storage_idle;

	Config configParser;
	Manager manager;

	bool configParsed = configParser.loadConfigs(filePath,
			                                     &ipToWatch,
			                                     &disksToMonitor,
			                                     &disksToSpinDown,
			                                     &wakeAt,
			                                     &sleepMode,
			                                     &suspend_if_cpu_idle,
			                                     &suspend_if_storage_idle,
			                                     &check_if_idle_every,
												 &stop_monitoring_for,
												 &reset_monitoring_after,
												 &suspend_after);

	if(configParsed)
	{
		printHeaderMessage("Using the following validated configuration:", false);

		cout << "\nSuspending if the CPU is idle: " << (suspend_if_cpu_idle ? "Yes" : "No");
		cout << "\nSuspending if the selected drives are idle: " << (suspend_if_storage_idle ? "Yes" : "No");

		cout << "\nDon't suspend the machine if any of these IPs is online: ";
		for(size_t i = 0, size = ipToWatch.size(); i < size; ++i)
		{
			cout << ipToWatch[i] << ",";
		}

		cout << "\nSuspend the machine if these drives are idle: ";
		for(size_t i = 0, size = disksToMonitor.size(); i < size; ++i)
		{
			cout << disksToMonitor[i] << ",";
		}

		cout << "\nSpin down these drives if idle: ";
		for(size_t i = 0, size = disksToSpinDown.size(); i < size; ++i)
		{
			cout << disksToSpinDown[i] << ",";
		}

		cout << "\nWake up the machine at the following times: ";
		for(size_t i = 0, size = wakeAt.size(); i < size; ++i)
		{
			cout << wakeAt[i] << ",";
		}

		cout << "\nSleep mode is: "
		     << (sleepMode == MEM ? "Suspend to RAM" : (sleepMode == DISK ? "Suspend to disk" : "Stand by"))
		     << endl;

		manager.setWhatToMonitor(suspend_if_cpu_idle, suspend_if_storage_idle);
		manager.setIpsToWatch(ipToWatch);
		manager.setDisksToMonitor(disksToMonitor);
		manager.setCpusToMonitor();
		manager.setTimesToWakeAt(wakeAt);
		manager.setSleepMode(sleepMode);
		manager.setTimers(check_if_idle_every,
		   	    		  stop_monitoring_for,
		   	    		  reset_monitoring_after,
		   	    		  suspend_after);
	}
	else
	{
		cout << "No path file provided, using default settings." << endl;
	}

	printHeaderMessage("Monitoring the machine", false);

	manager.watchSystem();

	return 0;
}
