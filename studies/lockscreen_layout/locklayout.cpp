#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiLabel>
#include <DuiGridLayoutPolicy>
#include <DuiLinearLayoutPolicy>
#include <DuiLayout>
#include <DuiTheme>
#include <DuiSceneManager>
#include <DuiSlider>
#include <QGraphicsWidget>

#include <QDebug>

DuiLayout* createWidgets(DuiApplicationPage* p);

int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);
    qDebug() << Q_FUNC_INFO << DuiTheme::loadCSS("./locklayout.css");
    DuiApplicationWindow window;
    DuiApplicationPage page;
    page.setFullscreen(true);
    page.setDisplayMode(0);
    page.setPannableAreaInteractive(false);

    DuiLayout* layout = new DuiLayout;
    DuiLayout* widgets = createWidgets(&page);

    QSize l_size = DuiSceneManager::instance()->visibleSceneSize(Dui::Landscape);
    QSize p_size = DuiSceneManager::instance()->visibleSceneSize(Dui::Portrait);

    DuiGridLayoutPolicy* l_policy = new DuiGridLayoutPolicy(layout);
    l_policy->setSpacing(10);
    l_policy->setRowFixedHeight(1, l_size.height());
    l_policy->setColumnFixedWidth(0, l_size.width());
    l_policy->addItemAtPosition(widgets, 1, 0, Qt::AlignCenter);

    DuiGridLayoutPolicy* p_policy = new DuiGridLayoutPolicy(layout);
    p_policy->setSpacing(10);
    p_policy->setRowFixedHeight(1, p_size.height());
    p_policy->setColumnFixedWidth(0, p_size.width());
    p_policy->addItemAtPosition(widgets, 1, 0, Qt::AlignCenter);

    layout->setLandscapePolicy(l_policy);
    layout->setPortraitPolicy(p_policy);

    page.centralWidget()->setLayout(layout);

    window.show();
    page.appearNow();

    return app.exec();
}

DuiLayout* createWidgets(DuiApplicationPage* p)
{
    DuiLayout* layout = new DuiLayout(p);
    DuiGridLayoutPolicy* policy = new DuiGridLayoutPolicy(layout);

    policy->setSpacing(10);

    policy->setColumnAlignment(0, Qt::AlignCenter);

    QGraphicsWidget* spacert = new QGraphicsWidget;
    spacert->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));

    DuiLabel* time = new DuiLabel("10:45PM");
    time->setAlignment(Qt::AlignCenter);

    DuiLabel* date = new DuiLabel("Friday, 3rd September, 2009");
    date->setAlignment(Qt::AlignCenter);

    DuiSlider* slider = new DuiSlider(NULL, "dot");
    slider->setOrientation(Qt::Horizontal);
    slider->setObjectName("unlockslider");
    slider->setMinimumWidth(450);
    slider->setMaximumWidth(450);
    slider->setMaximumHeight(80);

    QGraphicsWidget* sl = new QGraphicsWidget;
    sl->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    DuiLabel* i1 = new DuiLabel("[1]");
    i1->setAlignment(Qt::AlignCenter);

    DuiLabel* i2 = new DuiLabel("[2]");
    i2->setAlignment(Qt::AlignCenter);

    DuiLabel* i3 = new DuiLabel("[3]");
    i3->setAlignment(Qt::AlignCenter);

    DuiLabel* i4 = new DuiLabel("[4]");
    i4->setAlignment(Qt::AlignCenter);

    QGraphicsWidget* sr = new QGraphicsWidget;
    sr->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    QGraphicsWidget* spacerb = new QGraphicsWidget;
    spacerb->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));


    /* 0   1    2    3    4    5
      +---------------------------+
    0 |           time            |
      +---------------------------+
    1 |           date            |
      +---------------------------+
    2 |          slider           |
      +---+----+----+----+----+---+
    3 |sl | i1 | i2 | i3 | i4 |sr |
      +---+----+----+----+----+---+
    */


    // row, column, rowspan, colspan

    policy->addItemAtPosition(spacert, 1, 0, 1, 6);
    policy->addItemAtPosition(time,    2, 0, 1, 6);
    policy->addItemAtPosition(date,    3, 0, 1, 6);
    policy->addItemAtPosition(slider,  4, 0, 1, 6);
    policy->addItemAtPosition(sl,      5, 0);
    policy->addItemAtPosition(i1,      5, 1);
    policy->addItemAtPosition(i2,      5, 2);
    policy->addItemAtPosition(i3,      5, 3);
    policy->addItemAtPosition(i4,      5, 4);
    policy->addItemAtPosition(sr,      5, 5);
    policy->addItemAtPosition(spacerb, 6, 0, 1, 6);

    return layout;
}
