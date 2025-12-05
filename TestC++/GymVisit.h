#pragma once
#include <string>
#include <cstring>

using namespace std;

struct GymVisit {
    unsigned int visitId;
    string fullName;
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    string gymAddress;
    bool isDeleted;

    GymVisit();
    string getFormattedDateTime() const;
    string getFormattedDate() const;
    string getFormattedTime() const;
};

struct NameIndex {
    int originalIndex;
    string fullName;

    NameIndex();
    NameIndex(int originalIndex, string name);
};

struct VisitIdIndex {
    int originalIndex;
    long visitId;

    VisitIdIndex();
    VisitIdIndex(int originalIndex, long visitId);
};

struct VisitCountIndex {
    int originalIndex;
    unsigned short visitCount;

    VisitCountIndex();
    VisitCountIndex(int originalIndex, unsigned short visitCount);
};