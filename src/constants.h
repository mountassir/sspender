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

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

using namespace std;

const double TOTAL_MINUTS_IN_DAY = 1440;
const double SUSPEND_AFTER       = 15;     //suspend if system idle # times
const double CHECK_IF_IDLE_EVERY = 1;     //check if system is idle every # minutes
const double STOP_MONITORING_FOR = 5;    //stop monitoring system for # minutes
const double RESET_MONITORING_IF_BUSY_FOR = 3; //reset monitoring counter if found busy # times
const double NETWORK_LIMIT       = 500;    //500KB/s total network traffic
const double CPU_LIMIT           = 10;     //10% cpu usage
const double STORAGE_READ_WRITE_LIMIT    = 400;    //400KB/s across all drives
const double STORAGE_LOAD_LIMIT  = 10;    //5% across all drives
const double CPU_IDLE_INDEX      = 5;      //index of cpu idle value in iostat output
const double DISK_READ_INDEX     = 4;      //index of disk read value in iostat output
const double DISK_WRITE_INDEX    = 5;      //index of disk write value in iostat output
const int    MONITORING_THREAD_FREQUENCY = 1000; //The frequency at which the threads will monitor usage (ms)
const string BEFOR_CPU_LINE_IDENTIFIER   = "avg-cpu";   //line before cpu usage in iostat output
const string BEFORE_DISK_LINE_IDENTIFIER = "Device";    //line before disks usage in iostat output
const string PING_IDENTIFIER = "from";    //string to look for in ping output if client is connected
const string CONFIG_DELIMITER = ",";      //delimiter used in the config file

enum SLEEP_MODE {STAND_BY, MEM, DISK};

#endif
