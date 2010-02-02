/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#ifndef PROFILECONTAINER_H
#define PROFILECONTAINER_H

#include <DuiContainer>

class DuiLabel;
class DuiButton;
class DuiSlider;
class DuiImageWidget;

/*!
 * A widget that shows and controls the basic properties of a profile: the mute
 * state, the volume level and the vibration on/off property.
 */
class ProfileContainer: public DuiContainer
{
    Q_OBJECT

public:
    ProfileContainer(
            int            id, 
            const QString &title, 
            int            level, 
            bool           vibra, 
            DuiWidget     *parent = 0);
    ~ProfileContainer ();

    void setLevel (int value);
    void setVibration (bool enabled);
    int id ();

signals:
    void sliderValueChanged (int newValue);
    void vibrationChanged (bool enabled);

private slots:
    void slotSliderValueChanged (int newValue); 
    void slotVibrationSwitchToggled (bool toggle);
    
private:
    void setLayout();

private:
    DuiSlider         *m_Slider;
    DuiButton         *m_VibrationSwitch;
    DuiLabel          *m_VibrationLabel;
    DuiLabel          *m_OnOffLabel;
    DuiImageWidget    *m_Img;
    int                m_Level;
    const int          m_ProfileId;
};

#endif
