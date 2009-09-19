#ifndef UNLOCKSLIDERMODEL_H
#define UNLOCKSLIDERMODEL_H

#include <duiwidgetmodel.h>

class UnlockSliderModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL(UnlockSliderModel)

public:
    enum SliderState
    {
        Pressed,
        Released
    };

    DUI_MODEL_PROPERTY(UnlockSliderModel::SliderState, state, State, true, UnlockSliderModel::Released)
    DUI_MODEL_PROPERTY(qreal, position, Position, false, 0)
    DUI_MODEL_PROPERTY(bool, lockIconVisible, LockIconVisible, true, true)
    // TODO: blink/hilight/whatever

    // I guess orientation is not needed in our case?
    // DUI_MODEL_PROPERTY(Qt::Orientation, orientation, Orientation, true, Qt::Vertical)
};

#endif // UNLOCKSLIDERMODEL_H
