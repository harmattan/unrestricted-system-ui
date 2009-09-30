#ifndef UNLOCKSLIDERMODEL_H
#define UNLOCKSLIDERMODEL_H

#include <duiwidgetmodel.h>

class UnlockSliderModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL(UnlockSliderModel)

public:
    DUI_MODEL_PROPERTY(bool,            handlePressed, HandlePressed, true,  false)
    DUI_MODEL_PROPERTY(qreal,           position,      Position,      false, 0)
    DUI_MODEL_PROPERTY(bool,            iconVisible,   IconVisible,   true,  true)
    DUI_MODEL_PROPERTY(bool,            magnetic,      Magnetic,      true,  true)
    DUI_MODEL_PROPERTY(bool,            blinking,      Blinking,      true,  false)
    DUI_MODEL_PROPERTY(Qt::Orientation, orientation,   Orientation,   true,  Qt::Horizontal)
};

#endif // UNLOCKSLIDERMODEL_H
