#ifndef TIMECONTAINER_H
#define TIMECONTAINER_H

#include <DuiContainer>

class DuiImageWidget;
class DuiLabel;

class TimeContainer : public DuiContainer
{
public:
    TimeContainer(const QString &text, DuiImageWidget *image, DuiWidget *parent = 0);
    virtual ~TimeContainer();

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
