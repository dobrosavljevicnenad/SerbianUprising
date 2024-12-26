#ifndef ATTACKTRANSFERDIALOG_H
#define ATTACKTRANSFERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QScreen>
#include <QApplication>
#include <QPainter>

class AttackTransferDialog : public QDialog {
    Q_OBJECT

public:
    explicit AttackTransferDialog(const QString &source, const QString &target,
                                  int sourceArmies, int targetArmies,
                                  const QPixmap &sourceImage, const QPixmap &targetImage,
                                  QWidget *parent = nullptr);
    int getSelectedArmies() const;

private:
    QLabel *sourceLabel;
    QLabel *targetLabel;
    QLabel *infoLabel;
    QLabel *sourceImageLabel;
    QLabel *targetImageLabel;
    QSlider *slider;
    QSpinBox *spinBox;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // ATTACKTRANSFERDIALOG_H
