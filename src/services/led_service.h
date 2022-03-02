#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr.h>
#include <soc.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#define LED_SERVICE_UUID 0xd4, 0x86, 0x48, 0x24, 0x54, 0xB3, 0x43, 0xA1, \
			            0xBC, 0x20, 0x97, 0x8F, 0xC3, 0x76, 0xC2, 0x75

#define RX_CHARACTERISTIC_UUID  0xA6, 0xE8, 0xC4, 0x60, 0x7E, 0xAA, 0x41, 0x6B, \
			                    0x95, 0xD4, 0x9D, 0xCC, 0x08, 0x4F, 0xCF, 0x6A


/** @brief Callback type for when new data is received. */
typedef void (*data_rx_cb_t)(uint8_t *data, uint8_t length);

/** @brief Callback struct used by the led_service Service. */
struct led_service_cb 
{
	/** Data received callback. */
	data_rx_cb_t    data_rx_cb;
};

// Public functions:
int led_service_init(void);
struct led_rgb* get_led_data(void);
bool is_fresh_data(void);
void new_data_serviced(void);
bool is_partying_time(void);