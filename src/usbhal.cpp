#include "usbhal.h"

UsbHal::UsbHal() :
        udi(NULL),
        state(InitFailed)
{
    dbus_error_init(&error);

    // Connect to system bus
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
    if (dbus_error_is_set(&error)) {
        errorMsg = error.message;
        dbus_error_free(&error);
    }

    // Create the HAL context
    ctx = libhal_ctx_new();
    if (!ctx) {
        errorMsg = "libhal_ctx_new() failed";
        return;
    }

    // Set D-Bus connection
    if (!libhal_ctx_set_dbus_connection(ctx, conn)) {
        errorMsg = error.message;
        dbus_error_free(&error);
        return;
    }

    // Initialise HAL context
    if (!libhal_ctx_init(ctx, &error)) {
        errorMsg = error.message;
        return;
    }

    // Get the USB cable identifier
    state = initUdi();
}

UsbHal::~UsbHal()
{
    if (udi) {
        delete[] udi;
    }

    if (ctx) {
        libhal_ctx_free(ctx);
    }
}

UsbHal::State UsbHal::getCableState()
{
    if (!udi || state == InitFailed) {
        return state;
    }

    dbus_error_init(&error);

    char* prop = libhal_device_get_property_string(ctx, udi, "usb_device.mode", &error);
    if (dbus_error_is_set(&error)) {
        errorMsg.sprintf("D-Bus error: %s", error.message);
        dbus_error_free(&error);
    }

    if (prop == NULL) {
        errorMsg.sprintf("Couldn't read 'usb_device.mode' from %s", udi);
        state = Invalid;
    } else if (strcmp(prop, "b_peripheral") == 0 || strcmp(prop, "a_peripheral") == 0) {
        state = PeripheralWait;
    } else if (strcmp(prop, "a_host") == 0 || strcmp(prop, "b_host") == 0) {
        state = Host;
    } else if (strcmp(prop, "b_idle") == 0 || strcmp(prop, "a_idle") == 0) {
        state = CableDetached;
    } else if (strcmp(prop, "UNDEFINED") == 0) {
        errorMsg == "'usb_device.mode' is UNDEFINED, not changing the state";
    } else {
        errorMsg.sprintf("Unknown USB cable type: %s", prop);
        state = CableDetached;
    }

    if (prop) {
        libhal_free_string(prop);
    }

    errorMsg = "";
    return state;

}

UsbHal::State UsbHal::initUdi()
{
    State ret = Invalid;
    char **list;
    int num_devices = 0;

    list = libhal_manager_find_device_string_match(ctx, "button.type", "usb.cable", &num_devices, &error);
    if (list != NULL && num_devices > 0) {
        udi = new char[strlen(list[0]) + 1];
        strcpy(udi, list[0]);
    } else {
        errorMsg = "Couldn't find USB cable identifier";
        ret = InitFailed;
    }
    libhal_free_string_array(list);

    return ret;
}
