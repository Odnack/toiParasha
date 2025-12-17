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

//Метод записи данных нового пользователя
GymVisit GymConsole::inputVisitData() const
{
	GymVisit visit;

	cout << "Введите номер посещения: ";
	cin >> visit.visitId;
	cin.ignore();

	cout << "Введите ФИО: ";
	getline(cin, visit.fullName); //базовая функия чтения с консоли

	cout << "Введите дату и время (в формате: 'год месяц день час минута'): ";
	cin >> visit.year >> visit.month >> visit.day >> visit.hour >> visit.minute;
	cin.ignore();

	cout << "Введите адрес зала: ";
	getline(cin, visit.gymAddress);

	visit.isDeleted = false; //выставление флага удаления
	return visit;
}

//метод вывода посещения
void GymConsole::displayVisit(const GymVisit& visit, int number) const
{
	if (visit.isDeleted)
	{
		return;
	}

	cout << setw(5) << left << to_string(number) + " |"
		<< setw(16) << left << visit.visitId << " | "
		<< setw(22) << left << visit.fullName << " | "
		<< setw(16) << left << visit.getFormattedDateTime() << " | "
		<< visit.gymAddress << endl;
}

void GymConsole::displayVisitWithCount(const GymVisit& visit, int number, unsigned short count) const {
	cout << setw(5) << left << to_string(number) + " |"
		<< setw(16) << left << visit.visitId << " | "
		<< setw(22) << left << visit.fullName << " | "
		<< setw(16) << left << visit.getFormattedDateTime() << " | "
		<< setw(16) << visit.gymAddress << " | "
		<< count << endl;
}

//Организация ввода и записи данных
void GymConsole::handleInputData()
{
	GymVisit newVisit = inputVisitData();
	dataManager.addVisit(newVisit);
	cout << "Запись успешно добавлена" << endl;
}

//метод вывода списка
void GymConsole::handleDisplayData(int choice) const
{
	//getRecordCount() возвращает поличество записей в списке
	//int getRecordCount() const { return visits->getSize(); }
	//int getSize() const { return size; }
	int recordCount = dataManager.getRecordCount();
	const ListNode* current;

	if (recordCount == 0)
	{
		cout << "Нет данных для отображения" << endl;
		return;
	}

	cout << "Список посещений:" << endl;
	printHeader();

	if (choice == 1) {
		current = dataManager.getVisits(); //записывает имеющиеся записи в порядке возрастания
		//ListNode* getVisits() const { return visits->getForwardLinear(); }
		//ListNode* OrderedLinkedList::getForwardLinear() const { return head; } - возвращает первый элемент
	}
	else if (choice == 2){
		current = dataManager.getVisitsBackward(); //записывает имеющиеся записи в порядке убывания
		//ListNode* getVisitsBackward() const { return visits->getBackwardRecursive(); }
	}
	else {
		cout << "Неверный выбор!" << endl;
		return;
	}
	
	int count = 1;
	for (int i = 0; i < recordCount; i++) 
	{
		GymVisit visit = current->visit; //запись первого посещения
		if (!visit.isDeleted)
		{
			displayVisit(visit, count);
			count++;
		}
		current = current->next; //переход к следующему посещению
	}
}

//метод добавления данных в список из файла
void GymConsole::handleLoadFromFile()
{
	string filename;
	cout << "Введите имя файла: ";
	getline(cin, filename);

	//запись данных в файл и запись количества строк (записей) в переменную loadedCount
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

void GymConsole::displayByNameAsc() const
{
	int recordCount = dataManager.getRecordCount();
	int count = 1;
	const ListNode* current = dataManager.getVisits();

	cout << "Данные отсортированные по ФИО по возрастанию:" << endl;
	printHeader();

	for (int i = 0; i < recordCount; i++)
	{
		GymVisit visit = current->visit;
		if (!visit.isDeleted)
		{
			displayVisit(visit, count);
			count++;
		}
		current = current->next;
	}
}

void GymConsole::displayByNameDesc() const
{
	int recordCount = dataManager.getRecordCount();
	int count = 1;
	const ListNode* current = dataManager.getVisits();

	cout << "Данные отсортированные по ФИО по убыванию:" << endl;
	printHeader();
	for (int i = 0; i < recordCount; i++)
	{
		GymVisit visit = current->visit;
		if (!visit.isDeleted)
		{
			displayVisit(visit, count);
			count++;
		}
		current = current->next;
	}
}

void GymConsole::searchByName() const
{
	char targetName[100];
	int choice;
	GymVisit** visits;
		int recordCount;

	cout << "Введите ФИО для поиска: ";
	cin.getline(targetName, 100);

	cout << "Введите способ поиска(1 - итерационный, 2 - рекурсивный): " << endl;
	cin >> choice;
	cin.ignore();

	if (choice == 1) 
	{
		//вызов функции итерационного поиска
		visits = dataManager.findByName(targetName, recordCount, false);
	}
	else if (choice == 2)
	{
		//вызов функции рекурсивного поиска
		visits = dataManager.findByName(targetName, recordCount, true);
	}
	else //иначе возврат в меню
	{
		cout << "Неверный выбор!" << endl;
		return;
	}

	//если нет записей с заданным ФИО, то выводится сообщение, азтем выход из метода
	if (recordCount == 0)
	{
		cout << "Запись с ФИО '" << targetName << "' не найдена" << endl;
	}
	else //иначе выводится заголовок и все записи с заданным ФИО
	{
		cout << endl << "Все записи с ФИО '" << targetName << "':" << endl;
		printHeader();

		int count = 1;

		for (int i = 0; i < recordCount; i++)
		{
			displayVisit(*(visits[i]), count++); //метод описан в пункте 2.
		}
	}
}

//функция пометки записи на удаление
void GymConsole::markForDeletion()
{
	//получение количества записей
	int activeRecordsCount = dataManager.getActiveRecordCount();
	if (activeRecordsCount == 0) //если записей нет, происходит выход из метода
	{
		cout << "Нет данных для удаления" << endl;
		return;
	}

	handleDisplayData(1); //вывод всех записей (итерационный)

	char targetName[100];
	cout << "Введите ФИО из записи для удаления: ";
	cin.getline(targetName, 100);

	int findCount;
	GymVisit** visit = dataManager.findByName(targetName, findCount, false); //поиск по имени (итерационный)
	
	if (findCount == 0) // при отсуствии записей с заданным ФИО происходит выход из метода
	{
		cout << "Запись не найдена!" << endl;
		return;
	}

	//попытка пометить запись на удаление и вывод соотвествующего сообщения
	if (dataManager.tryMarkVisitDeleted(visit, findCount))
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
	int recordCount = dataManager.getRecordCount();
	ListNode* current = dataManager.getVisits();
	for (int i = 0; i < recordCount; i++)
	{
		GymVisit visit = current->visit;
		if (visit.isDeleted)
		{
			displayVisit(visit, deletedCount);
			deletedCount++;
		}
		current = current->next;
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
	current = dataManager.getVisits();
	for (int i = 0; i < recordCount; i++)
	{
		GymVisit visit = current->visit;
		if (visit.isDeleted)
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
		current = current->next;

	}
}

//метод удаления записи
void GymConsole::physicalDeletion()
{
	int deletedCount = 0;
	int recordCount = dataManager.getRecordCount(); //запись количества записей
	ListNode* current = dataManager.getVisits(); // запись первого посещения в порядке возрастания

	//цикл подсчёта количства записей, помеченных на удаление
	for (int i = 0; i < recordCount; i++)
	{
		GymVisit visit = current->visit;
		if (visit.isDeleted)
		{
			deletedCount++;
		}
		current = current->next;
	}

	if (deletedCount == 0) //если таких записей нет, то происходит выход из метода
	{
		cout << "Нет записей для физического удаления" << endl;
		return;
	}

	//подтверждение удаления записей
	cout << "Будет удалено " << deletedCount << " записей. Продолжить? (д/н): ";
	char confirm;
	cin >> confirm;
	cin.ignore();

	//при подстверждении вызывается метод удаления, 
	// при отмене - вывод соотвествующего сообщения
	if (confirm == 'д' || confirm == 'Д')
	{
		dataManager.physicalDeleteMarked();
		cout << "Физически удалено записей: " << deletedCount << endl;
	}
	else
	{
		cout << "Операция отменена" << endl;
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
		cout << "2. Ввод данных из файла" << endl;
		cout << "3. Вывод данных на экран" << endl;
		cout << "4. Поиск данных по имени" << endl;
		cout << "5. Управление удалением записей" << endl;
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
			handleLoadFromFile();
			break;
		case 3:
			cout << "Введите вывод списка (1 - по возрастанию, 2 - по убыванию): " << endl;
			cin >> choice;
			cin.ignore();
			handleDisplayData(choice);
			break;
		case 4:
			searchByName();
			break;
		case 5:
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