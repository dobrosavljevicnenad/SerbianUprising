#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QFrame>

class CharacterWidget : public QWidget {
    Q_OBJECT

public:
    explicit CharacterWidget(unsigned clientId, QWidget *parent = nullptr);
    void setupPortrait(QLabel *portraitLabel, const QString &imagePath);
    void setArmyText(int troopsCount1, int troopsCount2);
public slots:
    void updateTerritoryInfo(const QString &territoryName, int troopsCount);
private:
    unsigned clientId;
    QLabel *territory;
    QLabel *territoryArmy;
    QLabel *army;
};

#endif // CHARACTERWIDGET_H
