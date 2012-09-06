#include "processlistmodel.hpp"
#include "netmon-types.hpp"
#include "netmon-hosts.hpp"

#include <cassert>
#include <QDebug>
#include <iostream>

extern HostList   hostList;
extern std::mutex mutexList;

NetmonProcessListModel::NetmonProcessListModel( QObject *parent )
	: QAbstractItemModel(parent)
{
	// build the host list
	for( const auto& host : hostList )
		hostIndex.push_back(host.first);
	// build the header name list
	for( const auto& host : hostList ){
		std::cout << host.first << ": " << host.second.HeaderProcessList.size() << std::endl;
		for( const auto& head : host.second.HeaderProcessList ){
			std::cout << "# add process list header " << head << std::endl;
			headNames.push_back( head );
		}
	}
	std::cout << headNames.size();
}

NetmonProcessListModel::~NetmonProcessListModel()
{}

// use the internalId as follows:
// internalId && 0xffff0000 identifies host
// internalId && 0x0000ffff identifies process
// upper 32 bits are reserved for now (possibly: flags)

struct ProcessIndex {
	const int hostId;
	const int processId;

	ProcessIndex( int host, int process )
		: hostId( host ), processId( process )
	{
		assert( host < 0xffff );
		assert( process < 0xffff );
	}

	ProcessIndex( qint64 internalId )
		: hostId( (internalId >> 16) & 0xffff ), processId( internalId & 0xffff )
	{}

	operator int () { return (hostId<<16) | processId ; }
};

QModelIndex NetmonProcessListModel::index( int row, int col, const QModelIndex &parent ) const
{
	size_t machineId = 0;
	size_t base = 0;

	if( col >= headNames.size() )
		return QModelIndex();

	while( base + hostList[hostIndex[machineId]].ProcessList.size() < static_cast<size_t>(row) )
		base += hostList[hostIndex[machineId++]].ProcessList.size();

	int processid = row - base;
	if( (machineId < hostList.size()) && 
	    (processid < hostList[hostIndex[machineId]].ProcessList.size()) ){
		ProcessIndex id(machineId, processid);

		return createIndex( row, col, id );
	}
	else return QModelIndex();

	return QModelIndex();
}

QModelIndex NetmonProcessListModel::parent( const QModelIndex &child ) const
{
	// at the moment this is just a flat process list; only in principle
	// multiple threads could be expanded/collapsed to a tree structure
	return QModelIndex();
}

int NetmonProcessListModel::rowCount( const QModelIndex &parent ) const
{
	int rows = 0;
	for( const auto host : hostList )
		rows += host.second.ProcessList.size();

	return rows;
}

int NetmonProcessListModel::columnCount( const QModelIndex &parent ) const
{
	return headNames.size();
}

QVariant NetmonProcessListModel::data( const QModelIndex &index, int role ) const
{
	// qDebug("NetmonProcessListModel::data role=%d, row=%d, col=%d", role, index.row(), index.column());

	// fetch the needed ProcessListItem
	const ProcessIndex id(index.internalId());

	const HostListItem & host = hostList[hostIndex[id.hostId]];
	const ProcessListItem & process = host.ProcessList[id.processId];

	int column = index.column();
	if( column >= headNames.size() ) return QVariant();

	switch(role){
	case Qt::DisplayRole:
		return QString(process.items[column].c_str());
	}

	return QVariant();
}

QVariant NetmonProcessListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// qDebug("NetmonProcessListModel::headerData section=%d, role=%d, orientation=%d",
	//        section, role, orientation);

	if( orientation == Qt::Vertical )
		return QVariant();

	switch(role){
	case Qt::DisplayRole:
		return QString(headNames[section].c_str());
	}

	return QVariant();
}
