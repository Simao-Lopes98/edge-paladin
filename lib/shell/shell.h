#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_err.h>
#include <esp_log.h>

#ifndef SHELL_H
#define SHELL_H

#define SHELL_TASK_STACK            4096
#define SHELL_TASK_NAME             "ShellTask"
#define SHELL_LINE_MAX_BUFF_SIZE    1024
#define SHELL_TOKEN_BUF_SIZE        64
#define SHELL_TOKEN_DELIM           " \t\r\n\a"
#define SHELL_LOG_TAG "SHELL"
#define SHELL_BUILTIN_MAX_NAME      16

typedef struct shellBuiltIn
{
    int (* function) (char **);
    char name [SHELL_BUILTIN_MAX_NAME];
} shellBuiltIn_t;


extern void shellSrv(void * pvParameters);

#endif