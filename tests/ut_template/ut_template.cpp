#include "ut_template.h"

void
Ut_Template::init ()
{
    // Instantiate the classes what we want to test
}

void
Ut_Template::cleanup ()
{
    // Drop the (tested) objects
}

void
Ut_Template::initTestCase ()
{
    // Init QT
    int argc = 1;
    char* app_name = (char*) "./ut_template";

    m_app = new QCoreApplication (argc, &app_name);
}

void
Ut_Template::cleanupTestCase ()
{
    delete m_app;
}

void
Ut_Template::testCase1 ()
{
    // XXX: Implement here a test case
    const QString test = "10";

    QCOMPARE(test, QString ("10"));

    QVERIFY(1 == 1);
}

QTEST_APPLESS_MAIN(Ut_Template)
