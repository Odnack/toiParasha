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
	void handleDisplayData() const;
	void handleSaveToFile() const;
	void handleLoadFromFile();
	void handleBuildIndices();
	void handleDisplayByIndices() const;
	void handleSearch() const;
	void handleEditRecord();
	void handleDeletionManagement();

	void displayByNameAsc() const;
	void displayByNameDesc() const;
	void displayByVisitIdAsc() const;
	void displayByVisitCountAsc() const; 
	void displayByVisitCountDesc() const;

	void searchByName() const;
	void searchVisitId() const;
	void searchVisitCount() const;

	void markForDeletion();
	void restoreRecord();
	void physicalDeletion();

	void showDisplaySubmenu() const;
	void showSearchSubmenu() const;
	void showDeletionSubmenu() const;

public:
	void run();
};
