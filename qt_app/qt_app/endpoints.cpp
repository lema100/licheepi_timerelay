#include "endpoints.h"

endpoint_base::result StaticEnpoint::get(void)
{
	auto filePath = _path.mid(path().length());
	if (!filePath.contains(".."))
	{
		if (filePath.isEmpty())
			filePath = "index.html";

		QFile file("static/" + filePath);

		if (file.open(QIODevice::ReadOnly))
		{
			QByteArray fdata = file.readAll();
			QMap<QString, QString> headers;
			QFileInfo fi(file);
			if (fi.fileName().endsWith("js"))
				headers["content-type"] = "application/javascript";
			if (fi.fileName().endsWith("html"))
				headers["content-type"] = "text/html;charset=utf-8";
			if (fi.fileName().endsWith("css"))
				headers["content-type"] = "text/css";

			return {qhttp::TStatusCode::ESTATUS_OK, headers, fdata};
		}
	}

	return {qhttp::TStatusCode::ESTATUS_NOT_FOUND, {}, jDoc_error("Unavailable path: " + filePath)};
}

endpoint_base::result SettingEnpoint::get(void)
{
	QJsonObject jObj;

	auto conf = _env->get_global();
	jObj["GPIO green led"] = conf.gpio_green;
	jObj["GPIO red led"] = conf.gpio_red;
	jObj["Relay count"] = conf.relay_count;
	jObj["Timezone"] = conf.timezone;
	jObj["HTTP port"] = conf.http_port;

	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_data(jObj)};
}

endpoint_base::result SettingEnpoint::post(void)
{
	QJsonObject jObj = QJsonDocument::fromJson(_body).object();

	auto conf = _env->get_global();
	if (jObj.contains("GPIO green led"))
		conf.gpio_green = jObj["GPIO green led"].toInt();
	if (jObj.contains("GPIO red led"))
		conf.gpio_red = jObj["GPIO red led"].toInt();
	if (jObj.contains("Relay count"))
		conf.relay_count = jObj["Relay count"].toInt();
	if (jObj.contains("Timezone"))
		conf.timezone = jObj["Timezone"].toString();
	if (jObj.contains("HTTP port"))
		conf.http_port = jObj["HTTP port"].toInt();
	_env->set_global(conf);

	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_data(jObj)};
}

endpoint_base::result TestEnpoint::get(void)
{
	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_info("GET test handler ok")};
}

endpoint_base::result HelloEndpoint::get(void)
{
	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_info("GET hello handler ok")};
}

endpoint_base::result SelfKillEndpoint::get(void)
{
	QCoreApplication::quit();
	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_warn("Start harakiri")};
}
