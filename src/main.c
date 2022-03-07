#include <errno.h>
#include <string.h>

#include <zephyr.h>
#include <sys/util.h>

#include <zephyr/types.h>
#include <stddef.h>
#include <sys/printk.h>
#include <sys/byteorder.h>
#include <drivers/gpio.h>
#include <soc.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include <settings/settings.h>

#include "services/led_service.h"

// Pull in some properties from the .prj file
// Plus various settings
#define DELAY_TIME K_MSEC(50)

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

// A necessary definition for BLE stack
static K_SEM_DEFINE(ble_init_ok, 0, 1);

// Define the connection we'll be establishing
struct bt_conn *my_connection;

// Array containing the advertising data
static const struct bt_data ad[] =
	{
		BT_DATA_BYTES(BT_DATA_UUID128_ALL, LED_SERVICE_UUID),
		BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

// Array containing the scan response data
static const struct bt_data sd[] =
	{
		BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

// What happens when we connect to the device
static void connected(struct bt_conn *conn, uint8_t err)
{
	struct bt_conn_info info;
	char addr[BT_ADDR_LE_STR_LEN];

	my_connection = conn;

	if (err)
	{
		printk("Connection failed (err %u)\n", err);
		return;
	}
	else if (bt_conn_get_info(conn, &info))
	{
		printk("Could not parse connection info\n");
	}
	else
	{
		bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

		printk("Connection established!		\n\
		Connected to: %s					\n\
		Role: %u							\n\
		Connection interval: %u				\n\
		Slave latency: %u					\n\
		Connection supervisory timeout: %u	\n",
			   addr, info.role, info.le.interval, info.le.latency, info.le.timeout);
	}
}

// What happens when the device is disconnected
static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason %u)\n", reason);
}

static void security_changed(struct bt_conn *conn, bt_security_t level,
							 enum bt_security_err err)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (!err)
	{
		printk("Security changed: level %u\n", level);
	}
	else
	{
		printk("Security failed: level %u err %d\n", level, err);
	}
}

// Callback for BLE update request
static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param)
{
	// If acceptable params, return true, otherwise return false.
	return true;
}

// Callback for BLE parameter update
static void le_param_updated(struct bt_conn *conn, uint16_t interval, uint16_t latency, uint16_t timeout)
{
	struct bt_conn_info info;
	char addr[BT_ADDR_LE_STR_LEN];

	if (bt_conn_get_info(conn, &info))
	{
		printk("Could not parse connection info\n");
	}
	else
	{
		bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

		printk("Connection parameters updated!	\n\
		Connected to: %s						\n\
		New Connection Interval: %u				\n\
		New Slave Latency: %u					\n\
		New Connection Supervisory Timeout: %u	\n",
			   addr, info.le.interval, info.le.latency, info.le.timeout);
	}
}

// Wire up all the callbacks we defined above to the bt struct
static struct bt_conn_cb conn_callbacks =
	{
		.connected = connected,
		.disconnected = disconnected,
		.security_changed = security_changed,
		.le_param_req = le_param_req,
		.le_param_updated = le_param_updated,
};

static void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Pairing cancelled\n");
}

static void pairing_complete(struct bt_conn *conn, bool bonded)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Pairing completed\n");
}

static void pairing_failed(struct bt_conn *conn, enum bt_security_err reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Pairing failed, reason %d\n", reason);
}

static struct bt_conn_auth_cb conn_auth_callbacks = {
	.passkey_display = NULL,
	.passkey_confirm = NULL,
	.cancel = auth_cancel,
	.pairing_confirm = NULL,
	.pairing_complete = pairing_complete,
	.pairing_failed = pairing_failed};

// This will be called to initalize our bluetooth stack
static void bt_ready(int err)
{
	if (err)
	{
		printk("BLE init failed with error code %d\n", err);
		return;
	}

	// Configure connection callbacks
	bt_conn_cb_register(&conn_callbacks);
	printk("Security Enabled\n");
	bt_conn_auth_cb_register(&conn_auth_callbacks);

	settings_load();

	// Initalize services
	err = led_service_init();

	if (err)
	{
		printk("Failed to init lightbox (err:%d)\n", err);
		return;
	}

	// Start advertising
	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad),
						  sd, ARRAY_SIZE(sd));
	if (err)
	{
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Advertising successfully started\n");

	k_sem_give(&ble_init_ok);
}

static void error(void)
{
	while (true)
	{
		printk("Error!\n");
		k_sleep(K_MSEC(1000));
	}
}

void main(void)
{
	int err = 0;

	printk("Starting Nordic Lightbox\n");

	// Configure connection callbacks
	bt_conn_cb_register(&conn_callbacks);
	// Configure security callbacks
	err = bt_conn_auth_cb_register(&conn_auth_callbacks);

	if (err)
	{
		printk("Failed to initalize auth callbacks\n");
	}

	err = bt_enable(NULL);
	if (err)
	{
		printk("Failed to enable BT\n");
	}

	k_sem_give(&ble_init_ok);

	if (IS_ENABLED(CONFIG_SETTINGS))
	{
		err = settings_load();
		if (!err)
		{
			printk("Settings loaded!\n");
		}
	}

	// Initalize services
	err = led_service_init();

	if (err)
	{
		printk("Failed to init lightbox (err:%d)\n", err);
		return;
	}

	// Start advertising
	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad),
						  sd, ARRAY_SIZE(sd));
	if (err)
	{
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Advertising successfully started\n");

	if (err)
	{
		printk("BLE initialization failed\n");
		error(); // Catch error
	}

	/* 	Bluetooth stack should be ready in less than 100 msec. 								\
																							\
		We use this semaphore to wait for bt_enable to call bt_ready before we proceed 		\
		to the main loop. By using the semaphore to block execution we allow the RTOS to 	\
		execute other tasks while we wait. */
	err = k_sem_take(&ble_init_ok, K_MSEC(500));

	if (!err)
	{
		printk("Bluetooth initialized\n");
	}
	else
	{
		printk("BLE initialization did not complete in time\n");
		error(); // Catch error
	}

	int ret = bt_unpair(BT_ID_DEFAULT, BT_ADDR_LE_ANY);
	printk("Attempting to unpair device %d\n", ret);
}