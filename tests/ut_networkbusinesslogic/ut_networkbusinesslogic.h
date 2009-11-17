#ifndef UT_NETWORKBUSINESSLOGIC_H
#define UT_NETWORKBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

class NetworkBusinessLogic;

class Ut_NetworkBusinessLogic : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testNetworkIcon();
    void testQueryAvailableOperators();
    void testSelectOperator();
    void testSignalStrengthIcon();

private: //methods
    QStringList addOperators();

private: // attributes
    NetworkBusinessLogic* m_subject;    
    QStringList operatorNames;    
};

#endif
