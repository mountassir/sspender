/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * constants.h: Part of sspender
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

enum SLEEP_MODE {STAND_BY, MEM, DISK};

const int NETWORK_LIMIT       = 500;    //# KB/s total network traffic
const int CPU_LIMIT           = 10;     //# % cpu usage
const int STORAGE_READ_WRITE_LIMIT    = 400;    //#KB/s across all drives
const int STORAGE_LOAD_LIMIT  = 10;     //# % across all drives
const int MONITORING_THREAD_FREQUENCY = 1000; //The frequency at which the threads will monitor usage (ms)
const int SUSPEND_OFFSET      = 5;      //set the wake up time # minutes before to allow enough warm up time
const int SECONDS_IN_MINUTE   = 60;
const int TOTAL_MINUTES_IN_DAY = 1440;

const int DISK_STATS_LINE_INDEX = 0;
const int DISK_LINE_NUM_OF_STATS = 11;
const int NUM_OF_CPU_WORK_JIFFIES = 3;
const string PING_IDENTIFIER = "from";    //string to look for in ping output if client is connected
const string CONFIG_DELIMITER = ",";      //delimiter used in the config file

//default settings
const int SUSPEND_AFTER       = 15;         //suspend system if idle for more than (x minute)
const int CHECK_IF_IDLE_EVERY = 1;          //check if system is idle every (x minute)
const int STOP_MONITORING_FOR = 5;          //stop monitoring system for (x minute)
const int RESET_MONITORING_IF_BUSY_FOR = 3; //reset monitoring counter if found busy (x minute)
const bool SUSPEND_IF_CPU_IDLE     = true;  //suspend the machine if the cpu is idle
const bool SUSPEND_IF_STORAGE_IDLE = false; //suspend the machine if the storage are idle
const bool NO_SUSPEND_IF_NOT_IDLE  = true;  //server cannot be suspended if this drive is busy
const bool SPIN_DOWN_DISK_IF_IDLE = false;   //spin down the disks when idle
const string  DEFAULT_SLEEP_MODE = "disk";  //Default sleep mode

#endif
