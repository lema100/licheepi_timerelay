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

	QVector<relay_cfg> get_relay(void);
	void set_relay(QVector<relay_cfg>);

	endpoint_config get_endpoint_conf(void);

	void set_global(global_conf);
	global_conf get_global(void);
	logger * get_logger(void);

private:
	QSettings settings;
	logger * _logger;
};

#endif // ENVIRONMENT_H
