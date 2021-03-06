#include "netmon-types.hpp"
#include "netmon-hosts.hpp"
#include "hostlistproxy.hpp"

#include <QSortFilterProxyModel>
#include <string>

extern HostList hostList;

NetmonHostlistProxy::NetmonHostlistProxy(NetmonHosts &hosts, QObject *parent)
    : QSortFilterProxyModel(parent), netmonHosts(hosts), filterEnabled(false)
{
}

void NetmonHostlistProxy::toggleFilter(bool enable)
{
	filterEnabled = enable;
	invalidateFilter();
}

bool NetmonHostlistProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	QString lhs = sourceModel()->data(left).toString();
	QString rhs = sourceModel()->data(right).toString();

	switch (left.column()) {
	case 0: // hostname
	case 1: // CPU
		return QString::localeAwareCompare(lhs, rhs);
	case 2: // Memory
	case 3: // Load
	case 4: // Uptime
		double lval = lhs.left(lhs.indexOf(" ", 0)).toDouble();
		double rval = rhs.left(rhs.indexOf(" ", 0)).toDouble();
		return lval < rval;
	}

	qDebug("Cannot sort these host items!");
	return false;
}

bool NetmonHostlistProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	if (filterEnabled) {
		if (sourceParent.isValid()) {
			std::string group = sourceParent.data().toString().toStdString();
			std::string host = netmonHosts.hostnames[group][sourceRow];
			return hostList[host].alive;
		} else
			return true;
	}

	return true;
}
