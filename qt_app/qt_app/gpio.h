#ifndef GPIO_H
#define GPIO_H

#include <QFile>
#include <QMap>

#ifdef _WIN32
#define NO_GPIO
#endif

class gpio
{
public:
	gpio(void);
	~gpio();

	static bool get_value(int num);
	static void set_value(int num, bool state);
};

#endif // GPIO_H
