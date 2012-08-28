#include<QMainWindow>
#include<QWidget>

class QAction;
class QObject;
class QHBoxLayout;
class QTabWidget;
class QToolBox;
class QTableView;

class NetmonWindow : public QMainWindow
{
	Q_OBJECT ;

public:
	NetmonWindow( int argc, char** argv );

private:
	QToolBox *toolbox;
};
