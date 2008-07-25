#include "AreaItem.h"
#include <QDebug>
static const int pointSize = 5;

AreaItem::AreaItem() : QGraphicsRectItem()
{
	setZValue(1);
	setRect(0,0,200,180);
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

AreaItem::~AreaItem()
{
}

void AreaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	/* Draw the area */
	
	painter->save();
	painter->setOpacity(100);
	painter->setPen(QPen(Qt::black, 1));
	(isSelected())? painter->setBrush(QColor(255, 253, 216, 100)) : painter->setBrush(QColor(255, 253, 216));
	painter->drawRect(rect());
	painter->restore();
	
	/* draw some text */
	painter->save();
	QString text("Hello world!");
	painter->drawText(5, 5, rect().width() - 5, rect().height() - 5,
			Qt::TextWordWrap, text);
	painter->restore();
	
	/* 
	 * If the object is selected, draw the control ball (to resize)
	 * and the bounding dotted
	 */
	if(isSelected())
	{
		painter->save();
		painter->setPen(QPen(Qt::blue, 1));
		painter->setBrush(QColor(56, 202, 255, 180));
		
		painter->drawEllipse(QRectF(rect().width() - pointSize,
							rect().height() - pointSize,
						pointSize * 2, pointSize * 2));
		painter->restore();
	}
}

QRectF AreaItem::boundingRect() const
{
	QRectF bRect = QGraphicsRectItem::boundingRect();
	bRect.setHeight(bRect.height() + 5);
	bRect.setWidth(bRect.width() + 5);
	
	return bRect;
}

QPainterPath AreaItem::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());
	return path;
}
