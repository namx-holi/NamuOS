/**
 * @file bios_defines.h
 * @ingroup kernel_bios
 * 
 * @todo Detailed description
 * 
 * @{
*/

#ifndef _KERNEL_BIOS_DEFINES_H
#define _KERNEL_BIOS_DEFINES_H 1

#include <stdint.h>

// NOTE: Assuming PS/2
// TODO: Prefix addresses with appropriate type based on type
// TODO: Update addresses to match new page table mapping.

// Ref: https://stanislavs.org/helppc/bios_data_area.html
#define BDA_INTERRUPT_VECTOR_TABLE ((uint16_t*)0x0000) // 256dwords
#define BDA_BOOTSTRAP_STACK_AREA   ((uint16_t*)0x0300) // 256bytes
#define BDA_COM1_PORT_ADDRESS      ((uint16_t*)0x0400) // word
#define BDA_COM2_PORT_ADDRESS      ((uint16_t*)0x0402) // word
#define BDA_COM3_PORT_ADDRESS      ((uint16_t*)0x0404) // word
#define BDA_COM4_PORT_ADDRESS      ((uint16_t*)0x0406) // word
#define BDA_LPT1_PORT_ADDRESS      ((uint16_t*)0x0408) // word
#define BDA_LPT2_PORT_ADDRESS      ((uint16_t*)0x040a) // word
#define BDA_LPT3_PORT_ADDRESS      ((uint16_t*)0x040c) // word

// Is Extended BIOS data area segment on PS/2
#define BDA_EBDA_SEGMENT ((uint16_t*)0x040e) // word

// Ref: https://stanislavs.org/helppc/int_11.html
#define BDA_EQUIPMENT_LIST_FLAGS ((uint16_t*)0x0410) // 2 bytes
    #define BDA_EQUIPT_IPL_MASK 0x0001
    #define BDA_EQUIPT_IPL_SHIFT 0
    #define BDA_EQUIPT_MATH_COPROCESSOR_MASK 0x0002
    #define BDA_EQUIPT_MATH_COPROCESSOR_SHIFT 1
    #define BDA_EQUIPT_POINTING_DEVICE_INSTALLED_MASK 0x0004
    #define BDA_EQUIPT_POINTING_DEVICE_INSTALLED_SHIFT 2

    // Unused on PS/2
    // #define BDA_EQUIPT_RAM_LT_256K_MASK 0x0008
    // #define BDA_EQUIPT_RAM_LT_256K_SHIFT 3

    // 00 - unused, 01 - 40x25 colour, 10 - 80x25 colour, 11 - 80x25 monochrome
    #define BDA_EQUIPT_INITIAL_VIDEO_MODE_MASK 0x0030
    #define BDA_EQUIPT_INITIAL_VIDEO_MODE_SHIFT 4

    // when bit 0=1, 00 - 1 drive, 01 - 2 drives, 10 - 3 drives, 11 - 4 drives
    #define BDA_EQUIPT_NB_DISKETTE_MASK 0x00c0
    #define BDA_EQUIPT_NB_DISKETTE_SHIFT 6
    #define BDA_EQUIPT_DMA_NOT_INSTALLED_MASK 0x0100
    #define BDA_EQUIPT_DMA_NOT_INSTALLED_SHIFT 8
    #define BDA_EQUIPT_NB_SERIAL_PORTS_MASK 0x0e00
    #define BDA_EQUIPT_NB_SERIAL_PORTS_SHIFT 9
    #define BDA_EQUIPT_GAME_ADAPTER_INSTALLED_MASK 0x1000
    #define BDA_EQUIPT_GAME_ADAPTER_INSTALLED_SHIFT 12
    #define BDA_EQUIPT_INTERNAL_MODEM_MASK 0x2000
    #define BDA_EQUIPT_INTERNAL_MODEM_SHIFT 13
    #define BDA_EQUIPT_NB_PRINTER_PORTS_MASK 0xc000
    #define BDA_EQUIPT_NB_PRINTER_PORTS_SHIFT 14

#define BDA_INFRARED_KEYBOARD_LINK_ERROR_COUNT ((uint16_t*)0x0412) // byte

// Number of contiguious 1k memory blocks found at startup
// Does not include video memory or extended RAM
#define BDA_MEMORY_SIZE_KB ((uint16_t*)0x0413) // word

#define BDA_PS2_BIOS_CONTROL_FLAGS ((uint16_t*)0x0416) // byte

#define BDA_KEYBOARD_FLAGS ((uint16_t*)0x0417) // 2 bytes
    // Masks, all single bits, can just check if non-zero
    #define BDA_KEYBOARD_RSHIFT_PRESSED 0x0001
    #define BDA_KEYBOARD_LSHIFT_PRESSED 0x0002
    #define BDA_KEYBOARD_CTRL_PRESESD 0x0004
    #define BDA_KEYBOARD_ALT_PRESSED 0x0008
    #define BDA_KEYBOARD_SCROLLLOCK_ACTIVE 0x0010
    #define BDA_KEYBOARD_NUMLOCK_ACTIVE 0x0020
    #define BDA_KEYBOARD_CAPSLOCK_ACTIVE 0x0040
    #define BDA_KEYBOARD_INSERT_ACTIVE 0x0080
    #define BDA_KEYBOARD_LCTRL_PRESSED 0x0100
    #define BDA_KEYBOARD_LALT_PRESSED 0x0200
    #define BDA_KEYBOARD_SYSTEM_KEY_PRESSED 0x0400
    #define BDA_KEYBOARD_SUSPEND_TOGGLED 0x0800
    #define BDA_KEYBOARD_SCROLLLOCK_PRESSED 0x1000
    #define BDA_KEYBOARD_NUMLOCK_PRESSED 0x2000
    #define BDA_KEYBOARD_CAPSLOCK_PRESSED 0x4000
    #define BDA_KEYBOARD_INSERT_PRESSED 0x8000

#define BDA_STORAGE_FOR_ALTERNATE_KEYPAD_ENTRY ((uint16_t*)0x0419) // byte

// TODO: Are these in 0x480/0x0482 instead?
#define BDA_KEYBOARD_BUFFER_ADDR_OFFSET 0x1e // Subtract from head/tail to get index
#define BDA_KEYBOARD_BUFFER_HEAD ((uint16_t*)0x041a) // word, + 0x0400
#define BDA_KEYBOARD_BUFFER_TAIL ((uint16_t*)0x041c) // word, + 0x0400
#define BDA_KEYBOARD_BUFFER ((char*)0x041e) // 32 bytes, circular queue

#define BDA_DRIVE_RECALIBRATION_STATUS ((uint16_t*)0x043e) // byte
    // Masks, all single bits, can just check if non-zero
    #define BDA_DRIVE_RECALIBRATION_0 0x01
    #define BDA_DRIVE_RECALIBRATION_1 0x02
    #define BDA_DRIVE_RECALIBRATION_2 0x04
    #define BDA_DRIVE_RECALIBRATION_3 0x08
    #define BDA_DRIVE_RECALIBRATION_WORKING_INTERRUPT 0x80

#define BDA_DISKETTE_MOTOR_STATUS ((uint16_t*)0x043f) // byte
    // Masks, all single bits, can just check if non-zero
    #define BDA_DISKETTE_MOTOR_0 0x01
    #define BDA_DISKETTE_MOTOR_1 0x02
    #define BDA_DISKETTE_MOTOR_2 0x04
    #define BDA_DISKETTE_MOTOR_3 0x08
    #define BDA_DISKETTE_WRITE_OPERATION 0x80

// Ref: https://stanislavs.org/helppc/int_8.html
#define BDA_MOTOR_SHUTOFF_COUNTER ((uint16_t*)0x0440) // byte

#define BDA_LAST_DISKETTE_OPERATION_STATUS ((uint16_t*)0x0441) // byte
    // Masks, all single bits, can just check if non-zero
    #define BDA_LAST_DISKETTE_OPERATION_INVALID 0x01
    #define BDA_LAST_DISKETTE_OPERATION_ADDRESS_NOT_FOUND 0x02
    #define BDA_LAST_DISKETTE_OPERATION_SECTOR_NOT_FOUND 0x04
    #define BDA_LAST_DISKETTE_OPERATION_DMA_ERROR 0x08
    #define BDA_LAST_DISKETTE_OPERATION_DATA_ERROR 0x10
    #define BDA_LAST_DISKETTE_OPERATION_CONTROLLER_FAILURE 0x20
    #define BDA_LAST_DISKETTE_OPERATION_SEEK_FAILED 0x40
    #define BDA_LAST_DISKETTE_OPERATION_TIMEOUT 0x80

// Ref: https://stanislavs.org/helppc/765.html
#define BDA_NEC_DISKETTE_CONTROLLER_STATUS ((uint16_t*)0x0442) // 7 bytes

// Ref: https://stanislavs.org/helppc/int_10-0.html
#define BDA_CURRENT_VIDEO_MODE ((uint16_t*)0x0449) // byte

#define BDA_NB_SCREEN_COLUMNS ((uint16_t*)0x044a) // word
#define BDA_VIDEO_REGEN_BUFFER_SIZE ((uint16_t*)0x044c) // word, in bytes
#define BDA_VIDEO_REGEN_BUFFER_OFFSET ((uint16_t*)0x044e) // word, in bytes

// TODO: Break into things
// high order byte = row, low order byte = column
// TODO: Also merge back into one value of 8 words?
#define BDA_CURSOR_POSITION_PAGE1 ((uint16_t*)0x0450) // 1 word
#define BDA_CURSOR_POSITION_PAGE2 ((uint16_t*)0x0452) // 1 word
#define BDA_CURSOR_POSITION_PAGE3 ((uint16_t*)0x0454) // 1 word
#define BDA_CURSOR_POSITION_PAGE4 ((uint16_t*)0x0456) // 1 word
#define BDA_CURSOR_POSITION_PAGE5 ((uint16_t*)0x0458) // 1 word
#define BDA_CURsOR_POSITION_PAGE6 ((uint16_t*)0x045a) // 1 word
#define BDA_CURsOR_POSITION_PAGE7 ((uint16_t*)0x045c) // 1 word
#define BDA_CURsOR_POSITION_PAGE8 ((uint16_t*)0x045e) // 1 word

#define BDA_CURSOR_ENDING_SCAN_LINE ((uint16_t*)0x0460) // 1 byte
#define BDA_CURSOR_STARTING_SCAN_LINE ((uint16_t*)0x0461) // byte
#define BDA_ACTIVE_DISPLAY_PAGE_NUMBER ((uint16_t*)0x0462) // byte

// TODO: Continue from base port address for active 6845 CRT controller

// NOTE: This is what the addresses look like before using a higher half kernel
// #define VIDEO_MEMORY_ADDR_MONOCHROME ((uint16_t*)0xb0000)
// #define VIDEO_MEMORY_ADDR_COLOUR ((uint16_t*)0xb8000)

// NOTE: This is what the addresses look like when only mapping kernel in page
//  table in higher half kernel, specifically using page 1024 for video
// #define VIDEO_MEMORY_ADDR_MONOCHROME ((uint16_t*)0xC03F7000)
// #define VIDEO_MEMORY_ADDR_COLOUR ((uint16_t*)0xC03FF000)

// NOTE: This is what the addresses look like when fully mapping the first 1 MiB
//  in the page table, not specifically looking for VGA.
#define VIDEO_MEMORY_ADDR_MONOCHROME ((uint16_t*)0xb0000)
#define VIDEO_MEMORY_ADDR_COLOUR ((uint16_t*)0xb8000)




#endif

/** @} */
