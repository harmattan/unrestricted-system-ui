#ifndef VOLUMEBAR_H
#define VOLUMEBAR_H

#include <DuiWidget>

class DuiSlider;
class DuiContainer;
class VolumeBarLogic;
class DuiStatusIndicatorMenuInterface;

class VolumeBar : public DuiWidget
{
    Q_OBJECT

public:
    VolumeBar (DuiStatusIndicatorMenuInterface &statusIndicatorMenu,
               QGraphicsItem *parent = NULL);
    virtual ~VolumeBar ();

private slots:
    void volumeChanged (int val);

private:
    DuiSlider       *m_bar;
    VolumeBarLogic  *m_logic;
};

#endif
