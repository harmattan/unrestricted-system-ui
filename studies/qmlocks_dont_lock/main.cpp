#include <QTimer>
#include <DuiButton>
#include <DuiHomeButtonPanel>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <QDebug>



#include "main.h"

TestObj::TestObj() {}
TestObj::~TestObj() {}

void TestObj::locksChanged(Maemo::QmLocks::Lock l, Maemo::QmLocks::State s) 
{
	qDebug() << "lock " << l << ", state "<< (s==Maemo::QmLocks::Locked?"Locked": "unlocked");
}


int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);
    qInstallMsgHandler(0);

    Maemo::QmLocks l;
    TestObj t;
    QObject::connect(&l, SIGNAL(stateChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)), 
	    &t, SLOT(locksChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)));


    return app.exec();
}

