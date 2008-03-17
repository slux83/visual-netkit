#ifndef SVGITEMLINK_H
#define SVGITEMLINK_H

#include <QGraphicsSvgItem>
#include <QGraphicsItem>

class SvgItemLink : public QGraphicsSvgItem
{
	Q_OBJECT	
	
public:
	 SvgItemLink(SVGItemNode *sourceNode, SVGItemNode *destNode);
	 ~SvgItemLink();
	 
	 SvgItemLink();
	 
	 SVGItemNode *sourceNode() const;
	 void setSourceNode(SVGItemNode *node);
	
	 SVGItemNode *destNode() const;
	 void setDestNode(SVGItemNode *node);
	
	 void adjust();
	
	 enum { Type = UserType + 2 };
	 int type() const { return Type; }

protected:
     QRectF boundingRect() const;
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	 SVGItemNode *source, *dest;

     QPointF sourcePoint;
     QPointF destPoint;

};
#endif
