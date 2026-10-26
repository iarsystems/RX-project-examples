/*Non-Secure information */
FLASH_NS_IMAGE_START = 0x0200FFFC;
_SRAM0_NS_GLB_VAL_START = 0x0200BF00;
_PRE_INT_FUNC_NS = 0x0200F000;

_FLASH_NS_IMAGE_START = DEFINED(FLASH_NS_IMAGE_START) ? FLASH_NS_IMAGE_START : 0x0200FFFC;


MEMORY
{
/*	RAM_NS : ORIGIN = 0x02000000, LENGTH = 1572864 */
    RAM_NS : ORIGIN = 0x02008000, LENGTH = 0x8000
	
	ROM_NS : ORIGIN = 0x7FF00000, LENGTH = 1048576
	OFS1_NS : ORIGIN = 0x06C2E850, LENGTH = 462768
	OFS2_NS : ORIGIN = 0x06E00000, LENGTH = 200432
	OFS3_NS : ORIGIN = 0x06F41D00, LENGTH = 528

/*	RAM_S : ORIGIN = 0x82000000, LENGTH = 1572864 */
	RAM_S : ORIGIN = 0x82000000, LENGTH = 0x8000

	ROM_S : ORIGIN = 0xFFF00000, LENGTH = 1048576
	OFS1_S : ORIGIN = 0x86C2E850, LENGTH = 462768
	OFS2_S : ORIGIN = 0x86E00000, LENGTH = 200432
	OFS3_S : ORIGIN = 0x86F41D00, LENGTH = 528
}
SECTIONS
{
	.exvectors 0xFFFFFF80: AT(0xFFFFFF80)
	{
	    _exvectors_start = .;
		KEEP(*(.exvectors))
	} > ROM_S
	.fvectors 0xFFFFFFFC: AT(0xFFFFFFFC)
	{
		KEEP(*(.fvectors))
	} > ROM_S
	.text 0xFFF00000: AT(0xFFF00000)
	{
		*(.text)
		*(.text.*)
		*(P)
		etext = .;
	} > ROM_S
	.rvectors ALIGN(4):
	{
		_rvectors_start = .;
		KEEP(*(.rvectors))
		_rvectors_end = .;
	} > ROM_S
	.init :
	{
		KEEP(*(.init))
		__preinit_array_start = .;
		KEEP(*(.preinit_array))
		__preinit_array_end = .;
		__init_array_start = (. + 3) & ~ 3;
		KEEP(*(.init_array))
		KEEP(*(SORT(.init_array.*)))
		__init_array_end = .;
		__fini_array_start = .;
		KEEP(*(.fini_array))
		KEEP(*(SORT(.fini_array.*)))
		__fini_array_end = .;
	} > ROM_S
	.fini :
	{
		KEEP(*(.fini))
	} > ROM_S
	.got :
	{
		*(.got)
		*(.got.plt)
	} > ROM_S
	.rodata :
	{
		*(.rodata)
		*(.rodata.*)
		*(C_1)
		*(C_2)
		*(C)
		_erodata = .;
	} > ROM_S
	gcc_exceptions_table :
	{
	    KEEP (*(.gcc_except_table))
	    *(.gcc_except_table.*)
	} > ROM_S
	.eh_frame_hdr :
	{
		*(.eh_frame_hdr)
	} > ROM_S
	.eh_frame :
	{
		*(.eh_frame)
	} > ROM_S
	.jcr :
	{
		*(.jcr)
	} > ROM_S
	.tors :
	{
		__CTOR_LIST__ = .;
		. = ALIGN(2);
		___ctors = .;
		*(.ctors)
		___ctors_end = .;
		__CTOR_END__ = .;
		__DTOR_LIST__ = .;
		___dtors = .;
		*(.dtors)
		___dtors_end = .;
		__DTOR_END__ = .;
		. = ALIGN(2);
		_mdata = .;
	} > ROM_S

	.istack_s 0x82000300: AT(0x82000100)
	{
		_istack_s = .;
	} > RAM_S

	.ustack 0x82000200: AT(0x82000200)
	{
		_ustack = .;
	} > RAM_S
	.istack 0x82000100: AT(0x82000100)
	{
		_istack = .;
	} > RAM_S
	.data 0x82000204: AT(_mdata)
	{
		_data = .;
		*(.data)
		*(.data.*)
		*(D)
		*(D_1)
		*(D_2)
		*(.sdata.*)
		_edata = .;
	} > RAM_S
	.bss :
	{
		_bss = .;
		*(.bss)
		*(.bss.**)
		*(COMMON)
		*(B)
		*(B_1)
		*(B_2)
		_ebss = .;
		. = ALIGN(128);
		_end = .;
	} > RAM_S AT>RAM_S
	.banksel_ns 0x06C4EC60: AT(0x06C4EC60)
	{
		KEEP(*(.banksel_ns))
	} >OFS1_NS
	.blsel_ns 0x06C4EC70: AT(0x06C4EC70)
	{
		KEEP(*(.blsel_ns))
	} >OFS1_NS
	.bpsd_nsn 0x06C9F480: AT(0x06C9F480)
	{
		KEEP(*(.bpsd_nsn))
	} >OFS1_NS
	.bps_nsn 0x06C9F600: AT(0x06C9F600)
	{
		KEEP(*(.bps_nsn))
	} >OFS1_NS
	.dualsel 0x86C2E850: AT(0x86C2E850)
	{
		KEEP(*(.dualsel))
	} >OFS1_S
	.sas 0x86C2E870: AT(0x86C2E870)
	{
		KEEP(*(.sas))
	} >OFS1_S
	.sas2 0x86C2E874: AT(0x86C2E874)
	{
		KEEP(*(.sas2))
	} >OFS1_S
	.banksel_s 0x86C2E9B0: AT(0x86C2E9B0)
	{
		KEEP(*(.banksel_s))
	} >OFS1_S
	.blsel_s 0x86C2E9C0: AT(0x86C2E9C0)
	{
		KEEP(*(.blsel_s))
	} >OFS1_S
	.banksel_sel 0x86C2E9E0: AT(0x86C2E9E0)
	{
		KEEP(*(.banksel_sel))
	} >OFS1_S
	.blsel_sel 0x86C2E9F0: AT(0x86C2E9F0)
	{
		KEEP(*(.blsel_sel))
	} >OFS1_S
	.bpsd_sn 0x86C9F080: AT(0x86C9F080)
	{
		KEEP(*(.bpsd_sn))
	} >OFS1_S
	.bps_sn 0x86C9F200: AT(0x86C9F200)
	{
		KEEP(*(.bps_sn))
	} >OFS1_S
	.ofs1_ns 0x06E20850: AT(0x06E20850)
	{
		KEEP(*(.ofs1_ns))
	} >OFS2_NS
	.pbps_nsm 0x06E20C00: AT(0x06E20C00)
	{
		KEEP(*(.pbps_nsm))
	} >OFS2_NS
	.pbpsd_nsm 0x06E20C80: AT(0x06E20C80)
	{
		KEEP(*(.pbpsd_nsm))
	} >OFS2_NS
	.hoemrtpk0 0x86E00000: AT(0x86E00000)
	{
		KEEP(*(.hoemrtpk0))
	} >OFS2_S
	.hoemrtpk1 0x86E00030: AT(0x86E00030)
	{
		KEEP(*(.hoemrtpk1))
	} >OFS2_S
	.hoemrtpk2 0x86E00060: AT(0x86E00060)
	{
		KEEP(*(.hoemrtpk2))
	} >OFS2_S
	.hoemrtpk3 0x86E00090: AT(0x86E00090)
	{
		KEEP(*(.hoemrtpk3))
	} >OFS2_S
	.fsblctrl0 0x86E00400: AT(0x86E00400)
	{
		KEEP(*(.fsblctrl0))
	} >OFS2_S
	.fsblctrl1 0x86E00404: AT(0x86E00404)
	{
		KEEP(*(.fsblctrl1))
	} >OFS2_S
	.fsblctrl2 0x86E00408: AT(0x86E00408)
	{
		KEEP(*(.fsblctrl2))
	} >OFS2_S
	.samr 0x86E00414: AT(0x86E00414)
	{
		KEEP(*(.samr))
	} >OFS2_S
	.sacc00 0x86E00420: AT(0x86E00420)
	{
		KEEP(*(.sacc00))
	} >OFS2_S
	.sacc10 0x86E00430: AT(0x86E00430)
	{
		KEEP(*(.sacc10))
	} >OFS2_S
	.sacc01 0x86E00440: AT(0x86E00440)
	{
		KEEP(*(.sacc01))
	} >OFS2_S
	.sacc11 0x86E00450: AT(0x86E00450)
	{
		KEEP(*(.sacc11))
	} >OFS2_S
	.sacc02 0x86E00460: AT(0x86E00460)
	{
		KEEP(*(.sacc02))
	} >OFS2_S
	.sacc12 0x86E00470: AT(0x86E00470)
	{
		KEEP(*(.sacc12))
	} >OFS2_S
	.sacc03 0x86E00480: AT(0x86E00480)
	{
		KEEP(*(.sacc03))
	} >OFS2_S
	.sacc13 0x86E00490: AT(0x86E00490)
	{
		KEEP(*(.sacc13))
	} >OFS2_S
	.ofs0 0x86E10640: AT(0x86E10640)
	{
		KEEP(*(.ofs0))
	} >OFS2_S
	.ofs2 0x86E10644: AT(0x86E10644)
	{
		KEEP(*(.ofs2))
	} >OFS2_S
	.mde 0x86E1064C: AT(0x86E1064C)
	{
		KEEP(*(.mde))
	} >OFS2_S
	.ofs1_s 0x86E107A0: AT(0x86E107A0)
	{
		KEEP(*(.ofs1_s))
	} >OFS2_S
	.ofs1_sel 0x86E107D0: AT(0x86E107D0)
	{
		KEEP(*(.ofs1_sel))
	} >OFS2_S
	.pbps_sm 0x86E10A00: AT(0x86E10A00)
	{
		KEEP(*(.pbps_sm))
	} >OFS2_S
	.pbpsd_sm 0x86E10A80: AT(0x86E10A80)
	{
		KEEP(*(.pbpsd_sm))
	} >OFS2_S
	.revoke 0x86E30E10: AT(0x86E30E10)
	{
		KEEP(*(.revoke))
	} >OFS2_S
	.zhuk 0x86E30E20: AT(0x86E30E20)
	{
		KEEP(*(.zhuk))
	} >OFS2_S
	.arccs 0x86E30E30: AT(0x86E30E30)
	{
		KEEP(*(.arccs))
	} >OFS2_S
	.ufsblcr 0x86E30E50: AT(0x86E30E50)
	{
		KEEP(*(.ufsblcr))
	} >OFS2_S
	.pofspssn 0x86E30E60: AT(0x86E30E60)
	{
		KEEP(*(.pofspssn))
	} >OFS2_S
	.pofspsns0 0x86E30EE0: AT(0x86E30EE0)
	{
		KEEP(*(.pofspsns0))
	} >OFS2_S
	.arcs 0x86F41D00: AT(0x86F41D00)
	{
		KEEP(*(.arcs))
	} >OFS3_S
	.arcns0 0x86F41E00: AT(0x86F41E00)
	{
		KEEP(*(.arcns0))
	} >OFS3_S
	.arcns1 0x86F41E20: AT(0x86F41E20)
	{
		KEEP(*(.arcns1))
	} >OFS3_S
	.arcns2 0x86F41E40: AT(0x86F41E40)
	{
		KEEP(*(.arcns2))
	} >OFS3_S
	.arcns3 0x86F41E60: AT(0x86F41E60)
	{
		KEEP(*(.arcns3))
	} >OFS3_S
	.arcns 0x86F41E00: AT(0x86F41E00)
	{
		KEEP(*(.arcns))
	} >OFS3_S
	.arcoembl 0x86F41F00: AT(0x86F41F00)
	{
		KEEP(*(.arcoembl))
	} >OFS3_S
}
