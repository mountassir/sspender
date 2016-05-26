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

#ifndef DISK_H_
#define DISK_H_


#include "Device.h"
#include "utils.h"

using namespace std;

class Disk : public Device {

private:
	string m_uuid;
	int m_sectorSize;
	bool m_shouldSpinDownIfIdle;

public:
	Disk(const string &diskName,
		 const string &diskUuid,
		 bool spinDown,
		 bool suspendIfIdle) :
			 Device(diskName, suspendIfIdle),
				 m_uuid(diskUuid),
				 m_shouldSpinDownIfIdle(spinDown),
				 m_sectorSize(0) { };

	virtual void initDevice();
	virtual void monitorUsage();
	virtual string getStatesFileName();
	virtual void calculateUsage(ifstream &statesFile, DeviceUsage *diskUsage);
	bool spinDownIfIdle() {return m_shouldSpinDownIfIdle; };

protected:
	int  getDiskSectorSize(const string &diskName);
	void calculateDiskUsage(double *totalRead, double *totalWrite);
	void updateReadWrite();
	void getDiskStats(ifstream &statesFile, DiskStats *stats);
	void parseDiskStats(const vector<string> &fileOutput, DiskStats *stats);
};

#endif
