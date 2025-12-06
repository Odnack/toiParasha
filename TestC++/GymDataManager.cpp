#include "GymDataManager.h"
#include <fstream>
#include <sstream>

using namespace std;

GymDataManager::GymDataManager()
	: visits(nullptr), nameIndex(nullptr), visitIdIndex(nullptr),
	recordCount(0), capacity(0)
{
}

GymDataManager::~GymDataManager()
{
	delete[] visits;
	delete[] visitIdIndex;
	delete nameIndex;
}

void GymDataManager::resizeIfNeeded()
{
	if (recordCount >= capacity)
	{
		int newCapacity = capacity == 0 ? 10 : capacity * 2;
		GymVisit* newVisits = new GymVisit[newCapacity];

		if (visits != nullptr)
		{
			for (int i = 0; i < recordCount; i++)
			{
				newVisits[i] = visits[i];
			}
			delete[] visits;
		}

		visits = newVisits;
		capacity = newCapacity;
	}
}

void GymDataManager::addVisit(const GymVisit& visit)
{
	resizeIfNeeded();
	visits[recordCount++] = visit;

	if (nameIndex) 
	{
		nameIndex->insertRecord(visit.fullName, recordCount);
	}
	buildVisitIdIndex();
	buildVisitCountIndex();
}

bool GymDataManager::tryEditVisit(int index, const GymVisit& newData)
{
	if (index < 0 || index >= recordCount)
	{
		return false;
	}

	if (nameIndex) 
	{
		nameIndex->removeRecord(visits[index].fullName, index);
	}

	visits[index] = newData;

	if (nameIndex) 
	{
		nameIndex->insertRecord(newData.fullName, index);
	}
	buildVisitIdIndex();
	buildVisitCountIndex();
	return true;
}

bool GymDataManager::tryMarkVisitDeleted(int index)
{
	if (index < 0 || index >= recordCount || visits[index].isDeleted)
	{
		return false;
	}
	visits[index].isDeleted = true;
	return true;
}

bool GymDataManager::tryRestoreVisit(int index)
{
	if (index < 0 || index >= recordCount || !visits[index].isDeleted)
	{
		return false;
	}
	visits[index].isDeleted = false;
	return true;
}

void GymDataManager::physicalDeleteMarked()
{
	int newCount = 0;
	for (int i = 0; i < recordCount; i++)
	{
		if (!visits[i].isDeleted)
		{
			visits[newCount++] = visits[i];
		}
	}
	recordCount = newCount;
}

int GymDataManager::getActiveRecordCount() const
{
	int count = 0;
	for (int i = 0; i < recordCount; i++)
	{
		if (!visits[i].isDeleted)
		{
			count++;
		}
	}

	return count;
}

void GymDataManager::buildNameIndex()
{
	delete nameIndex; //удаление предыдущего индекса
	nameIndex = new AVLTreeIndex();

	//Цикл добавления записей в индекс, кроме помеченных на удаление
	for (int i = 0; i < recordCount; i++)
	{
		if (!visits[i].isDeleted) {
			nameIndex->insertRecord(visits[i].fullName, i);
		}
	}
}

AVLNode* GymDataManager::findByNameRecursive(const string& targetName) const 
{
	if (!nameIndex) 
	{ 
		return nullptr; 
	}
	return nameIndex->findRecursive(targetName);
}

AVLNode* GymDataManager::findByNameIterative(const string& targetName) const 
{
	if (!nameIndex)
	{
		return nullptr;
	}
	return nameIndex->findIterative(targetName);
}

void GymDataManager::buildVisitCountIndex()
{
	//Жестко костылим, потому что не можем использовать хэш мапы, я люблю свою жизнь
	delete[] visitCountIndex;
	unsigned short* nameCounts = new unsigned short[recordCount]();
	int uniqueNamesCount = 0;
	int* originalIndices = new int[recordCount];
	visitCountIndex = new VisitCountIndex[recordCount];

	for (int i = 0; i < recordCount; i++)
	{
		string currentName = visits[i].fullName;
		unsigned short count = 0;

		for (int j = 0; j < recordCount; j++)
		{
			if (visits[j].fullName == currentName)
			{
				count++;
			}
		}

		visitCountIndex[i] = VisitCountIndex(i, count);
	}

	visitCountIndexSize = recordCount;

	delete[] nameCounts;

	sortVisitCountIndex();
}

void GymDataManager::buildVisitIdIndex()
{
	delete[] visitIdIndex;
	visitIdIndex = new VisitIdIndex[recordCount];

	for (int i = 0; i < recordCount; i++)
	{
		visitIdIndex[i] = VisitIdIndex(i, visits[i].visitId);
	}

	sortVisitIdIndex();
}

void GymDataManager::sortVisitIdIndex()
{
	for (int i = 0; i < recordCount - 1; i++)
	{
		for (int j = 0; j < recordCount - i - 1; j++)
		{
			if (visitIdIndex[j].visitId > visitIdIndex[j + 1].visitId)
			{
				VisitIdIndex temp = visitIdIndex[j];
				visitIdIndex[j] = visitIdIndex[j + 1];
				visitIdIndex[j + 1] = temp;
			}
		}
	}
}

void GymDataManager::sortVisitCountIndex()
{
	for (int i = 0; i < visitCountIndexSize - 1; i++)
	{
		for (int j = 0; j < visitCountIndexSize - i - 1; j++)
		{
			if (visitCountIndex[j].visitCount > visitCountIndex[j + 1].visitCount)
			{
				VisitCountIndex temp = visitCountIndex[j];
				visitCountIndex[j] = visitCountIndex[j + 1];
				visitCountIndex[j + 1] = temp;
			}
		}
	}
}

int GymDataManager::binarySearchByVisitId(int targetVisitId) const
{
	int left = 0, right = recordCount - 1;

	while (left <= right) {
		int mid = left + (right - left) / 2;
		VisitIdIndex midVisitId = visitIdIndex[mid];
		if (midVisitId.visitId == targetVisitId)
		{
			return mid;
		}
		else if (midVisitId.visitId < targetVisitId)
		{
			left = mid + 1;
		}
		else
		{
			right = mid - 1;
		}
	}
	return -1;
}

int GymDataManager::binarySearchByVisitCount(int targetVisitCount) const
{
	int left = 0, right = visitCountIndexSize - 1;
	while (left <= right) {
		int mid = left + (right - left) / 2;
		VisitCountIndex midVisitId = visitCountIndex[mid];
		if (midVisitId.visitCount == targetVisitCount)
		{
			return mid;
		}
		else if (midVisitId.visitCount < targetVisitCount)
		{
			left = mid + 1;
		}
		else
		{
			right = mid - 1;
		}
	}
	return -1;
}

int GymDataManager::binarySearchVisitCountRecursive(int targetVisitCount, int left, int right) const
{
	if (left > right)
	{
		return -1;
	}

	int mid = left + (right - left) / 2;
	VisitCountIndex midVisitId = visitCountIndex[mid];

	if (midVisitId.visitCount == targetVisitCount)
	{
		return mid;
	}
	else if (midVisitId.visitCount < targetVisitCount)
	{
		return binarySearchVisitCountRecursive(targetVisitCount, mid + 1, right);
	}
	else
	{
		return binarySearchVisitCountRecursive(targetVisitCount, left, mid - 1);
	}
}

bool GymDataManager::trySaveToFile(const string& filename, bool append) const
{
	ofstream file;
	if (append)
	{
		file.open(filename, ios::app);
	}
	else
	{
		file.open(filename);
	}

	if (!file.is_open())
	{
		return false;
	}

	for (int i = 0; i < recordCount; i++)
	{
		if (!visits[i].isDeleted)
		{
			file << visits[i].visitId << ";"
				<< visits[i].fullName << ";"
				<< visits[i].day << ";"
				<< visits[i].month << ";"
				<< visits[i].year << ";"
				<< visits[i].hour << ";"
				<< visits[i].minute << ";"
				<< visits[i].gymAddress << endl;
		}
	}

	file.close();
	return true;
}

int GymDataManager::loadFromFile(const string& filename)
{
	ifstream file(filename);
	if (!file.is_open())
	{
		return 0;
	}

	int loadedCount = 0;
	string line;

	while (getline(file, line))
	{
		GymVisit newVisit;
		istringstream iss(line);
		string token;

		if (!getline(iss, token, ';')) 
		{
			continue;
		}
		newVisit.visitId = stoi(token);

		if (!getline(iss, token, ';'))
		{
			continue;
		}
		newVisit.fullName = token;

		if (!getline(iss, token, ';'))
		{
			continue;
		}
		newVisit.day = stoi(token);

		if (!getline(iss, token, ';'))
		{
			continue;
		}
		newVisit.month = stoi(token);

		if (!getline(iss, token, ';'))
		{
			continue;
		}
		newVisit.year = stoi(token);

		if (!getline(iss, token, ';'))
		{
			continue;
		}
		newVisit.hour = stoi(token);

		if (!getline(iss, token, ';'))
		{
			continue;
		}
		newVisit.minute = stoi(token);

		if (!getline(iss, token))
		{
			continue;
		}
		newVisit.gymAddress = token;

		addVisit(newVisit);
		loadedCount++;
	}
	file.close();
	return loadedCount;
}
