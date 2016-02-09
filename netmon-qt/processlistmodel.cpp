#include "processlistmodel.hpp"
#include "netmon-types.hpp"
#include "netmon-hosts.hpp"

#include <QBrush>

#include <QDebug>
#include <iostream>

extern HostList hostList;
extern std::mutex mutexList;

NetmonProcessListModel::NetmonProcessListModel(QObject *parent) : QAbstractItemModel(parent)
{
	// build the host list
	std::lock_guard<std::mutex> lock_hostList(mutexList);
	for (const auto &host : hostList) {
		hostIndex.push_back(host.first);
	}

	// setup default column headers and translations
	column = {"Host", "PID", "CMD", "UID", "STIME", "TIME", "RSS", "C", "S"};
	header = {"Host", "process", "Command", "User", "Started", "CPU time", "RSS (kB)", "CPU usage", "S"};
}

NetmonProcessListModel::~NetmonProcessListModel() {}

QModelIndex NetmonProcessListModel::index(int row, int col, const QModelIndex &parent) const
{
	size_t machineId = 0;
	size_t base = 0;

	// qDebug("processlistmodel::index row=%d, col=%d", row, col );
	if ((col < 0) || row < 0)
		return QModelIndex();
	if (static_cast<size_t>(col) >= header.size())
		return QModelIndex();

	std::lock_guard<std::mutex> lock_hostList(mutexList);

	while (base + hostList[hostIndex[machineId]].ProcessList.size() < static_cast<size_t>(row))
		base += hostList[hostIndex[machineId++]].ProcessList.size();

	int processId = row - base - 1;
	// TODO: fix stupid bug fixing

	if ((static_cast<size_t>(machineId) < hostList.size()) &&
	    (static_cast<size_t>(processId) < hostList[hostIndex[machineId]].ProcessList.size())) {
		ProcessIndex id(machineId, processId);
		// if( id == 0 )
		// 	std::cerr << "creating invalid index: " << row << "\t" << col << std::endl;

		return createIndex(row, col, id);
	} else {
		// std::cerr << "return invalid model index: machineId=" << machineId << "<" << hostList.size()
		//           << "processId=" << processId << "<" << hostList[hostIndex[machineId]].ProcessList.size()
		//           << std::endl;std:
		return QModelIndex();
	}

	// std::cerr << "return invalid model index: can this even be reached?" << std::endl;
	return QModelIndex();
}

QModelIndex NetmonProcessListModel::parent(const QModelIndex &child) const
{
	// at the moment this is just a flat process list; only in principle
	// multiple threads could be expanded/collapsed to a tree structure
	return QModelIndex();
}

int NetmonProcessListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	std::lock_guard<std::mutex> lock_hostList(mutexList);
	int rows = 0;
	for (const auto host : hostList)
		rows += host.second.ProcessList.size();

	return rows;
}

int NetmonProcessListModel::columnCount(const QModelIndex &parent) const
{
	// qDebug("process list columns: %d", column.size());
	return column.size();
}

QVariant NetmonProcessListModel::data(const QModelIndex &index, int role) const
{
	int col = index.column();
	if (static_cast<size_t>(col) >= header.size())
		return QVariant();

	std::lock_guard<std::mutex> lock_hostList(mutexList);

	// fetch the needed ProcessListItem
	std::string item = "No Info";
	const ProcessIndex id(index.internalId());
	const HostListItem &host = hostList[hostIndex.at(id.hostId)];
	if (index.column() == 0) {
		item = host.hostname;
	} else {
		const ProcessListItem &process = host.ProcessList.at(id.processId);
		auto it = host.HeadToIndex.find(column[col]);
		if (it != host.HeadToIndex.cend())
			item = process.items[it->second];
	}

	switch (role) {
	case Qt::DisplayRole:
	case Qt::ToolTipRole:
		return QString(item.c_str());
	}
	return QVariant();
}

QVariant NetmonProcessListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical)
		return QVariant();

	switch (role) {
	case Qt::DisplayRole:
		return QString(header[section].c_str());
	}

	return QVariant();
}
