#ifndef BATTLERESULTSDIALOG_H
#define BATTLERESULTSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "Results.h"
#include <QHeaderView>
#include <QPushButton>
#include <QGridLayout>
#include <QTableWidgetItem>
#include <QColor>
#include "BattleReplayDialog.h"
#include "../graph/graph.hpp"

class BattleResultsDialog : public QDialog {
    Q_OBJECT

public:
    explicit BattleResultsDialog(const std::vector<Results> battleResults, QWidget *parent = nullptr);
    void setResults(const QVector<QStringList> &results);
    void setBackgroundImage(const QString &imagePath);

private slots:
    void onTableCellClicked(int tableIndex, Results results);

private:
    const std::vector<Results> battleResults;  // Store battle results
    QGridLayout *gridLayout;                   // Main grid layout for dialog
    QGridLayout *containerLayout;              // Layout for tables inside the container
    QWidget *tablesContainer;                  // Container widget for tables
    QString getButtonStyle();
    QPushButton *closeButton;
    graph::Graph *m_graph;
};

#endif // BATTLERESULTSDIALOG_H
