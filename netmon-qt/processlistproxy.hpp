#pragma once

#include <QSortFilterProxyModel>

#include "netmon-types.hpp"
#include "netmon-hosts.hpp"

class HostSelections;

/**
 *  Proxy class for sorting and filtering of process list items
 *  delivered by NetmonProcessListModel. Implements basic sorting of
 *  process items and filtering of by a single username and a list of
 *  host selections.='
 */
class NetmonProcessListProxy : public QSortFilterProxyModel
{
	Q_OBJECT

	HostSelections &hostSelected;

	bool userFilter;
	int userFilterColumn;
	QString userFilterString;
	bool commandFilter;
	QString commandFilterString;

      public:
	NetmonProcessListProxy(HostSelections &selections, QObject *parent = 0);

      public slots:
	void toggleUserFilter(bool enable);
	void updateUserFilter(const QString &text);
	void toggleCommandFilter(bool enable);
	void updateCommandFilter(const QString &text);

      protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
	bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};
