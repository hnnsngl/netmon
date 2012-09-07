#pragma once

#include <QModelIndex>
#include <QSortFilterProxyModel>

#include "netmon-hosts.hpp"

class NetmonHostlistProxy : public QSortFilterProxyModel
{
	Q_OBJECT

	NetmonHosts &netmonHosts;
	bool filterDisabled;

public:
	NetmonHostlistProxy(NetmonHosts& netmonHosts, QObject *parent = 0);

protected:
	bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const;
	bool lessThan( const QModelIndex &lhs, const QModelIndex &rhs ) const;
};
