#include "event.h"
#include <sstream>
#include <cmath>
#include <iostream>
// Constructor implementation
Event::Event(EventType type, const std::string& outcome,
             int outcomeGain, const std::vector<MapLayer*>& territories,
             Army *army, const std::string& date)
    : type(type),outcome(outcome), outcomeGain(outcomeGain),
    territories(territories), army(army), date(date) {}

EventType Event::getType() const{return type;}


std::string Event::getDescription() const {return description;}

std::string Event::getOutcome() const {return outcome;}

int Event::getOutcomeGain() const {return outcomeGain;}

const std::vector<MapLayer*>& Event::getTerritories() const {return territories;}

Army* Event::getArmy() const {return army;}

std::string Event::getDate() const {return date;}


double Event::calculateEventPercentage(const std::string &currentDate ) const{
    int eventYear,eventMonth;

    std::stringstream ssYear(date.substr(date.rfind('-') + 1 ));
    std::stringstream ssMonth(date.substr(date.find('-') + 1, 2));
    ssYear >> eventYear;
    ssMonth >> eventMonth;

    int currentYear, currentMonth;

    std::stringstream ssCurrentYear(currentDate.substr(currentDate.rfind('-') + 1));
    std::stringstream ssCurrentMonth(currentDate.substr(currentDate.find('-') + 1, 2));


    int eventQuarter = (eventMonth - 1)/3+1;
    int currentQuarter = (currentMonth -1 )/3+1;

    int quartersSinceEvent = (currentYear - eventYear)*4+(currentQuarter - eventQuarter);

    double eventPercentage = std::exp(-quartersSinceEvent/100.0) * 100.0;
    return eventPercentage;

}

// Print event details
void Event::printEventDetails() const {
    std::cout << "Event Description: " << description << "\n"
              << "Type: " << static_cast<int>(type) << "\n"
              << "Outcome: " << outcome << "\n"
              << "Outcome Gain: " << outcomeGain << "\n"
              << "Number of Territories: " << territories.size() << "\n"
              << "Date: " << date << "\n";
}
EventType stringToEventType(const std::string& str) {
    if (str == "BATTLE") return EventType::BATTLE;
    if (str == "PROGRESS") return EventType::PROGRESS;
    if (str == "UPRISING") return EventType::UPRISING;
    if (str == "ECONOMIC_EVENT") return EventType::ECONOMIC_EVENT;
    if (str == "NATURAL_DISASTER") return EventType::NATURAL_DISASTER;
    throw std::invalid_argument("Invalid EventType string: " + str);
}
