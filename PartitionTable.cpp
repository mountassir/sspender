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

#include "PartitionTable.h"

PartitionTable::PartitionTable() {
	// TODO Auto-generated constructor stub

}

bool PartitionTable::isPartitionValid(const string &partitionName)
{
	PartitionsTableIterator disksIterator = m_partitionTable.begin();

	while(disksIterator != m_partitionTable.end())
	{
		PartitionsIterator partitionIterator = disksIterator->second.second.begin();

		while(partitionIterator != disksIterator->second.second.end())
		{
			if(partitionIterator->second == partitionName)
			{
				return true;
			}

			partitionIterator++;
		}

		disksIterator++;
	}

	return false;
}

bool PartitionTable::isDiskValid(const string &diskName)
{
	PartitionsTableIterator disksIterator = m_partitionTable.begin();

	while(disksIterator != m_partitionTable.end())
	{
		if(disksIterator->second.first == diskName)
		{
			return true;
		}

		disksIterator++;
	}

	return false;
}

//bool PartitionTable::loadPartitionTable()
//{
//	vector<string> fileContent;
//	string fileName = "/proc/partitions";
//
//	parseFile(fileName, &fileContent);
//
//	for(size_t i = 1, size = fileContent.size(); i < size; ++i)
//	{
//		string line = fileContent[i];
//		vector<string> splitLine;
//
//		splitByEmptySpace(line, &splitLine);
//
//		if(splitLine.size() == 4)
//		{
//			int deviceMajor = atoi(splitLine[0].c_str());
//			int deviceMinor = atoi(splitLine[1].c_str());
//			string deviceName = splitLine[3];
//
//			if(deviceMinor == 0) //got a disk
//			{
//
//			}
//			else
//			if(deviceMinor > 0) //got a partition
//			{
//
//			}
//		}
//	}
//}

bool PartitionTable::insertDisk(int majorId, const string &diskName)
{
	Partitions emptyPartitionList;
	DiskTree diskTree(diskName, emptyPartitionList);

	//PartitionsTableInsert = m_partitionTable.ins

	return true;
}

bool PartitionTable::insertPartition(int majorId, int minorId, const string &partitionName)
{

}

