 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Mazen Khaled
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

/*******************************************************************************
 *                              Module Definitions                             *
 *******************************************************************************/

/* Id for the company in the AUTOSAR */
#define PORT_VENDOR_ID    (1000U)

/* Dio Module Id */
#define PORT_MODULE_ID    (124U)

/* Dio Instance Id */
#define PORT_INSTANCE_ID  (0U)
   
/*******************************************************************************
 *                              API Definitions                                *
 *******************************************************************************/   
/* Service ID for Port Init Function*/
#define Port_SETUPGPIOPIN_SID                   (uint8)0x00

/* Service ID for Port Set Pin Direction Function*/
#define PORT_SET_PIN_DIRECTION_SID              (uint8)0x01

/* Service ID for Port Refresh Direction Function*/
#define PORT_REFRESH_PORT_DIRECTION_SID         (uint8)0x02

/* Service ID for Port Get Version Info Function*/
#define PORT_GET_VERSION_INFO_SID               (uint8)0x03

/* Service ID for Port Set Pin Mode Function*/
#define PORT_SET_PIN_MODE_SID                   (uint8)0x04
   
/*******************************************************************************
 *                              DET ERROR ID                                   *
 *******************************************************************************/  
#define PORT_E_PARAM_PIN                        (uint8)0x0A
#define PORT_E_INIT_FAILED                     	(uint8)0x0C
#define PORT_E_PARAM_INVALID_MODE               (uint8)0x0D
#define PORT_E_UNINIT                           (uint8)0x0F

/*******************************************************************************
 *                           Software Version 1.0.0                            *
 *******************************************************************************/
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*******************************************************************************
 *                           AUTOSAR Version 4.0.3                             *
 *******************************************************************************/
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*******************************************************************************
 *                           Macros For Port State                             *
 *******************************************************************************/
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/*******************************************************************************
*                           Standard AUTOSAR types                             *
********************************************************************************/
#include "Std_Types.h"
   
/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/*******************************************************************************
*                      Port Pre-Compile Configuration Header file              *
********************************************************************************/   
#include "Port_Cfg.h"
   
/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Dio_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Dio_Cfg.h does not match the expected version"
#endif   

/*******************************************************************************
*                               Non AUTOSAR files                              *
********************************************************************************/   
#include "Common_Macros.h"
      
/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/*********************Description: Enum to hold PIN direction*******************/
typedef enum
{
    INPUT,OUTPUT
}Port_PinDirection;

/************Description: Enum to hold internal resistor type for PIN***********/
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistor;

/******************************Description: Index of Array**********************/
typedef uint8 Port_PinType;

/*******************************Enum to hold Mode of PIN************************/
typedef enum
{
    DIO, ADC = 0,
    Mode1,
    Mode2,
    Mode3,
    Mode4,
    Mode5,
    Mode6,
    Mode7,
    Mode8,
    Mode9,
    Mode10,
    Mode11,
    Mode12,
    Mode13,
    Mode14,  
}Port_PinModeType;

/* Description: Structure to configure each individual PIN:
 *	1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *	2. the number of the pin in the PORT.
 *      3. the direction of pin --> INPUT or OUTPUT
 *      4. the internal resistor --> Disable, Pull up or Pull down
 *      5. Initial Pin Value
 *      6. Pin Mode
 */
typedef struct 
{
    uint8 port_num; 
    uint8 pin_num;
    Port_PinDirection direction;
    Port_InternalResistor resistor;
    uint8 initial_value;
    Port_PinModeType Mode;
}Channel_ConfigType;

/******************Structure Hold Array of Channel Configration*****************/
typedef struct 
{
  Channel_ConfigType Channel_config[PORT_CONFIGURED_CHANNLES];
}Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/*******************************************************************************
* Service Name          : Port_SetupGpioPin
* Sync/Async            : Synchronous
* Reentrancy            : reentrant
* Parameters (in)       : ConfigPtr - Pointer to post-build configuration data
* Parameters (inout)    : None
* Parameters (out)      : None
* Return value          : None
* Description           : Function to Setup the pin configuration:
*                               - Setup the pin as Digital GPIO pin
*                               - Setup the direction of the GPIO pin
*                               - Setup the internal resistor for i/p pin
********************************************************************************/
void Port_SetupGpioPin(const Port_ConfigType *ConfigPtr );

/*******************************************************************************
* Service Name          : Port_SetPinDirection
* Sync/Async            : Synchronous
* Reentrancy            : reentrant
* Parameters (in)       : Pin - Pin Number 0 to 38
*                         Direction - Set Port Pin Direction
* Parameters (inout)    : None
* Parameters (out)      : None
* Return value          : None
* Description           : Function to Setup the pin Direction.
********************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
  void Port_SetPinDirection( Port_PinType Pin_Index, Port_PinDirection Direction );
#endif

/*******************************************************************************
* Service Name          : Port_RefreshPortDirection
* Sync/Async            : Synchronous
* Reentrancy            : Non Reentrant
* Parameters (in)       : None
* Parameters (inout)    : None
* Parameters (out)      : None
* Return value          : None
* Description           : Refreshes port direction.
********************************************************************************/
void Port_RefreshPortDirection( void );

/*******************************************************************************
* Service Name          : Port_GetVersionInfo
* Sync/Async            : Synchronous
* Reentrancy            : Non Reentrant
* Parameters (in)       : versioninfo 
* Parameters (inout)    : None
* Parameters (out)      : None
* Return value          : None
* Description           : Returns the version information of this module.
********************************************************************************/
#if (PortVersionInfoApi == STD_ON)
  void Port_GetVersionInfo( Std_VersionInfoType* versioninfo );
#endif

/*******************************************************************************
* Service Name          : Port_SetPinMode
* Sync/Async            : Synchronous
* Reentrancy            : reentrant
* Parameters (in)       : Pin - Pin Number 0 to 38
*                         Mode - Set Port Pin Mode
* Parameters (inout)    : None
* Parameters (out)      : None
* Return value          : None
* Description           : Function to Setup the pin Mode.
********************************************************************************/
void Port_SetPinMode( Port_PinType Pin_Index, Port_PinModeType Mode );


/*******************************************************************************
 *                              External Variables                             *
 *******************************************************************************/
extern const Port_ConfigType Port_Configuration;

#endif /* PORT_H */
