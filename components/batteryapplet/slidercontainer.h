#ifndef SLIDERCONTAINER_H
#define SLIDERCONTAINER_H

#include <DuiContainer>

class DuiButton;
class DuiLabel;
class DuiSlider;
class DuiLinearLayoutPolicy; 

class SliderContainer : public DuiContainer
{
    Q_OBJECT

public:
    SliderContainer (DuiWidget *parent = 0);
    virtual ~SliderContainer ();
    void retranslate ();

public slots:
    void initSlider(const QStringList &values);
    void updateSlider(const QString &value);
    void initPSMAutoButton(bool toggle);
    void PSMAutoDisabled();

private slots:
    void PSMAutoButtonToggled(bool toggle);
    void sliderValueChanged(int value);

signals:
    void PSMAutoToggled(bool);
    void PSMThresholdValueChanged(QString);

private:
    DuiButton               *PSMAutoButton;
    DuiSlider               *PSMSlider;
    DuiLabel                *textLabel;
    QStringList              sliderValues;
    DuiLinearLayoutPolicy   *layout_policy;

    void    setLayout();
    void    toggleSliderExistence (bool toggle);

};

#endif // SLIDERCONTAINER_H
