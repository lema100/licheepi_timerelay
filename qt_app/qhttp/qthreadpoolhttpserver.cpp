#include "qthreadpoolhttpserver.hpp"
#include "qhttpserverconnection.hpp"
#include "private/qhttpserver_private.hpp"
#include <QEventLoop>

namespace qhttp {
namespace server {

RunRequestTask::RunRequestTask(qintptr handle,TBackend backendType, int timeout,TServerHandler handler):
	_handle(handle),
	_handler(handler),
	_backendType(backendType),
	_timeout(timeout)
{
}

RunRequestTask::~RunRequestTask()
{
}

void RunRequestTask::run()
{
	QEventLoop eventloop;
	
	QHttpConnection* conn = QHttpConnection::create(_handle, _backendType, &eventloop);
	conn->setTimeOut(_timeout);
	QObject::connect(conn, &QHttpConnection::newRequest, _handler);
	QObject::connect(conn, &QHttpConnection::disconnected, [&eventloop]() {
		eventloop.exit(); 
	});
	eventloop.exec();//
	delete conn;
}


QThreadPoolHttpServer::QThreadPoolHttpServer(int threads, QObject * parent): QHttpServer(parent), _pool(this) {
	_pool.setMaxThreadCount(threads);
	setTimeOut(360000);
}

void QThreadPoolHttpServer::incomingConnection(qintptr handle)
{
	_pool.start(new RunRequestTask(handle, backendType(), get_d()->itimeOut, get_d()->ihandler));
}


}
}