#include "netmon-types.hpp"
#include "netmon-hosts.hpp"

#include <QMainWindow>
#include <QObject>
#include <QWidget>


class QAction;
class QObject;
class QHBoxLayout;
class QTabWidget;
class QToolBox;
class QTreeView;
class QToolBar;

class NetmonHostlistModel;
class NetmonProcessListModel;


class NetmonWindow : public QMainWindow
{
	Q_OBJECT

public:
	NetmonWindow();
	// NetmonWindow( int argc, char** argv );
	virtual ~NetmonWindow();

public slots:
	void filterUpdate();

private:
	QToolBox *toolbox;

	NetmonHosts netmonHosts;

	NetmonHostlistModel *model_hostlist;
	NetmonProcessListModel *model_processes;

	QToolBar *mainToolBar, *filterToolbar;

	QAction *main_exit;
	QAction *hosts_expand, *hosts_collapse;
	QAction *filter_user;

	void createModels();
	void createToolbar();
	void createViews();
};
