#include "hostlistmodel.hpp"
#include "netmon-types.hpp"
#include "netmon-hosts.hpp"

#include <QtGui>
#include <QString>
#include <QDebug>

NetmonHostlistModel::NetmonHostlistModel( QObject *parent )
	: QAbstractItemModel(parent)
{}

NetmonHostlistModel::~NetmonHostlistModel()
{}

QModelIndex NetmonHostlistModel::index( int row, int col, const QModelIndex &parent ) const
{
	qDebug("NetmonHostlistModel::index row=%d col=%d", row, col);
	if( parent == QModelIndex() ){
		return createIndex( row, col );
	}
	return createIndex( row, col, parent.row() );
}

QModelIndex NetmonHostlistModel::parent( const QModelIndex &child ) const
{
	qDebug("NetmonHostlistModel::parent");

	if( child == index( child.row(), child.column(), QModelIndex() ) )
		return QModelIndex();

	else return  QModelIndex();
	// otherwise we need to determine the group of the current host and create the corresponding index
}

int NetmonHostlistModel::rowCount( const QModelIndex &parent ) const
{
	qDebug("NetmonHostlistModel::rowCount");
	// in the first level this is the number of host groups
	if( parent == QModelIndex() ){
		qDebug("  root level");
		return 3;
	}

	// in the second level, it is the number of hosts in that group
	if( parent.parent() == QModelIndex() ){
		qDebug("Host level");
		return 0;
	}

	// otherwise there are no entries
	return 0;
}

int NetmonHostlistModel::columnCount( const QModelIndex &parent ) const
{
	// qDebug("NetmonHostlistModel::columnCount");
	return 5;
}

QVariant NetmonHostlistModel::data( const QModelIndex &index, int role ) const
{
	qDebug("NetmonHostlistModel::data role=%d", role);

	if( !index.isValid() ) 
		return QVariant();

	switch(role){
	case Qt::DisplayRole:
		// The key data to be rendered in the form of text. (QString)
		if( index.parent() == QModelIndex() ){
			if( index.column() == 0 )
				return QString("Hostgroup item");
		}
		else {
			return QString("Row%1, Column%2") .arg(index.row() + 1) .arg(index.column() + 1);
		}
		break;

	case Qt::ToolTipRole:
		// The data displayed in the item's tooltip. (QString)
		//
		// use for cpuinfo and meminfo
		break;

	case Qt::DecorationRole:
		// The data to be rendered as a decoration in the form of an icon. (QColor, QIcon or QPixmap)
		//
		// use for icons ?
		break;

	case Qt::BackgroundRole:
		// The background brush used for items rendered with the default delegate. (QBrush)
		//
		// use to color the background of the load field, or possibly the entire host item
		break;
	}		
	return QVariant();
}

QVariant NetmonHostlistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// qDebug("NetmonHostlistModel::headerData section=%d, orientation=%d, role=%d",
	//        section, orientation, role);

	switch( role ){
	case Qt::DisplayRole:
		if( orientation == Qt::Horizontal ){
			switch( section ){
			case 0:
				return QString("Host");
			case 1:
				return QString("CPU");
			case 2:
				return QString("Memory");
			case 3:
				return QString("Load");
			case 4:
				return QString("Uptime");
			}
		}
		return QString("%1") .arg(section);	
	// case Qt::SizeHintRole:
	// 	qDebug("return size hint");
	// 	return QVariant(60);
	}

	return QVariant();
}
