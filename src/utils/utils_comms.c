#include "utils_comms.h"
#include <stdio.h>

static const char *TAG = "UTILS_COMMS";

/**
 * @brief Convert an array of 6 bytes to a MAC address string.
 *
 * @param mac_bytes Array of 6 bytes representing the MAC address.
 * @param out_str Output buffer (should be at least 18 bytes).
 */
void mac_bytes_to_str(const uint8_t mac_bytes[6], char *out_str) {
    sprintf(out_str, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac_bytes[0], mac_bytes[1], mac_bytes[2],
            mac_bytes[3], mac_bytes[4], mac_bytes[5]);
}