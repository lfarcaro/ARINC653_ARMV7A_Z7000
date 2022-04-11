/* Reset handler */
ENTRY(PORT_RESETHANDLER)

/* Modules' stacks' sizes */
MODULE1_MODULE_STACK_SIZE = 0x00004000;
MODULE2_MODULE_STACK_SIZE = 0x00004000;

MEMORY
{

	/* Internal SRAM */

	/* Vector table regions */
	SRAM_MODULE1_MODULE_VECTORTABLE : ORIGIN = 0x00000000, LENGTH = 0x00001000
	SRAM_MODULE2_MODULE_VECTORTABLE : ORIGIN = 0x00001000, LENGTH = 0x00001000

	/* External DDR memory */

	/* System regions */
	DDR_SYSTEM_BOOT : ORIGIN = 0x00100000, LENGTH = 0x00001000 /* Boot region */
	DDR_SYSTEM_CODE : ORIGIN = 0x00101000, LENGTH = 0x00020000 /* Code region */
	DDR_SYSTEM_DATA : ORIGIN = 0x00121000, LENGTH = 0x00100000 /* Data region */

	/* MODULE1 module regions */
	DDR_MODULE1_MODULE_CODE : ORIGIN = 0x00221000, LENGTH = 0x00001000 /* Code region */
	DDR_MODULE1_MODULE_DATA : ORIGIN = 0x00222000, LENGTH = 0x00001000 /* Data region */

	/* PARTITION1 partition regions */
	DDR_PARTITION1_PARTITION_CODE : ORIGIN = 0x00223000, LENGTH = 0x00001000 /* Code region */
	DDR_PARTITION1_PARTITION_DATA : ORIGIN = 0x00224000, LENGTH = 0x00001000 /* Data region */

	/* MODULE2 module regions */
	DDR_MODULE2_MODULE_CODE : ORIGIN = 0x00225000, LENGTH = 0x00001000 /* Code region */
	DDR_MODULE2_MODULE_DATA : ORIGIN = 0x00226000, LENGTH = 0x00001000 /* Data region */

	/* PARTITION2 partition regions */
	DDR_PARTITION2_PARTITION_CODE : ORIGIN = 0x00227000, LENGTH = 0x00001000 /* Code region */
	DDR_PARTITION2_PARTITION_DATA : ORIGIN = 0x00228000, LENGTH = 0x00001000 /* Data region */

	/* Stack regions */
	/* ORIGIN = 0x00229000, LENGTH = 0x00001000 - Stack area start separator region */
	DDR_MODULE1_MODULE_STACK : ORIGIN = 0x0022A000, LENGTH = 0x00004000 /* System stack region */
	DDR_PARTITION1_PARTITION_DEFAULTPROCESS_STACK : ORIGIN = 0x0022E000, LENGTH = 0x00001000 /* PARTITION1 partition default process stack region */
	DDR_MODULE2_MODULE_STACK : ORIGIN = 0x0022F000, LENGTH = 0x00004000 /* System stack region */
	DDR_PARTITION2_PARTITION_DEFAULTPROCESS_STACK : ORIGIN = 0x00233000, LENGTH = 0x00001000 /* PARTITION2 partition default process stack region */
	/* ORIGIN = 0x00234000, LENGTH = 0x00001000 - Stack area end separator region */

	/* Heap regions */
	DDR_MODULE1_MODULE_HEAP : ORIGIN = 0x00235000, LENGTH = 0x00100000 /* MODULE1 module heap region */
	DDR_PARTITION1_PARTITION_HEAP : ORIGIN = 0x00335000, LENGTH = 0x00001000 /* PARTITION1 partition heap region */
	DDR_MODULE2_MODULE_HEAP : ORIGIN = 0x00336000, LENGTH = 0x00100000 /* MODULE2 module heap region */
	DDR_PARTITION2_PARTITION_HEAP : ORIGIN = 0x00436000, LENGTH = 0x00001000 /* PARTITION2 partition heap region */

	/* Translation table regions */
	DDR_MODULE1_MODULE_FLTRANSLATIONTABLE : ORIGIN = 0x00438000, LENGTH = 0x00100000 /* MODULE1 module FL translation table region */
	DDR_MODULE1_MODULE_SLTRANSLATIONTABLE : ORIGIN = 0x00538000, LENGTH = 0x00100000 /* MODULE1 module SL translation table region */
	DDR_MODULE2_MODULE_FLTRANSLATIONTABLE : ORIGIN = 0x00638000, LENGTH = 0x00100000 /* MODULE2 module FL translation table region */
	DDR_MODULE2_MODULE_SLTRANSLATIONTABLE : ORIGIN = 0x00738000, LENGTH = 0x00100000 /* MODULE2 module SL translation table region */

	/* Data regions' image regions */
	DDR_PARTITION1_PARTITION_DATA_IMAGE : ORIGIN = 0x00838000, LENGTH = 0x00001000 /* PARTITION1 partition data image region */
	DDR_PARTITION2_PARTITION_DATA_IMAGE : ORIGIN = 0x00839000, LENGTH = 0x00001000 /* PARTITION2 partition data image region */
}

SECTIONS
{

	/* External DDR memory */

	/* System regions */
	.system_code : { configuration*.o(.text) *configuration*.o(.text) } > DDR_SYSTEM_CODE
	.system_data : { configuration*.o(.bss, .rodata, .data) *configuration*.o(.bss, .rodata, .data) } > DDR_SYSTEM_DATA

	/* MODULE1 module regions */
	.module1_module_code : { module1_module*.o(.text) *module1_module*.o(.text) } > DDR_MODULE1_MODULE_CODE
	.module1_module_data : { module1_module*.o(.bss, .rodata, .data) *module1_module*.o(.bss, .rodata, .data) } > DDR_MODULE1_MODULE_DATA

	/* PARTITION1 partition region */
	.partition1_partition_code : { partition1_partition*.o(.text) *partition1_partition*.o(.text) } > DDR_PARTITION1_PARTITION_CODE
	.partition1_partition_data : { partition1_partition*.o(.bss, .rodata, .data) *partition1_partition*.o(.bss, .rodata, .data) } > DDR_PARTITION1_PARTITION_DATA

	/* MODULE2 module regions */
	.module2_module_code : { module2_module*.o(.text) *module2_module*.o(.text) } > DDR_MODULE2_MODULE_CODE
	.module2_module_data : { module2_module*.o(.bss, .rodata, .data) *module2_module*.o(.bss, .rodata, .data) } > DDR_MODULE2_MODULE_DATA

	/* PARTITION2 partition region */
	.partition2_partition_code : { partition2_partition*.o(.text) *partition2_partition*.o(.text) } > DDR_PARTITION2_PARTITION_CODE
	.partition2_partition_data : { partition2_partition*.o(.bss, .rodata, .data) *partition2_partition*.o(.bss, .rodata, .data) } > DDR_PARTITION2_PARTITION_DATA

	/* System region */
	.boot : {} > DDR_SYSTEM_BOOT
	.module1_module_stack (NOLOAD) : {
		. = ALIGN(16);
		_stack_end = .;
		_core0_module_stack_end = .;
		_module1_module_stack_end = .;
		. += MODULE1_MODULE_STACK_SIZE;
		. = ALIGN(16);
		_module1_module_stack_start = .;
		_core0_module_stack_start = .;
		_stack_start = .;
		__undef_stack = .;
	} > DDR_MODULE1_MODULE_STACK
	.module2_module_stack (NOLOAD) : {
		. = ALIGN(16);
		_core1_module_stack_end = .;
		_module2_module_stack_end = .;
		. += MODULE2_MODULE_STACK_SIZE;
		. = ALIGN(16);
		_module2_module_stack_start = .;
		_core1_module_stack_start = .;
	} > DDR_MODULE2_MODULE_STACK
	.text : {
		KEEP (*(.vectors))
		*(.boot)
		*(.text)
		*(.text.*)
		*(.gnu.linkonce.t.*)
		*(.plt)
		*(.gnu_warning)
		*(.gcc_execpt_table)
		*(.glue_7)
		*(.glue_7t)
		*(.vfp11_veneer)
		*(.ARM.extab)
		*(.gnu.linkonce.armextab.*)
	} > DDR_SYSTEM_CODE
	.bss (NOLOAD) : {
		_bss_start = .;
		*(.bss)
		*(.bss.*)
		*(.gnu.linkonce.b.*)
		*(.sbss)
		*(.sbss.*)
		*(.gnu.linkonce.sb.*)
		*(.sbss2)
		*(.sbss2.*)
		*(.gnu.linkonce.sb2.*)
		*(.tbss)
		*(.tbss.*)
		*(.gnu.linkonce.tb.*)
		*(COMMON)
		_bss_end = .;
	} > DDR_SYSTEM_DATA
	.rodata : {
		*(.rodata)
		*(.rodata.*)
		*(.rodata1)
		*(.rodata1.*)
		*(.gnu.linkonce.r.*)
	} > DDR_SYSTEM_DATA
	.data : {
		*(.data)
		*(.data.*)
		*(.data1)
		*(.data1.*)
		*(.gnu.linkonce.d.*)
		*(.sdata)
		*(.sdata.*)
		*(.sdata2)
		*(.sdata2.*)
		*(.gnu.linkonce.s.*)
		*(.gnu.linkonce.s2.*)
		*(.tdata)
		*(.tdata.*)
		*(.gnu.linkonce.td.*)
		*(.jcr)
		*(.got)
		*(.got.plt)
	} > DDR_SYSTEM_DATA
	.init : {
		KEEP (*(.init))
	} > DDR_SYSTEM_CODE
	.preinit_array : {
		__preinit_array_start = .;
		KEEP (*(SORT(.preinit_array.*)))
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
	} > DDR_SYSTEM_DATA
	.init_array : {
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;
	} > DDR_SYSTEM_DATA
	.ctors : {
		__CTOR_LIST__ = .;
		___CTORS_LIST___ = .;
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE(*crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))
		__CTOR_END__ = .;
		___CTORS_END___ = .;
	} > DDR_SYSTEM_DATA
	.fini : {
		KEEP (*(.fini))
	} > DDR_SYSTEM_CODE
	.fini_array : {
		__fini_array_start = .;
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array))
		__fini_array_end = .;
	} > DDR_SYSTEM_DATA
	.dtors : {
		__DTOR_LIST__ = .;
		___DTORS_LIST___ = .;
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE(*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
		__DTOR_END__ = .;
		___DTORS_END___ = .;
	} > DDR_SYSTEM_DATA
	.eh_frame : {
		*(.eh_frame)
	} > DDR_SYSTEM_DATA
	.eh_framehdr : {
		__eh_framehdr_start = .;
		*(.eh_framehdr)
		__eh_framehdr_end = .;
	} > DDR_SYSTEM_DATA
	.gcc_except_table : {
		*(.gcc_except_table)
	} > DDR_SYSTEM_DATA
	.ARM.exidx : {
		__exidx_start = .;
		*(.ARM.exidx*)
		*(.gnu.linkonce.armexidix.*.*)
		__exidx_end = .;
	} > DDR_SYSTEM_DATA
	.ARM.attributes : {
		__ARM.attributes_start = .;
		*(.ARM.attributes)
		__ARM.attributes_end = .;
	} > DDR_SYSTEM_DATA
}
