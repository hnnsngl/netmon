#include "netmon-qt-window.hpp"

#include "hostlistmodel.hpp"
#include "hostlistproxy.hpp"
#include "processlistmodel.hpp"
#include "processlistproxy.hpp"

#include "netmon-lib.hpp"
#include "netmon-hosts.hpp"

#include <QObject>
#include <QAction>
#include <QWidget>
#include <QFrame>
#include <QGroupBox>
#include <QBoxLayout>
#include <QToolBar>
#include <QTableView>
#include <QTreeView>
#include <QMainWindow>
#include <QApplication>
#include <QKeyEvent>
#include <QSplitter>
#include <QDir>
#include <QItemSelectionModel>

#include <iostream>
#include <string>

extern HostList hostList;

void createHostList(const NetmonHosts& netmonHosts)
{
	for( const auto& group : netmonHosts.hostnames )
		for( const auto& host : group.second ){
			std::cerr << "# add to hostList: " << host << std::endl;
			hostList[host] = build_HostListItem(host, 9221);
		}
}

NetmonWindow::NetmonWindow()
	: netmonHosts(QDir::homePath().toStdString() + std::string("/.netmon-hosts")),
	  hostSelected(netmonHosts)
{
 // FIXME: move this stuff to the main program file main() function
	createHostList(netmonHosts);
	// refresh_HostList_blocking(9221);
	print_HostList(false);
	createModels();

	// main parts
	QSplitter *splitter = new QSplitter(Qt::Vertical, this);
	setCentralWidget(splitter);

	/* host list frame */
	QFrame *frame_hostlist = new QFrame(this);
	splitter->addWidget( frame_hostlist );
	QVBoxLayout *layout_hostlist = new QVBoxLayout(frame_hostlist);
	view_hostlist = new QTreeView(frame_hostlist);
	view_hostlist->setModel( proxy_hostlist );
	view_hostlist->expandAll();
	view_hostlist->setSortingEnabled(true);
	view_hostlist->sortByColumn(0, Qt::AscendingOrder);
	view_hostlist->setSelectionMode( QAbstractItemView::ExtendedSelection ); // could also use ::MultipleSeclection here
	layout_hostlist->addWidget(view_hostlist);
	connect(view_hostlist->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
	        this, SLOT(HostSelectionChanged(const QItemSelection&, const QItemSelection&)));

	/* processes frame */
	QFrame *frame_processes = new QFrame(this);
	splitter->addWidget( frame_processes );
	QVBoxLayout *layout_processes = new QVBoxLayout(frame_processes);
	view_processes = new QTreeView(frame_processes);
	view_processes->setModel( proxy_processes );
	view_processes->setSortingEnabled(true);
	view_processes->setSelectionMode( QAbstractItemView::NoSelection );
	layout_processes->addWidget(view_processes);

	// /** settings frame */
	// QFrame *frame_settings = new QFrame(this);
	// toolbox->addWidgetaddItem( frame_settings, tr("Settings") );
	// QVBoxLayout *layout_settings = new QVBoxLayout(frame_settings);
	// QPushButton* button = new QPushButton( "This Button does &Nothing", frame_settings );
	// layout_settings->addWidget(button);

	/** actions */
	main_exit = new QAction(tr("&Exit"), this);
  main_exit->setIcon(QIcon(":/images/application-exit.png"));
  main_exit->setShortcut(tr("Ctrl+Q"));
  main_exit->setStatusTip(tr("Exit"));
  connect(main_exit, SIGNAL(triggered()), qApp, SLOT(quit()));

  hosts_expand = new QAction(tr("Expand Host Groups"), this);
  hosts_expand->setIcon(QIcon(":/images/placeholder.png"));
  hosts_expand->setShortcut(tr("Ctrl++"));
  connect(hosts_expand, SIGNAL(triggered()), view_hostlist, SLOT(expandAll()));

  hosts_collapse = new QAction(tr("Collapse Host Groups"), this);
  hosts_collapse->setIcon(QIcon(":/images/placeholder.png"));
  hosts_collapse->setShortcut(tr("Ctrl+-"));
  connect(hosts_collapse, SIGNAL(triggered()), view_hostlist, SLOT(collapseAll()));

  filter_user = new QAction(tr("Own Processes"), this);
  filter_user->setIcon(QIcon(":/images/placeholder.png"));
  connect(filter_user, SIGNAL(triggered()), this, SLOT(filterUpdate()));

  filter_hosts_dead = new QAction(tr("Filter Dead Hosts"), this);
  filter_hosts_dead->setIcon(QIcon(":/images/host-offline.png"));
  filter_hosts_dead->setCheckable(true);
  connect(filter_hosts_dead, SIGNAL(toggled(bool)), proxy_hostlist, SLOT(toggleFilter(bool)));

	/** toolbar */
  mainToolBar = addToolBar(tr("&Netmon"));
  mainToolBar->addAction(main_exit);
  mainToolBar->addAction(hosts_expand);
  mainToolBar->addAction(hosts_collapse);
  mainToolBar->addAction(filter_user);
  mainToolBar->addAction(filter_hosts_dead);
  // filterToolbar = addToolBar(tr("&Filter"));
  // filterToolbar->addAction(filter_own_processes);
}

void NetmonWindow::filterUpdate()
{}

void NetmonWindow::HostSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
	auto toggleSelection = [&](const QModelIndex&item, bool enable)
		{
			if( item.isValid() && item.column()==0 ){
				std::string itemname = item.data().toString().toStdString();
				if( item.parent().isValid() )	hostSelected.selectHost(itemname, enable);
				else hostSelected.selectGroup(itemname, enable);
			}
		};

	qDebug("HostSelectionsChanged:");
	for( const auto & index : selected.indexes() ){
		if(index.column()==0){
			toggleSelection( index, true );
		}
	}
	for( const auto & index : deselected.indexes() ){
		if(index.column()==0){
			toggleSelection( index, false );
		}
	}

	hostSelected.print(std::cerr);

	proxy_processes->invalidate();
}

NetmonWindow::~NetmonWindow()
{}

void NetmonWindow::createModels()
{
	model_hostlist = new NetmonHostlistModel(netmonHosts, this);
	proxy_hostlist = new NetmonHostlistProxy(netmonHosts, this);
	proxy_hostlist->setSourceModel(model_hostlist);

	model_processes = new NetmonProcessListModel(this);
	proxy_processes = new NetmonProcessListProxy(hostSelected, this);
	proxy_processes->setSourceModel(model_processes);
}

void NetmonWindow::createToolbar()
{
}
