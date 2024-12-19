#ifndef CLIENTGAMEMANAGER_H
#define CLIENTGAMEMANAGER_H

#include "../base/graph/graph.hpp"
#include<QGraphicsScene>
#include<iostream>
#include<QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <map>
#include <qlabel.h>
#include <qlistwidget.h>
#include <qpushbutton.h>
#include "../base/Mechanics/addarmymanager.h"
#include "Items/customarrowitem.h"
#include "infowidget.h"
#include "mapscene.h"
#include "../base/Mechanics/Action.h"
#include "../base/Entities/player.h"
#include "../base/Mechanics/Results.h"
#include "../base/Mechanics/BattleResultsDialog.h"
#include "../base/Entities/year.h"
#include "../base/Entities/region.h"
#include "../base/Entities/city.h"
#include "map.h"


class ClientGameManager : public QObject{

    Q_OBJECT

public:
    explicit ClientGameManager(QGraphicsScene* scene,  QObject* parent = nullptr);
    enum class MapMode { Default, Relief, Regions,Culture, CityLevel };
    void applyMapMode(MapMode mode);
    void initializeUI(QLabel* headerLabel, QPushButton* endTurnButton, QPushButton* moveButton, QPushButton* infoButton,
                      QListWidget* moveList,QPushButton* armyButton,QPushButton* reliefButton,QPushButton* regionsButton,
                      QPushButton*cityButton,QPushButton*cultureButton,QPushButton*defaultButton,NodeInfoWidget* nodeInfoWidget );
    void initializeGraphics(QJsonObject graphData);
    void printConnections();
    void processDataFromServer(const QJsonObject& data);
    void updateGraphics();
    void EndTurnClicked(const QVector<Action>& actions, int id);
    void allReset();
    void drawArrow(int playerId, MapLayer* from, MapLayer* to, int number,int actionId);
    void addAction(const Action& action);
    QString GetCurrentAction(const Action& action);
    AddArmyManager& getArmyManager();
    void removeActionById(int actionId);
    void removeArrowByActionId(int actionId);
    void clearArrows();
    void setScene(MapScene *newScene);
    void setId(int id);
    void disableInteractions();
    void enableInteractions();
    void clearExplosions();
    QVector<QStringList> generateBattleResults();
    std::vector<Results> resultsVector;
    Year year();

    QMap<MapLayer*,graph::Vertex*> layerToVertex;

private slots:
    void printExplosion(graph::Vertex *target);

signals:
    void layerClicked(MapLayer* layer);
    void endTurnActionsReady(const QVector<Action>& actions, int id);
    void gameYearUpdated(QString gameYear);


private:
    bool init = false;
    std::vector<MapLayer*> explosions;
    QGraphicsScene* scene;
    std::unique_ptr<graph::Graph> clientGraph;
    std::vector<MapLayer*> layers;
    AddArmyManager armyManager;
    std::map<int,std::vector<CustomArrowItem*>> arrows;
    Player player;
    Year gameYear;
    QVector<Region*>regions;
    Map* map;


private://UI
    QLabel* headerLabel;
    QPushButton* endTurnButton;
    QListWidget *moveList;
    QPushButton *moveButton;
    QPushButton *armyButton;
    QPushButton* infoButton;
    NodeInfoWidget* nodeInfoWidget;
    QPushButton* reliefButton;
    QPushButton* regionsButton;
    QPushButton* cityButton;
    QPushButton* defaultButton;
    QPushButton* cultureButton;

public:
    unsigned TurnId = 1;
    int ClientId;
    BattleResultsDialog *dialog;
    QVector<Action> actionBuffer;
    graph::Graph*getClientGraph() const;
    Player getPlayer() const;
    void removePlaceAction(int actionId);
};
#endif // CLIENTGAMEMANAGER_H
