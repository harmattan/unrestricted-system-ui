#ifndef PROFILECONTAINER_H
#define PROFILECONTAINER_H

#include <DuiContainer>

class DuiButton;
class DuiSlider;

class ProfileContainer: public DuiContainer
{
    Q_OBJECT

public:
    ProfileContainer(int id, const QString &title, int level, bool vibra, DuiWidget *parent=0);
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
