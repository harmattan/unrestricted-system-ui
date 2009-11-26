#ifndef MAIN_H
#define MAIN_H

#include <QObject>
#include <QPointer>
#include <QTimer>
#include <DuiApplicationPage>
#include <QDBusAbstractAdaptor>

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
class TestObj : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.page_win_appear.dbus.if")

public:
    TestObj(bool enableBack = false, int time = 1);
    virtual ~TestObj();

public slots:
    void handlePage();
    void Show();
    void Hide();
    void Start();
    inline void Stop(){time = 0;};

private:
    QPointer<TestPage> p;
    int time;
};

#endif // MAIN_H
