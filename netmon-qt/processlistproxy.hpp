#pragma once

#include <QSortFilterProxyModel>

#include "netmon-types.hpp"
#include "netmon-hosts.hpp"

class HostSelections;

class NetmonProcessListProxy : public QSortFilterProxyModel
{
	Q_OBJECT

	HostSelections & hostSelected;

	bool userFilter;
	int userFilterColumn;
	QString userFilterString;

public:

	NetmonProcessListProxy( HostSelections & selections, QObject *parent = 0 );

public slots:
	void toggleUserFilter( bool enable );

protected:
	bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const;
	bool lessThan( const QModelIndex &left, const QModelIndex &right ) const;
};
