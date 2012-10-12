#pragma once

#include "netmon-hosts.hpp"
#include "netmon-selections.hpp"
#include "netmon-types.hpp"
#include "hostlistmodel.hpp"
#include "hostlistproxy.hpp"
#include "processlistmodel.hpp"
#include "processlistproxy.hpp"

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <unordered_map>

class QAction;
class QObject;
class QHBoxLayout;
class QLineEdit;
class QTabWidget;
class QToolBox;
class QTreeView;
class QTableView;
class QToolBar;

class QItemSelectionModel;
class QItemSelection;

class NetmonWindow : public QMainWindow
{
	Q_OBJECT

public:
	NetmonWindow();
	// NetmonWindow( int argc, char** argv );
	virtual ~NetmonWindow();

public slots:
	void filterUpdate();
	void HostSelectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
	void updateAll();

	void showAboutDialog();

private:
	NetmonHosts netmonHosts;
	HostSelections hostSelected;

	NetmonHostlistModel *model_hostlist;
	QItemSelectionModel *hosts_selected;
	NetmonHostlistProxy *proxy_hostlist;
	QTreeView *view_hostlist;

	NetmonProcessListModel *model_processes;
	NetmonProcessListProxy *proxy_processes;
	QTreeView * view_processes;

	QToolBar *mainToolBar;

	QAction *main_exit, *main_update;
	QAction *hosts_expand, *hosts_collapse;
	QAction *filter_user, *filter_hosts_dead;
	QAction *main_about;

	QLineEdit *filterEdit;

	void createModels();
	void createToolbar();
};
