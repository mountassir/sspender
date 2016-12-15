/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * PartitionTable.h: Part of sspender
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
#include <iostream>

#include "utils.h"

using namespace std;

typedef map<string, std::vector<string>> PartitionsTable;

typedef pair<string, std::vector<string>> DiskKey;

typedef PartitionsTable::iterator PartitionsTableIterator;
typedef PartitionsTable::const_iterator PartitionsTableConstIterator;

typedef pair<PartitionsTableIterator, bool> PartitionsTableInsert;

class PartitionTable
{
private:
	PartitionsTable m_partitionTable;
	vector<string> m_allValidBlocks;

	bool insertDisk(const string &diskName);
	bool insertPartition(const string &diskName, const string &partitionName);

public:
	void loadPartitionTable();
	bool isPartitionValid(const string &partitionName, string *diskName = NULL);
	bool isDiskValid(const string &diskName);
	bool isBlockValid(const string &blockName);
	bool isRealDisk(const string &blockName);
	void getAllDisks(vector<string> *disks);
	void getAllPartitions(vector<string> *partitions);
	const PartitionsTable* getPartitionsTable() {return &m_partitionTable;};
};

ostream & operator<<(ostream &os, PartitionTable &partitionTable);

#endif
