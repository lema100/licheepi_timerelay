#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QObject>
#include <QDebug>
#include <QTimer>

#include "gpio.h"
#include "env.h"

#define VERSION "v.1.0b"
#define DESCRIBE "Lema embedded base timerelay"

class app : public QCoreApplication
{
	Q_OBJECT

public:
	app(int argc, char *argv[]);
	~app();

private:
	QCommandLineParser _args;
	QTimer _everySec;

	bool args(void);
	bool _args_init;
	bool _listen_state;
	env _env;
	QVector<relay_cfg> _relay_config;

public slots:
	void on_timeout(void);
};

#endif // APP_H
