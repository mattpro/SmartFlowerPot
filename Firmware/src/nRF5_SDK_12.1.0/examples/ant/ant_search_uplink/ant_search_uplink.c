/*
This software is subject to the license described in the license.txt file
included with this software distribution. You may not use this file except in compliance
with this license.

Copyright (c) Dynastream Innovations Inc. 2015
All rights reserved.
*/

#include "ant_search_uplink.h"
#include <stdint.h>
#include "ant_interface.h"
#include "ant_parameters.h"
#include "app_error.h"
#include "app_util.h"
#include "boards.h"
#include "sdk_config.h"
#include "ant_channel_config.h"
#include "ant_search_config.h"
#include "nrf_soc.h"

#define WILDCARD_DEVICE_NUMBER          0x00                    /**< Wildcard device number. */
// Miscellaneous defines.
#define ANT_CHANNEL_DEFAULT_NETWORK     0x00                    /**< ANT Channel Network. */
#define ANT_CHANNEL_NUMBER              0x00                    /**< ANT Channel Number. */

// Operation modes
static enum
{
    MODE_NONE,                                                  /**< Do not send any uplink messages. */
    MODE_SEND_UPLINK_ANY,                                       /**< Send uplink messages to any devices. */
    MODE_SEND_UPLINK_SPECIFIC,                                  /**< Send a single uplink message to a specific device. */
} m_mode;

static uint8_t m_tx_buffer[ANT_STANDARD_DATA_PAYLOAD_SIZE];     /**< Transmit buffer. */
static uint8_t m_counter;                                       /**< Counter to include in transmit data. */
static bool    m_sent;


/**@brief Send uplink message
 * @param[in] device_number Device number of intended destination of message. Set to WILDCARD_DEVICE_NUMBER for any node.
 */
static void uplink_message_send(uint16_t device_number)
{
    uint8_t err_code;

    m_tx_buffer[0] = 0xFF;
    m_tx_buffer[1] = 0xFF;
    m_tx_buffer[2] = 0xFF;
    m_tx_buffer[3] = 0xFF;
    m_tx_buffer[4] = 0xFF;
    m_tx_buffer[5] = 0xFF;
    m_tx_buffer[6] = 0xFF;
    m_tx_buffer[7] = m_counter;

    if (device_number != WILDCARD_DEVICE_NUMBER)
    {
        // Set the channel ID of the specific device to send message to
        err_code = sd_ant_channel_id_set(ANT_CHANNEL_NUMBER,
                                         device_number,
                                         CHAN_ID_DEV_TYPE,
                                         CHAN_ID_TRANS_TYPE);
        APP_ERROR_CHECK(err_code);
    }

    err_code = sd_ant_broadcast_message_tx(ANT_CHANNEL_NUMBER,
                                           ANT_STANDARD_DATA_PAYLOAD_SIZE,
                                           m_tx_buffer);
    APP_ERROR_CHECK(err_code);

    m_counter++;
}


void ant_search_uplink_setup(void)
{
    uint32_t err_code;

    m_mode    = MODE_NONE;
    m_counter = 0;

    ant_channel_config_t channel_config =
    {
        .channel_number    = ANT_CHANNEL_NUMBER,
        .channel_type      = CHANNEL_TYPE_SLAVE,
        .ext_assign        = EXT_PARAM_ALWAYS_SEARCH,
        .rf_freq           = RF_FREQ,
        .transmission_type = CHAN_ID_TRANS_TYPE,
        .device_type       = CHAN_ID_DEV_TYPE,
        .device_number     = WILDCARD_DEVICE_NUMBER,
        .network_number    = ANT_CHANNEL_DEFAULT_NETWORK,
    };

    ant_search_config_t search_config = DEFAULT_ANT_SEARCH_CONFIG(ANT_CHANNEL_NUMBER);

    // Keep searching always
    search_config.low_priority_timeout = ANT_LOW_PRIORITY_TIMEOUT_DISABLE;

    // Disable high priority search to minimize disruption to other channels while searching
    search_config.high_priority_timeout = ANT_HIGH_PRIORITY_SEARCH_DISABLE;

    // Configure the channel
    err_code = ant_channel_init(&channel_config);
    APP_ERROR_CHECK(err_code);

    // Configure search parameters
    err_code = ant_search_init(&search_config);
    APP_ERROR_CHECK(err_code);

    // Enable extended rx messages, including the channel ID
    err_code = sd_ant_lib_config_set(ANT_LIB_CONFIG_MESG_OUT_INC_DEVICE_ID);
    APP_ERROR_CHECK(err_code);

    // Open background scanning channel
    err_code = sd_ant_channel_open(ANT_CHANNEL_NUMBER);
    APP_ERROR_CHECK(err_code);
}


void ant_search_uplink_bsp_evt_handler(bsp_event_t evt)
{
    switch (evt)
    {
        case BSP_EVENT_KEY_0:
            m_mode = MODE_SEND_UPLINK_ANY;
            uplink_message_send(WILDCARD_DEVICE_NUMBER);
            LEDS_OFF(BSP_LED_1_MASK);
            break;

        case BSP_EVENT_KEY_1:
            m_mode = MODE_SEND_UPLINK_SPECIFIC;
            m_sent = false;
            LEDS_OFF(BSP_LED_1_MASK);
            break;

        default:
            break; // No implementation needed
    }
}


void ant_search_uplink_event_handler(ant_evt_t * p_ant_evt)
{
    switch (p_ant_evt->event)
    {
        case EVENT_TX:
            LEDS_INVERT(BSP_LED_1_MASK);
            if (m_mode == MODE_SEND_UPLINK_ANY)
            {
                uplink_message_send(WILDCARD_DEVICE_NUMBER);
            }
            else if (m_mode == MODE_SEND_UPLINK_SPECIFIC)
            {
                // Reset the channel ID to wild card to continue receiving messages from any device
                uint32_t err_code = sd_ant_channel_id_set(ANT_CHANNEL_NUMBER,
                                                          WILDCARD_DEVICE_NUMBER,
                                                          CHAN_ID_DEV_TYPE,
                                                          CHAN_ID_TRANS_TYPE);
                APP_ERROR_CHECK(err_code);
            }
            break;

        case EVENT_RX:
            LEDS_INVERT(BSP_LED_0_MASK);
            if (m_mode == MODE_SEND_UPLINK_SPECIFIC && !m_sent)
            {
                ANT_MESSAGE * p_message = (ANT_MESSAGE *) p_ant_evt->msg.evt_buffer;
                // Decode extended data to get channel ID of the received message and send message to that specific device
                if (p_message->ANT_MESSAGE_stExtMesgBF.bANTDeviceID)
                {
                    uint16_t device_number = uint16_decode(p_message->ANT_MESSAGE_aucExtData);
                    uplink_message_send(device_number);
                    m_sent = true;
                }
            }
            break;

        default:
            break; // No implementation needed
    }
}


