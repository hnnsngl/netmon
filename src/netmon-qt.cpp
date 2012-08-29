#include <QApplication>

#include "netmon-qt-window.hpp"

int main( int argc, char** argv )
{
	QApplication application( argc, argv );

	NetmonWindow netmonWindow(argc, argv);
	netmonWindow.resize(600, 600);
  netmonWindow.show();

	return application.exec();
}
