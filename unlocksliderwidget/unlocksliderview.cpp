#include "unlocksliderview.h"
#include "unlockslider.h"

#include <QDebug>

UnlockSliderView::UnlockSliderView(UnlockSlider *controller) :
    DuiWidgetView(controller)
{
}

UnlockSliderView::~UnlockSliderView()
{
}

void UnlockSliderView::modelModified(const QList<const char*>& modifications)
{
    DuiWidgetView::modelModified(modifications);
}

void UnlockSliderView::modelChanged()
{
}

QSizeF UnlockSliderView::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    return QSizeF(100,100);
}

void UnlockSliderView::drawContents (QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    QRectF gay = geometry();
    qDebug() << "gaymetrix:" << gay.top() << gay.left() << gay.width() << gay.height() << gay.bottom() << gay.right();

    painter->drawText(0, 20, "pieru");
}

void UnlockSliderView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void UnlockSliderView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

