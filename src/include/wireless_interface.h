#define WLS_CHANNEL_MAX_LEN           8
#define WLS_BANDWIDTH_MAX_LENGTH      8
#define WLS_DATARATE_MAX_LENGTH       8
#define WLS_DATARATE_MAX_LENGTH       8
#define WLS_SUPPORTED_CHANNEL_MAX_LEN 120

#define WLS_SSID_MAX_LENGTH     32
#define WLS_PASSWORD_MAX_LENGTH 8

#define WLS_READNETWORK     "WLS_READNETWORK"
#define WLS_SCAN            "WLS_SCAN"
#define WLS_CONNECT         "WLS_CONNECT"
#define WLS_DISCONNECT      "WLS_DISCONNECT"
#define WLS_SELECTNETID     "WLS_SELECTNETID"
#define WLS_CONNSTATUS      "WLS_CONNSTATUS"
#define WLS_CHANSTATUS      "WLS_CHANSTATUS"
#define WLS_SETFRQCHAN      "WLS_SETFRQCHAN"
#define WLS_SETBANDWIDTH    "WLS_SETBANDWIDTH"
#define WLS_SETDATARATE     "WLS_SETDATARATE"

typedef struct
{
    uint8_t ssid[WLS_SSID_MAX_LENGTH];
    uint8_t password[WLS_PASSWORD_MAX_LENGTH];
} wireless_connection_t;

typedef struct
{
    uint8_t supported_channel[120];
} wireless_channel_status_t;
