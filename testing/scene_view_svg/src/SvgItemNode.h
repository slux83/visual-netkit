#ifndef SVGITEMNODE_H
#define SVGITEMNODE_H

#include <QGraphicsSvgItem>
#include <QLinkedList>

#include "SvgItemLink.h"

class SvgItemNode : public QGraphicsSvgItem
{
	Q_OBJECT

private:
	QLinkedList<QGraphicsItem *> links;
	
public:
	SvgItemNode();
	void addSvgItemLink(SvgItemLink *l);
};
#endif
