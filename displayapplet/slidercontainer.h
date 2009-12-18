#ifndef SLIDERCONTAINER_H
#define SLIDERCONTAINER_H

#include <DuiContainer>

class DuiLabel;
class DuiSlider;

class SliderContainer : public DuiContainer
{
    Q_OBJECT

public:
    SliderContainer(const QString &infoText, const QString &valuePattern = QString("%1"), DuiWidget *parent = 0);
    virtual ~SliderContainer();

    void initSlider(const QList<int> &values, int index);

private slots:
    void valueChanged(int index);

private:
    void updateSlider(int value);

signals:
    void sliderValueChanged(int);

private: //methods
    void setLayout();

private: //attributes
    DuiLabel *label;
    DuiSlider *slider;
    QString valuePattern;
    QList<int> sliderValues;

};

#endif // SLIDERCONTAINER_H
