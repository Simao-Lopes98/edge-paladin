#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_chip_info.h>
#include <esp_timer.h>
#include <shell.h>

#ifndef UTILS_H
#define UTILS_H

#define UTILS_LOG_TAG                   "UTILS"
#define UTILS_INFO_DESC                 "List information regarding the hardware"
#define UTILS_REBOOT_DESC               "Warm reboots the system"
#define UTILS_UPTIME_DESC               "Get uptime since boot, in microseconds"

#define UTILS_MAX_STR_SZ                256


extern void utils_init(void);

#endif