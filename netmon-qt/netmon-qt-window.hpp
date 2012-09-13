#include "netmon-hosts.hpp"
#include "netmon-selections.hpp"
#include "netmon-types.hpp"

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <unordered_map>

class QAction;
class QObject;
class QHBoxLayout;
class QTabWidget;
class QToolBox;
class QTreeView;
class QTableView;
class QToolBar;

class QItemSelectionModel;
class QItemSelection;

class NetmonHostlistModel;
class NetmonHostlistProxy;

class NetmonProcessListModel;
class NetmonProcessListProxy;

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

	QToolBar *mainToolBar, *filterToolbar;

	QAction *main_exit, *main_update;
	QAction *hosts_expand, *hosts_collapse;
	QAction *filter_user, *filter_hosts_dead;

	void createModels();
	void createToolbar();
};
