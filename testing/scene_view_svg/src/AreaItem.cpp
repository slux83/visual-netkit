#include "AreaItem.h"
#include <QDebug>
static const int pointSize = 5;

AreaItem::AreaItem() : QGraphicsRectItem()
{
	setZValue(1);
	setRect(0,0,200,180);
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setPen(QPen(Qt::black, 2));
	setBrush(QColor(255, 253, 216));
}

AreaItem::~AreaItem()
{
}

void AreaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsRectItem::paint(painter, option, widget);
	
	if(isSelected())
	{
		painter->setPen(QPen(Qt::blue, 1));
		painter->setBrush(QColor(56, 202, 255, 180));
			
		painter->drawEllipse(QRectF(rect().width() - pointSize,
							rect().height() - pointSize,
							pointSize * 2, pointSize * 2));
	}
}

QRectF AreaItem::boundingRect() const
{
	QRectF bRect = QGraphicsRectItem::boundingRect();
	bRect.setHeight(bRect.height() + 5);
	bRect.setWidth(bRect.width() + 5);
	
	return bRect;
}
