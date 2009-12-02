#ifndef PROFILEBUTTONS_H
#define PROFILEBUTTONS_H

#include <DuiContainer>

class DuiButtonGroup;

class ProfileButtons : public DuiContainer
{
    Q_OBJECT

public:
    ProfileButtons(DuiWidget *parent=0);
    virtual ~ProfileButtons();

    void init(QMap<int, QString> data, int currentId);
    QString selectedProfileName();

    bool selectProfile(int id);

signals:
    void profileSelected(int id);

private slots:
    void buttonToggled(bool checked);

private:    
    void addButton(QString name, int id, bool checked);
    void setLayout();    

private:   
    DuiButtonGroup *buttons;    

};

#endif // PROFILEBUTTONS_H
