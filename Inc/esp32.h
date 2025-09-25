#include "tipos.h"

void Test_pedir_IP_WIFI(void);
void check_BLE_status(void);//
void WR_en_BLE(uint8_t *data,uint8_t bytes);
void WR_en_TCP( uint8_t connID, uint8_t *data, uint16_t size);
void Init_ESP32(void);
void TCP_handler(void);
uint8_t Send_RS232_ESP_ACK(uint16_t timeout,char *data);
char Send_RS232_ESP_ACK_delayLargo(char *);
void Send_RS232_ESP_Sin_Evento_HEX(char *data, int size);
void Send_RS232_ESP_Sin_Evento(char *que_envio);
void WR_en_MQTT(uint8_t *data, uint16_t size,uint8_t respuestaAcomando);
void Activar_BLE();
void get_BLEMAC(bool ESP32_INIT); //
void ConectarMQTT();
void reConectarMQTT();//
void EsperaIPWifi();
void FACTORY_RESET();//
void ESP_RESET();//
void TestReconexionMQTT();

