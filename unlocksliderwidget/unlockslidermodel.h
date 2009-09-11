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

    DUI_MODEL_PROPERTY(int, minimum, Minimum, false, 0)
    DUI_MODEL_PROPERTY(int, maximum, Maximum, false, 100)
    DUI_MODEL_PROPERTY(int, value,   Value,   false, 0)
};

/*
00032     DUI_MODEL_PROPERTY(DuiSliderModel::SliderState, state, State, true, DuiSliderModel::Released)
00033     DUI_MODEL_PROPERTY(int, minimum, Minimum, false, 0)
00034     DUI_MODEL_PROPERTY(int, maximum, Maximum, false, 100)
00035     DUI_MODEL_PROPERTY(int, value, Value, false, 0)
00036     DUI_MODEL_PROPERTY(Qt::Orientation, orientation, Orientation, true, Qt::Vertical)
00037     DUI_MODEL_PROPERTY(QString, minLabelIcon, MinLabelIcon, true, QString())
00038     DUI_MODEL_PROPERTY(QString, minLabelText, MinLabelText, true, QString())
00039     DUI_MODEL_PROPERTY(bool, minLabelVisible, MinLabelVisible, true, true)
00040     DUI_MODEL_PROPERTY(QString, maxLabelIcon, MaxLabelIcon, true, QString())
00041     DUI_MODEL_PROPERTY(QString, maxLabelText, MaxLabelText, true, QString())
00042     DUI_MODEL_PROPERTY(bool, maxLabelVisible, MaxLabelVisible, true, true)
00043     DUI_MODEL_PROPERTY(QString, thumbLabelIcon, ThumbLabelIcon, true, QString())
00044     DUI_MODEL_PROPERTY(QString, thumbLabelText, ThumbLabelText, true, QString())
00045     DUI_MODEL_PROPERTY(bool, thumbLabelVisible, ThumbLabelVisible, true, true)

  */

#endif // UNLOCKSLIDERMODEL_H
