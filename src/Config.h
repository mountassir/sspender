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

#ifndef CONFIG_H_
#define CONFIG_H_

#include <config4cpp/Configuration.h>
#include <functional>
#include <iostream>
#include <vector>

#include "utils.h"
#include "constants.h"

using namespace config4cpp;
using namespace std;

class Config
{
public:
	bool loadConfigs(const string &filePath,
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
					 int *suspend_after);

	bool loockupFieldInCfgFile(Configuration *cfg,
					           const char *scope,
					           const char *fieldName,
					           const char **output,
					           const string &defaultValue);

	bool loockupFieldInCfgFile(Configuration *cfg,
					           const char *scope,
					           const char *fieldName,
					           int *output,
					           int defaultValue);

	bool loockupFieldInCfgFile(Configuration *cfg,
					           const char *scope,
					           const char *fieldName,
					           bool *output,
					           bool defaultValue);

	void parseMultiCoiceArgs(const char *input,
							 vector<string> *output,
							 bool (*validator)(const string &));

	void parseMultiCoiceSupportingAll(const char *input,
									  vector<string> *output,
									  vector<string> allAvailableOptions,
									  bool (*validator)(const string &));

	string charTostring(const char *input);
	void parseSleepMode(const char *inputSleepMode, SLEEP_MODE *sleepMode);
};

#endif
