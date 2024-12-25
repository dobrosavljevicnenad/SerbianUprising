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
#include <QFileDialog>
#include <QDir>
#include <QFileInfoList>
#include <QRegularExpression>
#include <QMessageBox>
#include <map>
#include <qlabel.h>
#include <qlistwidget.h>
#include <qmediaplayer.h>
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
#include "../Interface/Items/characterwidget.h"
#include "map.h"
#include "filemanager.h"


class ClientGameManager : public QObject{

    Q_OBJECT

public:
    explicit ClientGameManager(QGraphicsScene* scene,  QObject* parent = nullptr);
    enum class MapMode { Default, Relief, Regions,Culture, CityLevel };
    void applyMapMode(MapMode mode);
    void initializeUI(QLabel* headerLabel, QPushButton* endTurnButton, QPushButton* moveButton, QPushButton* infoButton,
                      QListWidget* moveList,QPushButton* armyButton,QPushButton* reliefButton,QPushButton* regionsButton,
                      QPushButton*cityButton,QPushButton*cultureButton,QPushButton*defaultButton,
                      NodeInfoWidget* nodeInfoWidget, CharacterWidget *characterWidget);
    void initializeGraphics(QJsonObject graphData);
    void printConnections();
    void processDataFromServer(const QJsonObject& data);
    void updateGraphics();
    void EndTurnClicked(const QVector<Action>& actions, int id);
    void allReset();
    void drawArrow(int playerId, MapLayer* from, MapLayer* to, int number,int actionId, QColor color = Qt::white, bool isTemporary = false);
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
    void updateFog();
    QVector<QStringList> generateBattleResults();
    std::vector<Results> resultsVector;
    Year year();
    graph::Graph*getClientGraph() const;
    Player getPlayer() const;
    void removePlaceAction(int actionId);
    void clearTemporaryArrows();
    void saveGame();
    void loadGame();
    void processLoadData(const QJsonObject &gameData);
    QString loadGamePath = nullptr;
    int getClientId() const;

    QMap<MapLayer*,graph::Vertex*> layerToVertex;

    bool server_closed = false;

private slots:
    void printExplosion(graph::Vertex *target);

signals:
    void layerClicked(MapLayer* layer);
    void endTurnActionsReady(const QVector<Action>& actions, int id);
    void gameYearUpdated(QString gameYear);
    void gameDataLoaded(const QJsonObject& graphData);
    void updateCharacterWidget(const QString &territoryName, int troops);



private:
    bool init = false;
    std::vector<MapLayer*> explosions;
    QGraphicsScene* scene;
    std::unique_ptr<graph::Graph> clientGraph;
    std::vector<MapLayer*> layers;
    AddArmyManager armyManager;
    std::map<int,std::vector<CustomArrowItem*>> arrows;
    std::vector<CustomArrowItem*> temporaryArrows;
    Year gameYear;
    QVector<Region*>regions;
    Map* map;
    FileManager fileManager;
    QMediaPlayer *musicPlayer;
    QAudioOutput *audioOutput;


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
    CharacterWidget *characterWidget;

public:
    unsigned TurnId = 1;
    int ClientId;
    BattleResultsDialog *dialog;
    QVector<Action> actionBuffer;

    std::vector<std::tuple<graph::Vertex*, graph::Edge*, QColor>> getValidatedEdges(graph::Vertex* vertex);

    void onLayerHovered(MapLayer *layer);
    Player player;
};
#endif // CLIENTGAMEMANAGER_H
