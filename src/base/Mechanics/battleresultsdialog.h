#ifndef BATTLERESULTSDIALOG_H
#define BATTLERESULTSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "results.h"
#include <QHeaderView>
#include <QPushButton>
#include <QGridLayout>
#include <QTableWidgetItem>
#include <QColor>
#include "battlereplaydialog.h"
#include "../graph/graph.hpp"
#include <iostream>
#include <QScreen>

class BattleResultsDialog : public QDialog {
    Q_OBJECT

public:
    explicit BattleResultsDialog(const std::vector<Results> battleResults, QWidget *parent = nullptr);
    void setResults(const QVector<QStringList> &results);
    void setBackgroundImage(const QString &imagePath);

private slots:
    void onTableCellClicked(int tableIndex, Results results);

private:
    const std::vector<Results> battleResults;
    QGridLayout *gridLayout;
    QGridLayout *containerLayout;
    QWidget *tablesContainer;
    QString getButtonStyle();
    QPushButton *closeButton;
    graph::Graph *m_graph;
};

#endif // BATTLERESULTSDIALOG_H
