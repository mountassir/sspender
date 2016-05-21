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

#include "ConfigParser.h"

#include <sstream>
#include <vector>

namespace
{
	bool isValidIpAddress(const string &ipAddress)
	{
		bool ipIsValid = true;

		vector<string> splitString;

		splitStringByDelimiter(&splitString, ipAddress, ".");

		size_t size = splitString.size();

		if(size != 4)
		{
			ipIsValid = false;
		}

		for(size_t i = 0; i < size; ++i)
		{
			char* p;

			long converted = strtol(splitString[i].c_str(), &p, 10);

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

	bool isValidTime(const string &time)
	{
		bool timeIsValid = true;

		vector<string> splitString;

		splitStringByDelimiter(&splitString, time, ":");

		size_t size = splitString.size();

		if(size != 2)
		{
			timeIsValid = false;
		}

		for(size_t i = 0; i < size; ++i)
		{
			char* p;

			long converted = strtol(splitString[i].c_str(), &p, 10);

			if(*p || ( (converted < 0) || (i == 0 && converted > 23) || (i == 1 && converted > 60)))
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

	bool isValidDisk(const string &disk)
	{
		vector<string> disks, partitions;

		getAllDisksAndPartitions(&disks, &partitions);

		for(size_t i = 0, size = partitions.size(); i < size; ++i)
		{
			if(partitions[i] == disk)
			{
				return true;
			}
		}

		for(size_t i = 0, size = disks.size(); i < size; ++i)
		{
			if(disks[i] == disk)
			{
				return true;
			}
		}

		cout << "Invalid disk/partition " << "'" << disk << "'" << endl;

		return false;
	}
}

bool ConfigParser::loadConfigs(const string &filePath,
                         vector<string> *ipToWatch,
                         vector<string> *disksToMonitor,
                         vector<string> *disksToSpinDown,
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

	const Setting& root = cfg.getRoot();

	const char *configFile = filePath.c_str();

	string ips_to_watch;
	string disks_to_monitor;
	string disks_to_spin_down;
	string wake_at;
	string sleep_mode;

	printHeaderMessage("Reading config file = " + filePath, false);

	try
	{
		loockupFieldInCfgFile(root, string("ips_to_watch"),            ips_to_watch,             string(""));
		loockupFieldInCfgFile(root, string("disks_to_monitor"),        disks_to_monitor,         string("all"));
		loockupFieldInCfgFile(root, string("disks_to_spin_down"),      disks_to_spin_down,       string(""));
		loockupFieldInCfgFile(root, string("wake_at"),                 wake_at,                  string(""));
		loockupFieldInCfgFile(root, string("sleep_mode"),              sleep_mode,               string("disk"));
		loockupFieldInCfgFile(root, string("check_if_idle_every"),     *check_if_idle_every,     1);
		loockupFieldInCfgFile(root, string("stop_monitoring_for"),     *stop_monitoring_for,     5);
		loockupFieldInCfgFile(root, string("reset_monitoring_after"),  *reset_monitoring_after,  3);
		loockupFieldInCfgFile(root, string("suspend_after"),           *suspend_after,           5);
		loockupFieldInCfgFile(root, string("suspend_if_cpu_idle"),     *suspend_if_cpu_idle,     true);
		loockupFieldInCfgFile(root, string("suspend_if_storage_idle"), *suspend_if_storage_idle, true);
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
		 << "disks_to_monitor = "                 << disks_to_monitor         << "\n"
		 << "disks_to_spin_down = "               << disks_to_spin_down       << "\n"
		 << "wake_at = "                          << wake_at                  << "\n"
		 << "sleep_mode = "                       << sleep_mode               << "\n"
		 << "check_if_idle_every (minutes) = "    << *check_if_idle_every     << "\n"
		 << "stop_monitoring_for (minutes) = "    << *stop_monitoring_for     << "\n"
		 << "reset_monitoring_after (minutes) = " << *reset_monitoring_after  << "\n"
		 << "suspend_after (minutes) = "          << *suspend_after           << "\n";

	vector<string> allDisks, allPartitions;

	getAllDisksAndPartitions(&allDisks, &allPartitions);

	parseMultiChoiceSupportingAll(disks_to_monitor, disksToMonitor, allDisks, isValidDisk);
	parseMultiChoiceSupportingAll(disks_to_spin_down, disksToSpinDown, allDisks, isValidDisk);

	parseMultiChoiceArgs(ips_to_watch, ipToWatch, isValidIpAddress);
	parseMultiChoiceArgs(wake_at, wakeAt, isValidTime);
	parseSleepMode(sleep_mode, sleepMode);

	return true;
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
void ConfigParser::loockupFieldInCfgFile(const Setting& fileRoot,
									     const string &fieldName,
									     T &output,
									     const T &defaultValue)
{
	if(fileRoot.exists(fieldName))
	{
		cout << "Found '" << fieldName << "'\n";

		if(!fileRoot.lookupValue(fieldName, output))
		{
			cout << "Failed to lookup '" << fieldName << "'\n";

			ConfigException exp;

			throw(exp);
		}
	}
	else
	{
		cout << "Could not find '" << fieldName
			 << "', using default value '" << defaultValue << "'\n";

		output = defaultValue;
	}
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
