#ifndef FRAMES_H
#define FRAMES_H


#include <stdint.h>
#include <stdbool.h>

#define SIZE_CERCED_DATA_U8 ((uint8_t)27)
#define SIZE_NOT_CERCED_DATA_U8 ((uint8_t)5)
#define SIZE_PAYLOAD_U8 ((uint8_t)23)
#define SIZE_TRAME_U8 ((uint8_t)32)

typedef union 
{
    struct trame_str
    {
        union cerced_data_UN
        {
            struct cerced_data_str
            {
                uint8_t protocol_version_U7 : 7;
                uint8_t ack_needed_B;
                uint8_t source_address_U8;
                uint8_t destination_address_U8;
                uint8_t trame_type_U8;
                uint8_t payload_U8A[SIZE_PAYLOAD_U8];
            }cerced_data_str;
            uint8_t cerced_data_U32[SIZE_CERCED_DATA_U8];
        };
        union not_cerced_data_UN
        {
            struct not_cerced_data_str
            {
                uint8_t nb_nodes_traverses_U8;
                uint32_t CRC_ID_U32;
            }not_cerced_data_str;
            uint8_t not_cerced_data_U32[SIZE_NOT_CERCED_DATA_U8];
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








#endif // FRAMES_H