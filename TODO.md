* WP1
    * BLE
        * Add bytes to MAC string as utility BLE function
        * Create ble_common to create shared dependencies between client and server
        * Add compiler flag to set current build as client or server
        * Enable ble_scan only if current device is client
        * Add notify channel on server with dummy periodic data
        * Connect 1 client to 1 server and subscribe to the latter notify channel
        * Measure RSSI continuously and benchmark distance estimation
        * Implement ToF PING and benchmark distance estimation
        * Clean and refactor code before BLE branch merge
            * Include organization (h/c and order)
            * Add TAG to all c files and replace printf with ESP_LOG
            * Clean imports
            * Refactor CMakeLists files following ESP build documentation