#pragma once

#include "netmon-hosts.hpp"

#include <QObject>
#include <QVariant>
#include <QModelIndex>
#include <QAbstractItemModel>

class NetmonHostlistModel : public QAbstractItemModel
{
	Q_OBJECT

	NetmonHosts & netmonHosts;

public:
	NetmonHostlistModel( NetmonHosts & _netmonHosts, QObject *parent );
	virtual ~NetmonHostlistModel();

	// AbstractItemModel Interface (required methods)	
	QModelIndex index( int row, int col, const QModelIndex& parent = QModelIndex() ) const;
	QModelIndex parent( const QModelIndex& child ) const;
	int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QVariant headerData(int section, Qt::Orientation, int role) const;
};
