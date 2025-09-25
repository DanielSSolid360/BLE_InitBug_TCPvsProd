///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
// Standard C Included Files
#include <string.h>
#include <stdio.h>
// SDK Included Files
/* Including needed modules to compile this module/procedure */
//#include "Cpu.h"
//#include "Events.h"
//#include "clockMan1.h"
//#include "osa1.h"
#include "main.h"
#include "variables.h"
#include "varios.h"
#include "tipos.h"

uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}



int BorrarDeAquiHastaAqui(uint32_t AdressInicio,uint32_t AdressFinal) //  Para borrar el bootloader antiguo
{

	
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;
	int sofar=0;
	__HAL_RCC_TIM14_IS_CLK_DISABLED();
	  /* Unlock the Flash to enable the flash control register access *************/
	   HAL_FLASH_Unlock();
		 HAL_Delay(100);
	   /* Erase the user Flash area*/

	  uint32_t StartPage = GetPage(AdressInicio);
	  uint32_t EndPage = GetPage(AdressFinal);

	   /* Fill EraseInit structure*/
	   EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	   EraseInitStruct.Page 	= StartPage;
	   EraseInitStruct.NbPages     = ((AdressFinal - AdressInicio)/FLASH_PAGE_SIZE);

	   if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	   {
	     /*Error occurred while page erase.*/
		  __HAL_RCC_TIM14_IS_CLK_ENABLED();
		  return HAL_FLASH_GetError ();
	   }

	   __HAL_RCC_TIM14_IS_CLK_ENABLED();
	   return 0;


}


uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint64_t *Data, uint16_t numberofwords,char borrar)
{
	
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;
	int sofar=0;
	__HAL_RCC_TIM14_IS_CLK_DISABLED();
	  /* Unlock the Flash to enable the flash control register access *************/
	   HAL_FLASH_Unlock();
	   Delay(100);
	   /* Erase the user Flash area*/

	  uint32_t StartPage = GetPage(StartPageAddress);
	  uint32_t EndPageAdress = StartPageAddress + numberofwords*8;
	  uint32_t EndPage = GetPage(EndPageAdress);

	   /* Fill EraseInit structure*/
	   EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	   EraseInitStruct.Page 	= StartPage;
	   EraseInitStruct.NbPages     = ((EndPage - StartPage)/FLASH_PAGE_SIZE) +1;

	   if (borrar==SI)
	   {	
	   	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	   	{
	   	  /*Error occurred while page erase.*/
			  __HAL_RCC_TIM14_IS_CLK_ENABLED();
			  return HAL_FLASH_GetError ();
	   	}
	   }	
	   /* Program the user Flash area word by word*/

	   while (sofar<numberofwords)
	   {
	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, StartPageAddress, Data[sofar]) == HAL_OK)
	     {
	    	 StartPageAddress += 8;  // use StartPageAddress += 2 for half word and 8 for double word
	    	 sofar++;
	     }
	     else
	     {
	       /* Error occurred while writing data in Flash memory*/
	    	 __HAL_RCC_TIM14_IS_CLK_ENABLED();
	    	 return HAL_FLASH_GetError ();
	     }
	   }

	   /* Lock the Flash to disable the flash control register access (recommended
	      to protect the FLASH memory against possible unwanted operation) *********/

	   __HAL_RCC_TIM14_IS_CLK_ENABLED();
	   return 0;
}


uint32_t prg_flash(uint32_t destAdrss, uint16_t cuantos, uint32_t *deDonde)
{
	uint32_t error;
	
	error=Flash_Write_Data (destAdrss,(void *)deDonde,cuantos,SI);
	
	return (error);
}

uint32_t  prg_flash_sin_borrar(uint32_t destAdrss, uint16_t cuantos, uint32_t * deDonde)
{
	uint32_t error;
	
	error=Flash_Write_Data (destAdrss,(void *)deDonde,cuantos,NO);
	
	return (error);
}


///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////
