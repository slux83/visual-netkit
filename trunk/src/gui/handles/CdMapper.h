/**
 * VisualNetkit is an advanced graphical tool for NetKit <http://www.netkit.org>
 * Copyright (C) 2008  Alessio Di Fazio, Paolo Minasi
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CDMAPPER_H_
#define CDMAPPER_H_

#include <QObject>
#include <QPointF>
#include <QMap>
#include "../AddCdForm.h"
#include "../CollisionDomainItem.h"
#include "../../core/CollisionDomain.h"

/**
 * This class is the singleton controller for the collision domain
 * It's also map the graphical (view) item with the domain object (model)
 */
class CdMapper : public QObject
{
	Q_OBJECT
	
private:
	static CdMapper *instance;
	AddCdForm *addCdForm;
	
	/* the pair: <VIEW, DOMAIN> */
	QMap<CollisionDomainItem*, CollisionDomain*> mappings;

public:
	CdMapper();
	virtual ~CdMapper();
	static CdMapper* getInstance();
	void showAddCdForm(QPointF pos);
	void addNewMapping(CollisionDomainItem *cdItem, CollisionDomain *cd);
	CollisionDomain* getCD(CollisionDomainItem *cdItem) { return mappings.value(cdItem); };
	QList<CollisionDomain*> getCDs() { return mappings.values(); };
	CollisionDomainItem* getCdItem(CollisionDomain *cd) { return mappings.key(cd); };
	QList<CollisionDomainItem*> getCdItems() { return mappings.keys(); };
	void updateCdCounter(CollisionDomain *cd, bool increase);
	void ensureAllItemsAreJoined();

public slots:
	void changeGraphicsLabelCd(CollisionDomain *cd, QString pluginName, QString label);
	void clear();

};

#endif /*CDMAPPER_H_*/
