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
#include <QLineEdit>
#include <QMessageBox>

#include <iostream>
#include <string>
#include <cstdlib>

extern HostList hostList;

void createHostList(const NetmonHosts &netmonHosts)
{
	for (const auto &group : netmonHosts.hostnames)
		for (const auto &host : group.second)
			hostList[host] = HostListItem(host);
}

NetmonWindow::~NetmonWindow() {}

NetmonWindow::NetmonWindow()
    : netmonHosts(QDir::homePath().toStdString() + std::string("/.netmon-hosts")), hostSelected(netmonHosts),
      updater(this, 5)
{
	// FIXME: move this stuff to the main program file main() function
	createHostList(netmonHosts);
	createModels();

	// start updater thread
	updater.start();
	connect(&updater, SIGNAL(updatedHostList(void)), this, SLOT(updateAll(void)));

	// main parts
	QSplitter *splitter = new QSplitter(Qt::Vertical, this);
	setCentralWidget(splitter);

	commandFilterEdit = new QLineEdit(this);
	commandFilterEdit->setToolTip("Command Filter");
	connect(commandFilterEdit, SIGNAL(textChanged(const QString)), proxy_processes,
	        SLOT(updateCommandFilter(const QString)));

	userFilterEdit = new QLineEdit(this);
	userFilterEdit->setText(QString(getenv("USER")));
	userFilterEdit->setToolTip("User Filter");
	connect(userFilterEdit, SIGNAL(textChanged(const QString)), proxy_processes,
	        SLOT(updateUserFilter(const QString)));

	// add host and process view widgets
	QFrame *frame_hostlist = new QFrame(this);
	QVBoxLayout *layout_hostlist = new QVBoxLayout(frame_hostlist);
	layout_hostlist->addWidget(view_hostlist);
	splitter->addWidget(frame_hostlist);

	QFrame *frame_processes = new QFrame(this);
	QVBoxLayout *layout_processes = new QVBoxLayout(frame_processes);
	layout_processes->addWidget(view_processes);
	splitter->addWidget(frame_processes);

	createToolbar();
}

void NetmonWindow::filterUpdate() {}

void NetmonWindow::createModels()
{
	// host list data and proxy model
	model_hostlist = new NetmonHostlistModel(netmonHosts, this);
	proxy_hostlist = new NetmonHostlistProxy(netmonHosts, this);
	proxy_hostlist->setSourceModel(model_hostlist);

	/* host list view */
	view_hostlist = new QTreeView(this);
	view_hostlist->setModel(proxy_hostlist);
	view_hostlist->expandAll();
	view_hostlist->setSortingEnabled(true);
	view_hostlist->sortByColumn(0, Qt::AscendingOrder);
	view_hostlist->setSelectionMode(
	    QAbstractItemView::ExtendedSelection); // could also use ::MultipleSeclection here
	view_hostlist->resizeColumnToContents(0);
	view_hostlist->setAlternatingRowColors(true);
	connect(view_hostlist->selectionModel(),
	        SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this,
	        SLOT(HostSelectionChanged(const QItemSelection &, const QItemSelection &)));

	// process list data and proxy model
	model_processes = new NetmonProcessListModel(this);
	proxy_processes = new NetmonProcessListProxy(hostSelected, this);
	proxy_processes->setSourceModel(model_processes);

	/* processes view */
	view_processes = new QTreeView(this);
	view_processes->setModel(proxy_processes);
	view_processes->setSortingEnabled(true);
	view_hostlist->resizeColumnToContents(0);
	view_processes->setSelectionMode(QAbstractItemView::NoSelection);
	view_processes->setAlternatingRowColors(true);
}

void NetmonWindow::createToolbar()
{
	/** actions */
	main_exit = new QAction(tr("&Exit Netmon"), this);
	main_exit->setIcon(QIcon(":/images/application-exit.png"));
	main_exit->setShortcut(tr("Ctrl+Q"));
	main_exit->setStatusTip(tr("Exit"));
	connect(main_exit, SIGNAL(triggered()), qApp, SLOT(quit()));

	main_update = new QAction(tr("Force &Update"), this);
	main_update->setIcon(QIcon(":/images/refresh.png"));
	main_update->setShortcut(tr("R"));
	connect(main_update, SIGNAL(triggered()), &updater, SLOT(forceUpdate()));

	main_about = new QAction(tr("About Netmon"), this);
	main_about->setIcon(QIcon(":/images/netmon.svg"));
	connect(main_about, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

	filter_command = new QAction(tr("Filter processes by commands"), this);
	filter_command->setIcon(QIcon(":/images/filter-command.png"));
	filter_command->setCheckable(true);
	commandFilterEdit->setEnabled(false);
	connect(filter_command, SIGNAL(toggled(bool)), commandFilterEdit, SLOT(setEnabled(bool)));
	connect(filter_command, SIGNAL(toggled(bool)), proxy_processes, SLOT(toggleCommandFilter(bool)));

	filter_user = new QAction(tr("Filter processes by user name"), this);
	filter_user->setIcon(QIcon(":/images/filter-user.png"));
	filter_user->setCheckable(true);
	// userFilterEdit->setEnabled(true);
	connect(filter_user, SIGNAL(toggled(bool)), userFilterEdit, SLOT(setEnabled(bool)));
	connect(filter_user, SIGNAL(toggled(bool)), proxy_processes, SLOT(toggleUserFilter(bool)));
	filter_user->setChecked(true);

	filter_hosts_dead = new QAction(tr("Select online hosts"), this);
	filter_hosts_dead->setIcon(QIcon(":/images/filter-online.png"));
	filter_hosts_dead->setCheckable(true);
	connect(filter_hosts_dead, SIGNAL(toggled(bool)), proxy_hostlist, SLOT(toggleFilter(bool)));

	/** create main toolbar */
	mainToolBar = addToolBar(tr("&Netmon"));
	mainToolBar->toggleViewAction()->setEnabled(false);
	mainToolBar->setFloatable(false);
	/* add toolbar buttons */
	mainToolBar->addAction(main_update);
	mainToolBar->addAction(filter_hosts_dead);
	mainToolBar->addSeparator();
	mainToolBar->addWidget(commandFilterEdit);
	mainToolBar->addAction(filter_command);
	mainToolBar->addWidget(userFilterEdit);
	mainToolBar->addAction(filter_user);
	mainToolBar->addSeparator();
	mainToolBar->addAction(main_about);
	mainToolBar->addAction(main_exit);

	// filterToolbar = addToolBar(tr("&Filter"));
	// filterToolbar->addAction(filter_own_processes);
}

void NetmonWindow::HostSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	auto toggleSelection = [&](const QModelIndex &item, bool enable) {
		if (item.isValid() && item.column() == 0) {
			std::string itemname = item.data().toString().toStdString();
			if (item.parent().isValid())
				hostSelected.selectHost(itemname, enable);
			else
				hostSelected.selectGroup(itemname, enable);
		}
	};

	for (const auto &index : selected.indexes()) {
		if (index.column() == 0) {
			toggleSelection(index, true);
		}
	}
	for (const auto &index : deselected.indexes()) {
		if (index.column() == 0) {
			toggleSelection(index, false);
		}
	}

#ifdef DEBUG
	hostSelected.print(std::cerr);
#endif

	proxy_processes->invalidate();
}

void updateModelIndexes(const QModelIndex &index, QAbstractItemView *view)
{
	int row = 0;
	while (index.sibling(row, 0).isValid()) {
		int col = 0;
		while (index.sibling(row, col).isValid()) {
			view->update(index.sibling(row, col));
			col++;
		}
		updateModelIndexes(index.child(row, 0), view);
		row++;
	}
}

void NetmonWindow::updateAll()
{
	//

	updateModelIndexes(view_hostlist->model()->index(0, 0, QModelIndex()), view_hostlist);
	updateModelIndexes(view_processes->model()->index(0, 0, QModelIndex()), view_processes);
}

void NetmonWindow::showAboutDialog()
{
	QMessageBox::about(this, QString("About Netmon"),
	                   QString("Netmon version 0.2\n"
	                           "License: GPL version 3 or later\n"
	                           "Authors: Martin Marenz and Hannes Nagel\n"
	                           "Sources: git clone ssh://boell.physik.uni-leipzig.de/home/nagel/src/netmon\n"));
}
