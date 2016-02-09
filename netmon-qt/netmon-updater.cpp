#include "netmon-updater.hpp"
#include "netmon-types.hpp"
#include "netmon-lib.hpp"

extern HostList hostList;
extern std::mutex mutexList;

NetmonUpdateThread::NetmonUpdateThread( QObject * parent, int _interval )
	: QThread(parent),
	  interval(_interval)
{}

void NetmonUpdateThread::run()
{
	const int minterval = interval * 1000;
	const int sleepstep = interval * 100;

	while(true){
		// qDebug("start update...");
		refreshHostListBlocking(9221);
		emit updatedHostList();
		// qDebug("finished update");

		forceit = false;
		for( int time = 0; time < minterval ; time += sleepstep )
			{
				if(forceit) break;
				// qDebug("sleeping %d %d", time, minterval);
				msleep(sleepstep);
			}
	}
}

void NetmonUpdateThread::forceUpdate()
{
	forceit = true;
}

// void NetmonUpdateThread::setEnabled(bool state)
// {
// 	enabled = state;
// }


