#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include "dcpwidget.h" 
#include "profilebuttons.h"


class profileDBusInterface;
class DuiButton;
class DuiContainer;
class DuiGridLayoutPolicy;
class DuiSlider;
class DuiWidgetController;
class ProfileContainer;

class ProfileWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    ProfileWidget(QGraphicsWidget *parent = 0);
    virtual ~ProfileWidget();

    QString currentProfile();

protected:
    void initWidget();

private slots:
    void profileSelected(ProfileButtons::ProfileId id);
    void sliderValueChanged(int index);
    void vibrationChanged(bool on);
    void orientationChanged(const Dui::Orientation &);

private: //methods
    DuiContainer* createContainer();

    void initSlider(DuiSlider *slider, int index, const QStringList &values);
    void updateSliderThumbLabel(DuiSlider *slider, const QString &value, const QString &pattern = QString(""));

private: //attributes

    ProfileButtons* profileButtons;
    ProfileContainer* ringingContainer;
    ProfileContainer* loudContainer;
    ProfileContainer* beepContainer;
    ProfileContainer* silentContainer;

    // DuiButtonGroup* btnGroup;

    QStringList volumeValues;
    profileDBusInterface *profileIf;    
};
#endif // PROFILEWIDGET_H
