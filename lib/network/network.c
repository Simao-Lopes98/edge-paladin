#include <network.h>

wifi_config_t wifi_config = {
    .sta = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASSWORD,
        .threshold ={
            .authmode = WIFI_AUTH_WPA2_PSK,
        },
    },
};