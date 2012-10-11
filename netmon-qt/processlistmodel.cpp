#include "processlistmodel.hpp"
#include "netmon-types.hpp"
#include "netmon-hosts.hpp"

#include <QBrush>

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
	auto host = hostList.begin();
	while( (host != hostList.end()) && (!host->second.alive) ) ++host;
	for( const auto& head : host->second.HeaderProcessList ){
		headNames.push_back( head );
	}

	column = { "Host", "CMD", "UID", "STIME", "TIME", "RSS", "C", "S"};
	header = { "Host", "Command", "User", "Started", "CPU time", "RSS (kB)", "CPU usage", "S" };
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

	// qDebug("processlistmodel::index row=%d, col=%d", row, col );
	if( (col < 0) || row < 0 )
		return QModelIndex();
	if( static_cast<size_t>(col) >= headNames.size() )
		return QModelIndex();

	while( base + hostList[hostIndex[machineId]].ProcessList.size() < static_cast<size_t>(row) )
		base += hostList[hostIndex[machineId++]].ProcessList.size();

	int processid = row - base;
	if( (static_cast<size_t>(machineId) < hostList.size()) && 
	    (static_cast<size_t>(processid) < hostList[hostIndex[machineId]].ProcessList.size()) ){
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
	if(parent.isValid())
		return 0;

	int rows = 0;
	for( const auto host : hostList )
		rows += host.second.ProcessList.size();

	return rows;
}

int NetmonProcessListModel::columnCount( const QModelIndex &parent ) const
{
	return column.size();
}

QVariant NetmonProcessListModel::data( const QModelIndex &index, int role ) const
{
	int col = index.column();
	if( static_cast<size_t>(col) >= headNames.size() ) return QVariant();

	// fetch the needed ProcessListItem
	std::string item = "No Info";
	const ProcessIndex id(index.internalId());
	const HostListItem & host = hostList[hostIndex.at(id.hostId)];
	if( index.column() == 0 ){
		item = host.hostname;
	} else {
		const ProcessListItem & process = host.ProcessList.at(id.processId);
		auto it = host.HeadToIndex.find( column[col] );
		if( it != host.HeadToIndex.cend() ) 
			item = process.items[ it->second ];
	}

	switch(role){
	case Qt::DisplayRole:
	case Qt::ToolTipRole:
		return QString(item.c_str());
	}
	return QVariant();
}

QVariant NetmonProcessListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if( orientation == Qt::Vertical )
		return QVariant();

	if( section < 0 || section > header.size() ){
		qDebug("processModel::headerData invalid section!");
		return QString("invalid section=%1") .arg(section);
	}

	switch(role){
	case Qt::DisplayRole:
		return QString(header[section].c_str());
	}

	return QVariant();
}
