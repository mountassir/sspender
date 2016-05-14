#ifndef CPU_H_
#define CPU_H_

#include "Device.h"
#include "utils.h"

using namespace std;

class Cpu : public Device {

private:

public:
	Cpu(const string &diskName) :
		Device(diskName) { };

	virtual void initDevice();
	virtual void monitorUsage();
	virtual string getStatesFileName();
	virtual void calculateUsage(ifstream &statesFile, DeviceUsage *cpukUsage);

protected:
	void sumCpuJiffies(const vector<string> &fileOutput, int *workJiffies, int *totalJiffies);
};

#endif
