#ifndef SVGITEMLINK_H
#define SVGITEMLINK_H

#include <QGraphicsSvgItem>
#include <QGraphicsItem>

class SvgItemNode;

class SvgItemLink : public QGraphicsItem
{
	Q_OBJECT	
	
public:
	 SvgItemLink(SvgItemNode *sourceNode, SvgItemNode *destNode);
	 ~SvgItemLink();
	 
	 SvgItemLink();
	 
	 SvgItemNode *sourceNode();
	 void setSourceNode(SvgItemNode *node);
	
	 SvgItemNode *destNode();
	 void setDestNode(SvgItemNode *node);
	
	 void adjust();
	
	 enum { Type = UserType + 2 };
	 int type() const { return Type; }

protected:
     QRectF boundingRect();
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	 SvgItemNode *source, *dest;

     QPointF sourcePoint;
     QPointF destPoint;

};
#endif
