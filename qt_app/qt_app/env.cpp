#include "env.h"

env::env(const QString & iniFile) : settings(iniFile, QSettings::IniFormat, nullptr)
{
	QMap<QString, LogLevel> string_to_level =
	{
		{ "OFF", LogLevel::OFF },
		{ "ERROR", LogLevel::ERROR },
		{ "WARNING", LogLevel::WARNING },
		{ "INFORM", LogLevel::INFORM },
		{ "DEBUG", LogLevel::DEBUG },
	};

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

QVector<relay_cfg> env::get_relay(void)
{
	QVector<relay_cfg> ret;
	relay_cfg tmp;

	QMap<QString, relay_mode_t> sting_to_mode =
	{
		{ "OFF", relay_mode_t::OFF },
		{ "TIME", relay_mode_t::TIME },
		{ "MANUAL", relay_mode_t::MANUAL },
		{ "PULSE", relay_mode_t::PULSE },
		{ "PWM", relay_mode_t::PWM },
	};

	int count = settings.value("RELAY_COUNT", 0).toInt();

	for (int i = 0; i < count; i++)
	{
		settings.beginGroup(QString("RELAY%1").arg(i));

		tmp.mode = sting_to_mode[settings.value("MODE", "OFF").toString()];
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

		ret.push_back(tmp);
	}

	return ret;
}

void env::set_relay(QVector<relay_cfg>)
{

}

void env::set_global(global_conf conf)
{
	settings.setValue("RELAY_COUNT", conf.relay_count);
	settings.setValue("TIMEZONE", conf.timezone);
	settings.setValue("GPIO_RED", conf.gpio_red);
	settings.setValue("GPIO_GREEN", conf.gpio_green);
}

global_conf env::get_global(void)
{
	global_conf ret;

	ret.relay_count = settings.value("RELAY_COUNT", 0).toInt();
	ret.timezone = settings.value("TIMEZONE", "UTC").toString();
	ret.gpio_red = settings.value("GPIO_RED", 0).toInt();
	ret.gpio_green = settings.value("GPIO_GREEN", 0).toInt();

	return ret;
}

endpoint_config env::get_endpoint_conf(void)
{
	endpoint_config ret;

	settings.beginGroup("ENDPOINTS");

	settings.endGroup();

	return ret;
}

QString env::get_http_port(void)
{
	return settings.value("HTTP_PORT", "8000").toString();
}
