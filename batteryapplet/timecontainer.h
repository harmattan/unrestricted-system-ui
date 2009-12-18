#ifndef TIMECONTAINER_H
#define TIMECONTAINER_H

#include <DuiContainer>

class DuiImage;
class DuiLabel;

class TimeContainer : public DuiContainer
{
public:
    TimeContainer(const QString &text, DuiImage *image, DuiWidget *parent = 0);
    virtual ~TimeContainer();

    void updateTimeLabel(const QString &text);

private:
    void setLayout();

private:
    DuiImage *image;
    DuiLabel *textLabel;
    DuiLabel *timeLabel;

};

#endif // STANDBYTIMECONTAINER_H
