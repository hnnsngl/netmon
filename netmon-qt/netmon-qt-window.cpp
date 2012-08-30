#include "netmon-qt-window.hpp"
#include "netmon-qt-models.hpp"

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
	// view_hostlist->setModel( model_hostlist );
	layout_hostlist->addWidget(view_hostlist);

	/* processes frame */
	QFrame *frame_processes = new QFrame(toolbox);
	toolbox->addItem( frame_processes, tr("Processes") );
	QVBoxLayout *layout_processes = new QVBoxLayout(frame_processes);
	QTableView *view_processes = new QTableView(frame_processes);
	// view_processes->setModel( model_processes );
	layout_processes->addWidget(view_processes);

	// no need for a "my processes" frame - if we just add an option to filter processes in this way
	// /** settings frame */
	// QFrame *frame_myprocs = new QFrame(toolbox);
	// toolbox->addItem( frame_myprocs, tr("My Processes") );
	// QVBoxLayout *layout_myprocs = new QVBoxLayout(frame_myprocs);
	// QTableView *view_myprocs = new QTableView(frame_myprocs);
	// view_myprocs->setModel( model_myprocs )
	// layout_myprocs->addWidget(table_myprocs);

	/** settings frame */
	QFrame *frame_settings = new QFrame(toolbox);
	QVBoxLayout *layout_settings = new QVBoxLayout(frame_settings);
	toolbox->addItem( frame_settings, tr("Settings") );

	// create and connect user actions
	QAction *action;

	action = new QAction(tr("Exit"), this);
	action->setShortcut(tr("Ctrl+Q"));
	connect(action, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

NetmonWindow::~NetmonWindow()
{}

void NetmonWindow::createModels()
{}
