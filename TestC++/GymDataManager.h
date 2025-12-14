#pragma once
#include "OrderedLinkedList.h"
#include "AVLTreeIndex.h"

using namespace std;

class GymDataManager {
private:
	OrderedLinkedList* visits;

public:
	GymDataManager();
	~GymDataManager();

	void addVisit(const GymVisit& visit);
	bool tryEditVisit(int index, const GymVisit& newData);
	bool tryMarkVisitDeleted(int index);
	bool tryRestoreVisit(int index);
	void physicalDeleteMarked();

	GymVisit* findByName(const string& targetName, int& outSize) const;

	ListNode* getVisits() const { return visits->getForwardLinear(); }

	int getRecordCount() const { return visits->getSize(); }
	int getActiveRecordCount() const;

	bool trySaveToFile(const string& filename, bool append = false) const;
	int loadFromFile(const string& filename);
};
