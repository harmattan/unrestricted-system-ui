/*
 * Test application.
 * Tests page creation & deletion frequently.
 */

#include "main.h"

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <QDebug>

/* TestPage */
TestPage::TestPage()
{
    qDebug() << Q_FUNC_INFO;
    setTitle("TestPage");
    createContent();
}

TestPage::~TestPage()
{
    qDebug() << Q_FUNC_INFO;
}

/* TestObj */
TestObj::TestObj() : QObject(), time(0)
{
    timer = new QTimer(0);
    connect(timer, SIGNAL(timeout()), this, SLOT(handlePage()));
    handlePage();
}

TestObj::~TestObj()
{
    delete p;
    if(timer != NULL)
    {
        timer->stop();
        delete timer;
        timer = NULL;
    }
}

void TestObj::handlePage()
{
    switch(time)
    {
        case 0: // start timer for 5 sec and
            qDebug() << Q_FUNC_INFO << "start timer," << time;
            time = 5;
            timer->start(time*1000);
        case 2: // create page
            if(p == NULL)
            {
                p = new TestPage();
                p->appear();
                qDebug() << Q_FUNC_INFO << "new & p->appear()," << time;
            }
            for (int i = 0; i < 300; ++i)
            {
                p->appear();
            }
            qDebug() << Q_FUNC_INFO << "p->appear() x 300" << time;
        break;
        case 5: // delete page and wait 2 sec
            time = 2;
            p->disappear();
            qDebug() << Q_FUNC_INFO << "p->disappear()" << time;
            delete p;
            timer->start(time);
        break;
    }
}


int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);

    DuiApplicationWindow w;
    w.show();

    TestObj obj;

    return app.exec();
}

