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
    DuiTheme::loadCSS("unlocksliderstyle.css");

    // Create widget controller and view.
    UnlockSlider slider;
    UnlockSliderView view(&slider);
    slider.setView(&view);

    // Set geometry (active area) of widget.
    slider.setGeometry(QRectF(100, 200, 500, 120));

    // Attach widget to parent widget so it will be drawn and run dui application.
    slider.setParentItem(p.centralWidget());

    return app.exec();
}

