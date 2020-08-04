#include "gpio.h"

gpio::gpio(void)
{
}

gpio::~gpio()
{
}

static void f_write(QString filename, QString data)
{
	QFile f(filename);
	f.open(QIODevice::WriteOnly);
	f.write(data.toLatin1());
	f.close();
}

static QString f_read(QString filename)
{
	QFile f(filename);
	f.open(QIODevice::ReadOnly);
	auto data = f.readAll();
	f.close();

	return QString(data);
}

#ifdef NO_GPIO
QMap<int, bool> states;

bool gpio::get_value(int num)
{
	return states[num];
}

void gpio::set_value(int num, bool state)
{
	states[num] = state;
}

#else
bool gpio::get_value(int num)
{
	f_write("/sys/class/gpio/export", QString::number(num));
	f_write(QString("/sys/class/gpio/gpio%1/direction").arg(num), QString("in").toLatin1());
	return f_read("/sys/class/gpio/gpio%1/value").contains("1") ? true : false;
}

void gpio::set_value(int num, bool state)
{
	f_write("/sys/class/gpio/export", QString::number(num));
	f_write(QString("/sys/class/gpio/gpio%1/direction").arg(num), QString("out").toLatin1());
	f_write(QString("/sys/class/gpio/gpio%1/value").arg(num), state ? "1" : "0");
}
#endif
