/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#ifndef PROFILECONTAINER_H
#define PROFILECONTAINER_H

#include <DuiContainer>

class DuiButton;
class DuiSlider;

class ProfileContainer: public DuiContainer
{
    Q_OBJECT

public:
    ProfileContainer(int id, const QString &title, int level, bool vibra, DuiWidget *parent = 0);
    virtual ~ProfileContainer();

    void setLevel(int value);
    void setVibration(bool enabled);
    int id();

signals:
    void sliderValueChanged(int newValue);
    void vibrationChanged(bool enabled);

private:
    void setLayout();

private:
    DuiSlider* slider;
    DuiButton* button;
    int level;
    const int profileId;
};

#endif // PROFILECONTAINER_H
