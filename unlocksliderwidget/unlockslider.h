#ifndef UNLOCKSLIDER_H
#define UNLOCKSLIDER_H

#include <DuiWidgetController>
#include "unlockslidermodel.h"

class UnlockSlider : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(UnlockSlider)

public:
    UnlockSlider();
    virtual ~UnlockSlider();
};

#endif // UNLOCKSLIDER_H
