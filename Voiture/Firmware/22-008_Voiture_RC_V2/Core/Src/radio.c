#include "../Inc/radio.h"

#define SIZE_BUFFER_U16 100
#define NUMBER_OF_RETRY_MAX_U8 3
#define TIMEOUT_ACKNOWLEDGE_ms_U32 1000


uint16_t curseur_ecriture_U16 = 0;
uint16_t curseur_lecture_U16 = 0;

bool waiting_acknowledge_B = false;
bool acknoledge_received_B = false;

uint32_t packet_ID_to_acknowledge_U32 = 0;

uint16_t error_counter_U16 = 0;

uint8_t retry_counter_U8 = 0;

uint32_t beginnning_send_time_U32 = 0;

RADIO_state_send_packet_en RADIO_state_send_packet_EN = RADIO_SEND_INIT_EN;

uint8_t buffer_U8[SIZE_BUFFER_U16];

typedef enum
{
    RADIO_SEND_INIT_EN = 0,
    RADIO_SEND_TRANSITION_TO_BEGIN_EN,
    RADIO_SEND_BEGIN_EN,
    RADIO_SEND_WAITING_ACK_EN,
    RADIO_SEND_SENDING_PACKET_EN
}RADIO_state_send_packet_en;

void RADIO_init(void)
{
    curseur_ecriture_U16 = 0;
    curseur_lecture_U16 = 0;
    retry_counter_U8 = 0;
    beginnning_send_time_U32 = 0;
    waiting_acknowledge_B = false;
    acknoledge_received_B = false;
}

void RADIO_SendStateMachine(void)
{
    switch(RADIO_state_send_packet_EN)
    {
        case RADIO_SEND_INIT_EN:
            RADIO_init();
            RADIO_state_send_packet_EN = RADIO_SEND_TRANSITION_TO_BEGIN_EN;
            break;

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
                packet_ID_to_acknowledge_U32 = XXX;
                beginnning_send_time_U32 = TOOLS_millis_U32();
                RADIO_state_send_packet_EN = RADIO_SEND_WAITING_ACK_EN;
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