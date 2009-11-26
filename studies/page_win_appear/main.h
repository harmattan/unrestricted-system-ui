#ifndef MAIN_H
#define MAIN_H

#include <QObject>
#include <QPointer>
#include <QTimer>
#include <DuiApplicationPage>

class DuiButton;
/* TestPage */
class  TestPage : public DuiApplicationPage
{
    Q_OBJECT

public:
    TestPage();
    virtual ~ TestPage();
};

/* TestObj */
class TestObj : public QObject
{
    Q_OBJECT
public:
    TestObj(bool enableBack = false);
    virtual ~TestObj();

public slots:
    void handlePage();

private:
    QPointer<TestPage> p;
    int time;
};

#endif // MAIN_H
