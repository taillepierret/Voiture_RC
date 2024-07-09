#include "../Inc/radio.h"
#include "../Inc/tools.h"
#include "../Inc/NRF24L01.h"

#define SIZE_BUFFER_U16 100
#define NUMBER_OF_RETRY_MAX_U8 3
#define TIMEOUT_ACKNOWLEDGE_ms_U32 1000
#define NUMBER_PACKET_TO_LOAD_U8  ((UINT8) 10)



typedef enum
{
    RADIO_SEND_TRANSITION_TO_BEGIN_EN,
    RADIO_SEND_BEGIN_EN,
    RADIO_SEND_WAITING_ACK_EN,
    RADIO_SEND_SENDING_PACKET_EN
}RADIO_state_send_packet_en;

typedef enum
{
    RADIO_RECEIVE_TRANSITION_TO_WAITING_FOR_RECEIVE_PACKET_EN,
    RADIO_RECEIVE_WAITING_FOR_RECEIVE_PACKET_EN,
    RADIO_RECEIVE_PROCESSING_EN
}RADIO_state_receive_packet_en;


uint16_t curseur_ecriture_U16 = 0;
uint16_t curseur_lecture_U16 = 0;

bool waiting_acknowledge_B = false;
bool acknoledge_received_B = false;

uint32_t packet_ID_to_acknowledge_U32 = 0;

uint16_t error_counter_U16 = 0;

uint8_t retry_counter_U8 = 0;

uint32_t beginnning_send_time_U32 = 0;

RADIO_state_send_packet_en RADIO_state_send_packet_EN = RADIO_SEND_TRANSITION_TO_BEGIN_EN;
RADIO_state_receive_packet_en RADIO_state_receive_packet_EN = RADIO_RECEIVE_TRANSITION_TO_BEGIN_EN;

uint8_t buffer_U8[SIZE_BUFFER_U16];

RADIO_trame_UN List_of_packet_to_send_ENA[NUMBER_PACKET_TO_LOAD_U8];

RADIO_my_ID_str my_ID_STR = {0, 0, 0};

void RADIO_init(void)
{
    curseur_ecriture_U16 = 0;
    curseur_lecture_U16 = 0;
    retry_counter_U8 = 0;
    beginnning_send_time_U32 = 0;
    waiting_acknowledge_B = false;
    acknoledge_received_B = false;
    NRF24_Init_EN(); //TODO
}

void RADIO_SendStateMachine(void)
{
    HAL_ret_val_en HAL_ret_val_EN;
    switch(RADIO_state_send_packet_EN)
    {
        case RADIO_SEND_TRANSITION_TO_BEGIN_EN:
            retry_counter_U8 = 0;
            beginnning_send_time_U32 = 0;
            waiting_acknowledge_B = false;
            acknoledge_received_B = false;
            packet_ID_to_acknowledge_U32 = 0;
            RADIO_state_send_packet_EN = RADIO_SEND_BEGIN_EN;
            break;

        case RADIO_SEND_BEGIN_EN:
            if(curseur_ecriture_U16 != curseur_lecture_U16)
            {
                RADIO_state_send_packet_EN = RADIO_SEND_SENDING_PACKET_EN;
            }
            else
            {
                //si pas de trame à envoyer alors on reste dans cette tache
                RADIO_state_send_packet_EN = RADIO_SEND_BEGIN_EN;
            }
            break;
        case RADIO_SEND_SENDING_PACKET_EN:
            if(retry_counter_U8 < NUMBER_OF_RETRY_MAX_U8)
            {
                //TODO Send packet
                HAL_ret_val_EN = NRF24_Transmit_EN(List_of_packet_to_send_ENA[curseur_lecture_U16].trame_U8, SIZE_TRAME_U8)
                if(HAL_ret_val_EN == NRF_OK_EN)
                {
                    packet_ID_to_acknowledge_U32 = List_of_packet_to_send_ENA[curseur_lecture_U16].cerced_data_UN.cerced_data_str.CRC_ID_U32;
                    beginnning_send_time_U32 = TOOLS_millis_U32();
                    RADIO_state_send_packet_EN = RADIO_SEND_WAITING_ACK_EN;
                }
                else
                {
                    //logguer la trame d'erreur
                    error_counter_U16++;
                    RADIO_state_send_packet_EN = RADIO_SEND_TRANSITION_TO_BEGIN_EN;
                }
            }
            else
            {
                //logguer la trame d'erreur
                error_counter_U16++;
                RADIO_state_send_packet_EN = RADIO_SEND_TRANSITION_TO_BEGIN_EN;
            }
            break;
        case RADIO_SEND_WAITING_ACK_EN:
            if(acknoledge_received_B == true)
            {
                curseur_lecture_U16++;
                if(curseur_lecture_U16 >= SIZE_BUFFER_U16)
                {
                    curseur_lecture_U16 = 0;
                }
                RADIO_state_send_packet_EN = RADIO_SEND_TRANSITION_TO_BEGIN_EN;
            }
            else if(TOOLS_millis_U32() - beginnning_send_time_U32 > TIMEOUT_ACKNOWLEDGE_ms_U32)
            {
                retry_counter_U8++;
                beginnning_send_time_U32 = 0;
                RADIO_state_send_packet_EN = RADIO_SEND_SENDING_PACKET_EN;
            }
            else
            {
                //on reste dans cette tache tant que l'on a pas reçu l'acknowledge ou que le timeout n'est pas atteint
                RADIO_state_send_packet_EN = RADIO_SEND_WAITING_ACK_EN;
            }
            break;
        default:
            break;
    }
}

void RADIO_setAcknowledgeReceivedFlag(bool value_B)
{
    if(value_B == true && acknoledge_received_B == true)
    {
        //logguer l'erreur
        error_counter_U16++;
        return;
    }
    acknoledge_received_B = value_B;
}


void RADIO_addPacketToSend(RADIO_trame_UN packet_to_send_EN)
{
    List_of_packet_to_send_ENA[curseur_ecriture_U16] = packet_to_send_EN;
    curseur_ecriture_U16++;
    if(curseur_ecriture_U16 >= SIZE_BUFFER_U16)
    {
        curseur_ecriture_U16 = 0;
    }

    if(curseur_ecriture_U16 == curseur_lecture_U16)
    {
        //logguer l'erreur
        error_counter_U16++;
    }
}

void RADIO_ReceiveStateMachine(void)
{
    NRF_ret_val_en NRF_ret_val_EN;
    RADIO_trame_UN received_packet_EN;
    switch(RADIO_state_receive_packet_EN)
    {
        case RADIO_RECEIVE_TRANSITION_TO_WAITING_FOR_RECEIVE_PACKET_EN:
            RADIO_state_receive_packet_EN = RADIO_RECEIVE_WAITING_FOR_RECEIVE_PACKET_EN;
            break;
        case RADIO_RECEIVE_WAITING_FOR_RECEIVE_PACKET_EN:
            NRF_ret_val_EN = NRF24_isDataAvailable_EN(0);
            if(NRF_ret_val_EN == NRF_DATA_AVAILABLE_EN)
            {
                RADIO_state_receive_packet_EN = RADIO_RECEIVE_PROCESSING_EN;
            }
            else
            {
                RADIO_state_receive_packet_EN = RADIO_RECEIVE_WAITING_FOR_RECEIVE_PACKET_EN;
            }
            break;
        case RADIO_RECEIVE_PROCESSING_EN:
            NRF_ret_val_EN = NRF24_Receive_EN(received_packet_EN.trame_U8);
            if(NRF_ret_val_EN == NRF_OK_EN)
            {
                // TODO: traitement de la trame
            }
            else
            {
                //logguer l'erreur
                error_counter_U16++;
            }
            RADIO_state_receive_packet_EN = RADIO_RECEIVE_TRANSITION_TO_WAITING_FOR_RECEIVE_PACKET_EN;
            break;
        default:
            break;
    }
}

void RADIO_SetMyID(uint8_t protocol_version_U8, uint8_t network_ID_U8, uint8_t my_address_U8)
{
    my_ID_STR.my_protocol_version_U8 = protocol_version_U8;
    my_ID_STR.my_network_ID_U8 = network_ID_U8;
    my_ID_STR.my_address_U8 = my_address_U8;
}

bool RADIO_SetMyGroups_B(uint8_t *groups_address_U8A, uint8_t size_groups_address_U8)
{
    if(size_groups_address_U8 > NUMBER_MAX_OF_GROUPS_U8)
    {
        return false;
    }
    for(uint8_t i = 0; i < NUMBER_MAX_OF_GROUPS_U8; i++)
    {
        my_ID_STR.my_groups_address_U8[i] = groups_address_U8A[i];
    }
    return true;
}

void RADIO_addGroup(uint8_t group_address_U8)
{
    for(uint8_t i_U8 = 0; i_U8 < NUMBER_MAX_OF_GROUPS_U8; i_U8++)
    {
        if(my_ID_STR.my_groups_address_U8[i_U8] == 0)
        {
            my_ID_STR.my_groups_address_U8[i_U8] = group_address_U8;
            return;
        }
    }
}

uint8_t* RADIO_getGroups(void)
{
    return my_ID_STR.my_groups_address_U8;
}

RADIO_steps_decode_packets_en RADIO_DecodePacket(RADIO_trame_UN received_packet_EN)
{
    RADIO_steps_decode_packets_en step_decode_packet_EN = RADIO_STEPS_CHECK_PROTOCOL_VERSION_EN;
    bool packet_is_for_me_B = false;
    switch (step_decode_packet_EN)
    {
        case RADIO_STEPS_CHECK_PROTOCOL_VERSION_EN:
            if(received_packet_EN.cerced_data_UN.cerced_data_str.protocol_version_U8 == my_ID_STR.my_protocol_version_U8)
            {
                step_decode_packet_EN = RADIO_STEPS_CHECK_NETWORK_ID_EN;
            }
            else
            {
                return RADIO_STEPS_CHECK_PROTOCOL_VERSION_EN;
            }
        case RADIO_STEPS_CHECK_NETWORK_ID_EN:
            if(received_packet_EN.cerced_data_UN.cerced_data_str.network_ID_U8 == my_ID_STR.my_network_ID_U8)
            {
                step_decode_packet_EN = RADIO_STEPS_CHECK_DESTINATION_ADDRESS_EN;
            }
            else
            {
                return RADIO_STEPS_CHECK_NETWORK_ID_EN;
            }
        case RADIO_STEPS_CHECK_DESTINATION_ADDRESS_EN:
            for (uint8_t i_U8 = 0; i_U8 < NUMBER_MAX_OF_GROUPS_U8; i_U8++)
            {
                if(received_packet_EN.cerced_data_UN.cerced_data_str.destination_address_U8 == my_ID_STR.my_groups_address_U8[i_U8])
                {
                    packet_is_for_me_B = true
                }
            }
            if(packet_is_for_me_B == true || received_packet_EN.cerced_data_UN.cerced_data_str.destination_address_U8 == my_ID_STR.my_address_U8)
            {
                step_decode_packet_EN = RADIO_STEPS_CHECK_CRC_ID_EN;
            }
            else
            {
                return RADIO_STEPS_CHECK_DESTINATION_ADDRESS_EN;
            }
        case RADIO_STEPS_CHECK_CRC_ID_EN:
            if(received_packet_EN.cerced_data_UN.cerced_data_str.CRC_ID_U32 == TOOLS_CRC32(received_packet_EN.not_cerced_data_UN.not_cerced_data_str.CRC_ID_U32, SIZE_NOT_CERCED_DATA_U8))
            {
                step_decode_packet_EN = RADIO_STEPS_DECODE_PROCESS_EN;
            }
            else
            {
                return RADIO_STEPS_CHECK_CRC_ID_EN;
            }
            break;
        case RADIO_STEPS_DECODE_PROCESS_EN:
            switch (received_packet_EN.cerced_data_UN.cerced_data_str.trame_type_U6)
            {
                case RADIO_ACKNOWLEDGEMENT_EN:
                    //TODO
                    break;
                case RADIO_TEMPERATURE_EN:
                    //TODO
                    break;
                case RADIO_HUMIDITY_EN:
                    //TODO
                    break;
                case RADIO_COMMAND_LEDS_STRIP_EN:
                    //TODO
                    break;
                case RADIO_BATTERY_PERCENTAGE_EN:
                    //TODO
                    break;
                
                default:
                    break;
                    
            }
}