#ifndef ENDPOINT_BASE_H
#define ENDPOINT_BASE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QList>

#include "../qhttp/qhttpserverresponse.hpp"
#include "../qhttp/qhttpserverrequest.hpp"
#include "../qhttp/qhttpserver.hpp"

#include "env.h"

namespace http = qhttp::server;

class endpoint_base
{
public:
	struct result
	{
		qhttp::TStatusCode _status;
		QMap<QString, QString> _headers;
		QByteArray _body;
	};
	
	endpoint_base();
	~endpoint_base();

	void set_config(env * __env);

	virtual void parse_query(http::QHttpRequest * _req);

	virtual result post(void);
	virtual result get(void);

	virtual QList<qhttp::THttpMethod> methods(void) = 0;
	virtual QString path(void) = 0;

	static QByteArray jDoc_mess(QString msg, QString status);
	static QByteArray jDoc_error(QString msg);
	static QByteArray jDoc_warn(QString msg);
	static QByteArray jDoc_info(QString msg);
	static QByteArray jDoc_data(QJsonObject data);
	static QByteArray jDoc_data(QJsonArray data);
	static QByteArray jDoc_data(QByteArray data);
	static void res_error(http::QHttpResponse * _res, QString msg);

protected:
	QString _path;
	QMap<QString, QStringList> _args;
	QByteArray _body;
	env * _env;
};

#endif // ENDPOINT_BASE_H
