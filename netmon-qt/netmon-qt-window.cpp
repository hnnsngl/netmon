#include "netmon-qt-window.hpp"
#include "hostlistmodel.hpp"
#include "processlistmodel.hpp"
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
	: netmonHosts(QDir::homePath().toStdString() + std::string("/.netmon-hosts"))
{
	createHostList(netmonHosts);
	// refresh_HostList_blocking(9221);
	print_HostList();
	createModels();

	// main parts
	QSplitter *splitter = new QSplitter(Qt::Vertical, this);
	setCentralWidget(splitter);

	/* host list frame */
	QFrame *frame_hostlist = new QFrame(this);
	splitter->addWidget( frame_hostlist );
	QVBoxLayout *layout_hostlist = new QVBoxLayout(frame_hostlist);
	view_hostlist = new QTreeView(frame_hostlist);
	view_hostlist->setModel( model_hostlist );
	layout_hostlist->addWidget(view_hostlist);

	/* processes frame */
	QFrame *frame_processes = new QFrame(this);
	splitter->addWidget( frame_processes );
	QVBoxLayout *layout_processes = new QVBoxLayout(frame_processes);
	view_processes = new QTableView(frame_processes);
	view_processes->setModel( model_processes );
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
  filter_user->setShortcut(tr("Ctrl+U"));
  connect(filter_user, SIGNAL(triggered()), this, SLOT(filterUpdate()));

	/** toolbar */
  mainToolBar = addToolBar(tr("&Netmon"));
  mainToolBar->addAction(main_exit);
  mainToolBar->addAction(hosts_expand);
  mainToolBar->addAction(hosts_collapse);
  mainToolBar->addAction(filter_user);
  // filterToolbar = addToolBar(tr("&Filter"));
  // filterToolbar->addAction(filter_own_processes);
}

void NetmonWindow::filterUpdate()
{}

NetmonWindow::~NetmonWindow()
{}

void NetmonWindow::createModels()
{
	model_hostlist = new NetmonHostlistModel(netmonHosts, this); // FIXME
	model_processes = new NetmonProcessListModel(this);
}

void NetmonWindow::createToolbar()
{
}
