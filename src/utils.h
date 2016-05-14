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

using namespace std;

void getAllDisksAndPartitions(vector<string> *disks, vector<string> *partitions);
bool convertTimeToMinutes(string time, double *totalMinutes);
bool getCurremtTimeInMinutes(double *totalMinutes);
void printHeaderMessage(const string &message, bool logTime);
void splitByEmptySpace(const string &input, vector<string> *output);
bool runSystemCommand(const string &command, vector<string> *output = 0);
double getDoubleFromSystemCall(const string &command);
string trimString(string input);
void splitStringByDelimiter(vector<string> *output,
		                    const string &input,
		                    const string &delimiter);
void parseFile(const string & fileName, vector<string>* fileOutput);
void parseFile(ifstream &file, vector<string>* fileOutput);
string getCurrentDateTime();

#endif
