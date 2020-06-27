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
	
	struct config
	{
	};

	endpoint_base();
	~endpoint_base();

	void set_config(config conf);

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
	QMap<QString, QStringList> _args;
	config _conf;
};

#endif // ENDPOINT_BASE_H