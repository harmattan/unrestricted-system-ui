#ifndef NETWORKTECHNOLOGY_H
#define NETWORKTECHNOLOGY_H

#include <QTimer>

#ifdef UNIT_TEST
#include "radioaccessstub.h"
#include "networkcellstub.h"
#else
#include <RadioAccess>
#include <NetworkCell>
#endif

using namespace Cellular;

class NetworkTechnology : public QObject
{
    Q_OBJECT

public:

    enum Technology {
        None = -1,
        TwoG,
        TwoPointFiveG,
        ThreeG,
        ThreePointFiveG
    };

    NetworkTechnology(RadioAccess *radioAccess, QObject *parent = 0);
    ~NetworkTechnology();

    NetworkTechnology::Technology currentTechnology();

private slots:
    void checkTechnology();

signals:
    void technologyChanged(NetworkTechnology::Technology technology);

private: //methods
    void init();
    void updateTechnology();

private: //attributes
    RadioAccess *radioAccess;
    NetworkCell *networkCell;
    NetworkTechnology::Technology technology;
    QTimer *timer;

#ifdef UNIT_TEST
    friend class Ut_NetworkTechnology;
#endif // UNIT_TEST

};

#endif // NETWORKTECHNOLOGY_H
