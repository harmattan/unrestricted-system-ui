#ifndef UT_NETWORKBUSINESSLOGIC_H
#define UT_NETWORKBUSINESSLOGIC_H

#include "radioaccessstub.h"
#include "networktechnology.h"

#include <QtTest/QtTest>
#include <QObject>
#include <QSignalSpy>


class Ut_NetworkTechnology : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testNetworkIcon();    

private: //methods
    void checkSpy(int count = 0, NetworkTechnology::Technology technology = NetworkTechnology::None);

private: // attributes
    Cellular::RadioAccess *radioAccessStub;
    NetworkTechnology *m_subject;
    QSignalSpy *spy;
};

#endif
