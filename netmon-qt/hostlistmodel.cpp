#include "hostlistmodel.hpp"
#include "netmon-types.hpp"
#include "netmon-hosts.hpp"

#include <QtGui>
#include <QString>
#include <QDebug>
#include <QPixmap>

#include <mutex>

NetmonHostlistModel::NetmonHostlistModel( NetmonHosts & _netmonHosts, QObject *parent )
	: QAbstractItemModel(parent),
	  netmonHosts(_netmonHosts)
{}

NetmonHostlistModel::~NetmonHostlistModel()
{}

extern HostList hostList;
extern std::mutex mutexList;

// usage of internalId: positive hostitems refer to hostgroups, hostgroups refer to -1

QModelIndex NetmonHostlistModel::index( int row, int col, const QModelIndex &parent ) const
{
	if( ! parent.isValid() ){
		return createIndex( row, col, -1 );
	}

	if( parent.internalId() < 0 ){
		return createIndex( row, col, parent.row() );
	}
	qDebug("returning invalid index!");
	return QModelIndex();
}

QModelIndex NetmonHostlistModel::parent( const QModelIndex &child ) const
{
	if( !child.isValid() )
		return QModelIndex();

	// check if child points to a hostgroup item
	if( child.internalId() < 0 )
		return QModelIndex();

	return createIndex( child.internalId(), 0, -1 );
}

int NetmonHostlistModel::rowCount( const QModelIndex &parent ) const
{
	// root hast hostgroups as children
	if( ! parent.isValid() )
		return netmonHosts.hostGroups.size();

	// hostgroups have hosts as children
	if( parent.internalId() == -1 ){
		std::string group = netmonHosts.hostGroups[ parent.row() ];
		// qDebug("::rowCount (Host level) (%s) --> %d", group.c_str(), netmonHosts.hostnames[group].size() );
		return netmonHosts.hostnames[group].size();
	}

	// hosts have no children
	return 0;
}

int NetmonHostlistModel::columnCount( const QModelIndex &parent ) const
{
	// qDebug("NetmonHostlistModel::columnCount");
	return 5;
}

QVariant NetmonHostlistModel::data( const QModelIndex &index, int role ) const
{
	// handle invalid data requestsn
	if( !index.isValid() ) 
		return QVariant();

	// handle hostgroup data requests
	if( index.internalId() == -1 )
		switch(role){
		case Qt::DisplayRole: // key data to be rendered in the form of text. (QString)
			if( index.column() == 0 ) return QString(netmonHosts.hostGroups.at(index.row() ).c_str());
		default:
			return QVariant();
		}

	else {
		std::lock_guard<std::mutex> lock_hostList(mutexList);

		const std::string group = netmonHosts.hostGroups[index.internalId()];
		const std::string hostname = netmonHosts.hostnames[group][index.row()];
		const HostListItem & hostitem = hostList[netmonHosts.hostnames[group][index.row()]];
		switch(role){
		case Qt::DisplayRole:
			switch(index.column()){
			case 0: return QString(hostname.c_str());
			case 1: return QString("%2 (%1 cores)") .arg(hostitem.processors) .arg(hostitem.cpuname.c_str());
			case 2: return QString(hostitem.memory.c_str());
			case 3: return QString(hostitem.load.c_str());
			case 4: return QString(hostitem.uptime.c_str());
			default:
				return QString("item: Row%1, Column%2") .arg(index.row() + 1) .arg(index.column() + 1);
			}
			return QVariant();

		case Qt::ToolTipRole:
			switch( index.column() ){
			case 1: return QString::fromStdString(hostitem.tooltip_cpu);
			case 2: return QString::fromStdString(hostitem.meminfo);
			case 3: return QString::fromStdString(hostitem.tooltip_load);
			}
			return QVariant();

		case Qt::BackgroundRole:
			switch( index.column() ){
			case 3:
				if( ! hostitem.alive ) return QVariant();
				int hue = 120 - 60*hostitem.avgload1/std::max(1,hostitem.processors);
				QColor color = QColor::fromHsv(std::max(0, std::min(359, static_cast<int>(hue))) , 255, 255 );
				return QBrush(color);
			}
			return QVariant();

		case Qt::DecorationRole:
			switch(index.column()){
			case 0:
				if( hostitem.alive )
					return QVariant(QPixmap(":/images/host-ok.png").scaledToHeight(12));
				else return QVariant(QPixmap(":/images/host-down.png").scaledToHeight(12));
			case 3:
				if( (hostitem.avgload1 > hostitem.avgload2) && (hostitem.avgload2 > hostitem.avgload3) )
					return QPixmap(":/images/arrow-up.png").scaledToHeight(12);
				if( (hostitem.avgload1 < hostitem.avgload2) && (hostitem.avgload2 < hostitem.avgload3) )
					return QPixmap(":/images/arrow-down.png").scaledToHeight(12);
				return QPixmap(":/images/placeholder.png").scaledToHeight(12);
			}

		default:
			return QVariant();
		}
	}

	return QVariant();
}

QVariant NetmonHostlistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	switch( role ){
	case Qt::DisplayRole:
		if( orientation == Qt::Horizontal ){
			switch( section ){
			case 0:
				return QString(tr("Host"));
			case 1:
				return QString(tr("CPU"));
			case 2:
				return QString(tr("Memory"));
			case 3:
				return QString(tr("Load"));
			case 4:
				return QString(tr("Uptime"));
			}
		}
		return QString("%1") .arg(section);	
	case Qt::SizeHintRole:
		if(section == 0) return QSize(0,0);
	}
	return QVariant();
}
