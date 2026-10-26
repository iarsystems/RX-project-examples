/******************************************************************************
* File Name		: r_usb_cdefusbip.h
* Version		: 2.00
* Description	: Definition of USB-spec macro & USB-IP macro
*******************************************************************************
* History	: DD.MM.YYYY Version Description
*			: 31.03.2012 1.80    Basic FW mini Ver2.00(ES Version)
*			: 30.11.2012 2.00
******************************************************************************/
#ifndef __R_USB_CDEFUSBIP_H__
#define __R_USB_CDEFUSBIP_H__


/*****************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
USB specification define(not change)
******************************************************************************/
/* Descriptor type	Define */
#define	USB_DT_DEVICE					(0x01u)		/* Configuration Descriptor */
#define	USB_DT_CONFIGURATION			(0x02u)		/* Configuration Descriptor */
#define	USB_DT_STRING					(0x03u)		/* Configuration Descriptor */
#define	USB_DT_INTERFACE				(0x04u)		/* Interface Descriptor */
#define	USB_DT_ENDPOINT					(0x05u)		/* Endpoint Descriptor */
#define	USB_DT_DEVICE_QUALIFIER			(0x06u)		/* Device Qualifier Descriptor */
#define	USB_DT_OTHER_SPEED_CONF			(0x07u)		/* Other Speed Configuration Descriptor */
#define	USB_DT_INTERFACE_POWER			(0x08u)		/* Interface Power Descriptor */
#define	USB_DT_OTGDESCRIPTOR			(0x09u)		/* OTG Descriptor */
#define	USB_DT_HUBDESCRIPTOR			(0x29u)		/* HUB descriptor */

/* Descriptor index */
#define	USB_DESCRIPTOR_SIZE			(0u)	/* Index of bLength in All Descriptors */
#define	USB_DESCRIPTOR_TYPE			(1u)	/* Index of bDescriptorType in All Descriptors */
#define	USB_DEV_MAX_PKT_SIZE		(7u)	/* Index of bMAXPacketSize in Device Descriptor */
#define	USB_DEV_VENDOR_LO			(8u)	/* Index of idVendor in Device Descriptor */
#define	USB_DEV_VENDOR_HI			(9u)	/* Index of idVendor in Device Descriptor */
#define	USB_DEV_PRODUCT_LO			(10u)	/* Index of idProduct in Device Descriptor */
#define	USB_DEV_PRODUCT_HI			(11u)	/* Index of idProduct in Device Descriptor */
#define	USB_DEV_IMANUFACTURER		(14u)	/* Index of iManufacturer in Device Descriptor */
#define	USB_DEV_IPRODUCT			(15u)	/* Index of iProduct in Device Descriptor */
#define	USB_DEV_ISERIALNUMBER		(16u)	/* Index of iSerialNumber in Device Descriptor */
#define	USB_DEV_NUM_CONFIG			(17u)	/* Index of bNumConfigurations in Device Descriptor */
#define	USB_CON_TOTALLENG_LO		(2u)	/* Index of wTotalLength in Configuration Descriptor */
#define	USB_CON_TOTALLENG_HI		(3u)	/* Index of wTotalLength in Configuration Descriptor */
#define	USB_CON_NUM_INTERFACE		(4u)	/* Index of bNumInterfaces in Configuration Descriptor */
#define	USB_CON_CONFIG_VAL			(5u)	/* Index of bConfigurationValue in Configuration Descriptor */
#define	USB_CON_ATTRIBUTE			(7u)	/* Index of bmAttributes in Configuration Descriptor */
#define	USB_INTF_NUM_INTERFACE		(2u)	/* Index of bInterfaceNumber in Interface Descriptor */
#define	USB_INTF_ALTERNATE			(3u)	/* Index of bAlternateSetting in Interface Descriptor */
#define	USB_INTF_CLASS				(5u)	/* Index of bInterfaceClass in Interface Descriptor */
#define	USB_EDNP_ADDRESS			(2u)	/* Index of bEndpointAddress in Endpoint Descriptor */
#define	USB_EDNP_ATTRIBUTE			(3u)	/* Index of bmAttributes in Endpoint Descriptor */
#define	USB_EDNP_MAXPACKETSIZE_LO	(4u)	/* Index of wMaxPacketSize in Endpoint Descriptor */
#define	USB_EDNP_MAXPACKETSIZE_HI	(5u)	/* Index of wMaxPacketSize in Endpoint Descriptor */
#define	USB_EDNP_INTERVAL			(6u)	/* Index of bInterval in Endpoint Descriptor */
#define	USB_SOFT_CHANGE				(0u)

/* Device class Define	*/
#define	USB_DEVCLS_INTF					(0x00u)		/* Class information at interface */
#define	USB_DEVCLS_COMM					(0x02u)		/* Communication Device */
#define	USB_DEVCLS_HUB					(0x90u)		/* HUB Device */
#define	USB_DEVCLS_DIAG					(0xDCu)		/* Diagnostic Device */
#define	USB_DEVCLS_WIRE					(0xE0u)		/* Wireless Controller */
#define	USB_DEVCLS_APL					(0xFEu)		/* Application-Specific */
#define	USB_DEVCLS_VEN					(0xFFu)		/* Vendor-Specific */

/* Interface class Define */
#define	USB_IFCLS_NOT					(0x00u)		/* Un corresponding Class */
#define	USB_IFCLS_AUD					(0x01u)		/* Audio Class */
#define	USB_IFCLS_CDCC					(0x02u)		/* CDC-Control Class */
#define	USB_IFCLS_HID					(0x03u)		/* HID Class */
#define	USB_IFCLS_PHY					(0x05u)		/* Physical Class */
#define	USB_IFCLS_IMG					(0x06u)		/* Image Class */
#define	USB_IFCLS_PRN					(0x07u)		/* Printer Class */
#define	USB_IFCLS_MAS					(0x08u)		/* Mass Storage Class */
#define	USB_IFCLS_HUB					(0x09u)		/* HUB Class */
#define	USB_IFCLS_CDCD					(0x0Au)		/* CDC-Data Class */
#define	USB_IFCLS_CHIP					(0x0Bu)		/* Chip/Smart Card Class */
#define	USB_IFCLS_CNT					(0x0Cu)		/* Content-Sequrity Class */
#define	USB_IFCLS_VID					(0x0Du)		/* Video Class */
#define	USB_IFCLS_DIAG					(0xDCu)		/* Diagnostic Device */
#define	USB_IFCLS_WIRE					(0xE0u)		/* Wireless Controller */
#define	USB_IFCLS_APL					(0xFEu)		/* Application-Specific */
#define	USB_IFCLS_VEN					(0xFFu)		/* Vendor-Specific Class */
#define	USB_IFCLS_HET					(0xAAu)		/* Host electrical test class */

/* Endpoint Descriptor	Define */
#define	USB_EP_DIRMASK					(0x80u)		/* Endpoint direction mask [2] */
#define	USB_EP_IN						(0x80u)		/* In  Endpoint */
#define	USB_EP_OUT						(0x00u)		/* Out Endpoint */
#define	USB_EP_NUMMASK					(0x0Fu)		/* Endpoint number mask [2] */
#define	USB_EP_USGMASK					(0x30u)		/* Usage type mask [2] */
#define	USB_EP_SYNCMASK					(0x0Cu)		/* Synchronization type mask [2] */
#define	USB_EP_TRNSMASK					(0x03u)		/* Transfer type mask [2] */
#define	USB_EP_CNTRL					(0x00u)		/* Control	   Transfer */
#define	USB_EP_ISO						(0x01u)		/* Isochronous Transfer */
#define	USB_EP_BULK						(0x02u)		/* Bulk		   Transfer */
#define	USB_EP_INT						(0x03u)		/* Interrupt   Transfer */

/* Configuration descriptor bit define */
#define	USB_CF_RESERVED					(0x80u)		/* Reserved(set to 1) */
#define	USB_CF_SELFP					(0x40u)		/* Self Powered */
#define	USB_CF_BUSP						(0x00u)		/* Bus Powered */
#define	USB_CF_RWUPON					(0x20u)		/* Remote Wakeup ON */
#define	USB_CF_RWUPOFF					(0x00u)		/* Remote Wakeup OFF */

/* OTG descriptor bit define */
#define	USB_OTG_HNP						(0x02u)		/* HNP support */
#define	USB_OTG_SRP						(0x01u)		/* SRP support */

/* USB Standard request */
/* USB_BREQUEST				0xFF00u(b15-8) */
#define	USB_GET_STATUS					(0x00u)
#define	USB_CLEAR_FEATURE				(0x01u)
#define	USB_REQRESERVED					(0x02u)
#define	USB_SET_FEATURE					(0x03u)
#define	USB_REQRESERVED1				(0x04u)
#define	USB_SET_ADDRESS					(0x05u)
#define	USB_GET_DESCRIPTOR				(0x06u)
#define	USB_SET_DESCRIPTOR				(0x07u)
#define	USB_GET_CONFIGURATION			(0x08u)
#define	USB_SET_CONFIGURATION			(0x09u)
#define	USB_GET_INTERFACE				(0x0Au)
#define	USB_SET_INTERFACE				(0x0Bu)
#define	USB_SYNCH_FRAME					(0x0Cu)

/* USB_BMREQUESTTYPEDIR		0x0080u(b7) */
#define	USB_HOST_TO_DEV					(0u)
#define	USB_DEV_TO_HOST					(1u)
#define	USB_REQUEST_TYPE(x,y,z)			(uint8_t)((uint8_t)(((uint8_t)x<<7)|((uint8_t)y<<5))|z)

/* USB_BMREQUESTTYPETYPE	0x0060u(b6-5) */
#define	USB_STANDARD					(0u)
#define	USB_CLASS						(1u)
#define	USB_VENDOR						(2u)

/* USB_BMREQUESTTYPERECIP	0x001Fu(b4-0) */
#define	USB_DEVICE						(0u)
#define	USB_INTERFACE					(1u)
#define	USB_ENDPOINT					(2u)
#define	USB_OTHER						(3u)

/* GET_STATUS request information */
/* Standard Device status */
#define	USB_GS_BUSPOWERD				(0x0000u)
#define	USB_GS_SELFPOWERD				(0x0001u)
#define	USB_GS_REMOTEWAKEUP				(0x0002u)

/* Endpoint status */
#define	USB_GS_NOTHALT					(0x0000u)
#define	USB_GS_HALT						(0x0001u)

/* CLEAR_FEATURE/GET_FEATURE/SET_FEATURE request information */
/* Standard Feature Selector */
#define	USB_ENDPOINT_HALT				(0x0000u)
#define	USB_DEVICE_REMOTE_WAKEUP		(0x0001u)
#define	USB_TEST_MODE					(0x0002u)

/* GET_DESCRIPTOR/SET_DESCRIPTOR request information */
/* Standard Descriptor type */
#define	USB_HUB_DESCRIPTOR				(0x0000u)
#define	USB_DEV_DESCRIPTOR				(0x0100u)
#define	USB_CONF_DESCRIPTOR				(0x0200u)
#define	USB_STRING_DESCRIPTOR			(0x0300u)
#define	USB_INTERFACE_DESCRIPTOR		(0x0400u)
#define	USB_ENDPOINT_DESCRIPTOR			(0x0500u)
#define	USB_DEV_QUALIFIER_DESCRIPTOR	(0x0600u)
#define	USB_OTHER_SPEED_CONF_DESCRIPTOR	(0x0700u)
#define	USB_INTERFACE_POWER_DESCRIPTOR	(0x0800u)

/* VendorID , ProductID define	*/
#define	USB_NOVENDOR		(0xFFFFu)	/* Vendor ID nocheck */
#define	USB_NOPRODUCT		(0xFFFFu)	/* Product ID nocheck */


/******************************************************************************
USB-H/W register define(not change)
******************************************************************************/
/* Root port */
#define	USB_PORT0			(0u)
#define	USB_PORT1			(1u)

/* Device connect information */
#define	USB_LNST_ATTACH		(0x0041u)
#define	USB_LNST_DETACH		(0x0042u)
#define	USB_RHST_ATTACH		(0x0043u)

/* Reset Handshake result */
#define	USB_NOCONNECT		(0xFFu)		/* Speed undecidedness */
#define	USB_FSCONNECT		(0x01u)		/* Full-Speed connect */
#define	USB_HSCONNECT		(0x02u)		/* Hi-Speed connect */
#define	USB_LSCONNECT		(0x03u)		/* Low-Speed connect */
#define	USB_CONNECT			(0xF0u)

/* Pipe define */
#define	USB_USEPIPE			(0x00FEu)
#define	USB_PERIPIPE		(0x00FDu)
#define	USB_PIPE0			(0x0000u)	/* PIPE 0 */
#define	USB_PIPE1			(0x0001u)	/* PIPE 1 */
#define	USB_PIPE2			(0x0002u)	/* PIPE 2 */
#define	USB_PIPE3			(0x0003u)	/* PIPE 3 */
#define	USB_PIPE4			(0x0004u)	/* PIPE 4 */
#define	USB_PIPE5			(0x0005u)	/* PIPE 5 */
#define	USB_PIPE6			(0x0006u)	/* PIPE 6 */
#define	USB_PIPE7			(0x0007u)	/* PIPE 7 */
#define	USB_PIPE8			(0x0008u)	/* PIPE 8 */
#define	USB_PIPE9			(0x0009u)	/* PIPE 9 */

#ifdef  __RX
#define	USB_MIN_PIPE_NO		USB_PIPE1	/* PIPE1 ... PIPE9 */
#define	USB_MAX_PIPE_NO		USB_PIPE9	/* PIPE1 ... PIPE9 */
#else
#define	USB_MIN_PIPE_NO		USB_PIPE4	/* PIPE4 ... PIPE7 */
#define	USB_MAX_PIPE_NO		USB_PIPE7	/* PIPE4 ... PIPE7 */
#endif  /* __RX */

/* Pipe configuration table define */
#define	USB_EPL				4u			/* Pipe configuration table length */
#define	USB_TYPFIELD		(0xC000u)	/* Transfer type */
#define	  USB_PERIODIC		(0x8000u)	/* Periodic pipe */
#define	  USB_ISO			(0xC000u)	/* Isochronous */
#define	  USB_INT			(0x8000u)	/* Interrupt */
#define	  USB_BULK			(0x4000u)	/* Bulk */
#define	  USB_NOUSE			(0x0000u)	/* Not configuration */
#define	USB_BFREFIELD		(0x0400u)	/* Buffer ready interrupt mode select */
#define	  USB_BFREON		(0x0400u)
#define	  USB_BFREOFF		(0x0000u)
#define	USB_DBLBFIELD		(0x0200u)	/* Double buffer mode select */
#define	  USB_DBLBON		(0x0200u)
#define	  USB_DBLBOFF		(0x0000u)
#define	USB_CNTMDFIELD		(0x0100u)	/* Continuous transfer mode select */
#define	  USB_CNTMDON		(0x0100u)
#define	  USB_CNTMDOFF		(0x0000u)
#define	USB_SHTNAKFIELD		(0x0080u)	/* Transfer end NAK */
#define	  USB_SHTNAKON		(0x0080u)
#define	  USB_SHTNAKOFF		(0x0000u)
#define	USB_DIRFIELD		(0x0010u)	/* Transfer direction select */
#define	  USB_DIR_H_OUT		(0x0010u)	/* HOST OUT */
#define	  USB_DIR_P_IN		(0x0010u)	/* PERI IN */
#define	  USB_DIR_H_IN		(0x0000u)	/* HOST IN */
#define	  USB_DIR_P_OUT		(0x0000u)	/* PERI OUT */
#define	  USB_BUF2FIFO		(0x0010u)	/* Buffer --> FIFO */
#define	  USB_FIFO2BUF		(0x0000u)	/* FIFO --> buffer */
#define	USB_EPNUMFIELD		(0x000Fu)	/* Endpoint number select */
#define	USB_MAX_EP_NO		(15u)		/* EP0 EP1 ... EP15 */
#define	  USB_EP0			(0x0000u)
#define	  USB_EP1			(0x0001u)
#define	  USB_EP2			(0x0002u)
#define	  USB_EP3			(0x0003u)
#define	  USB_EP4			(0x0004u)
#define	  USB_EP5			(0x0005u)
#define	  USB_EP6			(0x0006u)
#define	  USB_EP7			(0x0007u)
#define	  USB_EP8			(0x0008u)
#define	  USB_EP9			(0x0009u)
#define	  USB_EP10			(0x000Au)
#define	  USB_EP11			(0x000Bu)
#define	  USB_EP12			(0x000Cu)
#define	  USB_EP13			(0x000Du)
#define	  USB_EP14			(0x000Eu)
#define	  USB_EP15			(0x000Fu)

#define	USB_BUF_SIZE(x)	((uint16_t)(((x) / 64u) - 1u) << 10u)
#define	USB_BUF_NUMB(x)	(x)

#define	USB_MAX_PACKET(x)	(x)

#define	USB_IFISFIELD		(0x1000u)	/* Isochronous in-buf flash mode */
#define	  USB_IFISON		(0x1000u)
#define	  USB_IFISOFF		(0x0000u)
#define	USB_IITVFIELD		(0x0007u)	/* Isochronous interval */
#define	USB_IITV_TIME(x)	(x)

/* FIFO port & access define */
#define	USB_CUSE			(0u)		/* CFIFO CPU trans */
#define	USB_CUSE2			(5u)		/* CFIFO CPU trans (not tran count) */

/* DEVICE address */
#define	  USB_DEVICE_0		(0x00u)		/* Device address 0 */
#define	  USB_DEVICE_1		(0x01u)		/* Device address 1 */
#define	  USB_DEVICE_2		(0x02u)		/* Device address 2 */
#define	  USB_DEVICE_3		(0x03u)		/* Device address 3 */
#define	  USB_DEVICE_4		(0x04u)		/* Device address 4 */
#define	  USB_DEVICE_5		(0x05u)		/* Device address 5 */
#define	  USB_DEVICE_6		(0x06u)		/* Device address 6 */
#define	  USB_DEVICE_7		(0x07u)		/* Device address 7 */
#define	  USB_DEVICE_8		(0x08u)		/* Device address 8 */
#define	  USB_DEVICE_9		(0x09u)		/* Device address 9 */
#define	  USB_DEVICE_A		(0x0Au)		/* Device address A */
#define	  USB_NODEVICE		(0x0Fu)		/* No device */
#define	  USB_DEVADDRBIT	(12u)


/*****************************************************************************
USB_597IP_PP Register definition(not change)
******************************************************************************/
/* System Configuration Control Register */
/*	USB_SYSCFG0		(*((REGP*)(USB_BASE+0x00))) */
#define	USB_XTAL				(0xC000u)	/* b15-14: Crystal selection */
#define	  USB_XTAL48			(0x8000u)		/* 48MHz */
#define	  USB_XTAL24			(0x4000u)		/* 24MHz */
#define	  USB_XTAL12			(0x0000u)		/* 12MHz */
#define	USB_XCKE				(0x2000u)	/* b13: External clock enable */
#define	USB_PLLC				(0x0800u)	/* b11: PLL control */
#define	USB_SCKE				(0x0400u)	/* b10: USB clock enable */
#define	USB_CNEN				(0x0100u)	/* b8: To copy from the english DS for BC.  */
#define	USB_HSE					(0x0080u)	/* b7: Hi-speed enable */
#define	USB_DCFM				(0x0040u)	/* b6: Function select */
#define	USB_DRPD				(0x0020u)	/* b5: D+/D- pull down control */
#define	USB_DPRPU				(0x0010u)	/* b4: D+ pull up control */
#define	USB_DMRPU				(0x0008u)	/* b4: D- pull up control */
#define	USB_USBE				(0x0001u)	/* b0: USB module enable */
/*	USB_SYSCFG1		(*((REGP*)(USB_BASE+0x02))) */
#define	USB_CNTFLG				(0x1000u)	/* b12: Auto clock monitor */
#define	USB_PCSDIS				(0x0200u)	/* b9: not CS wakeup */
#define	USB_LPSME				(0x0100u)	/* b8: Low power sleep mode */
//#define	USB_HSE					(0x0080u)	/* b7: Hi-speed enable */
//#define	USB_DRPD				(0x0020u)	/* b5: D+/D- pull down control */

/* System Configuration Status Register */
/*	USB_SYSSTS0		(*((REGP*)(USB_BASE+0x04))) */
/*	USB_SYSSTS1		(*((REGP*)(USB_BASE+0x06))) */
#define	USB_OVCBIT				(0x8000u)	/* b15-14: Over-current bit */
#define	USB_OVCMON				(0xC000u)	/* b15-14: Over-current monitor */
#define	USB_HTACT				(0x0040u)	/* b6: Sequencer status monitor */
#define	USB_SOFEA				(0x0020u)	/* b5: SOF monitor */
#define	USB_IDMON				(0x0004u)	/* b2: ID-pin monitor */
#define	USB_LNST				(0x0003u)	/* b1-0: D+, D- line status */
#define	  USB_SE1				(0x0003u)		/* SE1 */
#define	  USB_FS_KSTS			(0x0002u)		/* Full-Speed K State */
#define	  USB_FS_JSTS			(0x0001u)		/* Full-Speed J State */
#define	  USB_LS_JSTS			(0x0002u)		/* Low-Speed J State */
#define	  USB_LS_KSTS			(0x0001u)		/* Low-Speed K State */
#define	  USB_SE0				(0x0000u)		/* SE0 */

/* Device State Control Register */
/*	USB_DVSTCTR0		(*((REGP*)(USB_BASE+0x08))) */
/*	USB_DVSTCTR1		(*((REGP*)(USB_BASE+0x0A))) */
#define	USB_HNPBTOA			(0x0800u)	/* b11: Host negotiation protocol (BtoA) */
#define	USB_EXTLP0				(0x0400u)	/* b10: External port */
#define	USB_VBOUT				(0x0200u)	/* b9: VBUS output */
#define	USB_WKUP				(0x0100u)	/* b8: Remote wakeup */
#define	USB_RWUPE				(0x0080u)	/* b7: Remote wakeup sense */
#define	USB_USBRST				(0x0040u)	/* b6: USB reset enable */
#define	USB_RESUME				(0x0020u)	/* b5: Resume enable */
#define	USB_UACT				(0x0010u)	/* b4: USB bus enable */
#define	USB_RHST				(0x0007u)	/* b2-0: Reset handshake status */
#define	  USB_HSPROC			(0x0004u)		/* HS handshake processing */
#define	  USB_HSMODE			(0x0003u)		/* Hi-Speed mode */
#define	  USB_FSMODE			(0x0002u)		/* Full-Speed mode */
#define	  USB_LSMODE			(0x0001u)		/* Low-Speed mode */
#define	  USB_UNDECID			(0x0000u)		/* Undecided */

/* Test Mode Register */
/*	USB_TESTMODE		(*((REGP*)(USB_BASE+0x0C))) */
#define	USB_UTST				(0x000Fu)	/* b3-0: Test select */
#define	  USB_H_TST_PACKET		(0x000Cu)		/* HOST TEST Packet */
#define	  USB_H_TST_SE0_NAK		(0x000Bu)		/* HOST TEST SE0 NAK */
#define	  USB_H_TST_K			(0x000Au)		/* HOST TEST K */
#define	  USB_H_TST_J			(0x0009u)		/* HOST TEST J */
#define	  USB_H_TST_NORMAL		(0x0000u)		/* HOST Normal Mode */
#define	  USB_P_TST_PACKET		(0x0004u)		/* PERI TEST Packet */
#define	  USB_P_TST_SE0_NAK		(0x0003u)		/* PERI TEST SE0 NAK */
#define	  USB_P_TST_K			(0x0002u)		/* PERI TEST K */
#define	  USB_P_TST_J			(0x0001u)		/* PERI TEST J */
#define	  USB_P_TST_NORMAL		(0x0000u)		/* PERI Normal Mode */

/* Data Pin Configuration Register */
/*	USB_PINCFG			(*((REGP*)(USB_BASE+0x0E))) */
#define	USB_LDRV				(0x8000u)	/* b15: Drive Current Adjust */
#define	  USB_VIF1				(0x0000u)		/* VIF = 1.8V */
#define	  USB_VIF3				(0x8000u)		/* VIF = 3.3V */
#define	USB_INTA				(0x0001u)	/* b1: USB INT-pin active */


/* DMAx Pin Configuration Register */
/*	USB_DMA0CFG		(*((REGP*)(USB_BASE+0x10))) */
/*	USB_DMA1CFG		(*((REGP*)(USB_BASE+0x12))) */
#define	USB_DREQA				(0x4000u)	/* b14: Dreq active select */
#define	USB_BURST				(0x2000u)	/* b13: Burst mode */
#define	USB_DACKA				(0x0400u)	/* b10: Dack active select */
#define	USB_DFORM				(0x0380u)	/* b9-7: DMA mode select */
#define	  USB_CPU_ADR_RD_WR		(0x0000u)		/* Address + RD/WR mode (CPU bus) */
#define	  USB_CPU_DACK_RD_WR	(0x0100u)		/* DACK + RD/WR (CPU bus) */
#define	  USB_CPU_DACK_ONLY		(0x0180u)		/* DACK only (CPU bus) */
#define	  USB_SPLIT_DACK_ONLY	(0x0200u)		/* DACK only (SPLIT bus) */
#define	USB_DENDA				(0x0040u)	/* b6: Dend active select */
#define	USB_PKTM				(0x0020u)	/* b5: Packet mode */
#define	USB_DENDE				(0x0010u)	/* b4: Dend enable */
#define	USB_OBUS				(0x0004u)	/* b2: OUTbus mode */


/* CFIFO/DxFIFO Port Register */
/*	USB_CFIFO			(*((REGP*)(USB_BASE+0x14))) */
/*	USB_CFIFO_8			(*((REGP8*)(USB_BASE+0x14))) */
/*	USB_D0FIFO			(*((REGP*)(USB_BASE+0x18))) */
/*	USB_D0FIFO_8		(*((REGP8*)(USB_BASE+0x18))) */
/*	USB_D1FIFO			(*((REGP*)(USB_BASE+0x1C))) */
/*	USB_D1FIFO_8		(*((REGP8*)(USB_BASE+0x1C))) */

/* CFIFO/DxFIFO Port Select Register */
/*	USB_CFIFOSEL		(*((REGP*)(USB_BASE+0x20))) */
/*	USB_D0FIFOSEL		(*((REGP*)(USB_BASE+0x28))) */
/*	USB_D1FIFOSEL		(*((REGP*)(USB_BASE+0x2C))) */
#define	USB_RCNT				(0x8000u)	/* b15: Read count mode */
#define	USB_REW					(0x4000u)	/* b14: Buffer rewind */
#define	USB_DCLRM				(0x2000u)	/* b13: DMA buffer clear mode */
#define	USB_DREQE				(0x1000u)	/* b12: DREQ output enable */
#define	USB_MBW					(0x0400u)	/* b10: Maximum bit width for FIFO access */
#define	  USB_MBW_8				(0x0000u)		/*  8bit */
#define	  USB_MBW_16			(0x0400u)		/* 16bit */
#define	USB_BIGEND				(0x0100u)	/* b8: Big endian mode */
#define	  USB_FIFO_LITTLE		(0x0000u)		/* Little dendian */
#define	  USB_FIFO_BIG			(0x0100u)		/* Big endifan */
#define	USB_ISEL				(0x0020u)	/* b5: DCP FIFO port direction select */
#define	USB_CURPIPE				(0x000Fu)	/* b2-0: PIPE select */

/* CFIFO/DxFIFO Port Control Register */
/*	USB_CFIFOCTR		(*((REGP*)(USB_BASE+0x22))) */
/*	USB_D0FIFOCTR		(*((REGP*)(USB_BASE+0x2A))) */
/*	USB_D1FIFOCTR		(*((REGP*)(USB_BASE+0x2E))) */
#define	USB_BVAL				(0x8000u)	/* b15: Buffer valid flag */
#define	USB_BCLR				(0x4000u)	/* b14: Buffer clear */
#define	USB_FRDY				(0x2000u)	/* b13: FIFO ready */
#define	USB_DTLN				(0x0FFFu)	/* b11-0: FIFO data length */


/* Interrupt Enable Register 0 */
/*	USB_INTENB0		(*((REGP*)(USB_BASE+0x30))) */
#define	USB_VBSE				(0x8000u)	/* b15: VBUS interrupt */
#define	USB_RSME				(0x4000u)	/* b14: Resume interrupt */
#define	USB_SOFE				(0x2000u)	/* b13: Frame update interrupt */
#define	USB_DVSE				(0x1000u)	/* b12: Device state transition interrupt */
#define	USB_CTRE				(0x0800u)	/* b11: Control transfer stage transition interrupt */
#define	USB_BEMPE				(0x0400u)	/* b10: Buffer empty interrupt */
#define	USB_NRDYE				(0x0200u)	/* b9: Buffer notready interrupt */
#define	USB_BRDYE				(0x0100u)	/* b8: Buffer ready interrupt */

/* Interrupt Enable Register 1 */
/*	USB_INTENB1		(*((REGP*)(USB_BASE+0x32))) */
/*	USB_INTENB2		(*((REGP*)(USB_BASE+0x34))) */
#define	USB_OVRCRE				(0x8000u)	/* b15: Over-current interrupt */
#define	USB_BCHGE				(0x4000u)	/* b14: USB us chenge interrupt */
#define	USB_DTCHE				(0x1000u)	/* b12: Detach sense interrupt */
#define	USB_ATTCHE				(0x0800u)	/* b11: Attach sense interrupt */
#define	USB_EOFERRE				(0x0040u)	/* b6: EOF error interrupt */
#define	USB_SIGNE				(0x0020u)	/* b5: SETUP IGNORE interrupt */
#define	USB_SACKE				(0x0010u)	/* b4: SETUP ACK interrupt */
#define	USB_PDDETINTE			(0x0001u)	/* b0: SETUP PDDETINT interrupt */

/* BRDY Interrupt Enable/Status Register */
/*	USB_BRDYENB		(*((REGP*)(USB_BASE+0x36))) */
/*	USB_BRDYSTS		(*((REGP*)(USB_BASE+0x46))) */
#define	USB_BRDY9				(0x0200u)	/* b9: PIPE9 */
#define	USB_BRDY8				(0x0100u)	/* b8: PIPE8 */
#define	USB_BRDY7				(0x0080u)	/* b7: PIPE7 */
#define	USB_BRDY6				(0x0040u)	/* b6: PIPE6 */
#define	USB_BRDY5				(0x0020u)	/* b5: PIPE5 */
#define	USB_BRDY4				(0x0010u)	/* b4: PIPE4 */
#define	USB_BRDY3				(0x0008u)	/* b3: PIPE3 */
#define	USB_BRDY2				(0x0004u)	/* b2: PIPE2 */
#define	USB_BRDY1				(0x0002u)	/* b1: PIPE1 */
#define	USB_BRDY0				(0x0001u)	/* b1: PIPE0 */

/* NRDY Interrupt Enable/Status Register */
/*	USB_NRDYENB		(*((REGP*)(USB_BASE+0x38))) */
/*	USB_NRDYSTS		(*((REGP*)(USB_BASE+0x48))) */
#define	USB_NRDY9				(0x0200u)	/* b9: PIPE9 */
#define	USB_NRDY8				(0x0100u)	/* b8: PIPE8 */
#define	USB_NRDY7				(0x0080u)	/* b7: PIPE7 */
#define	USB_NRDY6				(0x0040u)	/* b6: PIPE6 */
#define	USB_NRDY5				(0x0020u)	/* b5: PIPE5 */
#define	USB_NRDY4				(0x0010u)	/* b4: PIPE4 */
#define	USB_NRDY3				(0x0008u)	/* b3: PIPE3 */
#define	USB_NRDY2				(0x0004u)	/* b2: PIPE2 */
#define	USB_NRDY1				(0x0002u)	/* b1: PIPE1 */
#define	USB_NRDY0				(0x0001u)	/* b1: PIPE0 */

/* BEMP Interrupt Enable/Status Register */
/*	USB_BEMPENB		(*((REGP*)(USB_BASE+0x3A))) */
/*	USB_BEMPSTS		(*((REGP*)(USB_BASE+0x4A))) */
#define	USB_BEMP9				(0x0200u)	/* b9: PIPE9 */
#define	USB_BEMP8				(0x0100u)	/* b8: PIPE8 */
#define	USB_BEMP7				(0x0080u)	/* b7: PIPE7 */
#define	USB_BEMP6				(0x0040u)	/* b6: PIPE6 */
#define	USB_BEMP5				(0x0020u)	/* b5: PIPE5 */
#define	USB_BEMP4				(0x0010u)	/* b4: PIPE4 */
#define	USB_BEMP3				(0x0008u)	/* b3: PIPE3 */
#define	USB_BEMP2				(0x0004u)	/* b2: PIPE2 */
#define	USB_BEMP1				(0x0002u)	/* b1: PIPE1 */
#define	USB_BEMP0				(0x0001u)	/* b0: PIPE0 */

/* SOF Pin Configuration Register */
/*	USB_SOFCFG			(*((REGP*)(USB_BASE+0x3C))) */
#define	USB_TRNENSEL			(0x0100u)	/* b8: Select transaction enable period */
#define	USB_BRDYM				(0x0040u)	/* b6: BRDY clear timing */
#define	USB_INTL				(0x0020u)	/* b5: Interrupt sense select */
#define	USB_EDGESTS				(0x0010u)	/* b4:  */
#define	USB_SOFMODE				(0x000Cu)	/* b3-2: SOF pin select */
#define	  USB_SOF_125US			(0x0008u)		/* SOF 125us Frame Signal */
#define	  USB_SOF_1MS			(0x0004u)		/* SOF 1ms Frame Signal */
#define	  USB_SOF_DISABLE		(0x0000u)		/* SOF Disable */


/* Interrupt Status Register 0 */
/*	USB_INTSTS0		(*((REGP*)(USB_BASE+0x40))) */
#define	USB_VBINT				(0x8000u)	/* b15: VBUS interrupt */
#define	USB_RESM				(0x4000u)	/* b14: Resume interrupt */
#define	USB_SOFR				(0x2000u)	/* b13: SOF update interrupt */
#define	USB_DVST				(0x1000u)	/* b12: Device state transition interrupt */
#define	USB_CTRT				(0x0800u)	/* b11: Control transfer stage transition interrupt */
#define	USB_BEMP				(0x0400u)	/* b10: Buffer empty interrupt */
#define	USB_NRDY				(0x0200u)	/* b9: Buffer notready interrupt */
#define	USB_BRDY				(0x0100u)	/* b8: Buffer ready interrupt */
#define	USB_VBSTS				(0x0080u)	/* b7: VBUS input port */
#define	USB_DVSQ				(0x0070u)	/* b6-4: Device state */
#define	  USB_DS_SPD_CNFG		(0x0070u)		/* Suspend Configured */
#define	  USB_DS_SPD_ADDR		(0x0060u)		/* Suspend Address */
#define	  USB_DS_SPD_DFLT		(0x0050u)		/* Suspend Default */
#define	  USB_DS_SPD_POWR		(0x0040u)		/* Suspend Powered */
#define	  USB_DS_SUSP			(0x0040u)		/* Suspend */
#define	  USB_DS_CNFG			(0x0030u)		/* Configured */
#define	  USB_DS_ADDS			(0x0020u)		/* Address */
#define	  USB_DS_DFLT			(0x0010u)		/* Default */
#define	  USB_DS_POWR			(0x0000u)		/* Powered */
#define	USB_DVSQS				(0x0030u)	/* b5-4: Device state */
#define	USB_VALID				(0x0008u)	/* b3: Setup packet detect flag */
#define	USB_CTSQ				(0x0007u)	/* b2-0: Control transfer stage */
#define	  USB_CS_SQER			(0x0006u)		/* Sequence error */
#define	  USB_CS_WRND			(0x0005u)		/* Ctrl write nodata status stage */
#define	  USB_CS_WRSS			(0x0004u)		/* Ctrl write status stage */
#define	  USB_CS_WRDS			(0x0003u)		/* Ctrl write data stage */
#define	  USB_CS_RDSS			(0x0002u)		/* Ctrl read status stage */
#define	  USB_CS_RDDS			(0x0001u)		/* Ctrl read data stage */
#define	  USB_CS_IDST			(0x0000u)		/* Idle or setup stage */

/* Interrupt Status Register 1 */
/*	USB_INTSTS1		(*((REGP*)(USB_BASE+0x42))) */
/*	USB_INTSTS2		(*((REGP*)(USB_BASE+0x44))) */
#define	USB_OVRCR				(0x8000u)	/* b15: Over-current interrupt */
#define	USB_BCHG				(0x4000u)	/* b14: USB bus chenge interrupt */
#define	USB_DTCH				(0x1000u)	/* b12: Detach sense interrupt */
#define	USB_ATTCH				(0x0800u)	/* b11: Attach sense interrupt */
#define	USB_EOFERR				(0x0040u)	/* b6: EOF-error interrupt */
#define	USB_SIGN				(0x0020u)	/* b5: Setup ignore interrupt */
#define	USB_SACK				(0x0010u)	/* b4: Setup ack interrupt */
#define	USB_PDDETINT			(0x0001u)	/* b0: Pddetint sense interrupt */

/* Frame Number Register */
/*	USB_FRMNUM			(*((REGP*)(USB_BASE+0x4C))) */
#define	USB_OVRN				(0x8000u)	/* b15: Overrun error */
#define	USB_CRCE				(0x4000u)	/* b14: Received data error */
#define	USB_FRNM				(0x07FFu)	/* b10-0: Frame number */

/* Micro Frame Number Register */
/*	USB_UFRMNUM		(*((REGP*)(USB_BASE+0x4E))) */
#define	USB_UFRNM				(0x0007u)	/* b2-0: Micro frame number */


/* USB Address / Low Power Status Recovery Register */
/*	USB_USBADDR		(*((REGP*)(USB_BASE+0x50))) */
#define	USB_USBADDR_MASK		(0x007Fu)	/* b6-0: USB address */
#define	USB_STSRECOV			(0x0F00u)	/* b11-8: Status Recovery */
#define	  USB_STSR_SET			(0x0800u)

/* USB Request Type Register */
/*	USB_USBREQ			(*((REGP*)(USB_BASE+0x54))) */
#define	USB_BREQUEST			(0xFF00u)	/* b15-8: USB_BREQUEST */
#define	USB_BMREQUESTTYPE		(0x00FFu)	/* b7-0: USB_BMREQUESTTYPE */
#define	USB_BMREQUESTTYPEDIR	(0x0080u)	/* b7  : Data transfer direction */
#define	USB_BMREQUESTTYPETYPE	(0x0060u)	/* b6-5: Type */
#define	USB_BMREQUESTTYPERECIP	(0x001Fu)	/* b4-0: Recipient */

/* USB Request Value Register */
/*	USB_USBVAL			(*((REGP*)(USB_BASE+0x56))) */
#define	USB_WVALUE				(0xFFFFu)	/* b15-0: wValue */
#define	USB_DT_TYPE				(0xFF00u)
#define	USB_GET_DT_TYPE(v)		(((v) & USB_DT_TYPE) >> 8)
#define	USB_DT_INDEX			(0x00FFu)
#define	USB_CONF_NUM			(0x00FFu)
#define	USB_ALT_SET				(0x00FFu)

/* USB Request Index Register */
/*	USB_USBINDX		(*((REGP*)(USB_BASE+0x58))) */
#define	USB_WINDEX				(0xFFFFu)	/* b15-0: wIndex */
#define	USB_TEST_SELECT			(0xFF00u)	/* b15-b8: Test Mode Selectors */
#define	  USB_TEST_J			(0x0100u)		/* Test_J */
#define	  USB_TEST_K			(0x0200u)		/* Test_K */
#define	  USB_TEST_SE0_NAK		(0x0300u)		/* Test_SE0_NAK */
#define	  USB_TEST_PACKET		(0x0400u)		/* Test_Packet */
#define	  USB_TEST_FORCE_ENABLE (0x0500u)		/* Test_Force_Enable */
#define	  USB_TEST_STSelectors	(0x0600u)		/* Standard test selectors */
#define	  USB_TEST_RESERVED		(0x4000u)		/* Reserved */
#define	  USB_TEST_VSTMODES		(0xC000u)		/* VendorSpecific test modes */
#define	USB_EP_DIR				(0x0080u)	/* b7: Endpoint Direction */
#define	  USB_EP_DIR_IN			(0x0080u)
#define	  USB_EP_DIR_OUT		(0x0000u)

/* USB Request Length Register */
/*	USB_USBLENG		(*((REGP*)(USB_BASE+0x5A))) */
#define	USB_WLENGTH				(0xFFFFu)	/* b15-0: wLength */


/* Pipe Window Select Register */
/*	USB_PIPESEL		(*((REGP*)(USB_BASE+0x64))) */
#define	USB_PIPENM				(0x0007u)	/* b2-0: Pipe select */

/* Default Control Pipe Configuration Register */
/*	USB_DCPCFG			(*((REGP*)(USB_BASE+0x5C))) */
/* Pipe Configuration Register */
/*	USB_PIPECFG		(*((REGP*)(USB_BASE+0x68))) */
/* Refer to usbd_DefUSBIP.h */
#define	USB_TYPE			(0xC000u)	/* b15-14: Transfer type */
#define	USB_BFRE			(0x0400u)	/* b10: Buffer ready interrupt mode select */
#define	USB_DBLB			(0x0200u)	/* b9: Double buffer mode select */
#define	USB_CBTMD			(0x0100u)	/* b8: Continuous transfer mode select */
#define	USB_SHTNAK			(0x0080u)	/* b7: Transfer end NAK */
#define	USB_DIR				(0x0010u)	/* b4: Transfer direction select */
#define	USB_EPNUM			(0x000Fu)	/* b3-0: Eendpoint number select */

/* Pipe Buffer Configuration Register */
/*	USB_PIPEBUF		(*((REGP*)(USB_BASE+0x6A))) */
/* Refer to usbd_DefUSBIP.h */
#define	USB_BUFSIZE				(0x7C00u)	/* b14-10: Pipe buffer size */
#define	USB_BUFNMB				(0x007Fu)	/* b6-0: Pipe buffer number */
#define	USB_PIPE0BUF			(256u)
#define	USB_PIPEXBUF			(64u)

/* Default Control Pipe Maxpacket Size Register */
/* Pipe Maxpacket Size Register */
/*	USB_DCPMAXP		(*((REGP*)(USB_BASE+0x5E))) */
/* Pipe Maxpacket Size Register */
/*	USB_PIPEMAXP		(*((REGP*)(USB_BASE+0x6C))) */
#define	USB_DEVSEL				(0xF000u)	/* b15-14: Device address select */
#define	USB_MAXP				(0x007Fu)	/* b6-0: Maxpacket size of default control pipe */
#define	USB_MXPS				(0x07FFu)	/* b10-0: Maxpacket size */

/* Pipe Cycle Configuration Register */
/*	USB_PIPEPERI		(*((REGP*)(USB_BASE+0x6E))) */
/* Refer to usbd_DefUSBIP.h */
#define	USB_IFIS			(0x1000u)	/* b12: Isochronous in-buffer flash mode select */
#define	USB_IITV			(0x0007u)	/* b2-0: Isochronous interval */

/* Default Control Pipe Control Register */
/*	USB_DCPCTR			(*((REGP*)(USB_BASE+0x60))) */
/* Pipex Control Register */
/*	USB_PIPE1CTR		(*((REGP*)(USB_BASE+0x70))) */
/*	USB_PIPE2CTR		(*((REGP*)(USB_BASE+0x72))) */
/*	USB_PIPE3CTR		(*((REGP*)(USB_BASE+0x74))) */
/*	USB_PIPE4CTR		(*((REGP*)(USB_BASE+0x76))) */
/*	USB_PIPE5CTR		(*((REGP*)(USB_BASE+0x78))) */
/*	USB_PIPE6CTR		(*((REGP*)(USB_BASE+0x7A))) */
/*	USB_PIPE7CTR		(*((REGP*)(USB_BASE+0x7C))) */
/*	USB_PIPE8CTR		(*((REGP*)(USB_BASE+0x7E))) */
/*	USB_PIPE9CTR		(*((REGP*)(USB_BASE+0x80))) */
#define	USB_BSTS				(0x8000u)	/* b15: Buffer status */
#define	USB_SUREQ				(0x4000u)	/* b14: Send USB request  */
#define	USB_INBUFM				(0x4000u)	/* b14: IN buffer monitor (Only for PIPE1 to 5) */
#define	USB_CSCLR				(0x2000u)	/* b13: c-split status clear */
#define	USB_CSSTS				(0x1000u)	/* b12: c-split status */
#define	USB_SUREQCLR			(0x0800u)	/* b11: stop setup request */
#define	USB_ATREPM				(0x0400u)	/* b10: Auto repeat mode */
#define	USB_ACLRM				(0x0200u)	/* b9: buffer auto clear mode */
#define	USB_SQCLR				(0x0100u)	/* b8: Sequence bit clear */
#define	USB_SQSET				(0x0080u)	/* b7: Sequence bit set */
#define	USB_SQMON				(0x0040u)	/* b6: Sequence bit monitor */
#define	USB_PBUSY				(0x0020u)	/* b5: pipe busy */
#define	USB_PINGE				(0x0010u)	/* b4: ping enable */
#define	USB_CCPL				(0x0004u)	/* b2: Enable control transfer complete */
#define	USB_PID					(0x0003u)	/* b1-0: Response PID */
#define	  USB_PID_STALL			(0x0002u)		/* STALL */
#define	  USB_PID_BUF			(0x0001u)		/* BUF */
#define	  USB_PID_NAK			(0x0000u)		/* NAK */


/* PIPExTRE */
/*	USB_PIPE1TRE		(*((REGP*)(USB_BASE+0x90))) */
/*	USB_PIPE2TRE		(*((REGP*)(USB_BASE+0x94))) */
/*	USB_PIPE3TRE		(*((REGP*)(USB_BASE+0x98))) */
/*	USB_PIPE4TRE		(*((REGP*)(USB_BASE+0x9C))) */
/*	USB_PIPE5TRE		(*((REGP*)(USB_BASE+0xA0))) */
#define	USB_TRENB				(0x0200u)	/* b9: Transaction count enable */
#define	USB_TRCLR				(0x0100u)	/* b8: Transaction count clear */


/* PIPExTRN */
/*	USB_PIPE1TRN		(*((REGP*)(USB_BASE+0x92))) */
/*	USB_PIPE2TRN		(*((REGP*)(USB_BASE+0x96))) */
/*	USB_PIPE3TRN		(*((REGP*)(USB_BASE+0x9A))) */
/*	USB_PIPE4TRN		(*((REGP*)(USB_BASE+0x9E))) */
/*	USB_PIPE5TRN		(*((REGP*)(USB_BASE+0xA2))) */
#define	USB_TRNCNT				(0xFFFFu)	/* b15-0: Transaction counter */

/* USBBCCTRLx */
#define	USB_PDDETSTS			(0x0200u)
#define	USB_CHGDETSTS			(0x0100u)
#define	USB_BATCHGE				(0x0080u)
#define	USB_DCPMODE				(0x0040u)
#define	USB_VDMSRCE				(0x0020u)
#define	USB_IDPSINKE			(0x0010u)
#define	USB_VDPSRCE				(0x0008u)
#define	USB_IDMSINKE			(0x0004u)
#define	USB_IDPSRCE				(0x0002u)
#define	USB_RPDME				(0x0001u)


/* DEVADDx */
/*	USB_DEVADD0		(*((REGP*)(USB_BASE+0xD0))) */
/*	USB_DEVADD1		(*((REGP*)(USB_BASE+0xD2))) */
/*	USB_DEVADD2		(*((REGP*)(USB_BASE+0xD4))) */
/*	USB_DEVADD3		(*((REGP*)(USB_BASE+0xD6))) */
/*	USB_DEVADD4		(*((REGP*)(USB_BASE+0xD8))) */
/*	USB_DEVADD5		(*((REGP*)(USB_BASE+0xDA))) */
/*	USB_DEVADD6		(*((REGP*)(USB_BASE+0xDC))) */
/*	USB_DEVADD7		(*((REGP*)(USB_BASE+0xDE))) */
/*	USB_DEVADD8		(*((REGP*)(USB_BASE+0xE0))) */
/*	USB_DEVADD9		(*((REGP*)(USB_BASE+0xE2))) */
/*	USB_DEVADDA		(*((REGP*)(USB_BASE+0xE4))) */
#define	USB_UPPHUB				(0x7800u)
#define	USB_HUBPORT				(0x0700u)
#define	USB_USBSPD				(0x00C0u)
#define	  USB_NOTUSED			(0x0000u)
#define	  USB_FULLSPEED			(0x0080u)		/* Full-Speed connect */
#define	  USB_HIGHSPEED			(0x00C0u)		/* Hi-Speed connect */
#define	  USB_LOWSPEED			(0x0040u)		/* Low-Speed connect */
#define	USB_RTPORT				(0x0001u)




/* Root port */
#define	USB_NOPORT			(0xFFu)		/* Not connect */

/* Max device */
#define USB_MAXDEVADDR	(0x5u)

/* DCP Max packetsize */
#define	USB_MAXPFIELD		(0x7Fu)	/* Maxpacket size of DCP */

/* Device Address (DEVSEL) */
#define	USB_ADDR2DEVSEL(x)	(uint16_t)((uint16_t)(x) << USB_DEVADDRBIT)
#define	USB_DEVSEL2ADDR(x)	(usb_addr_t)((x) >> USB_DEVADDRBIT)


/******************************************************************************
Another define(not change)
******************************************************************************/
/* FIFO read / write result */
#define	USB_FIFOERROR		(0x0005u)		/* FIFO not ready */
#define	USB_WRITEEND		(0x0001u)		/* End of write (but packet may not be outputting) */
#define	USB_WRITESHRT		(0x0002u)		/* End of write (send short packet) */
#define	USB_WRITING			(0x0003u)		/* Write continues */
#define	USB_READEND			(0x0001u)		/* End of read */
#define	USB_READSHRT		(0x0002u)		/* Insufficient (receive short packet) */
#define	USB_READING			(0x0003u)		/* Read continues */
#define	USB_READOVER		(0x0004u)		/* Buffer size over */

/* Pipe define table end code */
#define	USB_PDTBLEND		(0xFFFFu)		/* End of table */

/* Control Transfer Stage */
#define	USB_IDLEST				(0u)	/* Idle */
#define	USB_SETUPNDC			(1u)	/* Setup Stage No Data Control */
#define	USB_SETUPWR				(2u)	/* Setup Stage Control Write */
#define	USB_SETUPRD				(3u)	/* Setup Stage Control Read */
#define	USB_DATAWR				(4u)	/* Data Stage Control Write */
#define	USB_DATARD				(5u)	/* Data Stage Control Read */
#define	USB_STATUSRD			(6u)	/* Status stage */
#define	USB_STATUSWR			(7u)	/* Status stage */

/* Transfer status Type */
#define	USB_CTRL_END			(1u)	/* Control transfer Normal termination */
#define	USB_DATA_OK				(3u)	/* Data transfer Normal termination */
#define	USB_DATA_SHT			(4u)	/* Control transfer / Data transfer Buffer short reception */
#define	USB_DATA_OVR			(5u)	/* Control transfer / Data transfer Buffer over reception */
#define	USB_DATA_STALL			(6u)	/* Control transfer / Data transfer STALL detection */
#define	USB_DATA_ERR			(7u)	/* Exceptions of the FIFO access error, no response or etc. */
#define	USB_DATA_DTCH			(8u)	/* Exceptions of the detach */
#define	USB_DATA_STOP			(9u)	/* Exceptions of the forced end */
#define	USB_DATA_TMO			(10u)	/* Exceptions of the timeout */

/* Callback argument */
#define	USB_NO_ARG			(0u)

/* USB interrupt type (common)*/
#define	USB_INT_UNKNOWN		(0x00u)
#define	USB_INT_BRDY		(0x01u)
#define	USB_INT_BEMP		(0x02u)
#define	USB_INT_NRDY		(0x03u)
/* USB interrupt type (PERI)*/
#define	USB_INT_VBINT		(0x11u)
#define	USB_INT_RESM		(0x12u)
#define	USB_INT_SOFR		(0x13u)
#define	USB_INT_DVST		(0x14u)
#define	USB_INT_CTRT		(0x15u)
#define	USB_INT_ATTACH		(0x16u)
#define	USB_INT_DETACH		(0x17u)
/* USB interrupt type (HOST)*/
#define	USB_INT_OVRCR0		(0x41u)
#define	USB_INT_BCHG0		(0x42u)
#define	USB_INT_DTCH0		(0x43u)
#define	USB_INT_ATTCH0		(0x44u)
#define	USB_INT_EOFERR0		(0x45u)
#define	USB_INT_PDDETINT0	(0x46u)
#define	USB_INT_OVRCR1		(0x51u)
#define	USB_INT_BCHG1		(0x52u)
#define	USB_INT_ATTCH1		(0x53u)
#define	USB_INT_DTCH1		(0x54u)
#define	USB_INT_EOFERR1		(0x55u)
#define	USB_INT_PDDETINT1	(0x56u)
#define	USB_INT_SACK		(0x61u)
#define	USB_INT_SIGN		(0x62u)


/******************************************************************************
USB driver specification define(not change)
******************************************************************************/
#define	USB_NULL			0
#define	USB_ERROR			0xFF
#define	USB_YES				0x01
#define	USB_NO				0xFE
#define	USB_0				0u
#define	USB_1				1u
#define	USB_2				2u
#define	USB_3				3u
#define	USB_4				4u
#define	USB_5				5u
#define	USB_6				6u
#define	USB_7				7u
#define	USB_8				8u
#define	USB_9				9u

#define	USB_BIT0				((uint16_t)0x0001)
#define	USB_BIT1				((uint16_t)0x0002)
#define	USB_BIT2				((uint16_t)0x0004)
#define	USB_BIT3				((uint16_t)0x0008)
#define	USB_BIT4				((uint16_t)0x0010)
#define	USB_BIT5				((uint16_t)0x0020)
#define	USB_BIT6				((uint16_t)0x0040)
#define	USB_BIT7				((uint16_t)0x0080)
#define	USB_BIT8				((uint16_t)0x0100)
#define	USB_BIT9				((uint16_t)0x0200)
#define	USB_BIT10				((uint16_t)0x0400)
#define	USB_BIT11				((uint16_t)0x0800)
#define	USB_BIT12				((uint16_t)0x1000)
#define	USB_BIT13				((uint16_t)0x2000)
#define	USB_BIT14				((uint16_t)0x4000)
#define	USB_BIT15				((uint16_t)0x8000)
#define	USB_BITSET(x)			(uint16_t)((uint16_t)1 << (x))


/******************************************************************************
Another define
******************************************************************************/
/* ControlPipe Max Packet size */
#define	USB_DEFPACKET		(0x0040u)	/* Default DCP Max packet size */

#define	USB_NONDEVICE			(0u)
#define	USB_DEVICEENUMERATION	(3u)
#define	USB_COMPLETEPIPESET		(10u)


/******************************************************************************
Task request message type
******************************************************************************/
/* API function command define */
#define	USB_INTERRUPT					(0x01u)

/* Davice state control command for API function(HOST/PERI common define) */
#define	USB_DO_TRANSFERSTART			(0x11u)
#define	USB_DO_TRANSFEREND				(0x12u)
#define	USB_DO_TRANSFER_STP				(0x13u)		/* Transfer stop */
#define	USB_DO_TRANSFER_TMO				(0x14u)		/* Time out */
#define	USB_DO_INITHWFUNCTION			(0x17u)		/* USB-IP initialize */
#define	USB_DO_SETHWFUNCTION			(0x18u)		/* USB-IP function set */

/* Davice state control command for API function(R_USB_PstdChangeDeviceState) */
#define	USB_DO_REMOTEWAKEUP				(0x21u)

/* Sequence control command define (HCD2MGR/UPL2MGR) */
#define	USB_MGR_CONTINUE				(0x30u)		/* Continues sequnce */
#define	USB_RTP_DETACH					(0x31u)		/* USB_INT_DTCH */
#define	USB_RTP_ATTACH					(0x32u)		/* USB_INT_ATTCH */
#define	USB_RTP_OVERCURRENT				(0x33u)		/* USB_INT_OVRCR */
#define	USB_RTP_REMOTEWAKEUP			(0x34u)		/* USB_INT_BCHG */
#define	USB_DO_SELECTIVE_SUSPEND		(0x35u)		/* SUSPEND control (device select)*/
#define	USB_DO_SELECTIVE_RESUME			(0x36u)		/* RESUME control (device select) */
#define	USB_DO_PORT_ENABLE				(0x37u)		/* VBOUT control */
#define	USB_DO_PORT_DISABLE				(0x38u)		/* VBOUT control */
#define	USB_DO_CLEAR_STALL				(0x39u)		/* Clear_Feature request */
#define	USB_DO_GLOBAL_SUSPEND			(0x3Au)		/* SUSPEND control (port select)*/
#define	USB_DO_GLOBAL_RESUME			(0x3Bu)		/* RESUME control (port select) */
#define	USB_MGR_NOSEQUENCE				(0x3Cu)

/* Davice state control command define (MGR2HCD) */
#define	USB_HCD_USBRESET				(0x41u)
#define	USB_HCD_SUSPEND					(0x42u)
#define	USB_HCD_RESUME					(0x43u)
#define	USB_HCD_VBON					(0x44u)
#define	USB_HCD_VBOFF					(0x45u)
#define	USB_HCD_CLEAR_STALL				(0x46u)


/******************************************************************************
CallBack type
******************************************************************************/
/* Host/Peripheral device information(state) callback information & USB device state */
#define	USB_STS_DETACH		(0x01)		/* detach(use by statediagram and the PORT_DISABLE callback) */
#define	USB_STS_ATTACH		(0x02)		/* attach(use by statediagram) */
#define	USB_STS_POWER		(0x03)		/* attach(use by PORT_ENABLE callback) */
#define	USB_STS_DEFAULT		(0x04)		/* default(USB-reset:use by statediagram) */
#define	USB_STS_ADDRESS		(0x05)		/* addressed(Set_Address:use by statediagram[peripheral only]) */
#define	USB_STS_CONFIGURED	(0x06)		/* configured(Set_Configuration:use by statediagram) */
#define	USB_STS_SUSPEND		(0x07)		/* suspend(use by statediagram[peripheral] and SUSPEND callback[host]) */
#define	USB_STS_RESUME		(0x08)		/* resume(use by statediagram[peripheral] and RESUME callback[host]) */
#define	USB_STS_OVERCURRENT	(0x09)		/* over current(use by statediagram[host onlt]) */
#define	USB_STS_WAKEUP		(0x0A)		/* remote wakeup(use by statediagram[host onlt]) */
#define	USB_STS_DS_SUSPEND	(0x0B)		/* suspend(For internal state management[host]:DeviceSelect) */
#define	USB_STS_PS_SUSPEND	(0x0C)		/* suspend(For internal state management[host]:PortSelect) */
#define	USB_STS_SS_SUSPEND	(0x0C)		/* suspend(For internal state management[host]:SystemSelect) */
#define	USB_STS_PORTOFF		(0x0D)		/* VBUS off */
#define	USB_STALL_SUCCESS	(0x0E)		/* Success clear STALL */
#define	USB_STALL_FAILURE	(0x0F)		/* Failure clear STALL */

/* Peripheral callback information */
#define	USB_SETSTALL		(0x11)		/* Set stall */
#define	USB_CLEARSTALL		(0x12)		/* Clear stall */
#define	USB_SETREMOTE		(0x13)		/* Enable remote wakeup */
#define	USB_CLEARREMOTE		(0x14)		/* Disavle remote wakeup */
#define	USB_PORTENABLE		(0x15)		/* Port enable */
#define	USB_PORTDISABLE		(0x16)		/* Port disable */
#define	USB_RECIPIENT		(0x17)

/******************************************************************************
Sequence control
******************************************************************************/
#define	USB_NOSEQE0						(0x00u)
#define	USB_NOSEQE1						(0x01u)
#define	USB_ATTACH0						(0x02u)
#define	USB_ENUMER1						(0x03u)
#define	USB_SUSPEN2						(0x04u)
#define	USB_SUSPEN0						(0x05u)
#define	USB_SUSPEN1						(0x06u)
#define	USB_RESUME2						(0x07u)
#define	USB_RESUME0						(0x08u)
#define	USB_RESUME1						(0x09u)
#define	USB_REMOTE0						(0x0Au)
#define	USB_REMOTE1						(0x0Bu)
#define	USB_DETACH0						(0x0Cu)
#define	USB_DETACH1						(0x0Du)
#define	USB_DETACH2						(0x0Eu)
#define	USB_OVERCU0						(0x0Fu)
#define	USB_PORT_E0						(0x10u)
#define	USB_PORT_E1						(0x11u)
#define	USB_PORT_D0						(0x12u)
#define	USB_PORT_D1						(0x13u)
#define	USB_CLRSTL0						(0x14u)
#define	USB_CLRSTL1						(0x15u)

/******************************************************************************
Host Battery Charging Define
******************************************************************************/
#ifdef USB_HOST_BC_ENABLE
/* BC State Define */
#define USB_BC_STATE_INIT	(0x00u)
#define USB_BC_STATE_DET	(0x01u)
#define USB_BC_STATE_CDP	(0x02u)
#define USB_BC_STATE_SDP	(0x03u)
#define USB_BC_STATE_DCP	(0x04u)
#define USB_BC_STATE_MAX 	(0x05u)

/* BC State Change Event Define */
#define USB_BC_EVENT_VB		(0x00u)
#define USB_BC_EVENT_AT		(0x01u)
#define USB_BC_EVENT_DT		(0x02u)
#define USB_BC_EVENT_MAX	(0x03u)

/* DCP Mode Setting Define */
#ifdef USB_BC_DCP_ENABLE
#define USB_BC_DCPMODE		(0x01u)
#else	/* USB_BC_DCP_ENABLE */
#define USB_BC_DCPMODE		(0x00u)
#endif	/* USB_BC_DCP_ENABLE */

#endif /*  USB_HOST_BC_ENABLE */

#endif  /* __R_USB_CDEFUSBIP_H__ */
/******************************************************************************
End Of File
******************************************************************************/
