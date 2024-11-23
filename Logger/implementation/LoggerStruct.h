#ifndef CMLIB_LOGGER_STRUCT_H
#define CMLIB_LOGGER_STRUCT_H

#include "Types.h"

void LoggerInit(const char path[static 1]);

void LoggerInitFile(FILE file[static 1]);

void LoggerInitConsole();

void LoggerFinish();

Logger* GetLogger();

#endif // CMLIB_LOGGER_STRUCT_H
