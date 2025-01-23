#include <bsp/spi5.h>
#include <defconfig.h>

#include <lib-stm32drv/gpio.h>
#include <cmsis/stm32f429xx.h>

static void __spi5_nss__ (uint8_t status);

// SPI handle
spi_conf_t __SPI5__;
// SPI5 pins
static gpio_pin_hdl_t spi5_sck_pin;
static gpio_pin_hdl_t spi5_miso_pin;
static gpio_pin_hdl_t spi5_mosi_pin;
static gpio_pin_hdl_t spi5_nss_pin;

void SPI5_init() {
    /* SPI5 SCK GPIO configuration */

    const gpio_pin_conf_t spi5_sck_conf = {
        ._gpioX     = GPIOF,
        ._idx       = 7,
        ._moder     = GPIO_MODER_ALTERNATE,
        ._otyper    = GPIO_OTYPER_OUTPUT_PP,
        ._ospeedr   = GPIO_OSPEEDR_VERY_HIGH,
        ._pupdr     = GPIO_PUPDR_UP,
        ._afr       = 5,
        ._clk       = GPIOF_CLK_EN
    };

    gpio_pin_hdl_ctor(&spi5_sck_pin, &spi5_sck_conf);

    /* SPI5 MISO GPIO configuration */

    const gpio_pin_conf_t spi5_miso_conf = {
        ._gpioX     = GPIOF,
        ._idx       = 8,
        ._moder     = GPIO_MODER_ALTERNATE,
        ._otyper    = GPIO_OTYPER_OUTPUT_PP,
        ._ospeedr   = GPIO_OSPEEDR_VERY_HIGH,
        ._pupdr     = GPIO_PUPDR_UP,
        ._afr       = 5, // AF_SPI5
        ._clk       = GPIOF_CLK_EN
    };

    gpio_pin_hdl_ctor(&spi5_miso_pin, &spi5_miso_conf);

    /* SPI5 MOSI GPIO configuration */

    const gpio_pin_conf_t spi5_mosi_conf = {
        ._gpioX     = GPIOF,
        ._idx       = 9,
        ._moder     = GPIO_MODER_ALTERNATE,
        ._otyper    = GPIO_OTYPER_OUTPUT_PP,
        ._ospeedr   = GPIO_OSPEEDR_VERY_HIGH,
        ._pupdr     = GPIO_PUPDR_UP,
        ._afr       = 5, // AF_SPI5
        ._clk       = GPIOF_CLK_EN
    };

    gpio_pin_hdl_ctor(&spi5_mosi_pin, &spi5_mosi_conf);

    /* SPI5 NSS GPIO configuration */

    const gpio_pin_conf_t spi5_nss_conf = {
        ._gpioX     = GPIOF,
        ._idx       = 6,
        ._moder     = GPIO_MODER_OUTPUT,
        ._otyper    = GPIO_OTYPER_OUTPUT_PP,
        ._ospeedr   = GPIO_OSPEEDR_VERY_HIGH,
        ._pupdr     = GPIO_PUPDR_UP,
        ._clk       = GPIOF_CLK_EN
    };

    gpio_pin_hdl_ctor(&spi5_nss_pin, &spi5_nss_conf);
    // NSS pin must be high not during SPI exchange
    __spi5_nss__(GPIO_PIN_SET);

    /* SPI5 configuration */

    spi_conf_t spi_conf =  {
        ._spiX              = SPI5,
        ._device_mode       = SPI_DEVICE_MODE_MASTER,
        ._bus_mode          = SPI_BUS_MODE_DUPLEX_FULL,
        ._slave_select_mode = SPI_SS_MODE_SOFTWARE,
        ._clk_div           = 0b110,
        ._data_length       = SPI_DATA_LENGTH_8B,
        ._data_order        = SPI_DATA_ORDER_MSB,
        ._cpol              = SPI_CPOL_LOW,
        ._cpha              = SPI_CPHA_LOW,
        ._clk_src           = CLK_SRC_APB2,
        ._clk_en            = RCC_APB2ENR_SPI5EN
    };

    spi_hdl_ctor(&__SPI5__, &spi_conf);
}

void SPI5_deinit()
{ /* TODO */ }

void SPI5_write(char* request, int size)
{
    // Before start spi exchange set NSS pin to low state
    __spi5_nss__(GPIO_PIN_RESET);
    // Send request to sx1276 by spi
    spi_hdl_tx(&__SPI5__, request, size);
    // After end spi exchange set NSS pin to high state
    __spi5_nss__(GPIO_PIN_SET);
}

void SPI5_read(char* request, char* response, int size)
{
    // Before start spi exchange set NSS pin to low state
    __spi5_nss__(GPIO_PIN_RESET);
    // Send request to sx1276 by spi
    spi_hdl_tx_rx(&__SPI5__, request, response, size);
    // After end spi exchange set NSS pin to high state
    __spi5_nss__(GPIO_PIN_SET);
}

void __spi5_nss__(uint8_t status)
{
    // Wait some time before setting nss pin
    // for (int i=0; i<0x03FF; i++)
        __NOP();
    // Set new status of nss pin
    gpio_pin_hdl_write(&spi5_nss_pin, status);
    // Wait some time after setting nss pin
    // for (int i=0; i<0x03FF; i++)Q
        __NOP();
}