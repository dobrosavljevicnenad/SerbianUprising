#ifndef CUSTOMINPUTDIALOG_H
#define CUSTOMINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QIntValidator>
#include <QScreen>
#include <QApplication>

class CustomInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit CustomInputDialog(const QString& title, const QString& labelText, QWidget* parent = nullptr, int maxValue = 0);
    QString getInputText() const;
    int getInputInt() const;

private:
    QLineEdit* inputLineEdit;
    int maxTroops;
};

#endif // CUSTOMINPUTDIALOG_H
