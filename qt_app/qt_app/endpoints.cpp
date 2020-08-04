#include "endpoints.h"

endpoint_base::result RelayEnpoint::get(void)
{
	QJsonObject jObj;

	auto relays = _env->get_relay();
	if (_args.contains("state"))
	{
		for (const auto & i : relays.keys())
			jObj[QString::number(i)] = gpio::get_value(relays[i].gpio) ? "ON" : "OFF";
	}
	else
	{
		for (const auto & i : relays.keys())
		{
			QJsonObject jObj_tmp, jObj_timeline;

			jObj_tmp["gpio"] = relays[i].gpio;
			jObj_tmp["state"] = relays[i].state ? "ON" : "OFF";
			jObj_tmp["mode"] = _env->sting_to_relay_mode.key(relays[i].mode);
			jObj_tmp["pulse off"] = relays[i].pulse_off / 1000;
			jObj_tmp["pulse on"] = relays[i].pulse_on / 1000;
			for (const auto & j : relays[i].timeline.keys())
				jObj_timeline[j.toString("hh:mm:ss")] = relays[i].timeline[j] ? "ON" : "OFF";
			jObj_tmp["timeline"] = jObj_timeline;
			jObj[QString::number(i)] = jObj_tmp;
		}
	}

	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_data(jObj)};
}

endpoint_base::result RelayEnpoint::post(void)
{
	QJsonObject jObj = QJsonDocument::fromJson(_body).object();

	auto relays = _env->get_relay();
	for (const auto & key : jObj.keys())
	{
		int i = key.toInt();
		if (relays.contains(i))
		{
			auto jRelay = jObj[key].toObject();
			if (jRelay.contains("gpio"))
				relays[i].gpio = jRelay["gpio"].toInt();
			if (jRelay.contains("state"))
				relays[i].state = jRelay["state"].toString() == "ON";
			if (jRelay.contains("mode"))
				relays[i].mode = _env->sting_to_relay_mode[jRelay["mode"].toString()];
			if (jRelay.contains("pulse off"))
				relays[i].pulse_off = jRelay["pulse_off"].toInt() * 1000;
			if (jRelay.contains("pulse on"))
				relays[i].pulse_on = jRelay["pulse_on"].toInt() * 1000;
			if (jRelay.contains("timeline"))
			{
				QMap<QTime, bool> timeline;
				auto jTimeline = jRelay["timeline"].toObject();
				for (const auto & key : jTimeline.keys())
					timeline[QTime::fromString(key, "hh:mm:ss")] = jTimeline[key] == "ON";
				relays[i].timeline = timeline;
			}
		}
	}
	_env->set_relay(relays);

	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_data(jObj)};
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
