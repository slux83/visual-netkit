#ifndef AREAITEM_H_
#define AREAITEM_H_

#include <QGraphicsRectItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class AreaItem : public QGraphicsRectItem
{
	
public:
	AreaItem();
	virtual ~AreaItem();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 ); 
	QRectF boundingRect() const;
 

	
};

#endif /*AREAITEM_H_*/
