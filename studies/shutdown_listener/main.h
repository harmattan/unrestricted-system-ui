#ifndef MAIN_H
#define MAIN_H

#include <QObject>

#include <qmsystem/qmsystemstate.h>

class TestObj : public QObject
{
    Q_OBJECT
public:
    TestObj();
    virtual ~TestObj();

public slots:
    void systemStateChanged(Maemo::QmSystemState::StateIndication);    

private:
    Maemo::QmSystemState *st;
};

#endif // MAIN_H
