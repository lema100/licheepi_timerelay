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
};

struct relay_cfg
{
	int gpio;
	relay_mode_t mode;
	QMap<QTime, bool> timeline;
};

struct global_conf
{
	int relay_count;
	QString timezone;
};

class env
{
public:
	env(const QString & iniFile);
	~env();

	QVector<relay_cfg> get_relay(void);
	void set_relay(QVector<relay_cfg>);

	void set_global(global_conf);
	global_conf get_global(void);
	logger * get_logger(void);

private:
	QSettings settings;
	logger * _logger;
};

#endif // ENVIRONMENT_H
