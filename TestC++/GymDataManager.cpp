#include "GymDataManager.h"
#include <fstream>
#include <sstream>

using namespace std;

GymDataManager::GymDataManager()
	: visits(new OrderedLinkedList())
{
}

GymDataManager::~GymDataManager()
{
	delete[] visits;
}

//метод вызывает упорядоченное добавление
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

//метод преобразования строк файла в поля и добавления в список
int GymDataManager::loadFromFile(const string& filename)
{
	ifstream file(filename); //открытие файла
	if (!file.is_open())
	{
		return 0;
	}

	int loadedCount = 0;
	string line;

	while (getline(file, line)) //проход по всем строкам файла и запись в поля посещения
	{
		GymVisit newVisit;
		istringstream iss(line);
		string token;

		//проверки, что на линиях есть точка с запятой
		if (!getline(iss, token, ';')) 
		{
			continue;
		}
		//преобразования к нужным типам данных полученные символы до ';'
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
