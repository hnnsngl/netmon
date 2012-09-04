#include "netmon-qt-window.hpp"
#include "hostlistmodel.hpp"
#include "processlistmodel.hpp"

#include <QObject>
#include <QAction>
#include <QWidget>
#include <QFrame>
#include <QBoxLayout>
#include <QToolBox>
#include <QTableView>
#include <QTreeView>
#include <QMainWindow>
#include <QApplication>
#include <QKeyEvent>
#include <QPushButton>
#include <QToolBar>

#include <iostream>

NetmonWindow::NetmonWindow()
	: netmonHosts("~/.netmon-hosts")
{
	std::cout << "Host list:" << std::endl << netmonHosts << std::endl;

	createModels();

	// main parts -- use a toolbox for now, but this is easily changed
	toolbox = new QToolBox(this);
	setCentralWidget(toolbox);

	/* host list frame */
	QFrame *frame_hostlist = new QFrame(toolbox);
	toolbox->addItem( frame_hostlist, tr("Hosts") );
	QVBoxLayout *layout_hostlist = new QVBoxLayout(frame_hostlist);
	QTreeView *view_hostlist = new QTreeView(frame_hostlist);
	view_hostlist->setModel( model_hostlist );
	layout_hostlist->addWidget(view_hostlist);

	/* processes frame */
	QFrame *frame_processes = new QFrame(toolbox);
	toolbox->addItem( frame_processes, tr("Processes") );
	QVBoxLayout *layout_processes = new QVBoxLayout(frame_processes);
	QTableView *view_processes = new QTableView(frame_processes);
	// view_processes->setModel( model_processes );
	layout_processes->addWidget(view_processes);

	/** settings frame */
	QFrame *frame_settings = new QFrame(toolbox);
	toolbox->addItem( frame_settings, tr("Settings") );
	QVBoxLayout *layout_settings = new QVBoxLayout(frame_settings);
	QPushButton* button = new QPushButton( "This Button does &Nothing", frame_settings );
	layout_settings->addWidget(button);

	/** actions */
	main_exit = new QAction(tr("&Exit"), this);
  main_exit->setIcon(QIcon(":/images/application-exit.png"));
  main_exit->setShortcut(tr("Ctrl+Q"));
  main_exit->setStatusTip(tr("Exit"));
  connect(main_exit, SIGNAL(triggered()), qApp, SLOT(quit()));

  hosts_expand = new QAction(tr("Expand"), this);
  hosts_expand->setIcon(QIcon(":/images/placeholder.png"));
  hosts_expand->setShortcut(tr("Ctrl++"));
  hosts_expand->setStatusTip(tr("Expand Host Groups"));
  connect(hosts_expand, SIGNAL(triggered()), view_hostlist, SLOT(expandAll()));

  hosts_collapse = new QAction(tr("Collapse"), this);
  hosts_collapse->setIcon(QIcon(":/images/placeholder.png"));
  hosts_collapse->setShortcut(tr("Ctrl+-"));
  hosts_collapse->setStatusTip(tr("Collapse Host Groups"));
  connect(hosts_collapse, SIGNAL(triggered()), view_hostlist, SLOT(collapseAll()));

	/** toolbar */
  mainToolBar = addToolBar(tr("&Netmon"));
  mainToolBar->addAction(main_exit);
  mainToolBar->addAction(hosts_expand);
  mainToolBar->addAction(hosts_collapse);

  // filterToolbar = addToolBar(tr("&Filter"));
  // filterToolbar->addAction(filter_own_processes);
}

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
