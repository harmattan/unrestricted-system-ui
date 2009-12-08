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

    void update(bool enabled, QString number = QString(""));
    bool isEnabled();

signals:
    void forwardingDisabled(QString interface);
    void enabled(bool enabled);

    // Should we skip CallAndSim logic class completely and keep the logic here
    //void buttonClicked();

private slots:
    void checked(bool check);

private:
    void showButton(bool show);

private:
    QString dbusFunction;
    DuiButton* checkButton;
    QGraphicsWidget* buttonWidget;
    QGraphicsWidget* dummyWidget; // for hiding the number button
    DuiButton* numberButton;
    DuiGridLayoutPolicy* lp;
    DuiGridLayoutPolicy* pp;
};

#endif // FORWARDINGWIDGET_H
