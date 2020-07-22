#include "endpoint_base.h"

endpoint_base::endpoint_base()
{
}

endpoint_base::~endpoint_base()
{

}

QByteArray endpoint_base::jDoc_mess(QString msg, QString status)
{
	QJsonObject jObj;
	jObj["status"] = status;
	jObj["data"] = msg;
	return QJsonDocument(jObj).toJson(QJsonDocument::Compact);
}

QByteArray endpoint_base::jDoc_error(QString msg)
{
	return jDoc_mess(msg, "error");
}

QByteArray endpoint_base::jDoc_warn(QString msg)
{
	return jDoc_mess(msg, "warning");
}

QByteArray endpoint_base::jDoc_info(QString msg)
{
	return jDoc_mess(msg, "info");
}

QByteArray endpoint_base::jDoc_data(QByteArray data)
{
	QJsonObject jObj;
	jObj["status"] = "ok";
	jObj["data"] = QString(data.toBase64());
	return QJsonDocument(jObj).toJson(QJsonDocument::Compact);
}

QByteArray endpoint_base::jDoc_data(QJsonArray data)
{
	QJsonObject jObj;
	jObj["status"] = "ok";
	jObj["data"] = data;
	return QJsonDocument(jObj).toJson(QJsonDocument::Compact);
}

QByteArray endpoint_base::jDoc_data(QJsonObject data)
{
	QJsonObject jObj;
	jObj["status"] = "ok";
	jObj["data"] = data;
	return QJsonDocument(jObj).toJson(QJsonDocument::Compact);
}

void endpoint_base::res_error(http::QHttpResponse * _res, QString msg)
{
	_res->setStatusCode(qhttp::TStatusCode::ESTATUS_NOT_FOUND);
	_res->addHeaderValue("content-type", QString("application/json"));
	_res->end(jDoc_error(msg));
}

void endpoint_base::set_config(env * __env)
{
	_env = __env;
}

void endpoint_base::parse_query(http::QHttpRequest * _req)
{
	_path = _req->url().path();
	_body = _req->collectedData();
	auto query = _req->url().query();
	for (const auto & param : query.split("&"))
	{
		auto key_val = param.split("=");
		if (key_val.size() == 2)
			_args[key_val[0]] = key_val[1].split(",");
		else
			_args[key_val[0]] = QStringList();
	}
}

endpoint_base::result endpoint_base::post(void)
{
	return {qhttp::TStatusCode::ESTATUS_NOT_FOUND, {}, jDoc_error("POST handler not described")};
}

endpoint_base::result endpoint_base::get(void)
{
	return {qhttp::TStatusCode::ESTATUS_NOT_FOUND, {}, jDoc_error("GET handler not described")};
}
