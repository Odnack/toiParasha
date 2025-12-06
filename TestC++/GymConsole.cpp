#include "GymConsole.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

void printHeader()
{
	cout << "------------------------------------------------------------------------------" << endl;
	cout << "#  | Номер посещения | ФИО                      | Дата и время    | Адрес зала" << endl;
	cout << "------------------------------------------------------------------------------" << endl;
}

void printAggregateHeader() {
	cout << "------------------------------------------------------------------------------------------------------------" << endl;
	cout << "#  | Номер посещения | ФИО                      | Дата и время    | Адрес зала       | Количество посещений" << endl;
	cout << "-----------------------------------------------------------------------------------------------------------" << endl;

}

GymVisit GymConsole::inputVisitData() const
{
	GymVisit visit;

	cout << "Введите номер посещения: ";
	cin >> visit.visitId;
	cin.ignore();

	cout << "Введите ФИО: ";
	getline(cin, visit.fullName);

	cout << "Введите дату и время (в формате: 'год месяц день час минута'): ";
	cin >> visit.year >> visit.month >> visit.day >> visit.hour >> visit.minute;
	cin.ignore();

	cout << "Введите адрес зала: ";
	getline(cin, visit.gymAddress);

	visit.isDeleted = false;
	return visit;
}

void GymConsole::displayVisit(const GymVisit& visit, int number) const
{
	if (visit.isDeleted)
	{
		return;
	}

	cout << setw(5) << left <<  to_string(number) + " |"
		<< setw(16) << left << visit.visitId << " | "
		<< setw(22) << left << visit.fullName << " | "
		<< setw(16) << left << visit.getFormattedDateTime() << " | "
		<< visit.gymAddress << endl;
}

void GymConsole::displayVisitWithCount(const GymVisit &visit, int number, unsigned short count) const {
  cout << setw(5) << left << to_string(number) + " |"
      << setw(16) << left << visit.visitId << " | "
      << setw(22) << left << visit.fullName << " | "
      << setw(16) << left << visit.getFormattedDateTime() << " | "
      << setw(16) << visit.gymAddress << " | "
      << count << endl;
}

void GymConsole::handleInputData()
{
	GymVisit newVisit = inputVisitData();
	dataManager.addVisit(newVisit);
	cout << "Запись успешно добавлена" << endl;
}

void GymConsole::handleDisplayData() const
{
	if (dataManager.getRecordCount() == 0)
	{
		cout << "Нет данных для отображения" << endl;
		return;
	}

	cout << "Список посещений:" << endl;
	printHeader();

	int count = 1;
	const GymVisit* visits = dataManager.getVisits();
	for (int i = 0; i < dataManager.getRecordCount(); i++)
	{
		const GymVisit& visit = visits[i];
		if (!visit.isDeleted)
		{
			displayVisit(visit, count);
			count++;
		}
	}
}

void GymConsole::handleSaveToFile() const
{
	if (dataManager.getActiveRecordCount() == 0)
	{
		cout << "Нет данных для сохранения" << endl;
		return;
	}

	string filename;
	cout << "Введите имя файла: ";
	getline(cin, filename);

	int mode;
	cout << "Выберите режим (1 - новый файл, 2 - дополнение): ";
	cin >> mode;
	cin.ignore();

	if (dataManager.trySaveToFile(filename, mode == 2))
	{
		cout << "Данные сохранены в файл: " << filename << endl;
	}
	else
	{
		cout << "Ошибка сохранения файла" << endl;
	}
}

void GymConsole::handleLoadFromFile()
{
	string filename;
	cout << "Введите имя файла: ";
	getline(cin, filename);

	int loadedCount = dataManager.loadFromFile(filename);
	if (loadedCount >= 0)
	{
		cout << "Загружено записей: " << loadedCount << endl;
	}
	else
	{
		cout << "Ошибка загрузки файла" << endl;
	}
}

void GymConsole::handleBuildIndices()
{
	if (dataManager.getRecordCount() == 0)
	{
		cout << "Нет данных для построения индексов" << endl;
		return;
	}

	dataManager.buildNameIndex();
	dataManager.buildVisitIdIndex();
	dataManager.buildVisitCountIndex();
	cout << "Индексы построены" << endl;
}

void GymConsole::displayByNameAsc() const
{
	const AVLTreeIndex* index = GymConsole::dataManager.getNameIndex();
	if (index == nullptr)
	{
		cout << "Индекс по ФИО не построен" << endl;
		return;
	}

	cout << "Данные отсортированные по ФИО по возрастанию:" << endl;
	printHeader();
	const GymVisit* visits = dataManager.getVisits();

	index->displayAsc(visits, this, &GymConsole::displayVisit);
}

void GymConsole::displayByNameDesc() const
{
	const AVLTreeIndex* index = GymConsole::dataManager.getNameIndex();
	if (index == nullptr)
	{
		cout << "Индекс по ФИО не построен" << endl;
		return;
	}

	cout << "Данные отсортированные по ФИО по убыванию:" << endl;
	printHeader();
	const GymVisit* visits = dataManager.getVisits();

	index->displayDesc(visits, this, &GymConsole::displayVisit);
}

void GymConsole::displayByVisitIdAsc() const
{
	const VisitIdIndex* index = GymConsole::dataManager.getVisitIdIndex();
	if (index == nullptr)
	{
		cout << "Индекс по номеру посещения не построен" << endl;
		return;
	}

	cout << "Данные отсортированные по номеру посещения (по возрастанию):" << endl;
	printHeader();
	const GymVisit* visits = dataManager.getVisits();
	int count = 1;
	for (int i = 0; i < dataManager.getRecordCount(); i++)
	{
		int originalIndex = index[i].originalIndex;
		const GymVisit& visit = visits[originalIndex];
		if (!visit.isDeleted) {
			displayVisit(visit, count);
			count++;
		}
	}
}

void GymConsole::displayByVisitCountAsc() const
{
	const VisitCountIndex* index = GymConsole::dataManager.getVisitCountIndex();
	if (index == nullptr)
	{
		cout << "Индекс по количеству посещений не построен" << endl;
		return;
	}

	cout << "Данные отсортированные по количеству посещений (по возрастанию):" << endl;
	printAggregateHeader();

	const GymVisit* visits = dataManager.getVisits();
	int count = 1;
	int indexSize = dataManager.getVisitCountIndexSize();
	for (int i = 0; i < indexSize; i++)
	{
		int originalIndex = index[i].originalIndex;
		const GymVisit& visit = visits[originalIndex];
		if (!visit.isDeleted)
		{
			GymConsole::displayVisitWithCount(visit, count, index[i].visitCount);
			count++;
		}
	}
}

void GymConsole::displayByVisitCountDesc() const
{
	const VisitCountIndex* index = GymConsole::dataManager.getVisitCountIndex();
	if (index == nullptr)
	{
		cout << "Индекс по количеству посещений не построен" << endl;
		return;
	}

	cout << "Данные отсортированные по количеству посещений (по убыванию):" << endl;
	printAggregateHeader();

	const GymVisit* visits = dataManager.getVisits();
	int count = 1;
	int indexSize = dataManager.getVisitCountIndexSize();
	for (int i = indexSize - 1; i >= 0; i--)
	{
		int originalIndex = index[i].originalIndex;
		const GymVisit& visit = visits[originalIndex];
		if (!visit.isDeleted)
		{
			GymConsole::displayVisitWithCount(visit, count, index[i].visitCount);
			count++;
		}
	}
}

void GymConsole::searchByName() const
{
	const AVLTreeIndex* index = dataManager.getNameIndex();
	if (index == nullptr)
	{
		cout << "Индекс по ФИО не построен!" << endl;
		return;
	}

	char targetName[100];
	int searchType;

	cout << "Введите ФИО для поиска: ";
	cin.getline(targetName, 100);
	cout << "Выберите вариант поиска (1 - Рекурсивный, 2 - Итерационный): ";
	cin >> searchType;
	cin.ignore();

	AVLNode* foundNode = nullptr;

	if (searchType == 1) {
		foundNode = dataManager.findByNameRecursive(targetName);
		cout << "Использован рекурсивный поиск" << endl;
	}
	else if (searchType == 2) {
		foundNode = dataManager.findByNameIterative(targetName);
		cout << "Использован итерационный поиск" << endl;
	}
	else {
		cout << "Неверный выбор типа поиска!" << endl;
		return;
	}

	if (foundNode == nullptr)
	{
		cout << "Запись с ФИО '" << targetName << "' не найдена" << endl;
	}
	else
	{
		const GymVisit* visits = dataManager.getVisits();

		cout << endl << "Все записи с ФИО '" << targetName << "':" << endl;
		printHeader();

		int count = 1;
		displayVisit(visits[foundNode->originalIndex], count++);

		AVLNode* current = foundNode->right;
		while (current != nullptr) {
			if (current->fullName == targetName) {
				displayVisit(visits[current->originalIndex], count++);
				current = current->right;
			}
			else {
				break;
			}
		}
	}
}

void GymConsole::searchVisitId() const
{
	const VisitIdIndex* index = dataManager.getVisitIdIndex();

	if (index == nullptr)
	{
		cout << "Индекс по номеру посещения не построен!" << endl;
		return;
	}

	int visitId;
	cout << "Введите номер посещения для поиска: ";
	cin >> visitId;
	cin.ignore();

	int searchIndex = dataManager.binarySearchByVisitId(visitId);

	if (searchIndex == -1)
	{
		cout << "Запись с номером посещения " << visitId << " не найдены" << endl;
	}
	else
	{
		int originalIndex = index[searchIndex].originalIndex;
		printHeader();
		displayVisit(dataManager.getVisits()[originalIndex], 1);
	}
}

void GymConsole::searchVisitCount() const
{
	const VisitCountIndex* index = dataManager.getVisitCountIndex();

	if (index == nullptr)
	{
		cout << "Индекс по количеству посещений не построен!" << endl;
		return;
	}

	int visitCount;
	cout << "Введите количество посещений для поиска: ";
	cin >> visitCount;
	cin.ignore();

	int searchIndex = dataManager.binarySearchByVisitCount(visitCount);

	if (searchIndex == -1)
	{
		cout << "Запись с количество посещений " << visitCount << " не найдены" << endl;
	}
	else
	{
		const GymVisit* visits = dataManager.getVisits();

		cout << endl << "Все записи  с количество посещений '" << visitCount << "':" << endl;
		printAggregateHeader();

		int currentIndex = searchIndex;
		while (currentIndex > 0 && index[currentIndex - 1].visitCount == visitCount)
		{
			currentIndex--;
		}

		int count = 1;
		while (currentIndex < dataManager.getVisitCountIndexSize() && index[currentIndex].visitCount == visitCount)
		{
			int originalIndex = index[currentIndex].originalIndex;
			const GymVisit& visit = visits[originalIndex];
			if (!visit.isDeleted)
			{
				GymConsole::displayVisitWithCount(visit, count, index[currentIndex].visitCount);
				count++;
			}
			currentIndex++;
		}
	}
}

void GymConsole::markForDeletion()
{
	int activeRecordsCount = dataManager.getActiveRecordCount();
	if (activeRecordsCount == 0)
	{
		cout << "Нет данных для удаления" << endl;
		return;
	}

	handleDisplayData();

	int recordNum;
	cout << "Введите номер записи для удаления: ";
	cin >> recordNum;
	cin.ignore();

	if (recordNum < 1 || recordNum > activeRecordsCount)
	{
		cout << "Неверный номер записи!" << endl;
		return;
	}

	int actualIndex = -1;
	int count = 0;
	for (int i = 0; i < dataManager.getRecordCount(); i++)
	{
		if (!dataManager.getVisits()[i].isDeleted)
		{
			count++;
			if (count == recordNum)
			{
				actualIndex = i;
				break;
			}
		}
	}

	if (actualIndex == -1)
	{
		cout << "Запись не найдена!" << endl;
		return;
	}

	if (dataManager.tryMarkVisitDeleted(actualIndex))
	{
		cout << "Запись помечена на удаление" << endl;
	}
	else
	{
		cout << "Запись уже помечена на удаление" << endl;
	}
}

void GymConsole::restoreRecord()
{
	if (dataManager.getRecordCount() == 0)
	{
		cout << "Нет данных для восстановления" << endl;
		return;
	}

	cout << "Удаленные записи:" << endl;
	int deletedCount = 0;
	for (int i = 0; i < dataManager.getRecordCount(); i++)
	{
		if (dataManager.getVisits()[i].isDeleted)
		{
			const GymVisit& visit = dataManager.getVisits()[i];
			cout << (deletedCount + 1) << ". Номер записи: " << visit.visitId
				<< " | ФИО: " << visit.fullName
				<< " | Дата: " << visit.getFormattedDate() << endl;
			deletedCount++;
		}
	}

	if (deletedCount == 0)
	{
		cout << "Нет удаленных записей" << endl;
		return;
	}

	int recordNum;
	cout << "Введите номер записи для восстановления: ";
	cin >> recordNum;
	cin.ignore();

	if (recordNum < 1 || recordNum > deletedCount)
	{
		cout << "Неверный номер записи" << endl;
		return;
	}

	int found = 0;
	for (int i = 0; i < dataManager.getRecordCount(); i++)
	{
		if (dataManager.getVisits()[i].isDeleted)
		{
			found++;
			if (found == recordNum)
			{
				if (dataManager.tryRestoreVisit(i))
				{
					cout << "Запись восстановлена" << endl;
				}
				else
				{
					cout << "Ошибка восстановления записи" << endl;
				}
				break;
			}
		}
	}
}

void GymConsole::physicalDeletion()
{
	int deletedCount = 0;
	for (int i = 0; i < dataManager.getRecordCount(); i++)
	{
		if (dataManager.getVisits()[i].isDeleted)
		{
			deletedCount++;
		}
	}

	if (deletedCount == 0)
	{
		cout << "Нет записей для физического удаления" << endl;
		return;
	}

	cout << "Будет удалено " << deletedCount << " записей. Продолжить? (д/н): ";
	char confirm;
	cin >> confirm;
	cin.ignore();

	if (confirm == 'д' || confirm == 'Д')
	{
		dataManager.physicalDeleteMarked();

		dataManager.buildNameIndex();
		dataManager.buildVisitIdIndex();
		dataManager.buildVisitCountIndex();

		cout << "Физически удалено записей: " << deletedCount << endl;
	}
	else
	{
		cout << "Операция отменена" << endl;
	}
}

void GymConsole::handleDisplayByIndices() const
{
	if (dataManager.getRecordCount() == 0)
	{
		cout << "Нет данных для отображения" << endl;
		return;
	}

	int subchoice;
	do {
		showDisplaySubmenu();
		cin >> subchoice;
		cin.ignore();

		switch (subchoice)
		{
		case 1:
			GymConsole::displayByNameAsc();
			break;
		case 2:
			GymConsole::displayByNameDesc();
			break;
		case 3:
			GymConsole::displayByVisitIdAsc();
			break;
		case 4:
			GymConsole::displayByVisitCountAsc();
			break;
		case 5:
			GymConsole::displayByVisitCountDesc();
			break;
		case 0:
			break;
		default:
			cout << "Неверный выбор" << endl;
		}
	} while (subchoice != 0);
}

void GymConsole::handleSearch() const
{
	if (dataManager.getRecordCount() == 0)
	{
		cout << "Нет данных для поиска" << endl;
		return;
	}

	int subchoice;
	do {
		showSearchSubmenu();
		cin >> subchoice;
		cin.ignore();

		switch (subchoice) {
		case 1:
			searchByName();
			break;
		case 2:
			searchVisitId();
			break;
		case 3:
			searchVisitCount();
			break;
		case 0:
			break;
		default:
			cout << "Неверный выбор" << endl;
		}
	} while (subchoice != 0);
}

void GymConsole::handleEditRecord()
{
	if (dataManager.getRecordCount() == 0)
	{
		cout << "Нет данных для редактирования" << endl;
		return;
	}

	handleDisplayData();

	int recordNum;
	cout << "Введите номер записи для редактирования: ";
	cin >> recordNum;
	cin.ignore();

	if (recordNum < 1 || recordNum > dataManager.getActiveRecordCount())
	{
		cout << "Неверный номер записи" << endl;
		return;
	}

	int actualIndex = -1;
	int count = 0;
	for (int i = 0; i < dataManager.getRecordCount(); i++)
	{
		if (!dataManager.getVisits()[i].isDeleted)
		{
			count++;
			if (count == recordNum) {
				actualIndex = i;
				break;
			}
		}
	}

	if (actualIndex == -1)
	{
		cout << "Запись не найдена" << endl;
		return;
	}

	const GymVisit& currentVisit = dataManager.getVisits()[actualIndex];

	cout << "Текущие данные:" << endl;
	cout << "1. Номер посещения: " << currentVisit.visitId << endl;
	cout << "2. ФИО: " << currentVisit.fullName << endl;
	cout << "3. Дата и время: " << currentVisit.getFormattedDateTime() << endl;
	cout << "4. Адрес зала: " << currentVisit.gymAddress << endl;

	int field;
	cout << "Выберите поле для редактирования (1-4): ";
	cin >> field;
	cin.ignore();

	GymVisit newData = currentVisit;

	switch (field)
	{
	case 1:
		cout << "Введите новый номер посещения: ";
		cin >> newData.visitId;
		cin.ignore();
		break;
	case 2:
		cout << "Введите новое ФИО: ";
		getline(cin, newData.fullName);
		break;
	case 3:
		cout << "Введите дату и время (в формате: 'год месяц день час минута'): ";
		cin >> newData.year >> newData.month >> newData.day >> newData.hour >> newData.minute;
		cin.ignore();
		break;
	case 4:
		cout << "Введите новый адрес зала: ";
		getline(cin, newData.gymAddress);
		break;
	default:
		cout << "Неверный выбор поля" << endl;
		return;
	}

	if (dataManager.tryEditVisit(actualIndex, newData))
	{
		cout << "Запись успешно отредактирована" << endl;
	}
	else
	{
		cout << "Ошибка редактирования записи" << endl;
	}
}

void GymConsole::handleDeletionManagement()
{
	if (dataManager.getRecordCount() == 0)
	{
		cout << "Нет данных для управления" << endl;
		return;
	}

	int subchoice;
	do {
		showDeletionSubmenu();
		cin >> subchoice;
		cin.ignore();

		switch (subchoice)
		{
		case 1:
			markForDeletion();
			break;
		case 2:
			restoreRecord();
			break;
		case 3:
			physicalDeletion();
			break;
		case 0:
			break;
		default:
			cout << "Неверный выбор!" << endl;
		}
	} while (subchoice != 0);
}

void GymConsole::showDisplaySubmenu() const
{
	cout << endl << "---Вывод данных по индексам---" << endl;
	cout << "1. По ФИО (по возрастанию)" << endl;
	cout << "2. По ФИО (по убыванию)" << endl;
	cout << "3. По номеру посещения (по возрастанию)" << endl;
	cout << "4. По количеству посещений (по возрастанию)" << endl;
	cout << "5. По количеству посещений (по убыванию)" << endl;
	cout << "0. Назад" << endl;
	cout << "Выберите действие: ";
}

void GymConsole::showSearchSubmenu() const
{
	cout << endl << "---Поиск данных---" << endl;
	cout << "1. По ФИО" << endl;
	cout << "2. По номеру посещения" << endl;
	cout << "3. По количеству посещений" << endl;
	cout << "0. Назад" << endl;
	cout << "Выберите действие: ";
}

void GymConsole::showDeletionSubmenu() const
{
	cout << endl << "---Управление удалением записей---" << endl;
	cout << "1. Пометить запись на удаление" << endl;
	cout << "2. Восстановить запись" << endl;
	cout << "3. Физическое удаление помеченных записей" << endl;
	cout << "0. Назад" << endl;
	cout << "Выберите действие: ";
}

void GymConsole::run()
{
	int choice;

	cout << "------------------------------------------" << endl;
	cout << "   Система учета посещений спортзала    " << endl;
	cout << "------------------------------------------" << endl;

	do {
		cout << endl << "--- Главное меню ---" << endl;
		cout << "1. Ввод данных с клавиатуры" << endl;
		cout << "2. Вывод данных на экран" << endl;
		cout << "3. Сохранение данных в файл" << endl;
		cout << "4. Загрузка данных из файла" << endl;
		cout << "5. Построить индексы" << endl;
		cout << "6. Вывод данных по индексам" << endl;
		cout << "7. Поиск данных" << endl;
		cout << "8. Редактирование записи" << endl;
		cout << "9. Управление удалением записей" << endl;
		cout << "0. Выход" << endl;
		cout << "Выберите действие: ";

		cin >> choice;
		cin.ignore();

		switch (choice)
		{
		case 1:
			handleInputData();
			break;
		case 2:
			handleDisplayData();
			break;
		case 3:
			handleSaveToFile();
			break;
		case 4:
			handleLoadFromFile();
			break;
		case 5:
			handleBuildIndices();
			break;
		case 6:
			handleDisplayByIndices();
			break;
		case 7:
			handleSearch();
			break;
		case 8:
			handleEditRecord();
			break;
		case 9:
			handleDeletionManagement();
			break;
		case 0:
			cout << "Выход из программы" << endl;
			break;
		default:
			cout << "Неверный выбор" << endl;
		}
		cin.clear();
	} while (choice != 0);
}