#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiLabel>
#include <DuiGridLayoutPolicy>
#include <DuiLayout>
#include <DuiTheme>
#include <DuiSceneManager>
#include <DuiSlider>
#include <QGraphicsWidget>

#include <QDebug>

int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);
    qDebug() << Q_FUNC_INFO << DuiTheme::loadCSS("locklayout.css");
    DuiApplicationWindow window;
    DuiApplicationPage page;

    page.centralWidget()->setGeometry(QRectF(0, 0, 300, 300));
    page.setFullscreen(true);
    page.setDisplayMode(0);
    page.setPannableAreaInteractive(false);

    DuiLayout* layout = new DuiLayout;

    DuiGridLayoutPolicy* l_policy = new DuiGridLayoutPolicy(layout);
    DuiGridLayoutPolicy* p_policy = new DuiGridLayoutPolicy(layout);

    l_policy->setSpacing(0);
    p_policy->setSpacing(0);

    for(int r=0; r<6; r++) {
        l_policy->setRowAlignment(r, Qt::AlignCenter);
        p_policy->setRowAlignment(r, Qt::AlignCenter);
        l_policy->setColumnAlignment(r, Qt::AlignCenter);
        p_policy->setColumnAlignment(r, Qt::AlignCenter);
    }

    QGraphicsWidget* s_l = new QGraphicsWidget;
    s_l->setMinimumHeight((480/2)-(194/2));
    s_l->setMaximumHeight((480/2)-(194/2));
    QGraphicsWidget* s_p = new QGraphicsWidget;
    s_p->setMinimumHeight((864/2)-(194/2));
    s_p->setMaximumHeight((864/2)-(194/2));
    DuiLabel* time = new DuiLabel("10:45PM");
    time->setAlignment(Qt::AlignCenter);
    DuiLabel* date = new DuiLabel("Friday, 3rd September, 2009");
    date->setAlignment(Qt::AlignCenter);
    QGraphicsWidget* sl = new QGraphicsWidget;
    DuiSlider* slider = new DuiSlider(NULL, "dot");
    slider->setOrientation(Qt::Horizontal);
    slider->setObjectName("unlockslider");
    slider->setMinimumWidth(350);
    slider->setMaximumWidth(380);
    slider->setMaximumHeight(80);
    QGraphicsWidget* s2 = new QGraphicsWidget;
    s2->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    DuiLabel* i1 = new DuiLabel("[1]");
    i1->setAlignment(Qt::AlignCenter);
    DuiLabel* i2 = new DuiLabel("[2]");
    i2->setAlignment(Qt::AlignCenter);
    DuiLabel* i3 = new DuiLabel("[3]");
    i3->setAlignment(Qt::AlignCenter);
    DuiLabel* i4 = new DuiLabel("[4]");
    i4->setAlignment(Qt::AlignCenter);
    QGraphicsWidget* s3 = new QGraphicsWidget;
    s3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    QGraphicsWidget* sr = new QGraphicsWidget;

    /* 0   1    2    3    4    5
      +---------------------------+
    0 |s_l/s_p                    |
      +---------------------------+
    1 |           time            |
      +---------------------------+
    2 |           date            |
      +---+-----------------------+
    3 |sl |      slider       |sr |
      +---+----+----+----+----+---+
    4 |s2 | i1 | i2 | i3 | i4 |s3 |
      +---+----+----+----+----+---+
    5 |s4                         |
      +---------------------------+
      */

    // row, column, rowspan, colspan

    l_policy->addItemAtPosition(s_l,     0, 0, 1, 6);
    l_policy->setRowStretchFactor(0, 1);
    l_policy->addItemAtPosition(time,   1, 0, 1, 6);
    l_policy->addItemAtPosition(date,   2, 0, 1, 6);
    l_policy->addItemAtPosition(sl,     3, 0);
    l_policy->addItemAtPosition(slider, 3, 1, 1, 4);
    l_policy->addItemAtPosition(sr,     3, 5);
    l_policy->addItemAtPosition(s2,     4, 0);
    l_policy->addItemAtPosition(i1,     4, 1);
    l_policy->addItemAtPosition(i2,     4, 2);
    l_policy->addItemAtPosition(i3,     4, 3);
    l_policy->addItemAtPosition(i4,     4, 4);
    l_policy->addItemAtPosition(s3,     4, 5);
    l_policy->setRowStretchFactor(5, 1);


    p_policy->addItemAtPosition(s_p,     0, 0, 1, 6);
    p_policy->setRowStretchFactor(0, 1);
    p_policy->addItemAtPosition(time,   1, 0, 1, 6);
    p_policy->addItemAtPosition(date,   2, 0, 1, 6);
    p_policy->addItemAtPosition(sl,     3, 0);
    p_policy->addItemAtPosition(slider, 3, 1, 1, 4);
    p_policy->addItemAtPosition(sr,     3, 5);
    p_policy->addItemAtPosition(s2,     4, 0);
    p_policy->addItemAtPosition(i1,     4, 1);
    p_policy->addItemAtPosition(i2,     4, 2);
    p_policy->addItemAtPosition(i3,     4, 3);
    p_policy->addItemAtPosition(i4,     4, 4);
    p_policy->addItemAtPosition(s3,     4, 5);
    p_policy->setRowStretchFactor(5, 1);


    layout->setLandscapePolicy(l_policy);
    layout->setPortraitPolicy(p_policy);

    /* Attach the layout to the page */
    page.centralWidget()->setObjectName("screencentralwidget");
    page.centralWidget()->setLayout(layout);
    window.show();
    page.appearNow();
    return app.exec();
}
