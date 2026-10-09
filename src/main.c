#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <network.h>


typedef struct
{
    uint32_t func_out_sel : 8;
    uint32_t func_out_inv_sel : 1;
    uint32_t func_out_oen_sel : 1;
    uint32_t func_out_oen_inv_sel : 1;
    uint32_t reserved : 21;
} gpio_func_out_sel_cfg_t;

typedef struct
{
    uint32_t out_data : 26;
    uint32_t reserved : 6;
} gpio_out_t;

typedef struct
{
    uint32_t data : 26;
    uint32_t reserved : 6;
} gpio_data_t;

typedef struct
{
    uint32_t io_mux_gpio_mcu_oe : 1;
    uint32_t io_mux_gpio_slp_sel : 1;
    uint32_t io_mux_gpio_mcu_wpd : 1;
    uint32_t io_mux_gpio_mcu_wpu : 1;
    uint32_t io_mux_gpio_mcu_ie : 1;
    uint32_t io_mux_gpio_mcu_drv : 2;
    uint32_t io_mux_gpio_fun_wpd : 1;
    uint32_t io_mux_gpio_fun_wpu : 1;
    uint32_t io_mux_gpio_fun_ie : 1;
    uint32_t io_mux_gpio_fun_drv : 2;
    uint32_t io_mux_gpio_mcu_sel : 3;
    uint32_t io_mux_gpio_filter_en : 1;
    uint32_t reserved : 16;
} io_mux_gpio_reg_t;


#define PRIPH_BAR   0x60000000UL
#define GPIO_BAR    (PRIPH_BAR + 0x4000UL)
#define IO_MUX_BAR  (PRIPH_BAR + 0x9000UL)

#define GPIO_FUNC0_OUT_SEL_CFG_REG (GPIO_BAR + (0x0554UL + (4 * 0)))
#define GPIO_FUNC8_OUT_SEL_CFG_REG (GPIO_BAR + (0x0554UL + (4 * 8)))
#define GPIO_OUT_REG            (GPIO_BAR + (0x0004UL))
#define GPIO_OUT_W1TS_REG       (GPIO_BAR + (0x0008UL))
#define GPIO_OUT_W1TC_REG       (GPIO_BAR + (0x000CUL))
#define GPIO_ENABLE_REG         (GPIO_BAR + (0x0020UL))
#define GPIO_ENABLE_W1TS_REG    (GPIO_BAR + (0x0024UL))
#define GPIO_ENABLE_W1TC_REG    (GPIO_BAR + (0x0028UL))

#define IO_MUX_GPIO8_REG        (IO_MUX_BAR + ((0x0004) + (4 * 8)))


#define GPIO_FUNC0_OUT_SEL_CFG  (* (volatile gpio_func_out_sel_cfg_t *) GPIO_FUNC0_OUT_SEL_CFG_REG)
#define GPIO_FUNC8_OUT_SEL_CFG  (* (volatile gpio_func_out_sel_cfg_t *) GPIO_FUNC8_OUT_SEL_CFG_REG)
#define GPIO_OUT                (* (volatile gpio_out_t *)GPIO_OUT_REG)
#define GPIO_ENABLE             (* (volatile gpio_data_t *)GPIO_ENABLE_REG)
#define GPIO_OUT_W1TS           (* (volatile gpio_data_t *)GPIO_OUT_W1TS_REG)
#define GPIO_OUT_W1TC           (* (volatile gpio_data_t *)GPIO_OUT_W1TC_REG)
#define GPIO_ENABLE_W1TS        (* (volatile gpio_data_t *)GPIO_ENABLE_W1TS_REG)
#define GPIO_ENABLE_W1TC        (* (volatile gpio_data_t *)GPIO_ENABLE_W1TC_REG)

#define  IO_MUX_GPIO8           (* (volatile io_mux_gpio_reg_t *)IO_MUX_GPIO8_REG)

void app_main() {

    GPIO_ENABLE_W1TS.data = (1 << 8);
    GPIO_FUNC8_OUT_SEL_CFG.func_out_sel = 128;
    GPIO_FUNC8_OUT_SEL_CFG.func_out_oen_sel = 1;
    GPIO_OUT_W1TS.data = (1 << 8);

    // IO_MUX_GPIO8.io_mux_gpio_mcu_oe = 1;
    
    while (1)
    {
        GPIO_OUT_W1TS.data = (1 << 8);
        printf ("Heelo %s - %s\n", WIFI_SSID, WIFI_PASSWORD);
        vTaskDelay(pdMS_TO_TICKS(1000));
        GPIO_OUT_W1TC.data = (1 << 8);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}