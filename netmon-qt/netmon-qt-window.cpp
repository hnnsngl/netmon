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

NetmonWindow::NetmonWindow()
{
	// main parts -- use a toolbox for now, but this is easily changed
	toolbox = new QToolBox(this);
	setCentralWidget(toolbox);

	/* host list frame */
	QFrame *frame_hostlist = new QFrame(toolbox);
	toolbox->addItem( frame_hostlist, tr("Hosts") );
	QVBoxLayout *layout_hostlist = new QVBoxLayout(frame_hostlist);
	QTreeView *view_hostlist = new QTreeView(frame_hostlist);
	model_hostlist = new NetmonHostlistModel(this);
	view_hostlist->setModel( model_hostlist );
	layout_hostlist->addWidget(view_hostlist);

	/* processes frame */
	QFrame *frame_processes = new QFrame(toolbox);
	toolbox->addItem( frame_processes, tr("Processes") );
	QVBoxLayout *layout_processes = new QVBoxLayout(frame_processes);
	QTableView *view_processes = new QTableView(frame_processes);
	model_processes = new NetmonProcessListModel(this);
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

	/** toolbar */
  mainToolBar = addToolBar(tr("&Netmon"));
  mainToolBar->addAction(main_exit);

  // filterToolbar = addToolBar(tr("&Filter"));
  // filterToolbar->addAction(filter_own_processes);
}

NetmonWindow::~NetmonWindow()
{}

void NetmonWindow::createModels()
{}

void NetmonWindow::createToolbar()
{
}
