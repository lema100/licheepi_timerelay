#include "endpoints.h"

endpoint_base::result SettingEnpoint::get(void)
{
	QJsonObject jObj;

	auto conf = _env->get_global();
	jObj["GPIO green led"] = conf.gpio_green;
	jObj["GPIO red led"] = conf.gpio_red;
	jObj["Relay count"] = conf.relay_count;
	jObj["Timezone"] = conf.timezone;

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
