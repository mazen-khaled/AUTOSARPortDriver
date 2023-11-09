 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Mazen Khaled
 ******************************************************************************/

/****************************Included Files************************************/

#include "Port.h"
#include "Port_Regs.h"

/*Check If Dev Error in ON*/
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  
  /* AUTOSAR Version checking between Det and Dio Modules */
  #include "Det.h"
  #if ((DET_AR_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION)\
   || (DET_AR_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION)\
   || (DET_AR_PATCH_VERSION != DIO_AR_RELEASE_PATCH_VERSION))
    #error "The AR version of Det.h does not match the expected version"
  #endif
   
#endif

/*******************************Globle Varibles********************************/
   
STATIC const Channel_ConfigType * Port_Channel_Config = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED; 

/****************************Function Definitions******************************/

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
*                               - Provide initial value for o/p pin
*                               - Setup the internal resistor for i/p pin
************************************************************************************/

void Port_SetupGpioPin(const Port_ConfigType * ConfigPtr )
{
    volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    uint8 count = 0; /*For While Loop*/
    uint8 delay = 0; /*For Setting Port Clock*/
    Port_Channel_Config = ConfigPtr->Channel_config; /*Pointer to the Channel_config structure*/

    #if(PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the input configuration pointer to structure is Not a Null Pointer  */
	if(NULL_PTR == ConfigPtr)
	{
		/* ConfigPtr is a Null Pointer, Report a DET Error as a PORT_E_INIT_FAILED */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_SETUPGPIOPIN_SID, PORT_E_INIT_FAILED);
		return;
	}
	else
    #endif
    
    /*For Configure all selected Ports*/      
    while( count <= PORT_CONFIGURED_CHANNLES){
      /*Get Current Port Base address*/
      switch(Port_Channel_Config[count].port_num)
      {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
      }
      
      /* Enable clock for PORT and allow time for clock to start*/
      SYSCTL_REGCGC2_REG |= (1<<Port_Channel_Config[count].port_num);
      delay = SYSCTL_REGCGC2_REG;
      
      /*Check the Pin if it's one of the Locked pins*/
      if( ((Port_Channel_Config[count].port_num == PORTD) && (Port_Channel_Config[count].pin_num == PD7)) || ((Port_Channel_Config[count].port_num == PORTF) && (Port_Channel_Config[count].pin_num == PF0)) ) /* PD7 or PF0 */
      {
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , Port_Channel_Config[count].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
      }
    
      /*Check the Pin if it's one of JTAG pins*/
      else if( (Port_Channel_Config[count].port_num == 2) && (Port_Channel_Config[count].pin_num <= 3) ) /* PC0 to PC3 */
      {
          /* Do Nothing ...  this is the JTAG pins */
      }
      
      else
      {
          /* Do Nothing ... No need to unlock the commit register for this pin */
      }
      
      /*Setting For Digital Pins*/
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Channel_Config[count].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Channel_Config[count].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
      *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_Channel_Config[count].pin_num * 4));     /* Clear the PMCx bits for this pin */
      
      /*Setting Pin Direction as OUTPUT*/
      if(Port_Channel_Config[count].direction == OUTPUT)
      {
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_Channel_Config[count].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
          
          if(Port_Channel_Config[count].initial_value == STD_HIGH)
          {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_Channel_Config[count].pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
          }
          else
          {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_Channel_Config[count].pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
          }
      }
      
      /*Setting Pin Direction as INPUT*/
      else if(Port_Channel_Config[count].direction == INPUT)
      {
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_Channel_Config[count].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
          
          /*Activate PULL UP Resistor*/
          if(Port_Channel_Config[count].resistor == PULL_UP)
          {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_Channel_Config[count].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
          }
          /*Activate PULL DOWN Resistor*/
          else if(Port_Channel_Config[count].resistor == PULL_DOWN)
          {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_Channel_Config[count].pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
          }
          /*Close Internal Resistor*/
          else
          {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_Channel_Config[count].pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_Channel_Config[count].pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
          }
      }
      else
      {
          /* Do Nothing */
      }
      
      /*Enable Port*/
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_Channel_Config[count].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
        count++;
      }   
      
      Port_Status = PORT_INITIALIZED;
}

/************************************************************************************
* Service Name          : Port_SetPinDirection
* Sync/Async            : Synchronous
* Reentrancy            : reentrant
* Parameters (in)       : Pin - Pin Number 0 to 38
*                         Direction - Set Port Pin Direction
* Parameters (inout)    : None
* Parameters (out)      : None
* Return value          : None
* Description           : Function to Setup the pin Direction.
************************************************************************************/
/*Optional Function Selected From Tool*/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)

  void Port_SetPinDirection( Port_PinType Pin_Index, Port_PinDirection Direction )
  {
    volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    
    #if(PORT_DEV_ERROR_DETECT == STD_ON)
      /* Check if this function is called before initialization the Port Module */
      if(PORT_NOT_INITIALIZED == Port_Status)
      {
              /* Port Module doesn't initialized yet, report a PORT_E_UNINIT DET Error */
              Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
      }
      else
      {
              /* Do nothing */
      }

      /* Check for incorrect PortPin ID Passed */
      if(PORT_CONFIGURED_CHANNLES <= Pin_Index)
      {
              /* Incorrect Pin ID passed, report a PORT_E_PARAM_PIN DET Error */
              Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);

      }
      else
      {
              /* Do nothing */
      }

      /*
       * Here you can see #endif is put after else so that if DET Error disabled through tool
       * i won't have a bug here said "else without previous if"
       */
    #endif

    /*Setting Pin Direction as OUTPUT*/
        if(Port_Channel_Config[Pin_Index].direction == OUTPUT)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
            
            if(Port_Channel_Config[Pin_Index].initial_value == STD_HIGH)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
            }
            else
            {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
            }
        }
        
        /*Setting Pin Direction as INPUT*/
        else if(Port_Channel_Config[Pin_Index].direction == INPUT)
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
            
            /*Activate PULL UP Resistor*/
            if(Port_Channel_Config[Pin_Index].resistor == PULL_UP)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
            }
            /*Activate PULL DOWN Resistor*/
            else if(Port_Channel_Config[Pin_Index].resistor == PULL_DOWN)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
            }
            /*Close Internal Resistor*/
            else
            {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
            }
        }
        else
        {
            /* Do Nothing */
        }
  }
#endif

/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction.
************************************************************************************/
void Port_RefreshPortDirection( void ){
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  Port_Channel_Config = Port_Configuration.Channel_config; /*Pointer to the configered data array*/
  uint8 count = 0;
  
  #if(PORT_DEV_ERROR_DETECT == STD_ON)
      /* Check if this function is called before initialization the Port Module */
      if(PORT_NOT_INITIALIZED == Port_Status)
      {
              /* Port Module doesn't initialized yet, report an PORT_E_UNINIT DET Error */
              Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_SID, PORT_E_UNINIT);
      }
      else
      {
              /* Do nothing, Pin direction is changeable */
      }
  #endif

  /*For Refresh all Ports*/
  while( count <= PORT_CONFIGURED_CHANNLES){ 
    /*Get Current Port Base address*/
    switch(Port_Channel_Config[count].port_num)
    {
      case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
               break;
      case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
               break;
      case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
               break;
      case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
               break;
      case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
               break;
      case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
               break;
    }
    /*Setting Pin Direction as OUTPUT*/
    if(Port_Channel_Config[count].direction == OUTPUT)
    {
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_Channel_Config[count].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
        if(Port_Channel_Config[count].initial_value == STD_HIGH)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_Channel_Config[count].pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_Channel_Config[count].pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
        }
    }
    
    /*Setting Pin Direction as INPUT*/
    else if(Port_Channel_Config[count].direction == INPUT)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_Channel_Config[count].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        
        /*Activate PULL UP Resistor*/
        if(Port_Channel_Config[count].resistor == PULL_UP)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_Channel_Config[count].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
        }
        /*Activate PULL DOWN Resistor*/
        else if(Port_Channel_Config[count].resistor == PULL_DOWN)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_Channel_Config[count].pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        /*Close Internal Resistor*/
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_Channel_Config[count].pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_Channel_Config[count].pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
    }
    else
    {
        /* Do Nothing */
    }
    count++;
  }
}

/************************************************************************************
* Service Name: Port_GetVersionInfo
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): versioninfo 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Returns the version information of this module.
************************************************************************************/
/*Optional Function*/
#if (PortVersionInfoApi == STD_ON)
  void Port_GetVersionInfo( Std_VersionInfoType* versioninfo ){
    #if (PORT_DEV_ERROR_DETECT == STD_ON)
            /* Check if input pointer is not Null pointer */
            if(NULL_PTR == versioninfo)
            {
                    /* Report to DET  */
                    Det_ReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,
                                    PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_PIN);
            }
            else
    #endif /* (DIO_DEV_ERROR_DETECT == STD_ON) */
            {
                    /* Copy the vendor Id */
                    versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
                    /* Copy the module Id */
                    versioninfo->moduleID = (uint16)PORT_MODULE_ID;
                    /* Copy Software Major Version */
                    versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
                    /* Copy Software Minor Version */
                    versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
                    /* Copy Software Patch Version */
                    versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
            }
}
#endif

/************************************************************************************
* Service Name: Port_SetPinMode
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin - Pin Number 0 to 38
*                  Mode - Set Port Pin Mode
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the pin Mode.
************************************************************************************/
void Port_SetPinMode( Port_PinType Pin_Index, Port_PinModeType Mode ){
  
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  Port_Channel_Config = Port_Configuration.Channel_config;
  
  #if(PORT_DEV_ERROR_DETECT == STD_ON)

	/* Check if this function is called before initialization the Port Module */
	if(PORT_NOT_INITIALIZED == Port_Status)
	{
		/* Port Module doesn't initialized yet, report an PORT_E_UNINIT DET Error */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_SID, PORT_E_UNINIT);
	}

	else
	{
		/* Do nothing, Pin direction is changeable */
	}
  #endif
  
  /*Get Current Port Base address*/
  switch(Port_Channel_Config[Pin_Index].port_num)
  {
    case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
             break;
    case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
             break;
    case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
             break;
    case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
             break;
    case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
             break;
    case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
             break;
  }
  
  if (Mode == DIO){
    /*Setting For Digital Pins*/
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_Channel_Config[Pin_Index].pin_num * 4));     /* Clear the PMCx bits for this pin */
  }
  else if (Mode == ADC){
    /*Setting For Analog Pins*/
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_Channel_Config[Pin_Index].pin_num * 4));     /* Clear the PMCx bits for this pin */
  }
  else{
    /*Setting For Altirnative Mode Pins*/
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Channel_Config[Pin_Index].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~( ~((uint32)(Mode)) << (Port_Channel_Config[Pin_Index].pin_num * 4));     /* Clear the PMCx bits for this pin and Setting an Altilnative Mode*/
  } 
}