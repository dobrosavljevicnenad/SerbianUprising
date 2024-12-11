#ifndef BATTLERESULTSDIALOG_H
#define BATTLERESULTSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>

class BattleResultsDialog : public QDialog {
    Q_OBJECT

public:
    explicit BattleResultsDialog(QWidget *parent = nullptr);
    void setResults(const QVector<QStringList> &results);

private:
    QLabel *titleLabel;
    QTableWidget *resultsTable;
};

#endif // BATTLERESULTSDIALOG_H
