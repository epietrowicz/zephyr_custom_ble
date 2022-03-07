#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/printk.h>
#include <sys/byteorder.h>
#include <zephyr.h>
#include <soc.h>
#include <drivers/led_strip.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/addr.h>
#include <bluetooth/gatt.h>

#include "led_service.h"

#define BT_UUID_MY_SERVICE BT_UUID_DECLARE_128(LED_SERVICE_UUID)
#define BT_UUID_MY_SERVICE_RX BT_UUID_DECLARE_128(RX_CHARACTERISTIC_UUID)

#define MAX_TRANSMIT_SIZE 240

struct led_rgb led_data[25];

bool fresh_data = false;
bool party_mode = false;

uint8_t data_rx[MAX_TRANSMIT_SIZE];

// Public functions:
int led_service_init(void)
{
    int err = 0;

    memset(&data_rx, 0, MAX_TRANSMIT_SIZE);
    memset(&led_data, 0x00, sizeof(led_data));

    return err;
}

// Return the latest LED array
struct led_rgb *get_led_data(void)
{
    return led_data;
}

// Indicate to the main function that the LED array has changed
bool is_fresh_data(void)
{
    return fresh_data;
}

// Who's trying to party?
bool is_partying_time(void)
{
    return party_mode;
}

// Did we make our update?
void new_data_serviced(void)
{
    fresh_data = false;
}

/* This function is called whenever the RX Characteristic has been written to by a Client */
static ssize_t on_receive(struct bt_conn *conn,
                          const struct bt_gatt_attr *attr,
                          const void *buf,
                          uint16_t len,
                          uint16_t offset,
                          uint8_t flags)
{
    // Data coming in off the BLE write
    const uint8_t *buffer = buf;

    printk("Received data, handle %d, conn %p, data: 0x", attr->handle, conn);
    for (uint8_t i = 0; i < len; i++)
    {
        printk("%02X", buffer[i]);
    }
    printk("\n");

    // 0x01 ff 00 ff -- example write format
    uint8_t led_index = buffer[0];

    // Did we send a party command?
    if (led_index == 255)
    {
        party_mode = true;
    }

    // If not, pull in the data for the LED index
    struct led_rgb color;
    color.r = buffer[1];
    color.g = buffer[2];
    color.b = buffer[3];

    // Stuff it in our global array
    led_data[led_index] = color;

    printk("led number %d was set with r: %d, g: %d, b: %d \n", led_index, color.r, color.g, color.b);

    fresh_data = true;

    return len;
}

/* This function is called whenever the CCCD register has been changed by the client*/
static void on_cccd_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);
    switch (value)
    {
    case BT_GATT_CCC_NOTIFY:
        // Start sending stuff!
        break;

    case BT_GATT_CCC_INDICATE:
        // Start sending stuff via indications
        break;

    case 0:
        // Stop sending stuff
        break;

    default:
        printk("Error, CCCD has been set to an invalid value");
    }
}

/* LED Service Declaration and Registration */
BT_GATT_SERVICE_DEFINE(led_service,
                       BT_GATT_PRIMARY_SERVICE(BT_UUID_MY_SERVICE),
                       BT_GATT_CHARACTERISTIC(BT_UUID_MY_SERVICE_RX,
                                              BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                                              BT_GATT_PERM_READ_ENCRYPT | BT_GATT_PERM_WRITE_ENCRYPT,
                                              NULL, on_receive, NULL),
                       BT_GATT_CCC(on_cccd_changed,
                                   BT_GATT_PERM_READ_ENCRYPT | BT_GATT_PERM_WRITE_ENCRYPT), );