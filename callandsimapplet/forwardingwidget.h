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
    ForwardingWidget(const QString& label, QGraphicsItem* parent = 0);
    ~ForwardingWidget();

    void update(bool enabled, QString number = QString(""));
    bool isEnabled();

signals:
    void enabled(bool enabled);
    void buttonClicked();

private slots:
    void toggled(bool checked);

private:
    void showButton(bool show);

private:
    DuiButton* checkButton;
    QGraphicsWidget* buttonWidget;
    QGraphicsWidget* dummyWidget; // for hiding the number button
    DuiButton* numberButton;
    DuiGridLayoutPolicy* lp;
    DuiGridLayoutPolicy* pp;
};

#endif // FORWARDINGWIDGET_H
