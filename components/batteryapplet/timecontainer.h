#ifndef TIMECONTAINER_H
#define TIMECONTAINER_H

#include <DuiContainer>

class DuiImageWidget;
class DuiLabel;

/*!
 * A simple widget with two labels, one is the title/meaning of the widget, the
 * other is a time value. This widget is used to show the remaining battery time
 * in stand-by and active mode.
 * This widget also holds an image, but does not handle its changes.
 */
class TimeContainer : public DuiContainer
{
public:
    TimeContainer(const QString &text, DuiImageWidget *image,
                  DuiWidget *parent = 0);

    void updateTimeLabel (const QString &text);
    void setText (const QString &text);

private:
    void setLayout();

private:
    DuiImageWidget *image;
    DuiLabel *textLabel;
    DuiLabel *timeLabel;

};

#endif // STANDBYTIMECONTAINER_H
