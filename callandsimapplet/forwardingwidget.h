#ifndef FORWARDINGWIDGET_H
#define FORWARDINGWIDGET_H

#include <QObject>
#include <QGraphicsWidget>

class DuiButton;
class DuiGridLayoutPolicy;

class ForwardingWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    ForwardingWidget(const QString& label, const QString& toggleFunction, QGraphicsItem* parent = 0);
    ~ForwardingWidget();

    void setButtonVisible(bool visible);

signals:
    void forwardingDisabled(QString interface);
    void buttonClicked();

private slots:
    void checked(bool check);

private:
    QString dbusFunction;
    DuiButton* checkButton;
    DuiButton* numberButton;
    QGraphicsWidget* dummyWidget; // helps to
    DuiGridLayoutPolicy* lp;
    DuiGridLayoutPolicy* pp;
};

#endif // FORWARDINGWIDGET_H
