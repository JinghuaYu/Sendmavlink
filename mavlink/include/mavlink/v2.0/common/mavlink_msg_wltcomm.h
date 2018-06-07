#pragma once
// MESSAGE WIRELESS TRANSMISSION COMMUNICATION PACKING

#define MAVLINK_MSG_ID_WLTCOMM          16711422 /* 0xFEFEFE */

#define MAVLINK_MSG_ID_WLTCOMM_LEN      255

MAVPACKED(
typedef struct __mavlink_wltcomm_t {
 char data[MAVLINK_MSG_ID_WLTCOMM_LEN]; /*< MAVLink version, not writable by user, gets added by protocol because of magic data type: uint8_t_mavlink_version*/
}) mavlink_wltcomm_t;

#define MAVLINK_MSG_ID_WLTCOMM_MIN_LEN  MAVLINK_MSG_ID_WLTCOMM_LEN
#define MAVLINK_MSG_ID_16711422_LEN     MAVLINK_MSG_ID_WLTCOMM_LEN
#define MAVLINK_MSG_ID_16711422_MIN_LEN MAVLINK_MSG_ID_WLTCOMM_LEN

#define MAVLINK_MSG_ID_WLTCOMM_CRC      123
#define MAVLINK_MSG_ID_16711422_CRC     MAVLINK_MSG_ID_WLTCOMM_CRC

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_WLTCOMM { \
    16711422, \
    "WLTCOMM", \
    1, \
    {   { "data", NULL, MAVLINK_TYPE_CHAR, MAVLINK_MSG_ID_WLTCOMM_LEN, 0, offsetof(mavlink_wltcomm_t, data) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_WLTCOMM { \
    "WLTCOMM", \
    1, \
    {  { "data", NULL, MAVLINK_TYPE_CHAR, MAVLINK_MSG_ID_WLTCOMM_LEN, 0, offsetof(mavlink_wltcomm_t, data) }, \
         } \
}
#endif

/**
 * @brief Pack a wltcomm message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param data A char type pointer to the data array, and its size should not greater than 255 bytes
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_wltcomm_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const char *data)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WLTCOMM_LEN];

    _mav_put_char_array(buf, 0, data, MAVLINK_MSG_ID_WLTCOMM_LEN);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WLTCOMM_LEN);
#else
    mavlink_wltcomm_t packet;

    mav_array_memcpy(packet.data, data, sizeof(char)*MAVLINK_MSG_ID_WLTCOMM_LEN);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WLTCOMM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WLTCOMM;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_WLTCOMM_MIN_LEN, MAVLINK_MSG_ID_WLTCOMM_LEN, MAVLINK_MSG_ID_WLTCOMM_CRC);
}

/**
 * @brief Pack a wltcomm message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param data A char type pointer to the data array, and its size should not greater than 255 bytes
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_wltcomm_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg, const char *data)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WLTCOMM_LEN];
    _mav_put_char_array(buf, 0, data, MAVLINK_MSG_ID_WLTCOMM_LEN);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WLTCOMM_LEN);
#else
    mavlink_wltcomm_t packet;

    mav_array_memcpy(packet.data, data, sizeof(char)*MAVLINK_MSG_ID_WLTCOMM_LEN);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WLTCOMM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WLTCOMM;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_WLTCOMM_MIN_LEN, MAVLINK_MSG_ID_WLTCOMM_LEN, MAVLINK_MSG_ID_WLTCOMM_CRC);
}

/**
 * @brief Encode a wltcomm struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param wltcomm C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_wltcomm_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_wltcomm_t* wltcomm)
{
    return mavlink_msg_wltcomm_pack(system_id, component_id, msg, wltcomm->data);
}

/**
 * @brief Encode a wltcomm struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param wltcomm C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_wltcomm_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_wltcomm_t* wltcomm)
{
    return mavlink_msg_wltcomm_pack_chan(system_id, component_id, chan, msg, wltcomm->data);
}

/**
 * @brief Send a wltcomm message
 * @param chan MAVLink channel to send the message
 *
 * @param data A char type pointer to the data array, and its size should not greater than 255 bytes
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_wltcomm_send(mavlink_channel_t chan, const char *data)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HEARTBEAT_LEN];

    _mav_put_char_array(buf, 0, data, MAVLINK_MSG_ID_WLTCOMM_LEN);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WLTCOMM, buf, MAVLINK_MSG_ID_WLTCOMM_MIN_LEN, MAVLINK_MSG_ID_WLTCOMM_LEN, MAVLINK_MSG_ID_WLTCOMM_CRC);
#else
    mavlink_wltcomm_t packet;

    mav_array_memcpy(packet.key, data, sizeof(char)*MAVLINK_MSG_ID_WLTCOMM_LEN);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WLTCOMM, (const char *)&packet, MAVLINK_MSG_ID_WLTCOMM_MIN_LEN, MAVLINK_MSG_ID_WLTCOMM_LEN, MAVLINK_MSG_ID_WLTCOMM_CRC);
#endif
}

/**
 * @brief Send a wltcomm message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_wltcomm_send_struct(mavlink_channel_t chan, const mavlink_wltcomm_t* wltcomm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_wltcomm_send(chan, wltcomm->data);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WLTCOMM, (const char *)wltcomm, MAVLINK_MSG_ID_WLTCOMM_MIN_LEN, MAVLINK_MSG_ID_WLTCOMM_LEN, MAVLINK_MSG_ID_WLTCOMM_CRC);
#endif
}

#if MAVLINK_MSG_ID_WLTCOMM_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_wltcomm_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char* data)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;

    _mav_put_char_array(buf, 0, data, MAVLINK_MSG_ID_WLTCOMM_LEN);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WLTCOMM, buf, MAVLINK_MSG_ID_WLTCOMM_MIN_LEN, MAVLINK_MSG_ID_WLTCOMM_LEN, MAVLINK_MSG_ID_WLTCOMM_CRC);
#else
    mavlink_wltcomm_t *packet = (mavlink_wltcomm_t *)msgbuf;

    mav_array_memcpy(packet->key, data, sizeof(char)*MAVLINK_MSG_ID_WLTCOMM_LEN);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WLTCOMM, (const char *)packet, MAVLINK_MSG_ID_WLTCOMM_MIN_LEN, MAVLINK_MSG_ID_WLTCOMM_LEN, MAVLINK_MSG_ID_WLTCOMM_CRC);
#endif
}
#endif

#endif

// MESSAGE WLTCOMM UNPACKING


/**
 * @brief Get field type from wltcomm message
 *
 * @return data A char type pointer to a data array, and its size should not lower than 255 bytes
 */
static inline uint16_t mavlink_msg_wltcomm_get_data(const mavlink_message_t* msg, char *data)
{
    return _MAV_RETURN_char_array(msg, data, MAVLINK_MSG_ID_WLTCOMM_LEN, 0);
}

/**
 * @brief Decode a wltcomm message into a struct
 *
 * @param msg The message to decode
 * @param wltcomm C-struct to decode the message contents into
 */
static inline void mavlink_msg_wltcomm_decode(const mavlink_message_t* msg, mavlink_wltcomm_t* wltcomm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_wltcomm_get_data(msg, wltcomm->data);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_WLTCOMM_LEN? msg->len : MAVLINK_MSG_ID_WLTCOMM_LEN;
        memset(wltcomm, 0, MAVLINK_MSG_ID_WLTCOMM_LEN);
    memcpy(wltcomm, _MAV_PAYLOAD(msg), len);
#endif
}
