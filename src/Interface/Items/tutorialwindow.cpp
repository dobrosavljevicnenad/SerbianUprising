#include "tutorialwindow.h"
#include <qboxlayout.h>
#include <qevent.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qscrollarea.h>
#include <qpushbutton.h>

TutorialWindow::TutorialWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void TutorialWindow::setupUI() {
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color: #2D2D2D; color:white; font-size: 16px;");

    tutorialList = new QListWidget(this);
    tutorialContent = new QStackedWidget(this);

    tutorialList->addItem("Army Movement");
    tutorialList->addItem("Recruitment");
    tutorialList->addItem("Events");
    tutorialList->addItem("Battle Results");
    tutorialList->setFixedWidth(150);
    tutorialContent->addWidget(createTutorialContent(
        "Army Movement",{
            {"To move your army, you need to select <b>Move</b> action from the left panel and click on a territory you control.<br>"
             "Each territory displays the number of soldiers stationed there, so you can plan your movements accordingly.<br><br>"
             "Strategic Tip: Concentrate your troops on key territories to prepare for future battles or defend critical regions."
             ,":/resources/Images/army_movement1.png"},
            {"Once selected, arrows will appear, indicating where you can move your troops. The arrows are color-coded for clarity:<br>"
             "- <b>Red</b>: Attack an enemy-controlled territory.<br>"
             "- <b>Green</b>: Transfer troops to your own controlled territory.<br>"
             "- <b>Blue</b>: Move troops across naval routes.<br><br>"
             "Time Management: Troop movement might take multiple turns, so plan ahead to avoid leaving key territories undefended."
             ,":/resources/Images/army_movement2.png"},
            {"Click on a destination territory indicated by an arrow. "
             "A dialog box will open, allowing you to choose the number of soldiers to send.<br>"
             "You can cancel the action by clicking on it in the left panel.<br><br>"
             ,":/resources/Images/army_movement3.png"},
            {"Once confirmed, the movement action will appear on the right side of the screen, ready to be executed.<br>"
             "You can cancel the action by selecting it from the left panel.<br>"
              "Plan troop movements carefully to outmaneuver your opponents!<br><br>"
              "Tips for Success:<br>"
              "- Use multiple small troop movements to mislead your opponent and keep them guessing.<br>"
              "- Prioritize attacking weaker territories to expand your control quickly.<br>"
              "- Keep some troops in reserve to defend key territories and respond to unexpected attacks.<br>"
              "- Naval routes (blue arrows) can provide strategic opportunities for surprise attacks or reinforcements.<br><br>"
             ,":/resources/Images/army_movement4.png"}
        }));

    tutorialContent->addWidget(createTutorialContent(
        "Recruitment",{
            {"Recruitment allows you to strengthen your forces by deploying new troops to controlled territories.<br>"
             "In the top-right corner of the screen, you can see the total number of available troops for deployment.<br><br>"
             "Strategic Tip: Focus on controlling culturally aligned territories to maximize troop production."
             ,":/resources/Images/recruitment1.png"},
            {"This number depends on the territories you control, their regions, and their cultural background:<br>"
             "- <b>Serbian</b> territories generate more troops if they belong to Serbian culture.<br>"
             "- <b>Ottoman</b> territories produce more troops if they are aligned with Ottoman culture.<br><br>"
             "Controlling an entire region provides additional bonuses for recruitment."
             ,":/resources/Images/recruitment2.png"},
            {"To deploy troops, select <b>Army</b> from the left panel and click on a controlled territory. "
             "A dialog will appear, allowing you to choose the number of troops to place on that territory.<br><br>"
             "Note: Be mindful of the maximum capacity of each territory!"
             ,":/resources/Images/recruitment3.png"},
            {"Territory capacities are based on their city level:<br>"
             "- <b>City Level 1</b>: Maximum of 10 soldiers.<br>"
             "- <b>City Level 2</b>: Maximum of 50 soldiers.<br>"
             "- <b>City Level 3</b>: Unlimited soldiers.<br><br>"
             "Shortcut Commands:<br>"
             "- <b>Ctrl + Click</b>: Place 1 soldier.<br>"
             "- <b>Shift + Click</b>: Place 10 soldiers.<br>"
             "- <b>Ctrl + Shift + Click</b>: Place 100 soldiers."
             ,":/resources/Images/recruitment4.png"},
            {"A dialog box will also display current troop availability and region bonuses.<br>"
             "Pay attention to recruitment limitations and optimize for long-term strategy.<br><br>"
             ,":/resources/Images/recruitment5.png"},
            {"Note: If recruitment exceeds the territory's capacity, extra soldiers will not be added.<br><br>"
             "Once confirmed, the recruitment action will appear on the right side of the screen, ready to be executed.<br>"
             "To cancel, click on the action in the left panel.<br><br>"
             "Plan your recruitment carefully to ensure your forces are ready for future challenges!"
             ,":/resources/Images/recruitment6.png"}
        }));


    tutorialContent->addWidget(createTutorialContent(
        "Events",
        {
            {"<b>Events</b> play a crucial role in shaping your strategy and gameplay. They can occur at any time during the game and can have a variety of effects on your empire.<br><br>"
             "There are three main types of events:<br>"
             "- <b>Historical Events:</b> These are based on real-world history and add immersion by introducing challenges or opportunities inspired by historical events.<br>"
             "- <b>Random Events:</b> These occur unpredictably, such as gaining additional troops or receiving a morale boost.<br>"
             "- <b>Territory-Specific Events:</b> These are triggered when specific territories are conquered, potentially rewarding you with reinforcements, morale boosts, or even starting a rebellion in another territory.<br><br>"
            ,":/resources/Images/events1.png"},
            {"For example:<br>"
             "- Successfully conquering a key territory might grant bonus soldiers.<br>"
             "- Losing control of a culturally important region might trigger a rebellion in neighboring territories.<br>"
             "Managing events effectively can help you maintain stability and gain an advantage over your opponent, so plan your strategy carefully!"
             ,":/resources/Images/events2.png"}
        }
        ));

    tutorialContent->addWidget(createTutorialContent(
        "Battle Results",
        {
            {"At the end of each turn, the <b>Battle Results</b> window will open, summarizing all battles that occurred during that turn. Each battle is represented in a compact box, showing key details at a glance.<br>"
             "To dive deeper into any specific battle, simply click on its box to open the <b>Battle Replay</b> window.<br>"
             ,":/resources/Images/image4.png"},
            {"In the <b>Battle Replay</b> window, you can use the <b>Replay</b> button to simulate the battle and observe how it unfolded."
             "Additionally, this window highlights key <b>advantages</b> that influenced the outcome of the battle, such as:<br>"
             "- <b>Terrain</b> bonuses (e.g., forests, hills).<br>"
             "- <b>Morale</b> boosts or penalties.<br>"
            ,":/resources/Images/image3.png"},
        }
        ));

    connect(tutorialList, &QListWidget::currentRowChanged, this, &TutorialWindow::handleTutorialSelection);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(tutorialList);
    mainLayout->addWidget(tutorialContent);

    tutorialList->setCurrentRow(0);

    QPushButton *closeButton = new QPushButton("Close", this);
    closeButton->setStyleSheet("background-color: #a62517; color: white; font-size: 18px;");
    closeButton->setGeometry(width(), 10, 80, 30);
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
}

void TutorialWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    }
    QWidget::keyPressEvent(event);
}

QWidget *TutorialWindow::createTutorialContent(const QString &title, const QVector<QPair<QString, QString>>& textAndImages) {
    QWidget *content = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(content);

    QLabel *titleLabel = new QLabel("<h1 style='font-size: 28px; color: white;'>" + title + "</h1>", this);
    layout->addWidget(titleLabel);

    QVBoxLayout *textLayout = new QVBoxLayout();

    for (const auto &pair : textAndImages) {
        QHBoxLayout *textImageLayout = new QHBoxLayout();

        QLabel *descriptionLabel = new QLabel(this);
        descriptionLabel->setText(pair.first);
        descriptionLabel->setStyleSheet("font-size: 18px; color: white; border: none; padding: 5px;");
        descriptionLabel->setWordWrap(true);
        descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

        textImageLayout->addWidget(descriptionLabel);

        QLabel *imageLabel = new QLabel(this);
        QPixmap pixmap(pair.second);
        pixmap = pixmap.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(pixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        textImageLayout->addWidget(imageLabel);

        textLayout->addLayout(textImageLayout);
    }

    QWidget *textContent = new QWidget();
    textContent->setLayout(textLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(textContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    layout->addWidget(scrollArea);
    content->setLayout(layout);
    return content;
}


void TutorialWindow::handleTutorialSelection(int index) {
    tutorialContent->setCurrentIndex(index);
}
