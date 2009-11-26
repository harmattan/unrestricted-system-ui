#ifndef TIMECONTAINER_H
#define TIMECONTAINER_H

#include <DuiContainer>

class DuiLabel;

class TimeContainer : public DuiContainer
{
public:
    TimeContainer(DuiWidget *parent=0);
    virtual ~TimeContainer();

    void updateTimeLabel(const QString &text);

protected: //methods
    virtual void setLayout() = 0;

protected: //attributes    
    DuiLabel *timeLabel;    

};

#endif // STANDBYTIMECONTAINER_H
