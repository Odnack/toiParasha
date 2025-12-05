#pragma once
#include "GymVisit.h"
#include "AVLTreeIndex.h"

using namespace std;

class GymDataManager {
private:
	GymVisit* visits;
	AVLTreeIndex* nameIndex;
	VisitIdIndex* visitIdIndex;
	VisitCountIndex* visitCountIndex;
	int visitCountIndexSize;

	int recordCount;
	int capacity;

	void resizeIfNeeded();

	int binarySearchVisitCountRecursive(int targetVisitCount, int left, int right) const;

	void sortVisitIdIndex();
	void sortVisitCountIndex();

public:
	GymDataManager();
	~GymDataManager();

	void addVisit(const GymVisit& visit);
	bool tryEditVisit(int index, const GymVisit& newData);
	bool tryMarkVisitDeleted(int index);
	bool tryRestoreVisit(int index);
	void physicalDeleteMarked();

	void buildNameIndex();
	void buildVisitIdIndex();
	void buildVisitCountIndex();

	int findByNameRecursive(const string& targetName) const;
	int findByNameIterative(const string& targetName) const;
	int binarySearchByVisitId(int targetVisitId) const;
	int binarySearchByVisitCount(int targetVisitCount) const;

	const GymVisit* getVisits() const { return visits; }
	const AVLTreeIndex* getNameIndex() const { return nameIndex; }
	const VisitIdIndex* getVisitIdIndex() const { return visitIdIndex; }
	const VisitCountIndex* getVisitCountIndex() const { return visitCountIndex; }
	const int getVisitCountIndexSize() const { return visitCountIndexSize; }

	int getRecordCount() const { return recordCount; }
	int getActiveRecordCount() const;

	bool trySaveToFile(const string& filename, bool append = false) const;
	int loadFromFile(const string& filename);
};
