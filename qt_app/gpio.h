#ifndef GPIO_H
#define GPIO_H

#include <QFile>

class gpio
{
public:
	gpio(void);
	~gpio();

	static bool get_value(int num);
	static void set_value(int num, bool state);

};

#endif // GPIO_H
