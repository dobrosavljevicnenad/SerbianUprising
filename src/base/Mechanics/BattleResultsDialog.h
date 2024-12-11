#ifndef BATTLERESULTSDIALOG_H
#define BATTLERESULTSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "Results.h"
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>  // Include QGridLayout
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QColor>
#include "BattleReplayDialog.h"
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
    QGridLayout *gridLayout;  // Declare gridLayout as a member of the class
    QTableWidget *resultsTable;
};

#endif // BATTLERESULTSDIALOG_H

