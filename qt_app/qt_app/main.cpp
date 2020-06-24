#include "app.h"

int main(int argc, char *argv[])
{
	app _app(argc, argv);

	if (_app.get_http_state())
		_app.exec();

	return 0;
}
