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
	logger * get_logger(void);

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
