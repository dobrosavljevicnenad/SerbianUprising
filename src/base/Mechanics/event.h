#ifndef EVENT_H
#define EVENT_H

#include <string>
#include "../../Interface/maplayer.h"
#include "../Entities/army.h"
#include <vector>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <qpushbutton.h>
#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QString>
#include <sstream>
#include <cmath>
#include <iostream>
#include <QApplication>

enum class EventType{
    BATTLE,
    PROGRESS,
    UPRISING,
    ECONOMIC_EVENT,
    NATURAL_DISASTER
};

class Event {

public:
    Event();

    Event(EventType type, const std::string& outcome,
          int outcomeGain, const std::vector<MapLayer*>& territories,
          Army* army, const std::string& date);

    EventType getType() const;



    std::string getDescription() const;
    QString getPicturePath() const;
    std::string getOutcome() const;
    int getOutcomeGain() const;
    const std::vector<MapLayer*>& getTerritories() const;
    Army* getArmy() const;
    std::string getDate() const;
    void showEventWindow();

    void printEventDetails() const;
    EventType stringToEventType(const std::string str);



private:
    EventType type;
    std::string description;
    QString picture_path;
    std::string outcome;
    int outcomeGain;
    std::vector<MapLayer*> territories;
    Army *army;
    QWidget *infoWindow;
    QLabel *typeLabel;
    QLabel *outcomeLabel;
    QLabel *gainLabel;
    QLabel *territoryCountLabel;
    QLabel *dateLabel;
    std::string date;
};


#endif // EVENT_H
