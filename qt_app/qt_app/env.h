#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QSettings>
#include <QVector>
#include <QDebug>
#include "logger.h"

enum relay_mode_t
{
	OFF,
	TIME,
	MANUAL,
	PULSE,
	PWM,
};

struct endpoint_config
{
};

struct eth_cfg
{
	bool dhcp;
	QString ip;
	QString mask;
	QString gateway;
};

struct relay_cfg
{
	int gpio;
	bool state;
	relay_mode_t mode;
	QMap<QTime, bool> timeline;
	int pulse_on, pulse_off;
};

struct global_conf
{
	int gpio_red, gpio_green;
	int relay_count;
	QString timezone;
	QStringList timezone_list;
	int http_port;
};

class env
{
public:
	env(const QString & iniFile);
	~env();

	QMap<int, relay_cfg> get_relay(void);
	void set_relay(QMap<int, relay_cfg>);

	endpoint_config get_endpoint_conf(void);

	void set_global(global_conf);
	global_conf get_global(void);

	void set_eth(eth_cfg);
	eth_cfg get_eth(void);

	logger * get_logger(void);

	const QMap<QString, LogLevel> string_to_level =
	{
		{ "OFF", LogLevel::OFF },
		{ "ERROR", LogLevel::ERROR },
		{ "WARNING", LogLevel::WARNING },
		{ "INFORM", LogLevel::INFORM },
		{ "DEBUG", LogLevel::DEBUG },
	};
	const QMap<QString, relay_mode_t> sting_to_relay_mode =
	{
		{ "OFF", relay_mode_t::OFF },
		{ "TIME", relay_mode_t::TIME },
		{ "MANUAL", relay_mode_t::MANUAL },
		{ "PULSE", relay_mode_t::PULSE },
		{ "PWM", relay_mode_t::PWM },
	};

private:
	QSettings settings;
	logger * _logger;
};

#endif // ENVIRONMENT_H
