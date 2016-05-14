#ifndef DISK_H_
#define DISK_H_


#include "Device.h"
#include "utils.h"

using namespace std;

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

class Disk : public Device {

private:
	int m_sectorSize;

public:
	Disk(const string &diskName) :
		Device(diskName), m_sectorSize(0) { };

	virtual void initDevice();
	virtual void monitorUsage();
	virtual string getStatesFileName();
	virtual void calculateUsage(ifstream &statesFile, DeviceUsage *diskUsage);

protected:
	int  getDiskSectorSize(const string &diskName);
	void calculateDiskUsage(double *totalRead, double *totalWrite);
	void updateReadWrite();
	void getDiskStats(ifstream &statesFile, DiskStats *stats);
	void parseDiskStats(const vector<string> &fileOutput, DiskStats *stats);
};

#endif
