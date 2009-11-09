#ifndef MAIN_H
#define MAIN_H

#include <QObject>
#include <NetworkRegistration>

using namespace Cellular;

class TestObj : public QObject
{
    Q_OBJECT
public:
    TestObj();
    virtual ~TestObj();

private slots:
    void availableNetworksReceived(bool, const QList<AvailableOperator*> &, const QString &);

private:
    NetworkRegistration *networkRegistration;    

};

#endif // MAIN_H
