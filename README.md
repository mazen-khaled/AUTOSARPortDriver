# AUTOSAR Port Driver Development (ARM Project)
## Project Overview
This project involves the development of a Port Driver for the AUTOSAR architecture on the Tiva C (ARM Cortex-M4F) microcontroller. The system is based on a simple time-triggered operating system (OS) and implements basic functionalities required to control the I/O ports of the microcontroller in an AUTOSAR-compliant manner.

![image](https://github.com/user-attachments/assets/b0418123-c102-4144-ac79-cdeb71d20941)

## Microcontroller
Tiva C (ARM Cortex-M4F)

## File Structure
### Application Layer
App.c / App.h
- Contains the application logic using the drivers and OS services.
  
### Hardware Abstraction Layer (HAL)
- Button.c / Button.h: Implements functions for handling button input.
- Led.c / Led.h: Contains functions for controlling LED output.

### OS Layer
Os.c / Os.h: Implements a simple time-triggered operating system used to schedule tasks.

### Microcontroller Abstraction Layer (MCAL)
- Port.c / Port.h: Implements the *AUTOSAR Port Driver* that manages the microcontroller's physical I/O ports.
- Port_Cfg.h / Port_PBCfg.c: Provides configuration data for the Port Driver (static and post-build).
- Dio.c / Dio.h: Digital Input/Output driver for controlling the GPIO pins.
- Dio_Cfg.h / Dio_PBcfg.c: Configuration data for the DIO driver.
- Dio_Regs.h / Port_Regs.h: Defines memory-mapped register addresses for DIO and Port peripherals.
- Gpt.c / Gpt.h: General Purpose Timer (GPT) driver for time-triggered functionality.

### Microcontroller Compiler and Registers
- Common_Macros.h: Contains common macros used across the project.
- Compiler.h: Provides compiler-specific configuration options.
- Platform_Types.h: Defines platform-dependent data types for portability.
- Std_Types.h: Defines standard AUTOSAR data types.
- tm4c123gh6pm_registers.h: Contains register definitions specific to the Tiva C microcontroller.

### Diagnostics
Det.c / Det.h: Diagnostic Event Trace (DET) driver, used for error reporting within the system.
### Startup Code
cstartup_M.c: Contains the startup code for initializing the system, stack, and interrupts.
### Main Application
The main application implements a basic time-triggered operating system that schedules tasks at specific intervals using the SysTick timer.

- Interrupt Control: The system globally enables or disables IRQ interrupts using the Enable_Interrupts() and Disable_Interrupts() macros, ensuring that critical sections can execute without interruptions.
- Timer Callback: The function Os_start() initializes the SysTick timer to generate an interrupt every 20ms. A callback is set to Os_NewTimerTick() to manage the system's tick count and signal when a new timer tick occurs.
- Task Scheduling: The Os_Scheduler() function runs an infinite loop, checking for new timer ticks. Based on the elapsed time (g_Time_Tick_Count), it schedules tasks such as:

    - Button_Task() for reading button input.
    - Led_Task() for controlling LEDs.
    - App_Task() for executing the main application logic.

#### Task Execution Intervals:
- Every 20ms, the system reads the button input.
- Every 40ms, it also controls the LED state.
- Every 60ms, the application logic is executed.
- Every 120ms, all tasks are executed, and the timer is reset.
This simple, modular design enables periodic execution of tasks in a predictable, time-triggered manner.

## Features
AUTOSAR Compliant Port Driver: Implements the AUTOSAR Port Driver module to configure and control microcontroller pins.
Simple Time-Triggered OS: A basic operating system schedules tasks at predefined intervals using the GPT driver.
Modular Design: The project follows AUTOSAR layered architecture, enhancing portability and scalability.
## How to Build
1) Clone the repository.
2) Open the project in an ARM-compatible IDE (e.g. IAR).
3) Compile and link the project to generate the firmware.
4) Flash the firmware onto the Tiva C microcontroller.
