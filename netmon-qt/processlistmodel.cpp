#include "processlistmodel.hpp"
#include "netmon-types.hpp"
#include "netmon-hosts.hpp"

#include <QDebug>

NetmonProcessListModel::NetmonProcessListModel( QObject *parent )
	: QAbstractItemModel(parent)
{}

NetmonProcessListModel::~NetmonProcessListModel()
{}

QModelIndex NetmonProcessListModel::index( int row, int col, const QModelIndex &parent ) const
{
	qDebug("NetmonProcessListModel::index row=%d col=%d", row, col);
	return QModelIndex();
}

QModelIndex NetmonProcessListModel::parent( const QModelIndex &child ) const
{
	qDebug("NetmonProcessListModel::parent");
	return QModelIndex();
}

int NetmonProcessListModel::rowCount( const QModelIndex &parent ) const
{
	qDebug("NetmonProcessListModel::rowCount");
	return 0;
}

int NetmonProcessListModel::columnCount( const QModelIndex &parent ) const
{
	qDebug("NetmonProcessListModel::columnCount");
	return 2;
}

QVariant NetmonProcessListModel::data( const QModelIndex &index, int role ) const
{
	qDebug("NetmonProcessListModel::data role=%d", role);
	return QVariant();
}

QVariant NetmonProcessListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	qDebug("NetmonProcessListModel::headerData section=%d, role=%d", section, role);
	return section;
}
