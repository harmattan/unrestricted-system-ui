#ifndef MAIN_H
#define MAIN_H

#include <QObject>
#include <DuiApplicationWindow>
#include <NetworkRegistration>

using namespace Cellular;

class TestObj : public QObject
{
    Q_OBJECT
public:
    TestObj(DuiApplicationWindow &win);
    virtual ~TestObj();

private slots:
    void availableNetworksReceived(bool, const QList<AvailableOperator*> &, const QString &);
    void selectionCompleted(bool success, const QString &reason);

private:
    bool visible;
    DuiApplicationWindow *win;
    NetworkRegistration *networkRegistration;    


public slots:
    void doStuff();


};

#endif // MAIN_H
