#include "event.h"


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

QString Event::getPicturePath() const {return picture_path;}

void Event::showEventWindow() {


    infoWindow = new QWidget;
    infoWindow->setWindowTitle("Event Details");
    infoWindow->setMinimumSize(600,400);

    infoWindow->setStyleSheet(
        "background-color: #f0e6d2;"
        "border: 4px solid #4e3629;"
        "border-radius: 15px;"
        );

    QVBoxLayout * layout = new QVBoxLayout(infoWindow);


    QString eventType;
    switch(type){
        case EventType::BATTLE:
            eventType = "Battle Event";
            break;
        case EventType::PROGRESS:
            eventType = "Progress Event";
            break;
        case EventType:: UPRISING:
            eventType = "Uprising Event";
            break;
        case EventType::ECONOMIC_EVENT:
            eventType = "Economic event";
            break;
        case EventType::NATURAL_DISASTER:
            eventType = "Natural Disaster";
            break;
    }

        typeLabel = new QLabel("<h1 style='color: #d7a13d; font-family: \"SimSun\", serif; font-size: 26px;'>"
    "Karadjordje zauzima Cetinje</h1>", infoWindow);
    typeLabel->setAlignment(Qt::AlignCenter);
    typeLabel->setStyleSheet("border: none");
    layout->addWidget(typeLabel);

    QLabel *imageLabel = new QLabel(infoWindow);
    QPixmap label_image(picture_path);
    label_image = label_image.scaled(infoWindow->width(),100,Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    imageLabel->setPixmap(label_image);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("margin-bottom: 15px");
    layout->addWidget(imageLabel);

    outcomeLabel = new QLabel(QString("<p style='color: #6b3f3d; font-size: 16px;'>%1</p>")
                                  .arg(QString::fromStdString(outcome)), infoWindow);

    layout->addWidget(outcomeLabel);

    QPushButton *closeButton = new QPushButton("X",infoWindow);
    layout->addWidget(closeButton);
    closeButton->setFixedSize(70,50);
    closeButton->setStyleSheet(        "background-color: #ff6347;"
                               "border: 1px solid #8b4513;"
                               "border-radius: 5px;"
                               "text-align: center;"
                               "font-size: 14px; padding: 8px;");


    gainLabel = new QLabel(QString("<p style='color: #006400;'>Outcome Gain: %1</p>").arg(outcomeGain), infoWindow);
    gainLabel->setStyleSheet("boder : none;");
    layout->addWidget(gainLabel);

    territoryCountLabel = new QLabel(QString("<p style='color: #4b0082;'>Territories Affected: %1</p>").arg(territories.size()), infoWindow);
    layout->addWidget(territoryCountLabel);

    QObject::connect(closeButton, &QPushButton::clicked,infoWindow,&QWidget::close);

    infoWindow->show();

}

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
