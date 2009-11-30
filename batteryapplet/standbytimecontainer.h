#ifndef STANDBYTIMECONTAINER_H
#define STANDBYTIMECONTAINER_H

#include "timecontainer.h"

class StandByTimeContainer : public TimeContainer
{
public:
    StandByTimeContainer(DuiWidget *parent=0);
    virtual ~StandByTimeContainer();

private: //methods
    void setLayout();

};

#endif // STANDBYTIMECONTAINER_H
