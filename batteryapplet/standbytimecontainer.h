#ifndef STANDBYTIMECONTAINER_H
#define STANDBYTIMECONTAINER_H

#include "timecontainer.h"

class DuiLabel;

class StandByTimeContainer : public TimeContainer
{
public:
    StandByTimeContainer(DuiWidget *parent);
    virtual ~StandByTimeContainer();



private: //methods
    void setLayout();

private: //attributes
    DuiLabel *timeLabel;

};

#endif // STANDBYTIMECONTAINER_H
