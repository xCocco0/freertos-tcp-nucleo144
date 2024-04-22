
#ifndef __LOGGER_H
#define __LOGGER_H

void vLoggerInit(void);

void vLoggerPrint(const char *format, ...);
void vLoggerPrintline(const char *format, ...);

#endif /* __LOGGER_H */
