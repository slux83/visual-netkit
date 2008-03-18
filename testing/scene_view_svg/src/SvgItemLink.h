#ifndef SVGITEMLINK_H
#define SVGITEMLINK_H

#include <QGraphicsSvgItem>
#include <QRectF>

class SvgItemNode;

class SvgItemLink : public QGraphicsSvgItem
{
	Q_OBJECT	
	
public:
	 SvgItemLink(SvgItemNode *sourceNode, SvgItemNode *destNode);
	 ~SvgItemLink();
	 
	 SvgItemLink();
	 
	 SvgItemNode *sourceNode() const;
	 void setSourceNode(SvgItemNode *node);
	
	 SvgItemNode *destNode() const;
	 void setDestNode(SvgItemNode *node);
	
	 void adjust();
	
	 enum { Type = UserType + 2 };
	 int type() const { return Type; }

protected:
	 QGraphicsLineItem* draw();

private:
	 SvgItemNode *source, *dest;

     QPointF sourcePoint;
     QPointF destPoint;

};
#endif
