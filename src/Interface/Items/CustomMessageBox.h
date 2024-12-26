#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class CustomMessageBox : public QDialog {
    Q_OBJECT

public:
    explicit CustomMessageBox(const QString &message, QWidget *parent = nullptr);

    static void showMessage(const QString &message, QWidget *parent = nullptr);

private:
    QLabel *messageLabel;
    QPushButton *okButton;
};

#endif // CUSTOMMESSAGEBOX_H
