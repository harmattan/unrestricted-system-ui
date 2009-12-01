#include <QTimer>
#include <DuiButton>
#include <DuiHomeButtonPanel>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <QDebug>

#include "main.h"

TestObj::TestObj() {}
TestObj::~TestObj() {}

void TestObj::toggled(bool b) 
{
  qDebug() << "toggled" << b;
}


int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);
    qInstallMsgHandler(0);
    DuiApplicationWindow w;

    DuiApplicationPage p;
    DuiButton button(QString("Ju"), &p);
    button.setGeometry(QRect(100,200,200,200));

    TestObj t;
    QObject::connect(&button, SIGNAL(toggled(bool)), &t, SLOT(toggled(bool)));

    button.setCheckable(true);
    button.setChecked(true);


    p.appear();
    w.show();

    w.homeButtonPanel()->disappearNow();

    return app.exec();
}

