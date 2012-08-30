#include "netmon-types.hpp"
#include <map>

#include <QMainWindow>
#include <QObject>
#include <QWidget>


class QAction;
class QObject;
class QHBoxLayout;
class QTabWidget;
class QToolBox;
class QTableView;
class QTreeView;

class NetmonHostlistModel;
class NetmonProcessListModel;


class NetmonWindow : public QMainWindow
{
	Q_OBJECT

public:
	NetmonWindow();
	// NetmonWindow( int argc, char** argv );
	virtual ~NetmonWindow();

private:
	QToolBox *toolbox;

	NetmonHostlistModel *model_hostlist;
	NetmonProcessListModel *model_processes;

	std::map<std::string, HostListItem> hosts;

	void createModels();
	void createViews();
};
