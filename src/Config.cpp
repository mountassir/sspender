#include "Config.h"

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

bool Config::loadConfigs(const string &filePath,
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
	Configuration *  cfg = Configuration::create();

	const char *scope = "";
	const char *configFile = filePath.c_str();

	const char *ips_to_watch;
	const char *disks_to_monitor;
	const char *disks_to_spin_down;
	const char *wake_at;
	const char *sleep_mode;

	printHeaderMessage("Reading config file = " + filePath, false);

	try
	{
		cfg->parse(configFile);

		loockupFieldInCfgFile(cfg, scope, "ips_to_watch",           &ips_to_watch,           "");
		loockupFieldInCfgFile(cfg, scope, "disks_to_monitor",       &disks_to_monitor,       "all");
		loockupFieldInCfgFile(cfg, scope, "disks_to_spin_down",     &disks_to_spin_down,     "");
		loockupFieldInCfgFile(cfg, scope, "wake_at",                &wake_at,                "");
		loockupFieldInCfgFile(cfg, scope, "sleep_mode",             &sleep_mode,             "disk");
		loockupFieldInCfgFile(cfg, scope, "check_if_idle_every",    check_if_idle_every,     1);
		loockupFieldInCfgFile(cfg, scope, "stop_monitoring_for",    stop_monitoring_for,     5);
		loockupFieldInCfgFile(cfg, scope, "reset_monitoring_after", reset_monitoring_after,  3);
		loockupFieldInCfgFile(cfg, scope, "suspend_after",          suspend_after,           5);
		loockupFieldInCfgFile(cfg, scope, "suspend_if_cpu_idle",    suspend_if_cpu_idle,     true);
		loockupFieldInCfgFile(cfg, scope, "suspend_if_storage_idle",suspend_if_storage_idle, true);
	}
	catch(const ConfigurationException & ex)
	{
		cout << ex.c_str() << endl;
		cfg->destroy();

		return false;
	}

	printHeaderMessage("Loaded configuration from file = " + filePath, false);

	cout << "suspend_if_cpu_idle = "              << suspend_if_cpu_idle      << "\n"
		 << "suspend_if_storage_idle = "          << suspend_if_storage_idle  << "\n"
	     << "ips_to_watch = "                     << ips_to_watch             << "\n"
		 << "disks_to_monitor = "                 << disks_to_monitor         << "\n"
		 << "disks_to_spin_down = "               << disks_to_spin_down       << "\n"
		 << "wake_at = "                          << wake_at                  << "\n"
		 << "sleep_mode = "                       << sleep_mode               << "\n"
		 << "check_if_idle_every (minutes) = "    << *check_if_idle_every     << "\n"
		 << "stop_monitoring_for (minutes) = "    << *stop_monitoring_for     << "\n"
		 << "reset_monitoring_after (minutes) = " << *reset_monitoring_after  << "\n"
		 << "suspend_after (minutes) = "          << *suspend_after
		 << endl;

	vector<string> allDisks, allPartitions;

	getAllDisksAndPartitions(&allDisks, &allPartitions);

	parseMultiCoiceSupportingAll(disks_to_monitor, disksToMonitor, allDisks, isValidDisk);
	parseMultiCoiceSupportingAll(disks_to_spin_down, disksToSpinDown, allDisks, isValidDisk);

	parseMultiCoiceArgs(ips_to_watch, ipToWatch, isValidIpAddress);
	parseMultiCoiceArgs(wake_at, wakeAt, isValidTime);
	parseSleepMode(sleep_mode, sleepMode);

	cfg->destroy();

	return true;
}

bool Config::loockupFieldInCfgFile(Configuration *cfg,
								   const char *scope,
								   const char *fieldName,
								   const char **output,
								   const string &defaultValue)
{
	try
	{
		*output = cfg->lookupString(scope, fieldName);

		cout << "Found '" << fieldName << "'" << endl;
	}
	catch(const ConfigurationException & ex)
	{
		cout << ex.c_str() << endl;
		cout << "using default value '" << defaultValue << "' for " << fieldName << endl;
		*output = defaultValue.c_str();
		return false;
	}

	return true;
}

bool Config::loockupFieldInCfgFile(Configuration *cfg,
								   const char *scope,
								   const char *fieldName,
								   int *output,
								   int defaultValue)
{
	try
	{
		*output = cfg->lookupInt(scope, fieldName);

		cout << "Found '" << fieldName << "'" << endl;
	}
	catch(const ConfigurationException & ex)
	{
		cout << ex.c_str() << endl;
		cout << "using default value '" << defaultValue << "' for " << fieldName << endl;
		*output = defaultValue;
		return false;
	}

	return true;
}

bool Config::loockupFieldInCfgFile(Configuration *cfg,
								   const char *scope,
								   const char *fieldName,
								   bool *output,
								   bool defaultValue)
{
	try
	{
		*output = cfg->lookupBoolean(scope, fieldName);

		cout << "Found '" << fieldName << "'" << endl;
	}
	catch(const ConfigurationException & ex)
	{
		cout << ex.c_str() << endl;
		cout << "using default value '" << defaultValue << "' for " << fieldName << endl;
		*output = defaultValue;
		return false;
	}

	return true;
}

string Config::charTostring(const char *input)
{
	ostringstream oss;

	oss << input;

	return oss.str();
}

void Config::parseMultiCoiceSupportingAll(const char *input,
										  vector<string> *output,
										  vector<string> allAvailableOptions,
										  bool (*validator)(const string &))
{
	vector<string> splitString;

	string trimedInput = trimString(charTostring(input));

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

void Config::parseMultiCoiceArgs(const char *input,
		                         vector<string> *output,
		                         bool (*validator)(const string &))
{
	vector<string> splitString;

	string trimedInput = trimString(charTostring(input));

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

void Config::parseSleepMode(const char *inputSleepMode, SLEEP_MODE *sleepMode)
{
	string trimedInput = trimString(charTostring(inputSleepMode));

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
