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

#ifndef CONFIGPARSER_H_
#define CONFIGPARSER_H_

#include <config4cpp/Configuration.h>
#include <libconfig.h++>
#include <functional>
#include <iostream>
#include <vector>

#include "utils.h"
#include "constants.h"

//using namespace config4cpp;
using namespace std;
using namespace libconfig;

class ConfigParser
{
public:
	bool loadConfigs(const string &filePath,
			         vector<string> *ipToWatch,
                     vector<string> *disksToMonitor,
			         vector<string> *wakeAt,
			         SLEEP_MODE *sleepMode,
			         bool *suspend_if_cpu_idle,
					 bool *suspend_if_storage_idle,
					 int *check_if_idle_every,
					 int *stop_monitoring_for,
					 int *reset_monitoring_after,
					 int *suspend_after);

	template <typename T>
	void loockupFieldInCfgFile(const Setting& scope,
					           const string &fieldName,
					           T &output,
					           const T *defaultValue = NULL);

	bool readFile(libconfig::Config &cfg, const string &filePath);

	void parseMultiChoiceArgs(const string &input,
							  vector<string> *output,
							  bool (*validator)(const string &));

	void parseMultiChoiceSupportingAll(const string &input,
									   vector<string> *output,
									   vector<string> allAvailableOptions,
									   bool (*validator)(const string &));

	void parseSleepMode(const string &inputSleepMode, SLEEP_MODE *sleepMode);
};

#endif
