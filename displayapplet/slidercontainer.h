#ifndef SLIDERCONTAINER_H
#define SLIDERCONTAINER_H

#include <DuiContainer>

class DuiLabel;
class DuiSlider;

class SliderContainer : public DuiContainer
{
    Q_OBJECT

public:
    SliderContainer(DuiWidget *parent, const QString &infoText, const QString &valuePattern = QString("%1"));
    virtual ~SliderContainer();

public slots:
    void initSlider(int index, const QStringList &values);    

private slots:    
    void valueChanged(int value);

private:
    void updateSlider(const QString &value);

signals:    
    void sliderValueChanged(QString);

private: //methods
    void setLayout();   

private: //attributes
    DuiLabel *label;
    DuiSlider *slider;
    QString valuePattern;
    QStringList sliderValues;

};

#endif // SLIDERCONTAINER_H
