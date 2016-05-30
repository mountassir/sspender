/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * utils.h: Part of sspender
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
#include <cstring>
#include <ctime>

using namespace std;

struct DiskCfg
{
	string diskName;
	string diskUUID;
	bool suspendIfIdle;
	bool spinDown;

//	ostream& operator << (ostream &os, const DiskCfg &cfg)
//	{
//
//	}
};

struct DeviceUsage
{
	double load;
	double totalRead;
	double totalWritten;

	void reset ()
	{
		load = 0;
		totalRead = 0;
		totalWritten = 0;
	}
};

struct DiskStats
{
	int num_r_io_processed;  //number of read I/Os processed
	int num_r_io_merged;     //number of read I/Os merged
	int num_r_sectors;       //number of sectors read
	int time_r_ticks;        //total wait time for read requests
	int num_w_io_processed;  //number of write I/Os processed
	int num_w_io_merged;     //number of write I/Os merged
	int num_w_sectors;       //number of sectors written
	int time_w_ticks;        //total wait time for write requests
	int num_in_flight;       //number of I/Os currently in flight
	int time_io_ticks;       //total time this block device has been active
	int time_in_queue;       //total wait for all requests
};

bool convertTimeToMinutes(string time, double *totalMinutes);
void getCurremtTimeInMinutes(double *totalMinutes);
bool uuidToDiskName(const string &uuid, string *diskName);
void printHeaderMessage(const string &message, bool logTime);
void splitByEmptySpace(const string &input, vector<string> *output);
bool runSystemCommand(const string &command, vector<string> *output = NULL);
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
