#ifndef EVENT_H
#define EVENT_H

#include <string>
#include "../../Interface/maplayer.h"
#include "../Entities/Army.h"
#include <vector>



enum class EventType{
    BATTLE,
    PROGRESS,
    UPRISING,
    ECONOMIC_EVENT,
    NATURAL_DISASTER
};

class Event{

    private:
        EventType type;
        std::string description;
        std::string outcome;
        int outcomeGain;
        std::vector<MapLayer*> territories;
        Army *army;
        std::string date;

    public:
        Event(EventType type, const std::string& outcome,
                int outcomeGain, const std::vector<MapLayer*>& territories,
              Army* army, const std::string& date);

        EventType getType() const;

        std::string getDescription() const;
        std::string getOutcome() const;
        int getOutcomeGain() const;
        const std::vector<MapLayer*>& getTerritories() const;
        Army* getArmy() const;
        std::string getDate() const;


        double calculateEventPercentage(const std::string& currentDate) const;
        void printEventDetails() const;
        EventType stringToEventType(const std::string& str);

};


#endif // EVENT_H
