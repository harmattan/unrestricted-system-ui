#ifndef UT_TEMPLATE_H
#define UT_TEMPLATE_H

#include <QtTest/QtTest>
#include <QCoreApplication>
#include <QObject>

class Ut_Template : public QObject
{
    Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();
    // List the testcases here:
    void testCase1 ();

private:
    QCoreApplication    *m_app;
};

#endif
