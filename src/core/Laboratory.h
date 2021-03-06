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

#ifndef LABORATORY_H_
#define LABORATORY_H_

#include "VirtualMachine.h"

#include <QString>
#include <QList>
#include <QMap>
#include <QDateTime>
#include <QDir>
#include <QDebug>

/* Property key for lab */
enum PropertyKey
{
	Name = 0,
	Version = 1,
	Date = 2,
	Description = 3,
	Authors = 4,
	Email = 5,
	Website = 6
};

/**
 * The laboratory object
 */
class Laboratory : public QObject
{
	Q_OBJECT
	
private:
	bool isSaved;		//true if the lab exist on filesystem
	bool isChanged;		//true if a part of lab is changed by user
	QString description;
	QString version;
	QString date;
	QString authors;
	QString email;
	QString website;
	QMap<QString, VirtualMachine *> machines;
	QMap<QString, CollisionDomain *> collisionDomains;
	QDir labPath;
	
public:
	Laboratory(QString newVersion, QString newAuthors);
	Laboratory();
	virtual ~Laboratory();
	
	/* Getter functions */
	QString getDate() { return date; };
	QString getVersion() { return version; };
	QString getDescription() { return description; };
	QString getAuthors() { return authors; };
	QString getEmail() { return email; };
	QString getWebsite() { return website; };
	bool getSaveState() { return isSaved; };
	bool getChangedState() { return isChanged; };
	QDir getLabPath() { return labPath; };
	
	QMap<QString, VirtualMachine *> getMachines() { return machines; };
	QMap<QString, CollisionDomain *> getCollisionDomains() { return collisionDomains; };
	
	/* Setter functions */
	void setDate(QString newValue) { date = newValue; };
	void setVersion(QString newValue) { version = newValue; };
	void setDescription(QString newValue) { description = newValue; };
	void setAuthors(QString newValue) { authors = newValue; };
	void setEmail(QString newValue) { email = newValue; };
	void setWebsite(QString newValue) { website = newValue; };
	void setSavedState(bool saveState) { isSaved = saveState; };
	void setChangedState(bool changedState) { isChanged = changedState; };
	void setLabPath(QString path) { labPath.setPath(path); };
	
	/* Other functions */
	bool vmExist(QString vmName) { return machines.contains(vmName); };
	bool cdExist(QString cdName) { return collisionDomains.contains(cdName); };
	void addMachine(VirtualMachine *vm);
	void addCollisionDomain(CollisionDomain *cd);
	void updateCdKey(QString oldKey, QString newKey, CollisionDomain *cd);
	void updateVmKey(QString oldKey, QString newKey, VirtualMachine *vm);
	void removeCd(CollisionDomain *cd) { collisionDomains.remove(cd->getName()); };
	void removeVm(VirtualMachine *vm) { machines.remove(vm->getName()); };

};

#endif /*LABORATORY_H_*/
