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

#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

void getAllDisksAndPartitions(vector<string> *disks, vector<string> *partitions);
bool convertTimeToMinutes(string time, double *totalMinutes);
void getCurremtTimeInMinutes(double *totalMinutes);
bool uuidToDiskName(const string &uuid, string *diskName);
void printHeaderMessage(const string &message, bool logTime);
void splitByEmptySpace(const string &input, vector<string> *output);
bool runSystemCommand(const string &command, vector<string> *output = 0);
double getDoubleFromSystemCall(const string &command);
double roundValue(double value);
string trimString(string input);
string charTostring(const char *input);
void splitStringByDelimiter(vector<string> *output,
		                    const string &input,
		                    const string &delimiter);
void parseFile(const string & fileName, vector<string>* fileOutput);
void parseFile(ifstream &file, vector<string>* fileOutput);
string getCurrentDateTime();

#endif
