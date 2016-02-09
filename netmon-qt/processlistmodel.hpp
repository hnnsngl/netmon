#pragma once

#include "netmon-types.hpp"

#include <vector>
#include <string>
#include <cassert>

#include <QObject>
#include <QVariant>
#include <QModelIndex>
#include <QAbstractItemModel>

// use the internalId as follows:
// internalId && 0xffff0000 identifies host
// internalId && 0x0000ffff identifies process
// upper 32 bits are reserved for now (possibly: flags)

struct ProcessIndex
{
	const int hostId;
	const int processId;

	ProcessIndex(int host, int process) : hostId(host), processId(process)
	{
		assert(host < 0xffff);
		assert(process < 0xffff);
	}

	ProcessIndex(qint64 internalId) : hostId((internalId >> 16) & 0xffff), processId(internalId & 0xffff) {}

	operator int() { return (hostId << 16) | processId; }
};

class NetmonProcessListModel : public QAbstractItemModel
{
	Q_OBJECT

      public:
	NetmonProcessListModel(QObject *parent);
	~NetmonProcessListModel();

	std::vector<std::string> column;
	std::vector<std::string> header;

	// AbstractItemModel Interface (required methods)
	QModelIndex index(int row, int col, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &child) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	std::vector<std::string> hostIndex;
};
