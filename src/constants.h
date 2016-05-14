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
const double STORAGE_LIMIT       = 400;    //400KB/s across all drives
const double CPU_IDLE_INDEX      = 5;      //index of cpu idle value in iostat output
const double DISK_READ_INDEX     = 4;      //index of disk read value in iostat output
const double DISK_WRITE_INDEX    = 5;      //index of disk write value in iostat output
const string BEFOR_CPU_LINE_IDENTIFIER   = "avg-cpu";   //line before cpu usage in iostat output
const string BEFORE_DISK_LINE_IDENTIFIER = "Device";    //line before disks usage in iostat output
const string PING_IDENTIFIER = "from";    //string to look for in ping output if client is connected
const string CONFIG_DELIMITER = ",";      //delimiter used in the config file

enum SLEEP_MODE {STAND_BY, MEM, DISK};

#endif
