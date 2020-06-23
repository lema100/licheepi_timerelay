#include "endpoints.h"

endpoint_base::result TestEnpoint::get(void)
{
	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_info("GET test handler ok")};
}
endpoint_base::result HelloEndpoint::get(void)
{
	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_info("GET hello handler ok")};
}

endpoint_base::result SelfKillEndpoint::get(void)
{
	QCoreApplication::quit();
	return {qhttp::TStatusCode::ESTATUS_OK, {}, jDoc_warn("Start harakiri")};
}
