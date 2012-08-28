#include "netmon-qt-window.hpp"

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

NetmonWindow::NetmonWindow( int argc, char** argv )
{
	// main parts
	toolbox = new QToolBox(this);
	setCentralWidget(toolbox);

	/** host list frame */
	QFrame *frame_hostlist = new QFrame(toolbox);
	toolbox->addItem( frame_hostlist, tr("Hosts") );
	QVBoxLayout *layout_hostlist = new QVBoxLayout(frame_hostlist);
	QTableView *table_hostlist = new QTableView(frame_hostlist);
	layout_hostlist->addWidget(table_hostlist);

	/** processes frame */
	QFrame *frame_processes = new QFrame(toolbox);
	toolbox->addItem( frame_processes, tr("Processes") );
	QVBoxLayout *layout_processes = new QVBoxLayout(frame_processes);
	QTableView *table_processes = new QTableView(frame_processes);
	layout_processes->addWidget(table_processes);

	/** settings frame */
	QFrame *frame_myprocs = new QFrame(toolbox);
	toolbox->addItem( frame_myprocs, tr("My Processes") );
	QVBoxLayout *layout_myprocs = new QVBoxLayout(frame_myprocs);
	QTableView *table_myprocs = new QTableView(frame_myprocs);
	layout_myprocs->addWidget(table_myprocs);

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
