#include <iostream>

#include "../include/Dfwm.h"
#include "../include/logger.h"

using namespace std;

int main(int argc, char *argv[]) {

#ifdef DEBUG
        setlogmask(LOG_UPTO(LOG_DEBUG));
        LOGGER_INFO("verbose logging enabled");
#else
        setlogmask(LOG_UPTO(LOG_NOTICE));
#endif

        openlog(argv[0], LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

	Dfwm dfwm;
	dfwm.run();

        closelog();

	return 0;
}
