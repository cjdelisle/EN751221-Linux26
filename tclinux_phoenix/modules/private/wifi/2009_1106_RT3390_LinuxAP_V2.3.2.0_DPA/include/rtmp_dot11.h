/*

*/

#ifndef __DOT11_BASE_H__
#define __DOT11_BASE_H__

#include "rtmp_type.h"


// 4-byte HTC field.  maybe included in any frame except non-QOS data frame.  The Order bit must set 1.
typedef struct PACKED {
#ifdef RT_BIG_ENDIAN
    UINT32		RDG:1;	//RDG / More PPDU
    UINT32		ACConstraint:1;	//feedback request
    UINT32		rsv2:5;  //calibration sequence
    UINT32		NDPAnnouce:1;	// ZLF announcement
    UINT32		CSISTEERING:2;	//CSI/ STEERING
    UINT32		rsv1:2;	// Reserved
    UINT32		CalSeq:2;  //calibration sequence
    UINT32		CalPos:2;	// calibration position
    UINT32		MFBorASC:7;	//Link adaptation feedback containing recommended MCS. 0x7f for no feedback or not available
    UINT32		MFSI:3;	//SET to the received value of MRS. 0x111 for unsolicited MFB.
    UINT32		MSI:3;	//MCS Request, MRQ Sequence identifier
    UINT32		MRQ:1;	//MCS feedback. Request for a MCS feedback
    UINT32		TRQ:1;	//sounding request
    UINT32		rsv:1;	// Reserved
#else
    UINT32		rsv:1;	// Reserved
    UINT32		TRQ:1;	//sounding request
    UINT32		MRQ:1;	//MCS feedback. Request for a MCS feedback
    UINT32		MSI:3;	//MCS Request, MRQ Sequence identifier
    UINT32		MFSI:3;	//SET to the received value of MRS. 0x111 for unsolicited MFB.
    UINT32		MFBorASC:7;	//Link adaptation feedback containing recommended MCS. 0x7f for no feedback or not available
    UINT32		CalPos:2;	// calibration position
    UINT32		CalSeq:2;  //calibration sequence
    UINT32		rsv1:2;	// Reserved
    UINT32		CSISTEERING:2;	//CSI/ STEERING
    UINT32		NDPAnnouce:1;	// ZLF announcement
    UINT32		rsv2:5;  //calibration sequence
    UINT32		ACConstraint:1;	//feedback request
    UINT32		RDG:1;	//RDG / More PPDU
#endif /* !RT_BIG_ENDIAN */
} HT_CONTROL, *PHT_CONTROL;

// 2-byte QOS CONTROL field
typedef struct PACKED {
#ifdef RT_BIG_ENDIAN
    USHORT      Txop_QueueSize:8;
    USHORT      AMsduPresent:1;
    USHORT      AckPolicy:2;  //0: normal ACK 1:No ACK 2:scheduled under MTBA/PSMP  3: BA
    USHORT      EOSP:1;
    USHORT      TID:4;
#else
    USHORT      TID:4;
    USHORT      EOSP:1;
    USHORT      AckPolicy:2;  //0: normal ACK 1:No ACK 2:scheduled under MTBA/PSMP  3: BA
    USHORT      AMsduPresent:1;
    USHORT      Txop_QueueSize:8;
#endif /* !RT_BIG_ENDIAN */
} QOS_CONTROL, *PQOS_CONTROL;	


// 2-byte Frame control field
typedef	struct	PACKED {
#ifdef RT_BIG_ENDIAN
	USHORT		Order:1;			// Strict order expected
	USHORT		Wep:1;				// Wep data
	USHORT		MoreData:1;			// More data bit
	USHORT		PwrMgmt:1;			// Power management bit
	USHORT		Retry:1;			// Retry status bit
	USHORT		MoreFrag:1;			// More fragment bit
	USHORT		FrDs:1;				// From DS indication
	USHORT		ToDs:1;				// To DS indication
	USHORT		SubType:4;			// MSDU subtype
	USHORT		Type:2;				// MSDU type
	USHORT		Ver:2;				// Protocol version
#else
	USHORT		Ver:2;				// Protocol version
	USHORT		Type:2;				// MSDU type
	USHORT		SubType:4;			// MSDU subtype
	USHORT		ToDs:1;				// To DS indication
	USHORT		FrDs:1;				// From DS indication
	USHORT		MoreFrag:1;			// More fragment bit
	USHORT		Retry:1;			// Retry status bit
	USHORT		PwrMgmt:1;			// Power management bit
	USHORT		MoreData:1;			// More data bit
	USHORT		Wep:1;				// Wep data
	USHORT		Order:1;			// Strict order expected
#endif /* !RT_BIG_ENDIAN */
} FRAME_CONTROL, *PFRAME_CONTROL;

typedef	struct	PACKED _HEADER_802_11	{
    FRAME_CONTROL   FC;
    USHORT          Duration;
    UCHAR           Addr1[MAC_ADDR_LEN];
    UCHAR           Addr2[MAC_ADDR_LEN];
	UCHAR			Addr3[MAC_ADDR_LEN];
#ifdef RT_BIG_ENDIAN
	USHORT			Sequence:12;
	USHORT			Frag:4;
#else
	USHORT			Frag:4;
	USHORT			Sequence:12;
#endif /* !RT_BIG_ENDIAN */
	UCHAR			Octet[0];
}	HEADER_802_11, *PHEADER_802_11;

typedef struct PACKED _PSPOLL_FRAME {
    FRAME_CONTROL   FC;
    USHORT          Aid;
    UCHAR           Bssid[MAC_ADDR_LEN];
    UCHAR           Ta[MAC_ADDR_LEN];
}   PSPOLL_FRAME, *PPSPOLL_FRAME;

typedef	struct	PACKED _RTS_FRAME	{
    FRAME_CONTROL   FC;
    USHORT          Duration;
    UCHAR           Addr1[MAC_ADDR_LEN];
    UCHAR           Addr2[MAC_ADDR_LEN];
}RTS_FRAME, *PRTS_FRAME;

#endif // __DOT11_BASE_H__ //
