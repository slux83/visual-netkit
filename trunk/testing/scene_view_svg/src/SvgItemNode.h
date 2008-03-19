#ifndef SVGITEMNODE_H
#define SVGITEMNODE_H

#include <QGraphicsSvgItem>
#include <QLinkedList>

#include "SvgItemLink.h"

class SvgItemNode : public QGraphicsSvgItem
{
	Q_OBJECT

public:
	SvgItemNode();
	void addSvgItemLink(SvgItemLink *l);
	QList<SvgItemLink *> getAllLinks();
    
private:
    QList<SvgItemLink *> links;
    
protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

};
#endif
