#ifndef _logger_
#define _logger_

#include <syslog.h>

#ifdef DEBUG
        #define LOGGER_INFO(msg) syslog(LOG_INFO, msg)
        #define LOGGER_INFOF(fmt, ...) syslog(LOG_INFO, fmt, __VA_ARGS__) 
        #define LOGGER_DEBUG(msg) syslog(LOG_DEBUG, msg)
        #define LOGGER_DEBUGF(fmt, ...) syslog(LOG_DEBUG, fmt, __VA_ARGS__)
#else
        #define LOGGER_INFO
        #define LOGGER_INFOF
        #define LOGGER_DEBUG
        #define LOGGER_DEBUGF
#endif


#define LOGGER_ERR(msg) syslog(LOG_ERR, msg)
#define LOGGER_ERRF(fmt, ...) syslog(LOG_ERR, fmt, __VA_ARGS__)

#endif
