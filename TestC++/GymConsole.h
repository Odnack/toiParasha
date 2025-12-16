#include "GymDataManager.h"
#include <iostream>

using namespace std;

class GymConsole {
private:
	GymDataManager dataManager;

	GymVisit inputVisitData() const;
	void displayVisit(const GymVisit& visit, int number) const;
	void displayVisitWithCount(const GymVisit& visit, int number, unsigned short count) const;

	void handleInputData();
	void handleDisplayData(int choise) const;
	void handleLoadFromFile();
	void handleDeletionManagement();

	void displayByNameAsc() const;
	void displayByNameDesc() const;

	void searchByName() const;

	void markForDeletion();
	void restoreRecord();
	void physicalDeletion();

	void showDeletionSubmenu() const;

public:
	void run();
};
