MEMORY
{
  /* Define each memory region */
  /* First 8kB are reserved for bootloader */
  BootFlash (rx) : ORIGIN = 0x0000, LENGTH = 8K /* 8K bytes */

  
  RamLoc16 (rwx) : ORIGIN = 0x10000000, LENGTH = 64K /* 64K bytes */
  RamAHB16 (rwx) : ORIGIN = 0x20000000, LENGTH = 16K /* 16K bytes */
}
  __BootFlash_start     = 0x0000;          /* Bootloader start address (vector table) */
  __BootFlash_end       = 0x1FFF;          /* Bootloader end address (last byte) */
  __AppFlash_start      = 0x2000;          /* Application start address (vector table) */
  __AppFlash_end        = 0x1FFF + 248K;   /* Application end address (last byte) */
  __FWUpdateFlash_start = 0x40000;         /* Firmware update region start address */
  __FWUpdateFlash_end   = 0x3FFFF + 256K;  /* Firmware update region end address (last byte) */

  /* Last 256 bytes of flash reserved to the fw_info struct */
  __FWInfo_addr         = __FWUpdateFlash_end - 255;

  /* Define a symbol for the top of each memory region */
  __top_BootFlash = 0x0000 + 8K;
  __top_RamLoc16 = 0x10000000 + 64K;
  __top_RamAHB16 = 0x20000000 + 16K;

ENTRY(ResetISR)
EXTERN(NMI_Handler)
EXTERN(HardFault_Handler)
EXTERN(MemManage_Handler)
EXTERN(BusFault_Handler)
EXTERN(UsageFault_Handler)

SECTIONS
{
    /* MAIN TEXT SECTION */
    .boot : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))

        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        LONG(LOADADDR(.text)); 
        LONG(    ADDR(.text)); 
        LONG(  SIZEOF(.text)); 
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)
        *(.boot*) 
    } > BootFlash

    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > RamLoc16 AT> BootFlash

    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this.
     */
    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > BootFlash
    __exidx_start = .;

    .ARM.exidx : ALIGN(4)
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > BootFlash
    __exidx_end = .;

    _etext = .;

    /* DATA section for RamAHB16 */
    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$RamAHB16)
        *(.data.$RAM2*)
        *(.data.$RamAHB16*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;
    } > RamAHB16 AT> BootFlash

    /* MAIN DATA SECTION */

    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > RamLoc16

    /* Main DATA section (RamLoc16) */
    .data : ALIGN(4)
    {
        FILL(0xff)
        _data = . ;
        *(vtable)
        *(.ramfunc*)
        *(.data*)
        . = ALIGN(4) ;
        _edata = . ;
    } > RamLoc16 AT> BootFlash

    /* BSS section for RamAHB16 */
    .bss_RAM2 : ALIGN(4)
    {
        PROVIDE(__start_bss_RAM2 = .) ;
        *(.bss.$RAM2*)
        *(.bss.$RamAHB16*)
        . = ALIGN(4) ;
        PROVIDE(__end_bss_RAM2 = .) ;
    } > RamAHB16

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        __bss_end__ = .;
        PROVIDE(end = .);
    } > RamLoc16

    /* NOINIT section for RamAHB16 */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
        *(.noinit_RAM2*)
        *(.noinit_RamAHB16*)
        . = ALIGN(4) ;
    } > RamAHB16

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*)
         . = ALIGN(4) ;
        _end_noinit = .;
    } > RamLoc16

    PROVIDE(_pvHeapStart = DEFINED(__user_heap_base) ? __user_heap_base : .);
    PROVIDE(_vStackTop = DEFINED(__user_stack_top) ? __user_stack_top : __top_RamLoc16 - 0);


    PROVIDE(_VectorChecksum = 0 - (_vStackTop + ResetISR + NMI_Handler + HardFault_Handler + MemManage_Handler + BusFault_Handler + UsageFault_Handler + 6 ));
}
