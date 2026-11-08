#include <uart.h>


/* CMDs */
static int uart (char ** args);

CMD_REGISTER(uart, &uart, UART_CMD_DESC);

/* Helpers */
static void uartParityToStr (uart_parity_t parityMode, char * parityStr);
static float uartStopBitsToNumb (uart_stop_bits_t stopBits);
static int uartDataBitsToNumb (uart_word_length_t dataBits);

static uart_config_t uart_config = {0};

esp_err_t uartInitDrv ( uart_port_t port, 
                        gpio_num_t tx, 
                        gpio_num_t rx, 
                        int baudrate)
{
    esp_err_t ret = ESP_OK;

    uart_config.baud_rate = baudrate;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.source_clk = UART_SCLK_DEFAULT;

    ret = uart_driver_install (port, UART_RX_BUFF_SIZE, 0, 0, NULL, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGW (UART_LOG_TAG, "Fail uart_driver_install");
        return ret;
    }

    ret = uart_param_config (port, &uart_config);
    if (ret != ESP_OK)
    {
        ESP_LOGW (UART_LOG_TAG, "Fail uart_param_config");
        return ret;
    }

    ret = uart_set_pin( port, tx, rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (ret != ESP_OK)
    {
        ESP_LOGW (UART_LOG_TAG, "Fail uart_set_pin");
        return ret;
    }

    return ret;

}
                                
int uartWrite (uart_port_t port, char * strToWrite)
{
    // TODO - Add guards for port and str. Return -1 if bad params

    return uart_write_bytes ( port, 
                            (const char *) strToWrite, 
                            strlen(strToWrite));
}

int uartWriteByte (uart_port_t port, char * charToWrite)
{
        // TODO - Add guards for port and str. Return -1 if bad params

    return uart_write_bytes ( port, 
                            (const char *) charToWrite, 
                            1);
}

int uartRead (uart_port_t port, char * readStr)
{
    // TODO - Add guards for port and str. Return -1 if bad params
    
    return uart_read_bytes (port,
                            readStr, 
                            (sizeof (readStr) -1), 
                            pdMS_TO_TICKS(50));
}

static void uartParityToStr (uart_parity_t parityMode, char * parityStr)
{
    switch (parityMode)
    {
        case UART_PARITY_DISABLE:
            strncpy(parityStr, "Parity Disabled", UART_MAX_PARITY_STR);
            break;
        case UART_PARITY_EVEN:
            strncpy(parityStr, "Parity Even", UART_MAX_PARITY_STR);
            break;
        case UART_PARITY_ODD:
            strncpy(parityStr, "Parity Odd", UART_MAX_PARITY_STR);
            break;
        default:
            // Not possible
            break;
    }
}

static float uartStopBitsToNumb (uart_stop_bits_t stopBits)
{
    switch (stopBits)
    {
        case UART_STOP_BITS_1:
            return 1.0;
        case UART_STOP_BITS_1_5:
            return 1.5;
        case UART_STOP_BITS_2:
            return 2.0;
        default:
            // Not possible
            break;
    }
    // Not possible
    return 404.0;
}

static int uartDataBitsToNumb (uart_word_length_t dataBits)
{
    switch (dataBits)
    {
        case UART_DATA_5_BITS:
            return 5;
        case UART_DATA_6_BITS:
            return 6;
        case UART_DATA_7_BITS:
            return 7;
        case UART_DATA_8_BITS:
            return 8;
        default:
            // Not possible
            break;
    }
    // Not possible
    return 404.0;
}

/* CMDs */
// TODO: The user should be able to select which uart port. Only 1 is programmed now
static int uart (char ** args)
{
    char infoToPrint[UART_MAX_STR_SZ];
    char parityStr[UART_MAX_PARITY_STR];

    if (args == NULL || args [0] == NULL || args [1] == NULL)
    {
        snprintf(infoToPrint, UART_MAX_STR_SZ, 
                "Invalid arguments for uart\r\n"
                "-a [Get all info]\r\n"
                "-baud [Get Baudrate]");
        shellWriteLine (infoToPrint);
        return 1;
    }

    if (strncmp(args[1], "-a", UART_MAX_CMD_SZ) == 0)
    {
        uartParityToStr(uart_config.parity, parityStr);
        snprintf(infoToPrint, UART_MAX_STR_SZ, 
                "Baundrate: %d\r\n"
                "Databits: %d\r\n"
                "Parity: %s\r\n"
                "Stopbits: %.2f",
                uart_config.baud_rate,
                uartDataBitsToNumb(uart_config.data_bits),
                parityStr,
                uartStopBitsToNumb(uart_config.stop_bits));
    } 
    else if (strncmp(args[1], "-baud", UART_MAX_CMD_SZ) == 0)
    {
        snprintf(infoToPrint, UART_MAX_STR_SZ, 
                "Baundrate: %d",
                uart_config.baud_rate);
    }
    else
    {
        snprintf(infoToPrint, UART_MAX_STR_SZ, 
                "Invalid arguments for uart\r\n"
                "-a [Get all info]\r\n"
                "-baud [Get Baudrate]");
    }
    shellWriteLine (infoToPrint);

    return 1;
}