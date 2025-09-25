#include "main.h"
#include "variables.h"
#include "tipos.h"

// size of array to copy__Launch_Command function to
// It should be at least equal to actual size of __Launch_Command func
// User can change this value based on RAM size availability and actual size of __Launch_Command function


#define BUFFER_SIZE_BYTE          0x80

#define DEBUGENABLE               0x00

#define READ_NORMAL_MARGIN        0x00
#define READ_USER_MARGIN          0x01
#define READ_FACTORY_MARGIN       0x02

#define ONE_KB                    1024

#define FTFx_REG_BASE             0x40020000
#define P_FLASH_BASE              0x00000000

#define BACKDOOR_KEY_LOCATION         0x400

// Program flash IFR
#if (FSL_FEATURE_FLASH_IS_FTFE == 1)
#define PFLASH_IFR                0x3C0
#else // FSL_FEATURE_FLASH_IS_FTFL == 1 or FSL_FEATURE_FLASH_IS_FTFA = =1
#define PFLASH_IFR                0xC0
#endif
// Program Flash block information
#define P_FLASH_SIZE            (FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE * FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT)
#define P_BLOCK_NUM             FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT
#define P_SECTOR_SIZE           FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE
// Data Flash block information
#define FLEXNVM_BASE            FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS
#define FLEXNVM_SECTOR_SIZE     FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE
#define FLEXNVM_BLOCK_SIZE      FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE
#define FLEXNVM_BLOCK_NUM       FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT

// Flex Ram block information
#define EERAM_BASE              FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS
#define EERAM_SIZE              FSL_FEATURE_FLASH_FLEX_RAM_SIZE


// Has flash cache control in MCM module
#if (FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS == 1)
#define CACHE_DISABLE             MCM_BWR_PLACR_DFCS(MCM_BASE_PTR, 1);
// Has flash cache control in FMC module
#elif (FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS == 1)
#if defined(FMC_PFB1CR) && defined(FMC_PFB1CR_B1SEBE_MASK)
#define CACHE_DISABLE     FMC_PFB0CR &= ~(FMC_PFB0CR_B0SEBE_MASK | FMC_PFB0CR_B0IPE_MASK | FMC_PFB0CR_B0DPE_MASK | FMC_PFB0CR_B0ICE_MASK | FMC_PFB0CR_B0DCE_MASK);\
                                  FMC_PFB1CR &= ~(FMC_PFB1CR_B1SEBE_MASK | FMC_PFB1CR_B1IPE_MASK | FMC_PFB1CR_B1DPE_MASK | FMC_PFB1CR_B1ICE_MASK | FMC_PFB1CR_B1DCE_MASK);
#else
#define CACHE_DISABLE     FMC_PFB0CR &= ~(FMC_PFB0CR_B0SEBE_MASK | FMC_PFB0CR_B0IPE_MASK | FMC_PFB0CR_B0DPE_MASK | FMC_PFB0CR_B0ICE_MASK | FMC_PFB0CR_B0DCE_MASK);
#endif
#else
// No cache in the device
#define CACHE_DISABLE
#endif

  
uint32_t GetPage(uint32_t Addr);
int BorrarDeAquiHastaAqui(uint32_t AdressInicio,uint32_t AdressFinal);
uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint64_t *Data, uint16_t numberofwords,char borrar);
uint32_t prg_flash(uint32_t destAdrss, uint16_t cuantos, uint32_t *deDonde);
uint32_t  prg_flash_sin_borrar(uint32_t destAdrss, uint16_t cuantos, uint32_t * deDonde);
