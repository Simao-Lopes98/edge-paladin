#include <uart.h>


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