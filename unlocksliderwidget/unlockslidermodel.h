#ifndef UNLOCKSLIDERMODEL_H
#define UNLOCKSLIDERMODEL_H

#include <duiwidgetmodel.h>

class UnlockSliderModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL(UnlockSliderModel)

public:
    DUI_MODEL_PROPERTY(bool, handlePressed, HandlePressed, true, false)
    DUI_MODEL_PROPERTY(qreal, position, Position, false, 0)
    DUI_MODEL_PROPERTY(bool, iconVisible, IconVisible, true, true)
    // TODO: blink/hilight/whatever
};

#endif // UNLOCKSLIDERMODEL_H
