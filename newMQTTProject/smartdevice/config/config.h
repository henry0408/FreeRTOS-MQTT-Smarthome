#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_NOOS 				1

#define CONFIG_SUPPORT_HAL	 1

#define CONFIG_USE_FREERTOS		1

#define DEBUG 1
#if DEBUG
#define debug_printf printf
#else
#define debug_printf(...)
#endif



#endif


