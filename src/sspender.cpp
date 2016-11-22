/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * sspender.cpp: Part of sspender
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
#include "PartitionTable.h"
#include "ConfigParser.h"

using namespace std;

int main(int argc, char *argv[])
{
	string filePath;

	if (argc > 1)
	{
		filePath = argv[1];
	}

	vector<string> ipToWatch, wakeAt;
	vector<DiskCfg> disksToMonitor;
	SLEEP_MODE sleepMode;
	int check_if_idle_every;
    int stop_monitoring_for;
    int reset_monitoring_after;
    int suspend_after;
    bool suspend_if_cpu_idle;
    bool suspend_if_storage_idle;

	PartitionTable partitionTable;

	partitionTable.loadPartitionTable();

	cout << "PartitionTable loaded:\n" << partitionTable << "\n";

	ConfigParser configParser(partitionTable);
	Manager manager;

	bool configParsed = configParser.loadConfigs(filePath,
												 partitionTable,
			                                     &ipToWatch,
			                                     &disksToMonitor,
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
		cout << "\nSuspending if the storage idle: " << (suspend_if_storage_idle ? "Yes" : "No");

		cout << "\nDon't suspend the machine if any of these IPs is online: ";
		for(size_t i = 0, size = ipToWatch.size(); i < size; ++i)
		{
			cout << ipToWatch[i] << ",";
		}

		cout << "\nSuspend the machine if these drives are idle: ";
		for(size_t i = 0, size = disksToMonitor.size(); i < size; ++i)
		{
			if(disksToMonitor[i].suspendIfIdle)
			{
				cout << disksToMonitor[i].diskName << ",";
			}
		}

		cout << "\nSpin down these drives if they are idle: ";
		for(size_t i = 0, size = disksToMonitor.size(); i < size; ++i)
		{
			if(disksToMonitor[i].spinDown)
			{
				cout << disksToMonitor[i].diskName << ",";
			}
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

		printHeaderMessage("Monitoring the machine", false);

		manager.monitorSystemUsage();
	}
	else
	{
		cout << "Failed to parse the cfg file, quitting." << endl;
	}

	return(EXIT_SUCCESS);
}
