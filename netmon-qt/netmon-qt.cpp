#include <QApplication>
#include <QIcon>

#include "netmon-qt-window.hpp"

HostList hostList;
std::mutex mutexList;

int main(int argc, char **argv)
{
	QApplication application(argc, argv);
	application.setWindowIcon(QIcon(":/images/netmon.svg"));

	NetmonWindow netmonWindow;
	netmonWindow.resize(600, 600);
	netmonWindow.show();

	return application.exec();
}
