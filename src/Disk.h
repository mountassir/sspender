/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Disk.h: Part of sspender
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

#ifndef DISK_H_
#define DISK_H_


#include "Device.h"
#include "utils.h"

using namespace std;

/*
 * This class represents a single disk that will be monitored,
 * stores, updates and exports all available statistics.
 * This extends the class Device
 */
class Disk : public Device {

private:
	string m_uuid;                 //disk UUID
	int m_sectorSize;              //disk sector size
	bool m_isSpinning;             //is the disk spinning
	bool m_shouldSpinDownIfIdle;   //should spin down this disk when idle?

	void spinDown();
	bool isDiskSpinning();
	void setSpinningState(bool spinningState);

public:
	Disk(const string &diskName,
		 const string &diskUuid,
		 bool spinDown,
		 bool suspendIfIdle) :
			 Device(diskName, suspendIfIdle),
				 m_uuid(diskUuid),
				 m_sectorSize(0),
				 m_isSpinning(true),
				 m_shouldSpinDownIfIdle(spinDown) { };

	//initialize this disk
	virtual void initDevice();

	//start monitoring this drive's usage
	virtual void monitorUsage();

	//reads the stats from the file and calculate this disk's usage
	virtual void calculateUsage(ifstream &statesFile, DeviceUsage *diskUsage);

	//returns the file containing the stats information for this disk
	virtual string getStatesFileName();

	virtual bool shouldMonitorUsage();

	virtual void setIdle(bool state);

protected:
	//return the sector size of this disk
	int  getDiskSectorSize(const string &diskName);

	//get and parse all the stats for this disk
	void getDiskStats(ifstream &statesFile, DiskStats *stats);

	//returns whether we should spin down this disk when it's idle or not
	bool shouldSpinDownIfIdle();
};

#endif
