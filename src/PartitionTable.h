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

#ifndef PARTITIONTABLE_H_
#define PARTITIONTABLE_H_

#include <map>

using namespace std;

typedef map<int, string> Partitions;
typedef pair<string, Partitions> DiskTree;
typedef map<int, DiskTree> PartitionsTable;

typedef PartitionsTable::const_iterator PartitionsTableIterator;
typedef Partitions::const_iterator PartitionsIterator;

typedef pair<PartitionsTableIterator, bool> PartitionsTableInsert;
typedef pair<PartitionsIterator, bool> PartitionsInsert;

class PartitionTable
{
private:
	PartitionsTable m_partitionTable;

	bool insertDisk(int majorId, const string &diskName);
	bool insertPartition(int majorId, int minorId, const string &partitionName);

public:
	PartitionTable();
	bool loadPartitionTable();
	bool isPartitionValid(const string &partitionName);
	bool isDiskValid(const string &diskName);
};

#endif
