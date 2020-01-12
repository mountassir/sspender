/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * PartitionTable.cpp: Part of sspender
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

bool PartitionTable::isPartitionValid(const string &partitionName, string *parentDisk /*NULL*/)
{
	PartitionsTableConstIterator disksIterator = m_partitionTable.begin();

	while(disksIterator != m_partitionTable.end())
	{
		for(size_t i = 0, len = disksIterator->second.size(); i < len; ++i)
		{
			if(disksIterator->second[i] == partitionName)
			{
				if(parentDisk)
				{
					*parentDisk = disksIterator->first;
				}

				return true;
			}
		}

		disksIterator++;
	}

	return false;
}

bool PartitionTable::isDiskValid(const string &diskName)
{
	PartitionsTableConstIterator disksIterator = m_partitionTable.begin();

	while(disksIterator != m_partitionTable.end())
	{
		if(disksIterator->first == diskName)
		{
			return true;
		}

		disksIterator++;
	}

	return false;
}

bool PartitionTable::isBlockValid(const string &blockName)
{
	for(size_t i = 1, size = m_allValidBlocks.size(); i < size; ++i)
	{
		if(m_allValidBlocks[i].compare(blockName) == 0)
		{
			return true;
		}
	}

	return false;
}

bool PartitionTable::isRealDisk(const string &blockName)
{
	//return blockName.compare("block") != 0;
	/*
	 * disk nvme0n1 is under nvme0
	 * sda disk is under block
	 * so this will not work
	 */

	return true;
}

void PartitionTable::loadPartitionTable()
{
	vector<string> fileContent;
	string fileName = "/proc/partitions";

	parseFile(fileName, &fileContent);

	for(size_t i = 1, size = fileContent.size(); i < size; ++i)
	{
		string line = fileContent[i];
		vector<string> splitLine;

		splitByEmptySpace(line, &splitLine);

		if(splitLine.size() == 4)
		{
			m_allValidBlocks.push_back(splitLine[3]);
		}
	}

	for(size_t i = 0, size = m_allValidBlocks.size(); i < size; ++i)
	{
		string parentDisk;

		parentDiskOfBlockDevice(m_allValidBlocks[i], &parentDisk);

		if(isBlockValid(parentDisk))
		{
			insertPartition(parentDisk, m_allValidBlocks[i]);
		}
		else
		{
			if(isRealDisk(parentDisk))
			{
				insertDisk(m_allValidBlocks[i]);
			}
		}
	}
}

bool PartitionTable::insertDisk(const string &diskName)
{
	PartitionsTableConstIterator diskIter = m_partitionTable.find(diskName);

	if(diskIter != m_partitionTable.end())
	{
		cout << "Disk '" << diskName << "' already exists\n";
	}
	else
	{
		std::vector<string> partirions;

		DiskKey newDisk (diskName, partirions);

		PartitionsTableInsert insert = m_partitionTable.insert(newDisk);

		return insert.second;
	}

	return false;
}

bool PartitionTable::insertPartition(const string &diskName, const string &partitionName)
{
	PartitionsTableIterator diskIter = m_partitionTable.find(diskName);

	if(diskIter != m_partitionTable.end())
	{
		diskIter->second.push_back(partitionName);

		return true;
	}
	else
	{
		cout << "Disk '" << diskName << "' was not found\n";
	}

	return false;
}

void PartitionTable::getAllDisks(vector<string> *disks)
{
	PartitionsTableConstIterator disksIterator = m_partitionTable.begin();

	while(disksIterator != m_partitionTable.end())
	{
		disks->push_back(disksIterator->first);

		disksIterator++;
	}
}

void PartitionTable::getAllPartitions(vector<string> *partitions)
{
	PartitionsTableConstIterator disksIterator = m_partitionTable.begin();

	while(disksIterator != m_partitionTable.end())
	{
		for(size_t i = 0, len = disksIterator->second.size(); i < len; ++i)
		{
			partitions->push_back(disksIterator->second[i]);
		}

		disksIterator++;
	}
}

ostream & operator<<(ostream &os, PartitionTable &partitionTable)
{
	const PartitionsTable *table = partitionTable.getPartitionsTable();

	PartitionsTableConstIterator disksIterator = table->begin();

	while(disksIterator != table->end())
	{
		os << "-" << disksIterator->first << endl;

		for(size_t i = 0, len = disksIterator->second.size(); i < len; ++i)
		{
			os << " |_" << disksIterator->second[i] << endl;
		}

		disksIterator++;
	}

	return os;
}

