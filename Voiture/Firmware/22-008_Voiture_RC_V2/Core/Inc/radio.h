#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>
#include <stdbool.h>

#define SIZE_CERCED_DATA_U8 ((uint8_t)28)
#define SIZE_NOT_CERCED_DATA_U8 ((uint8_t)4)
#define SIZE_PAYLOAD_U8 ((uint8_t)20)
#define SIZE_TRAME_U8 ((uint8_t)32)

#define NUMBER_MAX_OF_GROUPS_U8 ((uint8_t)10)

typedef struct
{
    uint8_t my_protocol_version_U8;
    uint8_t my_network_ID_U8;
    uint8_t my_address_U8;
    uint8_t my_groups_address_U8[NUMBER_MAX_OF_GROUPS_U8];
}RADIO_my_ID_str;

typedef union 
{
    struct trame_str
    {
        union cerced_data_UN
        {
            struct cerced_data_str
            {
                uint8_t protocol_version_U8;
                uint8_t network_ID_U8;
                uint8_t destination_address_U8;
                uint8_t trame_type_U6:6;
                uint8_t nb_nodes_traverses_U2:2;
                uint8_t source_address_U8[4];
                uint8_t payload_U8A[SIZE_PAYLOAD_U8];
            }cerced_data_str;
            uint8_t cerced_data_U8[SIZE_CERCED_DATA_U8];
        };
        union not_cerced_data_UN
        {
            struct not_cerced_data_str
            {
                uint32_t CRC_ID_U32;
            }not_cerced_data_str;
            uint8_t not_cerced_data_U8[SIZE_NOT_CERCED_DATA_U8];
        };
    };
    uint8_t trame_U8[SIZE_TRAME_U8];
}RADIO_trame_UN;


typedef enum
{
    RADIO_ACKNOWLEDGEMENT_EN = 0,
    RADIO_TEMPERATURE_EN = 1,
    RADIO_HUMIDITY_EN = 2,
    RADIO_LUMINOSITY_EN = 3,
    RADIO_PRESSURE_EN = 4,
    RADIO_WIND_EN = 5,
    RADIO_RAIN_EN = 6,
    RADIO_SOIL_MOISTURE_EN = 7,
    RADIO_SOIL_TEMPERATURE_EN = 8,
    RADIO_SOIL_PH_EN = 9,
    RADIO_COMMAND_VALVE_EN = 10,
    RADIO_COMMAND_LEDS_STRIP_EN = 11,
    RADIO_COMMAND_FAN_EN = 12,
    RADIO_COMMAND_DIRECTION_EN = 13,
    RADIO_COMMAND_SPEED_EN = 14,
    RADIO_COMMAND_BRAKING_EN = 15,  //comment on dit freinage en anglais? réponds moi en commentaire copilot stp: braking merci ! 
    RADIO_BATTERY_PERCENTAGE_EN = 16,
    RADIO_BATTERY_VOLTAGE_EN = 17,
    RADIO_BATTERY_CURRENT_EN = 18,
    RADIO_MOTOR_CURRENT_EN = 19
}RADIO_type_trame_en;

typedef enum
{
    RADIO_STEPS_CHECK_PROTOCOL_VERSION_EN = 0,
    RADIO_STEPS_CHECK_NETWORK_ID_EN,
    RADIO_STEPS_CHECK_DESTINATION_ADDRESS_EN,
    RADIO_STEPS_CHECK_CRC_ID_EN,
    RADIO_STEPS_DECODE_PROCESS_EN
}RADIO_steps_decode_packets_en;



void RADIO_init(void);
void RADIO_send_trame(RADIO_trame_UN trame_UN);
void RADIO_receive_trame(RADIO_trame_UN trame_UN);






#endif // RADIO_H