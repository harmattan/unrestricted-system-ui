#ifndef PROFILECONTAINER_H
#define PROFILECONTAINER_H

#include <DuiContainer>

class DuiButton;
class DuiSlider;

class ProfileContainer: public DuiContainer
{
    Q_OBJECT

public:
    ProfileContainer(const QString &title, const QStringList& volumeLevels, int levelIndex, bool vibrationEnabled, DuiWidget *parent);
    virtual ~ProfileContainer();

    void setLevel(int levelIndex);
    void setVibration(bool enabled);

signals:
    void sliderValueChanged(int newValue);
    void vibrationChanged(bool on);

private:
    void setLayout();

private:
    const QStringList& volumeLevels;
    DuiSlider* slider;
    DuiButton* button;
    int levelIndex;
};

#endif // PROFILECONTAINER_H
