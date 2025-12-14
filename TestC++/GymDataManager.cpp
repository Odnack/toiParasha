#include "GymDataManager.h"
#include <fstream>
#include <sstream>

using namespace std;

GymDataManager::GymDataManager()
	: visits(nullptr)
{
}

GymDataManager::~GymDataManager()
{
	delete[] visits;
}

void GymDataManager::addVisit(const GymVisit& visit)
{
	visits->addOrdered(visit);
}

bool GymDataManager::tryEditVisit(int index, const GymVisit& newData)
{
	if (visits->remove(index))
	{
		visits->addOrdered(newData);
		return true;
	}
	return false;
}

bool GymDataManager::tryMarkVisitDeleted(int index)
{
	return visits->tryMarkDeleted(index);
}

bool GymDataManager::tryRestoreVisit(int index)
{
	return visits->tryRestore(index);
}

void GymDataManager::physicalDeleteMarked()
{
	visits->removeMarked();
}

int GymDataManager::getActiveRecordCount() const
{
	int count = 0;
	int recordCount = getRecordCount();
	ListNode* current = visits->getForwardLinear();

	for (int i = 0; i < recordCount; i++)
	{
		GymVisit visit = current->visit;

		if (!visit.isDeleted)
		{
			count++;
		}
		current = current->next;
	}

	return count;
}

GymVisit* GymDataManager::findByName(const string& targetName, int& outSize) const
{
	return visits->findAll(targetName, outSize);
}

bool GymDataManager::trySaveToFile(const string& filename, bool append) const
{
	ofstream file;
	ListNode* current = visits->getForwardLinear();
	if (current == nullptr)
	{
		return false;
	}
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

	int recordCount = getRecordCount();
	for (int i = 0; i < recordCount; i++)
	{
		GymVisit visit = current->visit;
		if (!visit.isDeleted)
		{
			file << visit.visitId << ";"
				<< visit.fullName << ";"
				<< visit.day << ";"
				<< visit.month << ";"
				<< visit.year << ";"
				<< visit.hour << ";"
				<< visit.minute << ";"
				<< visit.gymAddress << endl;
		}
		current = current->next;
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
