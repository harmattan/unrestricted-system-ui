#include <DuiApplication>
#include <QDebug>

#include "main.h"

TestObj::TestObj()
{
    qDebug() << "Shutdown listener: Creating TestObj";
    st = new Maemo::QmSystemState();
    connect(st, SIGNAL(systemStateChanged(Maemo::QmSystemState::StateIndication)),
            this, SLOT(systemStateChanged(Maemo::QmSystemState::StateIndication)));    
}
TestObj::~TestObj()
{ 
    delete st;
    st = NULL;
}

void TestObj::systemStateChanged(Maemo::QmSystemState::StateIndication state)
{
    qDebug() << "Shutdown listener: " << state;
}

int main(int argc, char** argv)
{    
    DuiApplication app(argc, argv);
    qInstallMsgHandler(0);
    qDebug() << "Starting shutdown listener";
    TestObj t;
    return app.exec();
}

