/*
 * Test application.
 * Creates a page with freestyle layout and rotates view.
 */
#include "main.h"

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <QDebug>
#include <DuiButton>
#include <DuiGridLayoutPolicy>
#include <DuiFreestyleLayoutPolicy>
#include <DuiSceneManager>


/* TestPage */
TestPage::TestPage()
{
    qDebug() << "TestPage()";
    setFullscreen(true);
    setNavigationBarVisible(false);
    setTitle("TestPage");
    createContent();
}

TestPage::~TestPage()
{
    qDebug() << "~TestPage()";
    delete btn;
}

void TestPage::createContent()
{
    DuiApplicationPage::createContent();

    btn = new DuiButton(QString("Close"), 0 );
    btn->setObjectName("Button");

    //createGridLayout();
    createFreestyleLayout();

    connect(DuiSceneManager::instance(),
        SIGNAL(orientationChanged(const Dui::Orientation &)),
        this, SLOT(orientationChanged(const Dui::Orientation &)));

}

void TestPage::createGridLayout()
{
    DuiLayout *layout = new DuiLayout(centralWidget());
    DuiGridLayoutPolicy* policy = new DuiGridLayoutPolicy(layout);
    policy->addItemAtPosition(btn, 0, 0, 1, 5);
}

void TestPage::createFreestyleLayout()
{
    DuiLayout *layout = new DuiLayout();
    centralWidget()->setLayout(layout);

    qreal left,top,right,bottom;
    layout->getContentsMargins(&left,&top,&right,&bottom);
    qDebug() << "margins l:"<<left<<"r:"<<right<<"t:"<<top<<"b:"<<bottom;


    // 864 x 480
    QSize size = DuiSceneManager::instance()->visibleSceneSize();
    int horizontal = size.width() - (left + right);
    int vertical = size.height() - (top + bottom);
    qDebug() << "horizontal:" << horizontal << "vertical:" << vertical;

    if(horizontal < vertical)
    {
        int tmp = horizontal;
        horizontal = vertical;
        vertical = tmp;
    }

    // Landscape
    DuiFreestyleLayoutPolicy* l_policy = new DuiFreestyleLayoutPolicy(layout);
    l_policy->setObjectName("l_policy");
    layout->setLandscapePolicy(l_policy);

    QRect rect = QRect(QPoint(40, vertical*0.81f), QPoint(horizontal-40, vertical*0.95f));
    l_policy->addItemAtGeometry(btn, rect);

    // Portrait
    DuiFreestyleLayoutPolicy* p_policy = new DuiFreestyleLayoutPolicy(layout);
    p_policy->setObjectName("p_policy");
    layout->setPortraitPolicy(p_policy);

    rect = QRect(QPoint(40, horizontal*0.81f), QPoint(vertical-40, horizontal*0.95f));
    p_policy->addItemAtGeometry(btn, rect);

    qDebug() << "activate" << ((DuiLayout*)(centralWidget()->layout()))->policy()->objectName();
    ((DuiLayout*)(centralWidget()->layout()))->policy()->activate();
}

void TestPage::orientationChanged(const Dui::Orientation &orientation)
{
    qDebug() << "orientationChanged:" << orientation << "for" << ((DuiLayout*)(centralWidget()->layout()))->policy()->objectName();

    int horizontal = DuiSceneManager::instance()->visibleSceneSize().width();
    int vertical = DuiSceneManager::instance()->visibleSceneSize().height();
    qDebug() << "w:" << horizontal << "h:" << vertical;

    //    ((DuiLayout*)(centralWidget()->layout()))->policy()->activate();
/*   if (orientation == Dui::Portrait) {
        qDebug() << "kissa";
        p_policy->activate();
    } else {
        l_policy->activate();
    }
*/}

DuiButton* TestPage::getButton()
{
    return btn;
}


/* TestObj */
TestObj::TestObj() : QObject(), time(0), timer(NULL)
{
    // pageAppear();
    createPage();
}

void TestObj::createPage()
{
    p = new TestPage();
    connect(p->getButton(), SIGNAL(released()), this, SLOT(close()));

    p->appear();
}

void TestObj::close()
{
    qDebug() << "close()";
    p->disappear();
    DuiApplication::instance()->applicationWindow()->hide();
}

void TestObj::pageAppear()
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

void TestObj::doRotation()
{
    int orientation = (int)DuiSceneManager::instance()->orientationAngle();
    orientation += 90;
    orientation %= 360;
    DuiSceneManager::instance()->setOrientationAngle((Dui::OrientationAngle) orientation);
}

void TestObj::handlePage()
{
    switch(time)
    {
        case 0: // start timer for 5 sec and
            time = 5;
            timer->start(time*1000);
        case 2: // create page
            if(p == NULL)
            {
                createPage();
            }
            for (int i = 0; i < 300; ++i)
            {
                p->appear();
            }
        break;
        case 5: // delete page and wait 2 sec
            time = 2;
            p->disappear();
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

    /* Comment in if rotation test needed. */
    QTimer *rotation = new QTimer(0);
    rotation->setInterval(5*1000);
    rotation->start();
    QObject::connect(rotation, SIGNAL(timeout()), &obj, SLOT(doRotation()));
    /**/

    return app.exec();
}

