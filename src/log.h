#ifndef NODENET_LOG_H
#define NODENET_LOG_H

enum log_level {
	LOG_DEBUG,
	LOG_ERROR,
};

void nodenet_log(const char *format, ...);
void nodenet_log_info(const char *format, ...);
void nodenet_log_debug(const char *format, ...);
void nodenet_log_error(const char *format, ...);

#endif