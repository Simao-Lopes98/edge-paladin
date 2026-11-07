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
#define SHELL_UNKOWN_CMD_MSG_SZ     64
#define SHELL_UNKOWN_CMD_STR        "Unkown command"
#define SHELL_BUILTIN_MAX_NAME      16
#define SHELL_BUILTIN_MAX_DESC      256
#define SHELL_HELP_DESC             "Lists all functions"
#define SHELL_CLEAR_DESC            "Clears the terminal screen"


typedef struct shellCmd
{
    int (* function) (char **);
    char name [SHELL_BUILTIN_MAX_NAME];
    char desc [SHELL_BUILTIN_MAX_DESC];
} shellCmd_t;

// Linker table symbols
extern const shellCmd_t _my_cmds_start[];
extern const shellCmd_t _my_cmds_end[];

// Registers the symbol on the .my_cmds memory section
#define CMD_REGISTER(id, func, description)                         \
    static const shellCmd_t _cmd_##id                               \
    __attribute__((used, aligned(4), section(".my_cmds."#id))) = {  \
        .name = #id, .function = func, .desc = description          \
    }                                                               \


#define CMD_COUNT() (((uintptr_t)_my_cmds_end - (uintptr_t)_my_cmds_start) / sizeof(shellCmd_t))


extern void shellSrv(void * pvParameters);

#endif