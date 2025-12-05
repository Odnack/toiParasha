#include "GymVisit.h"
#include <sstream>
#include <iomanip>

using namespace std;

GymVisit::GymVisit() : visitId(0), year(0), month(0), day(0),
hour(0), minute(0), isDeleted(false) {
    fullName[0] = '\0';
    gymAddress[0] = '\0';
}

string GymVisit::getFormattedDateTime() const {
    ostringstream oss;
    oss << getFormattedDate() << " " << getFormattedTime();
    return oss.str();
}

string GymVisit::getFormattedDate() const {
    ostringstream oss;
    oss << setw(2) << setfill('0') << day << "."
        << setw(2) << setfill('0') << month << "."
        << year;
    return oss.str();
}

string GymVisit::getFormattedTime() const {
    ostringstream oss;
    oss << setw(2) << setfill('0') << hour << ":"
        << setw(2) << setfill('0') << minute;
    return oss.str();
}

NameIndex::NameIndex() : originalIndex(0) {
    fullName[0] = '\0';
}

NameIndex::NameIndex(int originalIndex, string name) : originalIndex(originalIndex) {
    fullName = name;
}

VisitIdIndex::VisitIdIndex()
    : originalIndex(0), visitId(0) {
}
VisitIdIndex::VisitIdIndex(int originalIndex, long visitId)
    : originalIndex(originalIndex), visitId(visitId){}

VisitCountIndex::VisitCountIndex()
    : originalIndex(0), visitCount(0) {
}
VisitCountIndex::VisitCountIndex(int originalIndex, unsigned short visitCount)
    : originalIndex(originalIndex), visitCount(visitCount) {
}