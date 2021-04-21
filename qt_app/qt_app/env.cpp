 #include "env.h"

env::env(const QString & iniFile) : settings(iniFile, QSettings::IniFormat, nullptr)
{
	_logger = new logger(settings.value("LOG_FILE_PATH", iniFile.split(".").first() + ".log").toString(), "");
	_logger->set_log_level(string_to_level[settings.value("LOG_LEVEL", "OFF").toString()]);
}

env::~env()
{
}

logger * env::get_logger(void)
{
	return _logger;
}

QMap<int, relay_cfg> env::get_relay(void)
{
	QMap<int, relay_cfg> ret;
	relay_cfg tmp;

	int count = settings.value("RELAY_COUNT", 0).toInt();

	for (int i = 0; i < count; i++)
	{
		settings.beginGroup(QString("RELAY%1").arg(i));

		tmp.mode = sting_to_relay_mode[settings.value("MODE", "OFF").toString()];
		tmp.state = settings.value("STATE", "OFF").toString() == "ON";
		tmp.gpio = settings.value("GPIO", 0).toInt();
		tmp.pulse_off = settings.value("PULSE_OFF", 0).toInt() * 1000;
		tmp.pulse_on = settings.value("PULSE_ON", 0).toInt() * 1000;

		tmp.timeline.clear();
		auto time_str = settings.value("TIMELINE", "00:00:00.OFF").toString().split(",");
		for (const auto & i : time_str)
		{
			auto point = i.split(".");
			auto time = QTime::fromString(point.first(), "hh:mm:ss");
			if (time.isValid())
				tmp.timeline[time] = point.last() == "ON";
		}

		settings.endGroup();

		ret[i] = tmp;
	}

	return ret;
}

void env::set_relay(QMap<int, relay_cfg> in)
{
	int count = settings.value("RELAY_COUNT", 0).toInt();

	for (int i = 0; i < count; i++)
	{
		if (in.keys().contains(i))
		{
			settings.beginGroup(QString("RELAY%1").arg(i));

			settings.setValue("MODE", sting_to_relay_mode.key(in[i].mode));
			settings.setValue("STATE", in[i].state ? "ON" : "OFF");
			settings.setValue("GPIO", in[i].gpio);
			settings.setValue("PULSE_OFF", in[i].pulse_off / 1000);
			settings.setValue("PULSE_ON", in[i].pulse_on / 1000);

			QStringList timeline;
			for (const auto & key : in[i].timeline.keys())
				timeline.append(key.toString("hh:mm:ss") + "." + (in[i].timeline[key] ? "ON" : "OFF"));
			settings.setValue("TIMELINE", timeline.join(","));

			settings.endGroup();
		}
	}
}

void env::set_global(global_conf conf)
{
	settings.setValue("RELAY_COUNT", conf.relay_count);
	settings.setValue("TIMEZONE", conf.timezone);
	settings.setValue("GPIO_RED", conf.gpio_red);
	settings.setValue("GPIO_GREEN", conf.gpio_green);
	settings.setValue("HTTP_PORT", conf.http_port);
}

global_conf env::get_global(void)
{
	global_conf ret;

	ret.relay_count = settings.value("RELAY_COUNT", 0).toInt();
	ret.timezone = settings.value("TIMEZONE", "UTC").toString();
	ret.timezone_list = settings.value("TIMEZONE_LIST", "UTC-4,UTC-3,UTC-2,UTC-1,UTC,UTC+1,UTC+2,UTC+3,UTC+4").toString().split(",");
	ret.gpio_red = settings.value("GPIO_RED", 0).toInt();
	ret.gpio_green = settings.value("GPIO_GREEN", 0).toInt();
	ret.http_port = settings.value("HTTP_PORT", 8000).toInt();

	return ret;
}

void env::set_eth(eth_cfg conf)
{
	settings.beginGroup("ETHERNET");

	settings.setValue("DHCP", conf.dhcp);
	settings.setValue("IP", conf.ip);
	settings.setValue("MASK", conf.mask);
	settings.setValue("GATEWAY", conf.gateway);

	settings.endGroup();
}

eth_cfg env::get_eth(void)
{
	eth_cfg ret;

	settings.beginGroup("ETHERNET");

	ret.dhcp = settings.value("DHCP", false).toBool();
	ret.ip = settings.value("IP", "192.168.0.100").toString();
	ret.mask = settings.value("MASK", "255.255.255.0").toString();
	ret.gateway = settings.value("GATEWAY", "192.168.0.1").toString();

	settings.endGroup();

	return ret;
}

endpoint_config env::get_endpoint_conf(void)
{
	endpoint_config ret;

	settings.beginGroup("ENDPOINTS");

	settings.endGroup();

	return ret;
}
