/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef PROFILEBUTTONS_H
#define PROFILEBUTTONS_H

#include <DuiContainer>

class DuiButtonGroup;

class ProfileButtons : public DuiContainer
{
    Q_OBJECT

public:
    ProfileButtons (DuiWidget *parent = 0);
    virtual ~ProfileButtons();

    void init (QMap<int, QString> data, int currentId);
    QString selectedProfileName();

public slots:
    bool selectProfile(int id);

signals:
    void profileSelected(int id);

private slots:
    void buttonToggled(bool checked);

private:
    void addButton(QString name, int id, bool checked);
    void setLayout();

private:
    DuiButtonGroup *m_Buttons;

};

#endif // PROFILEBUTTONS_H
