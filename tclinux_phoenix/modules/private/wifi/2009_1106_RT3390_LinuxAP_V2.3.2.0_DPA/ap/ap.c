/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************
 
    Module Name:
    soft_ap.c
 
    Abstract:
    Access Point specific routines and MAC table maintenance routines
 
    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    John Chang  08-04-2003    created for 11g soft-AP

 */

#include "rt_config.h"
#ifdef WSC_AP_SUPPORT
#if !defined(TCSUPPORT_LED_WPS_BICOLOR) 
#define WPS_LED_TIMER_INTERVAL 100
#endif
RALINK_TIMER_STRUCT     WPSLedTimer;
UCHAR  WPSLedTimerRunning = FALSE;
#endif

BOOLEAN ApCheckLongPreambleSTA(
    IN PRTMP_ADAPTER pAd);

char const *pEventText[EVENT_MAX_EVENT_TYPE] = {
	"restart access point",
	"successfully associated",
	"has disassociated",
	"has been aged-out and disassociated" ,    
	"active countermeasures",
	"has disassociated with invalid PSK password"};

/*
	==========================================================================
	Description:
		Initialize AP specific data especially the NDIS packet pool that's
		used for wireless client bridging.
	==========================================================================
 */
NDIS_STATUS APInitialize(
	IN  PRTMP_ADAPTER   pAd)
{
	NDIS_STATUS     Status = NDIS_STATUS_SUCCESS;
	INT				i;			

	DBGPRINT(RT_DEBUG_TRACE, ("---> APInitialize\n"));

	// Init Group key update timer, and countermeasures timer
	for (i = 0; i < MAX_MBSSID_NUM; i++)
		RTMPInitTimer(pAd, &pAd->ApCfg.MBSSID[i].REKEYTimer, GET_TIMER_FUNCTION(GREKEYPeriodicExec), pAd,  TRUE); 
	
	RTMPInitTimer(pAd, &pAd->ApCfg.CounterMeasureTimer, GET_TIMER_FUNCTION(CMTimerExec), pAd, FALSE);

#ifdef IDS_SUPPORT
	// Init intrusion detection timer
	RTMPInitTimer(pAd, &pAd->ApCfg.IDSTimer, GET_TIMER_FUNCTION(RTMPIdsPeriodicExec), pAd, FALSE);
	pAd->ApCfg.IDSTimerRunning = FALSE;
#endif // IDS_SUPPORT //

#ifdef WAPI_SUPPORT
	// Init WAPI rekey timer
	RTMPInitWapiRekeyTimerAction(pAd, NULL);
#endif // WAPI_SUPPORT //

#ifdef WDS_SUPPORT
	APWdsInitialize(pAd);
#endif // WDS_SUPPORT //

#ifdef IGMP_SNOOP_SUPPORT
	MulticastFilterTableInit(&pAd->pMulticastFilterTable);
#endif // IGMP_SNOOP_SUPPORT //

#ifdef WDS_SUPPORT	
	NdisAllocateSpinLock(&pAd->WdsTabLock);
#endif // WDS_SUPPORT //

	#ifdef TC_SOC
#ifdef CONFIG_MIPS_TC3262
	NdisAllocateSpinLock(&pAd->TCLock);
#endif	
	tcInitTimer(pAd, &pAd->PollingModeDetect, (PVOID)&PeriodicPollingModeDetect);
	pAd->PollingModeDetectRunning = FALSE;
	#endif

	DBGPRINT(RT_DEBUG_TRACE, ("<--- APInitialize\n"));
	return Status;
}

/*
	==========================================================================
	Description:
		Shutdown AP and free AP specific resources
	==========================================================================
 */
VOID APShutdown(
	IN PRTMP_ADAPTER pAd)
{
	DBGPRINT(RT_DEBUG_TRACE, ("---> APShutdown\n"));
//	if (pAd->OpMode == OPMODE_AP)
		APStop(pAd);

	MlmeRadioOff(pAd);
/* mark by wy, this is done by firmware in MlmeRadioOff.
#ifdef RTMP_MAC_PCI
	// reset DMA Index
	RTMP_IO_WRITE32(pAd, WPDMA_RST_IDX , 0xFFFFFFFF);
	RTMP_IO_WRITE32(pAd, PBF_SYS_CTRL, 0xe1f);
	RTMP_IO_WRITE32(pAd, PBF_SYS_CTRL, 0xe00);
#endif // RTMP_MAC_PCI //
*/

#ifdef IGMP_SNOOP_SUPPORT
	MultiCastFilterTableReset(&pAd->pMulticastFilterTable);
#endif // IGMP_SNOOP_SUPPORT //

	NdisFreeSpinLock(&pAd->MacTabLock);
#ifdef WDS_SUPPORT
	NdisFreeSpinLock(&pAd->WdsTabLock);
#endif // WDS_SUPPORT //
	DBGPRINT(RT_DEBUG_TRACE, ("<--- APShutdown\n"));
}

/*
	==========================================================================
	Description:
		Start AP service. If any vital AP parameter is changed, a STOP-START
		sequence is required to disassociate all STAs.

	IRQL = DISPATCH_LEVEL.(from SetInformationHandler)
	IRQL = PASSIVE_LEVEL. (from InitializeHandler)  

	Note:
		Can't call NdisMIndicateStatus on this routine.

		RT61 is a serialized driver on Win2KXP and is a deserialized on Win9X
		Serialized callers of NdisMIndicateStatus must run at IRQL = DISPATCH_LEVEL.

	==========================================================================
 */
VOID APStartUp(
	IN PRTMP_ADAPTER pAd) 
{
	ULONG		offset, i;
	UINT32		Value = 0;
	BOOLEAN		bWmmCapable = FALSE;
	UCHAR		apidx;
	BOOLEAN		TxPreamble, SpectrumMgmt;
	UCHAR		BBPR1 = 0, BBPR3 = 0, byteValue = 0;
	UCHAR		phy_mode = pAd->CommonCfg.DesiredPhyMode;
	
	DBGPRINT(RT_DEBUG_TRACE, ("===> APStartUp\n"));
	/*merge from linos*/
	UCHAR FourByteOffset = 0;		
	UCHAR NWlanExt = 0;
		
#ifdef INF_AMAZON_SE
	for (i=0;i<NUM_OF_TX_RING;i++)
	{
		pAd->BulkOutDataSizeLimit[i]=24576;
	}
#endif // INF_AMAZON_SE //
		
	AsicDisableSync(pAd);

	TxPreamble = (pAd->CommonCfg.TxPreamble == Rt802_11PreambleLong ? 0 : 1);

#ifdef A_BAND_SUPPORT
	// Decide the Capability information field
	// In IEEE Std 802.1h-2003, the spectrum management bit is enabled in the 5 GHz band 
	if ((pAd->CommonCfg.Channel > 14) && pAd->CommonCfg.bIEEE80211H == TRUE)
		SpectrumMgmt = TRUE;
	else
#endif // A_BAND_SUPPORT //
		SpectrumMgmt = FALSE;	
	/*generate bssid from cpe mac address start, merge from linos, 20100208*/
	FourByteOffset = pAd->CurrentAddress[5]%4;		
	DBGPRINT(RT_DEBUG_WARN, ("\r\nFourByteOffset is %d", FourByteOffset));		
	NWlanExt = pAd->ApCfg.BssidNum;		
	DBGPRINT(RT_DEBUG_WARN, ("\r\nNWlanExt is %d", NWlanExt));

	switch(NWlanExt){			
		case 1:				
			memcpy(pAd->ApCfg.MBSSID[0].Bssid, pAd->CurrentAddress, 6);				
			break;			
		case 2:				
			memcpy(pAd->ApCfg.MBSSID[0].Bssid, pAd->CurrentAddress, 6);				
			memcpy(pAd->ApCfg.MBSSID[1].Bssid, pAd->CurrentAddress, 6);			
			switch(FourByteOffset){					
				case 1:					
				case 3:						
					pAd->ApCfg.MBSSID[0].Bssid[5]--;						
					break;					
				case 0:					
				case 2:						
					pAd->ApCfg.MBSSID[1].Bssid[5]++;						
					break;				
			}				
			break;			
		case 3:				
			memcpy(pAd->ApCfg.MBSSID[0].Bssid, pAd->CurrentAddress, 6);				
			memcpy(pAd->ApCfg.MBSSID[1].Bssid, pAd->CurrentAddress, 6);				
			memcpy(pAd->ApCfg.MBSSID[2].Bssid, pAd->CurrentAddress, 6);				
			switch(FourByteOffset){					
				case 0:						
					pAd->ApCfg.MBSSID[1].Bssid[5]++;						
					pAd->ApCfg.MBSSID[2].Bssid[5] += 2;						
					break;					
				case 1:						
					pAd->ApCfg.MBSSID[0].Bssid[5]--;						
					pAd->ApCfg.MBSSID[2].Bssid[5]++;						
					break;					
				case 2:						
					pAd->ApCfg.MBSSID[0].Bssid[5] -= 2;						
					pAd->ApCfg.MBSSID[1].Bssid[5]--;						
					break;					
				case 3:						
					pAd->ApCfg.MBSSID[0].Bssid[5] -= 3;						
					pAd->ApCfg.MBSSID[1].Bssid[5] -= 2;						
					pAd->ApCfg.MBSSID[2].Bssid[5]--;						
					break;				
			}			
			break;			
		case 4:				
			memcpy(pAd->ApCfg.MBSSID[0].Bssid, pAd->CurrentAddress, 6);				
			memcpy(pAd->ApCfg.MBSSID[1].Bssid, pAd->CurrentAddress, 6);				
			memcpy(pAd->ApCfg.MBSSID[2].Bssid, pAd->CurrentAddress, 6);				
			memcpy(pAd->ApCfg.MBSSID[3].Bssid, pAd->CurrentAddress, 6);				
			switch(FourByteOffset){					
				case 0:						
					pAd->ApCfg.MBSSID[1].Bssid[5]++;						
					pAd->ApCfg.MBSSID[2].Bssid[5] += 2;						
					pAd->ApCfg.MBSSID[3].Bssid[5] += 3;						
					break;					
				case 1:						
					pAd->ApCfg.MBSSID[0].Bssid[5]--;						
					pAd->ApCfg.MBSSID[2].Bssid[5]++;						
					pAd->ApCfg.MBSSID[3].Bssid[5] += 2;						
					break;					
				case 2:						
					pAd->ApCfg.MBSSID[0].Bssid[5] -= 2;						
					pAd->ApCfg.MBSSID[1].Bssid[5]--;						
					pAd->ApCfg.MBSSID[3].Bssid[5]++;						
					break;					
				case 3:						
					pAd->ApCfg.MBSSID[0].Bssid[5] -= 3;						
					pAd->ApCfg.MBSSID[1].Bssid[5] -= 2;						
					pAd->ApCfg.MBSSID[2].Bssid[5]--;						
					break;				
			}			
		break;		
	}

	/*change pAd->CurrentAddress by pAd->ApCfg.MBSSID[0].Bssid*/
	memcpy(pAd->CurrentAddress, pAd->ApCfg.MBSSID[0].Bssid, 6);
	
	for(i = 0; i < NWlanExt; i++){			
		DBGPRINT(RT_DEBUG_WARN,("MBSSID[%d] MAC=%02x:%02x:%02x:%02x:%02x:%02x\n", i,	        
			pAd->ApCfg.MBSSID[i].Bssid[0], pAd->ApCfg.MBSSID[i].Bssid[1],	       
			pAd->ApCfg.MBSSID[i].Bssid[2], pAd->ApCfg.MBSSID[i].Bssid[3],	        
			pAd->ApCfg.MBSSID[i].Bssid[4], pAd->ApCfg.MBSSID[i].Bssid[5]));		
	}
	/*generate bssid from cpe mac address end, merge from linos, 20100208*/
	
	for (apidx=0; apidx<pAd->ApCfg.BssidNum; apidx++)
	{
		if ((pAd->ApCfg.MBSSID[apidx].SsidLen <= 0) || (pAd->ApCfg.MBSSID[apidx].SsidLen > MAX_LEN_OF_SSID))
		{
			NdisMoveMemory(pAd->ApCfg.MBSSID[apidx].Ssid, "HT_AP", 5);
			pAd->ApCfg.MBSSID[apidx].Ssid[5] = '0'+apidx;
			pAd->ApCfg.MBSSID[apidx].SsidLen = 6;			
		}

		/* re-copy the MAC to virtual interface to avoid these MAC = all zero,
		   when re-open the ra0,
		   i.e. ifconfig ra0 down, ifconfig ra0 up, ifconfig ra0 down, ifconfig up ... */
#if 0
		COPY_MAC_ADDR(pAd->ApCfg.MBSSID[apidx].Bssid, pAd->CurrentAddress);
		pAd->ApCfg.MBSSID[apidx].Bssid[5] += apidx;
#endif

		if (pAd->ApCfg.MBSSID[apidx].MSSIDDev != NULL)
		{
			NdisMoveMemory(RTMP_OS_NETDEV_GET_PHYADDR(pAd->ApCfg.MBSSID[apidx].MSSIDDev), 
								pAd->ApCfg.MBSSID[apidx].Bssid,
								MAC_ADDR_LEN);
		}

		if (pAd->ApCfg.MBSSID[apidx].bWmmCapable)
		{
        	bWmmCapable = TRUE;
		}
		
		pAd->ApCfg.MBSSID[apidx].CapabilityInfo =
			CAP_GENERATE(1, 0, (pAd->ApCfg.MBSSID[apidx].WepStatus != Ndis802_11EncryptionDisabled), TxPreamble, pAd->CommonCfg.bUseShortSlotTime, SpectrumMgmt);

		
		if (bWmmCapable == TRUE)
		{
			/*
				In WMM spec v1.1, A WMM-only AP or STA does not set the "QoS"
				bit in the capability field of association, beacon and probe
				management frames.
			*/
//			pAd->ApCfg.MBSSID[apidx].CapabilityInfo |= 0x0200;
		} /* End of if */

#ifdef UAPSD_AP_SUPPORT
        if (pAd->CommonCfg.bAPSDCapable == TRUE)
		{
			/* QAPs set the APSD subfield to 1 within the Capability Information
			   field when the MIB attribute dot11APSDOptionImplemented is true
			   and set it to 0 otherwise. STAs always set this subfield to 0. */
            pAd->ApCfg.MBSSID[apidx].CapabilityInfo |= 0x0800;
        } /* End of if */
#endif // UAPSD_AP_SUPPORT //

		
		// decide the mixed WPA cipher combination  
		if (pAd->ApCfg.MBSSID[apidx].WepStatus == Ndis802_11Encryption4Enabled)
		{
			switch ((UCHAR)pAd->ApCfg.MBSSID[apidx].AuthMode)
			{
				// WPA mode
				case Ndis802_11AuthModeWPA:
				case Ndis802_11AuthModeWPAPSK:
					pAd->ApCfg.MBSSID[apidx].WpaMixPairCipher = WPA_TKIPAES_WPA2_NONE;
					break;	

				// WPA2 mode
				case Ndis802_11AuthModeWPA2:
				case Ndis802_11AuthModeWPA2PSK:
					pAd->ApCfg.MBSSID[apidx].WpaMixPairCipher = WPA_NONE_WPA2_TKIPAES;
					break;

				// WPA and WPA2 both mode
				case Ndis802_11AuthModeWPA1WPA2:
				case Ndis802_11AuthModeWPA1PSKWPA2PSK:	

					// In WPA-WPA2 and TKIP-AES mixed mode, it shall use the maximum 
					// cipher capability unless users assign the desired setting.
					if ((pAd->ApCfg.MBSSID[apidx].WpaMixPairCipher == MIX_CIPHER_NOTUSE) ||
						(pAd->ApCfg.MBSSID[apidx].WpaMixPairCipher == WPA_NONE_WPA2_TKIPAES) ||
						(pAd->ApCfg.MBSSID[apidx].WpaMixPairCipher == WPA_TKIPAES_WPA2_NONE))
						pAd->ApCfg.MBSSID[apidx].WpaMixPairCipher = WPA_TKIPAES_WPA2_TKIPAES;										
					break;				
			}
											
		}
		else
			pAd->ApCfg.MBSSID[apidx].WpaMixPairCipher = MIX_CIPHER_NOTUSE;

		// Generate the corresponding RSNIE
		RTMPMakeRSNIE(pAd, pAd->ApCfg.MBSSID[apidx].AuthMode, pAd->ApCfg.MBSSID[apidx].WepStatus, apidx);

	}

#ifdef DOT11_N_SUPPORT
	if (phy_mode != pAd->CommonCfg.PhyMode)
		RTMPSetPhyMode(pAd, phy_mode);

	SetCommonHT(pAd);
#endif // DOT11_N_SUPPORT //
	
	COPY_MAC_ADDR(pAd->CommonCfg.Bssid, pAd->CurrentAddress);

	// Select DAC according to HT or Legacy, write to BBP R1(bit4:3)
	// In HT mode and two stream mode, both DACs are selected.
	// In legacy mode or one stream mode, DAC-0 is selected.
	{
#ifdef DOT11_N_SUPPORT
	if ((pAd->CommonCfg.PhyMode >= PHY_11ABGN_MIXED) && (pAd->Antenna.field.TxPath == 2))
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BBPR1);
		BBPR1 &= (~0x18);
		BBPR1 |= 0x10;
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BBPR1);
	}
	else
#endif // DOT11_N_SUPPORT //
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BBPR1);
		BBPR1 &= (~0x18);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BBPR1);
	}
	}

	// Receiver Antenna selection, write to BBP R3(bit4:3)
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BBPR3);
	BBPR3 &= (~0x18);
	if(pAd->Antenna.field.RxPath == 3)
	{
		BBPR3 |= (0x10);
	}
	else if(pAd->Antenna.field.RxPath == 2)
	{
		BBPR3 |= (0x8);
	}
	else if(pAd->Antenna.field.RxPath == 1)
	{
		BBPR3 |= (0x0);
	}
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BBPR3);
//2008/11/05: KH add to support Antenna power-saving of AP<--
#ifdef DOT11_N_SUPPORT
			if (pAd->CommonCfg.bGreenAPEnable==TRUE
				&&(pAd->Antenna.field.RxPath>1||pAd->Antenna.field.TxPath>1))
			{
				
				EnableAPMIMOPS(pAd);
			}
#endif // DOT11_N_SUPPORT //
//2008/11/05: KH add to support Antenna power-saving of AP-->
	
	//if ((pAd->CommonCfg.PhyMode >= PHY_11ABGN_MIXED) || bWmmCapable)
	if ((pAd->CommonCfg.PhyMode > PHY_11G) || bWmmCapable)  // edit by Benson, fix the bug of no 11n support
	{
		// EDCA parameters used for AP's own transmission
		if (pAd->CommonCfg.APEdcaParm.bValid == FALSE)
		{
			pAd->CommonCfg.APEdcaParm.bValid = TRUE;
			pAd->CommonCfg.APEdcaParm.Aifsn[0] = 3;
			pAd->CommonCfg.APEdcaParm.Aifsn[1] = 7;
			pAd->CommonCfg.APEdcaParm.Aifsn[2] = 1;
			pAd->CommonCfg.APEdcaParm.Aifsn[3] = 1;

			pAd->CommonCfg.APEdcaParm.Cwmin[0] = 4;
			pAd->CommonCfg.APEdcaParm.Cwmin[1] = 4;
			pAd->CommonCfg.APEdcaParm.Cwmin[2] = 3;
			pAd->CommonCfg.APEdcaParm.Cwmin[3] = 2;

			pAd->CommonCfg.APEdcaParm.Cwmax[0] = 6;
			pAd->CommonCfg.APEdcaParm.Cwmax[1] = 10;
			pAd->CommonCfg.APEdcaParm.Cwmax[2] = 4;
			pAd->CommonCfg.APEdcaParm.Cwmax[3] = 3;

			pAd->CommonCfg.APEdcaParm.Txop[0]  = 0;
			pAd->CommonCfg.APEdcaParm.Txop[1]  = 0;
			pAd->CommonCfg.APEdcaParm.Txop[2]  = 94;	//96;
			pAd->CommonCfg.APEdcaParm.Txop[3]  = 47;	//48;
		}
		AsicSetEdcaParm(pAd, &pAd->CommonCfg.APEdcaParm);

		// EDCA parameters to be annouced in outgoing BEACON, used by WMM STA
		if (pAd->ApCfg.BssEdcaParm.bValid == FALSE)
		{
			pAd->ApCfg.BssEdcaParm.bValid = TRUE;
			pAd->ApCfg.BssEdcaParm.Aifsn[0] = 3;
			pAd->ApCfg.BssEdcaParm.Aifsn[1] = 7;
			pAd->ApCfg.BssEdcaParm.Aifsn[2] = 2;
			pAd->ApCfg.BssEdcaParm.Aifsn[3] = 2;

			pAd->ApCfg.BssEdcaParm.Cwmin[0] = 4;
			pAd->ApCfg.BssEdcaParm.Cwmin[1] = 4;
			pAd->ApCfg.BssEdcaParm.Cwmin[2] = 3;
			pAd->ApCfg.BssEdcaParm.Cwmin[3] = 2;

			pAd->ApCfg.BssEdcaParm.Cwmax[0] = 10;
			pAd->ApCfg.BssEdcaParm.Cwmax[1] = 10;
			pAd->ApCfg.BssEdcaParm.Cwmax[2] = 4;
			pAd->ApCfg.BssEdcaParm.Cwmax[3] = 3;

			pAd->ApCfg.BssEdcaParm.Txop[0]  = 0;
			pAd->ApCfg.BssEdcaParm.Txop[1]  = 0;
			pAd->ApCfg.BssEdcaParm.Txop[2]  = 94;	//96;
			pAd->ApCfg.BssEdcaParm.Txop[3]  = 47;	//48;
		}
	}
	else
		AsicSetEdcaParm(pAd, NULL);

#ifdef DOT11_N_SUPPORT
	if (pAd->CommonCfg.PhyMode < PHY_11ABGN_MIXED)
	{
		// Patch UI
		pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth = BW_20;
	}

	// init
	if (pAd->CommonCfg.bRdg)
	{	
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RDG_ACTIVE);
		AsicEnableRDG(pAd);
	}
	else	
	{
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RDG_ACTIVE);
		AsicDisableRDG(pAd);
	}	
#endif // DOT11_N_SUPPORT //

	/*mark by xyzhu_nj*/
	//COPY_MAC_ADDR(pAd->ApCfg.MBSSID[BSS0].Bssid, pAd->CurrentAddress);
	
	AsicSetBssid(pAd, pAd->CurrentAddress); 
	AsicSetMcastWC(pAd);
	// In AP mode,  First WCID Table in ASIC will never be used. To prevent it's 0xff-ff-ff-ff-ff-ff, Write 0 here.
	// p.s ASIC use all 0xff as termination of WCID table search.
	RTMP_IO_WRITE32(pAd, MAC_WCID_BASE, 0x00);
	RTMP_IO_WRITE32(pAd, MAC_WCID_BASE+4, 0x0);

#ifdef DFS_SUPPORT
#ifndef NEW_DFS
	RTMPPrepareRadarDetectParams(pAd);
#endif
#endif // DFS_SUPPORT //

	// reset WCID table 
	for (i=1; i<255; i++)
	{
		offset = MAC_WCID_BASE + (i * HW_WCID_ENTRY_SIZE);	
		RTMP_IO_WRITE32(pAd, offset, 0x0);
		RTMP_IO_WRITE32(pAd, offset+4, 0x0);
	}

	
	pAd->MacTab.Content[0].Addr[0] = 0x01;
	pAd->MacTab.Content[0].HTPhyMode.field.MODE = MODE_OFDM;
	pAd->MacTab.Content[0].HTPhyMode.field.MCS = 3;
	pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel;
	
	AsicBBPAdjust(pAd);

	// Clear BG-Protection flag
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_BG_PROTECTION_INUSED);	
	AsicSwitchChannel(pAd, pAd->CommonCfg.CentralChannel, FALSE);
	AsicLockChannel(pAd, pAd->CommonCfg.CentralChannel);
 	
	MlmeSetTxPreamble(pAd, (USHORT)pAd->CommonCfg.TxPreamble);	
	for (apidx = 0; apidx < pAd->ApCfg.BssidNum; apidx++)
	{
		MlmeUpdateTxRates(pAd, FALSE, apidx);
#ifdef DOT11_N_SUPPORT
		if (pAd->CommonCfg.PhyMode > PHY_11G)
			MlmeUpdateHtTxRates(pAd, apidx);
#endif // DOT11_N_SUPPORT //
	}
	
	// Set the RadarDetect Mode as Normal, bc the APUpdateAllBeaconFram() will refer this parameter.
	pAd->CommonCfg.RadarDetect.RDMode = RD_NORMAL_MODE;
#if 0//xyyou from Evan to fix rekey fail after setting Ioctl
	// start sending BEACON out
	APMakeAllBssBeacon(pAd);
	APUpdateAllBeaconFrame(pAd);
#endif

	// Disable Protection first.
	AsicUpdateProtect(pAd, 0, (ALLN_SETPROTECT|CCKSETPROTECT|OFDMSETPROTECT), TRUE, FALSE);
	
	APUpdateCapabilityAndErpIe(pAd);
#ifdef DOT11_N_SUPPORT
	APUpdateOperationMode(pAd);
#endif // DOT11_N_SUPPORT //

#ifdef A_BAND_SUPPORT
	if ( (pAd->CommonCfg.Channel > 14)
		&& (pAd->CommonCfg.bIEEE80211H == 1)
		&& RadarChannelCheck(pAd, pAd->CommonCfg.Channel))
	{
		pAd->CommonCfg.RadarDetect.RDMode = RD_SILENCE_MODE;
		pAd->CommonCfg.RadarDetect.RDCount = 0;
		pAd->CommonCfg.RadarDetect.InServiceMonitorCount = 0;
#ifdef DFS_SUPPORT
		BbpRadarDetectionStart(pAd); // start Radar detection.
#endif // DFS_SUPPORT //
	}
	else
#endif // A_BAND_SUPPORT //	
	{
		pAd->CommonCfg.RadarDetect.RDMode = RD_NORMAL_MODE;
		AsicEnableBssSync(pAd);
#ifdef CONFIG_AP_SUPPORT
#ifdef CARRIER_DETECTION_SUPPORT
		if (pAd->CommonCfg.Channel <= 14)
		{
			if (pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth  == BW_40)
			{
				if ((pAd->CommonCfg.CarrierDetect.Enable == 0)
						&& ((pAd->CommonCfg.RadarDetect.RDDurRegion == JAP)
							|| (pAd->CommonCfg.RadarDetect.RDDurRegion == JAP_W53)
							|| (pAd->CommonCfg.RadarDetect.RDDurRegion == JAP_W56)))
				{
					pAd->CommonCfg.CarrierDetect.Enable = 1;
				}
			}
		}
#ifdef A_BAND_SUPPORT		
		else
		{
			if ((pAd->CommonCfg.CarrierDetect.Enable == 0)
				&& ((pAd->CommonCfg.RadarDetect.RDDurRegion == JAP)
					|| (pAd->CommonCfg.RadarDetect.RDDurRegion == JAP_W53)
					|| (pAd->CommonCfg.RadarDetect.RDDurRegion == JAP_W56)))
			{
				pAd->CommonCfg.CarrierDetect.Enable = 1;
			}
		}
#endif // A_BAND_SUPPORT //

		if (pAd->CommonCfg.CarrierDetect.Enable == TRUE)
		{
			// trun on Carrier-Detection. (Carrier-Detect with CTS protection).
			CARRIER_DETECT_START(pAd, 1);
		}
#endif // CARRIER_DETECTION_SUPPORT //
#endif // CONFIG_AP_SUPPORT //
	}

	// Pre-tbtt interrupt setting.
	RTMP_IO_READ32(pAd, INT_TIMER_CFG, &Value);
	Value &= 0xffff0000;
	Value |= 6 << 4; // Pre-TBTT is 6ms before TBTT interrupt. 1~10 ms is reasonable.
	RTMP_IO_WRITE32(pAd, INT_TIMER_CFG, Value);
	// Enable pre-tbtt interrupt
	RTMP_IO_READ32(pAd, INT_TIMER_EN, &Value);
	Value |=0x1;
	RTMP_IO_WRITE32(pAd, INT_TIMER_EN, Value);

	// Set LED
#ifdef LED_CONTROL_SUPPORT
	RTMPSetLED(pAd, LED_LINK_UP);
#endif // LED_CONTROL_SUPPORT //

#if 0//xyyou from Evan to fix rekey fail after setting Ioctl	
#ifdef WAPI_SUPPORT
	RTMPStartWapiRekeyTimerAction(pAd, NULL);
#endif // WAPI_SUPPORT //
#endif

	/* Initialize security variable per entry, 
		1. 	pairwise key table, re-set all WCID entry as NO-security mode.
		2.	access control port status
	*/
	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
	{
		pAd->MacTab.Content[i].PortSecured  = WPA_802_1X_PORT_NOT_SECURED;
		AsicRemovePairwiseKeyEntry(pAd, (UCHAR)i);
	}
		
	// Init Security variables
	for (apidx = 0; apidx < pAd->ApCfg.BssidNum; apidx++)
	{
		USHORT		Wcid = 0;	
		PMULTISSID_STRUCT	pMbss = &pAd->ApCfg.MBSSID[apidx];

		pMbss->PortSecured = WPA_802_1X_PORT_NOT_SECURED;

		if (IS_WPA_CAPABILITY(pMbss->AuthMode))
		{   
			pMbss->DefaultKeyId = 1;
		}

		/* Get a specific WCID to record this MBSS key attribute */
		GET_GroupKey_WCID(Wcid, apidx);

		// When WEP, TKIP or AES is enabled, set group key info to Asic
		if (pMbss->WepStatus == Ndis802_11WEPEnabled)
		{
    		UCHAR	CipherAlg;
			UCHAR	idx;

			for (idx=0; idx < SHARE_KEY_NUM; idx++)
			{
				CipherAlg = pAd->SharedKey[apidx][idx].CipherAlg;

				if (pAd->SharedKey[apidx][idx].KeyLen > 0)
				{
					// Set key material to Asic
    				AsicAddSharedKeyEntry(pAd, apidx, idx, &pAd->SharedKey[apidx][idx]);	
		
					if (idx == pMbss->DefaultKeyId)
					{
						/* Generate 3-bytes IV randomly for software encryption using */						
				    	for(i = 0; i < LEN_WEP_TSC; i++)
							pAd->SharedKey[apidx][idx].TxTsc[i] = RandomByte(pAd);   
											
						/* Update WCID attribute table and IVEIV table */
						RTMPSetWcidSecurityInfo(pAd, 
												apidx, 
												idx, 												
												CipherAlg,
												Wcid, 
												SHAREDKEYTABLE);					
					}
				}
			}
    		}
		else if ((pMbss->WepStatus == Ndis802_11Encryption2Enabled) ||
				 (pMbss->WepStatus == Ndis802_11Encryption3Enabled) ||
				 (pMbss->WepStatus == Ndis802_11Encryption4Enabled))
		{
#if 0//xyyou from Evan to fix rekey fail after setting Ioctl
			// Group rekey related
			if ((pMbss->WPAREKEY.ReKeyInterval != 0) 
				&& ((pMbss->WPAREKEY.ReKeyMethod == TIME_REKEY) || (
					pMbss->WPAREKEY.ReKeyMethod == PKT_REKEY))) 
			{
				// Regularly check the timer
				if (pMbss->REKEYTimerRunning == FALSE)
				{
					RTMPSetTimer(&pMbss->REKEYTimer, GROUP_KEY_UPDATE_EXEC_INTV);

					pMbss->REKEYTimerRunning = TRUE;
					pMbss->REKEYCOUNTER = 0;
				}
				DBGPRINT(RT_DEBUG_TRACE, (" %s : Group rekey method= %ld , interval = 0x%lx\n",
											__FUNCTION__, pMbss->WPAREKEY.ReKeyMethod,
											pMbss->WPAREKEY.ReKeyInterval));
			}
			else
				pMbss->REKEYTimerRunning = FALSE;
#endif

			/* Generate GMK and GNonce randomly per MBSS */
			GenRandom(pAd, pMbss->Bssid, pMbss->GMK);
			GenRandom(pAd, pMbss->Bssid, pMbss->GNonce);		

			/* Derive GTK per BSSID */
			WpaDeriveGTK(pMbss->GMK, 
						(UCHAR*)pMbss->GNonce, 
						pMbss->Bssid, 
						pMbss->GTK, 
						LEN_TKIP_GTK);

			/* Install Shared key */
			WPAInstallSharedKey(pAd, 
								pMbss->GroupKeyWepStatus, 
								apidx, 
								pMbss->DefaultKeyId, 
								Wcid,
								TRUE,
								pMbss->GTK);
		
		}
#ifdef WAPI_SUPPORT
		else if (pMbss->WepStatus == Ndis802_11EncryptionSMS4Enabled)
		{	
			INT	cnt;
		
			// Initial the related variables
			pMbss->DefaultKeyId = 0;
			NdisMoveMemory(pMbss->key_announce_flag, AE_BCAST_PN, LEN_WAPI_TSC);
			if (pAd->MACVersion < RALINK_3883_VERSION)
				pMbss->sw_wpi_encrypt = TRUE;
			else
				pMbss->sw_wpi_encrypt = FALSE;					

			// Generate NMK randomly
			for (cnt = 0; cnt < LEN_WAPI_NMK; cnt++)
				pMbss->NMK[cnt] = RandomByte(pAd);
			
			/* Count GTK for this BSSID */
			RTMPDeriveWapiGTK(pMbss->NMK, pMbss->GTK);

			/* Install Shared key */
			WAPIInstallSharedKey(pAd, 
								 pMbss->GroupKeyWepStatus, 
								 apidx, 
								 pMbss->DefaultKeyId, 
								 Wcid,
								 pMbss->GTK);
			
		}
#endif // WAPI_SUPPORT //

#ifdef DOT1X_SUPPORT
		// Send singal to daemon to indicate driver had restarted
		if ((pMbss->AuthMode == Ndis802_11AuthModeWPA) || (pMbss->AuthMode == Ndis802_11AuthModeWPA2)
        		|| (pMbss->AuthMode == Ndis802_11AuthModeWPA1WPA2) || (pMbss->IEEE8021X == TRUE))
		{
			POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
		
	        	SendSingalToDaemon(SIGUSR1, pObj->apd_pid);
    		}
#endif // DOT1X_SUPPORT //

		DBGPRINT(RT_DEBUG_TRACE, ("### BSS(%d) AuthMode(%d)=%s, WepStatus(%d)=%s , AccessControlList.Policy=%ld\n", apidx, pMbss->AuthMode, GetAuthMode(pMbss->AuthMode), 
																  pMbss->WepStatus, GetEncryptType(pMbss->WepStatus), pMbss->AccessControlList.Policy));
	}

	// Disable Protection first.
	//AsicUpdateProtect(pAd, 0, (ALLN_SETPROTECT|CCKSETPROTECT|OFDMSETPROTECT), TRUE, FALSE);
#ifdef PIGGYBACK_SUPPORT
	RTMPSetPiggyBack(pAd, pAd->CommonCfg.bPiggyBackCapable);
#endif // PIGGYBACK_SUPPORT //
#ifdef AP_LOG_EVENT_SUPPORT
	ApLogEvent(pAd, pAd->CurrentAddress, EVENT_RESET_ACCESS_POINT);
#endif // AP_LOG_EVENT_SUPPORT //
	pAd->Mlme.PeriodicRound = 0;
	pAd->Mlme.OneSecPeriodicRound = 0;

	OPSTATUS_SET_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED);

	RTMP_IndicateMediaState(pAd);


	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);

	// start sending BEACON out
	APMakeAllBssBeacon(pAd);
	APUpdateAllBeaconFrame(pAd);

	if ( (pAd->CommonCfg.Channel > 14)
		&& (pAd->CommonCfg.bIEEE80211H == 1)
		&& RadarChannelCheck(pAd, pAd->CommonCfg.Channel))
	{
		pAd->CommonCfg.RadarDetect.RDMode = RD_SILENCE_MODE;
		pAd->CommonCfg.RadarDetect.RDCount = 0;
		pAd->CommonCfg.RadarDetect.InServiceMonitorCount = 0;
#ifdef DFS_SUPPORT
		BbpRadarDetectionStart(pAd); // start Radar detection.
#endif // DFS_SUPPORT //
	}
	else
	{
		pAd->CommonCfg.RadarDetect.RDMode = RD_NORMAL_MODE;
		AsicEnableBssSync(pAd);
#ifdef CONFIG_AP_SUPPORT
#ifdef CARRIER_DETECTION_SUPPORT
		if (pAd->CommonCfg.Channel <= 14)
		{
			if (pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth  == BW_40)
			{
				if ((pAd->CommonCfg.CarrierDetect.Enable == 0)
						&& ((pAd->CommonCfg.RadarDetect.RDDurRegion == JAP)
							|| (pAd->CommonCfg.RadarDetect.RDDurRegion == JAP_W53)
							|| (pAd->CommonCfg.RadarDetect.RDDurRegion == JAP_W56)))
				{
					pAd->CommonCfg.CarrierDetect.Enable = 1;
				}
			}
		}
		else
		{
			if ((pAd->CommonCfg.CarrierDetect.Enable == 0)
				&& ((pAd->CommonCfg.RadarDetect.RDDurRegion == JAP)
					|| (pAd->CommonCfg.RadarDetect.RDDurRegion == JAP_W53)
					|| (pAd->CommonCfg.RadarDetect.RDDurRegion == JAP_W56)))
			{
				pAd->CommonCfg.CarrierDetect.Enable = 1;
			}
		}

		if (pAd->CommonCfg.CarrierDetect.Enable == TRUE)
		{
			// trun on Carrier-Detection. (Carrier-Detect with CTS protection).
			CARRIER_DETECT_START(pAd, 1);
		}
#endif // CARRIER_DETECTION_SUPPORT //
#endif // CONFIG_AP_SUPPORT //
	}

	// Pre-tbtt interrupt setting.
	RTMP_IO_READ32(pAd, INT_TIMER_CFG, &Value);
	Value &= 0xffff0000;
	Value |= 6 << 4; // Pre-TBTT is 6ms before TBTT interrupt. 1~10 ms is reasonable.
	RTMP_IO_WRITE32(pAd, INT_TIMER_CFG, Value);
	// Enable pre-tbtt interrupt
	RTMP_IO_READ32(pAd, INT_TIMER_EN, &Value);
	Value |=0x1;
	RTMP_IO_WRITE32(pAd, INT_TIMER_EN, Value);

#ifdef WAPI_SUPPORT
	RTMPStartWapiRekeyTimerAction(pAd, NULL);
#endif // WAPI_SUPPORT //

	/* Set group re-key timer if necessary. 
	   It must be processed after clear flag "fRTMP_ADAPTER_HALT_IN_PROGRESS" */
	for (apidx = 0; apidx < pAd->ApCfg.BssidNum; apidx++)
	{
		PMULTISSID_STRUCT	pMbss = &pAd->ApCfg.MBSSID[apidx];

		if ((pMbss->WepStatus == Ndis802_11Encryption2Enabled) ||
			(pMbss->WepStatus == Ndis802_11Encryption3Enabled) ||
			(pMbss->WepStatus == Ndis802_11Encryption4Enabled))
		{
			// Group rekey related
			if ((pMbss->WPAREKEY.ReKeyInterval != 0) 
				&& ((pMbss->WPAREKEY.ReKeyMethod == TIME_REKEY) || (
					pMbss->WPAREKEY.ReKeyMethod == PKT_REKEY))) 
			{
				// Regularly check the timer
				if (pMbss->REKEYTimerRunning == FALSE)
				{
					RTMPSetTimer(&pMbss->REKEYTimer, GROUP_KEY_UPDATE_EXEC_INTV);

					pMbss->REKEYTimerRunning = TRUE;
					pMbss->REKEYCOUNTER = 0;
				}
				DBGPRINT(RT_DEBUG_TRACE, (" %s : Group rekey method= %ld , interval = 0x%lx\n",
											__FUNCTION__, pMbss->WPAREKEY.ReKeyMethod,
											pMbss->WPAREKEY.ReKeyInterval));
			}
			else
				pMbss->REKEYTimerRunning = FALSE;
		}
	}
	
#ifdef WDS_SUPPORT
	// Prepare WEP key
	WdsPrepareWepKeyFromMainBss(pAd);

	// Add wds key infomation to ASIC	
	AsicUpdateWdsRxWCIDTable(pAd);
#endif // WDS_SUPPORT //

#ifdef IDS_SUPPORT
	// Start IDS timer
	if (pAd->ApCfg.IdsEnable)
	{
		if (pAd->CommonCfg.bWirelessEvent == FALSE)
			DBGPRINT(RT_DEBUG_WARN, ("!!! WARNING !!! The WirelessEvent parameter doesn't be enabled \n"));
		
		RTMPIdsStart(pAd);
	}
#endif // IDS_SUPPORT //
#ifdef RTMP_INTERNAL_TX_ALC
	//if (pAd->TxPowerCtrl.bInternalTxALC == TRUE)
	{
		for (i = 0; i < MAX_NUM_OF_CHANNELS; i++)
		{
			if (pAd->CommonCfg.Channel == pAd->TxPower[i].Channel)
			{
				pAd->TxPowerCtrl.idxTxPowerTable = pAd->TxPower[i].Power;
				if ((pAd->TxPowerCtrl.idxTxPowerTable < LOWERBOUND_TX_POWER_TUNING_ENTRY) || 
					(pAd->TxPowerCtrl.idxTxPowerTable > UPPERBOUND_TX_POWER_TUNING_ENTRY))
				{
					pAd->TxPowerCtrl.idxTxPowerTable = 0; // default array index					
				}
				break;
			}
		}
		DBGPRINT(RT_DEBUG_ERROR, ("Channel = %d, PwrIdx = %d\n", pAd->CommonCfg.Channel, pAd->TxPowerCtrl.idxTxPowerTable));
	}
#endif // RTMP_INTERNAL_TX_ALC //




#ifdef WSC_AP_SUPPORT
	if(!WPSLedTimerRunning){
		RTMPInitTimer(pAd, &WPSLedTimer, GET_TIMER_FUNCTION(WPSLedPeriodicExec), pAd, TRUE);
		RTMPSetTimer(&WPSLedTimer, WPS_LED_TIMER_INTERVAL);
		WPSLedTimerRunning = TRUE;
	}
#endif
	DBGPRINT(RT_DEBUG_TRACE, ("<=== APStartUp\n"));
}

/*
	==========================================================================
	Description:
		disassociate all STAs and stop AP service.
	Note:
	==========================================================================
 */
VOID APStop(
	IN PRTMP_ADAPTER pAd) 
{
	BOOLEAN     Cancelled;
	UINT32		Value;
	INT			apidx;
	
	DBGPRINT(RT_DEBUG_TRACE, ("!!! APStop !!!\n"));

#ifdef DFS_SUPPORT
	{
		RadarDetectionStop(pAd);
		BbpRadarDetectionStop(pAd);
	}
#endif // DFS_SUPPORT //


#ifdef WDS_SUPPORT
	WdsDown(pAd);
#endif // WDS_SUPPORT //

	MacTableReset(pAd);

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);

	// Disable pre-tbtt interrupt
	RTMP_IO_READ32(pAd, INT_TIMER_EN, &Value);
	Value &=0xe;
	RTMP_IO_WRITE32(pAd, INT_TIMER_EN, Value);
	// Disable piggyback
	RTMPSetPiggyBack(pAd, FALSE);

   	AsicUpdateProtect(pAd, 0,  (ALLN_SETPROTECT|CCKSETPROTECT|OFDMSETPROTECT), TRUE, FALSE);

	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))
	{
		//RTMP_ASIC_INTERRUPT_DISABLE(pAd);
		AsicDisableSync(pAd);
		// Set LED
#ifdef LED_CONTROL_SUPPORT		
		RTMPSetLED(pAd, LED_LINK_DOWN);
#endif // LED_CONTROL_SUPPORT //
	}



	for (apidx = 0; apidx < MAX_MBSSID_NUM; apidx++)
	{
		if (pAd->ApCfg.MBSSID[apidx].REKEYTimerRunning == TRUE)
		{
			RTMPCancelTimer(&pAd->ApCfg.MBSSID[apidx].REKEYTimer, &Cancelled);
			pAd->ApCfg.MBSSID[apidx].REKEYTimerRunning = FALSE;
		}
	}

	if (pAd->ApCfg.CMTimerRunning == TRUE)
	{
		RTMPCancelTimer(&pAd->ApCfg.CounterMeasureTimer, &Cancelled);
		pAd->ApCfg.CMTimerRunning = FALSE;
	}
	
#ifdef WAPI_SUPPORT
	RTMPCancelWapiRekeyTimerAction(pAd, NULL);
#endif // WAPI_SUPPORT //
	
	//
	// Cancel the Timer, to make sure the timer was not queued.
	//
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED);

#ifdef IDS_SUPPORT
	// if necessary, cancel IDS timer
	RTMPIdsStop(pAd);
#endif // IDS_SUPPORT //

#ifdef WSC_AP_SUPPORT
	if(WPSLedTimerRunning){
		RTMPCancelTimer(&WPSLedTimer, &Cancelled);
		WPSLedTimerRunning = FALSE;
	}
#endif

}

/*
	==========================================================================
	Description:
		This routine is used to clean up a specified power-saving queue. It's
		used whenever a wireless client is deleted.
	==========================================================================
 */
VOID APCleanupPsQueue(
	IN  PRTMP_ADAPTER   pAd,
	IN  PQUEUE_HEADER   pQueue)
{
	PQUEUE_ENTRY pEntry;
	PNDIS_PACKET pPacket;

	DBGPRINT(RT_DEBUG_TRACE, ("APCleanupPsQueue (0x%08lx)...\n", (ULONG)pQueue));

	while (pQueue->Head)
	{
		DBGPRINT(RT_DEBUG_TRACE,
					("APCleanupPsQueue %ld...\n",pQueue->Number));

		pEntry = RemoveHeadQueue(pQueue);
		//pPacket = CONTAINING_RECORD(pEntry, NDIS_PACKET, MiniportReservedEx);
		pPacket = QUEUE_ENTRY_TO_PACKET(pEntry);
		RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);
	}
}

/*
	==========================================================================
	Description:
		This routine is called by APMlmePeriodicExec() every second to check if
		1. any associated client in PSM. If yes, then TX MCAST/BCAST should be
		   out in DTIM only
		2. any client being idle for too long and should be aged-out from MAC table
		3. garbage collect PSQ
	==========================================================================
*/
VOID MacTableMaintenance(
	IN PRTMP_ADAPTER pAd)
{
	int i;
#ifdef DOT11_N_SUPPORT
	BOOLEAN	bRdgActive;
#endif // DOT11_N_SUPPORT //
#ifdef RTMP_MAC_PCI
	unsigned long	IrqFlags;
#endif // RTMP_MAC_PCI //
	UINT	fAnyStationPortSecured[MAX_MBSSID_NUM];
 	UINT 	bss_index;
	MAC_TABLE *pMacTable;

	for (bss_index = BSS0; bss_index < MAX_MBSSID_NUM; bss_index++)
		fAnyStationPortSecured[bss_index] = 0;

	pMacTable = &pAd->MacTab;
	pMacTable->fAnyStationInPsm = FALSE;
	pMacTable->fAnyStationBadAtheros = FALSE;
	pMacTable->fAnyTxOPForceDisable = FALSE;
	pMacTable->fAllStationAsRalink = TRUE;
#ifdef DOT11_N_SUPPORT
	pMacTable->fAnyStationNonGF = FALSE;
	pMacTable->fAnyStation20Only = FALSE;
	pMacTable->fAnyStationIsLegacy = FALSE;
	pMacTable->fAnyStationMIMOPSDynamic = FALSE;
	//Support Green AP
	pMacTable->fAnyStationIsHT=FALSE;

#ifdef DOT11N_DRAFT3
	pMacTable->fAnyStaFortyIntolerant = FALSE;
#endif // DOT11N_DRAFT3 //
#endif // DOT11_N_SUPPORT //

#ifdef WAPI_SUPPORT
	pMacTable->fAnyWapiStation = FALSE;
#endif // WAPI_SUPPORT //

	for (i = 1; i < MAX_LEN_OF_MAC_TABLE; i++) 
	{
		MAC_TABLE_ENTRY *pEntry = &pMacTable->Content[i];
		BOOLEAN bDisconnectSta = FALSE;

		if (!IS_ENTRY_CLIENT(pEntry))
			continue;

		if (pEntry->NoDataIdleCount == 0)
			pEntry->StationKeepAliveCount = 0;

		pEntry->NoDataIdleCount ++;  
		pEntry->StaConnectTime ++;

		// 0. STA failed to complete association should be removed to save MAC table space.
		if ((pEntry->Sst != SST_ASSOC) && (pEntry->NoDataIdleCount >= pEntry->AssocDeadLine))
		{
			DBGPRINT(RT_DEBUG_TRACE, ("%02x:%02x:%02x:%02x:%02x:%02x fail to complete ASSOC in %d sec\n",
					pEntry->Addr[0],pEntry->Addr[1],pEntry->Addr[2],pEntry->Addr[3],
					pEntry->Addr[4],pEntry->Addr[5],MAC_TABLE_ASSOC_TIMEOUT));
			MacTableDeleteEntry(pAd, pEntry->Aid, pEntry->Addr);
			continue;
		}

		// 1. check if there's any associated STA in power-save mode. this affects outgoing
		//    MCAST/BCAST frames should be stored in PSQ till DtimCount=0
		if (pEntry->PsMode == PWR_SAVE)
			pMacTable->fAnyStationInPsm = TRUE;

#ifdef DOT11_N_SUPPORT
		if (pEntry->MmpsMode == MMPS_DYNAMIC)
		{
			pMacTable->fAnyStationMIMOPSDynamic = TRUE;
		}

		if (pEntry->MaxHTPhyMode.field.BW == BW_20)
			pMacTable->fAnyStation20Only = TRUE;

		if (pEntry->MaxHTPhyMode.field.MODE != MODE_HTGREENFIELD)
			pMacTable->fAnyStationNonGF = TRUE;

		if ((pEntry->MaxHTPhyMode.field.MODE == MODE_OFDM) || (pEntry->MaxHTPhyMode.field.MODE == MODE_CCK))
		{
			pMacTable->fAnyStationIsLegacy = TRUE;
		}
		else
		{
			pMacTable->fAnyStationIsHT=TRUE;
		}
		
#ifdef DOT11N_DRAFT3
		if (pEntry->bForty_Mhz_Intolerant)
			pMacTable->fAnyStaFortyIntolerant = TRUE;
#endif // DOT11N_DRAFT3 //
#endif // DOT11_N_SUPPORT //
		
		if (pEntry->bIAmBadAtheros)
		{
			pMacTable->fAnyStationBadAtheros = TRUE;
#ifdef DOT11_N_SUPPORT
			if (pAd->CommonCfg.IOTestParm.bRTSLongProtOn == FALSE)
				AsicUpdateProtect(pAd, 8, ALLN_SETPROTECT, FALSE, pMacTable->fAnyStationNonGF);
#endif // DOT11_N_SUPPORT //
			if (pEntry->WepStatus != Ndis802_11EncryptionDisabled)
			{
				pMacTable->fAnyTxOPForceDisable = TRUE;
			}
		}

		// detect the station alive status
#ifdef WMM_ACM_SUPPORT
		/* WMM ACM: QAP can send any packet to QSTA even no any TSPEC is built */
//		if (ACMP_IsAllACEnabled(pAd) == ACM_RTN_FAIL)
#endif // WMM_ACM_SUPPORT //
		// detect the station alive status
		if ((pAd->ApCfg.MBSSID[pEntry->apidx].StationKeepAliveTime > 0) &&
			(pEntry->NoDataIdleCount >= pAd->ApCfg.MBSSID[pEntry->apidx].StationKeepAliveTime))
		{
			MULTISSID_STRUCT *pMbss = &pAd->ApCfg.MBSSID[pEntry->apidx];

			/*
				If no any data success between ap and the station for
				StationKeepAliveTime, try to detect whether the station is
				still alive.

				Note: Just only keepalive station function, no disassociation
				function if too many no response.
			*/

			/*
				For example as below:

				1. Station in ACTIVE mode,

		        ......
		        sam> tx ok!
		        sam> count = 1!	 ==> 1 second after the Null Frame is acked
		        sam> count = 2!	 ==> 2 second after the Null Frame is acked
		        sam> count = 3!
		        sam> count = 4!
		        sam> count = 5!
		        sam> count = 6!
		        sam> count = 7!
		        sam> count = 8!
		        sam> count = 9!
		        sam> count = 10!
		        sam> count = 11!
		        sam> count = 12!
		        sam> count = 13!
		        sam> count = 14!
		        sam> count = 15! ==> 15 second after the Null Frame is acked
		        sam> tx ok!      ==> (KeepAlive Mechanism) send a Null Frame to
										detect the STA life status
		        sam> count = 1!  ==> 1 second after the Null Frame is acked
		        sam> count = 2!
		        sam> count = 3!
		        sam> count = 4!
		        ......

				If the station acknowledges the QoS Null Frame,
				the NoDataIdleCount will be reset to 0.


				2. Station in legacy PS mode,

				We will set TIM bit after 15 seconds, the station will send a
				PS-Poll frame and we will send a QoS Null frame to it.
				If the station acknowledges the QoS Null Frame, the
				NoDataIdleCount will be reset to 0.


				3. Station in legacy UAPSD mode,

				Currently we don��t support the keep alive mechanism.
				So if your station is in UAPSD mode, the station will be
				kicked out after 300 seconds.

				Note: the rate of QoS Null frame can not be 1M of 2.4GHz or
				6M of 5GHz, or no any statistics count will occur.
			*/

			if (pEntry->StationKeepAliveCount++ == 0)
			{
				if (pEntry->PsMode == PWR_SAVE)
				{
					// use TIM bit to detect the PS station
					WLAN_MR_TIM_BIT_SET(pAd, pEntry->apidx, pEntry->Aid);
				}
				else
				{
					// use Null or QoS Null to detect the ACTIVE station
					BOOLEAN bQosNull = FALSE;
	
					if (CLIENT_STATUS_TEST_FLAG(pEntry, fCLIENT_STATUS_WMM_CAPABLE))
						bQosNull = TRUE;

		            ApEnqueueNullFrame(pAd, pEntry->Addr, pEntry->CurrTxRate,
	    	                           pEntry->Aid, pEntry->apidx, bQosNull, TRUE, 0);
				}
			}
			else
			{
				if (pEntry->StationKeepAliveCount >= pMbss->StationKeepAliveTime)
					pEntry->StationKeepAliveCount = 0;
			}
		}

		// 2. delete those MAC entry that has been idle for a long time
		if (pEntry->PsMode == PWR_SAVE)
		{
			pEntry->ContinueTxFailCnt = 0;
		}

		if (pEntry->NoDataIdleCount >= pEntry->StaIdleTimeout)
		{
			bDisconnectSta = TRUE;
			DBGPRINT(RT_DEBUG_WARN, ("ageout %02x:%02x:%02x:%02x:%02x:%02x after %d-sec silence\n",
					pEntry->Addr[0],pEntry->Addr[1],pEntry->Addr[2],pEntry->Addr[3],
					pEntry->Addr[4],pEntry->Addr[5],MAC_TABLE_AGEOUT_TIME));
#ifdef AP_LOG_EVENT_SUPPORT			
			ApLogEvent(pAd, pEntry->Addr, EVENT_AGED_OUT);
#endif // AP_LOG_EVENT_SUPPORT //
		}
		else if (pEntry->ContinueTxFailCnt >= pAd->ApCfg.EntryLifeCheck)
		{
			bDisconnectSta = TRUE;
			DBGPRINT(RT_DEBUG_WARN, ("STA-%02x:%02x:%02x:%02x:%02x:%02x had left\n",
					pEntry->Addr[0],pEntry->Addr[1],pEntry->Addr[2],pEntry->Addr[3],
					pEntry->Addr[4],pEntry->Addr[5]));			
		}

		if (bDisconnectSta)
		{
			// send wireless event - for ageout 
			if (pAd->CommonCfg.bWirelessEvent)
				RTMPSendWirelessEvent(pAd, IW_AGEOUT_EVENT_FLAG, pEntry->Addr, 0, 0); 

			if (pEntry->Sst == SST_ASSOC)
			{
				PUCHAR      pOutBuffer = NULL;
				NDIS_STATUS NStatus;
				ULONG       FrameLen = 0;
				HEADER_802_11 DeAuthHdr;
				USHORT      Reason;

				//  send out a DISASSOC request frame
				NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);
				if (NStatus != NDIS_STATUS_SUCCESS) 
				{
					DBGPRINT(RT_DEBUG_TRACE, (" MlmeAllocateMemory fail  ..\n"));
					//NdisReleaseSpinLock(&pAd->MacTabLock);
					continue;
				}
				Reason = REASON_DEAUTH_STA_LEAVING;
				DBGPRINT(RT_DEBUG_WARN, ("Send DEAUTH - Reason = %d frame  TO %x %x %x %x %x %x \n",Reason,pEntry->Addr[0],
										pEntry->Addr[1],pEntry->Addr[2],pEntry->Addr[3],pEntry->Addr[4],pEntry->Addr[5]));
				MgtMacHeaderInit(pAd, &DeAuthHdr, SUBTYPE_DEAUTH, 0, pEntry->Addr, pAd->ApCfg.MBSSID[pEntry->apidx].Bssid);				
		    	MakeOutgoingFrame(pOutBuffer,            &FrameLen, 
		    	                  sizeof(HEADER_802_11), &DeAuthHdr, 
		    	                  2,                     &Reason, 
		    	                  END_OF_ARGS);				
		    	MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
		    	MlmeFreeMemory(pAd, pOutBuffer);
			}

			MacTableDeleteEntry(pAd, pEntry->Aid, pEntry->Addr);
			continue;
		}

		// 3. garbage collect the PsQueue if the STA has being idle for a while
		if (pEntry->PsQueue.Head)
		{
			pEntry->PsQIdleCount ++;  
			if (pEntry->PsQIdleCount > 2) 
			{
/*****ralink patch. 20100920 shnwind.*****/				
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
				RTMP_IRQ_LOCK(&pAd->irq_lock, IrqFlags);
#else				
				NdisAcquireSpinLock(&pAd->irq_lock);
#endif				
				APCleanupPsQueue(pAd, &pEntry->PsQueue);
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
				RTMP_IRQ_UNLOCK(&pAd->irq_lock, IrqFlags);
#else				
				NdisReleaseSpinLock(&pAd->irq_lock);
#endif				
/*****ralink patch. 20100920 shnwind add end.*****/				
				pEntry->PsQIdleCount = 0;
				WLAN_MR_TIM_BIT_CLEAR(pAd, pEntry->apidx, pEntry->Aid);
			}
		}
		else
			pEntry->PsQIdleCount = 0;
	
#ifdef UAPSD_AP_SUPPORT
        UAPSD_QueueMaintenance(pAd, pEntry);
#endif // UAPSD_AP_SUPPORT //

		// check if this STA is Ralink-chipset 
		if (!CLIENT_STATUS_TEST_FLAG(pEntry, fCLIENT_STATUS_RALINK_CHIPSET))
			pMacTable->fAllStationAsRalink = FALSE;

		// Check if the port is secured
		if (pEntry->PortSecured == WPA_802_1X_PORT_SECURED)
			fAnyStationPortSecured[pEntry->apidx]++;
#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
		if ((pEntry->BSS2040CoexistenceMgmtSupport) 
			&& (pAd->CommonCfg.Bss2040CoexistFlag & BSS_2040_COEXIST_INFO_NOTIFY)
			&& (pAd->CommonCfg.bBssCoexEnable == TRUE)
		)
		{
			SendNotifyBWActionFrame(pAd, pEntry->Aid, pEntry->apidx);
	}
#endif // DOT11N_DRAFT3 //
#endif // DOT11_N_SUPPORT //
#ifdef WAPI_SUPPORT
		if (pEntry->WepStatus == Ndis802_11EncryptionSMS4Enabled)
			pMacTable->fAnyWapiStation = TRUE;
#endif // WAPI_SUPPORT //

	}

	// Update the state of port per MBSS
	for (bss_index = BSS0; bss_index < MAX_MBSSID_NUM; bss_index++)
	{
		if (fAnyStationPortSecured[bss_index] > 0)
			pAd->ApCfg.MBSSID[bss_index].PortSecured = WPA_802_1X_PORT_SECURED;
		else
			pAd->ApCfg.MBSSID[bss_index].PortSecured = WPA_802_1X_PORT_NOT_SECURED;
	}

#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
	if (pAd->CommonCfg.Bss2040CoexistFlag & BSS_2040_COEXIST_INFO_NOTIFY)
		pAd->CommonCfg.Bss2040CoexistFlag &= (~BSS_2040_COEXIST_INFO_NOTIFY);
#endif // DOT11N_DRAFT3 //

	// If all associated STAs are Ralink-chipset, AP shall enable RDG.
	if (pAd->CommonCfg.bRdg && pMacTable->fAllStationAsRalink)
	{
		bRdgActive = TRUE;
	}
	else
	{
		bRdgActive = FALSE;
	}
//Green AP Setting
#ifdef DOT11_N_SUPPORT
			if (pAd->Antenna.field.RxPath>1||pAd->Antenna.field.TxPath>1)
			{

				if(pAd->MacTab.fAnyStationIsHT==FALSE
					&&pAd->CommonCfg.bGreenAPEnable==TRUE)
					EnableAPMIMOPS(pAd);
				else
					DisableAPMIMOPS(pAd);

			}
#endif // DOT11_N_SUPPORT //

	if (bRdgActive != RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RDG_ACTIVE))
	{
		if (bRdgActive)
		{
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RDG_ACTIVE);
			AsicEnableRDG(pAd);
		}
		else
		{
			RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RDG_ACTIVE);
			AsicDisableRDG(pAd);
		}
	}

	if ((pMacTable->fAnyStationBadAtheros == FALSE) && (pAd->CommonCfg.IOTestParm.bRTSLongProtOn == TRUE))
	{
		AsicUpdateProtect(pAd, pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode, ALLN_SETPROTECT, FALSE, pMacTable->fAnyStationNonGF);
	}
#endif // DOT11_N_SUPPORT //

#ifdef RTMP_MAC_PCI
	RTMP_IRQ_LOCK(&pAd->irq_lock, IrqFlags);
#endif // RTMP_MAC_PCI //
	// 4. garbage collect pAd->MacTab.McastPsQueue if backlogged MCAST/BCAST frames
	//    stale in queue. Since MCAST/BCAST frames always been sent out whenever 
	//    DtimCount==0, the only case to let them stale is surprise removal of the NIC,
	//    so that ASIC-based Tbcn interrupt stops and DtimCount dead.
	if (pMacTable->McastPsQueue.Head)
	{
		UINT bss_index;

		pMacTable->PsQIdleCount ++;
		if (pMacTable->PsQIdleCount > 1)
		{
			/* Normally, should not be here;
			   because bc/mc packets will be moved to SwQueue when DTIM = 0 and
			   DTIM period < 2 seconds;
			   If enter here, it is the kernel bug or driver bug */

			//NdisAcquireSpinLock(&pAd->MacTabLock);
			APCleanupPsQueue(pAd, &pMacTable->McastPsQueue);
			//NdisReleaseSpinLock(&pAd->MacTabLock);
			pMacTable->PsQIdleCount = 0;

		        /* sanity check */
			if (pAd->ApCfg.BssidNum > MAX_MBSSID_NUM)
				pAd->ApCfg.BssidNum = MAX_MBSSID_NUM;
		        /* End of if */
	        
		        /* clear MCAST/BCAST backlog bit for all BSS */
			for(bss_index=BSS0; bss_index<pAd->ApCfg.BssidNum; bss_index++)
				WLAN_MR_TIM_BCMC_CLEAR(bss_index);
		        /* End of for */
		}
	}
	else
		pMacTable->PsQIdleCount = 0;
#ifdef RTMP_MAC_PCI
	RTMP_IRQ_UNLOCK(&pAd->irq_lock, IrqFlags);
#endif // RTMP_MAC_PCI //
}


UINT32 MacTableAssocStaNumGet(
	IN PRTMP_ADAPTER pAd)
{
	UINT32 num = 0;
	UINT32 i;


	for (i = 1; i < MAX_LEN_OF_MAC_TABLE; i++) 
	{
		MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[i];

		if (!IS_ENTRY_CLIENT(pEntry))
			continue;

		if (pEntry->Sst == SST_ASSOC)
			num ++;
	}

	return num;
}


/*
	==========================================================================
	Description:
		Look up a STA MAC table. Return its Sst to decide if an incoming
		frame from this STA or an outgoing frame to this STA is permitted.
	Return:
	==========================================================================
*/
MAC_TABLE_ENTRY *APSsPsInquiry(
	IN  PRTMP_ADAPTER   pAd, 
	IN  PUCHAR pAddr, 
	OUT SST   *Sst, 
	OUT USHORT *Aid,
	OUT UCHAR *PsMode,
	OUT UCHAR *Rate) 
{
	MAC_TABLE_ENTRY *pEntry = NULL;
	
	if (MAC_ADDR_IS_GROUP(pAddr)) // mcast & broadcast address
	{
		*Sst        = SST_ASSOC;
		*Aid        = MCAST_WCID;	// Softap supports 1 BSSID and use WCID=0 as multicast Wcid index
		*PsMode     = PWR_ACTIVE;
		*Rate       = pAd->CommonCfg.MlmeRate; 
	} 
	else // unicast address
	{
		pEntry = MacTableLookup(pAd, pAddr);
		if (pEntry) 
		{
			*Sst        = pEntry->Sst;
			*Aid        = pEntry->Aid;
			*PsMode     = pEntry->PsMode;
			if ((pEntry->AuthMode >= Ndis802_11AuthModeWPA) && (pEntry->GTKState != REKEY_ESTABLISHED))
				*Rate   = pAd->CommonCfg.MlmeRate;
			else
			*Rate       = pEntry->CurrTxRate;
		} 
		else 
		{
			*Sst        = SST_NOT_AUTH;
			*Aid        = MCAST_WCID;
			*PsMode     = PWR_ACTIVE;
			*Rate       = pAd->CommonCfg.MlmeRate; 
		}
	}
	return pEntry;
}

/*
	==========================================================================
	Description:
		Update the station current power save mode. Calling this routine also
		prove the specified client is still alive. Otherwise AP will age-out
		this client once IdleCount exceeds a threshold.
	==========================================================================
 */
BOOLEAN APPsIndicate(
	IN PRTMP_ADAPTER pAd, 
	IN PUCHAR pAddr, 
	IN ULONG Wcid, 
	IN UCHAR Psm) 
{
	MAC_TABLE_ENTRY *pEntry;
    UCHAR old_psmode;

	if (Wcid >= MAX_LEN_OF_MAC_TABLE)
	{
		return PWR_ACTIVE;	
	}

	pEntry = &pAd->MacTab.Content[Wcid];
	old_psmode = pEntry->PsMode;
	if (pEntry)	
	{
		/*
			Change power save mode first because we will call
			RTMPDeQueuePacket() in APHandleRxPsPoll().

			Or when Psm = PWR_ACTIVE, we will not do Aggregation in
			RTMPDeQueuePacket().
		*/
		pEntry->NoDataIdleCount = 0;

		if ((pEntry->PsMode == PWR_SAVE) && (Psm == PWR_ACTIVE))
		{
			pEntry->PsMode = Psm;
		
			// TODO: For RT2870, how to handle about the BA when STA in PS mode????
#ifdef RTMP_MAC_PCI
#ifdef DOT11_N_SUPPORT
			// When sta wake up, we send BAR to refresh the BA sequence.
			SendRefreshBAR(pAd, pEntry);
#endif // DOT11_N_SUPPORT //
#endif // RTMP_MAC_PCI //
			DBGPRINT(RT_DEBUG_INFO, ("APPsIndicate - %02x:%02x:%02x:%02x:%02x:%02x wakes up, act like rx PS-POLL\n", pAddr[0],pAddr[1],pAddr[2],pAddr[3],pAddr[4],pAddr[5]));
			// sleep station awakes, move all pending frames from PSQ to TXQ if any
			APHandleRxPsPoll(pAd, pAddr, pEntry->Aid, TRUE);
		}
		else
		{
			/* Update status */
			pEntry->PsMode = Psm;
		}

		/* move to above section */
//		pEntry->NoDataIdleCount = 0;
//		pEntry->PsMode = Psm;
	} 
	else 
	{
		// not in table, try to learn it ???? why bother?
	}
	return old_psmode;
}

/*
	==========================================================================
	Description:
		This routine is called to log a specific event into the event table.
		The table is a QUERY-n-CLEAR array that stop at full.
	==========================================================================
 */
 #ifdef AP_LOG_EVENT_SUPPORT
VOID ApLogEvent(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR   pAddr,
	IN USHORT   Event)
{
	if (pAd->EventTab.Num < MAX_NUM_OF_EVENT)
	{
		RT_802_11_EVENT_LOG *pLog = &pAd->EventTab.Log[pAd->EventTab.Num];
		RTMP_GetCurrentSystemTime(&pLog->SystemTime);
		COPY_MAC_ADDR(pLog->Addr, pAddr);
		pLog->Event = Event;
		DBGPRINT_RAW(RT_DEBUG_TRACE,("LOG#%ld %02x:%02x:%02x:%02x:%02x:%02x %s\n",
			pAd->EventTab.Num, pAddr[0], pAddr[1], pAddr[2], 
			pAddr[3], pAddr[4], pAddr[5], pEventText[Event]));
		pAd->EventTab.Num += 1;
	}
}
#endif // AP_LOG_EVENT_SUPPORT //

#ifdef DOT11_N_SUPPORT
/*
	==========================================================================
	Description:
		Operationg mode is as defined at 802.11n for how proteciton in this BSS operates. 
		Ap broadcast the operation mode at Additional HT Infroamtion Element Operating Mode fields.
		802.11n D1.0 might has bugs so this operating mode use  EWC MAC 1.24 definition first.

		Called when receiving my bssid beacon or beaconAtJoin to update protection mode.
		40MHz or 20MHz protection mode in HT 40/20 capabale BSS.
		As STA, this obeys the operation mode in ADDHT IE.
		As AP, update protection when setting ADDHT IE and after new STA joined.
	==========================================================================
*/
VOID APUpdateOperationMode(
	IN PRTMP_ADAPTER pAd)
{
	pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode = 0;

	if ((pAd->ApCfg.LastNoneHTOLBCDetectTime + (5 * OS_HZ)) > pAd->Mlme.Now32) // non HT BSS exist within 5 sec
	{
		pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode = 1;
    	AsicUpdateProtect(pAd, pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode, ALLN_SETPROTECT, FALSE, TRUE);
	}

   	// If I am 40MHz BSS, and there exist HT-20MHz station. 
	// Update to 2 when it's zero.  Because OperaionMode = 1 or 3 has more protection.
	if ((pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode == 0) && (pAd->MacTab.fAnyStation20Only) && (pAd->CommonCfg.DesiredHtPhy.ChannelWidth == 1))
	{
		pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode = 2;
		AsicUpdateProtect(pAd, pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode, (ALLN_SETPROTECT), TRUE, pAd->MacTab.fAnyStationNonGF);
	}
		
	if (pAd->MacTab.fAnyStationIsLegacy)
	{
		pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode = 3;
		AsicUpdateProtect(pAd, pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode, (ALLN_SETPROTECT), TRUE, pAd->MacTab.fAnyStationNonGF);
	}
	
	pAd->CommonCfg.AddHTInfo.AddHtInfo2.NonGfPresent = pAd->MacTab.fAnyStationNonGF;
}
#endif // DOT11_N_SUPPORT //

/*
	==========================================================================
	Description:
		Update ERP IE and CapabilityInfo based on STA association status.
		The result will be auto updated into the next outgoing BEACON in next
		TBTT interrupt service routine
	==========================================================================
 */
VOID APUpdateCapabilityAndErpIe(
	IN PRTMP_ADAPTER pAd)
{
	UCHAR  i, ErpIeContent = 0;
	BOOLEAN ShortSlotCapable = pAd->CommonCfg.bUseShortSlotTime;
	UCHAR	apidx;
	BOOLEAN	bUseBGProtection;
	BOOLEAN	LegacyBssExist;


	if (pAd->CommonCfg.PhyMode == PHY_11B)
		return;

	for (i=1; i<MAX_LEN_OF_MAC_TABLE; i++)
	{
		PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];
		if (!IS_ENTRY_CLIENT(pEntry) || (pEntry->Sst != SST_ASSOC))
			continue;

		// at least one 11b client associated, turn on ERP.NonERPPresent bit
		// almost all 11b client won't support "Short Slot" time, turn off for maximum compatibility
		if (pEntry->MaxSupportedRate < RATE_FIRST_OFDM_RATE)
		{
			ShortSlotCapable = FALSE;
			ErpIeContent |= 0x01;
		}

		// at least one client can't support short slot
		if ((pEntry->CapabilityInfo & 0x0400) == 0)
			ShortSlotCapable = FALSE;
	}

	// legacy BSS exist within 5 sec
	if ((pAd->ApCfg.LastOLBCDetectTime + (5 * OS_HZ)) > pAd->Mlme.Now32) 
	{
		LegacyBssExist = TRUE;
	}
	else
	{
		LegacyBssExist = FALSE;
	}
	
	// decide ErpIR.UseProtection bit, depending on pAd->CommonCfg.UseBGProtection
	//    AUTO (0): UseProtection = 1 if any 11b STA associated
	//    ON (1): always USE protection
	//    OFF (2): always NOT USE protection
	if (pAd->CommonCfg.UseBGProtection == 0)
	{
		ErpIeContent = (ErpIeContent)? 0x03 : 0x00;
		//if ((pAd->ApCfg.LastOLBCDetectTime + (5 * OS_HZ)) > pAd->Mlme.Now32) // legacy BSS exist within 5 sec
		if (LegacyBssExist)
		{
			ErpIeContent |= 0x02;                                     // set Use_Protection bit
		}
	}
	else if (pAd->CommonCfg.UseBGProtection == 1)   
		ErpIeContent |= 0x02;
	else
		;

	bUseBGProtection = (pAd->CommonCfg.UseBGProtection == 1) ||    // always use
						((pAd->CommonCfg.UseBGProtection == 0) && ERP_IS_USE_PROTECTION(ErpIeContent));
#ifdef A_BAND_SUPPORT
	// always no BG protection in A-band. falsely happened when switching A/G band to a dual-band AP
	if (pAd->CommonCfg.Channel > 14) 
		bUseBGProtection = FALSE;
#endif // A_BAND_SUPPORT //

	if (bUseBGProtection != OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_BG_PROTECTION_INUSED))
	{
		USHORT OperationMode = 0;
		BOOLEAN	bNonGFExist = 0;

#ifdef DOT11_N_SUPPORT
		OperationMode = pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode;
		bNonGFExist = pAd->MacTab.fAnyStationNonGF;
#endif // DOT11_N_SUPPORT //
		if (bUseBGProtection)
		{
			OPSTATUS_SET_FLAG(pAd, fOP_STATUS_BG_PROTECTION_INUSED);
			AsicUpdateProtect(pAd, OperationMode, (OFDMSETPROTECT), FALSE, bNonGFExist);
		}
		else
		{
			OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_BG_PROTECTION_INUSED);
			AsicUpdateProtect(pAd, OperationMode, (OFDMSETPROTECT), TRUE, bNonGFExist);
		}
	}

	// Decide Barker Preamble bit of ERP IE
	if ((pAd->CommonCfg.TxPreamble == Rt802_11PreambleLong) || (ApCheckLongPreambleSTA(pAd) == TRUE))
		pAd->ApCfg.ErpIeContent = (ErpIeContent | 0x04);
	else
		pAd->ApCfg.ErpIeContent = ErpIeContent;

#ifdef A_BAND_SUPPORT
	// Force to use ShortSlotTime at A-band
	if (pAd->CommonCfg.Channel > 14)
		ShortSlotCapable = TRUE;
#endif // A_BAND_SUPPORT //
	
	//
	// deicide CapabilityInfo.ShortSlotTime bit
	//
    for (apidx=0; apidx<pAd->ApCfg.BssidNum; apidx++)
    {
		USHORT *pCapInfo = &(pAd->ApCfg.MBSSID[apidx].CapabilityInfo);

		// In A-band, the ShortSlotTime bit should be ignored. 
		if (ShortSlotCapable && (pAd->CommonCfg.Channel <= 14))
    		(*pCapInfo) |= 0x0400;
#ifdef A_BAND_SUPPORT		
		else
    		(*pCapInfo) &= 0xfbff;
#endif // A_BAND_SUPPORT //		


   		if (pAd->CommonCfg.TxPreamble == Rt802_11PreambleLong)
			(*pCapInfo) &= (~0x020);
		else
			(*pCapInfo) |= 0x020;

	}

	AsicSetSlotTime(pAd, ShortSlotCapable);

}

/*
	==========================================================================
	Description:
        Check to see the exist of long preamble STA in associated list
    ==========================================================================
 */
BOOLEAN ApCheckLongPreambleSTA(
    IN PRTMP_ADAPTER pAd)
{
    UCHAR   i;
    
    for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
    {
		PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];
		if (!IS_ENTRY_CLIENT(pEntry) || (pEntry->Sst != SST_ASSOC))
			continue;
	            
        if (!CAP_IS_SHORT_PREAMBLE_ON(pEntry->CapabilityInfo))
        {
            return TRUE;
        }
    }

    return FALSE;
}    

/*
	==========================================================================
	Description:
		Check if the specified STA pass the Access Control List checking.
		If fails to pass the checking, then no authentication nor association 
		is allowed
	Return:
		MLME_SUCCESS - this STA passes ACL checking

	==========================================================================
*/
BOOLEAN ApCheckAccessControlList(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR        pAddr,
	IN UCHAR         Apidx)
{
	BOOLEAN Result = TRUE;

    if (pAd->ApCfg.MBSSID[Apidx].AccessControlList.Policy == 0)       // ACL is disabled
        Result = TRUE;
    else
    {
        ULONG i;
        if (pAd->ApCfg.MBSSID[Apidx].AccessControlList.Policy == 1)   // ACL is a positive list
            Result = FALSE;
        else                                              // ACL is a negative list
            Result = TRUE;
        for (i=0; i<pAd->ApCfg.MBSSID[Apidx].AccessControlList.Num; i++)
        {
            if (MAC_ADDR_EQUAL(pAddr, pAd->ApCfg.MBSSID[Apidx].AccessControlList.Entry[i].Addr))
            {
                Result = !Result;
                break;
            }
        }
    }

    if (Result == FALSE)
    {
        DBGPRINT(RT_DEBUG_TRACE, ("%02x:%02x:%02x:%02x:%02x:%02x failed in ACL checking\n",
        pAddr[0],pAddr[1],pAddr[2],pAddr[3],pAddr[4],pAddr[5]));
    }

    return Result;
}

/*
	==========================================================================
	Description:
		This routine update the current MAC table based on the current ACL.
		If ACL change causing an associated STA become un-authorized. This STA
		will be kicked out immediately.
	==========================================================================
*/
VOID ApUpdateAccessControlList(
    IN PRTMP_ADAPTER pAd,
    IN UCHAR         Apidx)
{
	USHORT   AclIdx, MacIdx;
	BOOLEAN  Matched;

	PUCHAR      pOutBuffer = NULL;
	NDIS_STATUS NStatus;
	ULONG       FrameLen = 0;
	HEADER_802_11 DisassocHdr;
	USHORT      Reason;

	
	//Apidx = pObj->ioctl_if;
	ASSERT(Apidx <= MAX_MBSSID_NUM);
	DBGPRINT(RT_DEBUG_TRACE, ("ApUpdateAccessControlList : Apidx = %d\n", Apidx));
	
    // ACL is disabled. Do nothing about the MAC table.
    if (pAd->ApCfg.MBSSID[Apidx].AccessControlList.Policy == 0)
		return;

	for (MacIdx=0; MacIdx < MAX_LEN_OF_MAC_TABLE; MacIdx++)
	{
		if (!IS_ENTRY_CLIENT(&pAd->MacTab.Content[MacIdx])) 
			continue;

		//
		// We only need to update associations related to ACL of MBSSID[Apidx].
		//
        if (pAd->MacTab.Content[MacIdx].apidx != Apidx) 
            continue;
    
		Matched = FALSE;
        for (AclIdx = 0; AclIdx < pAd->ApCfg.MBSSID[Apidx].AccessControlList.Num; AclIdx++)
		{
            if (MAC_ADDR_EQUAL(&pAd->MacTab.Content[MacIdx].Addr, pAd->ApCfg.MBSSID[Apidx].AccessControlList.Entry[AclIdx].Addr))
			{
				Matched = TRUE;
				break;
			}
		}

        if ((Matched == FALSE) && (pAd->ApCfg.MBSSID[Apidx].AccessControlList.Policy == 1))
		{
			DBGPRINT(RT_DEBUG_TRACE, ("Apidx = %d\n", Apidx));
			DBGPRINT(RT_DEBUG_TRACE, ("pAd->ApCfg.MBSSID[%d].AccessControlList.Policy = %ld\n", Apidx,
				pAd->ApCfg.MBSSID[Apidx].AccessControlList.Policy));
			DBGPRINT(RT_DEBUG_TRACE, ("STA not on positive ACL. remove it...\n"));
			
			// Before delete the entry from MacTable, send disassociation packet to client.
			if (pAd->MacTab.Content[MacIdx].Sst == SST_ASSOC)
			{
				//  send out a DISASSOC frame
				NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);
				if (NStatus != NDIS_STATUS_SUCCESS) 
				{
					DBGPRINT(RT_DEBUG_TRACE, (" MlmeAllocateMemory fail  ..\n"));
					return;
				}

				Reason = REASON_DECLINED;
				DBGPRINT(RT_DEBUG_ERROR, ("ASSOC - Send DISASSOC  Reason = %d frame  TO %x %x %x %x %x %x \n",Reason,pAd->MacTab.Content[MacIdx].Addr[0],
					pAd->MacTab.Content[MacIdx].Addr[1],pAd->MacTab.Content[MacIdx].Addr[2],pAd->MacTab.Content[MacIdx].Addr[3],pAd->MacTab.Content[MacIdx].Addr[4],pAd->MacTab.Content[MacIdx].Addr[5]));
				MgtMacHeaderInit(pAd, &DisassocHdr, SUBTYPE_DISASSOC, 0, pAd->MacTab.Content[MacIdx].Addr, pAd->ApCfg.MBSSID[pAd->MacTab.Content[MacIdx].apidx].Bssid);
				MakeOutgoingFrame(pOutBuffer, &FrameLen, sizeof(HEADER_802_11), &DisassocHdr, 2, &Reason, END_OF_ARGS);
				MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
				MlmeFreeMemory(pAd, pOutBuffer);

				RTMPusecDelay(5000);
			}
			MacTableDeleteEntry(pAd, pAd->MacTab.Content[MacIdx].Aid, pAd->MacTab.Content[MacIdx].Addr);
		}
        else if ((Matched == TRUE) && (pAd->ApCfg.MBSSID[Apidx].AccessControlList.Policy == 2))
		{
			DBGPRINT(RT_DEBUG_TRACE, ("Apidx = %d\n", Apidx));
			DBGPRINT(RT_DEBUG_TRACE, ("pAd->ApCfg.MBSSID[%d].AccessControlList.Policy = %ld\n", Apidx,
				pAd->ApCfg.MBSSID[Apidx].AccessControlList.Policy));
			DBGPRINT(RT_DEBUG_TRACE, ("STA on negative ACL. remove it...\n"));
			
			// Before delete the entry from MacTable, send disassociation packet to client.
			if (pAd->MacTab.Content[MacIdx].Sst == SST_ASSOC)
			{
				// send out a DISASSOC frame
				NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);
				if (NStatus != NDIS_STATUS_SUCCESS) 
				{
					DBGPRINT(RT_DEBUG_TRACE, (" MlmeAllocateMemory fail  ..\n"));
					return;
				}

				Reason = REASON_DECLINED;
				DBGPRINT(RT_DEBUG_ERROR, ("ASSOC - Send DISASSOC  Reason = %d frame  TO %x %x %x %x %x %x \n",Reason,pAd->MacTab.Content[MacIdx].Addr[0],
					pAd->MacTab.Content[MacIdx].Addr[1],pAd->MacTab.Content[MacIdx].Addr[2],pAd->MacTab.Content[MacIdx].Addr[3],pAd->MacTab.Content[MacIdx].Addr[4],pAd->MacTab.Content[MacIdx].Addr[5]));
				MgtMacHeaderInit(pAd, &DisassocHdr, SUBTYPE_DISASSOC, 0, pAd->MacTab.Content[MacIdx].Addr, pAd->ApCfg.MBSSID[pAd->MacTab.Content[MacIdx].apidx].Bssid);
				MakeOutgoingFrame(pOutBuffer, &FrameLen, sizeof(HEADER_802_11), &DisassocHdr, 2, &Reason, END_OF_ARGS);
				MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
				MlmeFreeMemory(pAd, pOutBuffer);

				RTMPusecDelay(5000);
			}
			MacTableDeleteEntry(pAd, pAd->MacTab.Content[MacIdx].Aid, pAd->MacTab.Content[MacIdx].Addr);
		}
	}
}

/* 
	==========================================================================
	Description:
		Send out a NULL frame to a specified STA at a higher TX rate. The 
		purpose is to ensure the designated client is okay to received at this
		rate.
	==========================================================================
 */
VOID ApEnqueueNullFrame(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR        pAddr,
	IN UCHAR         TxRate,
	IN UCHAR         PID,
	IN UCHAR         apidx,
    IN BOOLEAN       bQosNull,
    IN BOOLEAN       bEOSP,
    IN UCHAR         OldUP)
{
	NDIS_STATUS    NState;
	PHEADER_802_11 pNullFr;
	PUCHAR pFrame;
    ULONG		   Length;


	// since TxRate may change, we have to change Duration each time
	NState = MlmeAllocateMemory(pAd, (PUCHAR *)&pFrame);
	pNullFr = (PHEADER_802_11) pFrame;
    Length = sizeof(HEADER_802_11);

	if (NState == NDIS_STATUS_SUCCESS) 
	{
//		if ((PID & 0x3f) < WDS_PAIRWISE_KEY_OFFSET) // send to client
		{
			MgtMacHeaderInit(pAd, pNullFr, SUBTYPE_NULL_FUNC, 0, pAddr, pAd->ApCfg.MBSSID[apidx].Bssid);
			pNullFr->FC.Type = BTYPE_DATA;
			pNullFr->FC.FrDs = 1;
			pNullFr->Duration = RTMPCalcDuration(pAd, TxRate, 14);

#ifdef UAPSD_AP_SUPPORT
            if (bQosNull)
			{
                UCHAR *qos_p = ((UCHAR *)pNullFr) + Length;

				pNullFr->FC.SubType = SUBTYPE_QOS_NULL;

				/* copy QOS control bytes */
				qos_p[0] = ((bEOSP) ? (1 << 4) : 0) | OldUP;
				qos_p[1] = 0;
				Length += 2;
			} /* End of if */
#endif // UAPSD_AP_SUPPORT //

			DBGPRINT(RT_DEBUG_INFO, ("send NULL Frame @%d Mbps to AID#%d...\n", RateIdToMbps[TxRate], PID & 0x3f));
            MiniportMMRequest(pAd, MapUserPriorityToAccessCategory[7], (PUCHAR)pNullFr, Length);
		}
//#ifdef  WDS
	MlmeFreeMemory(pAd, pFrame);
	}
}

VOID    ApSendFrame(
	IN  PRTMP_ADAPTER   pAd,
	IN  PVOID           pBuffer,
	IN  ULONG           Length,
	IN  UCHAR           TxRate,
	IN  UCHAR           PID)
{
}

/* 
	==========================================================================
	Description:
		Send out a ACK frame to a specified STA upon receiving PS-POLL
	==========================================================================
 */
VOID ApEnqueueAckFrame(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR        pAddr,
	IN UCHAR         TxRate,
	IN UCHAR		 apidx) 
{
	NDIS_STATUS    NState;
	PHEADER_802_11  pAckFr;
	PUCHAR			pFrame;

	// since TxRate may change, we have to change Duration each time
	NState = MlmeAllocateMemory(pAd, &pFrame);
	pAckFr = (PHEADER_802_11) pFrame;
	if (NState == NDIS_STATUS_SUCCESS) 
	{
		MgtMacHeaderInit(pAd, pAckFr, SUBTYPE_ACK, 0, pAddr, pAd->ApCfg.MBSSID[apidx].Bssid);
		pAckFr->FC.Type = BTYPE_CNTL;
		MiniportMMRequest(pAd, 0, (PUCHAR)pAckFr, 10);
		MlmeFreeMemory(pAd, pFrame);
	}
}

VOID APSwitchChannel(
	IN PRTMP_ADAPTER pAd,
	IN INT Channel)
{
	INT CentralChannel = Channel;
#ifdef DOT11_N_SUPPORT
	UCHAR byteValue = 0;
	UINT32 Value;
	
	if ((pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth  == BW_40) && (pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset == EXTCHA_ABOVE))
	{
		CentralChannel = Channel + 2;
		//  TX : control channel at lower 
		RTMP_IO_READ32(pAd, TX_BAND_CFG, &Value);
		Value &= (~0x1);
		RTMP_IO_WRITE32(pAd, TX_BAND_CFG, Value);

		//  RX : control channel at lower 
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, 3, &byteValue);
		byteValue &= (~0x20);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 3, byteValue);

	}
	else if ((Channel > 2) && (pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth  == BW_40) && (pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset == EXTCHA_BELOW))
	{
		CentralChannel = Channel - 2;
		//  TX : control channel at upper 
		RTMP_IO_READ32(pAd, TX_BAND_CFG, &Value);
		Value |= (0x1);		
		RTMP_IO_WRITE32(pAd, TX_BAND_CFG, Value);

		//  RX : control channel at upper 
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, 3, &byteValue);
		byteValue |= (0x20);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, 3, byteValue);
	}
#endif // DOT11_N_SUPPORT //

	AsicSwitchChannel(pAd, CentralChannel, TRUE);

}

#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
/*
	Depends on the 802.11n Draft 4.0, Before the HT AP start a BSS, it should scan some specific channels to
collect information of existing BSSs, then depens on the collected channel information, adjust the primary channel 
and secondary channel setting.

	For 5GHz,
		Rule 1: If the AP chooses to start a 20/40 MHz BSS in 5GHz and that occupies the same two channels
				as any existing 20/40 MHz BSSs, then the AP shall ensure that the primary channel of the 
				new BSS is identical to the primary channel of the existing 20/40 MHz BSSs and that the 
				secondary channel of the new 20/40 MHz BSS is identical to the secondary channel of the 
				existing 20/40 MHz BSSs, unless the AP discoverr that on those two channels are existing
				20/40 MHz BSSs with different primary and secondary channels.
		Rule 2: If the AP chooses to start a 20/40MHz BSS in 5GHz, the selected secondary channel should
				correspond to a channel on which no beacons are detected during the overlapping BSS
				scan time performed by the AP, unless there are beacons detected on both the selected
				primary and secondary channels.
		Rule 3: An HT AP should not start a 20 MHz BSS in 5GHz on a channel that is the secondary channel 
				of a 20/40 MHz BSS.
	For 2.4GHz,
		Rule 1: The AP shall not start a 20/40 MHz BSS in 2.4GHz if the value of the local variable "20/40
				Operation Permitted" is FALSE.

		20/40OperationPermitted =  (P == OPi for all values of i) AND
								(P == OTi for all values of i) AND
								(S == OSi for all values if i)
		where
			P 	is the operating or intended primary channel of the 20/40 MHz BSS
			S	is the operating or intended secondary channel of the 20/40 MHz BSS
			OPi  is member i of the set of channels that are members of the channel set C and that are the
				primary operating channel of at least one 20/40 MHz BSS that is detected within the AP's 
				BSA during the previous X seconds
			OSi  is member i of the set of channels that are members of the channel set C and that are the
				secondary operating channel of at least one 20/40 MHz BSS that is detected within AP's
				BSA during the previous X seconds
			OTi  is member i of the set of channels that comparises all channels that are members of the 
				channel set C that were listed once in the Channel List fields of 20/40 BSS Intolerant Channel
				Report elements receved during the previous X seconds and all channels that are members
				of the channel set C and that are the primary operating channel of at least one 20/40 MHz
				BSS that were detected within the AP's BSA during the previous X seconds.
			C	is the set of all channels that are allowed operating channels within the current operational
				regulatory domain and whose center frequency falls within the 40 MHz affected channel 
				range given by following equation:
					                                                 Fp + Fs                  Fp + Fs
					40MHz affected channel range = [ ------  - 25MHz,  ------- + 25MHz ]
					                                                      2                          2
					Where 
						Fp = the center frequency of channel P
						Fs = the center frequency of channel S

			"==" means that the values on either side of the "==" are to be tested for equaliy with a resulting 
				 Boolean value.
			        =>When the value of OPi is the empty set, then the expression (P == OPi for all values of i) 
			        	is defined to be TRUE
			        =>When the value of OTi is the empty set, then the expression (P == OTi for all values of i) 
			        	is defined to be TRUE
			        =>When the value of OSi is the empty set, then the expression (S == OSi for all values of i) 
			        	is defined to be TRUE
*/

INT GetBssCoexEffectedChRange(
	IN RTMP_ADAPTER *pAd,
	IN BSS_COEX_CH_RANGE *pCoexChRange)
{
	INT index, cntrCh = 0;

	memset(pCoexChRange, 0, sizeof(BSS_COEX_CH_RANGE));
	
	// Build the effected channel list, if something wrong, return directly.	
	if (pAd->CommonCfg.Channel <= 14)
	{	// For 2.4GHz band 
		for (index = 0; index < pAd->ChannelListNum; index++)
		{
			if(pAd->ChannelList[index].Channel == pAd->CommonCfg.Channel)
				break;
		}

		if (index < pAd->ChannelListNum)
		{
			/* First get the primary channel */
			pCoexChRange->primaryCh = index;
			
			/* Now check about the secondary and central channel */
			if(pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_ABOVE)
			{
				if ((index + 4) < pAd->ChannelListNum)
				{
					cntrCh = index + 2;
					pCoexChRange->secondaryCh = index + 4;
				}
			}
			else
			{
				if ((index - 4) >=0)
				{
					cntrCh = index - 2;
					pCoexChRange->secondaryCh = index - 4;
				}
			}

			if (cntrCh)
			{
				pCoexChRange->effectChStart = (cntrCh - 5) > 0 ? (cntrCh - 5) : 0;
				pCoexChRange->effectChEnd= (cntrCh + 5) > 0 ? (cntrCh + 5) : 0;
				DBGPRINT(RT_DEBUG_TRACE,("2.4GHz: Found CtrlCh idx(%d) from the ChList, ExtCh=%s, PrimaryCh=[Idx:%d, CH:%d], SecondaryCh=[Idx:%d, CH:%d], effected Ch=[CH:%d~CH:%d]!\n", 
										index, 
										((pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_ABOVE) ? "ABOVE" : "BELOW"), 
										pCoexChRange->primaryCh, pAd->ChannelList[pCoexChRange->primaryCh].Channel, 
										pCoexChRange->secondaryCh, pAd->ChannelList[pCoexChRange->secondaryCh].Channel,
										pAd->ChannelList[pCoexChRange->effectChStart].Channel,
										pAd->ChannelList[pCoexChRange->effectChEnd].Channel));
			}
			return TRUE;
		}

		// It should not happened!
		DBGPRINT(RT_DEBUG_ERROR, ("2.4GHz: Didn't found valid channel range, Ch index=%d, ChListNum=%d, CtrlCh=%d\n", 
									index, pAd->ChannelListNum, pAd->CommonCfg.Channel));
	}
	else
	{	// For 5GHz band
		for (index = 0; index < pAd->ChannelListNum; index++)
		{
			if(pAd->ChannelList[index].Channel == pAd->CommonCfg.Channel)
				break;
		}

		if (index < pAd->ChannelListNum)
		{
			/* First get the primary channel */
			pCoexChRange->primaryCh = pAd->ChannelList[index].Channel;
			
			/* Now check about the secondary and central channel */
			if(pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_ABOVE)
			{
				pCoexChRange->effectChStart = pCoexChRange->primaryCh;
				pCoexChRange->effectChEnd = pCoexChRange->primaryCh + 4;
				pCoexChRange->secondaryCh = pCoexChRange->effectChEnd;
			}
			else
			{
				pCoexChRange->effectChStart = pCoexChRange->primaryCh -4;
				pCoexChRange->effectChEnd = pCoexChRange->primaryCh;
				pCoexChRange->secondaryCh = pCoexChRange->effectChStart;
			}

			DBGPRINT(RT_DEBUG_TRACE,("5.0GHz: Found CtrlCh idx(%d) from the ChList, ExtCh=%s, PriCh=[Idx:%d, CH:%d], SecCh=[Idx:%d, CH:%d], effected Ch=[CH:%d~CH:%d]!\n", 
										index, 
										((pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_ABOVE) ? "ABOVE" : "BELOW"), 
										pCoexChRange->primaryCh, pAd->ChannelList[pCoexChRange->primaryCh].Channel, 
										pCoexChRange->secondaryCh, pAd->ChannelList[pCoexChRange->secondaryCh].Channel,
										pAd->ChannelList[pCoexChRange->effectChStart].Channel,
										pAd->ChannelList[pCoexChRange->effectChEnd].Channel));
			return TRUE;
		}
		else
		{
			// It should not happened!
			DBGPRINT(RT_DEBUG_ERROR, ("5GHz: Cannot found the CtrlCh(%d) in ChList, something wrong?\n", 
						pAd->CommonCfg.Channel));
		}
	}

	return FALSE;
}

VOID APOverlappingBSSScan(
	IN RTMP_ADAPTER *pAd)
{
	BOOLEAN		needFallBack = FALSE;
	UCHAR 		Channel = pAd->CommonCfg.Channel;
	UINT8		BBPValue = 0;
	INT		chStartIdx, chEndIdx, index,curPriChIdx, curSecChIdx;


	// We just care BSS who operating in 40MHz N Mode.
	if ((pAd->CommonCfg.PhyMode < PHY_11ABGN_MIXED) || 
		(pAd->CommonCfg.RegTransmitSetting.field.BW  == BW_20))
	{
		DBGPRINT(RT_DEBUG_TRACE, ("The pAd->PhyMode=%d, BW=%d, didn't need channel adjustment!\n", 
				pAd->CommonCfg.PhyMode, pAd->CommonCfg.RegTransmitSetting.field.BW));
		return;
	}


	// Build the effected channel list, if something wrong, return directly.	
	if (pAd->CommonCfg.Channel <= 14)
	{	// For 2.4GHz band 
		for (index = 0; index < pAd->ChannelListNum; index++)
		{
			if(pAd->ChannelList[index].Channel == pAd->CommonCfg.Channel)
				break;
		}

		if (index < pAd->ChannelListNum)
		{

			if(pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_ABOVE)
			{
				curPriChIdx = index;
				curSecChIdx = ((index + 4) < pAd->ChannelListNum) ? (index + 4) : (pAd->ChannelListNum - 1);
				
				chStartIdx = (curPriChIdx >= 3) ? (curPriChIdx - 3) : 0;
				chEndIdx = ((curSecChIdx + 3) < pAd->ChannelListNum) ? (curSecChIdx + 3) : (pAd->ChannelListNum - 1);
			}
			else
			{
				curPriChIdx = index;
				curSecChIdx = ((index - 4) >=0 ) ? (index - 4) : 0;
				chStartIdx =(curSecChIdx >= 3) ? (curSecChIdx - 3) : 0;
				chEndIdx =  ((curPriChIdx + 3) < pAd->ChannelListNum) ? (curPriChIdx + 3) : (pAd->ChannelListNum - 1);;
			}
		}
		else
		{
			// It should not happened!
			DBGPRINT(RT_DEBUG_ERROR, ("2.4GHz: Cannot found the Control Channel(%d) in ChannelList, something wrong?\n", 
						pAd->CommonCfg.Channel));
			return;
		}
	}
#ifdef A_BAND_SUPPORT	
	else
	{	// For 5GHz band
		for (index = 0; index < pAd->ChannelListNum; index++)
		{
			if(pAd->ChannelList[index].Channel == pAd->CommonCfg.Channel)
				break;
		}

		if (index < pAd->ChannelListNum)
		{
			curPriChIdx = index;
			if(pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_ABOVE)
			{
				chStartIdx = index;
				chEndIdx = chStartIdx + 1;
				curSecChIdx = chEndIdx;
			}
			else
			{
				chStartIdx = index - 1;
				chEndIdx = index;
				curSecChIdx = chStartIdx;
			}
		}
		else
		{
			// It should not happened!
			DBGPRINT(RT_DEBUG_ERROR, ("5GHz: Cannot found the ControlChannel(%d) in ChannelList, something wrong?\n", 
						pAd->CommonCfg.Channel));
			return;
		}
	}
#endif // A_BAND_SUPPORT //
	{
		BSS_COEX_CH_RANGE  coexChRange;
		GetBssCoexEffectedChRange(pAd, &coexChRange);
	}	

	// Before we do the scanning, clear the bEffectedChannel as zero for latter use.
	for (index = 0; index < pAd->ChannelListNum; index++)
		pAd->ChannelList[index].bEffectedChannel = 0;
	
		
	// If we are not ready for Tx/Rx Pakcet, enable it now for receiving Beacons.
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_START_UP) == 0)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Card still not enable Tx/Rx, enable it now!\n"));
#ifdef RTMP_MAC_PCI
		// Enable Interrupt
		pAd->int_enable_reg = ((DELAYINTMASK)  | (RxINT|TxDataInt|TxMgmtInt)) & ~(0x03);
		pAd->int_disable_mask = 0;
		pAd->int_pending = 0;
	
		RTMP_IO_WRITE32(pAd, INT_SOURCE_CSR, 0xffffffff);  // clear garbage interrupts
		RTMP_ASIC_INTERRUPT_ENABLE(pAd);
#endif // RTMP_MAC_PCI //


		// Now Enable RxTx
		RTMPEnableRxTx(pAd);

		// APRxDoneInterruptHandle API will check this flag to decide accept incoming packet or not.
		// Set the flag be ready to receive Beacon frame for autochannel select.
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_START_UP);
	}


	DBGPRINT(RT_DEBUG_TRACE, ("Ready to do passive scanning for Channel[%d] to Channel[%d]!\n", 
			pAd->ChannelList[chStartIdx].Channel, pAd->ChannelList[chEndIdx].Channel));
	
	// Now start to do the passive scanning.
	pAd->CommonCfg.bOverlapScanning = TRUE;
	for (index = chStartIdx; index<=chEndIdx; index++)
	{
		Channel = pAd->ChannelList[index].Channel;

		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BBPValue);
		BBPValue &= (~0x20);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BBPValue);
		
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &BBPValue);
		BBPValue &= (~0x18);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, BBPValue);
		
		// Let BBP register at 20MHz to do scan	
		AsicSwitchChannel(pAd, Channel, TRUE);
		AsicLockChannel(pAd, Channel);

		DBGPRINT(RT_DEBUG_ERROR, ("SYNC - BBP R4 to 20MHz.l\n"));
		//DBGPRINT(RT_DEBUG_TRACE, ("Passive scanning for Channel %d.....\n", Channel));
		OS_WAIT(300); // wait for 200 ms at each channel.
	}
	pAd->CommonCfg.bOverlapScanning = FALSE;	
	

	// After scan all relate channels, now check the scan result to find out if we need fallback to 20MHz.
	for (index = chStartIdx; index <= chEndIdx; index++)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Channel[Idx=%d, Ch=%d].bEffectedChannel=0x%x!\n", 
					index, pAd->ChannelList[index].Channel, pAd->ChannelList[index].bEffectedChannel));
		if ((pAd->ChannelList[index].bEffectedChannel & 0x6)  && (index != curPriChIdx))
		{
			needFallBack = TRUE;
			DBGPRINT(RT_DEBUG_TRACE, ("needFallBack=TRUE due to OP/OT!\n"));
		}
		if ((pAd->ChannelList[index].bEffectedChannel & 0x1)  && (index != curSecChIdx))
		{
			needFallBack = TRUE;
			DBGPRINT(RT_DEBUG_TRACE, ("needFallBack=TRUE due to OS!\n"));
		}
	}
	
	// If need fallback, now do it.
	pAd->CommonCfg.Bss2040NeedFallBack = 0;
	if (needFallBack)
	{
		pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth = 0;
		pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset = 0;
		pAd->CommonCfg.LastBSSCoexist2040.field.BSS20WidthReq = 1;
		pAd->CommonCfg.Bss2040CoexistFlag |= BSS_2040_COEXIST_INFO_SYNC;
		pAd->CommonCfg.Bss2040NeedFallBack = 1;
	}

	return;	
}
#endif // DOT11N_DRAFT3 //

VOID EnableAPMIMOPS(
	IN PRTMP_ADAPTER pAd)
{
	UCHAR	BBPR3=0,BBPR1=0,RFValue=0;
	ULONG	TxPinCfg = 0x00050F0A;//Gary 2007/08/09 0x050A0A
	UINT32 	macdata=0;
#ifdef RT30xx
	// enable MMPS BBP control register
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BBPR3);
		BBPR3 |= 0x04;	//bit 2
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BBPR3);

		// enable MMPS MAC control register
		RTMP_IO_READ32(pAd, 0x1210, &macdata);
		macdata |= 0x09;	//bit 0, 3
		RTMP_IO_WRITE32(pAd, 0x1210, macdata);
		
		DBGPRINT(RT_DEBUG_INFO, ("EnableAPMIMOPS, 30xx changes the # of antenna to 1\n"));
#else


		
		pAd->CommonCfg.bBlockAntDivforGreenAP=TRUE;
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BBPR3);
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BBPR1);
		//RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R138, &BBPR138);
		
		BBPR3 &= (~0x18);
		BBPR1 &= (~0x18);
		RFValue &= 0x03;	//clear bit[7~2]
		RFValue |= 0xF0;
		// Turn off unused PA or LNA when only 1T or 1R
		TxPinCfg &= 0xFFFFFFF3;
		TxPinCfg &= 0xFFFFF3FF;
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BBPR3);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BBPR1);

		RTMP_IO_WRITE32(pAd, TX_PIN_CFG, TxPinCfg);
		DBGPRINT(RT_DEBUG_INFO, ("EnableAPMIMOPS, 305x/28xx changes the # of antenna to 1\n"));

#endif // RT30xx //
}

VOID DisableAPMIMOPS(
	IN PRTMP_ADAPTER pAd)
{
		UCHAR	BBPR3=0,BBPR1=0,RFValue=0;
		ULONG	TxPinCfg = 0x00050F0A;//Gary 2007/08/09 0x050A0A
		UINT32 	macdata=0;
#ifdef RT30xx
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BBPR3);
		BBPR3 &= ~(0x04);	//bit 2
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BBPR3);

		// enable MMPS MAC control register
		RTMP_IO_READ32(pAd, 0x1210, &macdata);
		macdata &= ~(0x09);	//bit 0, 3
		RTMP_IO_WRITE32(pAd, 0x1210, macdata);
		
				DBGPRINT(RT_DEBUG_INFO, ("DisableAPMIMOPS, 30xx reserve only one antenna\n"));
#else


		pAd->CommonCfg.bBlockAntDivforGreenAP=FALSE;
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BBPR3);
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BBPR1);
		//RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R138, &BBPR138);
		BBPR3 &= (~0x18);
		if(pAd->Antenna.field.RxPath == 3)
		{
			BBPR3 |= (0x10);
		}
		else if(pAd->Antenna.field.RxPath == 2)
		{
			BBPR3 |= (0x8);
		}
		else if(pAd->Antenna.field.RxPath == 1)
		{
			BBPR3 |= (0x0);
		}

		if ((pAd->CommonCfg.PhyMode >= PHY_11ABGN_MIXED) && (pAd->Antenna.field.TxPath == 2))
		{
			BBPR1 &= (~0x18);
			BBPR1 |= 0x10;
		}
		else
		{
			BBPR1 &= (~0x18);
		}

		
		RFValue &= 0x03;	//clear bit[7~2]
		if (pAd->Antenna.field.TxPath == 1)
			RFValue |= 0xA0;
		else if (pAd->Antenna.field.TxPath == 2)
			RFValue |= 0x80;
		if (pAd->Antenna.field.RxPath == 1)
			RFValue |= 0x50;
		else if (pAd->Antenna.field.RxPath == 2)
			RFValue |= 0x40;
		// Turn off unused PA or LNA when only 1T or 1R
		if (pAd->Antenna.field.TxPath == 1)
		{
			TxPinCfg &= 0xFFFFFFF3;
		}
		if (pAd->Antenna.field.RxPath == 1)
		{
			TxPinCfg &= 0xFFFFF3FF;
		}
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BBPR3);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BBPR1);
		RTMP_IO_WRITE32(pAd, TX_PIN_CFG, TxPinCfg);
		DBGPRINT(RT_DEBUG_INFO, ("DisableAPMIMOPS, 305x/28xx reserve only one antenna\n"));

#endif // RT30xx //
}
#endif // DOT11_N_SUPPORT //

