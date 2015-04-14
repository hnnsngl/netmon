#pragma once

#include <QThread>

class NetmonUpdateThread : public QThread
{
	Q_OBJECT

	int interval;
	bool forceit;
	bool enabled;

public:
	NetmonUpdateThread(QObject * parent , int _interval);
	void run();
	void setInterval();
	// void setEnabled(bool);
signals:
	void updatedHostList();

public slots:
	void forceUpdate();
};











