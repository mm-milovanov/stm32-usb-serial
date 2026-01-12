#include "spi1.h"

#include <ESDL/gpio.h>
#include <cmsis/stm32f1xx.h>

static void SPI1_setNssPin (uint8_t status);

// SPI handle
spi_conf_t __SPI1__;
// SPI1 pins
static gpio_pin_hdl_t spi1_sck_pin;
static gpio_pin_hdl_t spi1_miso_pin;
static gpio_pin_hdl_t spi1_mosi_pin;
static gpio_pin_hdl_t spi1_nss_pin;

void SPI1_init() {
    /* SPI1 SCK GPIO configuration */

    const gpio_pin_conf_t spi1_sck_conf = {
        .gpioX = GPIOA,
        .idx = 5,
        .cnf = GPIO_CNF_ALTERNATE_PUSH_PULL_OUTPUT,
        .mode = GPIO_MODE_OUTPUT_50MHZ,
        .clk = GPIOA_CLK_EN
    };

    gpio_pin_hdl_ctor(&spi1_sck_pin, &spi1_sck_conf);

    /* SPI1 MISO GPIO configuration */

    const gpio_pin_conf_t spi1_miso_conf = {
        .gpioX = GPIOA,
        .idx = 6,
        .cnf = GPIO_CNF_FLOATING_INPUT,
        .mode = GPIO_MODE_INPUT,
        .clk = GPIOA_CLK_EN
    };

    gpio_pin_hdl_ctor(&spi1_miso_pin, &spi1_miso_conf);

    /* SPI1 MOSI GPIO configuration */

    const gpio_pin_conf_t spi1_mosi_conf = {
        .gpioX = GPIOA,
        .idx = 7,
        .cnf = GPIO_CNF_ALTERNATE_PUSH_PULL_OUTPUT,
        .mode = GPIO_MODE_OUTPUT_50MHZ,
        .clk = GPIOA_CLK_EN
    };

    gpio_pin_hdl_ctor(&spi1_mosi_pin, &spi1_mosi_conf);

    /* SPI1 NSS GPIO configuration */

    const gpio_pin_conf_t spi1_nss_conf = {
        .gpioX = GPIOA,
        .idx = 4,
        .cnf = GPIO_CNF_PUSH_PULL_OUTPUT,
        .mode = GPIO_MODE_OUTPUT_50MHZ,
        .clk = GPIOA_CLK_EN
    };

    gpio_pin_hdl_ctor(&spi1_nss_pin, &spi1_nss_conf);
    // NSS pin must be high not during SPI exchange
    SPI1_setNssPin(GPIO_PIN_SET);

    /* SPI1 configuration */

    spi_conf_t spi_conf =  {
        ._spiX              = SPI1,
        ._device_mode       = SPI_DEVICE_MODE_MASTER,
        ._bus_mode          = SPI_BUS_MODE_DUPLEX_FULL,
        ._slave_select_mode = SPI_SS_MODE_SOFTWARE,
        ._clk_div           = 0b100,
        ._data_length       = SPI_DATA_LENGTH_8B,
        ._data_order        = SPI_DATA_ORDER_MSB,
        ._cpol              = SPI_CPOL_LOW,
        ._cpha              = SPI_CPHA_LOW,
        ._clk_src           = 1, // CLK_SRC_APB2,
        ._clk_en            = RCC_APB2ENR_SPI1EN
    };

    spi_hdl_ctor(&__SPI1__, &spi_conf);
}

void SPI1_deinit()
{ /* TODO: implement */ }

void SPI1_write(char* request, int size)
{
    // Before start spi exchange set NSS pin to low state
    SPI1_setNssPin(GPIO_PIN_RESET);
    // Send request to sx1276 by spi
    spi_hdl_tx(&__SPI1__, request, size);
    // After end spi exchange set NSS pin to high state
    SPI1_setNssPin(GPIO_PIN_SET);
}

void SPI1_read(char* request, char* response, int size)
{
    // Before start spi exchange set NSS pin to low state
    SPI1_setNssPin(GPIO_PIN_RESET);
    // Send request to sx1276 by spi
    spi_hdl_tx_rx(&__SPI1__, request, response, size);
    // After end spi exchange set NSS pin to high state
    SPI1_setNssPin(GPIO_PIN_SET);
}

void SPI1_setNssPin(uint8_t status)
{
    // Wait some time before setting nss pin
    __NOP();
    // Set new status of nss pin
    gpio_pin_hdl_write(&spi1_nss_pin, status);
    // Wait some time after setting nss pin
    __NOP();
}