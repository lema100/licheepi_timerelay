#include "endpoints.h"

endpoint_base::result RelayEnpoint::get(void)
{
	QJsonObject jObj;

	auto relays = _env->get_relay();
	if (_args.contains("state"))
	{
		if (_args.contains("relay"))
		{
			if (_args["state"].first() == "ON" || _args["state"].first() == "OFF")
			{
				relays[_args["relay"].first().toInt()].state = (_args["state"].first() == "ON") ? true : false;
				_env->set_relay(relays);
			}
		}
		else
		{
			for (const auto & i : relays.keys())
				jObj[QString::number(i)] = gpio::get_value(relays[i].gpio) ? "ON" : "OFF";
		}
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
				relays[i].pulse_off = jRelay["pulse off"].toInt() * 1000;
			if (jRelay.contains("pulse on"))
				relays[i].pulse_on = jRelay["pulse on"].toInt() * 1000;
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
	QJsonArray jArrTZ;

	auto conf = _env->get_global();
	for (const auto & i : conf.timezone_list)
		jArrTZ << i;

	jObj["GPIO green led"] = conf.gpio_green;
	jObj["GPIO red led"] = conf.gpio_red;
	jObj["Relay count"] = conf.relay_count;
	jObj["Timezone"] = conf.timezone;
	jObj["HTTP port"] = conf.http_port;
	jObj["Timezone list"] = jArrTZ;
	jObj["Current time UTC"] = QDateTime::currentDateTimeUtc().toString("yyyy.MM.dd hh:mm:ss");

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

endpoint_base::result EthEnpoint::post(void)
{
	QJsonObject jObj = QJsonDocument::fromJson(_body).object();

	auto conf = _env->get_eth();
	if (jObj.contains("DHCP"))
		conf.dhcp = jObj["DHCP"].toBool();
	if (jObj.contains("IP"))
		conf.ip = jObj["IP"].toString();
	if (jObj.contains("MASK"))
		conf.mask = jObj["MASK"].toString();
	if (jObj.contains("GATEWAY"))
		conf.gateway = jObj["GATEWAY"].toString();
	_env->set_eth(conf);

	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_data(jObj)};
}

endpoint_base::result EthEnpoint::get(void)
{
	QJsonObject jObj;

	auto conf = _env->get_eth();
	jObj["DHCP"] = conf.dhcp;
	jObj["IP"] = conf.ip;
	jObj["MASK"] = conf.mask;
	jObj["GATEWAY"] = conf.gateway;

	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_data(jObj)};
}

endpoint_base::result RebootEnpoint::get(void)
{
	qDebug() << "Reboot request";
	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_info("Reboot started")};
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
