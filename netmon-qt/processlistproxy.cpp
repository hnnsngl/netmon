#include "processlistproxy.hpp"
#include "netmon-selections.hpp"
#include "processlistmodel.hpp"

#include <string>
#include <QString>
#include <QVariant>
#include <cstdlib>
#include <iostream>
#include <algorithm>

NetmonProcessListProxy::NetmonProcessListProxy( HostSelections & selected, QObject *parent )
	: QSortFilterProxyModel(parent), hostSelected(selected),
	  userFilter( true ),
	  userFilterColumn( -1 ),
	  userFilterString( getenv("USER") )
{}

bool NetmonProcessListProxy::lessThan( const QModelIndex &left,
                                       const QModelIndex &right ) const
{
	QString lhs = left.data().toString();
	QString rhs = right.data().toString();
	return QString::localeAwareCompare(lhs, rhs);
}

void NetmonProcessListProxy::toggleUserFilter( bool enable )
{
	// // find processlist column for userid
	NetmonProcessListModel* model = static_cast<NetmonProcessListModel*>(sourceModel());
	auto it = std::find(model->column.cbegin(), model->column.cend(), "UID");
	// std::cerr << it* << std::endl;
	if( it != model->column.cend() )
		userFilterColumn = it - model->column.begin();
	else userFilterColumn = -1;
	qDebug("set up user filter: %d, %s", userFilterColumn, userFilterString.toStdString().c_str());

	if( userFilterColumn >= 0 ) userFilter = enable;
	invalidateFilter();
}

bool NetmonProcessListProxy::filterAcceptsRow( int sourceRow,
                                               const QModelIndex &sourceParent ) const
{
	bool acceptUser = true;
	if( userFilter ) {
		QString uid = sourceModel()->index( sourceRow, userFilterColumn, sourceParent).data().toString();
		acceptUser = (QString::compare( uid, userFilterString ) == 0);
	}

	// filter selected hosts
	NetmonProcessListModel *model = dynamic_cast<NetmonProcessListModel*>(sourceModel());
	int internalId = sourceModel()->index( sourceRow, 0, sourceParent ).internalId();
	std::string hostname = model->hostIndex[internalId >> 16];
	// std::cerr << "filtering " << hostname << " : " << hostSelected[hostname] << std::endl;
	bool acceptHost = hostSelected[hostname] || hostSelected.empty();

	// if( hostSelected[hostname] )
	// 	qDebug("serving row for %s: %d", hostname.c_str(), sourceRow );

	return acceptHost && acceptUser;
}
