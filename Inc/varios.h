#include "variables.h"

void Procesa_consola(void);
void Pinta_nuevos_eventos_BLE(void);
void GPIO_STATE(void);
void GPIO_processing_logic(uint8_t RLn, GPIO_TypeDef* GPIO_Port, uint16_t GPIO_X, volatile uint16_t* last_RL_Event, volatile uint32_t* Last_RL_Op, volatile uint32_t* openedGateWatcher, volatile uint16_t* RL_monitorTimer);
void Reenvia_TCP_por_BLE(void);
void Escribe_en_flash	(char *que_grabo,unsigned short offset,unsigned short cuantos);
void Lee_en_flash	(char *que_dejo,unsigned short offset,unsigned short cuantos);
void Delay(unsigned int retraso);
void LeeIDF(unsigned char *aqui,unsigned char * DireccionMemoria,char cuantos);
void Copia_SSDI(void);
void Copia_PASSWORD(void);
unsigned short crc16(unsigned char * data_p, unsigned short length, unsigned short cr_inico);
void ActualizaHora(void);
void Captura_fecha (char *deAqui);
void Actualizacion_Programada (void);
void check_pending_download(void);
void mqtt_recon_handler(void);
void pending_MQTT_messages();
void pending_wifi_handler();

