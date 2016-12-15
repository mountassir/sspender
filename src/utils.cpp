/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * utils.cpp: Part of sspender
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

#include "utils.h"


bool uuidToDiskName(const string &uuid, string *diskName)
{
	string command = "ls -l /dev/disk/by-uuid/" + uuid + " | awk '{print $NF}'";

	vector<string> output;

	bool commandExecuted = runSystemCommand(command, &output);

	if(commandExecuted && output.size() > 0)
	{
		string firstLine = output[0];

		vector<string> splitLine;

		splitStringByDelimiter(&splitLine, firstLine, "/");

		if(splitLine.size() == 3)
		{
			*diskName = splitLine[splitLine.size() -1 ];
		}
		else
		{
			cout << firstLine << "\n";
		}
	}

	return false;
}

bool parentDiskOfBlockDevice(const string &blockDevice, string *diskName)
{
	string command = "readlink /sys/class/block/" + blockDevice;

	vector<string> output;

	bool commandExecuted = runSystemCommand(command, &output);

	if(commandExecuted && output.size() > 0)
	{
		string firstLine = output[0];

		vector<string> splitLine;

		splitStringByDelimiter(&splitLine, firstLine, "/");

		if(splitLine.size() > 1)
		{
			*diskName = splitLine[splitLine.size() - 2 ];
		}
		else
		{
			cout << "Could not read link " << command << "\n";
		}
	}

	return false;
}

bool convertTimeToMinutes(string time, double *totalMinutes)
{
	replace(time.begin(), time.end(), ':', ' ');

	vector<string> splitTime;
	splitByEmptySpace(time, &splitTime);

	if(splitTime.size() < 1)
	{
		cout << "Failed to convert time to minutes: " << time << endl;
		return false;
	}

	*totalMinutes = (atoi(splitTime[0].c_str()) * 60) + atoi(splitTime[1].c_str());

	return true;
}

void getCurremtTimeInMinutes(double *totalMinutes)
{
    time_t currentTime = time(0);

    struct tm *now = localtime(&currentTime);

    *totalMinutes = (now->tm_hour * 60) + now->tm_min;
}

void printHeaderMessage(const string &message, bool logTime)
{
	if(logTime)
	{
		cout << "=====|| " << message << " | " << getCurrentDateTime() << endl;
	}
	else
	{
		cout << "\n==========|| " << message << endl;
	}
}

void splitByEmptySpace(const string &input, vector<string> *output)
{
	istringstream iss(input);

	copy(istream_iterator<string>(iss),
		 istream_iterator<string>(),
		 back_inserter(*output));
}

string trimString(string input)
{
	string::const_iterator bItr = input.begin();

	while(bItr != input.end() && (isspace(*bItr) || iscntrl(*bItr) || isblank(*bItr)))
	{
		++bItr;
	}

	string::const_reverse_iterator eItr = input.rbegin();

	while(eItr.base() != bItr && (isspace(*eItr) || iscntrl(*eItr) || isblank(*eItr)))
	{
		eItr++;
	}

	string trimedString = string(bItr, eItr.base());

	return trimedString;
}

bool runSystemCommand(const string &command, vector<string> *output /* = null*/)
{
	FILE *in;
	char buff[512];

	if(!(in = popen(command.c_str(), "r")))
	{
		return false;
	}

	if(output)
	{
		while(fgets(buff, sizeof(buff), in) != NULL)
		{
			buff[strcspn(buff, "\n")] = 0;

			output->push_back(buff);
		}
	}

	pclose(in);
	return true;
}

double getDoubleFromSystemCall(const string &command)
{
	FILE *in;
	char buff[512];

	if(!(in = popen(command.c_str(), "r")))
	{
		return -1;
	}

	double value;

	while(fgets(buff, sizeof(buff), in) != NULL)
	{
		value = atof(string(buff).c_str());
	}

	pclose(in);
	return value;
}

double roundValue(double value)
{
	return floor( value * 100.00 + 0.5 ) / 100.00;
}

double getMinutesDuration(const TimePoint& startTime)
{
	auto endTime = Clock::now();

	return chrono::duration_cast<chrono::minutes>(endTime - startTime).count();
}

void splitStringByDelimiter(vector<string> *output,
		                    const string &input,
		                    const string &delimiter)
{
	size_t last = 0;
	size_t next = 0;

	while ((next = input.find(delimiter, last)) != string::npos)
	{
		output->push_back(input.substr(last, next-last));
		last = next + 1;
	}

	output->push_back(input.substr(last));
}

string getCurrentDateTime()
{
	vector<string> dateOutput;

	runSystemCommand("date", &dateOutput);

	dateOutput[0].erase(std::remove(dateOutput[0].begin(), dateOutput[0].end(), '\n'), dateOutput[0].end());

	return dateOutput[0];
}

void parseFile(const string &fileName, vector<string>* fileOutput)
{
	string line;

	ifstream file (fileName.c_str());

	if (file.is_open())
	{
		while ( getline(file,line) )
		{
			fileOutput->push_back(line);
		}

		file.close();
	}
	else
	{
		cout << "Could not open " << fileName << endl;
	}
}

void parseFile(ifstream &file, vector<string>* fileOutput)
{
	string line;

	if (file.is_open())
	{
		file.clear();
		file.seekg(0, ios::beg);

		while ( getline(file,line) )
		{
			fileOutput->push_back(line);
		}
	}
	else
	{
		cout << "ifstream is not open!!" << endl;
	}
}

string charTostring(const char *input)
{
	ostringstream oss;

	oss << input;

	return oss.str();
}
