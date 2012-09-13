#include "processlistproxy.hpp"
#include "netmon-selections.hpp"
#include "processlistmodel.hpp"

#include <string>
#include <QString>
#include <QVariant>

NetmonProcessListProxy::NetmonProcessListProxy( HostSelections & selected, QObject *parent )
	: QSortFilterProxyModel(parent), hostSelected(selected)
{}

bool NetmonProcessListProxy::lessThan( const QModelIndex &left,
                                       const QModelIndex &right ) const
{
	QString lhs = left.data().toString();
	QString rhs = right.data().toString();
	return QString::localeAwareCompare(lhs, rhs);
}

bool NetmonProcessListProxy::filterAcceptsRow( int sourceRow,
                                               const QModelIndex &sourceParent ) const
{
	NetmonProcessListModel *model = dynamic_cast<NetmonProcessListModel*>(sourceModel());
	int internalId = sourceModel()->index( sourceRow, 0, sourceParent ).internalId();
	std::string hostname = model->hostIndex[internalId >> 16];
	// std::cerr << "filtering " << hostname << " : " << hostSelected[hostname] << std::endl;
	return hostSelected[hostname];
}
