#include "characterwidget.h"

CharacterWidget::CharacterWidget(unsigned clientId, QWidget *parent) : QWidget(parent),clientId(clientId) {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(10);

    QWidget *labelsContainer = new QWidget(this);
    QVBoxLayout *labelsLayout = new QVBoxLayout(labelsContainer);

    territory = new QLabel(this);
    territoryArmy = new QLabel(this);
    army = new QLabel(this);


    QString labelStyle =
        "color: #D4AF37; "
        "font-weight: bold; "
        "font-size: 12px; "
        "background-color: transparent;"
        "border-image: none;";

    territory->setStyleSheet(labelStyle);
    army->setStyleSheet(labelStyle);
    territoryArmy->setStyleSheet(labelStyle);

    labelsLayout->addWidget(army);
    labelsLayout->addWidget(territory);
    labelsLayout->addWidget(territoryArmy);

    labelsLayout->addStretch();

    if (clientId == 1) {
        labelsContainer->setStyleSheet(
            "background-color: rgba(74, 47, 47,180); "
            "border-radius: 10px; "
            "padding: 10px;"
            );
    } else {
        labelsContainer->setStyleSheet(
            "background-color: rgba(3, 66, 5,180); "
            "border-radius: 10px; "
            "padding: 10px;"
            );
    }

    QLabel *portrait = new QLabel(this);
    QPixmap pixmap;
    if (clientId == 1) {
        pixmap = QPixmap(":/resources/karadjordje.png");
    } else {
        pixmap = QPixmap(":/resources/pasha.png");
    }
    portrait->setPixmap(pixmap.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    portrait->setFrameStyle(QFrame::Box);
    if (clientId == 1) {
        portrait->setStyleSheet(
            "border: 2px solid gold; "
            "border-radius: 10px; "
            "background-color: rgba(74, 47, 47,180);"
            );
    } else {
        portrait->setStyleSheet(
            "border: 2px solid gold; "
            "border-radius: 10px; "
            "background-color: rgba(3, 66, 5,180);"
            );
    }

    mainLayout->addWidget(labelsContainer);
    mainLayout->addWidget(portrait);

    setStyleSheet(
        "background-color: rgba(0, 43, 54, 180); "
        "border-radius: 10px; "
        "border-image: url(:/resources/border1.png) 30 stretch;"
        );
}

void CharacterWidget::updateTerritoryInfo(const QString &territoryName, int troopsCount) {
    territory->setText(QString("Territory: %1").arg(territoryName));
    territoryArmy->setText(QString("Troops: %1").arg(troopsCount));
}
void CharacterWidget::setArmyText(int troopsCount1, int troopsCount2) {
    army->setText(QString("%1/%2 armies left").arg(troopsCount1)
                                                .arg(troopsCount2));
}
