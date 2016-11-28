/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Disk.cpp: Part of sspender
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

#include "Disk.h"

void Disk::initDevice()
{
	m_sectorSize = getDiskSectorSize(getDeviceName());

	isDeviceInitialized(true);
}

//eg: sda stats are stored in /sys/block/sda/stat
string Disk::getStatesFileName()
{
	stringstream ss;
	ss << "/sys/block/" << getDeviceName() << "/stat";

	return ss.str();
}

//make sure this disk is initialized and spawn a thread
//that will monitor its usage in the background
void Disk::monitorUsage()
{
	if(!isDeviceInitialized())
	{
		initDevice();
	}

	std::thread diskMonitorThread (monitorDeviceUsage, this, getWatchDogCopy());

	diskMonitorThread.detach();
}

void Disk::calculateUsage(ifstream &statsFile, DeviceUsage *diskUsage)
{
	DiskStats prevDiskStates, newDiskStates;

	//read all the stats from the file
	getDiskStats(statsFile, &prevDiskStates);

	//wait for MONITORING_THREAD_FREQUENCY ms
	std::this_thread::sleep_for(std::chrono::milliseconds(MONITORING_THREAD_FREQUENCY));

	//read all the stats again
	getDiskStats(statsFile, &newDiskStates);

	//get the total time this disk has spent dealing with IO requests
	//and get the percentage of that over the waiting time
	diskUsage->load = ( double(newDiskStates.time_io_ticks - prevDiskStates.time_io_ticks) / MONITORING_THREAD_FREQUENCY ) * 100;

	//get the total read/written secrtors during the waiting time,
	//multiply that by the sector size of the disk to get bytes
	//and then devide it by 1000 to get the Kbytes
	diskUsage->totalRead = ( double(newDiskStates.num_r_sectors - prevDiskStates.num_r_sectors) * m_sectorSize ) / 1000;
	diskUsage->totalWritten = ( double(newDiskStates.num_w_sectors - prevDiskStates.num_w_sectors) * m_sectorSize ) / 1000;
}

void Disk::setIdle(bool state)
{
	setIdleState(state);

	if(state)
	{
		spinDown();
	}
	else
	{
		setSpinningState(true);
	}
}

bool Disk::shouldMonitorUsage()
{
	return shouldSuspendIfIdle() || shouldSpinDownIfIdle();
}

void Disk::getDiskStats(ifstream &statsFile, DiskStats *stats)
{
	vector<string> fileOutput;

	parseFile(statsFile, &fileOutput);

	//make sure we got at least one line
	if(fileOutput.size() > 0)
	{
		string statesLine = fileOutput[DISK_STATS_LINE_INDEX];
		vector<string> tokens;

		splitByEmptySpace(statesLine, &tokens);

		if(tokens.size() == DISK_LINE_NUM_OF_STATS)
		{
			stats->num_r_io_processed = atoi(tokens[0].c_str());  //number of read I/Os processed
			stats->num_r_io_merged    = atoi(tokens[1].c_str());  //number of read I/Os merged
			stats->num_r_sectors      = atoi(tokens[2].c_str());  //number of sectors read
			stats->time_r_ticks       = atoi(tokens[3].c_str());  //total wait time for read requests
			stats->num_w_io_processed = atoi(tokens[4].c_str());  //number of write I/Os processed
			stats->num_w_io_merged    = atoi(tokens[5].c_str());  //number of write I/Os merged
			stats->num_w_sectors      = atoi(tokens[6].c_str());  //number of sectors written
			stats->time_w_ticks       = atoi(tokens[7].c_str());  //total wait time for write requests
			stats->num_in_flight      = atoi(tokens[8].c_str());  //number of I/Os currently in flight
			stats->time_io_ticks      = atoi(tokens[9].c_str());  //total time this block device has been active
			stats->time_in_queue      = atoi(tokens[10].c_str()); //total wait for all requests
		}
	}
}

int Disk::getDiskSectorSize(const string &diskName)
{
	int sectorSize = 0;

	vector<string> fileOutput;

	parseFile("/sys/block/" + diskName + "/queue/hw_sector_size", &fileOutput);

	if(fileOutput.size() > 0)
	{
		sectorSize = atoi(fileOutput[0].c_str());
	}

	return sectorSize;
}

void Disk::spinDown()
{
	if(isDiskSpinning())
	{
		cout << "Spinning " << getDeviceName() << " down." << endl;
		ostringstream oss;
		oss << "hdparm -Y /dev/" + getDeviceName();
		cout << oss.str() << endl;

		vector<string> output;

		runSystemCommand(oss.str(), &output);

		setSpinningState(false);
	}
}

bool Disk::isDiskSpinning()
{
	return m_isSpinning;
}

void Disk::setSpinningState(bool spinningState)
{
	m_isSpinning = spinningState;
}

bool Disk::shouldSpinDownIfIdle()
{
	return m_shouldSpinDownIfIdle;
}

ostream & operator<<(ostream &os, Disk &disk)
{
	DeviceUsage deviceUsage = {0, 0, 0};
	disk.getAvrgUsage(&deviceUsage);

	os << disk.getDeviceName() << " -" << (disk.getIdleState() ? " idle " : " busy") << "\n";
	os << "Load: " << deviceUsage.load <<  "%, Read - " << deviceUsage.totalRead << "KB/s, Written - " << deviceUsage.totalWritten << "KB/s.\n";

	return os;
}



