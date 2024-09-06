/// @file bios.c

#include <kernel/bios.h> // Implements

#include <kernel/bios_defines.h>
#include <stdio.h>


void bios_log_equipment(void) {
	uint16_t e = *BDA_EQUIPMENT_LIST_FLAGS;

	printf("IPL diskette installed: %d\n",
		(e & BDA_EQUIPT_IPL_MASK) >> BDA_EQUIPT_IPL_SHIFT);
	printf("Math coprocessor: %d\n",
		(e & BDA_EQUIPT_MATH_COPROCESSOR_MASK) >> BDA_EQUIPT_MATH_COPROCESSOR_SHIFT);
	printf("Pointing device installed: %d\n",
		(e & BDA_EQUIPT_POINTING_DEVICE_INSTALLED_MASK) >> BDA_EQUIPT_POINTING_DEVICE_INSTALLED_SHIFT);
	printf("Initial video mode: %d\n",
		(e & BDA_EQUIPT_INITIAL_VIDEO_MODE_MASK) >> BDA_EQUIPT_INITIAL_VIDEO_MODE_SHIFT);
	printf("# of diskette drives, less 1: %d\n",
		(e & BDA_EQUIPT_NB_DISKETTE_MASK) >> BDA_EQUIPT_NB_DISKETTE_SHIFT);
	printf("0 if DMA installed: %d\n",
		(e & BDA_EQUIPT_DMA_NOT_INSTALLED_MASK) >> BDA_EQUIPT_DMA_NOT_INSTALLED_SHIFT);
	printf("Number of serial ports: %d\n",
		(e & BDA_EQUIPT_NB_SERIAL_PORTS_MASK) >> BDA_EQUIPT_NB_SERIAL_PORTS_SHIFT);
	printf("Game adapter: %d\n",
		(e & BDA_EQUIPT_GAME_ADAPTER_INSTALLED_MASK) >> BDA_EQUIPT_GAME_ADAPTER_INSTALLED_SHIFT);
	printf("Internal modem: %d\n",
		(e & BDA_EQUIPT_INTERNAL_MODEM_MASK) >> BDA_EQUIPT_INTERNAL_MODEM_SHIFT);
	printf("Number of printer ports: %d\n",
		(e & BDA_EQUIPT_NB_PRINTER_PORTS_MASK) >> BDA_EQUIPT_NB_PRINTER_PORTS_SHIFT);
}
