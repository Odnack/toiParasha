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

//метод попытки пометить запись на удаление
bool GymDataManager::tryMarkVisitDeleted(GymVisit** visits, int size)
{
	for (int i = 0; i < size; i++)
	{
		GymVisit* visit = visits[i];
		//если запись не помечена на удаление, то помечается, возврат true
		if (!visit->isDeleted) 
		{
			visit->isDeleted = true;
			return true;
		}
	}
	
	//возврат false только в случае неудачной пометки на удаление
	return false;
}

bool GymDataManager::tryRestoreVisit(int index)
{
	return visits->tryRestore(index);
}

//вызов удаления
void GymDataManager::physicalDeleteMarked()
{
	visits->removeMarked();
}

//поучение активных записей
int GymDataManager::getActiveRecordCount() const
{
	int count = 0;
	int recordCount = getRecordCount(); // запись количества элементов списка
	ListNode* current = visits->getForwardLinear(); // запись списка в порядке возрастания 

	for (int i = 0; i < recordCount; i++) // цикл перебора всех элементов
	{
		GymVisit visit = current->visit;

		// если элемент не помечен на удаление, то количество увеличивается
		if (!visit.isDeleted)
		{
			count++;
		}
		// переход к следующему элементу
		current = current->next;
	}

	return count;
}

//вызов поиска по имени
GymVisit** GymDataManager::findByName(const string& targetName, int& outSize, bool isRercursive) const
{
	//если isRercursive true, то вызывается рекурсивный поиск, иначе итерационный
	return isRercursive 
		? visits->findAllRecursive(targetName, outSize)
		: visits->findAll(targetName, outSize);
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
