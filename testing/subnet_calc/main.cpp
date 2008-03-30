#include <QtCore>
#include <QCoreApplication>
#include <QHostAddress>
#include <QDebug>

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
    
	
	/**
	 * CIDR-NETMASK to IP-NETMASK
	 */
	quint8 cidrMask = 12;
	
	QHostAddress base(QHostAddress::Broadcast);
	
	QHostAddress conv((base.toIPv4Address() << (32 - cidrMask)));
	
	qDebug() << "/" + QByteArray::number(cidrMask) + " = " + conv.toString();
	
	/**************************************************************************/

	/**
	 * IP-NETMASK to CIDR-NETMASK
	 */
	
	QHostAddress netmask("255.255.255.0");
	quint32 cidrNotationNetmask = 0;
	
	QStringList l = netmask.toString().split(".");
	
	int div, test;
	for(int i=0; i<4; i++)
	{
		div = 256;
		while(div > 1)
		{
			div = div/2;
			test = l[i].toInt() - div;
			if(test >= 0)
			{
				cidrNotationNetmask++;
				l[i] = QString::number(test);
				qDebug() << l << cidrNotationNetmask << div;
			}
			else
				break;
			
		}
		
	}
	
	qDebug() << netmask << "=" << cidrNotationNetmask;
	
	return 0;
}
