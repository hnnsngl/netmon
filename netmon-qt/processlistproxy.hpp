#include <QSortFilterProxyModel>

#include "netmon-types.hpp"
#include "netmon-hosts.hpp"

class NetmonProcessListProxy : public QSortFilterProxyModel
{
	Q_OBJECT

public:

	NetmonProcessListProxy( QObject *parent = 0 );

protected:
	bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const;
	bool lessThan( const QModelIndex &left, const QModelIndex &right ) const;
};
