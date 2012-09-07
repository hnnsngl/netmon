#include "processlistproxy.hpp"

NetmonProcessListProxy::NetmonProcessListProxy( QObject *parent )
	: QSortFilterProxyModel(parent)
{}

bool NetmonProcessListProxy::lessThan( const QModelIndex &left,
                                      const QModelIndex &right ) const
{
	QString lhs = left.data().toString();
	QString rhs = right.data().toString();
}

bool NetmonProcessListProxy::filterAcceptsRow( int sourceRow,
                                               const QModelIndex &sourceParent ) const
{
	return true;
}
