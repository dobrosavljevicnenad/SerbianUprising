#ifndef TUTORIALWINDOW_H
#define TUTORIALWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>

class TutorialWindow : public QWidget {
    Q_OBJECT

public:
    explicit TutorialWindow(QWidget *parent = nullptr);

    QString formatDescriptionWithImages(const QString &description);
private slots:
    void handleTutorialSelection(int index);
private:
    void setupUI();
    QListWidget *tutorialList;
    QStackedWidget *tutorialContent;

    QWidget *createTutorialContent(const QString &title, const QVector<QPair<QString, QString> > &textAndImages);
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // TUTORIALWINDOW_H
