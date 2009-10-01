#include <DuiTheme>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiSlider>

#include "unlockslider.h"
#include "unlocksliderview.h"

int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);

    DuiApplicationWindow w;
    w.show();

    DuiApplicationPage p;
    p.appear();

    DuiTheme::addPixmapDirectory("./");
    DuiTheme::loadCSS("./unlocksliderstyle.css");

    // Create widget controller and view.
    UnlockSlider slider1;
    UnlockSlider slider2;
    slider1.setOrientation(Qt::Horizontal);
    slider2.setOrientation(Qt::Vertical);
    slider1.setGeometry(QRectF(400, 0, 400, 400));
    slider2.setGeometry(QRectF(0, 0, 400, 400));

    // Attach widget to parent widget so it will be drawn and run dui application.
    slider1.setParentItem(p.centralWidget());
    slider2.setParentItem(p.centralWidget());

    return app.exec();
}

