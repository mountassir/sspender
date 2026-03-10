/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * ConfigParser.h: Part of sspender
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

#ifndef CONFIGPARSER_H_
#define CONFIGPARSER_H_

#include <toml++/toml.hpp>
#include <functional>
#include <iostream>
#include <vector>

#include "utils.h"
#include "constants.h"
#include "PartitionTable.h"

using namespace std;

class ConfigParser
{
private:
	PartitionTable & m_partitionTable;

public:
	ConfigParser(PartitionTable & partitionTable) : m_partitionTable(partitionTable){} ;

	//parse all the settings from the cfg file
	bool loadConfigs(const string &filePath,
			         vector<ipCfg> &ipAddressesToWatch,
			         CpuCfg *cpuConfig,
                     vector<DiskCfg> &disksToMonitor,
			         vector<string> &wakeAt,
			         SLEEP_MODE *sleepMode,
					 int *check_if_idle_every,
					 int *stop_monitoring_for,
					 int *reset_monitoring_after,
					 int *suspend_after);

	//gets all the disks attached to the machine for monitoring
	void getAllDisksToMonitor(vector<DiskCfg> &diskConfigs);

	//parse a string containing multiple options 'option1,option2,option3'
	void parseMultiChoiceArgs(const string &input,
							  vector<string> &output,
							  bool (*validator)(const string &));

	//parse a string containing multiple options with 'all' being an option
	//'option1,all,option2,option3', if 'all' is detected then allAvailableOptions
	//will be used and all other options will be ignored
	void parseMultiChoiceSupportingAll(const string &input,
									   vector<string> &output,
									   vector<string> &allAvailableOptions,
									   bool (*validator)(const string &));

	//parse disk structures
	void addDisk(DiskCfg &disk, vector<DiskCfg> &diskConfigs);

	//parse sleep mode field
	void parseSleepMode(const string &inputSleepMode, SLEEP_MODE *sleepMode);
};

#endif
