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
	bool isValidIpAddress(const string &ipAddress)
	{
		bool ipIsValid = true;

		vector<string> splitString;

		splitStringByDelimiter(&splitString, ipAddress, ".");

		size_t size = splitString.size();

		//ip address is formed of 4 int separated by a '.'
		if(size != 4)
		{
			ipIsValid = false;
		}

		for(size_t i = 0; i < size; ++i)
		{
			char* p;

			long converted = strtol(splitString[i].c_str(), &p, 10);

			//ip address should only contain positive values
			if(*p || converted < 0)
			{
				ipIsValid = false;
			}
		}

		if(!ipIsValid)
		{
			cout << "Invalid IP address: " << ipAddress << endl;
		}

		return ipIsValid;
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
					 << "will monitor the parent disk '" << parentDisk << "'.\n";

				//if it is, we will monitor it's parent disk instead
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
		                 const PartitionTable &partitionTable,
                         vector<string> *ipToWatch,
                         vector<DiskCfg> *diskConfigs,
                         vector<string> *wakeAt,
                         SLEEP_MODE *sleepMode,
                         bool *suspend_if_cpu_idle,
                         bool *suspend_if_storage_idle,
                         int *check_if_idle_every,
						 int *stop_monitoring_for,
						 int *reset_monitoring_after,
						 int *suspend_after)
{
	libconfig::Config cfg;

	cfg.setAutoConvert(false);

	if(!readFile(cfg, filePath))
	{
		return false;
	}

	bool monitorAllDisks = false;
	string ips_to_watch = "";
	string wake_at = "";
	string sleep_mode = "";

	try
	{
		const Setting& fileRoot = cfg.getRoot();

		//different scopes/sections in the config file
		const Setting& tuningScope  = fileRoot["tuning"];
		const Setting& settingScope = fileRoot["setting"];
		const Setting& deviceScope  = settingScope["devices_to_monitor"];
		const Setting& diskScope    = deviceScope["disks"];

		printHeaderMessage("Reading config file = " + filePath, false);

		//rootCfgFile.tuning
		loockupFieldInCfgFile(tuningScope, string("check_if_idle_every"),    *check_if_idle_every,    &CHECK_IF_IDLE_EVERY);
		loockupFieldInCfgFile(tuningScope, string("stop_monitoring_for"),    *stop_monitoring_for,    &STOP_MONITORING_FOR);
		loockupFieldInCfgFile(tuningScope, string("suspend_after"),          *suspend_after,          &SUSPEND_AFTER);
		loockupFieldInCfgFile(tuningScope, string("reset_monitoring_after"), *reset_monitoring_after, &RESET_MONITORING_IF_BUSY_FOR);

		//rootCfgFile.setting
		loockupFieldInCfgFile(settingScope, string("suspend_if_cpu_idle"),     *suspend_if_cpu_idle,     &SUSPEND_IF_CPU_IDLE);
		loockupFieldInCfgFile(settingScope, string("suspend_if_storage_idle"), *suspend_if_storage_idle, &SUSPEND_IF_STORAGE_IDLE);
		loockupFieldInCfgFile(settingScope, string("ips_to_watch"), ips_to_watch);
		loockupFieldInCfgFile(settingScope, string("wake_at"),      wake_at);
		loockupFieldInCfgFile(settingScope, string("sleep_mode"), sleep_mode, &DEFAULT_SLEEP_MODE);

		//rootCfgFile.setting.devices
		loockupFieldInCfgFile(deviceScope, string("all_disks"),   monitorAllDisks);

		if(monitorAllDisks)
		{
			//get all disks attached to this machine to be monitored
			getAllDisksToMonitor(diskConfigs);
		}
		else
		{
			//rootCfgFile.setting.devices.disks
			parseDisks(diskScope, diskConfigs);
		}
	}
	catch(const ConfigException &configExep)
	{
		cout << configExep.what() << endl;

		return false;
	}

	printHeaderMessage("Loaded configuration from file = " + filePath, false);

	cout << "suspend_if_cpu_idle = "              << (*suspend_if_cpu_idle ? "true" : "false")      << "\n"
		 << "suspend_if_storage_idle = "          << (*suspend_if_storage_idle  ? "true" : "false") << "\n"
		 << "ips_to_watch = "                     << ips_to_watch             << "\n"
		 //<< "disks_to_monitor = "                 << disks_to_monitor         << "\n"
		 << "wake_at = "                          << wake_at                  << "\n"
		 << "sleep_mode = "                       << sleep_mode               << "\n"
		 << "check_if_idle_every (minutes) = "    << *check_if_idle_every     << "\n"
		 << "stop_monitoring_for (minutes) = "    << *stop_monitoring_for     << "\n"
		 << "reset_monitoring_after (minutes) = " << *reset_monitoring_after  << "\n"
		 << "suspend_after (minutes) = "          << *suspend_after           << "\n";

	parseMultiChoiceArgs(ips_to_watch, ipToWatch, isValidIpAddress);
	parseMultiChoiceArgs(wake_at, wakeAt, isValidTime);
	parseSleepMode(sleep_mode, sleepMode);

	return true;
}

void ConfigParser::parseDisks(const Setting& diskScope, vector<DiskCfg> *diskConfigs)
{
	for(size_t i = 0, len = diskScope.getLength(); i < len; ++i)
	{
		DiskCfg disk;

		loockupFieldInCfgFile(diskScope[i], string("no_suspend_if_not_idle"), disk.suspendIfIdle, &NO_SUSPEND_IF_NOT_IDLE);
		loockupFieldInCfgFile(diskScope[i], string("spind_down_if_idle"), disk.spinDown, &SPIN_DOWN_DISK_IF_IDLE);

		string diskUuid, diskName;
		bool gotValidUuid = false;

		//try to get UUID first
		if(loockupFieldInCfgFile(diskScope[i], string("uuid"), diskUuid))
		{
			gotValidUuid = uuidToDiskName(diskUuid, &diskName);
		}

		//if not, then look for disk name
		if(!gotValidUuid)
		{
			loockupFieldInCfgFile(diskScope[i], string("name"), diskName);
		}

		disk.diskUUID = diskUuid;
		disk.diskName = diskName;

		//make sure we got a valid disk
		//if we got a partition instead, validateDisk will change the diskName
		//to it's parent disk instead (we only monitor disks not partitions)
		if(validateDisk(disk, m_partitionTable))
		{
			vector<DiskCfg>::const_iterator iter = diskConfigs->begin();

			bool diskAlreadyAdded = false;

			while(iter != diskConfigs->end())
			{
				if(iter->diskName == disk.diskName)
				{
					cout << "'" << disk.diskName << "' already being monitored, skipping.\n";

					diskAlreadyAdded = true;

					break;
				}

				iter++;
			}

			if(!diskAlreadyAdded)
			{
				diskConfigs->push_back(disk);
			}
		}
		else
		{
			cout << "'" << disk.diskName << "' is not valid, skipping.\n";
		}
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
		DiskCfg diskCfg= {disks[i], "", true, false};

		diskConfigs->push_back(diskCfg);
	}

	cout << "\n";
}

bool ConfigParser::readFile(libconfig::Config &cfg, const string &filePath)
{
	try
	{
		cfg.readFile(filePath.c_str());
	}
	catch(const FileIOException &fioex)
	{
		std::cerr << "I/O error while reading file: " << filePath << std::endl;

		return false;
	}
	catch(const ParseException &pex)
	{
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
		<< " - " << pex.getError() << std::endl;

		return false;
	}

	return true;
}

template <typename T>
bool ConfigParser::loockupFieldInCfgFile(const Setting& scope,
									     const string &fieldName,
									     T &output,
									     const T *defaultValue /* = null*/)
{
	if(scope.exists(fieldName))
	{
		cout << "Found '" << fieldName << "'\n";

		if(!scope.lookupValue(fieldName, output))
		{
			cout << "Failed to lookup '" << fieldName << "'\n";

			ConfigException exp;

			throw(exp);
		}

		return true;
	}
	else //if the field does not exist, then use the defaultValue (if one is provided) instead
	if(defaultValue)
	{
		cout << "Could not find '" << fieldName
			 << "', using default value '" << *defaultValue << "'\n";

		output = *defaultValue;

		return true;
	}

	return false;
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
	if(trimedInput.compare("stand_by") == 0)
	{
		*sleepMode = STAND_BY;
	}
	else
	{
		cout << inputSleepMode << " is not a valid sleep mode (mem, disk or stand_by), " << " using disk as default." << endl;
		*sleepMode = DISK;
	}
}
