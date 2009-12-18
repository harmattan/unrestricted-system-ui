#ifndef SLIDERCONTAINER_H
#define SLIDERCONTAINER_H

#include <DuiContainer>

class DuiButton;
class DuiGridLayoutPolicy;
class DuiSlider;

class SliderContainer : public DuiContainer
{
    Q_OBJECT

public:
    SliderContainer(DuiWidget *parent = 0);
    virtual ~SliderContainer();

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

private: //methods
    void setLayout();
    void toggleSliderExistence(bool toggle);

private: //attributes
    DuiButton *PSMAutoButton;
    DuiSlider *PSMSlider;
    DuiGridLayoutPolicy *layoutPolicy;
    QStringList sliderValues;

};

#endif // SLIDERCONTAINER_H
