#include <utils.h>

/* CMDs */
static int info (char ** args);
static int reboot (char ** args);

/* Register CMDs */
CMD_REGISTER(info, &info, UTILS_INFO_DESC);
CMD_REGISTER(reboot, &reboot, UTILS_REBOOT_DESC);


// Create this symbol so this file is not discarted during linkage
void utils_init(void)
{
    //Do nothing
}

static int info (char ** args)
{
    esp_chip_info_t chipInfo;
    char infoToPrint[UTILS_CHIP_INFO_STR_SZ];

    esp_chip_info(&chipInfo);
    // TODO: Add features, model etc.
    snprintf(infoToPrint, 
             UTILS_CHIP_INFO_STR_SZ, 
             "Number of cores: %"PRIu8"\n", chipInfo.cores);
    
    shellWriteLine(infoToPrint);
    return 1;
}

static int reboot (char ** args)
{
    shellWriteLine("Rebooting...");
    esp_restart();

    // Will never happen
    return 1;
}