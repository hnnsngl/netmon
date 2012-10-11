#include "processlistproxy.hpp"
#include "netmon-selections.hpp"
#include "processlistmodel.hpp"

#include <string>
#include <QString>
#include <QVariant>
#include <cstdlib>
#include <mutex>
#include <iostream>
#include <algorithm>

extern HostList hostList;
extern std::mutex mutexList;

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

void NetmonProcessListProxy::updateTextFilter(const QString &text)
{
	commandFilterString = text;
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

	NetmonProcessListModel *model = dynamic_cast<NetmonProcessListModel*>(sourceModel());
	int internalId = sourceModel()->index( sourceRow, 0, sourceParent ).internalId();

	// filter selected hosts
	std::string hostname = model->hostIndex[internalId >> 16];
	// std::cerr << "filtering " << hostname << " : " << hostSelected[hostname] << std::endl;

	ProcessIndex index(internalId);
	const HostListItem & host = hostList[hostname];
	bool acceptUserFilter = true;
	if( host.alive ){
		const ProcessListItem & process = host.ProcessList.at(index.processId);
		auto it = host.HeadToIndex.find( "CMD" );
		QString command = "";
		if( it != host.HeadToIndex.cend() )
			command = QString::fromStdString( process.items[it->second] );
		std::cerr << hostname << "\t" << command.toStdString() << std::endl;
		acceptUserFilter = commandFilterString.isEmpty() || command.contains(commandFilterString);
	}

	// user text filter
	bool acceptHost = hostSelected[hostname] || hostSelected.empty();

	// if( hostSelected[hostname] )78
	// 	qDebug("serving row for %s: %d", hostname.c_str(), sourceRow );

	return acceptHost && acceptUser && acceptUserFilter;
}











