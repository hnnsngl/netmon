#include "netmon-types.hpp"

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

private:
	QToolBox *toolbox;

	NetmonHostlistModel *model_hostlist;
	NetmonProcessListModel *model_processes;

	QToolBar *mainToolBar, *filterToolbar;

	QAction *main_exit;
	QAction *filter_own_processes;

	void createModels();
	void createToolbar();
	void createViews();
};
