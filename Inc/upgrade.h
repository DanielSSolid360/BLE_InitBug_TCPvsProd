///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
// SDK Included Files
/* Including needed modules to compile this module/procedure */
//#include "Cpu.h"
//#include "Events.h"
//#include "clockMan1.h"
//#include "osa1.h"
#include "main.h"
#include "variables.h"
#include "prg_flash.h"
#include "stm32g0xx_hal.h"

#define VERSION_UPGARDE	("@VERSION_BOOT@0.0")
#define GPIO_NUMBER           (16u)

void AA_Hazupgrade(uint32_t,uint32_t,uint32_t);
int BorrarDe0000a1E000(uint32_t HIWDG);
//void Upgrade(void);
uint32_t GetPageUPG(uint32_t Addr);
HAL_StatusTypeDef HAL_FLASHEx_EraseUPG(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError, uint32_t );
HAL_StatusTypeDef FLASH_WaitForLastOperationUPG(uint32_t Timeout,uint32_t);
void FLASH_PageEraseUPG(uint32_t Banks, uint32_t Page);
void DelayUPG(uint32_t retraso,uint32_t );
HAL_StatusTypeDef HAL_FLASH_ProgramUPG(uint32_t TypeProgram, uint32_t Address, uint64_t Data, uint32_t);
void FLASH_Program_DOUBLEWORDUPG(uint32_t Address, uint64_t Data);
HAL_StatusTypeDef HAL_FLASH_UnlockUPG(void);
void FLASH_MassEraseUPG(uint32_t Banks);
int BorrarDe0000aE000(uint32_t );
HAL_StatusTypeDef HAL_IWDG_RefreshUPG(uint32_t);
HAL_StatusTypeDef HAL_UART_TransmitUPG(uint32_t, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_ReceiveUPG(uint32_t, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef UART_WaitOnFlagUntilTimeoutUPG(uint32_t, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout);
__weak uint32_t HAL_GetTickUPG(void);
void ZZ_version_cargador (char *resultado); 
void MiIntToAscii (char *resultado,int numero); 
int longitud(char cadena[]);
char concatena(char cadena1[], char cadena2[]);
void copiar(char *dest, char *src);
signed int Mi_aeabi_idiv(signed int num, signed int den);                                    
uint32_t Mi_udivmodsi4(uint32_t num, uint32_t den, int modwanted);  
void HAL_GPIO_TogglePinUPG(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WritePinUPG(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
uint16_t MedirTensionUPG(uint32_t,uint32_t HIWDG);

                                                                                    
