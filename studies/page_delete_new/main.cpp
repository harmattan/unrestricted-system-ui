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
TestObj::TestObj() : QObject(), time(2)
{
    handlePage();
}

TestObj::~TestObj()
{
    delete p;
}

void TestObj::handlePage()
{
    switch(time)
    {
        case 1: // delete page and wait 2 sec
            time = 2;
            qDebug() << Q_FUNC_INFO << "to disappear:" << (QObject*)p;
            if(p){
                qDebug() << Q_FUNC_INFO << "to disappear:" << (QObject*)p << "--> t->disappearNow(), wait" << time;
                p->disappearNow();
            }
            QTimer::singleShot(time*500, this, SLOT(handlePage()));
        break;
        case 2: // create page
            time = 1;
            QTimer::singleShot(time*500, this, SLOT(handlePage()));
            qDebug() << Q_FUNC_INFO << "to appear:" << (QObject*)p;
            if(!p){
                p = new TestPage();
                sleep(1);
            }
            qDebug() << Q_FUNC_INFO << "--> p->appearNow(DuiSceneWindow::DestroyWhenDone), wait" << time;
            p->appearNow(DuiSceneWindow::DestroyWhenDone);
        break;
    }
    qDebug() << Q_FUNC_INFO << "out" << (QObject*)p;
}


int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);

    DuiApplicationWindow w;
    w.show();

    TestObj obj;

    return app.exec();
}

