#pragma once
#include "qhttpserver.hpp"
#include <QThreadPool>
#include <QRunnable>

namespace qhttp {
namespace server {

class RunRequestTask :public QRunnable
{
public:
	RunRequestTask(qintptr handle, TBackend backendType,int timeout, TServerHandler handler);
	~RunRequestTask();
	void run() override;
private:
	qintptr _handle;
	TBackend _backendType;
	int _timeout;
	TServerHandler _handler;
};



class QHTTP_API QThreadPoolHttpServer : public QHttpServer
{
public:
	QThreadPoolHttpServer(int threads, QObject *parent);
	void incomingConnection(qintptr handle) override;
private:
	QThreadPool _pool;
};

}
}