/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * ConfigParser.cpp: Part of sspender
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

#include "ConfigParser.h"

#include <sstream>
#include <vector>

namespace
{
	//make sure that ipAddress is of format
	//'int.int.int.int'
	bool isValidIpv4Address(const string &ipAddress)
	{
		bool ipv4IsValid = true;

		vector<string> splitString;

		splitStringByDelimiter(&splitString, ipAddress, ".");

		size_t size = splitString.size();

		//ip address is formed of 4 int separated by a '.'
		if(size != 4)
		{
			ipv4IsValid = false;
		}

		for(size_t i = 0; i < size; ++i)
		{
			char* p;

			long converted = strtol(splitString[i].c_str(), &p, 10);

			//ip address should only contain positive values between 0 and 255
			if(*p || converted < 0 || converted > 255)
			{
				ipv4IsValid = false;
			}
		}

		if(!ipv4IsValid)
		{
			cout << "Invalid IPv4 address: " << ipAddress << endl;
		}

		return ipv4IsValid;
	}

	//time is of format 'int:int'
	bool isValidTime(const string &time)
	{
		bool timeIsValid = true;

		vector<string> splitString;

		splitStringByDelimiter(&splitString, time, ":");

		size_t size = splitString.size();

		//only hours and minutes are supported
		if(size != 2)
		{
			timeIsValid = false;
		}

		for(size_t i = 0; i < size; ++i)
		{
			char* p;

			long converted = strtol(splitString[i].c_str(), &p, 10);

			if(*p || ( (converted < 0)            //time should have positive numbers
				  || (i == 0 && converted > 23)   //hours should be 0 - 23
				  || (i == 1 && converted > 59))) //minutes should be 0 - 59
			{
				timeIsValid = false;
			}
		}

		if(!timeIsValid)
		{
			cout << "Invalid time: " << time << endl;
		}

		return timeIsValid;
	}

	//if you pass in a partition, it will be overwritten by it's parent disk
	//the function will check if the resulting disk name is valid and attached
	//to this machine
	bool validateDisk(DiskCfg &disk, PartitionTable &partitionTable)
	{
		//don't monitor the disk's usage if both options are false
		if(disk.suspendIfIdle || disk.spinDown)
		{
			string parentDisk;

			//check if this is a partition
			if(partitionTable.isPartitionValid(disk.diskName, &parentDisk))
			{
				cout << "'" << disk.diskName << "' is a partition, "
					 << "will monitor the parent disk '" << parentDisk << "' instead.\n";

				//if it is, we will monitor its parent disk instead
				disk.diskName = parentDisk;
			}

			//check if this is a valid disk that's attached to this machine
			return partitionTable.isDiskValid(disk.diskName);
		}
		else
		{
			cout << "Both no_suspend_if_not_idle and spind_down_if_idle "
				 << "are set to false for " << disk.diskName << ", skipping.\n";
		}

		return false;
	}
}

bool ConfigParser::loadConfigs(const string &filePath,
					vector<string> *ipToWatch,
					CpuCfg *cpuConfig,
					vector<DiskCfg> *disksToMonitor,
					vector<string> *wakeAt,
					SLEEP_MODE *sleepMode,
					int *check_if_idle_every,
					int *stop_monitoring_for,
					int *reset_monitoring_after,
					int *suspend_after)
{
	toml::table tomlTable;
    try
    {
        tomlTable = toml::parse_file(filePath);
    }
    catch (const toml::parse_error& err)
    {
        std::cerr << "Parsing the config file failed:\n" << err << "\n";
        return false;
    }

	//[setting]
	string ips_to_watch = tomlTable["setting"]["ips_to_watch"].value_or("");
	string wake_at      = tomlTable["setting"]["wake_at"].value_or("");
	string sleep_mode   = tomlTable["setting"]["sleep_mode"].value_or(DEFAULT_SLEEP_MODE);

	//[tuning]
	*check_if_idle_every    = tomlTable["tuning"]["check_if_idle_every"].value_or(CHECK_IF_IDLE_EVERY);
	*stop_monitoring_for    = tomlTable["tuning"]["stop_monitoring_for"].value_or(STOP_MONITORING_FOR);
	*reset_monitoring_after = tomlTable["tuning"]["reset_monitoring_after"].value_or(RESET_MONITORING_IF_BUSY_FOR);
	*suspend_after          = tomlTable["tuning"]["suspend_after"].value_or(SUSPEND_AFTER);

	//[devices_to_monitor.cpu]
	cpuConfig->cpuName = "CPU";
	cpuConfig->idle_load_threshold = tomlTable["devices_to_monitor"]["cpu"]["idle_load_threshold"].value_or(IDLE_LOAD_THRESHOLD);
	cpuConfig->idle_time_threshold = tomlTable["devices_to_monitor"]["cpu"]["idle_time_threshold"].value_or(IDLE_TIME_THRESHOLD);
	cpuConfig->suspendIfIdle       = tomlTable["devices_to_monitor"]["cpu"]["no_suspend_if_not_idle"].value_or(NO_SUSPEND_IF_NOT_IDLE);

	bool monitorAllDisks = tomlTable["devices_to_monitor"]["all_disks"].value_or(MONITOR_ALL_DISKS);

	if(monitorAllDisks)
	{
		//get all disks attached to this machine to be monitored
		getAllDisksToMonitor(disksToMonitor);
	}
	else
	{
		//[devices_to_monitor.disks]
		if (auto tableNodes = tomlTable["devices_to_monitor"]["disks"].as_array()) 
		{
			for (auto&& node : *tableNodes) 
			{
				if (auto diskTable = node.as_table())
				{
					DiskCfg diskCfg;

					diskCfg.suspendIfIdle = (*diskTable)["no_suspend_if_not_idle"].value_or(NO_SUSPEND_IF_NOT_IDLE);
					diskCfg.spinDown = (*diskTable)["spind_down_if_idle"].value_or(SPIN_DOWN_DISK_IF_IDLE);
					diskCfg.idle_load_threshold = (*diskTable)["idle_load_threshold"].value_or(IDLE_LOAD_THRESHOLD);
					diskCfg.idle_time_threshold = (*diskTable)["idle_time_threshold"].value_or(IDLE_TIME_THRESHOLD);

					string diskName = (*diskTable)["name"].value_or("");
					string diskUUID = (*diskTable)["uuid"].value_or("");
					bool gotValidUuid = false;

					//try to use UUID first 
					if(diskUUID != "")
					{
						gotValidUuid = uuidToDiskName(diskUUID, &diskCfg.diskName);
					}

					//if not, use disk name
					if(!gotValidUuid)
					{
						diskCfg.diskName = diskName;
					}

					addDisk(diskCfg, disksToMonitor);
				}
			}
		}
	}

	printHeaderMessage("Loaded configuration from file = " + filePath, false);

	cout << "ips_to_watch = "                << ips_to_watch             << "\n"
	<< "wake_at = "                          << wake_at                  << "\n"
	<< "sleep_mode = "                       << sleep_mode               << "\n"
	<< "check_if_idle_every (minutes) = "    << *check_if_idle_every     << "\n"
	<< "stop_monitoring_for (minutes) = "    << *stop_monitoring_for     << "\n"
	<< "reset_monitoring_after (minutes) = " << *reset_monitoring_after  << "\n"
	<< "suspend_after (minutes) = "          << *suspend_after           << endl;

	printHeaderMessage("Validating configurations from file = " + filePath, false);

	parseMultiChoiceArgs(ips_to_watch, ipToWatch, isValidIpv4Address);
	parseMultiChoiceArgs(wake_at, wakeAt, isValidTime);
	parseSleepMode(sleep_mode, sleepMode);

	return true;
}

void ConfigParser::addDisk(DiskCfg &diskcfg, vector<DiskCfg> *diskConfigs)
{
	//make sure we got a valid disk
	//if we got a partition instead, validateDisk will change the diskName
	//to it's parent disk instead (we only monitor disks not partitions)
	if(validateDisk(diskcfg, m_partitionTable))
	{
		vector<DiskCfg>::const_iterator iter = diskConfigs->begin();

		bool diskAlreadyAdded = false;

		while(iter != diskConfigs->end())
		{
			if(iter->diskName == diskcfg.diskName)
			{
				cout << "'" << diskcfg.diskName << "' already being monitored, skipping.\n";

				diskAlreadyAdded = true;

				break;
			}

			iter++;
		}

		if(!diskAlreadyAdded)
		{
			diskConfigs->push_back(diskcfg);
		}
	}
	else
	{
		cout << "'" << diskcfg.diskName << "' is not valid, skipping.\n";
	}
}

void ConfigParser::getAllDisksToMonitor(vector<DiskCfg> *diskConfigs)
{
	cout << "Getting all the disks attached to the machine: ";
	vector<string> disks;

	m_partitionTable.getAllDisks(&disks);

	for(size_t i = 0, size = disks.size(); i < size; ++i)
	{
		cout << disks[i] << ", ";
		DiskCfg diskCfg = {
			disks[i], 
			"", 
			IDLE_LOAD_THRESHOLD, 
			IDLE_TIME_THRESHOLD, 
			NO_SUSPEND_IF_NOT_IDLE, 
			SPIN_DOWN_DISK_IF_IDLE};

		diskConfigs->push_back(diskCfg);
	}

	cout << endl;
}

void ConfigParser::parseMultiChoiceSupportingAll(const string &input,
										         vector<string> *output,
										         vector<string> allAvailableOptions,
										         bool (*validator)(const string &))
{
	vector<string> splitString;

	string trimedInput = trimString(input);

	splitStringByDelimiter(&splitString, trimedInput, CONFIG_DELIMITER);

	for(size_t i = 0, size = splitString.size(); i < size; ++i)
	{
		if(trimString(splitString[i]) == "all")
		{
			for(size_t j = 0, size = allAvailableOptions.size(); j < size; ++j)
			{
				output->push_back(allAvailableOptions[j]);

				return;
			}
		}
	}

	for(size_t i = 0, size = splitString.size(); i < size; ++i)
	{
		string trimedField = trimString(splitString[i]);

		if(trimedField != "" && validator(trimedField))
		{
			output->push_back(trimedField);
		}
	}
}

void ConfigParser::parseMultiChoiceArgs(const string &input,
		                                vector<string> *output,
		                                bool (*validator)(const string &))
{
	vector<string> splitString;

	string trimedInput = trimString(input);

	splitStringByDelimiter(&splitString, trimedInput, CONFIG_DELIMITER);

	for(size_t i = 0, size = splitString.size(); i < size; ++i)
	{
		string trimedField = trimString(splitString[i]);

		if(trimedField != "" && validator(trimedField))
		{
			output->push_back(trimedField);
		}
	}
}

void ConfigParser::parseSleepMode(const string &inputSleepMode, SLEEP_MODE *sleepMode)
{
	string trimedInput = trimString(inputSleepMode);

	if(trimedInput.compare("mem") == 0)
	{
		*sleepMode = MEM;
	}
	else
	if(trimedInput.compare("disk") == 0)
	{
		*sleepMode = DISK;
	}
	else
	if(trimedInput.compare("standby") == 0)
	{
		*sleepMode = STAND_BY;
	}
	else
	{
		cout << inputSleepMode << " is not a valid sleep mode (mem, disk or standby), " << " using mem as default." << endl;
		*sleepMode = MEM;
	}
}
