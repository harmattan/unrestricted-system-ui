#include <QTimer>
#include <DuiButton>
#include <DuiApplication>
#include <QDebug>

#include "main.h"


TestObj::TestObj(DuiApplicationWindow &w) : QObject() {
    this->win = &w;
    this->visible = false;
}
TestObj::~TestObj() { }
void TestObj::doStuff()
{
    static int counter = 0;
    qDebug() << this->visible << counter++;
    if (this->visible = !this->visible)
        this->win->show();
    else
        this->win->hide();
}

int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);
    qInstallMsgHandler(0);
    DuiApplicationWindow w;

    DuiApplicationPage p;
    DuiButton button(QString("Just another widget"), &p);

    p.appear();

    TestObj obj(w);
    QTimer *timer = new QTimer(&app);
    timer->setInterval(3*1000);
    timer->start();
    QObject::connect(timer, SIGNAL(timeout()), &obj, SLOT(doStuff()));

    return app.exec();
}

