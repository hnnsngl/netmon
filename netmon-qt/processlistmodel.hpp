#pragma once

#include <QObject>
#include <QVariant>
#include <QModelIndex>
#include <QAbstractItemModel>

class NetmonProcessListModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
	NetmonProcessListModel( QObject *parent );
	~NetmonProcessListModel();

	// AbstractItemModel Interface (required methods)	
	QModelIndex index( int row, int col, const QModelIndex& parent = QModelIndex() ) const;
	QModelIndex parent( const QModelIndex& child ) const;
	int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:

};
