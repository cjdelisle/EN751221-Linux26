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
	ap_cfg.c

    Abstract:
    IOCTL related subroutines

    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
*/


#include "rt_config.h"

#ifdef RLM_CAL_CACHE_SUPPORT
#include "phy/rlm_cal_cache.h"
#endif /* RLM_CAL_CACHE_SUPPORT */

#define A_BAND_REGION_0				0
#define A_BAND_REGION_1				1
#define A_BAND_REGION_2				2
#define A_BAND_REGION_3				3
#define A_BAND_REGION_4				4
#define A_BAND_REGION_5				5
#define A_BAND_REGION_6				6
#define A_BAND_REGION_7				7
#define A_BAND_REGION_8				8
#define A_BAND_REGION_9				9
#define A_BAND_REGION_10			10

#define G_BAND_REGION_0				0
#define G_BAND_REGION_1				1
#define G_BAND_REGION_2				2
#define G_BAND_REGION_3				3
#define G_BAND_REGION_4				4
#define G_BAND_REGION_5				5
#define G_BAND_REGION_6				6

#ifdef MULTI_INF_SUPPORT
/* Index 0 for 2.4G, 1 for 5Ghz Card */
extern VOID* adapt_list[MAX_NUM_OF_INF];
#endif /* MULTI_INF_SUPPORT */

COUNTRY_CODE_TO_COUNTRY_REGION allCountry[] = {
	/* {Country Number, ISO Name, Country Name, Support 11A, 11A Country Region, Support 11G, 11G Country Region} */
	{0,		"DB",	"Debug",				TRUE,	A_BAND_REGION_7,	TRUE,	G_BAND_REGION_5},
	{8,		"AL",	"ALBANIA",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{12,	"DZ",	"ALGERIA",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{32,	"AR",	"ARGENTINA",			TRUE,	A_BAND_REGION_3,	TRUE,	G_BAND_REGION_1},
	{51,	"AM",	"ARMENIA",				TRUE,	A_BAND_REGION_2,	TRUE,	G_BAND_REGION_1},
	{36,	"AU",	"AUSTRALIA",			TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{40,	"AT",	"AUSTRIA",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{31,	"AZ",	"AZERBAIJAN",			TRUE,	A_BAND_REGION_2,	TRUE,	G_BAND_REGION_1},
	{48,	"BH",	"BAHRAIN",				TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{112,	"BY",	"BELARUS",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{56,	"BE",	"BELGIUM",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{84,	"BZ",	"BELIZE",				TRUE,	A_BAND_REGION_4,	TRUE,	G_BAND_REGION_1},
	{68,	"BO",	"BOLIVIA",				TRUE,	A_BAND_REGION_4,	TRUE,	G_BAND_REGION_1},
	{76,	"BR",	"BRAZIL",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{96,	"BN",	"BRUNEI DARUSSALAM",	TRUE,	A_BAND_REGION_4,	TRUE,	G_BAND_REGION_1},
	{100,	"BG",	"BULGARIA",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{124,	"CA",	"CANADA",				TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_0},
	{152,	"CL",	"CHILE",				TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{156,	"CN",	"CHINA",				TRUE,	A_BAND_REGION_4,	TRUE,	G_BAND_REGION_1},
	{170,	"CO",	"COLOMBIA",				TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_0},
	{188,	"CR",	"COSTA RICA",			FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{191,	"HR",	"CROATIA",				TRUE,	A_BAND_REGION_2,	TRUE,	G_BAND_REGION_1},
	{196,	"CY",	"CYPRUS",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{203,	"CZ",	"CZECH REPUBLIC",		TRUE,	A_BAND_REGION_2,	TRUE,	G_BAND_REGION_1},
	{208,	"DK",	"DENMARK",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{214,	"DO",	"DOMINICAN REPUBLIC",	TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_0},
	{218,	"EC",	"ECUADOR",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{818,	"EG",	"EGYPT",				TRUE,	A_BAND_REGION_2,	TRUE,	G_BAND_REGION_1},
	{222,	"SV",	"EL SALVADOR",			FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{233,	"EE",	"ESTONIA",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{246,	"FI",	"FINLAND",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{250,	"FR",	"FRANCE",				TRUE,	A_BAND_REGION_2,	TRUE,	G_BAND_REGION_1},
	{268,	"GE",	"GEORGIA",				TRUE,	A_BAND_REGION_2,	TRUE,	G_BAND_REGION_1},
	{276,	"DE",	"GERMANY",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{300,	"GR",	"GREECE",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{320,	"GT",	"GUATEMALA",			TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_0},
	{340,	"HN",	"HONDURAS",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{344,	"HK",	"HONG KONG",			TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{348,	"HU",	"HUNGARY",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{352,	"IS",	"ICELAND",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{356,	"IN",	"INDIA",				TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{360,	"ID",	"INDONESIA",			TRUE,	A_BAND_REGION_4,	TRUE,	G_BAND_REGION_1},
	{364,	"IR",	"IRAN",					TRUE,	A_BAND_REGION_4,	TRUE,	G_BAND_REGION_1},
	{372,	"IE",	"IRELAND",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{376,	"IL",	"ISRAEL",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{380,	"IT",	"ITALY",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{392,	"JP",	"JAPAN",				TRUE,	A_BAND_REGION_9,	TRUE,	G_BAND_REGION_1},
	{400,	"JO",	"JORDAN",				TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{398,	"KZ",	"KAZAKHSTAN",			FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{408,	"KP",	"KOREA DEMOCRATIC PEOPLE'S REPUBLIC OF",TRUE,	A_BAND_REGION_5,	TRUE,	G_BAND_REGION_1},
	{410,	"KR",	"KOREA REPUBLIC OF",	TRUE,	A_BAND_REGION_5,	TRUE,	G_BAND_REGION_1},
	{414,	"KW",	"KUWAIT",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{428,	"LV",	"LATVIA",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{422,	"LB",	"LEBANON",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{438,	"LI",	"LIECHTENSTEIN",		TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{440,	"LT",	"LITHUANIA",			TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{442,	"LU",	"LUXEMBOURG",			TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{446,	"MO",	"MACAU",				TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{807,	"MK",	"MACEDONIA",			FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{458,	"MY",	"MALAYSIA",				TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{484,	"MX",	"MEXICO",				TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_0},
	{492,	"MC",	"MONACO",				TRUE,	A_BAND_REGION_2,	TRUE,	G_BAND_REGION_1},
	{504,	"MA",	"MOROCCO",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{528,	"NL",	"NETHERLANDS",			TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{554,	"NZ",	"NEW ZEALAND",			TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{578,	"NO",	"NORWAY",				TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_0},
	{512,	"OM",	"OMAN",					TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{586,	"PK",	"PAKISTAN",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{591,	"PA",	"PANAMA",				TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_0},
	{604,	"PE",	"PERU",					TRUE,	A_BAND_REGION_4,	TRUE,	G_BAND_REGION_1},
	{608,	"PH",	"PHILIPPINES",			TRUE,	A_BAND_REGION_4,	TRUE,	G_BAND_REGION_1},
	{616,	"PL",	"POLAND",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{620,	"PT",	"PORTUGAL",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{630,	"PR",	"PUERTO RICO",			TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_0},
	{634,	"QA",	"QATAR",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{642,	"RO",	"ROMANIA",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{643,	"RU",	"RUSSIA FEDERATION",	FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{682,	"SA",	"SAUDI ARABIA",			FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{702,	"SG",	"SINGAPORE",			TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{703,	"SK",	"SLOVAKIA",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{705,	"SI",	"SLOVENIA",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{710,	"ZA",	"SOUTH AFRICA",			TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{724,	"ES",	"SPAIN",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{752,	"SE",	"SWEDEN",				TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{756,	"CH",	"SWITZERLAND",			TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{760,	"SY",	"SYRIAN ARAB REPUBLIC",	FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{158,	"TW",	"TAIWAN",				TRUE,	A_BAND_REGION_3,	TRUE,	G_BAND_REGION_0},
	{764,	"TH",	"THAILAND",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{780,	"TT",	"TRINIDAD AND TOBAGO",	TRUE,	A_BAND_REGION_2,	TRUE,	G_BAND_REGION_1},
	{788,	"TN",	"TUNISIA",				TRUE,	A_BAND_REGION_2,	TRUE,	G_BAND_REGION_1},
	{792,	"TR",	"TURKEY",				TRUE,	A_BAND_REGION_2,	TRUE,	G_BAND_REGION_1},
	{804,	"UA",	"UKRAINE",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{784,	"AE",	"UNITED ARAB EMIRATES",	FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{826,	"GB",	"UNITED KINGDOM",		TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_1},
	{840,	"US",	"UNITED STATES",		TRUE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_0},
	{858,	"UY",	"URUGUAY",				TRUE,	A_BAND_REGION_5,	TRUE,	G_BAND_REGION_1},
	{860,	"UZ",	"UZBEKISTAN",			TRUE,	A_BAND_REGION_1,	TRUE,	G_BAND_REGION_0},
	{862,	"VE",	"VENEZUELA",			TRUE,	A_BAND_REGION_5,	TRUE,	G_BAND_REGION_1},
	{704,	"VN",	"VIET NAM",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{887,	"YE",	"YEMEN",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{716,	"ZW",	"ZIMBABWE",				FALSE,	A_BAND_REGION_0,	TRUE,	G_BAND_REGION_1},
	{999,	"",	"",	0,	0,	0,	0}
};

#define NUM_OF_COUNTRIES	(sizeof(allCountry)/sizeof(COUNTRY_CODE_TO_COUNTRY_REGION))

#ifdef CFG_SUPPORT_MU_MIMO

/* iwprive test code */

INT32 hqa_mu_get_qd(PRTMP_ADAPTER pAd, RTMP_STRING * arg);
INT32 hqa_mu_get_init_mcs(PRTMP_ADAPTER pAd, RTMP_STRING * arg);
INT32 hqa_mu_get_lq(PRTMP_ADAPTER pAd, RTMP_STRING *arg);

INT32 hqa_mu_cal_init_mcs(PRTMP_ADAPTER pAd, RTMP_STRING *arg);
INT32 hqa_mu_cal_lq(PRTMP_ADAPTER pAd, RTMP_STRING *arg);

INT32 hqa_mu_set_snr_offset(PRTMP_ADAPTER pAd, RTMP_STRING *arg);
INT32 hqa_mu_set_zero_nss(PRTMP_ADAPTER pAd, RTMP_STRING *arg);
INT32 hqa_mu_set_speedup_lq(PRTMP_ADAPTER pAd, RTMP_STRING *arg);
INT32 hqa_mu_set_mu_table(PRTMP_ADAPTER pAd, RTMP_STRING *arg);
INT32 hqa_mu_set_group(PRTMP_ADAPTER pAd, RTMP_STRING *arg);
INT32 hqa_mu_set_enable(PRTMP_ADAPTER pAd, RTMP_STRING *arg);
INT32 hqa_mu_set_gid_up(PRTMP_ADAPTER pAd, RTMP_STRING *arg);
INT32 hqa_su_cal_lq(PRTMP_ADAPTER pAd, RTMP_STRING *arg);
INT32 hqa_su_get_lq(PRTMP_ADAPTER pAd, RTMP_STRING *arg);
INT32 hqa_mu_set_gid_up(PRTMP_ADAPTER pAd, RTMP_STRING *arg);
INT32 hqa_mu_set_trigger_mu_tx(PRTMP_ADAPTER pAd, RTMP_STRING *arg);


/* get function */
INT ShowMuEnableProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowMuProfileProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowGroupTblEntryProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowClusterTblEntryProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowGroupUserThresholdProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowGroupNssThresholdProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowTxReqMinTimeProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowSuNssCheckProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowCalcInitMCSProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowTxopDefaultProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowSuLossThresholdProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowMuGainThresholdProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowSecondaryAcPolicyProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowGroupTblDmcsMaskProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowMaxGroupSearchCntProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT ShowMuProfileTxStsCntProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

/* set function */
INT SetMuProfileProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetGroupTblEntryProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetClusterTblEntryProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetMuEnableProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetGroupUserThresholdProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetGroupNssThresholdProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetTxReqMinTimeProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetSuNssCheckProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetCalculateInitMCSProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetTriggerGIDMgmtFrameProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetTriggerMuTxProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetTriggerDegroupProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetTriggerGroupProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetTriggerBbpProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetTriggerSndProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetTxopDefaultProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetSuLossThresholdProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetMuGainThresholdProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetSecondaryAcPolicyProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetGroupTblDmcsMaskProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetMaxGroupSearchCntProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif

#ifdef CFG_SUPPORT_MU_MIMO_RA
// mura set function
INT SetMuraPeriodicSndProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetMuraTestAlgorithmProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetMuraTestAlgorithmInit(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetMuraFixedRateProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetMuraFixedGroupRateProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT SetMuraFixedSndParamProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

// mura get function
INT GetMuraMonitorStateProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

#endif

INT Set_CountryString_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_CountryCode_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

#ifdef EXT_BUILD_CHANNEL_LIST
INT Set_ChGeography_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* EXT_BUILD_CHANNEL_LIST */

#ifdef SPECIFIC_TX_POWER_SUPPORT
INT Set_AP_PKT_PWR(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* SPECIFIC_TX_POWER_SUPPORT */

INT Set_AP_SSID_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_TxRate_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);


INT	Set_OLBCDetection_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT	Set_AP_PerMbssMaxStaNum_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT	Set_AP_IdleTimeout_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#ifdef IAPP_SUPPORT
INT	Set_IappPID_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* IAPP_SUPPORT */

INT Set_AP_WpaMixPairCipher_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_AP_RekeyInterval_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_AP_RekeyMethod_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_AP_PMKCachePeriod_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_AP_ASSOC_REQ_RSSI_THRESHOLD(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_AP_KickStaRssiLow_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_BasicRate_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);


INT Set_BeaconPeriod_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_DtimPeriod_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_NoForwarding_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_NoForwardingBTNSSID_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_AP_WmmCapable_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_HideSSID_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_VLANID_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_VLANPriority_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_VLAN_TAG_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_AccessPolicy_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_ACLAddEntry_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_ACLDelEntry_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_ACLShowAll_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_ACLClearAll_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_SiteSurvey_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_AutoChannelSel_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#ifdef AP_SCAN_SUPPORT
INT Set_AutoChannelSelCheckTime_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* AP_SCAN_SUPPORT */

INT Set_BADecline_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Show_StaCount_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Show_DriverInfo_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Show_Sat_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Show_RAInfo_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

#ifdef RTMP_MAC_PCI
#ifdef DBG_DIAGNOSE
INT Set_DiagOpt_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_BDInfo_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_diag_cond_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Show_Diag_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* DBG_DAIGNOSE */
#endif /* RTMP_MAC_PCI */


INT show_timer_list(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT show_wtbl_state(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT show_radio_info_proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Show_Sat_Reset_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Show_MATTable_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#ifdef DOT1X_SUPPORT
INT Set_IEEE8021X_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_PreAuth_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_RADIUS_Server_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_RADIUS_Port_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_RADIUS_Key_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_DeletePMKID_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_DumpPMKID_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* DOT1X_SUPPORT */

INT Set_DisConnectSta_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_DisConnectAllSta_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);


#ifdef APCLI_SUPPORT
INT Set_ApCli_Enable_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_ApCli_Ssid_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_ApCli_Bssid_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_ApCli_TxMode_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_ApCli_TxMcs_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_ApCli_Channel_Proc(PRTMP_ADAPTER pAd,RTMP_STRING *arg);
INT Set_ApCli_WirelessMode_Proc(PRTMP_ADAPTER pAd,RTMP_STRING *arg);

#ifdef APCLI_AUTO_CONNECT_SUPPORT
INT Set_ApCli_AutoConnect_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* APCLI_AUTO_CONNECT_SUPPORT */
#ifdef APCLI_CONNECTION_TRIAL
INT Set_ApCli_Trial_Ch_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* APCLI_CONNECTION_TRIAL */


#ifdef MAC_REPEATER_SUPPORT
INT Set_ReptMode_Enable_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_Cli_Link_Map_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* MAC_REPEATER_SUPPORT */

#ifdef WSC_AP_SUPPORT
INT Set_AP_WscSsid_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* WSC_AP_SUPPORT */

#ifdef APCLI_CERT_SUPPORT
INT Set_ApCli_Cert_Enable_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
//Add for APCLI PMF 5.3.3.3 option test item. (Only Tx De-auth Req. and make sure the pkt can be Encrypted)
INT ApCliTxDeAuth(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* APCLI_CERT_SUPPORT */
#endif /* APCLI_SUPPORT */
#ifdef UAPSD_SUPPORT
INT Set_UAPSD_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* UAPSD_SUPPORT */

#ifdef WSC_AP_SUPPORT
INT Set_WscStatus_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

#ifdef BB_SOC
INT Set_WscOOB_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif

INT Set_WscStop_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

VOID RTMPIoctlWscProfile(
	IN PRTMP_ADAPTER pAdapter,
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);

VOID RTMPIoctlWscPINCode(
	IN PRTMP_ADAPTER pAdapter,
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);

VOID RTMPIoctlWscStatus(
	IN PRTMP_ADAPTER pAdapter,
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);

VOID RTMPIoctlGetWscDynInfo(
	IN PRTMP_ADAPTER pAdapter,
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);

VOID RTMPIoctlGetWscRegsDynInfo(
	IN PRTMP_ADAPTER pAdapter,
	IN RTMP_IOCTL_INPUT_STRUCT *wrq);

BOOLEAN WscCheckEnrolleeNonceFromUpnp(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	RTMP_STRING *pData,
	IN  USHORT			Length,
	IN  PWSC_CTRL       pWscControl);

UCHAR	WscRxMsgTypeFromUpnp(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  RTMP_STRING *pData,
	IN	USHORT				Length);

INT	    WscGetConfForUpnp(
	IN	PRTMP_ADAPTER	pAd,
	IN  PWSC_CTRL       pWscControl);

#ifdef CON_WPS
INT     Set_ConWpsApCliMode_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT     Set_ConWpsApcliAutoPreferIface_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT     Set_ConWpsApCliDisabled_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* CON_WPS */
INT	Set_AP_WscConfMode_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_AP_WscConfStatus_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_AP_WscMode_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_AP_WscGetConf_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT	Set_AP_WscPinCode_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_AP_WscSecurityMode_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_AP_WscMultiByteCheck_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);


INT	Set_WscVersion_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

#ifdef WSC_V2_SUPPORT
INT	Set_WscV2Support_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_WscVersion2_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_WscExtraTlvTag_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_WscExtraTlvType_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_WscExtraTlvData_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_WscSetupLock_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_WscFragment_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_WscFragmentSize_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_WscMaxPinAttack_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT	Set_WscSetupLockTime_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* WSC_V2_SUPPORT */
INT	Set_WscAutoTriggerDisable_Proc(
	IN	RTMP_ADAPTER *pAd,
	IN	RTMP_STRING	 *arg);
#endif /* WSC_AP_SUPPORT */


#ifdef CONFIG_AP_SUPPORT
#ifdef MCAST_RATE_SPECIFIC
INT Set_McastPhyMode(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_McastMcs(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Show_McastRate(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* MCAST_RATE_SPECIFIC */

#ifdef DOT11N_DRAFT3
INT Set_OBSSScanParam_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_AP2040ReScan_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* DOT11N_DRAFT3 */

INT Set_EntryLifeCheck_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);


#ifdef AP_QLOAD_SUPPORT
INT Set_QloadClr_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

/* QLOAD ALARM */
INT Set_QloadAlarmTimeThreshold_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT Set_QloadAlarmNumThreshold_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* AP_QLOAD_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */


INT Set_MemDebug_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

#ifdef CONFIG_AP_SUPPORT
INT Set_PowerSaveLifeTime_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* CONFIG_AP_SUPPORT */


#ifdef DYNAMIC_VGA_SUPPORT
INT	Set_DyncVgaEnable_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

INT set_false_cca_hi_th(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT set_false_cca_low_th(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* DYNAMIC_VGA_SUPPORT */

#ifdef MT_MAC
INT	Set_AP_TimEnable_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT setApTmrEnableProc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Show_TmrCalResult_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

#ifdef CUT_THROUGH
INT Set_CutThrough_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* CUT_THROUGH */

#ifdef DBG
INT    set_tim_update_proc(
       IN      PRTMP_ADAPTER   pAd,
       IN      RTMP_STRING     *arg);

INT    Set_AP_DumpTime_Proc(
       IN      PRTMP_ADAPTER   pAd,
       IN      RTMP_STRING     *arg);

INT    Set_BcnStateCtrl_Proc(
       IN      PRTMP_ADAPTER   pAd,
       IN      RTMP_STRING     *arg);

INT    set_dump_sta_rec_history_proc(
       IN      PRTMP_ADAPTER   pAd,
       IN      RTMP_STRING     *arg);

INT    set_dump_rx_ba_scoreboard_proc(
       IN      PRTMP_ADAPTER   pAd,
       IN      RTMP_STRING     *arg);
#endif
#ifdef CAL_TO_FLASH_SUPPORT	
INT Set_KtoFlash_Debug_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg);

INT Set_RDCE_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg);
#endif /* CAL_TO_FLASH_SUPPORT */

#ifdef TX_AGG_ADJUST_WKR
INT Set_AggAdjWkr_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg);
#endif /* TX_AGG_ADJUST_WKR */

#ifdef RLM_CAL_CACHE_SUPPORT
INT Set_RLM_Cal_Cache_Debug_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg);
#endif /* RLM_CAL_CACHE_SUPPORT */

#ifdef PKT_BUDGET_CTRL_SUPPORT
INT Set_PBC_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg);

#endif /*PKT_BUDGET_CTRL_SUPPORT*/

INT Set_BWF_Enable_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg);

#ifdef CONFIG_HOTSPOT_R2
INT Set_CR4_Hotspot_Flag(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg);
#endif /* CONFIG_HOTSPOT_R2 */

#ifdef HTC_DECRYPT_IOT
INT Set_HTC_Err_TH_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg);
INT Set_Entry_HTC_Err_Cnt_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg);
INT Set_WTBL_AAD_OM_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg);
#endif /* HTC_DECRYPT_IOT */

#ifdef DHCP_UC_SUPPORT
INT Set_DHCP_UC_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg);
#endif /* DHCP_UC_SUPPORT */
#endif /* MT_MAC */

/**
 * @addtogroup embedded_ioctl
 * @{
 * @code AP Set IOCTL
 */
static struct {
	RTMP_STRING *name;
	INT (*set_proc)(PRTMP_ADAPTER pAdapter, RTMP_STRING *arg);
} *PRTMP_PRIVATE_SET_PROC, RTMP_PRIVATE_SUPPORT_PROC[] = {
	{"RateAlg",						Set_RateAlg_Proc},
#ifdef NEW_RATE_ADAPT_SUPPORT
	{"PerThrdAdj",					Set_PerThrdAdj_Proc},
	{"LowTrafficThrd",				Set_LowTrafficThrd_Proc},
	{"TrainUpRule",					Set_TrainUpRule_Proc},
	{"TrainUpRuleRSSI",				Set_TrainUpRuleRSSI_Proc},
	{"TrainUpLowThrd",				Set_TrainUpLowThrd_Proc},
	{"TrainUpHighThrd",				Set_TrainUpHighThrd_Proc},
	{"RateTable",					Set_RateTable_Proc},
#endif /* NEW_RATE_ADAPT_SUPPORT */
	{"DriverVersion",				Set_DriverVersion_Proc},
	{"CountryRegion",				Set_CountryRegion_Proc},
	{"CountryRegionABand",			Set_CountryRegionABand_Proc},
	{"CountryString",				Set_CountryString_Proc},
	{"CountryCode",				Set_CountryCode_Proc},
#ifdef EXT_BUILD_CHANNEL_LIST
	{"ChGeography",				Set_ChGeography_Proc},
#endif /* EXT_BUILD_CHANNEL_LIST */
	{"SSID",						Set_AP_SSID_Proc},
	{"WirelessMode",				Set_WirelessMode_Proc},
	{"BasicRate",					Set_BasicRate_Proc},
	{"ShortSlot",					Set_ShortSlot_Proc},
	{"Channel",					Set_Channel_Proc},
	{"BeaconPeriod",				Set_BeaconPeriod_Proc},
	{"DtimPeriod",					Set_DtimPeriod_Proc},
	{"TxPower",					Set_TxPower_Proc},
	{"BGProtection",				Set_BGProtection_Proc},
	{"DisableOLBC", 				Set_OLBCDetection_Proc},
	{"TxPreamble",				Set_TxPreamble_Proc},
	{"RTSThreshold",				Set_RTSThreshold_Proc},
	{"FragThreshold",				Set_FragThreshold_Proc},
	{"TxBurst",					Set_TxBurst_Proc},
	{"MbssMaxStaNum",					Set_AP_PerMbssMaxStaNum_Proc},
#ifdef RTMP_MAC_PCI
	{"ShowRF",					Set_ShowRF_Proc},
#endif /* RTMP_MAC_PCI */
	{"IdleTimeout",					Set_AP_IdleTimeout_Proc},
#ifdef DOT11_N_SUPPORT
	{"BASetup",					Set_BASetup_Proc},
	{"BADecline",					Set_BADecline_Proc},
	{"SendMIMOPS",				Set_SendSMPSAction_Proc},
	{"BAOriTearDown",				Set_BAOriTearDown_Proc},
	{"BARecTearDown",				Set_BARecTearDown_Proc},
	{"HtBw",						Set_HtBw_Proc},
	{"HtMcs",						Set_HtMcs_Proc},
	{"HtGi",						Set_HtGi_Proc},
	{"HtOpMode",					Set_HtOpMode_Proc},
	{"HtStbc",					Set_HtStbc_Proc},
	{"HtExtcha",					Set_HtExtcha_Proc},
	{"HtMpduDensity",				Set_HtMpduDensity_Proc},
	{"HtBaWinSize",				Set_HtBaWinSize_Proc},
	{"HtMIMOPS",					Set_HtMIMOPSmode_Proc},
	{"HtRdg",						Set_HtRdg_Proc},
	{"HtLinkAdapt",				Set_HtLinkAdapt_Proc},
	{"HtAmsdu",					Set_HtAmsdu_Proc},
	{"HtAutoBa",					Set_HtAutoBa_Proc},
	{"HtProtect",					Set_HtProtect_Proc},
	{"HtMimoPs",					Set_HtMimoPs_Proc},
	{"HtTxStream",				Set_HtTxStream_Proc},
	{"HtRxStream",				Set_HtRxStream_Proc},
	{"ForceShortGI",				Set_ForceShortGI_Proc},
	{"ForceGF",		        		Set_ForceGF_Proc},
	{"HtTxBASize",					Set_HtTxBASize_Proc},
	{"BurstMode",					Set_BurstMode_Proc},
#ifdef GREENAP_SUPPORT
	{"GreenAP",					Set_GreenAP_Proc},
#endif /* GREENAP_SUPPORT */
	{"HtDisallowTKIP",				Set_HtDisallowTKIP_Proc},
#endif /* DOT11_N_SUPPORT */

#ifdef DOT11_VHT_AC
	{"VhtBw",					Set_VhtBw_Proc},
	{"VhtStbc",					Set_VhtStbc_Proc},
	{"VhtBwSignal",				set_VhtBwSignal_Proc},
	{"VhtDisallowNonVHT",		Set_VhtDisallowNonVHT_Proc},
#endif /* DOT11_VHT_AC */

#ifdef IAPP_SUPPORT
	{"IappPID",					Set_IappPID_Proc},
#endif /* IAPP_SUPPORT */

#ifdef AGGREGATION_SUPPORT
	{"PktAggregate",				Set_PktAggregate_Proc},
#endif /* AGGREGATION_SUPPORT */

	{"WmmCapable",				Set_AP_WmmCapable_Proc},

	{"NoForwarding",				Set_NoForwarding_Proc},
	{"NoForwardingBTNBSSID",		Set_NoForwardingBTNSSID_Proc},
	{"HideSSID",					Set_HideSSID_Proc},
	{"IEEE80211H",				Set_IEEE80211H_Proc},
	{"VLANID",					Set_VLANID_Proc},
	{"VLANPriority",				Set_VLANPriority_Proc},
	{"VLANTag",					Set_VLAN_TAG_Proc},
	{"AuthMode",					Set_SecAuthMode_Proc},
	{"EncrypType",					Set_SecEncrypType_Proc},
	{"WpaMixPairCipher", 			Set_AP_WpaMixPairCipher_Proc},
	{"RekeyInterval",				Set_AP_RekeyInterval_Proc},
	{"RekeyMethod", 				Set_AP_RekeyMethod_Proc},
	{"DefaultKeyID",				Set_SecDefaultKeyID_Proc},
	{"Key1",						Set_SecKey1_Proc},
	{"Key2",						Set_SecKey2_Proc},
	{"Key3",						Set_SecKey3_Proc},
	{"Key4",						Set_SecKey4_Proc},
	{"AccessPolicy",				Set_AccessPolicy_Proc},
	{"ACLAddEntry",					Set_ACLAddEntry_Proc},
	{"ACLDelEntry",					Set_ACLDelEntry_Proc},
	{"ACLShowAll",					Set_ACLShowAll_Proc},
	{"ACLClearAll",					Set_ACLClearAll_Proc},
	{"WPAPSK",					Set_SecWPAPSK_Proc},
	{"RadioOn",					Set_RadioOn_Proc},
	{"Lp",     						Set_Lp_Proc},
#ifdef SPECIFIC_TX_POWER_SUPPORT
	{"PktPwr",						Set_AP_PKT_PWR},
#endif /* SPECIFIC_TX_POWER_SUPPORT */
	{"AssocReqRssiThres",           Set_AP_ASSOC_REQ_RSSI_THRESHOLD},
	{"KickStaRssiLow",				Set_AP_KickStaRssiLow_Proc},
#ifdef AP_SCAN_SUPPORT
	{"SiteSurvey",					Set_SiteSurvey_Proc},
	{"AutoChannelSel",				Set_AutoChannelSel_Proc},
	{"ACSCheckTime",				Set_AutoChannelSelCheckTime_Proc},
#endif /* AP_SCAN_SUPPORT */
	{"ResetCounter",				Set_ResetStatCounter_Proc},
	{"DisConnectSta",				Set_DisConnectSta_Proc},
	{"DisConnectAllSta",			Set_DisConnectAllSta_Proc},
#ifdef DOT1X_SUPPORT
	{"IEEE8021X",					Set_IEEE8021X_Proc},
	{"PreAuth",						Set_PreAuth_Proc},
	{"PMKCachePeriod", 				Set_AP_PMKCachePeriod_Proc},
	{"own_ip_addr",					Set_OwnIPAddr_Proc},
	{"EAPifname",					Set_EAPIfName_Proc},
	{"PreAuthifname",				Set_PreAuthIfName_Proc},
	{"RADIUS_Server",				Set_RADIUS_Server_Proc},
	{"RADIUS_Port",					Set_RADIUS_Port_Proc},
	{"RADIUS_Key",					Set_RADIUS_Key_Proc},
	{"DeletePMKID",					Set_DeletePMKID_Proc},
	{"DumpPMKID",					Set_DumpPMKID_Proc},
#endif /* DOT1X_SUPPORT */
#ifdef DBG
	{"Debug",					Set_Debug_Proc},
	{"DebugCat",					Set_DebugCategory_Proc},
	{"DebugMntAddr",				Set_Debug_MonitorAddr},
#endif /* DBG */

#ifdef INTERNAL_CAPTURE_SUPPORT
	{"Icap_WifiSpec_Switch",	Set_Icap_WifiSpec_Switch},
	{"Icap_WifiSpec_Param",	        Set_Icap_WifiSpec_Param},
	{"Icap_WifiSpec_Result",	Get_Icap_WifiSpec_Result},
	{"Icap_WifiSpec_Sysram", Get_Icap_WifiSpec_Sysram},
	{"Icap_WifiSpec_RawDataProc", Get_Icap_WifiSpec_RawData_Proc},
	{"Icap_WifiSpec_WirelessInfo",   Get_Icap_WifiSpec_WirelessInfo},
#endif /* INTERNAL_CAPTURE_SUPPORT */
	{"IRR_ADC",                     Set_IRR_ADC},
	{"IRR_RxGain",                  Set_IRR_RxGain},
	{"IRR_TTG",                     Set_IRR_TTG},
	{"IRR_TTGOnOff",                Set_IRR_TTGOnOff},

#ifdef MT_DFS_SUPPORT //Jelly20150301
    {"ShowDfsCh",                   Set_DfsChannelShow_Proc},
    {"ShowDfsBw",                   Set_DfsBwShow_Proc},
    {"ShowDfsRDMode",               Set_DfsRDModeShow_Proc},

    {"ShowDfsRegion",               Set_DfsRDDRegionShow_Proc},
    {"RadarDetectStart",            Set_RadarDetectStart_Proc},
    {"RadarDetectStop",             Set_RadarDetectStop_Proc},
	{"ByPassCac",                   Set_ByPassCac_Proc},
    {"ShowDfsNOP",              Set_DfsNonOccupancyShow_Proc},
    {"RDDReport",					Set_RDDReport_Proc},
    {"DfsPulseDBG",                   Set_DfsPulseInfoMode_Proc},
    {"DfsPulseRead",                   Set_DfsPulseInfoRead_Proc},

    /* DFS zero wait */
    {"DfsZeroWaitCacTime",          Set_DfsZeroWaitCacTime_Proc},  
#endif

#if defined(DFS_SUPPORT) || defined(CARRIER_DETECTION_SUPPORT)
	{"RadarShow",					Set_RadarShow_Proc},
#ifdef DFS_SUPPORT
	{"RadarDebug",					Set_RadarDebug_Proc},
	/*{"RadarHit",					Set_RadarHit_Proc},*/
	{"CSPeriod",					Set_CSPeriod_Proc},
	{"ResetRadarHwDetect",					Set_ResetRadarHwDetect_Proc},
	{"DfsSwDisable",				Set_DfsSwDisable_Proc},
	{"DfsEvDropAdjTime",			Set_DfsEnvtDropAdjTime_Proc},
	{"RadarStart",					Set_RadarStart_Proc},
	{"RadarStop",					Set_RadarStop_Proc},
	{"RadarT1",						Set_RadarSetTbl1_Proc},
	{"RadarT2",						Set_RadarSetTbl2_Proc},
	{"PollTime",					Set_PollTime_Proc},
	{"PrintBusyIdle",				Set_PrintBusyIdle_Proc},
	{"BusyIdleRatio",				Set_BusyIdleRatio_Proc},
	{"DfsRssiHigh",				Set_DfsRssiHigh_Proc},
	{"DfsRssiLow",					Set_DfsRssiLow_Proc},
	{"ChMovTime",					Set_ChMovingTime_Proc},
	{"BlockChReset",				Set_BlockChReset_Proc},
#endif /* DFS_SUPPORT */
#ifdef CARRIER_DETECTION_SUPPORT
	{"CarrierDetect",				Set_CarrierDetect_Proc},
	{"CarrierCriteria",				Set_CarrierCriteria_Proc},
	{"CarrierReCheck",				Set_CarrierReCheck_Proc},
	{"CarrierGoneThreshold",			Set_CarrierGoneThreshold_Proc},
	{"CarrierDebug",				Set_CarrierDebug_Proc},
	{"Delta",						Set_CarrierDelta_Proc},
	{"DivFlag",						Set_CarrierDivFlag_Proc},
	{"CarrThrd",					Set_CarrierThrd_Proc},
	/* v2 functions */
	{"SymRund",					Set_CarrierSymRund_Proc},
	{"CarrMask",					Set_CarrierMask_Proc},
#endif /* CARRIER_DETECTION_SUPPORT */
#endif /* defined(DFS_SUPPORT) || defined(CARRIER_DETECTION_SUPPORT) */



#ifdef CONFIG_ATE
	{"ATE",	SetATE},
	{"ATEMPSDUMP", SetATEMPSDump},
	{"ATEMPSPHY", SetATEMPSPhyMode},
	{"ATEMPSRATE", SetATEMPSRate},
	{"ATEMPSPATH", SetATEMPSPath},
	{"ATEMPSLEN", SetATEMPSPayloadLen},
	{"ATEMPSTXCNT", SetATEMPSPktCnt},
	{"ATEMPSTXPWR", SetATEMPSPwr},
	{"ATEMPSNSS", SetATEMPSNss},
	{"ATEMPSPKTBW", SetATEMPSPktBw},
	{"ATEMPSTXSTART", SetATEMPSStart},
	{"ATELOGEN", SetATELOGEnable},
	{"ATELOGDUMP", SetATELOGDump},
	{"ATEMACTRX", SetATEMACTRx},
	{"ATETXSTREAM", SetATETxStream},
	{"ATERXSTREAM", SetATERxStream},
	{"ATETXSENABLE", SetATETxSEnable},
	{"ATERXFILTER", SetATERxFilter},
	{"ATELOGDIS", SetATELOGDisable},
	{"ATEDEQCNT", SetATEDeqCnt},
	{"ATEQID", SetATEQid},
	{"ATEDA", SetATEDa},
	{"ATESA", SetATESa},
	{"ADCDump", SetADCDump},
	{"ATEBSSID", SetATEBssid},
	{"ATECHANNEL", SetATEChannel},

#ifdef MT7615
    {"ATECHANNELEXT", set_ate_channel_ext},
    {"ATESTARTTXEXT", set_ate_start_tx_ext},
#endif /* MT7615 */
	{"ATEINITCHAN", SetATEInitChan},
#ifdef RTMP_TEMPERATURE_CALIBRATION
	{"ATETEMPCAL", SetATETempCal},
	{"ATESHOWTSSI",	SetATEShowTssi},
#endif /* RTMP_TEMPERATURE_CALIBRATION */
#ifdef RTMP_INTERNAL_TX_ALC
	{"ATETSSICBA", SetATETssiCalibration},
#if defined(RT3350) || defined(RT3352)
	{"ATETSSICALBRENABLE", RT335x_Set_ATE_TSSI_CALIBRATION_ENABLE_Proc},
#endif /* defined(RT3350) || defined(RT3352) */
#endif /* RTMP_INTERNAL_TX_ALC */
#if defined(RTMP_INTERNAL_TX_ALC) || defined(RTMP_TEMPERATURE_COMPENSATION)
	{"ATETSSICBAEX", SetATETssiCalibrationEx},
#endif /* defined(RTMP_INTERNAL_TX_ALC) || defined(RTMP_TEMPERATURE_COMPENSATION) */
	{"ATETXPOW0", SetATETxPower0},
	{"ATETXPOW1", SetATETxPower1},
	{"ATETXPOW2", SetATETxPower2},
	{"ATETXPOW3", SetATETxPower3},
	{"ATETXPOWEVAL", SetATETxPowerEvaluation},
	{"ATETXANT", SetATETxAntenna},
	{"ATERXANT", SetATERxAntenna},
#ifdef RT3350
	{"ATEPABIAS", SetATEPABias},
#endif /* RT3350 */
	{"ATETXFREQOFFSET", SetATETxFreqOffset},
	{"ATETXBW", SetATETxBw},
	{"ATETXLEN", SetATETxLength},
	{"ATETXCNT", SetATETxCount},
	{"ATETXMCS", SetATETxMcs},
	{"ATEVHTNSS", SetATEVhtNss},
	{"ATETXSTBC", SetATETxStbc},
	{"ATETXMODE", SetATETxMode},
	{"ATETXGI", SetATETxGi},
	{"ATERXFER", SetATERxFer},
	{"ATERRF", SetATEReadRF},
#if (!defined(RTMP_RF_RW_SUPPORT)) && (!defined(RLT_RF))
	{"ATEWRF1",	SetATEWriteRF1},
	{"ATEWRF2", SetATEWriteRF2},
	{"ATEWRF3", SetATEWriteRF3},
	{"ATEWRF4",	SetATEWriteRF4},
#endif /* (!defined(RTMP_RF_RW_SUPPORT)) && (!defined(RLT_RF)) */
	{"ATELDE2P", SetATELoadE2p},
	{"ATERE2P", SetATEReadE2p},
#ifdef LED_CONTROL_SUPPORT
#endif /* LED_CONTROL_SUPPORT */
	{"ATEAUTOALC", SetATEAutoAlc},
	{"ATETEMPSENSOR", SetATETempSensor},
	{"ATEIPG", SetATEIpg},
	{"ATEPAYLOAD", SetATEPayload},
	{"ATEFIXEDPAYLOAD", SetATEFixedPayload},
#ifdef TXBF_SUPPORT
#ifdef MT_MAC
    {"ATETxBfInit",              SetATETxBfDutInitProc},
    {"ATETxBfGdInit",            SetATETxBfGdInitProc},
    {"ATETxBfChanProfileUpdate", SetATETxBfChanProfileUpdate},
	{"ATETXBF",                  SetATETXBFProc},
	{"ATETXSOUNDING",            SetATETxSoundingProc},
    {"ATEIBfGdCal",              SetATEIBfGdCal},
    {"ATEIBfInstCal",            SetATEIBfInstCal},
    {"ATETxBfLnaGain",           SetATETxBfLnaGain},
    {"ATEIBfProfileConfig",      SetATEIBfProfileUpdate},
    {"ATEEBfProfileConfig",      SetATEEBfProfileConfig},
    {"ATETxBfProfileRead",       SetATETxBfProfileRead},
    {"ATETxPacketWithBf",        SetATETxPacketWithBf},
    {"ATEIBFPhaseE2pUpdate",     SetATETxBfPhaseE2pUpdate},
    {"ATEIBFPhaseComp",          SetATEIBfPhaseComp},
    {"ATEIBFPhaseVerify",        SetATEIBfPhaseVerify},
    {"ATEConTxETxBfGdProc",      SetATEConTxETxBfGdProc},
    {"ATEConTxETxBfInitProc",    SetATEConTxETxBfInitProc},
    {"ATESPE",                   SetATESpeIdx},
	{"ATETXEBF",                 SetATEEBfTx},
	{"ATEEBFCE",                 SetATEEBFCE},
	{"ATEEBFCEInfo",             SetATEEBFCEInfo},
	{"ATEEBFCEHELP",             SetATEEBFCEHelp},
#endif
#endif /* TXBF_SUPPORT */
	{"ATETTR", SetATETtr},
	{"ATESHOW", SetATEShow},
	{"ATEHELP", SetATEHelp},
#ifdef CONFIG_QA
	{"TxStop", SetTxStop},
	{"RxStop", SetRxStop},
#ifdef DBG
	{"EERead", SetEERead},
	{"EEWrite",	SetEEWrite},
	{"BBPRead", SetBBPRead},
	{"BBPWrite", SetBBPWrite},
#endif /* DBG */
#endif /* CONFIG_QA */

#if defined(MT7615)

#ifdef SINGLE_SKU_V2
    {"ATESINGLESKU", SetATESingleSKUEn},
#endif /* SINGLE_SKU_V2 */

    {"BFBackoffMode", SetATEBFBackoffMode},
    {"ATEPowerPercent", SetATEPowerPercentEn},
    {"ATEPowerDropLevel", SetATEPowerPercentCtrl},
    {"ATEBFBackoff", SetATEBFBackoffEn},
    {"ATETempComp", SetATETempCompEn},
    {"ATETSSI", SetATETSSIEn},
    {"ATETXPOWERCTRL", SetATETxPowerCtrlEn},
#endif /* defined(MT7615) */

#endif /* CONFIG_ATE */

#ifdef APCLI_SUPPORT
	{"ApCliEnable",				Set_ApCli_Enable_Proc},
	{"ApCliSsid",					Set_ApCli_Ssid_Proc},
	{"ApCliBssid",					Set_ApCli_Bssid_Proc},
	{"ApCliAuthMode",				Set_SecAuthMode_Proc},
	{"ApCliEncrypType",			Set_SecEncrypType_Proc},
	{"ApCliDefaultKeyID",			Set_SecDefaultKeyID_Proc},
	{"ApCliWPAPSK",				Set_SecWPAPSK_Proc},
	{"ApCliKey1",					Set_SecKey1_Proc},
	{"ApCliKey2",					Set_SecKey2_Proc},
	{"ApCliKey3",					Set_SecKey3_Proc},
	{"ApCliKey4",					Set_SecKey4_Proc},
	{"ApCliTxMode",					Set_ApCli_TxMode_Proc},
	{"ApCliTxMcs",					Set_ApCli_TxMcs_Proc},
#ifdef APCLI_CONNECTION_TRIAL
	/* 
	 for Trial the root AP which locates on another channel 
	 what if the connection is ok, it will make BSSID switch to the new channel.
	*/
	{"ApCliTrialCh",				Set_ApCli_Trial_Ch_Proc},
#endif /* APCLI_CONNECTION_TRIAL */
#ifdef DBDC_MODE
	{"ApCliChannel",					Set_ApCli_Channel_Proc},
	{"ApCliWirelessMode",					Set_ApCli_WirelessMode_Proc},
#endif /*DBDC_MODE*/
#ifdef APCLI_AUTO_CONNECT_SUPPORT
	{"ApCliAutoConnect", 			Set_ApCli_AutoConnect_Proc},
#endif /* APCLI_AUTO_CONNECT_SUPPORT */

#ifdef MAC_REPEATER_SUPPORT
	{"MACRepeaterEn",			    Set_ReptMode_Enable_Proc},
    {"CliLinkMAP",                  Set_Cli_Link_Map_Proc},
#endif /* MAC_REPEATER_SUPPORT */

#ifdef WSC_AP_SUPPORT
	{"ApCliWscSsid",				Set_AP_WscSsid_Proc},
#endif /* WSC_AP_SUPPORT */

#ifdef APCLI_CERT_SUPPORT
	{"ApCliCertEnable",				Set_ApCli_Cert_Enable_Proc},
	//Add for APCLI PMF 5.3.3.3 option test item. (Only Tx De-auth Req. and make sure the pkt can be Encrypted)
	{"ApCliTxDeAuth",				ApCliTxDeAuth},	
#endif /* APCLI_CERT_SUPPORT */

#ifdef DOT11W_PMF_SUPPORT
	{"ApCliPMFMFPC",                                         Set_ApCliPMFMFPC_Proc},
	{"ApCliPMFMFPR",                                         Set_ApCliPMFMFPR_Proc},
	{"ApCliPMFSHA256",                                      Set_ApCliPMFSHA256_Proc},
#endif /* DOT11W_PMF_SUPPORT */

#endif /* APCLI_SUPPORT */
#ifdef WSC_AP_SUPPORT
#ifdef CON_WPS
	{"ConWpsApCliMode",			Set_ConWpsApCliMode_Proc},
	{"ConWpsApCliDisabled",                     Set_ConWpsApCliDisabled_Proc},
	{"ConWpsApcliPreferIface",		Set_ConWpsApcliAutoPreferIface_Proc},	
#endif /* CON_WPS */
	{"WscConfMode",				Set_AP_WscConfMode_Proc},
	{"WscConfStatus",				Set_AP_WscConfStatus_Proc},
	{"WscMode",					Set_AP_WscMode_Proc},
	{"WscStatus",					Set_WscStatus_Proc},
	{"WscGetConf",				Set_AP_WscGetConf_Proc},
	{"WscPinCode",				Set_AP_WscPinCode_Proc},
	{"WscStop",                     		Set_WscStop_Proc},
	{"WscGenPinCode",               		Set_WscGenPinCode_Proc},
	{"WscVendorPinCode",            Set_WscVendorPinCode_Proc},
	{"WscSecurityMode",				Set_AP_WscSecurityMode_Proc},
	{"WscMultiByteCheck",			Set_AP_WscMultiByteCheck_Proc},
	{"WscVersion", 					Set_WscVersion_Proc},
#ifdef BB_SOC
	{"WscOOB",					Set_WscOOB_Proc},
	{"WscResetPinCode", 		Set_WscResetPinCode_Proc},
#endif /* BB_SOC */	

#ifdef WSC_V2_SUPPORT
	{"WscV2Support", 				Set_WscV2Support_Proc},
	{"WscVersion2", 				Set_WscVersion2_Proc},
	{"WscExtraTlvTag", 				Set_WscExtraTlvTag_Proc},
	{"WscExtraTlvType",				Set_WscExtraTlvType_Proc},
	{"WscExtraTlvData", 			Set_WscExtraTlvData_Proc},
	{"WscSetupLock", 				Set_WscSetupLock_Proc},
	{"WscFragment",					Set_WscFragment_Proc},
	{"WscFragmentSize", 			Set_WscFragmentSize_Proc},
	{"WscMaxPinAttack", 			Set_WscMaxPinAttack_Proc},
	{"WscSetupLockTime", 			Set_WscSetupLockTime_Proc},
#endif /* WSC_V2_SUPPORT */
#ifdef WSC_NFC_SUPPORT
	{"NfcStatus",					Set_NfcStatus_Proc},
	{"NfcPasswdToken",				Set_NfcPasswdToken_Proc},
	{"NfcConfigToken",				Set_NfcConfigurationToken_Proc},
	{"DoWpsByNFC",					Set_DoWpsByNFC_Proc},
	{"NfcRegenPK",					Set_NfcRegenPK_Proc}, /* For NFC negative test */
#endif /* WSC_NFC_SUPPORT */
	{"WscAutoTriggerDisable", 		Set_WscAutoTriggerDisable_Proc},
#endif /* WSC_AP_SUPPORT */
#ifdef UAPSD_SUPPORT
	{"UAPSDCapable",				Set_UAPSD_Proc},
#endif /* UAPSD_SUPPORT */
#ifdef IGMP_SNOOP_SUPPORT
	{"IgmpSnEnable",				Set_IgmpSn_Enable_Proc},
	{"IgmpAdd",					Set_IgmpSn_AddEntry_Proc},
	{"IgmpDel",					Set_IgmpSn_DelEntry_Proc},
#endif /* IGMP_SNOOP_SUPPORT */
#ifdef CONFIG_AP_SUPPORT
#ifdef MCAST_RATE_SPECIFIC
	{"McastPhyMode",				Set_McastPhyMode},
	{"McastMcs",					Set_McastMcs},
#endif /* MCAST_RATE_SPECIFIC */
#endif /* CONFIG_AP_SUPPORT */
	{"FixedTxMode",                 Set_FixedTxMode_Proc},
#ifdef CONFIG_APSTA_MIXED_SUPPORT
	{"OpMode",					Set_OpMode_Proc},
#endif /* CONFIG_APSTA_MIXED_SUPPORT */

#ifdef TXBF_SUPPORT
#if (!defined(MT_MAC))
	{"TxBfTag",					Set_TxBfTag_Proc},
	{"ReadITxBf",				Set_ReadITxBf_Proc},
	{"WriteITxBf",				Set_WriteITxBf_Proc},
	{"StatITxBf",				Set_StatITxBf_Proc},
	{"ReadETxBf",				Set_ReadETxBf_Proc},
	{"WriteETxBf",				Set_WriteETxBf_Proc},
	{"StatETxBf",				Set_StatETxBf_Proc},
	{"ITxBfTimeout",			Set_ITxBfTimeout_Proc},
	{"ETxBfTimeout",			Set_ETxBfTimeout_Proc},
	{"ITxBfCal",				Set_ITxBfCal_Proc},
	{"ITxBfLnaCal",				Set_ITxBfLnaCal_Proc},
#endif /* MT76x2, MT_MAC */
	{"InvTxBfTag",				Set_InvTxBfTag_Proc},
	{"ITxBfDivCal",				Set_ITxBfDivCal_Proc},

	{"ITxBfEn",					Set_ITxBfEn_Proc},
	{"ETxBfEnCond",				Set_ETxBfEnCond_Proc},
	{"ETxBfCodebook",			Set_ETxBfCodebook_Proc},
	{"ETxBfCoefficient",		Set_ETxBfCoefficient_Proc},
	{"ETxBfGrouping",			Set_ETxBfGrouping_Proc},
	{"ETxBfNoncompress",		Set_ETxBfNoncompress_Proc},
	{"ETxBfIncapable",			Set_ETxBfIncapable_Proc},
	{"NoSndgCntThrd",			Set_NoSndgCntThrd_Proc},
	{"NdpSndgStreams",			Set_NdpSndgStreams_Proc},


#ifdef MT_MAC
    {"TxBfTxApply",             Set_TxBfTxApply},
	{"TriggerSounding",			Set_Trigger_Sounding_Proc},
    {"StopSounding",			Set_Stop_Sounding_Proc},
    {"StaRecBfUpdate",          Set_StaRecBfUpdate},
    {"StaRecBfRead",            Set_StaRecBfRead},
    {"TxBfTxPwrBackOff",        Set_TxBfTxPwrBackOff},
    {"TxBfAwareCtrl",           Set_TxBfAwareCtrl},
#ifdef CONFIG_ATE
    {"StaRecCmmUpdate",         Set_StaRecCmmUpdate},
    {"BssInfoUpdate",           Set_BssInfoUpdate},
    {"DevInfoUpdate",           Set_DevInfoUpdate},
    {"ManualAssoc",             SetATEAssocProc},
#endif /* CONFIG_ATE */
    {"TxBfPfmuMemAlloc",        Set_TxBfPfmuMemAlloc},
    {"TxBfPfmuMemRelease",      Set_TxBfPfmuMemRelease},
    {"TxBfPfmuMemAllocMapRead", Set_TxBfPfmuMemAllocMapRead},
    {"TxBfProfileTagHelp",      Set_TxBfProfileTag_Help},
    {"TxBfProfileTagInValid",   Set_TxBfProfileTag_InValid},
    {"TxBfProfileTagPfmuIdx",   Set_TxBfProfileTag_PfmuIdx},
    {"TxBfProfileTagBfType",    Set_TxBfProfileTag_BfType},
    {"TxBfProfileTagBw",        Set_TxBfProfileTag_DBW},
    {"TxBfProfileTagSuMu",      Set_TxBfProfileTag_SuMu},
    {"TxBfProfileTagMemAlloc",  Set_TxBfProfileTag_Mem},
    {"TxBfProfileTagMatrix",    Set_TxBfProfileTag_Matrix},
    {"TxBfProfileTagSnr",       Set_TxBfProfileTag_SNR},
    {"TxBfProfileTagSmtAnt",    Set_TxBfProfileTag_SmartAnt},
    {"TxBfProfileTagSeIdx",     Set_TxBfProfileTag_SeIdx},
    {"TxBfProfileTagRmsdThrd",  Set_TxBfProfileTag_RmsdThrd},
    {"TxBfProfileTagMcsThrd",   Set_TxBfProfileTag_McsThrd},
    {"TxBfProfileTagTimeOut",   Set_TxBfProfileTag_TimeOut},
    {"TxBfProfileTagDesiredBw", Set_TxBfProfileTag_DesiredBW},
    {"TxBfProfileTagDesiredNc", Set_TxBfProfileTag_DesiredNc},
    {"TxBfProfileTagDesiredNr", Set_TxBfProfileTag_DesiredNr},
    {"TxBfProfileTagRead",      Set_TxBfProfileTagRead},
    {"TxBfProfileTagWrite",     Set_TxBfProfileTagWrite},
    {"TxBfProfileDataRead",     Set_TxBfProfileDataRead},
    {"TxBfProfileDataWrite",    Set_TxBfProfileDataWrite},
    {"TxBfProfilePnRead",       Set_TxBfProfilePnRead},
    {"TxBfProfilePnWrite",      Set_TxBfProfilePnWrite},
#endif /* MT_MAC */

#endif /* TXBF_SUPPORT */
#ifdef VHT_TXBF_SUPPORT
	{"VhtNDPA",				    Set_VhtNDPA_Sounding_Proc},
#endif /* VHT_TXBF_SUPPORT */

#if defined (CONFIG_WIFI_PKT_FWD)
	{"wf_fwd",		Set_WifiFwd_Proc},
	{"wf_fwd_down",		Set_WifiFwd_Down},
	{"wf_fwd_acs",	Set_WifiFwdAccessSchedule_Proc},
	{"wf_fwd_hij",  	Set_WifiFwdHijack_Proc},
	{"wf_fwd_rep",	Set_WifiFwdRepDevice},
	{"wf_fwd_show",  	Set_WifiFwdShowEntry},
	{"wf_fwd_del",    	Set_WifiFwdDeleteEntry},
	{"pkt_src_show",   Set_PacketSourceShowEntry},
	{"pkt_src_del",   	Set_PacketSourceDeleteEntry},
#endif /* CONFIG_WIFI_PKT_FWD */


#ifdef MT_MAC
#ifdef DBG
	{"FixedRate",				Set_Fixed_Rate_Proc},
	{"FixedRateFallback",		Set_Fixed_Rate_With_FallBack_Proc},
#endif /* DBG */
#endif /* MT_MAC */

#ifdef PRE_ANT_SWITCH
	{"PreAntSwitch",		    Set_PreAntSwitch_Proc},
	{"PreAntSwitchRSSI",		Set_PreAntSwitchRSSI_Proc},
	{"PreAntSwitchTimeout",		Set_PreAntSwitchTimeout_Proc},
#endif /* PRE_ANT_SWITCH */

#ifdef CFO_TRACK
	{"CFOTrack",				Set_CFOTrack_Proc},
#endif /* CFO_TRACK */

#ifdef STREAM_MODE_SUPPORT
	{"StreamMode",				Set_StreamMode_Proc},
	{"StreamModeMac",			Set_StreamModeMac_Proc},
	{"StreamModeMCS",			Set_StreamModeMCS_Proc},
#endif /* STREAM_MODE_SUPPORT */

#ifdef DBG_CTRL_SUPPORT
	{"DebugFlags",				Set_DebugFlags_Proc},
#ifdef INCLUDE_DEBUG_QUEUE
	{"DebugQueue",				Set_DebugQueue_Proc},
#endif /* INCLUDE_DEBUG_QUEUE */
#endif /* DBG_CTRL_SUPPORT */

	{"LongRetry",	        		Set_LongRetryLimit_Proc},
	{"ShortRetry",	        		Set_ShortRetryLimit_Proc},
	{"AutoFallBack",	        	Set_AutoFallBack_Proc},
#ifdef RTMP_MAC_PCI
#ifdef DBG_DIAGNOSE
	{"DiagOpt",					    Set_DiagOpt_Proc},
	{"BDInfo",					    Set_BDInfo_Proc},
    {"diag_cond",                   Set_diag_cond_Proc},
#endif /* DBG_DIAGNOSE */
#endif /* RTMP_MAC_PCI */

	{"MeasureReq",					Set_MeasureReq_Proc},
	{"TpcReq",						Set_TpcReq_Proc},
	{"TpcReqByAddr",				Set_TpcReqByAddr_Proc},
	{"PwrConstraint",				Set_PwrConstraint},
#ifdef TPC_SUPPORT
	{"TpcCtrl",						Set_TpcCtrl_Proc},
	{"TpcEn", 						Set_TpcEnable_Proc},
#endif /* TPC_SUPPORT */
#ifdef DOT11K_RRM_SUPPORT
	{"BcnReq",						Set_BeaconReq_Proc},
	{"LinkReq",						Set_LinkMeasureReq_Proc},
	{"RrmEnable",					Set_Dot11kRRM_Enable_Proc},
	{"TxReq",						Set_TxStreamMeasureReq_Proc},


	/* only for selftesting and debugging. */
	{"rrm", 						Set_RRM_Selftest_Proc},
#endif /* DOT11K_RRM_SUPPORT */
#ifdef DOT11V_WNM_SUPPORT
/*	{"WnmMaxIdlePeriod",			Set_WnmMaxIdlePeriod_Proc}, */
	{"WNMTransMantREQ",				Set_WNMTransMantREQ_Proc},
	{"APWNMDMSShow",				Set_APWNMDMSShow_Proc},
#endif /* DOT11V_WNM_SUPPORT */
#ifdef DOT11N_DRAFT3
	{"OBSSScanParam",				Set_OBSSScanParam_Proc},
	{"AP2040Rescan",			    Set_AP2040ReScan_Proc},
	{"HtBssCoex",					Set_HT_BssCoex_Proc},
	{"HtBssCoexApCntThr",			Set_HT_BssCoexApCntThr_Proc},
#endif /* DOT11N_DRAFT3 */
	{"EntryLifeCheck",				Set_EntryLifeCheck_Proc},
#ifdef DOT11R_FT_SUPPORT
	{"ft",							FT_Ioctl},
	{"ftenable",					Set_FT_Enable},
	{"ftmdid",						Set_FT_Mdid},
	{"ftr0khid",					Set_FT_R0khid},
	{"ftric",						Set_FT_RIC},
	{"ftotd",						Set_FT_OTD},
#endif /* DOT11R_FT_SUPPORT */

#ifdef RTMP_EFUSE_SUPPORT
	{"efuseLoadFromBin",			set_eFuseLoadFromBin_Proc}, /* For backward compatible, the usage is the same as bufferLoadFromBin + bufferWriteBack */
	{"efuseFreeNumber",			    set_eFuseGetFreeBlockCount_Proc},
	{"efuseDump",				    set_eFusedump_Proc},
#ifdef CONFIG_ATE
	{"bufferLoadFromEfuse",		    Set_LoadEepromBufferFromEfuse_Proc},
#ifdef BB_SOC
	{"efuseBufferModeWriteBack",	set_BinModeWriteBack_Proc},
#else
	{"efuseBufferModeWriteBack",	set_eFuseBufferModeWriteBack_Proc}, /* For backward compatible, the usage is the same as bufferWriteBack */
#endif
#endif /* CONFIG_ATE */
#endif /* RTMP_EFUSE_SUPPORT */
	{"bufferLoadFromBin", 			Set_LoadEepromBufferFromBin_Proc},
	{"bufferWriteBack", 			Set_EepromBufferWriteBack_Proc},
	{"bufferMode", 			        Set_bufferMode_Proc},

#ifdef CAL_FREE_IC_SUPPORT
	{"bufferLoadFromCalFree", 		Set_LoadCalFreeData_Proc},
	{"CheckCalFree",		        Set_CheckCalFree_Proc},
#endif




#ifdef AP_QLOAD_SUPPORT
	{"qloadclr",					Set_QloadClr_Proc},
	{"qloadalarmtimethres",			Set_QloadAlarmTimeThreshold_Proc}, /* QLOAD ALARM */
	{"qloadalarmnumthres",			Set_QloadAlarmNumThreshold_Proc}, /* QLOAD ALARM */
#endif /* AP_QLOAD_SUPPORT */

	{"ra_interval",					Set_RateAdaptInterval},

#ifdef THERMAL_PROTECT_SUPPORT
	{"tpc",						    set_thermal_protection_criteria_proc},
	{"tpc_duty",					set_thermal_protection_admin_ctrl_duty_proc},
#endif /* THERMAL_PROTECT_SUPPORT */


	{"memdebug",					Set_MemDebug_Proc},

#ifdef CONFIG_AP_SUPPORT
	{"pslifetime",					Set_PowerSaveLifeTime_Proc},

#ifdef MBSS_SUPPORT
	{"MBSSWirelessMode",			Set_MBSS_WirelessMode_Proc},
#endif /* MBSS_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */


	{"VcoPeriod",					Set_VcoPeriod_Proc},

#ifdef SINGLE_SKU
	{"ModuleTxpower",				Set_ModuleTxpower_Proc},
#endif /* SINGLE_SKU */


#ifdef DOT11W_PMF_SUPPORT
    {"PMFMFPC",                     Set_PMFMFPC_Proc},
    {"PMFMFPR",                     Set_PMFMFPR_Proc},
    {"PMFSHA256",                   Set_PMFSHA256_Proc},
#endif /* DOT11W_PMF_SUPPORT */

#ifdef MICROWAVE_OVEN_SUPPORT
	{"MO_FalseCCATh",				Set_MO_FalseCCATh_Proc},
#endif /* MICROWAVE_OVEN_SUPPORT */

#ifdef CONFIG_FPGA_MODE
	{"fpga_on",					set_fpga_mode},		/* 1 = manual mode, 2 = fix phy/mode/rate/mcs */
	{"dataphy",					set_data_phy_mode},	/* 0 = CCK, 1 = OFDM, 2 = MODE_HTMIX, 3 = HT-GF, 4 = VHT */
	{"databw",					set_data_bw},			/* 0 = 20M, 1 = 40M, 2 = 80M */
	{"datamcs",					set_data_mcs},			/* 0~ 15 */
	{"databasize",				set_data_basize},
	{"datagi",					set_data_gi},
	{"dataldpc",				set_data_ldpc},
	{"txcnt",					set_tx_kickcnt},
	{"stop_tr",					set_tr_stop},
	{"vco_cal",					set_vco_cal},

#ifdef MT_MAC
	{"txs_type",				set_txs_report_type},
	{"no_bcn",					set_no_bcn},
#endif /* MT_MAC */

#ifdef CAPTURE_MODE
	{"cap_start",				set_cap_start},
	{"cap_trigger",				set_cap_trigger},
	{"cap_dump",				set_cap_dump},
#endif /* CAPTURE_MODE */
#endif /* CONFIG_FPGA_MODE */

#if defined(WFA_VHT_PF) || defined(MT7603_FPGA) || defined(MT7628_FPGA) || defined(MT7636_FPGA) || defined(MT7637_FPGA)
	{"force_amsdu",				set_force_amsdu},
#endif /* defined(WFA_VHT_PF) || defined(MT7603_FPGA) */

#ifdef WFA_VHT_PF
	{"nss_mcs_opt",				set_vht_nss_mcs_opt},	// SIGMA
	{"opt_md_notif_ie",			set_vht_opmode_notify_ie}, // SIGMA
	{"force_op",					set_force_operating_mode},
	{"force_noack",				set_force_noack},
	{"force_vht_sgi",				set_force_vht_sgi},
	{"force_vht_stbc",				set_force_vht_tx_stbc},
	{"ext_cca",					set_force_ext_cca},
#endif /* WFA_VHT_PF */
	{"rf",						SetRF},
	{"tssi_enable", set_tssi_enable},
#ifdef DYNAMIC_VGA_SUPPORT
	{"DyncVgaEnable", Set_DyncVgaEnable_Proc},
	{"fc_hth", set_false_cca_hi_th},
	{"fc_lth", set_false_cca_low_th},
#endif /* DYNAMIC_VGA_SUPPORT */
#ifdef MT_MAC
	{"TimEnable",				Set_AP_TimEnable_Proc},//only for development purpose!! iwpriv ra0 set
    {"TmrVer",                  setTmrVerProc},
    {"TmrEnable",				setTmrEnableProc},//0: disalbe, 1: initialiter, 2: responser.
    {"TmrCal",                  SetTmrCalProc},
#ifdef TMR_VERIFY
/* only for TMR test */
    {"sendTmrAct", Set_SendTmrAction_Proc},
    {"tmrRate", Set_TmrRate_Proc},
    {"SpeEn", Set_TmrSpeEn_Proc},
#endif

    /* support CR4 cmds */
#if defined(MT7615) 
	{"cr4_query",                   set_cr4_query},
	{"cr4_set",                     set_cr4_set},
	{"cr4_capability",              set_cr4_capability},
	{"cr4_debug",                   set_cr4_debug},
#endif

    {"recover_lmac", set_recover_lmac},

    {"dump_remap_cr",               dump_remap_cr_content},

    {"get_fid",     set_get_fid},
	{"fwlog", set_fw_log},
	{"fwset",                   set_fw_cmd},
	{"fwget",                   get_fw_cmd},

#ifdef FW_DUMP_SUPPORT
	{"fwdump_maxsize", set_fwdump_max_size},
	{"fwdump_path", set_fwdump_path},
	{"fwdump_print", fwdump_print},
#endif
    {"protect", set_manual_protect},
    {"rts_thld", set_rts_thld},
    {"rdg", set_manual_rdg},

#if defined(MT7615) || defined(MT7622)
    {"cca_en", set_cca_en},
#endif /* defined(MT7615) || defined(MT7622) */

#ifdef DBG
    {"DumpTime",                Set_AP_DumpTime_Proc},//only for development purpose!! iwpriv ra0 set
    {"BcnStateCtrl",            Set_BcnStateCtrl_Proc},//only for development purpose!! iwpriv ra0 set
    {"trigger_tim",             set_tim_update_proc},
    {"DumpStaRecHistory",       set_dump_sta_rec_history_proc},
    {"DumpRxSB",                set_dump_rx_ba_scoreboard_proc},
#endif
#ifdef CUT_THROUGH
	{"cut_through", 		Set_CutThrough_Proc},
#endif /* CUT_THROUGH */
#ifdef RTMP_MAC_PCI
	{"pse_dbg", SetPSEWatchDog_Proc},
	{"pdma_dbg", Set_PDMAWatchDog_Proc},
#endif
	{"get_thermal_sensor", Set_themal_sensor},
#ifdef CFG_SUPPORT_MU_MIMO
    {"hqa_mu_cal_init_mcs",         hqa_mu_cal_init_mcs},
    {"hqa_mu_cal_lq",               hqa_mu_cal_lq},
    {"hqa_su_cal_lq",               hqa_su_cal_lq},
    {"hqa_mu_set_snr_offset",       hqa_mu_set_snr_offset},
    {"hqa_mu_set_zero_nss",         hqa_mu_set_zero_nss},
    {"hqa_mu_set_speedup_lq",       hqa_mu_set_speedup_lq},
    {"hqa_mu_set_mu_tbl",           hqa_mu_set_mu_table},
    {"hqa_mu_set_group",            hqa_mu_set_group},
    {"hqa_mu_set_enable",           hqa_mu_set_enable},
    {"hqa_mu_set_gid_up",           hqa_mu_set_gid_up},
    {"hqa_mu_set_trigger_mu_tx",    hqa_mu_set_trigger_mu_tx},

    /* jeffrey, 20141116 */
    /* the followings are relative to MU debbuging/verification, and not use for ATE */
    {"set_mu_profile",              SetMuProfileProc},              // set MU profile
    {"set_mu_grouptbl",             SetGroupTblEntryProc},          // set group table entry
    {"set_mu_clustertbl",           SetClusterTblEntryProc},        // set cluster table entry
    {"set_mu_enable",               SetMuEnableProc},           // set MU enable or disable
    {"set_mu_groupuserthreshold",   SetGroupUserThresholdProc},     // set group threshold
    {"set_mu_groupnssthreshold",    SetGroupNssThresholdProc},      // set group NSS
    {"set_mu_txreqmintime",         SetTxReqMinTimeProc},           // set TX req min. time
    {"set_mu_calcinitmcs",          SetCalculateInitMCSProc},       // set calculate init MCS
    {"set_mu_sunsscheck",           SetSuNssCheckProc},             // set enable or disable NSS check
    {"set_mu_txopdefault",          SetTxopDefaultProc},            // set MU enable or disable
    {"set_mu_sulossthreshold",      SetSuLossThresholdProc},        // set SU loss threshold
    {"set_mu_mugainthreshold",      SetMuGainThresholdProc},        // set MU gain threshold
    {"set_mu_secondaryacpolicy",    SetSecondaryAcPolicyProc},      // set secondary AC policy
    {"set_mu_grouptbldmcsmask",     SetGroupTblDmcsMaskProc},       // set group table DMCS mask enable or disable
    {"set_mu_maxgroupsearchcnt",    SetMaxGroupSearchCntProc},      // set Max group search count
    /* the followings are relative to trigger MU-flow test command */
    {"set_mu_send_gid_mgmt_frame",  SetTriggerGIDMgmtFrameProc},    // set trigger GID mgmt. frame
    {"set_mu_trigger_mutx",    SetTriggerMuTxProc},     // set trigger MU TX
    {"set_mu_trigger_degroup",      SetTriggerDegroupProc},     // set trigger MU degrouping
    {"set_mu_trigger_group",        SetTriggerGroupProc},       // set trigger MU grouping
    {"set_mu_trigger_bbp",          SetTriggerBbpProc},         // set trigger LQ
    {"set_mu_trigger_sounding",     SetTriggerSndProc},         // set trigger MU sounding
        /* the followings are relative to channel model setting */
#ifdef PALLADIUM
#ifdef CFG_SUPPORT_DYN_CHANNEL_MODEL
        //TODO: jeffrey
        {"set_mu_channelmodel", SetMuChannelModelProc},               // set MU enable or disable
#endif
#endif
#endif

#ifdef VOW_SUPPORT
    /* VOW GROUP table */
    {"vow_min_rate_token",  set_vow_min_rate_token},
    {"vow_max_rate_token",  set_vow_max_rate_token},
    {"vow_min_airtime_token",  set_vow_min_airtime_token},
    {"vow_max_airtime_token",  set_vow_max_airtime_token},
    {"vow_min_rate_bucket",  set_vow_min_rate_bucket},
    {"vow_max_rate_bucket",  set_vow_max_rate_bucket},
    {"vow_min_airtime_bucket",  set_vow_min_airtime_bucket},
    {"vow_max_airtime_bucket",  set_vow_max_airtime_bucket},
    {"vow_max_wait_time", set_vow_max_wait_time},
    {"vow_max_backlog_size", set_vow_max_backlog_size},


    /* VOW CTRL */
    {"vow_bw_enable", set_vow_bw_en},
    {"vow_refill_en", set_vow_refill_en},
    {"vow_airtime_fairness_en", set_vow_airtime_fairness_en},
    {"vow_txop_switch_bss_en", set_vow_txop_switch_bss_en},
    {"vow_dbdc_search_rule", set_vow_dbdc_search_rule},
    {"vow_refill_period", set_vow_refill_period},
    {"vow_bss_enable", set_vow_bss_en},


    {"vow_airtime_control_en", set_vow_airtime_ctrl_en},
    {"vow_bw_control_en", set_vow_bw_ctrl_en},

    //group other
    {"vow_bss_dwrr_quantum", set_vow_bss_dwrr_quantum},
    {"vow_group_dwrr_max_wait_time", set_vow_group_dwrr_max_wait_time},
    {"vow_group2band_map", set_vow_group2band_map},

    /* VOW STA table */
    {"vow_sta_dwrr_quantum", set_vow_sta_dwrr_quantum},
    {"vow_sta_dwrr_quantum_id", set_vow_sta_dwrr_quantum_id},
    {"vow_sta_ac_priority", set_vow_sta_ac_priority},
    {"vow_sta_pause", set_vow_sta_pause},
    {"vow_sta_psm", set_vow_sta_psm},
    {"vow_sta_group", set_vow_sta_group},
    {"vow_dwrr_max_wait_time", set_vow_dwrr_max_wait_time},


    /* USER */
    {"vow_min_rate", set_vow_min_rate},
    {"vow_max_rate", set_vow_max_rate},
    {"vow_min_ratio", set_vow_min_ratio},
    {"vow_max_ratio", set_vow_max_ratio},

    /* RX airtime */
    {"vow_rx_counter_clr", set_vow_rx_counter_clr},
    {"vow_rx_airtime_en", set_vow_rx_airtime_en},
    {"vow_rx_ed_offset", set_vow_rx_ed_offset},
    {"vow_rx_obss_backoff", set_vow_rx_obss_backoff},
//    {"vow_rx_add_obss", set_vow_rx_add_obss},
//    {"vow_rx_add_non_wifi", set_vow_rx_add_non_wifi},
    {"vow_rx_wmm_backoff", set_vow_rx_wmm_backoff},
    {"vow_om_wmm_backoff", set_vow_rx_om_wmm_backoff},
    {"vow_repeater_wmm_backoff", set_vow_rx_repeater_wmm_backoff},
    {"vow_rx_non_qos_backoff",  set_vow_rx_non_qos_backoff},
    {"vow_rx_bss_wmmset", set_vow_rx_bss_wmmset},
    {"vow_rx_om_wmm_sel", set_vow_rx_om_wmm_select},

    /* airtime estimator */
    {"vow_at_est_en", set_vow_at_est_en},
    {"vow_at_mon_period", set_vow_at_mon_period},

    /* badnode detector */
    {"vow_bn_en", set_vow_bn_en},
    {"vow_bn_mon_period", set_vow_bn_mon_period},
    {"vow_bn_fallback_th", set_vow_bn_fallback_th},
    {"vow_bn_per_th", set_vow_bn_per_th},

 /* airtime counter test */
    {"vow_counter_test", set_vow_counter_test_en},
    {"vow_counter_test_period", set_vow_counter_test_period},
    {"vow_counter_test_band", set_vow_counter_test_band},
    {"vow_counter_test_avgcnt", set_vow_counter_test_avgcnt},
    {"vow_counter_test_target", set_vow_counter_test_target},

    /* DVT */
    {"vow_dvt_en", set_vow_dvt_en},
    {"vow_monitor_sta", set_vow_monitor_sta},
    {"vow_show_sta", set_vow_show_sta},
    {"vow_monitor_bss", set_vow_monitor_bss},
    {"vow_monitor_mbss", set_vow_monitor_mbss},
    {"vow_show_mbss", set_vow_show_mbss},
    {"vow_avg_num", set_vow_avg_num},

	/*WATF*/
    {"vow_watf_en", set_vow_watf_en},
    {"vow_watf_q", set_vow_watf_q},
    {"vow_watf_add_entry", set_vow_watf_add_entry},
    {"vow_watf_del_entry", set_vow_watf_del_entry},	


    /* help */
    {"vow_help", set_vow_help},

#if defined(MT7615_FPGA) || defined(MT7622_FPGA)
    {"vow_tx_en", set_vow_tx_en},
    {"vow_tx_en_all", set_vow_tx_en_all},

    {"vow_cloned_wtbl", set_vow_cloned_wtbl},

    {"vow_sta_mbss", set_vow_sta_mbss},

    {"vow_set_pkt_ac", set_vow_pkt_ac},

    {"vow_sta_wmm", set_vow_sta_wmm},
    {"vow_sta_wmm_all", set_vow_sta_wmm_all},

    {"vow_sta_ac_all", set_vow_sta_ac_all},
    {"vow_sta_ac", set_vow_sta_ac},

    {"vow_sta_ack", set_vow_sta_ack},
    {"vow_sta_ack_all", set_vow_sta_ack_all},

    {"vow_charge_sta_dwrr", set_vow_charge_sta_dwrr},
    {"vow_charge_bw_time", set_vow_charge_bw_time},
    {"vow_charge_bw_len", set_vow_charge_bw_len},
    {"vow_charge_bw_dwrr", set_vow_charge_bw_dwrr},

    {"vow_bcmc_en", set_vow_bcmc_en},
    {"vow_set_life_time", set_vow_life_time},

    /* CR4 commands */
    {"vow_set_sta_cnt", set_vow_sta_cnt},
    {"vow_set_sta_q", set_vow_sta_q},
    {"vow_set_sta2_q", set_vow_sta2_q},
    {"vow_set_sta_th", set_vow_sta_th},

    /*
    {"vow_rx_add_obss", set_vow_rx_add_obss},
    {"vow_rx_add_non_wifi", set_vow_rx_add_non_wifi},
    */
#endif /* defined(MT7615_FPGA) || defined(MT7622_FPGA) */
#endif /* VOW_SUPPORT */

#ifdef RED_SUPPORT
    {"vow_set_red_en", 				set_vow_red_en},
	{"vow_set_red_show_sta", 		set_vow_red_show_sta},
	{"vow_set_red_tar_delay", 		set_vow_red_target_delay},
#endif /* RED_SUPPORT */
	{"cp_support",					set_cp_support_en},
#ifdef CFG_SUPPORT_MU_MIMO_RA
    {"mura_periodic_sounding",      SetMuraPeriodicSndProc},         // set trigger MURGA Algorithm sounding
    {"mura_algorithm_test",         SetMuraTestAlgorithmProc},
    {"mura_algorithm_init",         SetMuraTestAlgorithmInit},
    {"mura_algorithm_fixed",        SetMuraFixedRateProc},
    {"mura_algorithm_fixed_group",  SetMuraFixedGroupRateProc},
    {"mura_sounding_fixed_param",   SetMuraFixedSndParamProc},
#endif

#endif /* MT_MAC */
#ifdef BACKGROUND_SCAN_SUPPORT
	{"bgndscan",               set_background_scan},
	{"bgndscantest",		set_background_scan_test},
	{"bgndscannotify",		set_background_scan_notify},
	{"bgndscancfg",               set_background_scan_cfg},
#endif /* BACKGROUND_SCAN_SUPPORT */
#ifdef NEW_SET_RX_STREAM
        {"RxStream",                        Set_RxStream_Proc},
#endif
#ifdef ERR_RECOVERY
    {"ErrDetectOn", Set_ErrDetectOn_Proc},
    {"ErrDetectMode", Set_ErrDetectMode_Proc},
#endif /* ERR_RECOVERY */
#ifdef CUT_THROUGH
	{"CtLowWaterMark",Set_CtLowWaterMark_Proc},
#endif /*CUT_THROUGH*/
#ifdef BAND_STEERING
	{"BndStrgEnable", 		Set_BndStrg_Enable},
	{"BndStrgRssiDiff", 	Set_BndStrg_RssiDiff},
	{"BndStrgRssiLow", 		Set_BndStrg_RssiLow},
	{"BndStrgAge", 			Set_BndStrg_Age},
	{"BndStrgHoldTime", 	Set_BndStrg_HoldTime},
	{"BndStrgCheckTime", 	Set_BndStrg_CheckTime5G},
	{"BndStrgCndChk", 	Set_BndStrg_CndChkFlag},
	{"BndStrgFrmChk", 	Set_BndStrg_FrmChkFlag},
#ifdef BND_STRG_DBG
	{"BndStrgMntAddr", 	Set_BndStrg_MonitorAddr},
#endif /* BND_STRG_DBG */
#endif /* BAND_STEERING */
#ifdef SMART_CARRIER_SENSE_SUPPORT
	{"SCSEnable", Set_SCSEnable_Proc},
	{"SCSCfg", Set_SCSCfg_Proc},
#endif /* SMART_CARRIER_SENSE_SUPPORT */
	{"SKUCtrl",	SetSKUCtrl},
	{"PercentageCtrl",	SetPercentageCtrl},
	{"BFBackoffCtrl",	    SetBfBackoffCtrl},
	{"PowerUppeBoundCtrl",	SetPowerUpperBoundCtrl},
    {"RFTxAnt",  SetRfTxAnt},
    {"TxPowerInfo",         SetTxPowerInfo},
    {"TOAECtrl",            SetTOAECtrl},
    {"EDCCACtrl",           SetEDCCACtrl},
    {"SKUInfo",             SetSKUInfo},
    {"BFBackoffInfo",       SetBFBackoffInfo},

#ifdef LED_CONTROL_SUPPORT
	{"led_setting", Set_Led_Proc},
#endif /* LED_CONTROL_SUPPORT */	
#ifdef CAL_TO_FLASH_SUPPORT
	{"ktoflash_debug", Set_KtoFlash_Debug_Proc},
	{"RDCE",Set_RDCE_Proc},
#endif /* CAL_TO_FLASH_SUPPORT */
    {"hw_nat_register", set_hnat_register},
    	{"MibBucket", Set_MibBucket_Proc},
#ifdef RLM_CAL_CACHE_SUPPORT
    {"rlm_cal_cache",  Set_RLM_Cal_Cache_Debug_Proc},
#endif /* RLM_CAL_CACHE_SUPPORT */
#ifdef PKT_BUDGET_CTRL_SUPPORT
	{"pbc_ubound",Set_PBC_Proc},
#endif /*PKT_BUDGET_CTRL_SUPPORT*/
	{"bwf", Set_BWF_Enable_Proc},
#ifdef TX_AGG_ADJUST_WKR
	{"agg_adj_wkr", Set_AggAdjWkr_Proc},
#endif /* TX_AGG_ADJUST_WKR */

#ifdef HTC_DECRYPT_IOT
	{"htc_th",		Set_HTC_Err_TH_Proc},
	{"htc_entry_err_cnt",		Set_Entry_HTC_Err_Cnt_Proc},	
	{"wtbl_addom",		Set_WTBL_AAD_OM_Proc},	
#endif /* HTC_DECRYPT_IOT */

#ifdef DHCP_UC_SUPPORT
	{"dhcp_uc",		Set_DHCP_UC_Proc},
#endif /* DHCP_UC_SUPPORT */
#ifdef CONFIG_HOTSPOT_R2
	{"hs_flag" ,Set_CR4_Hotspot_Flag},
#endif /* CONFIG_HOTSPOT_R2 */
	{NULL,}
};

/**
 * @endcode
 * @}
 */

/**
 * @addtogroup embedded_ioctl
 * @{
 * @code AP Show IOCTL
 */

static struct {
	RTMP_STRING *name;
	INT (*set_proc)(RTMP_ADAPTER *pAd, RTMP_STRING *arg);
} *PRTMP_PRIVATE_SHOW_PROC, RTMP_PRIVATE_SHOW_SUPPORT_PROC[] = {
	{"stainfo",			    Show_MacTable_Proc},
#ifdef MEM_ALLOC_INFO_SUPPORT
	{"meminfo",			Show_MemInfo_Proc},
	{"pktmeminfo",		Show_PktInfo_Proc},
#endif /* MEM_ALLOC_INFO_SUPPORT */
#ifdef MT_MAC
	{"psinfo",			Show_PSTable_Proc},
	{"wtbl",				show_wtbl_proc},
	{"wtbltlv",				show_wtbltlv_proc},
	{"mibinfo", show_mib_proc},
	{"wifi_sys", show_wifi_sys},
#ifdef DBDC_MODE
	{"dbdcinfo",		ShowDbdcProc},
#endif
	{"protectinfo", show_protect_info},
    {"txopinfo", show_tx_burst_info},
	{"tmacinfo", ShowTmacInfo},
	{"agginfo", ShowAggInfo},
    {"manual_txop", ShowManualTxOP},
	{"pseinfo", ShowPseInfo},
	{"psedata", ShowPseData},
#if defined(MT7615) || defined(MT7622)
	{"pleinfo",             ShowPLEInfo},
	{"txdinfo",             show_TXD_proc},
	{"dumpmem",             show_mem_proc},
    {"ccainfo", show_cca_info},
#endif /* defined(MT7615) || defined(MT7622) */
#ifdef CUT_THROUGH
	{"ctinfo",              ShowCutThroughInfo},
#endif /* CUT_THROUGH */
#ifdef DMA_SCH_SUPPORT
	{"dschinfo", ShowDMASchInfo},
#endif
#endif /* MT_MAC */
	{"sta_tr",				Show_sta_tr_proc},
	{"peerinfo", 			show_stainfo_proc},
	{"stacountinfo",			Show_StaCount_Proc},
	{"secinfo",			Show_APSecurityInfo_Proc},
	{"descinfo",			Show_DescInfo_Proc},
	{"driverinfo", 			Show_DriverInfo_Proc},
	{"devinfo",			show_devinfo_proc},
	{"sysinfo",			show_sysinfo_proc},
	{"trinfo",	show_trinfo_proc},
	{"tpinfo",	            show_tpinfo_proc},
	{"pwrinfo", show_pwr_info},
	{"txqinfo",			show_txqinfo_proc},
	{"efuseinfo",			    show_efuseinfo_proc},
	{"e2pinfo",			    show_e2pinfo_proc},
    {"cr4_pdma_probe",              dump_cr4_pdma_debug_probe},	
#ifdef WDS_SUPPORT
	{"wdsinfo",				Show_WdsTable_Proc},
#endif /* WDS_SUPPORT */
#ifdef DOT11_N_SUPPORT
	{"bainfo",				Show_BaTable_Proc},
	{"channelset",				Show_ChannelSet_Proc},
#endif /* DOT11_N_SUPPORT */
	{"stat",				Show_Sat_Proc},
#ifdef RTMP_MAC_PCI
#ifdef DBG_DIAGNOSE
	{"diag",				Show_Diag_Proc},
#endif /* DBG_DIAGNOSE */
#endif /* RTMP_MAC_PCI */
	{"stat_reset",			Show_Sat_Reset_Proc},
#ifdef IGMP_SNOOP_SUPPORT
	{"igmpinfo",			Set_IgmpSn_TabDisplay_Proc},
#endif /* IGMP_SNOOP_SUPPORT */
#ifdef MCAST_RATE_SPECIFIC
	{"mcastrate",			Show_McastRate},
#endif /* MCAST_RATE_SPECIFIC */
#ifdef MAT_SUPPORT
	{"matinfo",			Show_MATTable_Proc},
#endif /* MAT_SUPPORT */
#ifdef DFS_SUPPORT
	{"blockch", 			Show_BlockCh_Proc},
#endif /* DFS_SUPPORT */
#ifdef DOT11R_FT_SUPPORT
	{"ftinfo",				Show_FTConfig_Proc},
#endif /* DOT11R_FT_SUPPORT */
#ifdef DOT11K_RRM_SUPPORT
	{"rrminfo",				RRM_InfoDisplay_Proc},
#endif /* DOT11K_RRM_SUPPORT */
#ifdef AP_QLOAD_SUPPORT
	{"qload",				Show_QoSLoad_Proc},
#endif /* AP_QLOAD_SUPPORT */
	{"TmrCal",				Show_TmrCalResult_Proc},
#ifdef APCLI_SUPPORT
	{"connStatus",			RTMPIoctlConnStatus},
#endif /* APCLI_SUPPORT */
#ifdef MAC_REPEATER_SUPPORT
	{"reptinfo",			Show_Repeater_Cli_Proc},
	{"rept_table",			Show_ReptTable_Proc},
#endif /* MAC_REPEATER_SUPPORT */

	{"rainfo",				Show_RAInfo_Proc},

#ifdef MBSS_SUPPORT
	{"mbss",			Show_MbssInfo_Display_Proc},
#endif /* MBSS_SUPPORT */
#ifdef WSC_AP_SUPPORT
	{"WscPeerList", 		WscApShowPeerList},
	{"WscPin", 			WscApShowPin},
#ifdef WSC_NFC_SUPPORT
	{"NfcStatus", 			Get_NfcStatus_Proc},
#endif /* WSC_NFC_SUPPORT */
#endif /* WSC_AP_SUPPORT */
	{"rfinfo", ShowRFInfo},
	{"bbpinfo", ShowBBPInfo},
	{"wfintcnt",ShowWifiInterruptCntProc},         // show wifi interrupt counter
#ifdef CFG_SUPPORT_MU_MIMO
    {"hqa_mu_get_init_mcs",         hqa_mu_get_init_mcs},
    {"hqa_mu_get_qd",               hqa_mu_get_qd},
    {"hqa_mu_get_lq",               hqa_mu_get_lq},
    {"hqa_su_get_lq",               hqa_su_get_lq},

    {"get_mu_enable",               ShowMuEnableProc},                  // show mu enable or disable
    {"get_mu_profile",              ShowMuProfileProc},                 // show mu profile entry
    {"get_mu_grouptbl",             ShowGroupTblEntryProc},             // show group table entry
    {"get_mu_clustertbl",           ShowClusterTblEntryProc},           // show cluster table entry
    {"get_mu_groupuserthreshold",   ShowGroupUserThresholdProc},        // show group user threshold
    {"get_mu_groupnssthreshold",    ShowGroupNssThresholdProc},         // show group NSS threshold
    {"get_mu_txreqmintime",         ShowTxReqMinTimeProc},              // show tx req. min. time
    {"get_mu_sunsscheck",           ShowSuNssCheckProc},                // show SU Nss check enable or disable
    {"get_mu_calcinitmcs",          ShowCalcInitMCSProc},               // show Init MCS
    {"get_mu_txopdefault",          ShowTxopDefaultProc},               // show TXOP default
    {"get_mu_sulossthreshold",      ShowSuLossThresholdProc},           // show SU loss threshold
    {"get_mu_mugainthreshold",      ShowMuGainThresholdProc},           // show MU gain threshold
    {"get_mu_secondaryacpolicy",    ShowSecondaryAcPolicyProc},         // show secondary AC policay
    {"get_mu_grouptbldmcsmask",     ShowGroupTblDmcsMaskProc},          // show group table DMCS mask enable or disable
    {"get_mu_maxgroupsearchcnt",    ShowMaxGroupSearchCntProc},         // show max. group table search count
    {"get_mu_txstatus",             ShowMuProfileTxStsCntProc},         // show mu profile tx status
#ifdef PALLADIUM
#ifdef CFG_SUPPORT_DYN_CHANNEL_MODEL
    {"get_mu_getchannelmodel",      ShowChannelModelProc},              // show channel model setting
#endif
#endif
#endif

#ifdef CFG_SUPPORT_MU_MIMO_RA
    {"mura_algorithm_monitor",  GetMuraMonitorStateProc},
#endif

	{"hwctrl",Show_HwCtrlStatistic_Proc},

#ifdef VOW_SUPPORT
    /* VOW RX */
    {"vow_rx_time", show_vow_rx_time},
    //{"vow_get_sta_token", show_vow_get_sta_token},
    {"vow_sta_conf", show_vow_sta_conf},
    {"vow_all_sta_conf", show_vow_all_sta_conf},
    {"vow_bss_conf", show_vow_bss_conf},
    {"vow_all_bss_conf", show_vow_all_bss_conf},
	{"vow_info", show_vow_info},

    //{"vow_status", show_vow_status}

    /* CR dump */
    {"vow_dump_sta", show_vow_dump_sta},
    {"vow_dump_bss_bitmap", show_vow_dump_bss_bitmap},
    {"vow_dump_bss", show_vow_dump_bss},
    {"vow_dump_vow", show_vow_dump_vow},
    {"vow_show_sta_dtoken", vow_show_sta_dtoken},
    {"vow_show_bss_dtoken", vow_show_bss_dtoken},
    {"vow_show_bss_atoken", vow_show_bss_atoken},
    {"vow_show_bss_ltoken", vow_show_bss_ltoken},

	/*WATF*/
	{"vow_watf_info", show_vow_watf_info},

    /* help */
    {"vow_help", show_vow_help},
#endif /* VOW_SUPPORT */
#ifdef RED_SUPPORT
	{"vow_red_info", show_vow_red_info},
#endif/* RED_SUPPORT */
#ifdef BAND_STEERING
	{"BndStrgList", 		Show_BndStrg_List},
	{"BndStrgInfo", 		Show_BndStrg_Info},
#endif /* BAND_STEERING */
	{"timer_list",show_timer_list},
	{"wtbl_stat",show_wtbl_state},
#ifdef SMART_CARRIER_SENSE_SUPPORT
    {"SCSInfo", Show_SCSinfo_proc},
#endif /* SMART_CARRIER_SENSE_SUPPORT */	
	{"MibBucket",       Show_MibBucket_Proc},
    {"EDCCAStatus",     ShowEDCCAStatus},
	{"radio_info",show_radio_info_proc},
#ifdef BACKGROUND_SCAN_SUPPORT
    {"bgndscaninfo", show_background_scan_info},	
#endif 
    {NULL,}
};

/**
 * @addtogroup embedded_ioctl
 * @{
 * @code AP RX IOCTL
 */

static struct {
	RTMP_STRING *name;
	INT (*rx_proc)(RTMP_ADAPTER *pAd, RTMP_STRING *arg, RTMP_IOCTL_INPUT_STRUCT *wrq);
} *PRTMP_PRIVATE_RX_PROC, RTMP_PRIVATE_RX_SUPPORT_PROC[] = {
#if defined(MT7615) || defined(MT7622)
        {"stat",          Show_Rx_Statistic},
        {"vector",      Set_Rx_Vector_Control},
#endif/* defined(MT7615) || defined(MT7622) */        
        {NULL,}
};


INT RTMPAPPrivIoctlSet(
	IN RTMP_ADAPTER *pAd,
	IN RTMP_IOCTL_INPUT_STRUCT *pIoctlCmdStr)
{
	RTMP_STRING *this_char, *value;
	INT Status = NDIS_STATUS_SUCCESS;

	while ((this_char = strsep((char **)&pIoctlCmdStr->u.data.pointer, ",")) != NULL)
	{
		if (!*this_char)
			 continue;

		if ((value = strchr(this_char, '=')) != NULL)
			*value++ = 0;

		if (!value
#ifdef WSC_AP_SUPPORT
            && (
                 (strcmp(this_char, "WscStop") != 0) &&
#ifdef BB_SOC
		 (strcmp(this_char, "WscResetPinCode") != 0) &&
#endif
                 (strcmp(this_char, "WscGenPinCode")!= 0)
               )
#endif /* WSC_AP_SUPPORT */
            )
			continue;

		for (PRTMP_PRIVATE_SET_PROC = RTMP_PRIVATE_SUPPORT_PROC; PRTMP_PRIVATE_SET_PROC->name; PRTMP_PRIVATE_SET_PROC++)
		{
			if (!strcmp(this_char, PRTMP_PRIVATE_SET_PROC->name))
			{
				if(!PRTMP_PRIVATE_SET_PROC->set_proc(pAd, value))
				{   /*FALSE:Set private failed then return Invalid argument */
					Status = -EINVAL;
				}
				break;  /*Exit for loop. */
			}
		}

		if(PRTMP_PRIVATE_SET_PROC->name == NULL)
		{  /*Not found argument */
			Status = -EINVAL;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IOCTL::(iwpriv) Command not Support [%s=%s]\n", this_char, value));
			break;
		}
	}

	return Status;
}


INT RTMPAPPrivIoctlShow(
	IN RTMP_ADAPTER *pAd,
	IN RTMP_IOCTL_INPUT_STRUCT *pIoctlCmdStr)
{
	RTMP_STRING *this_char, *value = NULL;
	INT Status = NDIS_STATUS_SUCCESS;

	while ((this_char = strsep((char **)&pIoctlCmdStr->u.data.pointer, ",")) != NULL)
	{
		if (!*this_char)
			continue;

		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s(): Before check, this_char=%s\n", __FUNCTION__, this_char));
		value = strchr(this_char, '=');
		if (value) {
			if (strlen(value) > 1) {
				*value=0;
				value++;
			} else
				value = NULL;
		}
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s(): after check, this_char=%s, value=%s\n", __FUNCTION__, this_char, (value == NULL ? "" : value)));

		for (PRTMP_PRIVATE_SHOW_PROC = RTMP_PRIVATE_SHOW_SUPPORT_PROC; PRTMP_PRIVATE_SHOW_PROC->name; PRTMP_PRIVATE_SHOW_PROC++)
		{
			if (!strcmp(this_char, PRTMP_PRIVATE_SHOW_PROC->name))
			{
				if(!PRTMP_PRIVATE_SHOW_PROC->set_proc(pAd, value))
				{   /*FALSE:Set private failed then return Invalid argument */
					Status = -EINVAL;
				}
				break;  /*Exit for loop. */
			}
		}

		if(PRTMP_PRIVATE_SHOW_PROC->name == NULL)
		{  /*Not found argument */
			Status = -EINVAL;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IOCTL::(iwpriv) Command not Support [%s=%s]\n", this_char, value));
			break;
		}
	}

	return Status;

}

#if defined(INF_AR9) || defined(BB_SOC)
#if defined(AR9_MAPI_SUPPORT) || defined(BB_SOC)
INT RTMPAPPrivIoctlAR9Show(
	IN RTMP_ADAPTER *pAd,
	IN RTMP_IOCTL_INPUT_STRUCT *pIoctlCmdStr)
{
	INT Status = NDIS_STATUS_SUCCESS;

		if(!strcmp(pIoctlCmdStr->u.data.pointer, "get_mac_table"))
		{
			RTMPAR9IoctlGetMacTable(pAd,pIoctlCmdStr);
		}
		else if(!strcmp(pIoctlCmdStr->u.data.pointer, "get_stat2"))
		{
			RTMPIoctlGetSTAT2(pAd,pIoctlCmdStr);
		}
		else if(!strcmp(pIoctlCmdStr->u.data.pointer, "get_radio_dyn_info"))
		{
			RTMPIoctlGetRadioDynInfo(pAd,pIoctlCmdStr);
		}
#ifdef WSC_AP_SUPPORT
		else if(!strcmp(pIoctlCmdStr->u.data.pointer, "get_wsc_profile"))
		{
			RTMPAR9IoctlWscProfile(pAd,pIoctlCmdStr);
		}
		else if(!strcmp(pIoctlCmdStr->u.data.pointer, "get_wsc_pincode"))
		{
			RTMPIoctlWscPINCode(pAd,pIoctlCmdStr);
		}
		else if(!strcmp(pIoctlCmdStr->u.data.pointer, "get_wsc_status"))
		{
			RTMPIoctlWscStatus(pAd,pIoctlCmdStr);
		}
		else if(!strcmp(pIoctlCmdStr->u.data.pointer, "get_wps_dyn_info"))
		{
			RTMPIoctlGetWscDynInfo(pAd,pIoctlCmdStr);
		}
		else if(!strcmp(pIoctlCmdStr->u.data.pointer, "get_wps_regs_dyn_info"))
		{
			RTMPIoctlGetWscRegsDynInfo(pAd,pIoctlCmdStr);
		}
#endif
	return Status;
}
#endif /*AR9_MAPI_SUPPORT*/
#endif/*AR9_INF*/

INT RTMPAPSetInformation(
	IN PRTMP_ADAPTER pAd,
	INOUT RTMP_IOCTL_INPUT_STRUCT *rq,
	IN INT cmd)
{
	RTMP_IOCTL_INPUT_STRUCT *wrq = (RTMP_IOCTL_INPUT_STRUCT *) rq;
	UCHAR Addr[MAC_ADDR_LEN];
	INT Status = NDIS_STATUS_SUCCESS;

#ifdef SNMP_SUPPORT
	/*snmp */
    UINT						KeyIdx = 0;
    PNDIS_AP_802_11_KEY			pKey = NULL;
	TX_RTY_CFG_STRUC			tx_rty_cfg;
	ULONG						ShortRetryLimit, LongRetryLimit;
	UCHAR						ctmp;
#endif /* SNMP_SUPPORT */


 	NDIS_802_11_WEP_STATUS              WepStatus;
 	NDIS_802_11_AUTHENTICATION_MODE     AuthMode = Ndis802_11AuthModeMax;
	NDIS_802_11_SSID                    Ssid;

#ifdef HOSTAPD_SUPPORT
	MAC_TABLE_ENTRY						*pEntry;
	STA_TR_ENTRY *tr_entry;
	struct ieee80211req_mlme			mlme;

	struct ieee80211req_key				Key;
	struct ieee80211req_del_key			delkey;
	UINT8 Wcid;
	BSS_STRUCT *pMbss ;
	WSC_LV_INFO            WscIEBeacon;
   	WSC_LV_INFO            WscIEProbeResp;
	int i;
#endif /*HOSTAPD_SUPPORT*/

#ifdef APCLI_SUPPORT
#endif/*APCLI_SUPPORT*/


	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;

	switch(cmd & 0x7FFF)
	{
#ifdef APCLI_SUPPORT
#endif/*APCLI_SUPPORT*/

    
    	case OID_802_11_DEAUTHENTICATION:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_DEAUTHENTICATION\n"));
			if (wrq->u.data.length != sizeof(MLME_DEAUTH_REQ_STRUCT))
				Status  = -EINVAL;
			else
			{
				MAC_TABLE_ENTRY *pEntry = NULL;
				MLME_DEAUTH_REQ_STRUCT  *pInfo = NULL;
				MLME_QUEUE_ELEM *Elem;

				os_alloc_mem(pAd, (UCHAR **)&Elem, sizeof(MLME_QUEUE_ELEM));

				if(Elem == NULL)
				{
					Status = -ENOMEM;
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_DEAUTHENTICATION, Failed!!\n"));
					break;
				}

#ifdef APCLI_SUPPORT
#endif/*APCLI_SUPPORT*/
				{
					if (Elem)
					{
						pInfo = (MLME_DEAUTH_REQ_STRUCT *) Elem->Msg;
						Status = copy_from_user(pInfo, wrq->u.data.pointer, wrq->u.data.length);

						if ((pEntry = MacTableLookup(pAd, pInfo->Addr)) != NULL)
						{
							Elem->Wcid = pEntry->wcid;
							MlmeEnqueue(pAd, AP_AUTH_STATE_MACHINE, APMT2_MLME_DEAUTH_REQ,
											sizeof(MLME_DEAUTH_REQ_STRUCT), Elem, 0);
							MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_DEAUTHENTICATION (Reason=%d)\n", pInfo->Reason));
						}
						os_free_mem(Elem);
					}
					else
						Status = -EFAULT;
				}
			}

			break;
#ifdef IAPP_SUPPORT
    	case RT_SET_IAPP_PID:
			{
				unsigned long IappPid;
				if (sizeof(IappPid)!=wrq->u.data.length){
					Status = -EFAULT;
					break;
				}
				if (copy_from_user(&IappPid, wrq->u.data.pointer, wrq->u.data.length))
				{
					Status = -EFAULT;
				}
    			else
    			{
					RTMP_GET_OS_PID(pObj->IappPid, IappPid);
					pObj->IappPid_nr = IappPid;
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_SET_APD_PID::(IappPid=%lu)\n", IappPid));
				}
    		}
			break;
#endif /* IAPP_SUPPORT */

#ifdef DOT11R_FT_SUPPORT
		case RT_SET_FT_STATION_NOTIFY:
		case RT_SET_FT_KEY_REQ:
		case RT_SET_FT_KEY_RSP:
		case RT_FT_KEY_SET:
		case RT_FT_NEIGHBOR_REPORT:
		case RT_FT_NEIGHBOR_REQUEST:
		case RT_FT_NEIGHBOR_RESPONSE:
		case RT_FT_ACTION_FORWARD:
		{
			UCHAR *pBuffer;

			FT_MEM_ALLOC(pAd, &pBuffer, wrq->u.data.length+1);
			if (pBuffer == NULL)
				break;

			if (copy_from_user(pBuffer, wrq->u.data.pointer, wrq->u.data.length))
			{
				Status = -EFAULT;
				FT_MEM_FREE(pAd, pBuffer);
				break;
			}

			switch(cmd & 0x7FFF)
			{
				case RT_SET_FT_STATION_NOTIFY:
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_SET_FT_STATION_NOTIFY\n"));
					FT_KDP_StationInform(pAd, pBuffer, wrq->u.data.length);
					break;
				case RT_SET_FT_KEY_REQ:
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_SET_FT_KEY_REQ\n"));
					FT_KDP_IOCTL_KEY_REQ(pAd, pBuffer, wrq->u.data.length);
					break;
				case RT_SET_FT_KEY_RSP:
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_SET_FT_KEY_RSP\n"));
					FT_KDP_KeyResponseToUs(pAd, pBuffer, wrq->u.data.length);
					break;
				case RT_FT_KEY_SET:
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_FT_KEY_SET\n"));
					/* Note: the key must be ended by 0x00 */
					pBuffer[wrq->u.data.length] = 0x00;
					FT_KDP_CryptKeySet(pAd, pBuffer, wrq->u.data.length);
					break;
				case RT_FT_NEIGHBOR_REPORT:
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_FT_NEIGHBOR_REPORT\n"));
#ifdef FT_KDP_FUNC_INFO_BROADCAST
					FT_KDP_NeighborReportHandle(pAd, pBuffer, wrq->u.data.length);
#endif /* FT_KDP_FUNC_INFO_BROADCAST */
					break;
				case RT_FT_NEIGHBOR_REQUEST:
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_FT_NEIGHBOR_REPORT\n"));
					FT_KDP_NeighborRequestHandle(pAd, pBuffer, wrq->u.data.length);
					break;
				case RT_FT_NEIGHBOR_RESPONSE:
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_FT_NEIGHBOR_RESPONSE\n"));
					FT_KDP_NeighborResponseHandle(pAd, pBuffer, wrq->u.data.length);
					break;
				case RT_FT_ACTION_FORWARD:
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("[ra%d] RT_FT_ACTION_FORWARD\n", pObj->ioctl_if));
					FT_RRB_ActionHandle(pAd, pObj->ioctl_if, pBuffer, wrq->u.data.length);
					break;
			}

			FT_MEM_FREE(pAd, pBuffer);
		}
			break;

		case OID_802_11R_SUPPORT:
			if (wrq->u.data.length != sizeof(BOOLEAN))
				Status  = -EINVAL;
			else
			{
				UCHAR apidx = pObj->ioctl_if;
				ULONG value;
				Status = copy_from_user(&value, wrq->u.data.pointer, wrq->u.data.length);
				pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtCapFlag.Dot11rFtEnable = (value == 0 ? FALSE : TRUE);
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("Set::OID_802_11R_SUPPORT(=%d) \n",
							pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtCapFlag.Dot11rFtEnable));
			}
			break;

		case OID_802_11R_MDID:
			if (wrq->u.data.length != FT_MDID_LEN)
				Status  = -EINVAL;
			else
			{
				UCHAR apidx = pObj->ioctl_if;
				Status = copy_from_user(pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtMdId, wrq->u.data.pointer, wrq->u.data.length);
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("Set::OID_802_11R_MDID(=%c%c) \n",
							pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtMdId[0],
							pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtMdId[0]));
			}

			break;


		case OID_802_11R_R0KHID:
			if (wrq->u.data.length <= FT_ROKH_ID_LEN)
				Status  = -EINVAL;
			else
			{
				UCHAR apidx = pObj->ioctl_if;
				Status = copy_from_user(pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtR0khId, wrq->u.data.pointer, wrq->u.data.length);
				pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtR0khIdLen = wrq->u.data.length;
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("Set::OID_802_11R_OID_802_11R_R0KHID(=%s) Len=%d\n",
							pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtR0khId,
							pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtR0khIdLen));
			}
			break;

		case OID_802_11R_RIC:
			if (wrq->u.data.length != sizeof(BOOLEAN))
				Status  = -EINVAL;
			else
			{
				UCHAR apidx = pObj->ioctl_if;
				ULONG value;
				Status = copy_from_user(&value, wrq->u.data.pointer, wrq->u.data.length);
				pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtCapFlag.RsrReqCap = (value == 0 ? FALSE : TRUE);
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("Set::OID_802_11R_RIC(=%d) \n",
							pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtCapFlag.Dot11rFtEnable));
			}
			break;

		case OID_802_11R_OTD:
			if (wrq->u.data.length != sizeof(BOOLEAN))
				Status  = -EINVAL;
			else
			{
				UCHAR apidx = pObj->ioctl_if;
				ULONG value;
				Status = copy_from_user(&value, wrq->u.data.pointer, wrq->u.data.length);
				pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtCapFlag.FtOverDs = (value == 0 ? FALSE : TRUE);
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("Set::OID_802_11R_OTD(=%d) \n",
							pAd->ApCfg.MBSSID[apidx].wdev.FtCfg.FtCapFlag.Dot11rFtEnable));
			}
			break;
#endif /* DOT11R_FT_SUPPORT */
    	case RT_SET_APD_PID:
			{
				unsigned long apd_pid;
				if(sizeof(apd_pid)!=wrq->u.data.length){
					Status = -EFAULT;
					break;
				}
				if (copy_from_user(&apd_pid, wrq->u.data.pointer, wrq->u.data.length))
				{
					Status = -EFAULT;
				}
    			else
    			{
					RTMP_GET_OS_PID(pObj->apd_pid, apd_pid);
					pObj->apd_pid_nr = apd_pid;
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_SET_APD_PID::(ApdPid=%lu)\n", apd_pid));
				}
    		}
			break;
		case RT_SET_DEL_MAC_ENTRY:
				if(MAC_ADDR_LEN!=wrq->u.data.length){
					Status = -EFAULT;
					break;
				}
    		if (copy_from_user(Addr, wrq->u.data.pointer, wrq->u.data.length))
		{
				Status = -EFAULT;
		}
    		else
    		{
			MAC_TABLE_ENTRY *pEntry = NULL;

			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_SET_DEL_MAC_ENTRY::(%02x:%02x:%02x:%02x:%02x:%02x)\n", Addr[0],Addr[1],Addr[2],Addr[3],Addr[4],Addr[5]));

			pEntry = MacTableLookup(pAd, Addr);
			if (pEntry)
			{
#ifdef MAC_REPEATER_SUPPORT
					/*
						Need to delete repeater entry if this is mac repeater entry.
					*/
					if (pAd->ApCfg.bMACRepeaterEn)
					{
						UCHAR apCliIdx, CliIdx;
						REPEATER_CLIENT_ENTRY *pReptEntry = NULL;
						pReptEntry = RTMPLookupRepeaterCliEntry(pAd, TRUE, pEntry->Addr, TRUE);
					
						MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Delete (%02x:%02x:%02x:%02x:%02x:%02x) mac repeater entry\n", 
							Addr[0],Addr[1],Addr[2],Addr[3],Addr[4],Addr[5]));
						
						if (pReptEntry)
						{
							apCliIdx = pReptEntry->MatchApCliIdx;
							CliIdx = pReptEntry->MatchLinkIdx;
							pReptEntry->Disconnect_Sub_Reason = APCLI_DISCONNECT_SUB_REASON_APCFG_DEL_MAC_ENTRY;
							MlmeEnqueue(pAd, APCLI_CTRL_STATE_MACHINE, APCLI_CTRL_DISCONNECT_REQ, 0, NULL,
											(64 + MAX_EXT_MAC_ADDR_SIZE*apCliIdx + CliIdx));
							RTMP_MLME_HANDLER(pAd);
						}
						
					}
#endif /* MAC_REPEATER_SUPPORT */

#ifdef DOT11R_FT_SUPPORT
					/*
						If AP send de-auth to Apple STA,
						Apple STA will re-do auth/assoc and security handshaking with AP again.
						@20150313
					*/
					if (IS_FT_RSN_STA(pEntry))
					{
						MacTableDeleteEntry(pAd, pEntry->Aid, Addr);
					}
					else
#endif /* DOT11R_FT_SUPPORT */
				MlmeDeAuthAction(pAd, pEntry, REASON_DISASSOC_STA_LEAVING, FALSE);
			}
    		}
			break;
#ifdef WSC_AP_SUPPORT
#ifdef CON_WPS
                case RT_OID_WSC_SET_CON_WPS_STOP:
                {
                        UCHAR       apidx = pObj->ioctl_if;
                        PWSC_UPNP_CTRL_WSC_BAND_STOP pWscUpnpBandStop;
                        PWSC_CTRL pWpsCtrl = NULL;
                        INT         IsAPConfigured;                       

                        os_alloc_mem(NULL, (UCHAR **)&pWscUpnpBandStop, sizeof(WSC_UPNP_CTRL_WSC_BAND_STOP));

                        if(pWscUpnpBandStop)
                        {
                                Status = copy_from_user(pWscUpnpBandStop, wrq->u.data.pointer, wrq->u.data.length);
                                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("CON_WPS BAND_STOP_CMD From[%s], isApCli[%d], is2gBand[%d]\n",
                                        pWscUpnpBandStop->ifName, pWscUpnpBandStop->isApCli, pWscUpnpBandStop->is2gBand));

                                if (pWscUpnpBandStop->isApCli)
                                {
                                        
						UCHAR i;
						struct wifi_dev *apcli_wdev;
						for (i=0; i < MAX_APCLI_NUM; i++)
						{
	                                        	pWpsCtrl = &pAd->ApCfg.ApCliTab[i].WscControl;
							apcli_wdev = &pAd->ApCfg.ApCliTab[i].wdev;
	                                        	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("CON_WPS FROM IOCTL: Stop the %s WPS, state [%d]\n",
	                                                               apcli_wdev->if_dev->name, pWpsCtrl->WscState));

                                        if (pWpsCtrl->WscState != WSC_STATE_OFF)
                                        {
                                                WscStop(pAd, TRUE, pWpsCtrl);
                                                pWpsCtrl->WscConfMode = WSC_DISABLE;
                                        }

						}			
                                }
                                else
                                {
					struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
                                        pWpsCtrl = &pAd->ApCfg.MBSSID[apidx].WscControl;
                                        IsAPConfigured = pWpsCtrl->WscConfStatus;

                                        if ((pWpsCtrl->WscConfMode != WSC_DISABLE) &&
                                            (pWpsCtrl->bWscTrigger == TRUE))
                                        {
                                                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("FROM IOCTL CON_WPS[%d]: Stop the AP Wsc Machine\n", apidx));
                                                WscBuildBeaconIE(pAd, IsAPConfigured, FALSE, 0, 0, apidx, NULL, 0, AP_MODE);
                                                WscBuildProbeRespIE(pAd, WSC_MSGTYPE_AP_WLAN_MGR, IsAPConfigured, FALSE, 0, 0,
                                                                                                                apidx, NULL, 0, AP_MODE);
						UpdateBeaconHandler(pAd, wdev, IE_CHANGE);
                                                WscStop(pAd, FALSE, pWpsCtrl);
                                        }

                                }
                                os_free_mem(pWscUpnpBandStop);
                        }
                }
                break;
#endif /* CON_WPS */

		case RT_OID_WSC_SET_SELECTED_REGISTRAR:
			{
				PUCHAR      upnpInfo;
				UCHAR	    apidx = pObj->ioctl_if;

#ifdef HOSTAPD_SUPPORT
				if (pAd->ApCfg.MBSSID[apidx].Hostapd == Hostapd_EXT)
					{
						MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WPS is control by hostapd now.\n"));
						Status = -EINVAL;
					}
				else
				{
#endif /*HOSTAPD_SUPPORT*/
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WSC::RT_OID_WSC_SET_SELECTED_REGISTRAR, wrq->u.data.length=%d!\n", wrq->u.data.length));
				os_alloc_mem(pAd, (UCHAR **)&upnpInfo, wrq->u.data.length);
				if(upnpInfo)
				{
					int len, Status;

					Status = copy_from_user(upnpInfo, wrq->u.data.pointer, wrq->u.data.length);
					if (Status == NDIS_STATUS_SUCCESS)
					{
						len = wrq->u.data.length;

						if((pAd->ApCfg.MBSSID[apidx].WscControl.WscConfMode & WSC_PROXY))
						{
							WscSelectedRegistrar(pAd, upnpInfo, len, apidx);
							if (pAd->ApCfg.MBSSID[apidx].WscControl.Wsc2MinsTimerRunning == TRUE)
							{
								BOOLEAN Cancelled;
								RTMPCancelTimer(&pAd->ApCfg.MBSSID[apidx].WscControl.Wsc2MinsTimer, &Cancelled);
							}
							/* 2mins time-out timer */
							RTMPSetTimer(&pAd->ApCfg.MBSSID[apidx].WscControl.Wsc2MinsTimer, WSC_TWO_MINS_TIME_OUT);
							pAd->ApCfg.MBSSID[apidx].WscControl.Wsc2MinsTimerRunning = TRUE;
						}
					}
					os_free_mem( upnpInfo);
				}
				else
				{
					Status = -EINVAL;
				}
#ifdef HOSTAPD_SUPPORT
					}
#endif /*HOSTAPD_SUPPORT*/

			}
			break;
		case RT_OID_WSC_EAPMSG:
			{
				RTMP_WSC_U2KMSG_HDR *msgHdr = NULL;
				PUCHAR pUPnPMsg = NULL;
				UINT msgLen = 0, Machine = 0, msgType = 0;
				int retVal, senderID = 0;
#ifdef HOSTAPD_SUPPORT
				UCHAR	    apidx = pObj->ioctl_if;

				if (pAd->ApCfg.MBSSID[apidx].Hostapd == Hostapd_EXT)
					{
						MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WPS is control by hostapd now.\n"));
						Status = -EINVAL;
					}
				else
				{
#endif /*HOSTAPD_SUPPORT*/


				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WSC::RT_OID_WSC_EAPMSG, wrq->u.data.length=%d, ioctl_if=%d\n", wrq->u.data.length, pObj->ioctl_if));

				msgLen = wrq->u.data.length;
				os_alloc_mem(pAd, (UCHAR **)&pUPnPMsg, msgLen);
				if (pUPnPMsg == NULL)
					Status = -EINVAL;
				else
				{
					int HeaderLen;
					RTMP_STRING *pWpsMsg;
					UINT WpsMsgLen;
					PWSC_CTRL pWscControl;
					BOOLEAN	bGetDeviceInfo = FALSE;

					NdisZeroMemory(pUPnPMsg, msgLen);
					retVal = copy_from_user(pUPnPMsg, wrq->u.data.pointer, msgLen);

					msgHdr = (RTMP_WSC_U2KMSG_HDR *)pUPnPMsg;
					senderID = get_unaligned((INT32 *)(&msgHdr->Addr2[0]));
					/*senderID = *((int *)&msgHdr->Addr2); */

					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_OID_WSC_EAPMSG++++++++\n\n"));
					hex_dump("MAC::", &msgHdr->Addr3[0], MAC_ADDR_LEN);
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_OID_WSC_EAPMSG++++++++\n\n"));

					HeaderLen = LENGTH_802_11 + LENGTH_802_1_H + sizeof(IEEE8021X_FRAME) + sizeof(EAP_FRAME);
					pWpsMsg = (RTMP_STRING *)&pUPnPMsg[HeaderLen];
					WpsMsgLen = msgLen - HeaderLen;

					/*assign the STATE_MACHINE type */
					Machine = WSC_STATE_MACHINE;
					msgType = WSC_EAPOL_UPNP_MSG;

					pWscControl = &pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl;
					/* If AP is unconfigured, WPS state machine will be triggered after received M2. */
					if ((pWscControl->WscConfStatus == WSC_SCSTATE_UNCONFIGURED)
#ifdef WSC_V2_SUPPORT
						&& (pWscControl->WscV2Info.bWpsEnable || (pWscControl->WscV2Info.bEnableWpsV2 == FALSE))
#endif /* WSC_V2_SUPPORT */
						)
					{
						if (strstr(pWpsMsg, "SimpleConfig") &&
							!pWscControl->EapMsgRunning &&
							!pWscControl->WscUPnPNodeInfo.bUPnPInProgress)
						{
							/* GetDeviceInfo */
							WscInit(pAd, FALSE, pObj->ioctl_if);
							/* trigger wsc re-generate public key */
							pWscControl->RegData.ReComputePke = 1;
							bGetDeviceInfo = TRUE;
						}
						else if (WscRxMsgTypeFromUpnp(pAd, pWpsMsg, WpsMsgLen) == WSC_MSG_M2 &&
								!pWscControl->EapMsgRunning &&
								!pWscControl->WscUPnPNodeInfo.bUPnPInProgress)
						{
							/* Check Enrollee Nonce of M2 */
							if (WscCheckEnrolleeNonceFromUpnp(pAd, pWpsMsg, WpsMsgLen, pWscControl))
							{
								WscGetConfWithoutTrigger(pAd, pWscControl, TRUE);
								pWscControl->WscState = WSC_STATE_SENT_M1;
							}
						}
					}

					retVal = MlmeEnqueueForWsc(pAd, msgHdr->envID, senderID, Machine, msgType, msgLen, pUPnPMsg);
					if((retVal == FALSE) && (msgHdr->envID != 0))
					{
						MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("MlmeEnqueuForWsc return False and envID=0x%x!\n", msgHdr->envID));
						Status = -EINVAL;
					}

					os_free_mem( pUPnPMsg);
				}
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_OID_WSC_EAPMSG finished!\n"));
#ifdef HOSTAPD_SUPPORT
					}
#endif /*HOSTAPD_SUPPORT*/
			}
			break;

		case RT_OID_WSC_READ_UFD_FILE:
			if (wrq->u.data.length > 0)
			{
				RTMP_STRING *pWscUfdFileName = NULL;
				UCHAR apIdx = pObj->ioctl_if;
				WSC_CTRL *pWscCtrl = &pAd->ApCfg.MBSSID[apIdx].WscControl;
				UCHAR RfIC = wmode_2_rfic(pAd->ApCfg.MBSSID[apIdx].wdev.PhyMode);

				os_alloc_mem(pAd, (UCHAR **)&pWscUfdFileName, wrq->u.data.length+1);
				if (pWscUfdFileName)
				{
					RTMPZeroMemory(pWscUfdFileName, wrq->u.data.length+1);
					if (copy_from_user(pWscUfdFileName, wrq->u.data.pointer, wrq->u.data.length))
						Status = -EFAULT;
					else
					{
						MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_OID_WSC_READ_UFD_FILE (WscUfdFileName=%s)\n", pWscUfdFileName));
						if (pWscCtrl->WscConfStatus == WSC_SCSTATE_UNCONFIGURED)
						{
							if (WscReadProfileFromUfdFile(pAd, apIdx, pWscUfdFileName))
							{
								pWscCtrl->WscConfStatus = WSC_SCSTATE_CONFIGURED;
								APStopByRf(pAd, RfIC);
								APStartUpByRf(pAd, RfIC);
							}
						}
						else
						{
							MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_OID_WSC_READ_UFD_FILE: AP is configured.\n"));
							Status = -EINVAL;
						}
					}
					os_free_mem( pWscUfdFileName);
				}
				else
					Status = -ENOMEM;
			}
			else
				Status = -EINVAL;
			break;

		case RT_OID_WSC_WRITE_UFD_FILE:
			if (wrq->u.data.length > 0)
			{
				RTMP_STRING*pWscUfdFileName = NULL;
				UCHAR apIdx = pObj->ioctl_if;
				WSC_CTRL *pWscCtrl = &pAd->ApCfg.MBSSID[apIdx].WscControl;

				os_alloc_mem(pAd, (UCHAR **)&pWscUfdFileName, wrq->u.data.length+1);
				if (pWscUfdFileName)
				{
					RTMPZeroMemory(pWscUfdFileName, wrq->u.data.length+1);
					if (copy_from_user(pWscUfdFileName, wrq->u.data.pointer, wrq->u.data.length))
						Status = -EFAULT;
					else
					{
						MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_OID_WSC_WRITE_UFD_FILE (WscUfdFileName=%s)\n", pWscUfdFileName));
						if (pWscCtrl->WscConfStatus == WSC_SCSTATE_CONFIGURED)
						{
							WscWriteProfileToUfdFile(pAd, apIdx, pWscUfdFileName);
						}
						else
						{
							MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_OID_WSC_WRITE_UFD_FILE: AP is un-configured.\n"));
							Status = -EINVAL;
						}
					}
					os_free_mem( pWscUfdFileName);
				}
				else
					Status = -ENOMEM;
			}
			else
				Status = -EINVAL;
			break;

		case RT_OID_WSC_UUID:
			if (wrq->u.data.length == (UUID_LEN_STR-1))
			{
				UCHAR 		apIdx = pObj->ioctl_if;
				pAd->ApCfg.MBSSID[apIdx].WscControl.Wsc_Uuid_Str[0] = '\0';
				Status = copy_from_user(&pAd->ApCfg.MBSSID[apIdx].WscControl.Wsc_Uuid_Str[0],
										wrq->u.data.pointer,
										wrq->u.data.length);
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("UUID ASCII string: %s\n",
										pAd->ApCfg.MBSSID[apIdx].WscControl.Wsc_Uuid_Str));
			}
			else if (wrq->u.data.length == UUID_LEN_HEX)
			{
				UCHAR 		apIdx = pObj->ioctl_if, ii;
				Status = copy_from_user(&pAd->ApCfg.MBSSID[apIdx].WscControl.Wsc_Uuid_E[0],
										wrq->u.data.pointer,
										wrq->u.data.length);

				for (ii=0; ii< 16; ii++)
				{
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%02x", (pAd->ApCfg.MBSSID[apIdx].WscControl.Wsc_Uuid_E[ii] & 0xff)));
				}
			}
			else
				Status = -EINVAL;
			break;
#endif /* WSC_AP_SUPPORT */


#ifdef SNMP_SUPPORT
		case OID_802_11_SHORTRETRYLIMIT:
			if (wrq->u.data.length != sizeof(ULONG))
				Status = -EINVAL;
			else
			{
				Status = copy_from_user(&ShortRetryLimit, wrq->u.data.pointer, wrq->u.data.length);
				AsicSetRetryLimit(pAd, TX_RTY_CFG_RTY_LIMIT_SHORT, ShortRetryLimit);
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_SHORTRETRYLIMIT (ShortRetryLimit=%ld)\n", ShortRetryLimit));
			}
			break;

		case OID_802_11_LONGRETRYLIMIT:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_LONGRETRYLIMIT \n"));
			if (wrq->u.data.length != sizeof(ULONG))
				Status = -EINVAL;
			else
			{
				Status = copy_from_user(&LongRetryLimit, wrq->u.data.pointer, wrq->u.data.length);
				AsicSetRetryLimit(pAd, TX_RTY_CFG_RTY_LIMIT_LONG, LongRetryLimit);
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_LONGRETRYLIMIT (,LongRetryLimit=%ld)\n", LongRetryLimit));
			}
			break;

		case OID_802_11_WEPDEFAULTKEYVALUE:
		{
			UINT KeyIdx;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_WEPDEFAULTKEYVALUE\n"));

			os_alloc_mem(pAd, (UCHAR **)&pKey, wrq->u.data.length);
			if (pKey == NULL)
			{
				Status= -EINVAL;
				break;
			}
			Status = copy_from_user(pKey, wrq->u.data.pointer, wrq->u.data.length);
			/*pKey = &WepKey; */

			if ( pKey->Length != wrq->u.data.length)
			{
				Status = -EINVAL;
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_WEPDEFAULTKEYVALUE, Failed!!\n"));
			}
			KeyIdx = pKey->KeyIndex & 0x0fffffff;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("pKey->KeyIndex =%d, pKey->KeyLength=%d\n", pKey->KeyIndex, pKey->KeyLength));

			/* it is a shared key */
			if (KeyIdx > 4)
				Status = -EINVAL;
			else
			{
				pAd->SharedKey[pObj->ioctl_if][pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId].KeyLen = (UCHAR) pKey->KeyLength;
				NdisMoveMemory(&pAd->SharedKey[pObj->ioctl_if][pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId].Key, &pKey->KeyMaterial, pKey->KeyLength);
				if (pKey->KeyIndex & 0x80000000)
				{
					/* Default key for tx (shared key) */
					pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId = (UCHAR) KeyIdx;
				}
				/*RestartAPIsRequired = TRUE; */
			}
			os_free_mem( pKey);
			break;

		}
		case OID_802_11_WEPDEFAULTKEYID:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_WEPDEFAULTKEYID \n"));

			if (wrq->u.data.length != sizeof(UCHAR))
				Status = -EINVAL;
			else
				Status = copy_from_user(&pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId, wrq->u.data.pointer, wrq->u.data.length);

			break;


		case OID_802_11_CURRENTCHANNEL:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_CURRENTCHANNEL \n"));
			if (wrq->u.data.length != sizeof(UCHAR))
				Status = -EINVAL;
			else
			{
				RTMP_STRING ChStr[5] = {0};
				Status = copy_from_user(&ctmp, wrq->u.data.pointer, wrq->u.data.length);
				snprintf(ChStr, sizeof(ChStr), "%d", ctmp);
				Set_Channel_Proc(pAd, ChStr);
			}
			break;
#endif /* SNMP_SUPPORT */


#ifdef WAPI_SUPPORT
		case OID_802_11_WAPI_PID:
			{
				unsigned long wapi_pid;
    			if ((wrq->u.data.length <= sizeof(unsigned long)) && copy_from_user(&wapi_pid, wrq->u.data.pointer, wrq->u.data.length))
				{
					Status = -EFAULT;
				}
    			else
    			{
					RTMP_GET_OS_PID(pObj->wapi_pid, wapi_pid);
					pObj->wapi_pid_nr = wapi_pid;
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OID_802_11_WAPI_PID::(WapiPid=%lu(0x%x))\n", wapi_pid, (INT) pObj->wapi_pid));
				}
    		}
			break;

		case OID_802_11_PORT_SECURE_STATE:
			if (wrq->u.data.length != sizeof(WAPI_PORT_SECURE_STRUCT))
                Status  = -EINVAL;
            else
            {
				MAC_TABLE_ENTRY 		*pEntry = NULL;
				WAPI_PORT_SECURE_STRUCT  wapi_port;
				STA_TR_ENTRY *tr_entry;

				Status = copy_from_user(&wapi_port, wrq->u.data.pointer, wrq->u.data.length);
                if (Status == NDIS_STATUS_SUCCESS)
                {
					if ((pEntry = MacTableLookup(pAd, wapi_port.Addr)) != NULL)
					{
						tr_entry = &pAd->MacTab.tr_entry[pEntry->wcid];
						switch (wapi_port.state)
						{
							case WAPI_PORT_SECURED:
								tr_entry->PortSecured = WPA_802_1X_PORT_SECURED;
								pEntry->PrivacyFilter = Ndis802_11PrivFilterAcceptAll;
								break;

							default:
								tr_entry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;
								pEntry->PrivacyFilter = Ndis802_11PrivFilter8021xWEP;
								break;
						}
					}
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_PORT_SECURE_STATE (state=%d)\n", wapi_port.state));
				}
            }
			break;

		case OID_802_11_UCAST_KEY_INFO:
			if (wrq->u.data.length != sizeof(WAPI_UCAST_KEY_STRUCT))
                Status  = -EINVAL;
            else
            {
				MAC_TABLE_ENTRY 		*pEntry = NULL;
				WAPI_UCAST_KEY_STRUCT   wapi_ukey;

				Status = copy_from_user(&wapi_ukey, wrq->u.data.pointer, wrq->u.data.length);
                if (Status == NDIS_STATUS_SUCCESS)
                {
					if ((pEntry = MacTableLookup(pAd, wapi_ukey.Addr)) != NULL)
					{
						pEntry->usk_id = wapi_ukey.key_id;
						NdisMoveMemory(pEntry->SecConfig.PTK, wapi_ukey.PTK, 64);

						/* Install pairwise key */
						WAPIInstallPairwiseKey(pAd, pEntry, TRUE);

						/* Start or re-start USK rekey mechanism, if necessary. */
						RTMPCancelWapiRekeyTimerByMacEntry(pAd, pEntry);
						RTMPStartWapiRekeyTimerByMacEntry(pAd, pEntry);
					}
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_UCAST_KEY_INFO complete\n"));
					hex_dump("WAPI UCAST KEY", pEntry->SecConfig.PTK, 64);
				}
            }
			break;

#endif /* WAPI_SUPPORT */

#ifdef DOT1X_SUPPORT
		case OID_802_DOT1X_PMKID_CACHE:
			RTMPIoctlAddPMKIDCache(pAd, wrq);
			break;

		case OID_802_DOT1X_RADIUS_DATA:
			Dot1xIoctlRadiusData(pAd, wrq);
			break;

		case OID_802_DOT1X_WPA_KEY:
			Dot1xIoctlAddWPAKey(pAd, wrq);
			break;

		case OID_802_DOT1X_STATIC_WEP_COPY:
			Dot1xIoctlStaticWepCopy(pAd, wrq);
			break;

		case OID_802_DOT1X_IDLE_TIMEOUT:
			RTMPIoctlSetIdleTimeout(pAd, wrq);
			break;
#endif /* DOT1X_SUPPORT */

        case OID_802_11_AUTHENTICATION_MODE:
		{
			struct wifi_dev *wdev = NULL;
			UCHAR RfIC; 

            if (wrq->u.data.length != sizeof(NDIS_802_11_AUTHENTICATION_MODE))
                Status  = -EINVAL;
            else
            {
 	           UINT32 AKMMap = 0;

                Status = copy_from_user(&AuthMode, wrq->u.data.pointer, wrq->u.data.length);
                if (AuthMode > Ndis802_11AuthModeMax)
                {
                    Status  = -EINVAL;
                    break;
                }
		AKMMap = SecAuthModeOldToNew(AuthMode);

#ifdef APCLI_SUPPORT
#endif/*APCLI_SUPPORT*/
                {
					wdev = &pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev; 
                    if (wdev->SecConfig.AKMMap != AKMMap)
                    {
                        /* Config has changed */
                        pAd->bConfigChanged = TRUE;
                    }
                    wdev->SecConfig.AKMMap = AKMMap;
                }
                wdev->PortSecured = WPA_802_1X_PORT_NOT_SECURED;
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_AUTHENTICATION_MODE (=0x%x) \n", wdev->SecConfig.AKMMap));
            }
			
			if(wdev){
				RfIC = wmode_2_rfic(wdev->PhyMode);
				APStopByRf(pAd, RfIC);
				APStartUpByRf(pAd, RfIC);
            }
            break;
        }
        case OID_802_11_WEP_STATUS:
		{
            if (wrq->u.data.length != sizeof(NDIS_802_11_WEP_STATUS))
                Status  = -EINVAL;
            else
            {
	            UINT32 EncryType = 0;
                Status = copy_from_user(&WepStatus, wrq->u.data.pointer, wrq->u.data.length);

		EncryType = SecEncryModeOldToNew(WepStatus);

#ifdef APCLI_SUPPORT
#endif/*APCLI_SUPPORT*/
		{
			struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev;
			UCHAR RfIC = wmode_2_rfic(wdev->PhyMode);

		/* Since TKIP, AES, WEP are all supported. It should not have any invalid setting */
                if (EncryType != 0)
                {
			if (wdev->SecConfig.PairwiseCipher != EncryType)
			{
				/* Config has changed */
				pAd->bConfigChanged = TRUE;
			}
			wdev->SecConfig.PairwiseCipher = EncryType;

			if (IS_CIPHER_TKIP(EncryType) && IS_CIPHER_CCMP128(EncryType))
				SET_CIPHER_TKIP(wdev->SecConfig.GroupCipher);
			else
				wdev->SecConfig.GroupCipher = EncryType;
                }
                else
                {
                    Status  = -EINVAL;
                    break;
                }
			APStopByRf(pAd, RfIC);
			APStartUpByRf(pAd, RfIC);
                	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::OID_802_11_WEP_STATUS (=0x%x)\n",EncryType));
		}
	}

            break;
        }
        case OID_802_11_SSID:
            if (wrq->u.data.length != sizeof(NDIS_802_11_SSID))
                Status = -EINVAL;
            else
            {
            		RTMP_STRING *pSsidString = NULL;
                	Status = copy_from_user(&Ssid, wrq->u.data.pointer, wrq->u.data.length);

                	if (Ssid.SsidLength > MAX_LEN_OF_SSID)
                    		Status = -EINVAL;
                	else
                	{
                		if (Ssid.SsidLength == 0)
		    			{
                				Status = -EINVAL;
		    			}
				else
                    		{
						os_alloc_mem(NULL, (UCHAR **)&pSsidString, MAX_LEN_OF_SSID+1);
					if (pSsidString)
					{
						NdisZeroMemory(pSsidString, MAX_LEN_OF_SSID+1);
						NdisMoveMemory(pSsidString, Ssid.Ssid, Ssid.SsidLength);
#ifdef APCLI_SUPPORT
#endif/*APCLI_SUPPORT*/
						{
							NdisZeroMemory((PCHAR)pAd->ApCfg.MBSSID[pObj->ioctl_if].Ssid,MAX_LEN_OF_SSID);
							strcpy((PCHAR)pAd->ApCfg.MBSSID[pObj->ioctl_if].Ssid,pSsidString);
							pAd->ApCfg.MBSSID[pObj->ioctl_if].SsidLen=strlen(pSsidString);
						}
								os_free_mem( pSsidString);
					}
					else
						Status = -ENOMEM;
                    }
                }
            }
            break;

#ifdef HOSTAPD_SUPPORT
	case HOSTAPD_OID_SET_802_1X:/*pure 1x is enabled. */
			Set_IEEE8021X_Proc(pAd,"1");
			break;

	case HOSTAPD_OID_SET_KEY:
	{
			UINT KeyIdx;
			Status  = -EINVAL;
			pObj = (POS_COOKIE) pAd->OS_Cookie;
			UINT apidx = pObj->ioctl_if;
            struct wifi_dev *wdev = NULL;
			if(wrq->u.data.length != sizeof(struct ieee80211req_key) || !wrq->u.data.pointer)
				break;
			Status = copy_from_user(&Key, wrq->u.data.pointer, wrq->u.data.length);
			pEntry = MacTableLookup(pAd, Key.ik_macaddr);


			if((Key.ik_type == CIPHER_WEP64) ||(Key.ik_type == CIPHER_WEP128))/*dynamic wep with 1x */
			{
				if (pEntry)/*pairwise key */
				{
					pEntry->PairwiseKey.KeyLen = Key.ik_keylen;
					NdisMoveMemory(pEntry->PairwiseKey.Key, Key.ik_keydata, Key.ik_keylen);
					pEntry->PairwiseKey.CipherAlg = Key.ik_type;
					KeyIdx=pAd->ApCfg.MBSSID[pEntry->func_tb_idx].DefaultKeyId;
                                  AsicAddPairwiseKeyEntry(
                                      pAd,
                                      (UCHAR)pEntry->wcid,
                                      &pEntry->PairwiseKey);

					RTMPAddWcidAttributeEntry(
						pAd,
						pEntry->func_tb_idx,
						KeyIdx, /* The value may be not zero */
						pEntry->PairwiseKey.CipherAlg,
						pEntry);
				}
				else/*group key */
				{
					pMbss = &pAd->ApCfg.MBSSID[apidx];
					printk("Key.ik_keyix=%x\n",Key.ik_keyix);
					KeyIdx = Key.ik_keyix& 0x0fff;
					printk("ra%d KeyIdx=%d\n",apidx,KeyIdx);
					printk("Key.ik_keyix=%x\n",Key.ik_keyix);
					/* it is a shared key */
					if (KeyIdx < 4)
					{
						pAd->SharedKey[apidx][KeyIdx].KeyLen = (UCHAR) Key.ik_keylen;
						NdisMoveMemory(pAd->SharedKey[apidx][KeyIdx].Key, &Key.ik_keydata, Key.ik_keylen);
						if (Key.ik_keyix & 0x8000)
						{
							/* Default key for tx (shared key) */
							printk("ra%d DefaultKeyId=%d\n",apidx,KeyIdx);
							pMbss->DefaultKeyId = (UCHAR) KeyIdx;
						}
						/*pMbss->DefaultKeyId=1; */

						pAd->SharedKey[apidx][KeyIdx].CipherAlg = Key.ik_type;
						AsicAddSharedKeyEntry(
							pAd,
							apidx,
							KeyIdx,
						  	&pAd->SharedKey[apidx][KeyIdx]
						  	);

						RTMPAddWcidAttributeEntry(
							pAd,
							apidx,
							KeyIdx,
							pAd->SharedKey[apidx][pMbss->DefaultKeyId].CipherAlg,
							NULL);
					}
				}
			}
			else if (pEntry)
			{
				KeyIdx = Key.ik_keyix& 0x0fff;
				if (pEntry->WepStatus == Ndis802_11TKIPEnable)
				{
					pEntry->PairwiseKey.KeyLen = LEN_TK;
					NdisMoveMemory(&pEntry->PTK[OFFSET_OF_PTK_TK], Key.ik_keydata, Key.ik_keylen);
					NdisMoveMemory(pEntry->PairwiseKey.Key, &pEntry->PTK[OFFSET_OF_PTK_TK], Key.ik_keylen);
				}

				if(pEntry->WepStatus == Ndis802_11AESEnable)
				{
					pEntry->PairwiseKey.KeyLen = LEN_TK;
					NdisMoveMemory(&pEntry->PTK[OFFSET_OF_PTK_TK], Key.ik_keydata, OFFSET_OF_PTK_TK);
					NdisMoveMemory(pEntry->PairwiseKey.Key, &pEntry->PTK[OFFSET_OF_PTK_TK], Key.ik_keylen);
				}


    				pEntry->PairwiseKey.CipherAlg = CIPHER_NONE;
    				if (pEntry->WepStatus == Ndis802_11TKIPEnable)
        				pEntry->PairwiseKey.CipherAlg = CIPHER_TKIP;
    				else if (pEntry->WepStatus == Ndis802_11AESEnable)
        				pEntry->PairwiseKey.CipherAlg = CIPHER_AES;

				pEntry->PairwiseKey.CipherAlg = Key.ik_type;

                            AsicAddPairwiseKeyEntry(
                                pAd,
                                (UCHAR)pEntry->wcid,
                                &pEntry->PairwiseKey);

				RTMPSetWcidSecurityInfo(pAd,
					pEntry->func_tb_idx,
					(UINT8)KeyIdx,
					pEntry->PairwiseKey.CipherAlg,
					pEntry->wcid,
					PAIRWISEKEYTABLE);
			}
			else
			{
				pMbss = &pAd->ApCfg.MBSSID[apidx];
                wdev = &pMbss->wdev;
				KeyIdx = Key.ik_keyix& 0x0fff;

				/*if (Key.ik_keyix & 0x8000) */
				{
					pMbss->DefaultKeyId = (UCHAR) KeyIdx;
				}

				if (pAd->ApCfg.MBSSID[apidx].GroupKeyWepStatus == Ndis802_11TKIPEnable)
				{
					pAd->SharedKey[apidx][pMbss->DefaultKeyId].KeyLen= LEN_TK;
					NdisMoveMemory(pAd->SharedKey[apidx][pMbss->DefaultKeyId].Key, Key.ik_keydata, 16);
					NdisMoveMemory(pAd->SharedKey[apidx][pMbss->DefaultKeyId].RxMic, (Key.ik_keydata+16+8), 8);
					NdisMoveMemory(pAd->SharedKey[apidx][pMbss->DefaultKeyId].TxMic, (Key.ik_keydata+16), 8);
				}

				if(pAd->ApCfg.MBSSID[apidx].GroupKeyWepStatus == Ndis802_11AESEnable)
				{
					pAd->SharedKey[apidx][pMbss->DefaultKeyId].KeyLen= LEN_TK;
					NdisMoveMemory(pAd->SharedKey[apidx][pMbss->DefaultKeyId].Key, Key.ik_keydata, 16);
					NdisMoveMemory(pAd->SharedKey[apidx][pMbss->DefaultKeyId].RxMic, (Key.ik_keydata+16+8), 8);
					NdisMoveMemory(pAd->SharedKey[apidx][pMbss->DefaultKeyId].TxMic, (Key.ik_keydata+16), 8);
				}

    				pAd->SharedKey[apidx][pMbss->DefaultKeyId].CipherAlg  = CIPHER_NONE;
    				if (pAd->ApCfg.MBSSID[apidx].GroupKeyWepStatus == Ndis802_11TKIPEnable)
        				pAd->SharedKey[apidx][pMbss->DefaultKeyId].CipherAlg = CIPHER_TKIP;
    				else if (pAd->ApCfg.MBSSID[apidx].GroupKeyWepStatus == Ndis802_11AESEnable)
        				pAd->SharedKey[apidx][pMbss->DefaultKeyId].CipherAlg = CIPHER_AES;

				hex_dump("Key.ik_keydata,", (unsigned char*) Key.ik_keydata, 32);

				AsicAddSharedKeyEntry(
					pAd,
					apidx,
					pMbss->DefaultKeyId,
					&pAd->SharedKey[apidx][pMbss->DefaultKeyId]
					);
				GET_GroupKey_WCID(wdev, Wcid);

				RTMPSetWcidSecurityInfo(pAd, apidx,(UINT8)KeyIdx,
									pAd->SharedKey[apidx][pMbss->DefaultKeyId].CipherAlg, Wcid, SHAREDKEYTABLE);

				/*RTMPAddWcidAttributeEntry(
				pAd,
				apidx,
				pMbss->DefaultKeyId,
				pAd->SharedKey[apidx][pMbss->DefaultKeyId].CipherAlg,
				NULL);*/
			}
			break;
		}
		case HOSTAPD_OID_DEL_KEY:

			Status  = -EINVAL;
			if(wrq->u.data.length != sizeof(struct ieee80211req_del_key) || !wrq->u.data.pointer)
				break;
			Status = copy_from_user(&delkey, wrq->u.data.pointer, wrq->u.data.length);
			pEntry = MacTableLookup(pAd, delkey.idk_macaddr);
			if (pEntry){
				/* clear the previous Pairwise key table */
				if(pEntry->wcid != 0)
				{
					NdisZeroMemory(&pEntry->PairwiseKey, sizeof(CIPHER_KEY));
					AsicRemovePairwiseKeyEntry(pAd,(UCHAR)pEntry->wcid);
				}
			}
			else if((delkey.idk_macaddr == NULL) && (delkey.idk_keyix < 4))
				/* remove group key */
				AsicRemoveSharedKeyEntry(pAd, pEntry->func_tb_idx, delkey.idk_keyix);
			break;

		case HOSTAPD_OID_SET_STA_AUTHORIZED:/*for portsecured flag. */

			if (wrq->u.data.length != sizeof(struct ieee80211req_mlme))
			{
				Status  = -EINVAL;
			}
			else
			{
				Status = copy_from_user(&mlme, wrq->u.data.pointer, wrq->u.data.length);
				pEntry = MacTableLookup(pAd, mlme.im_macaddr);
				if (!pEntry){
					Status = -EINVAL;
				}
				else
				{
					tr_entry = &pAd->MacTab.tr_entry[pEntry->wcid];
					switch (mlme.im_op)
					{
						case IEEE80211_MLME_AUTHORIZE:
							pEntry->PrivacyFilter = Ndis802_11PrivFilterAcceptAll;
							pEntry->WpaState = AS_PTKINITDONE;/*wpa state machine is not in use. */
							/*pAd->StaCfg[0].PortSecured= WPA_802_1X_PORT_SECURED; */
							tr_entry->PortSecured = WPA_802_1X_PORT_SECURED;
							break;
						case IEEE80211_MLME_UNAUTHORIZE:
							pEntry->PrivacyFilter = Ndis802_11PrivFilter8021xWEP;
							tr_entry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;
						break;
						default:
							Status = -EINVAL;
					}
				}
			}
			break;

		case HOSTAPD_OID_STATIC_WEP_COPY:
		{
			UINT KeyIdx;
			INT	 apidx;
			if (wrq->u.data.length != sizeof(struct ieee80211req_mlme))
			{
				Status  = -EINVAL;
			}
			else
			{
				Status = copy_from_user(&mlme, wrq->u.data.pointer, wrq->u.data.length);
				pEntry = MacTableLookup(pAd, mlme.im_macaddr);
				if (!pEntry){
					Status = -EINVAL;
				}
				else{
					/*Status  = -EINVAL; */
					printk("Woody HOSTAPD_OID_STATIC_WEP_COPY IEEE8021X=%d WepStatus=%d\n",
							pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev.IEEE8021X, pEntry->WepStatus);
					if (pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev.IEEE8021X != TRUE)
						break;
					if (pEntry->WepStatus != Ndis802_11WEPEnabled)
						break;

					apidx = pObj->ioctl_if;
					pMbss = &pAd->ApCfg.MBSSID[apidx];

					KeyIdx=pMbss->DefaultKeyId;
					printk("Woody HOSTAPD_OID_STATIC_WEP_COPY=%d\n",KeyIdx);
					pEntry->AuthMode=pAd->ApCfg.MBSSID[apidx].AuthMode;
					pEntry->PairwiseKey.KeyLen = pAd->SharedKey[apidx][KeyIdx].KeyLen;
					NdisMoveMemory(pEntry->PairwiseKey.Key, pAd->SharedKey[apidx][KeyIdx].Key, pAd->SharedKey[apidx][KeyIdx].KeyLen);
					pEntry->PairwiseKey.CipherAlg = pAd->SharedKey[apidx][KeyIdx].CipherAlg;

                  	AsicAddPairwiseKeyEntry(
	                      pAd,
	                      (UCHAR)pEntry->wcid,
	                      &pEntry->PairwiseKey);

					RTMPAddWcidAttributeEntry(
							pAd,
							pEntry->func_tb_idx,
							KeyIdx, /* The value may be not zero */
							pEntry->PairwiseKey.CipherAlg,
							pEntry);

				}
			}
			break;
		}
		case HOSTAPD_OID_SET_STA_DEAUTH:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::HOSTAPD_OID_SET_STA_DEAUTH\n"));
			MLME_DEAUTH_REQ_STRUCT  *pInfo;
			MLME_QUEUE_ELEM *Elem;

			os_alloc_mem(NULL, (UCHAR **)&Elem, sizeof(MLME_QUEUE_ELEM));
			if(Elem)
			{
				pInfo = (MLME_DEAUTH_REQ_STRUCT *) Elem->Msg;

				if (wrq->u.data.length != sizeof(struct ieee80211req_mlme))
				{
					Status  = -EINVAL;
				}
				else
				{
					Status = copy_from_user(&mlme, wrq->u.data.pointer, wrq->u.data.length);
					NdisMoveMemory(pInfo->Addr, mlme.im_macaddr, MAC_ADDR_LEN);
					if ((pEntry = MacTableLookup(pAd, pInfo->Addr)) != NULL)
					{
						pInfo->Reason = mlme.im_reason;
						Elem->Wcid = pEntry->wcid;
						MlmeEnqueue(pAd, AP_AUTH_STATE_MACHINE, APMT2_MLME_DEAUTH_REQ, sizeof(MLME_DEAUTH_REQ_STRUCT), Elem,0);
					}
				}
				os_free_mem(Elem);
			}
			break;

		case HOSTAPD_OID_SET_STA_DISASSOC:/*hostapd request to disassoc the station. */
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::HOSTAPD_OID_SET_STA_DISASSOC\n"));
			MLME_DISASSOC_REQ_STRUCT DisassocReq;
			if (wrq->u.data.length != sizeof(struct ieee80211req_mlme))
			{
				Status  = -EINVAL;
			}
			else
			{
				Status = copy_from_user(&mlme, wrq->u.data.pointer, wrq->u.data.length);
				NdisMoveMemory(DisassocReq.Addr, mlme.im_macaddr, MAC_ADDR_LEN);
				DisassocReq.Reason = mlme.im_reason;
				MlmeEnqueue(pAd, AP_ASSOC_STATE_MACHINE, APMT2_MLME_DISASSOC_REQ, sizeof(MLME_DISASSOC_REQ_STRUCT), &DisassocReq,0);
			}
			break;

		case OID_HOSTAPD_SUPPORT:/*notify the driver to support hostapd. */
			if (wrq->u.data.length != sizeof(BOOLEAN))
				Status  = -EINVAL;
			else
			{
				BOOLEAN hostapd_enable;
				int v, apidx;
				apidx = pObj->ioctl_if;
				Status = copy_from_user(&hostapd_enable, wrq->u.data.pointer, wrq->u.data.length);
				printk("OID_HOSTAPD_SUPPORT apidx=%d\n",apidx);
				pAd->ApCfg.MBSSID[apidx].Hostapd = hostapd_enable;
				BSS_STRUCT *pMBSSStruct;

				for(v=0;v<MAX_MBSSID_NUM(pAd);v++)
				{
					printk("ApCfg->MBSSID[%d].Hostapd == %s\n", v,
							(pAd->ApCfg.MBSSID[v].Hostapd == Hostapd_EXT ? "TRUE" : "FALSE"));
					pMBSSStruct = &pAd->ApCfg.MBSSID[v];
					pMBSSStruct->WPAREKEY.ReKeyInterval = 0;
					pMBSSStruct->WPAREKEY.ReKeyMethod = DISABLE_REKEY;
				}
			}
		break;

		case HOSTAPD_OID_COUNTERMEASURES:/*report txtsc to hostapd. */

			if (wrq->u.data.length != sizeof(BOOLEAN))
				Status  = -EINVAL;
			else
			{
				BOOLEAN countermeasures_enable;
				Status = copy_from_user(&countermeasures_enable, wrq->u.data.pointer, wrq->u.data.length);

				if(countermeasures_enable)
				{

    						{
        						MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Receive CM Attack Twice within 60 seconds ====>>> \n"));

							/* send wireless event - for counter measures */
							pAd->ApCfg.CMTimerRunning = FALSE;

						        for (i = 0; VALID_UCAST_ENTRY_WCID(pAd, i); i++)
						        {
						        	MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[i];
						            /* happened twice within 60 sec,  AP SENDS disaccociate all associated STAs.  All STA's transition to State 2 */
						            if (IS_ENTRY_CLIENT(pEntry))
						            {
						                MlmeDeAuthAction(pAd, &pAd->MacTab.Content[i], REASON_MIC_FAILURE,FALSE);
						            }
						        }

						        /* Further,  ban all Class 3 DATA transportation for a period of 60 sec */
						        /* disallow new association , too */
						        pAd->ApCfg.BANClass3Data = TRUE;
						    }


				}
				else
				{
    					    pAd->ApCfg.BANClass3Data = FALSE;
				}
			}
			break;

	case HOSTAPD_OID_SET_WPS_BEACON_IE:/*pure 1x is enabled. */
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("HOSTAPD_OID_SET_WPS_BEACON_IE\n"));
				if (wrq->u.data.length != sizeof(WSC_LV_INFO))
				{
					Status  = -EINVAL;
				}
				else
				{
					INT apidx;
					apidx = pObj->ioctl_if;
					pAd->ApCfg.MBSSID[apidx].HostapdWPS = TRUE;
					BSS_STRUCT *pMBSSStruct;
					NdisZeroMemory(&WscIEBeacon,sizeof(WSC_LV_INFO));
					Status = copy_from_user(&WscIEBeacon, wrq->u.data.pointer, wrq->u.data.length);
					pMBSSStruct = &pAd->ApCfg.MBSSID[apidx];
					NdisMoveMemory(pMBSSStruct->WscIEBeacon.Value,WscIEBeacon.Value, WscIEBeacon.ValueLen);
					pMBSSStruct->WscIEBeacon.ValueLen=WscIEBeacon.ValueLen;
                    UpdateBeaconHandler(
                        pAd,
                        wdev,
                        IE_CHANGE);
				}

			break;

	case HOSTAPD_OID_SET_WPS_PROBE_RESP_IE:/*pure 1x is enabled. */
				apidx = pObj->ioctl_if;
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("HOSTAPD_OID_SET_WPS_PROBE_RESP_IE\n"));
				if (wrq->u.data.length != sizeof(WSC_LV_INFO))
				{
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("HOSTAPD_OID_SET_WPS_PROBE_RESP_IE failed\n"));
					Status  = -EINVAL;
				}
				else
				{
					INT apidx;
                    struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
					apidx = pObj->ioctl_if;
					pAd->ApCfg.MBSSID[apidx].HostapdWPS = TRUE;
					BSS_STRUCT *pMBSSStruct;
					NdisZeroMemory(&WscIEProbeResp,sizeof(WSC_LV_INFO));
					Status = copy_from_user(&WscIEProbeResp, wrq->u.data.pointer, wrq->u.data.length);
					pMBSSStruct = &pAd->ApCfg.MBSSID[apidx];
					NdisMoveMemory(pMBSSStruct->WscIEProbeResp.Value,WscIEProbeResp.Value, WscIEProbeResp.ValueLen);
					pMBSSStruct->WscIEProbeResp.ValueLen=WscIEProbeResp.ValueLen;
					UpdateBeaconHandler(
                            pAd,
                            wdev,
                            IE_CHANGE);

				}
			break;
#endif /*HOSTAPD_SUPPORT*/

#ifdef CONFIG_HOTSPOT
	case OID_802_11_HS_TEST:
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("hotspot test\n"));
		break;
	case OID_802_11_HS_IE:
		{
			UCHAR *IE;
			os_alloc_mem(NULL, (UCHAR **)&IE, wrq->u.data.length);
			copy_from_user(IE, wrq->u.data.pointer, wrq->u.data.length);
			Set_AP_IE(pAd, IE, wrq->u.data.length);
			os_free_mem( IE);
		}
		break;
	case OID_802_11_HS_ANQP_RSP:
		{
			UCHAR *Buf;
			struct anqp_rsp_data *rsp_data;
			os_alloc_mem(NULL, (UCHAR **)&Buf, wrq->u.data.length);
			copy_from_user(Buf, wrq->u.data.pointer, wrq->u.data.length);
			rsp_data = (struct anqp_rsp_data *)Buf;
			Send_ANQP_Rsp(pAd,
				      rsp_data->peer_mac_addr,
				      rsp_data->anqp_rsp,
				      rsp_data->anqp_rsp_len);
			os_free_mem( Buf);
		}
		break;
	case OID_802_11_HS_ONOFF:
		{
			UCHAR *Buf;
			struct hs_onoff *onoff;
			os_alloc_mem(NULL, (UCHAR **)&Buf, wrq->u.data.length);
			copy_from_user(Buf, wrq->u.data.pointer, wrq->u.data.length);
			onoff = (struct hs_onoff *)Buf;
			Set_HotSpot_OnOff(pAd, onoff->hs_onoff, onoff->event_trigger, onoff->event_type);
			os_free_mem( Buf);
		}
		break;
	case OID_802_11_HS_PARAM_SETTING:
		{
			UCHAR *Buf;
			struct hs_param_setting *param_setting;
			os_alloc_mem(NULL, (UCHAR **)&Buf, wrq->u.data.length);
			copy_from_user(Buf, wrq->u.data.pointer, wrq->u.data.length);
			param_setting = (struct hs_param_setting *)Buf;
			Set_HotSpot_Param(pAd, param_setting->param, param_setting->value);
			os_free_mem( Buf);
		}
		break;
	case OID_802_11_HS_RESET_RESOURCE:
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("hotspot reset some resource\n"));
		Clear_Hotspot_All_IE(pAd);
		//Clear_All_PROXY_TABLE(pAd);
		break;	
	
#ifdef CONFIG_HOTSPOT_R2
	case OID_802_11_HS_INTERWORKING_ENABLE:
		{
			UCHAR *Buf;			
			POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	        UCHAR apidx = pObj->ioctl_if;
			PGAS_CTRL pGASCtrl = &pAd->ApCfg.MBSSID[apidx].GASCtrl;
			os_alloc_mem(NULL, (UCHAR **)&Buf, wrq->u.data.length);
			copy_from_user(Buf, wrq->u.data.pointer, wrq->u.data.length);
			pGASCtrl->b11U_enable = Buf[0];
			os_free_mem(Buf);
			//printk("\033[1;36m %s, %u BSS[%d] ,11U_enable:%d\033[0m\n", __FUNCTION__, __LINE__,apidx,pGASCtrl->b11U_enable);  /* Kyle Debug Print */
			MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("BSS[%d] ,11U_enable:%d\n",apidx,pGASCtrl->b11U_enable));
		}
		break;
	case OID_802_11_HS_SASN_ENABLE:
		{
			UCHAR *Buf;
			PHOTSPOT_CTRL pHSCtrl =  &pAd->ApCfg.MBSSID[pObj->ioctl_if].HotSpotCtrl;
			os_alloc_mem(NULL, (UCHAR **)&Buf, wrq->u.data.length);
			copy_from_user(Buf, wrq->u.data.pointer, wrq->u.data.length);
			pHSCtrl->bASANEnable = Buf[0];
			/* for 7615 offload to CR4 */
			hotspot_update_bssflag(pAd,fgASANEnable,Buf[0],pHSCtrl);
			hotspot_update_bss_info_to_cr4(pAd,pObj->ioctl_if);
			
			os_free_mem( Buf);
		}
		break;
	case OID_802_11_BSS_LOAD:
		{
			UCHAR *Buf;
			PHOTSPOT_CTRL pHSCtrl =  &pAd->ApCfg.MBSSID[pObj->ioctl_if].HotSpotCtrl;
			os_alloc_mem(NULL, (UCHAR **)&Buf, wrq->u.data.length);
			copy_from_user(Buf, wrq->u.data.pointer, wrq->u.data.length);
			pHSCtrl->QLoadTestEnable = Buf[0];
			pHSCtrl->QLoadCU = Buf[1];
			memcpy(&pHSCtrl->QLoadStaCnt, &Buf[2], 2);
			os_free_mem( Buf);
		}
		break;
#endif
#endif

#ifdef CONFIG_DOT11V_WNM
#ifdef CONFIG_HOTSPOT_R2
	case OID_802_11_WNM_BTM_REQ:
		{
			UCHAR *Buf;
			MAC_TABLE_ENTRY *pEntry = NULL;
			struct btm_req_data *req_data;
			os_alloc_mem(Buf, (UCHAR **)&Buf, wrq->u.data.length);
			copy_from_user(Buf, wrq->u.data.pointer, wrq->u.data.length);
			req_data = (struct btm_req_data *)Buf;

			if ((pEntry = MacTableLookup(pAd, req_data->peer_mac_addr)) == NULL)
			{ 				
			}
			else if (pEntry->BssTransitionManmtSupport != 1)
			{
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
					("OID_802_11_WNM_BTM_REQ::the peer does not support BTM(%d)\n", 
					pEntry->BssTransitionManmtSupport));
			}
			else if (IS_AKM_OPEN(pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev.SecConfig.AKMMap) ||
				((pEntry->SecConfig.Handshake.WpaState == AS_PTKINITDONE) &&
				(pEntry->SecConfig.Handshake.GTKState == REKEY_ESTABLISHED)))				
			{			
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("btm1\n"));
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
					("peer_mac_addr=%02x:%02x:%02x:%02x:%02x:%02x\n", 
					req_data->peer_mac_addr[0],
					req_data->peer_mac_addr[1],
					req_data->peer_mac_addr[2],
					req_data->peer_mac_addr[3],
					req_data->peer_mac_addr[4],
					req_data->peer_mac_addr[5]));
							
				Send_BTM_Req(pAd,
				     req_data->peer_mac_addr,
				     req_data->btm_req,
				     req_data->btm_req_len);
			}
			else
			{
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!btm2\n"));
				pEntry->IsBTMReqValid = TRUE;
				os_alloc_mem(pEntry->ReqbtmData, (UCHAR **)&pEntry->ReqbtmData, sizeof(struct btm_req_data)+req_data->btm_req_len); 
				memcpy(pEntry->ReqbtmData, Buf, sizeof(struct btm_req_data)+req_data->btm_req_len);
			}
			
			os_free_mem(Buf); 
		}
		break;
	case OID_802_11_WNM_NOTIFY_REQ:
		{
			UCHAR *Buf;
			MAC_TABLE_ENTRY  *pEntry;
			struct wnm_req_data *req_data;
			PHOTSPOT_CTRL pHSCtrl =  &pAd->ApCfg.MBSSID[pObj->ioctl_if].HotSpotCtrl;
			os_alloc_mem(Buf, (UCHAR **)&Buf, wrq->u.data.length);
			copy_from_user(Buf, wrq->u.data.pointer, wrq->u.data.length);

			req_data = (struct wnm_req_data *)Buf;
			//printk("addr=%02x:%02x:%02x:%02x:%02x:%02x\n", req_data->peer_mac_addr[0],req_data->peer_mac_addr[1],req_data->peer_mac_addr[2],req_data->peer_mac_addr[3],req_data->peer_mac_addr[4],req_data->peer_mac_addr[5]);
			//for(k=0;k<req_data->wnm_req_len;k++)
			//	printk("%02x:", *(req_data->wnm_req+k));

			//printk("req len=%d\n",req_data->wnm_req_len);
			if ((pEntry = MacTableLookup(pAd, req_data->peer_mac_addr)) == NULL)
    		{
    		}
			else if (((pHSCtrl->HotSpotEnable) && IS_AKM_OPEN(pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev.SecConfig.AKMMap)) ||
				((pEntry->SecConfig.Handshake.WpaState == AS_PTKINITDONE) && (pEntry->SecConfig.Handshake.GTKState = REKEY_ESTABLISHED)))
    		{
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,("wnm1\n"));
				Send_WNM_Notify_Req(pAd,
				     req_data->peer_mac_addr,
				     req_data->wnm_req,
				     req_data->wnm_req_len,
				     req_data->type);
			}
			else
			{
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,("wnm2\n"));
				pEntry->IsWNMReqValid = TRUE;
				os_alloc_mem(pEntry->ReqData, (UCHAR **)&pEntry->ReqData, sizeof(struct wnm_req_data)+req_data->wnm_req_len);
				memcpy(pEntry->ReqData, Buf, sizeof(struct wnm_req_data)+req_data->wnm_req_len);
			}

			os_free_mem( Buf);
		}
		break;
	case OID_802_11_QOSMAP_CONFIGURE:
		{
			UCHAR *Buf;
			MAC_TABLE_ENTRY  *pEntry;
			struct qosmap_data *req_data;
			unsigned int i;
			UCHAR PoolID = 0;

			os_alloc_mem(Buf, (UCHAR **)&Buf, wrq->u.data.length);
			copy_from_user(Buf, wrq->u.data.pointer, wrq->u.data.length);

			req_data = (struct qosmap_data *)Buf;

			if ((pEntry = MacTableLookup(pAd, req_data->peer_mac_addr)) != NULL)
    		{
    			//clear previous data
    			pEntry->DscpExceptionCount = 0;
 				memset(pEntry->DscpRange, 0xff, 16);
 				memset(pEntry->DscpException, 0xff, 42);

    			pEntry->DscpExceptionCount = req_data->qosmap_len-16;
 				memcpy((UCHAR *)pEntry->DscpRange, &req_data->qosmap[pEntry->DscpExceptionCount], 16);
 				if (pEntry->DscpExceptionCount != 0)
 					memcpy((UCHAR *)pEntry->DscpException, req_data->qosmap, pEntry->DscpExceptionCount);

				PoolID = hotspot_qosmap_add_pool(pAd,pEntry);
				hotspot_qosmap_update_sta_mapping_to_cr4(pAd,pEntry,PoolID);

				Send_QOSMAP_Configure(pAd,
				     req_data->peer_mac_addr,
				     req_data->qosmap,
				     req_data->qosmap_len,
				     pEntry->func_tb_idx);
			}
			else if ((req_data->peer_mac_addr[0] == 0)
				&& (req_data->peer_mac_addr[1] == 0)
				&& (req_data->peer_mac_addr[2] == 0)
				&& (req_data->peer_mac_addr[3] == 0)
				&& (req_data->peer_mac_addr[4] == 0)
				&& (req_data->peer_mac_addr[5] == 0) ) {
			/* Special MAC 00:00:00:00:00:00 for HS2 QoS Map Change using. */
				for (i=0; VALID_UCAST_ENTRY_WCID(pAd, i); i++) {
					pEntry = &pAd->MacTab.Content[i];

					if ((IS_ENTRY_CLIENT(pEntry))
							&& (pEntry->Sst == SST_ASSOC)) {
						if (pEntry->QosMapSupport) {
							pEntry->DscpExceptionCount = 0;
							memset(pEntry->DscpRange, 0xff, 16);
							memset(pEntry->DscpException, 0xff, 42);

							pEntry->DscpExceptionCount = req_data->qosmap_len-16;
							memcpy((UCHAR *)pEntry->DscpRange,
									&req_data->qosmap[pEntry->DscpExceptionCount], 16);
							if (pEntry->DscpExceptionCount != 0) {
								memcpy((UCHAR *)pEntry->DscpException,
										req_data->qosmap, pEntry->DscpExceptionCount);
							}
							MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
								("send QoS map frame: apidx=%d\n", pEntry->func_tb_idx));
							Send_QOSMAP_Configure(pAd,
									pEntry->Addr,
									req_data->qosmap,
									req_data->qosmap_len,
									pEntry->func_tb_idx);
						}
					}
				}
			}
			os_free_mem( Buf);
		}
		break;
#endif
#endif

#ifdef BAND_STEERING
	case OID_BNDSTRG_MSG:
		BndStrg_MsgHandle(pAd, wrq);
		break;
#endif /* BAND_STEERING */

#ifdef VOW_SUPPORT
#define VOW_CMD_STR_LEN 16
    case OID_802_11_VOW_BW_EN:
        /* not used now */
        break;
    case OID_802_11_VOW_BW_AT_EN:
            {
                P_VOW_UI_CONFIG cfg;
                UCHAR buf[VOW_CMD_STR_LEN];
                UINT8 group;

			    os_alloc_mem(cfg, (UCHAR **)&cfg, wrq->u.data.length);
			    copy_from_user(cfg, wrq->u.data.pointer, wrq->u.data.length);

                for(group = 0; group < cfg->ssid_num; group++)
                {
                    sprintf(buf, "%d-%d", group, cfg->val[group]);
                    set_vow_airtime_ctrl_en(pAd, buf);

			        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OID_802_11_VOW_BW_AT_EN(0x%08x) -> val %d\n", cmd, cfg->val[group]));
                }

			    os_free_mem(cfg);
            }
        break;
    case OID_802_11_VOW_BW_TPUT_EN:
            {
                P_VOW_UI_CONFIG cfg;
                UCHAR buf[VOW_CMD_STR_LEN];
                UINT8 group;

			    os_alloc_mem(cfg, (UCHAR **)&cfg, wrq->u.data.length);
			    copy_from_user(cfg, wrq->u.data.pointer, wrq->u.data.length);

                for(group = 0; group < cfg->ssid_num; group++)
                {
                    sprintf(buf, "%d-%d", group, cfg->val[group]);
                    set_vow_bw_ctrl_en(pAd, buf);

			        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OID_802_11_VOW_BW_TPUT_EN(0x%08x) -> val %d\n", cmd, cfg->val[group]));
                }

			    os_free_mem(cfg);
            }
        break;
        case OID_802_11_VOW_ATF_EN:
            {
			    UCHAR *val, buf[VOW_CMD_STR_LEN];

			    os_alloc_mem(val, (UCHAR **)&val, wrq->u.data.length);
			    copy_from_user(val, wrq->u.data.pointer, wrq->u.data.length);

                sprintf(buf, "%d", val[0]);
                set_vow_airtime_fairness_en(pAd, buf);

			    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OID_802_11_VOW_ATF_EN(0x%08x) -> val %d\n", cmd, val[0]));

			    os_free_mem(val);
            }
        break;

        case OID_802_11_VOW_RX_EN:
            {
			    UCHAR *val, buf[VOW_CMD_STR_LEN];

			    os_alloc_mem(val, (UCHAR **)&val, wrq->u.data.length);
			    copy_from_user(val, wrq->u.data.pointer, wrq->u.data.length);

                sprintf(buf, "%d", val[0]);
                set_vow_rx_airtime_en(pAd, buf);

			    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OID_802_11_VOW_RX_EN(0x%08x) -> val %d\n", cmd, val[0]));

			    os_free_mem(val);
            }
        break;

        case OID_802_11_VOW_GROUP_MAX_RATE:
            {
                P_VOW_UI_CONFIG cfg;
                UCHAR buf[VOW_CMD_STR_LEN];
                UINT8 group;

			    os_alloc_mem(cfg, (UCHAR **)&cfg, wrq->u.data.length);
			    copy_from_user(cfg, wrq->u.data.pointer, wrq->u.data.length);

                for(group = 0; group < cfg->ssid_num; group++)
                {
                    sprintf(buf, "%d-%d", group, cfg->val[group]);
                    set_vow_max_rate(pAd, buf);

			        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OID_802_11_VOW_GROUP_MAX_RATE(0x%08x) -> val %d\n", cmd, cfg->val[group]));
                }

			    os_free_mem(cfg);
            }
        break;

        case OID_802_11_VOW_GROUP_MIN_RATE:
            {
                P_VOW_UI_CONFIG cfg;
                UCHAR buf[VOW_CMD_STR_LEN];
                UINT8 group;

			    os_alloc_mem(cfg, (UCHAR **)&cfg, wrq->u.data.length);
			    copy_from_user(cfg, wrq->u.data.pointer, wrq->u.data.length);

                for(group = 0; group < cfg->ssid_num; group++)
                {
                    sprintf(buf, "%d-%d", group, cfg->val[group]);
                    set_vow_min_rate(pAd, buf);

			        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OID_802_11_VOW_GROUP_MIN_RATE(0x%08x) -> val %d\n", cmd, cfg->val[group]));
                }

			    os_free_mem(cfg);
            }
        break;

        case OID_802_11_VOW_GROUP_MAX_RATIO:
            {
                P_VOW_UI_CONFIG cfg;
                UCHAR buf[VOW_CMD_STR_LEN];
                UINT8 group;

			    os_alloc_mem(cfg, (UCHAR **)&cfg, wrq->u.data.length);
			    copy_from_user(cfg, wrq->u.data.pointer, wrq->u.data.length);

                for(group = 0; group < cfg->ssid_num; group++)
                {
                    sprintf(buf, "%d-%d", group, cfg->val[group]);
                    set_vow_max_ratio(pAd, buf);

			        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OID_802_11_VOW_GROUP_MAX_RATIO(0x%08x) -> val %d\n", cmd, cfg->val[group]));
                }

			    os_free_mem(cfg);
            }
        break;

        case OID_802_11_VOW_GROUP_MIN_RATIO:
            {
                P_VOW_UI_CONFIG cfg;
                UCHAR buf[VOW_CMD_STR_LEN];
                UINT8 group;

			    os_alloc_mem(cfg, (UCHAR **)&cfg, wrq->u.data.length);
			    copy_from_user(cfg, wrq->u.data.pointer, wrq->u.data.length);

                for(group = 0; group < cfg->ssid_num; group++)
                {
                    sprintf(buf, "%d-%d", group, cfg->val[group]);
                    set_vow_min_ratio(pAd, buf);

			        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OID_802_11_VOW_GROUP_MIN_RATIO(0x%08x) -> val %d\n", cmd, cfg->val[group]));
                }

			    os_free_mem(cfg);
            }
        break;
#endif /* VOW_SUPPORT */

#ifdef INTERNAL_CAPTURE_SUPPORT
        case OID_802_11_WIFISPECTRUM_SET_PARAMETER:
            {
                P_ICAP_WIFI_SPECTRUM_SET_STRUC_T pWifiSpecInfo;

                os_alloc_mem(pAd, (UCHAR **)&pWifiSpecInfo, wrq->u.data.length);
                copy_from_user(pWifiSpecInfo, wrq->u.data.pointer, wrq->u.data.length);
                
                pWifiSpecInfo->ucBW = Get_Icap_WifiSpec_Bw_Info(pAd, pWifiSpecInfo->u4CaptureNode);
                
                Status = MtCmdWifiSpectrumParamSet(pAd, pWifiSpecInfo->fgTrigger, pWifiSpecInfo->fgRingCapEn, 
                         pWifiSpecInfo->u4TriggerEvent, pWifiSpecInfo->u4CaptureNode, pWifiSpecInfo->u4CaptureLen,
                         pWifiSpecInfo->u4CapStopCycle, pWifiSpecInfo->ucBW, pWifiSpecInfo->u4MACTriggerEvent,
                         pWifiSpecInfo->u4SourceAddressLSB, pWifiSpecInfo->u4SourceAddressMSB, pWifiSpecInfo->u4Band);
                
                MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s : \n Trigger = 0x%08x\n"
                " RingCapEn  = 0x%08x\n TriggerEvent  = 0x%08x\n CaptureNode = 0x%08x\n CaptureLen = 0x%08x\n"
                " CapStopCycle = 0x%08x\n BW = 0x%08x\n MACTriggerEvent = 0x%08x\n SourceAddrLSB = 0x%08x\n"
                " SourceAddrMSB = 0x%08x\n Band = 0x%08x\n", __FUNCTION__, pWifiSpecInfo->fgTrigger, pWifiSpecInfo->fgRingCapEn, 
                pWifiSpecInfo->u4TriggerEvent, pWifiSpecInfo->u4CaptureNode, pWifiSpecInfo->u4CaptureLen, pWifiSpecInfo->u4CapStopCycle, 
                pWifiSpecInfo->ucBW, pWifiSpecInfo->u4MACTriggerEvent, pWifiSpecInfo->u4SourceAddressLSB, pWifiSpecInfo->u4SourceAddressMSB,
                pWifiSpecInfo->u4Band));

                os_free_mem(pWifiSpecInfo);
            }             
            break;
        case OID_802_11_WIFISPECTRUM_GET_CAPTURE_STOP_INFO:
            {
                Status = MtCmdWifiSpectrumResultGet(pAd, NULL); 

                if(Status != NDIS_STATUS_SUCCESS)
                {
                    Status = -NDIS_STATUS_FAILURE;
                }
                
                MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_TRACE,
                ("OID_802_11_WIFISPECTRUM_GET_CAPTURE_STOP_INFO Status : %d\n", Status));
            }            
            break;    
		case OID_802_11_WIFISPECTRUM_DUMP_DATA:
			{                 
				Status = MtCmdWifiSpectrumRawDataProc(pAd);	 

                if(Status != NDIS_STATUS_SUCCESS)
                {
                    Status = -NDIS_STATUS_FAILURE;
                }
                
                MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_TRACE,
                ("OID_802_11_WIFISPECTRUM_GET_CAPTURE_STOP_INFO Status : %d\n", Status));
			}                
			break;
#endif/*INTERNAL_CAPTURE_SUPPORT*/ 

   		default:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::unknown IOCTL's subcmd = 0x%08x\n", cmd));
			Status = -EOPNOTSUPP;
			break;
    }


	return Status;
}


INT RTMPAPQueryInformation(
	IN RTMP_ADAPTER *pAd,
	IN OUT RTMP_IOCTL_INPUT_STRUCT *rq,
	IN INT cmd)
{
	RTMP_IOCTL_INPUT_STRUCT	*wrq = (RTMP_IOCTL_INPUT_STRUCT *) rq;
	INT	Status = NDIS_STATUS_SUCCESS;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	RTMP_STRING driverVersion[16];
	UCHAR apidx = pObj->ioctl_if;
#ifdef WSC_AP_SUPPORT
	UINT WscPinCode = 0;
	PWSC_PROFILE pProfile;
	PWSC_CTRL pWscControl;
#endif /* WSC_AP_SUPPORT */

#ifdef SNMP_SUPPORT
	ULONG ulInfo;
	DefaultKeyIdxValue *pKeyIdxValue;
	INT valueLen;
	TX_RTY_CFG_STRUC tx_rty_cfg;
	ULONG ShortRetryLimit, LongRetryLimit;
	UCHAR snmp_tmp[64];
#endif /* SNMP_SUPPORT */

#ifdef HOSTAPD_SUPPORT
	struct default_group_key group_key;
	struct ieee80211req_key ik;
	unsigned char *p;
	MAC_TABLE_ENTRY *pEntry=(MAC_TABLE_ENTRY *)NULL;
	struct ieee80211req_wpaie wpaie;
	BSS_STRUCT *pMbss;
#endif /*HOSTAPD_SUPPORT*/

#ifdef APCLI_SUPPORT
	UCHAR ifIndex;
	BOOLEAN apcliEn=FALSE;
	PAPCLI_STRUCT pApCliEntry=NULL;
	NDIS_802_11_SSID                    Ssid;
#endif/*APCLI_SUPPORT*/


	NDIS_802_11_STATISTICS	*pStatistics;


#ifdef DOT1X_SUPPORT
	INT IEEE8021X = 0;
#endif /* DOT1X_SUPPORT */
	NDIS_802_11_AUTHENTICATION_MODE AuthMode = Ndis802_11AuthModeMax;
	struct wifi_dev *wdev = NULL;

	/* For all ioctl to this function, we assume that's query for AP/APCLI/GO device */
	if ((pObj->ioctl_if_type == INT_MBSSID) || (pObj->ioctl_if_type == INT_MAIN))
	{
		if (apidx >= pAd->ApCfg.BssidNum)
			return EFAULT;
		wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
	}

    switch(cmd)
    {
#ifdef DOT1X_SUPPORT
	case OID_802_11_SET_IEEE8021X:
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_SET_IEEE8021X \n"));
                wrq->u.data.length = sizeof(INT);
                if(IS_IEEE8021X_Entry(&pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev))
                        IEEE8021X=1;
                else
                        IEEE8021X=0;

                Status = copy_to_user(wrq->u.data.pointer, &IEEE8021X, wrq->u.data.length);
                break;
#endif /* DOT1X_SUPPORT */
	case OID_802_11_AUTHENTICATION_MODE:
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_AUTHENTICATION_MODE \n"));
	        wrq->u.data.length = sizeof(NDIS_802_11_AUTHENTICATION_MODE);
	        AuthMode= SecAuthModeNewToOld(pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev.SecConfig.AKMMap);
	        Status = copy_to_user(wrq->u.data.pointer, &AuthMode, wrq->u.data.length);
                break;
#ifdef APCLI_SUPPORT

        case OID_802_11_BSSID:

			if (pObj->ioctl_if_type != INT_APCLI)
				return FALSE;
			ifIndex = pObj->ioctl_if;
			pApCliEntry=&pAd->ApCfg.ApCliTab[ifIndex];
			apcliEn = pAd->ApCfg.ApCliTab[ifIndex].Enable;

			if (!apcliEn)
				return FALSE;

		     if(1)
	            {
	                Status = copy_to_user(wrq->u.data.pointer, pApCliEntry->MlmeAux.Bssid, sizeof(NDIS_802_11_MAC_ADDRESS));


	               MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_INFO, ("IOCTL::SIOCGIWAP(=%02x:%02x:%02x:%02x:%02x:%02x)\n",PRINT_MAC(pApCliEntry->MlmeAux.Bssid)));

	            }
	            else
	            {
	                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_BSSID(=EMPTY)\n"));
	                Status = -ENOTCONN;
	            }
            break;

        case OID_802_11_SSID:

			if (pObj->ioctl_if_type != INT_APCLI)
				return FALSE;

			ifIndex = pObj->ioctl_if;
			pApCliEntry=&pAd->ApCfg.ApCliTab[ifIndex];
			apcliEn = pAd->ApCfg.ApCliTab[ifIndex].Enable;

			if (!apcliEn)
				return FALSE;

			NdisZeroMemory(&Ssid, sizeof(NDIS_802_11_SSID));
			NdisZeroMemory(Ssid.Ssid, MAX_LEN_OF_SSID);
            		Ssid.SsidLength = pApCliEntry->CfgSsidLen;
			NdisMoveMemory(Ssid.Ssid, pApCliEntry->CfgSsid,Ssid.SsidLength);
            		wrq->u.data.length = sizeof(NDIS_802_11_SSID);
            		Status = copy_to_user(wrq->u.data.pointer, &Ssid, wrq->u.data.length);
           		 MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query Apcli::OID_802_11_SSID (Len=%d, ssid=%s)\n", Ssid.SsidLength,Ssid.Ssid));
            break;
#endif/*APCLI_SUPPORT*/


		case RT_OID_VERSION_INFO:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_VERSION_INFO \n"));
			wrq->u.data.length = strlen(AP_DRIVER_VERSION);
			snprintf(&driverVersion[0], sizeof(driverVersion), "%s", AP_DRIVER_VERSION);
			driverVersion[wrq->u.data.length] = '\0';
			if (copy_to_user(wrq->u.data.pointer, &driverVersion, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;

		case OID_802_11_NETWORK_TYPES_SUPPORTED:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_NETWORK_TYPES_SUPPORTED \n"));
			wrq->u.data.length = sizeof(UCHAR);
			if (copy_to_user(wrq->u.data.pointer, &pAd->RfIcType, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;

#ifdef IAPP_SUPPORT
		case RT_QUERY_SIGNAL_CONTEXT:
		{
			BOOLEAN FlgIs11rSup = FALSE;

			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_QUERY_SIGNAL_CONTEXT \n"));

#ifdef DOT11R_FT_SUPPORT
			FlgIs11rSup = TRUE;
#endif /* DOT11R_FT_SUPPORT */

			if (FlgIs11rSup == FALSE)
			{
			{
				Status = -EFAULT;
			}
		}
#ifdef DOT11R_FT_SUPPORT
			else
			{
				FT_KDP_SIGNAL *pFtKdp;
				FT_KDP_EVT_HEADER *pEvtHdr;


				/* query signal content for 11r */
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_QUERY_FT_KDP_CONTEXT \n"));

				FT_KDP_EventGet(pAd, &pFtKdp);
				if (pFtKdp != NULL)
					pEvtHdr = (FT_KDP_EVT_HEADER *)pFtKdp->Content;
				/* End of if */

				if ((pFtKdp != NULL) &&
					((RT_SIGNAL_STRUC_HDR_SIZE + pEvtHdr->EventLen) <=
														wrq->u.data.length))
				{
					/* copy the event */
					if (copy_to_user(
								wrq->u.data.pointer,
								pFtKdp,
								RT_SIGNAL_STRUC_HDR_SIZE + pEvtHdr->EventLen))
					{
						wrq->u.data.length = 0;
						Status = -EFAULT;
					}
					else
					{
						wrq->u.data.length = RT_SIGNAL_STRUC_HDR_SIZE;
						wrq->u.data.length += pEvtHdr->EventLen;
					}

					FT_MEM_FREE(pAd, pFtKdp);
				}
				else
				{
					/* no event is queued */
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("ft_kdp> no event is queued!\n"));
					wrq->u.data.length = 0;
				}
			}
#endif /* DOT11R_FT_SUPPORT */
		}
			break;

#ifdef DOT11R_FT_SUPPORT
		case RT_FT_DATA_ENCRYPT:
		case RT_FT_DATA_DECRYPT:
		{
			UCHAR *pBuffer;
			UINT32 DataLen;

			DataLen = wrq->u.data.length;

			/*
				Make sure the data length is multiple of 8
				due to AES_KEY_WRAP() limitation.
			*/
			if (DataLen & 0x07)
				DataLen += 8 - (DataLen & 0x07);
			/* End of if */

			FT_MEM_ALLOC(pAd, &pBuffer, DataLen+FT_KDP_KEY_ENCRYPTION_EXTEND);
			if (pBuffer == NULL)
				break;
			NdisZeroMemory(pBuffer, DataLen+FT_KDP_KEY_ENCRYPTION_EXTEND);

			if (copy_from_user(pBuffer, wrq->u.data.pointer, wrq->u.data.length))
			{
				Status = -EFAULT;
				FT_MEM_FREE(pAd, pBuffer);
				break;
			}

			switch(cmd)
			{
				case RT_FT_DATA_ENCRYPT:
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_FT_DATA_ENCRYPT \n"));
					FT_KDP_DataEncrypt(pAd, (UCHAR *)pBuffer, &DataLen);
					break;

				case RT_FT_DATA_DECRYPT:
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_FT_DATA_DECRYPT \n"));
					FT_KDP_DataDecrypt(pAd, (UCHAR *)pBuffer, &DataLen);
					break;
			}

			wrq->u.data.length = DataLen;
			if (copy_to_user(wrq->u.data.pointer, pBuffer, wrq->u.data.length))
				Status = -EFAULT;
			FT_MEM_FREE(pAd, pBuffer);
		}
		break;

		case RT_OID_802_11R_INFO:
			{
				UCHAR apidx = pObj->ioctl_if;
				PFT_CONFIG_INFO pFtConfig;
				PFT_CFG pFtCfg;

				os_alloc_mem(pAd, (UCHAR **)&pFtConfig, sizeof(FT_CONFIG_INFO));
				if (pFtConfig == NULL)
					break;

				pFtCfg = &pAd->ApCfg.MBSSID[apidx].wdev.FtCfg;
				NdisZeroMemory(pFtConfig, sizeof(FT_CONFIG_INFO));

				pFtConfig->FtSupport = pFtCfg->FtCapFlag.Dot11rFtEnable;
				pFtConfig->FtRicSupport = pFtCfg->FtCapFlag.RsrReqCap;
				pFtConfig->FtOtdSupport = pFtCfg->FtCapFlag.FtOverDs;
				NdisMoveMemory(pFtConfig->MdId, pFtCfg->FtMdId, FT_MDID_LEN);
				pFtConfig->R0KHIdLen = pFtCfg->FtR0khIdLen;
				NdisMoveMemory(pFtConfig->R0KHId, pFtCfg->FtR0khId, pFtCfg->FtR0khIdLen);

				wrq->u.data.length = sizeof(FT_CONFIG_INFO);
				Status = copy_to_user(wrq->u.data.pointer, pFtConfig, wrq->u.data.length);
				os_free_mem( pFtConfig);
			}
			break;
#endif /* DOT11R_FT_SUPPORT */

#endif /* IAPP_SUPPORT */


#ifdef WSC_AP_SUPPORT
		case RT_OID_WSC_QUERY_STATUS:
		{
			INT WscStatus;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_WSC_QUERY_STATUS \n"));
#ifdef APCLI_SUPPORT
			if (pObj->ioctl_if_type == INT_APCLI)
			{
				INT ApCliIdx = pObj->ioctl_if;
				APCLI_MR_APIDX_SANITY_CHECK(ApCliIdx);
				WscStatus = pAd->ApCfg.ApCliTab[ApCliIdx].WscControl.WscStatus;
			}
			else
#endif /* APCLI_SUPPORT */
			{
				WscStatus = pAd->ApCfg.MBSSID[apidx].WscControl.WscStatus;
			}

			wrq->u.data.length = sizeof(INT);
			if (copy_to_user(wrq->u.data.pointer, &WscStatus, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;
		}
		case RT_OID_WSC_PIN_CODE:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_WSC_PIN_CODE \n"));
			wrq->u.data.length = sizeof(UINT);
			/*WscPinCode = GenerateWpsPinCode(pAd, FALSE, apidx); */
			pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;

			WscPinCode = pWscControl->WscEnrolleePinCode;
			if (copy_to_user(wrq->u.data.pointer, &WscPinCode, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;
#ifdef APCLI_SUPPORT
        case RT_OID_APCLI_WSC_PIN_CODE:
            MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_APCLI_WSC_PIN_CODE \n"));
			wrq->u.data.length = sizeof(UINT);
			/*WscPinCode = GenerateWpsPinCode(pAd, TRUE, apidx); */
			WscPinCode = pAd->ApCfg.ApCliTab[apidx].WscControl.WscEnrolleePinCode;

			if (copy_to_user(wrq->u.data.pointer, &WscPinCode, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
            break;
#endif /* APCLI_SUPPORT */
		case RT_OID_WSC_UUID:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_WSC_QUERY_UUID \n"));
			wrq->u.data.length = UUID_LEN_STR;
			pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;
			if (copy_to_user(wrq->u.data.pointer, &pWscControl->Wsc_Uuid_Str[0], UUID_LEN_STR))
			{
				Status = -EFAULT;
			}
			break;
		case RT_OID_WSC_MAC_ADDRESS:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_WSC_MAC_ADDRESS \n"));
			wrq->u.data.length = MAC_ADDR_LEN;
			if (copy_to_user(wrq->u.data.pointer, pAd->ApCfg.MBSSID[apidx].wdev.bssid, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;
		case RT_OID_WSC_CONFIG_STATUS:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_WSC_CONFIG_STATUS \n"));
			wrq->u.data.length = sizeof(UCHAR);
			if (copy_to_user(wrq->u.data.pointer, &pAd->ApCfg.MBSSID[apidx].WscControl.WscConfStatus, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;

		case RT_OID_WSC_QUERY_PEER_INFO_ON_RUNNING:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_WSC_QUERY_PEER_INFO_ON_RUNNING \n"));

			if (pAd->ApCfg.MBSSID[apidx].WscControl.WscState > WSC_STATE_WAIT_M2)
			{
				wrq->u.data.length = sizeof(WSC_PEER_DEV_INFO);
				if (copy_to_user(wrq->u.data.pointer, &pAd->ApCfg.MBSSID[apidx].WscControl.WscPeerInfo, wrq->u.data.length))
				{
					Status = -EFAULT;
				}
			}
			else
			{
				Status = -EFAULT;
			}
			break;

		case RT_OID_802_11_WSC_QUERY_PROFILE:
			wrq->u.data.length = sizeof(WSC_PROFILE);
			os_alloc_mem(pAd, (UCHAR **)&pProfile, sizeof(WSC_PROFILE));
			if (pProfile == NULL)
			{
				Status = -EFAULT;
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RT_OID_802_11_WSC_QUERY_PROFILE fail!\n"));
				break;
			}
#ifdef APCLI_SUPPORT
			if (pObj->ioctl_if_type == INT_APCLI)
			{
				APCLI_MR_APIDX_SANITY_CHECK(apidx);		    
				pWscControl = &pAd->ApCfg.ApCliTab[apidx].WscControl;
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli : %d) RT_OID_802_11_WSC_QUERY_PROFILE :: This command is from apcli interface now.\n", apidx));
			}
		    	else
#endif /* APCLI_SUPPORT */
			pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;

			RTMPZeroMemory(pProfile, sizeof(WSC_PROFILE));
			NdisMoveMemory(pProfile, &pWscControl->WscProfile, sizeof(WSC_PROFILE));
            if ((pProfile->Profile[0].AuthType == WSC_AUTHTYPE_OPEN) && (pProfile->Profile[0].EncrType == WSC_ENCRTYPE_NONE))
            {
                pProfile->Profile[0].KeyLength = 0;
                NdisZeroMemory(pProfile->Profile[0].Key, 64);
            }
			if (copy_to_user(wrq->u.data.pointer, pProfile, wrq->u.data.length))
			{
				Status = -EFAULT;
			}

			os_free_mem( pProfile);
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_802_11_WSC_QUERY_PROFILE \n"));
			break;
#ifdef WSC_V2_SUPPORT
		case RT_OID_WSC_V2_SUPPORT:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_WSC_V2_SUPPORT (=%d)\n", pAd->ApCfg.MBSSID[apidx].WscControl.WscV2Info.bEnableWpsV2));
			wrq->u.data.length = sizeof(BOOLEAN);
			if (copy_to_user(wrq->u.data.pointer, &pAd->ApCfg.MBSSID[apidx].WscControl.WscV2Info.bEnableWpsV2, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;
		case RT_OID_WSC_FRAGMENT_SIZE:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_WSC_FRAGMENT_SIZE (=%d)\n", pAd->ApCfg.MBSSID[apidx].WscControl.WscFragSize));
			wrq->u.data.length = sizeof(USHORT);
			if (copy_to_user(wrq->u.data.pointer, &pAd->ApCfg.MBSSID[apidx].WscControl.WscFragSize, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;
#endif /* WSC_V2_SUPPORT */

#ifdef WSC_NFC_SUPPORT
		case RT_OID_NFC_STATUS:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_NFC_STATUS (=%d)\n", pAd->ApCfg.MBSSID[apidx].WscControl.NfcStatus));
			wrq->u.data.length = sizeof(UCHAR);
			if (copy_to_user(wrq->u.data.pointer, &pAd->ApCfg.MBSSID[apidx].WscControl.NfcStatus, wrq->u.data.length))
			{
				Status = -EFAULT;
			}
			break;
#endif /* WSC_NFC_SUPPORT */

#endif /* WSC_AP_SUPPORT */
#ifdef LLTD_SUPPORT
        case RT_OID_GET_LLTD_ASSO_TABLE:
            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::Get LLTD association table\n"));
            if ((wrq->u.data.pointer == NULL) || (apidx != MAIN_MBSSID))
            {
                Status = -EFAULT;
            }
            else
            {
                INT						    i;
                RT_LLTD_ASSOICATION_TABLE	AssocTab;

            	AssocTab.Num = 0;
            	for (i=0; VALID_UCAST_ENTRY_WCID(pAd, i); i++)
            	{
            		if (IS_ENTRY_CLIENT(&pAd->MacTab.Content[i]) && (pAd->MacTab.Content[i].Sst == SST_ASSOC))
            		{
            			COPY_MAC_ADDR(AssocTab.Entry[AssocTab.Num].Addr, &pAd->MacTab.Content[i].Addr);
                        AssocTab.Entry[AssocTab.Num].phyMode = pAd->ApCfg.MBSSID[apidx].wdev.PhyMode;
                        AssocTab.Entry[AssocTab.Num].MOR = RateIdToMbps[pAd->ApCfg.MBSSID[apidx].MaxTxRate] * 2;
            			AssocTab.Num += 1;
            		}
            	}
                wrq->u.data.length = sizeof(RT_LLTD_ASSOICATION_TABLE);
            	if (copy_to_user(wrq->u.data.pointer, &AssocTab, wrq->u.data.length))
            	{
            		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s: copy_to_user() fail\n", __FUNCTION__));
                    Status = -EFAULT;
            	}
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("AssocTab.Num = %d \n", AssocTab.Num));
            }
            break;
#ifdef APCLI_SUPPORT
		case RT_OID_GET_REPEATER_AP_LINEAGE:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Not Support : Get repeater AP lineage.\n"));
			break;
#endif /* APCLI_SUPPORT */

#endif /* LLTD_SUPPORT */
#ifdef DOT1X_SUPPORT
		case OID_802_DOT1X_CONFIGURATION:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::Get Radius setting(%lu)\n", (ULONG)sizeof(DOT1X_CMM_CONF)));
			Dot1xIoctlQueryRadiusConf(pAd, wrq);
			break;

		case OID_802_DOT1X_QUERY_STA_AID:
			RTMPIoctlQueryStaAid(pAd, wrq);
            break;

#ifdef RADIUS_ACCOUNTING_SUPPORT
		case OID_802_DOT1X_QUERY_STA_DATA:
			RTMPIoctlQueryStaData(pAd, wrq);
            break;
#endif /* RADIUS_ACCOUNTING_SUPPORT */

#endif /* DOT1X_SUPPORT */

		case RT_OID_802_11_MAC_ADDRESS:
                        wrq->u.data.length = MAC_ADDR_LEN;
                        Status = copy_to_user(wrq->u.data.pointer, &pAd->ApCfg.MBSSID[apidx].wdev.bssid, wrq->u.data.length);
			break;

#ifdef SNMP_SUPPORT
		case RT_OID_802_11_MANUFACTUREROUI:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_802_11_MANUFACTUREROUI \n"));
			wrq->u.data.length = ManufacturerOUI_LEN;
			Status = copy_to_user(wrq->u.data.pointer, &pAd->CurrentAddress, wrq->u.data.length);
			break;

		case RT_OID_802_11_MANUFACTURERNAME:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_802_11_MANUFACTURERNAME \n"));
			wrq->u.data.length = strlen(ManufacturerNAME);
			Status = copy_to_user(wrq->u.data.pointer, ManufacturerNAME, wrq->u.data.length);
			break;

		case RT_OID_802_11_RESOURCETYPEIDNAME:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_802_11_RESOURCETYPEIDNAME \n"));
			wrq->u.data.length = strlen(ResourceTypeIdName);
			Status = copy_to_user(wrq->u.data.pointer, ResourceTypeIdName, wrq->u.data.length);
			break;

		case RT_OID_802_11_PRIVACYOPTIONIMPLEMENTED:
		{
			ULONG ulInfo;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_802_11_PRIVACYOPTIONIMPLEMENTED \n"));
			ulInfo = 1; /* 1 is support wep else 2 is not support. */
			wrq->u.data.length = sizeof(ulInfo);
			Status = copy_to_user(wrq->u.data.pointer, &ulInfo, wrq->u.data.length);
			break;
		}
		case RT_OID_802_11_POWERMANAGEMENTMODE:
		{
			ULONG ulInfo;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_802_11_POWERMANAGEMENTMODE \n"));
			ulInfo = 1; /* 1 is power active else 2 is power save. */
			wrq->u.data.length = sizeof(ulInfo);
			Status = copy_to_user(wrq->u.data.pointer, &ulInfo, wrq->u.data.length);
			break;
		}
		case OID_802_11_WEPDEFAULTKEYVALUE:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_WEPDEFAULTKEYVALUE \n"));
			pKeyIdxValue = wrq->u.data.pointer;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("KeyIdxValue.KeyIdx = %d, \n",pKeyIdxValue->KeyIdx));

			valueLen = pAd->SharedKey[pObj->ioctl_if][pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId].KeyLen;
			NdisMoveMemory(pKeyIdxValue->Value,
						   &pAd->SharedKey[pObj->ioctl_if][pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId].Key,
						   valueLen);
			pKeyIdxValue->Value[valueLen]='\0';

			wrq->u.data.length = sizeof(DefaultKeyIdxValue);

			Status = copy_to_user(wrq->u.data.pointer, pKeyIdxValue, wrq->u.data.length);
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("DefaultKeyId = %d, total len = %d, str len=%d, KeyValue= %02x %02x %02x %02x \n", pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId, wrq->u.data.length, pAd->SharedKey[pObj->ioctl_if][pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId].KeyLen,
			pAd->SharedKey[pObj->ioctl_if][0].Key[0],
			pAd->SharedKey[pObj->ioctl_if][1].Key[0],
			pAd->SharedKey[pObj->ioctl_if][2].Key[0],
			pAd->SharedKey[pObj->ioctl_if][3].Key[0]));
			break;

		case OID_802_11_WEPDEFAULTKEYID:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_802_11_WEPDEFAULTKEYID \n"));
			wrq->u.data.length = sizeof(UCHAR);
			Status = copy_to_user(wrq->u.data.pointer, &pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId, wrq->u.data.length);
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("DefaultKeyId =%d \n", pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId));
			break;

		case RT_OID_802_11_WEPKEYMAPPINGLENGTH:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_802_11_WEPKEYMAPPINGLENGTH \n"));
			wrq->u.data.length = sizeof(UCHAR);
			Status = copy_to_user(wrq->u.data.pointer,
									&pAd->SharedKey[pObj->ioctl_if][pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId].KeyLen,
									wrq->u.data.length);
			break;

		case OID_802_11_SHORTRETRYLIMIT:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_SHORTRETRYLIMIT \n"));
			wrq->u.data.length = sizeof(ULONG);
			ShortRetryLimit = AsicGetRetryLimit(pAd, TX_RTY_CFG_RTY_LIMIT_SHORT);
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("ShortRetryLimit =%ld\n", ShortRetryLimit));
			Status = copy_to_user(wrq->u.data.pointer, &ShortRetryLimit, wrq->u.data.length);
			break;

		case OID_802_11_LONGRETRYLIMIT:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_LONGRETRYLIMIT \n"));
			wrq->u.data.length = sizeof(ULONG);
			LongRetryLimit = AsicGetRetryLimit(pAd, TX_RTY_CFG_RTY_LIMIT_LONG);
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("LongRetryLimit =%ld\n", LongRetryLimit));
			Status = copy_to_user(wrq->u.data.pointer, &LongRetryLimit, wrq->u.data.length);
			break;

		case RT_OID_802_11_PRODUCTID:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_802_11_PRODUCTID \n"));

#ifdef RTMP_MAC_PCI
			{

				USHORT  device_id;
				if (((POS_COOKIE)pAd->OS_Cookie)->pci_dev != NULL)
			    	pci_read_config_word(((POS_COOKIE)pAd->OS_Cookie)->pci_dev, PCI_DEVICE_ID, &device_id);
				else
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, (" pci_dev = NULL\n"));
				snprintf((RTMP_STRING *)snmp_tmp, sizeof(snmp_tmp), "%04x %04x\n", NIC_PCI_VENDOR_ID, device_id);
			}
#endif /* RTMP_MAC_PCI */
			wrq->u.data.length = strlen((RTMP_STRING *) snmp_tmp);
			Status = copy_to_user(wrq->u.data.pointer, snmp_tmp, wrq->u.data.length);
			break;

		case RT_OID_802_11_MANUFACTUREID:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_802_11_MANUFACTUREID \n"));
			wrq->u.data.length = strlen(ManufacturerNAME);
			Status = copy_to_user(wrq->u.data.pointer, ManufacturerNAME, wrq->u.data.length);
			break;

		case OID_802_11_CURRENTCHANNEL:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_CURRENTCHANNEL \n"));
			wrq->u.data.length = sizeof(UCHAR);
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("sizeof UCHAR=%d, channel=%d \n", sizeof(UCHAR), pAd->ApCfg.MBSSID[apidx].wdev.channel));
			Status = copy_to_user(wrq->u.data.pointer, &pAd->ApCfg.MBSSID[apidx].wdev.channel, wrq->u.data.length);
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Status=%d\n", Status));
			break;
#endif /* SNMP_SUPPORT */

        case OID_802_11_STATISTICS:
		os_alloc_mem(pAd, (UCHAR **)&pStatistics, sizeof(NDIS_802_11_STATISTICS));
            if (pStatistics)
            {
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_STATISTICS \n"));
                /* add the most up-to-date h/w raw counters into software counters */
			/*NICUpdateRawCountersNew(pAd);*/

			pStatistics->TransmittedFragmentCount.QuadPart = pAd->WlanCounters[0].TransmittedFragmentCount.QuadPart + pAd->WlanCounters[0].MulticastTransmittedFrameCount.QuadPart;
                pStatistics->MulticastTransmittedFrameCount.QuadPart = pAd->WlanCounters[0].MulticastTransmittedFrameCount.QuadPart;
                pStatistics->FailedCount.QuadPart = pAd->WlanCounters[0].FailedCount.QuadPart;
                pStatistics->RetryCount.QuadPart = pAd->WlanCounters[0].RetryCount.QuadPart;
                pStatistics->MultipleRetryCount.QuadPart = pAd->WlanCounters[0].MultipleRetryCount.QuadPart;
                pStatistics->RTSSuccessCount.QuadPart = pAd->WlanCounters[0].RTSSuccessCount.QuadPart;
                pStatistics->RTSFailureCount.QuadPart = pAd->WlanCounters[0].RTSFailureCount.QuadPart;
                pStatistics->ACKFailureCount.QuadPart = pAd->WlanCounters[0].ACKFailureCount.QuadPart;
                pStatistics->FrameDuplicateCount.QuadPart = pAd->WlanCounters[0].FrameDuplicateCount.QuadPart;
                pStatistics->ReceivedFragmentCount.QuadPart = pAd->WlanCounters[0].ReceivedFragmentCount.QuadPart;
                pStatistics->MulticastReceivedFrameCount.QuadPart = pAd->WlanCounters[0].MulticastReceivedFrameCount.QuadPart;
#ifdef DBG
                pStatistics->FCSErrorCount = pAd->RalinkCounters.RealFcsErrCount;
#else
                pStatistics->FCSErrorCount.QuadPart = pAd->WlanCounters[0].FCSErrorCount.QuadPart;
                pStatistics->FrameDuplicateCount.u.LowPart = pAd->WlanCounters[0].FrameDuplicateCount.u.LowPart / 100;
#endif
			pStatistics->TransmittedFrameCount.QuadPart = pAd->WlanCounters[0].TransmittedFragmentCount.QuadPart;
			pStatistics->WEPUndecryptableCount.QuadPart = pAd->WlanCounters[0].WEPUndecryptableCount.QuadPart;
                wrq->u.data.length = sizeof(NDIS_802_11_STATISTICS);
                Status = copy_to_user(wrq->u.data.pointer, pStatistics, wrq->u.data.length);
			os_free_mem( pStatistics);
            }
            else
            {
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_STATISTICS(mem alloc failed)\n"));
                Status = -EFAULT;
            }
            break;

	case RT_OID_802_11_PER_BSS_STATISTICS:
		{
			PMBSS_STATISTICS pMbssStat;
			INT apidx = pObj->ioctl_if;
			BSS_STRUCT *pMbss = &pAd->ApCfg.MBSSID[apidx];

			os_alloc_mem(pAd, (UCHAR * *) &pMbssStat, sizeof(MBSS_STATISTICS));
			NdisZeroMemory(pMbssStat, sizeof(MBSS_STATISTICS));

			pMbssStat->TransmittedByteCount = pMbss->TransmittedByteCount;
			pMbssStat->ReceivedByteCount =  pMbss->ReceivedByteCount;
			pMbssStat->TxCount =  pMbss->TxCount;
			pMbssStat->RxCount =  pMbss->RxCount;
			pMbssStat->RxErrorCount =  pMbss->RxErrorCount;
			pMbssStat->RxDropCount =  pMbss->RxDropCount;
			pMbssStat->TxErrorCount =  pMbss->TxErrorCount;
			pMbssStat->TxDropCount =  pMbss->TxDropCount;
			pMbssStat->ucPktsTx =  pMbss->ucPktsTx;
			pMbssStat->ucPktsRx =  pMbss->ucPktsRx;
			pMbssStat->mcPktsTx =  pMbss->mcPktsTx;
			pMbssStat->mcPktsRx =  pMbss->mcPktsRx;
			pMbssStat->bcPktsTx=  pMbss->bcPktsTx;
			pMbssStat->bcPktsRx=  pMbss->bcPktsRx;
			wrq->u.data.length = sizeof(MBSS_STATISTICS);
			Status = copy_to_user(wrq->u.data.pointer, pMbssStat, wrq->u.data.length);
			os_free_mem( pMbssStat);
		}
		break;



#ifdef WAPI_SUPPORT
		case OID_802_11_MCAST_TXIV:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::RT_OID_802_11_MCAST_TXIV \n"));
			Status  = -EINVAL;
			break;
		case OID_802_11_WAPI_CONFIGURATION:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::Get WAPI Configuration(%d)\n", sizeof(WAPI_CONF)));
			RTMPIoctlQueryWapiConf(pAd, wrq);
			break;
		case OID_802_11_WAPI_IE:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_WAPI_IE\n"));
			if (wrq->u.data.length != sizeof(WAPI_WIE_STRUCT))
                Status  = -EINVAL;
            else
            {
				WAPI_WIE_STRUCT   	wapi_ie;
				MAC_TABLE_ENTRY		*pWapiEntry;

				NdisZeroMemory(&wapi_ie, sizeof(WAPI_WIE_STRUCT));
				NdisMoveMemory(wapi_ie.addr, wrq->u.data.pointer, MAC_ADDR_LEN);

				pWapiEntry = MacTableLookup(pAd, wapi_ie.addr);

				if (pWapiEntry && IS_ENTRY_CLIENT(pWapiEntry) && (pWapiEntry->RSNIE_Len > 0))
				{
					wapi_ie.wie_len = pWapiEntry->RSNIE_Len;
					NdisMoveMemory(wapi_ie.wie, pWapiEntry->RSN_IE, pWapiEntry->RSNIE_Len);
				}

				if (copy_to_user(wrq->u.data.pointer, &wapi_ie, wrq->u.data.length))
				{
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: copy_to_user() fail\n", __FUNCTION__));
				}
            }
			break;
#endif /* WAPI_SUPPORT */

#ifdef HOSTAPD_SUPPORT

		case HOSTAPD_OID_GETWPAIE:/*report wpa ie of the new station to hostapd. */

			if (wrq->u.data.length != sizeof(wpaie))
			{
				Status = -EINVAL;
			}
			else if (copy_from_user(&wpaie, wrq->u.data.pointer, IEEE80211_ADDR_LEN))
			{
				Status = -EFAULT;
			}
			else
			{
				pEntry = MacTableLookup(pAd, wpaie.wpa_macaddr);
				if (!pEntry){
					Status = -EINVAL;
					break;
				}
				NdisZeroMemory(wpaie.rsn_ie,sizeof(wpaie.rsn_ie));
				/* For WPA1, RSN_IE=221 */
				if ((pEntry->AuthMode == Ndis802_11AuthModeWPA) || (pEntry->AuthMode == Ndis802_11AuthModeWPAPSK)
					||(pEntry->AuthMode == Ndis802_11AuthModeWPA2) || (pEntry->AuthMode == Ndis802_11AuthModeWPA2PSK)
					|| (pEntry->AuthMode == Ndis802_11AuthModeWPA1WPA2) ||(pEntry->AuthMode == Ndis802_11AuthModeWPA1PSKWPA2PSK)
					)
				{
					int ielen = pEntry->RSNIE_Len;
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("pEntry->RSNIE_Len=%d\n",pEntry->RSNIE_Len));
					if (ielen > sizeof(wpaie.rsn_ie))
						ielen = sizeof(wpaie.rsn_ie)-1;
					p = wpaie.rsn_ie;
					hex_dump("HOSTAPD_OID_GETWPAIE woody==>pEntry->RSN_IE", (unsigned char*)pEntry->RSN_IE,ielen);
					NdisMoveMemory(p, pEntry->RSN_IE, ielen);
				}
			}
			if(copy_to_user(wrq->u.data.pointer, &wpaie, sizeof(wpaie)))
				Status = -EFAULT;
			break;


		case HOSTAPD_OID_GET_SEQ:/*report txtsc to hostapd. */

			pMbss = &pAd->ApCfg.MBSSID[apidx];
			if (wrq->u.data.length != sizeof(ik))
			{
				Status = -EINVAL;
			}
			else if (copy_from_user(&ik, wrq->u.data.pointer, IEEE80211_ADDR_LEN))
			{
				Status = -EFAULT;
			}
			else
			{
				NdisZeroMemory(&ik.ik_keytsc, sizeof(ik.ik_keytsc));
				p = (unsigned char *)&ik.ik_keytsc;
				NdisMoveMemory(p+2, pAd->SharedKey[apidx][ pMbss->DefaultKeyId].TxTsc, 6);
				if(copy_to_user(wrq->u.data.pointer, &ik, sizeof(ik)))
					Status = -EFAULT;
			}
			break;


		case HOSTAPD_OID_GET_1X_GROUP_KEY:/*report default group key to hostapd. */

			pMbss = &pAd->ApCfg.MBSSID[apidx];
			if (wrq->u.data.length != sizeof(group_key))
			{
				Status = -EINVAL;
			}
			else
			{
				if(pAd->SharedKey[apidx][ pMbss->DefaultKeyId].KeyLen!=0 && pAd->SharedKey[apidx][ pMbss->DefaultKeyId].Key!=NULL)
				{
					group_key.ik_keyix = pMbss->DefaultKeyId;
					group_key.ik_keylen = pAd->SharedKey[apidx][ pMbss->DefaultKeyId].KeyLen;
					NdisMoveMemory(group_key.ik_keydata, pAd->SharedKey[apidx][ pMbss->DefaultKeyId].Key,pAd->SharedKey[apidx][ pMbss->DefaultKeyId].KeyLen);
					if(copy_to_user(wrq->u.data.pointer, &group_key, sizeof(group_key)))
						Status = -EFAULT;
				}
			}
			break;

#endif/*HOSTAPD_SUPPORT*/

#ifdef APCLI_SUPPORT
		case OID_GEN_MEDIA_CONNECT_STATUS:
			{
				ULONG ApCliIdx = pObj->ioctl_if;

				NDIS_MEDIA_STATE MediaState;
				PMAC_TABLE_ENTRY pEntry;
				STA_TR_ENTRY *tr_entry;
				PAPCLI_STRUCT pApCliEntry;

				if (pObj->ioctl_if_type != INT_APCLI)
				{
					Status = -EOPNOTSUPP;
					break;
				}
				else
				{
					APCLI_MR_APIDX_SANITY_CHECK(ApCliIdx);
					pApCliEntry = &pAd->ApCfg.ApCliTab[ApCliIdx];
					if (pApCliEntry->MacTabWCID >= MAX_LEN_OF_MAC_TABLE) {
						Status = -EOPNOTSUPP;
						break;
					}
					pEntry = &pAd->MacTab.Content[pApCliEntry->MacTabWCID];
					tr_entry = &pAd->MacTab.tr_entry[pApCliEntry->MacTabWCID];
					if (!IS_ENTRY_APCLI(pEntry) && !IS_ENTRY_REPEATER(pEntry))
					{
						Status = -EOPNOTSUPP;
						break;
					}

					if ((pAd->ApCfg.ApCliTab[ApCliIdx].Valid == TRUE)
						&& (tr_entry->PortSecured == WPA_802_1X_PORT_SECURED))
						MediaState = NdisMediaStateConnected;
					else
						MediaState = NdisMediaStateDisconnected;

					wrq->u.data.length = sizeof(NDIS_MEDIA_STATE);
					Status = copy_to_user(wrq->u.data.pointer, &MediaState, wrq->u.data.length);
				}
			}
			break;
#endif /* APCLI_SUPPORT */




		case OID_802_11_ACL_LIST:
			if (wrq->u.data.length < sizeof(RT_802_11_ACL))
			{
				Status = -EINVAL;
			}
			else
			{
				Status = copy_to_user(wrq->u.data.pointer, &pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList, sizeof(RT_802_11_ACL));
			}
			break;
#ifdef CONFIG_HOTSPOT
#ifdef CONFIG_DOT11V_WNM
		case OID_802_11_WNM_IPV4_PROXY_ARP_LIST:
			{
				BSS_STRUCT *pMbss;
				PUCHAR pProxyARPTable;
				UINT32 ARPTableLen;
				pMbss = &pAd->ApCfg.MBSSID[pObj->ioctl_if];
				ARPTableLen = IPv4ProxyARPTableLen(pAd, pMbss);
				os_alloc_mem(NULL, &pProxyARPTable, ARPTableLen);
				GetIPv4ProxyARPTable(pAd, pMbss, &pProxyARPTable);
				wrq->u.data.length = ARPTableLen;
				Status = copy_to_user(wrq->u.data.pointer, pProxyARPTable, ARPTableLen);
				os_free_mem( pProxyARPTable);
			}
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_11_WNM_PROXY_ARP_LIST\n"));
			break;
		case OID_802_11_WNM_IPV6_PROXY_ARP_LIST:
			{
				BSS_STRUCT *pMbss;
				PUCHAR pProxyARPTable;
				UINT32 ARPTableLen;
				pMbss = &pAd->ApCfg.MBSSID[pObj->ioctl_if];
				ARPTableLen = IPv6ProxyARPTableLen(pAd, pMbss);
				os_alloc_mem(NULL, &pProxyARPTable, ARPTableLen);
				GetIPv6ProxyARPTable(pAd, pMbss, &pProxyARPTable);
				wrq->u.data.length = ARPTableLen;
				Status = copy_to_user(wrq->u.data.pointer, pProxyARPTable, ARPTableLen);
				os_free_mem( pProxyARPTable);
			}
			break;
#endif
		case OID_802_11_SECURITY_TYPE:
			{
				BSS_STRUCT *pMbss;
				PUCHAR pType;
				struct security_type *SecurityType;
				//MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query:OID_802_11_SECURITY_TYPE\n"));
				printk("Query:OID_802_11_SECURITY_TYPE\n");
				os_alloc_mem(NULL, &pType, sizeof(*SecurityType));
				SecurityType = (struct security_type *)pType;
				pMbss = &pAd->ApCfg.MBSSID[pObj->ioctl_if];
				SecurityType->ifindex = pObj->ioctl_if;
				SecurityType->auth_mode = SecAuthModeNewToOld(pMbss->wdev.SecConfig.AKMMap);
				SecurityType->encryp_type = SecEncryModeNewToOld(pMbss->wdev.SecConfig.PairwiseCipher);				
				wrq->u.data.length = sizeof(*SecurityType);
				Status = copy_to_user(wrq->u.data.pointer, pType, sizeof(*SecurityType));
				os_free_mem( pType);
			}
			break;
		case OID_802_11_HS_BSSID:
			{
				BSS_STRUCT *pMbss;
				pMbss = &pAd->ApCfg.MBSSID[pObj->ioctl_if];
				wrq->u.data.length = 6;
				Status = copy_to_user(wrq->u.data.pointer, pMbss->wdev.bssid, 6);
			}
			break;

#ifdef CONFIG_HOTSPOT_R2			
		case OID_802_11_HS_OSU_SSID:
			{
				wrq->u.data.length = pAd->ApCfg.MBSSID[pObj->ioctl_if].SsidLen; //+2;
				//tmpbuf[0] = IE_SSID;
				//tmpbuf[1] = pAd->ApCfg.MBSSID[pObj->ioctl_if].SsidLen;			
				//memcpy(&tmpbuf[2], pAd->ApCfg.MBSSID[pObj->ioctl_if].Ssid, pAd->ApCfg.MBSSID[pObj->ioctl_if].SsidLen);
				//Status = copy_to_user(wrq->u.data.pointer, tmpbuf, wrq->u.data.length);
				Status = copy_to_user(wrq->u.data.pointer, pAd->ApCfg.MBSSID[pObj->ioctl_if].Ssid, pAd->ApCfg.MBSSID[pObj->ioctl_if].SsidLen);

				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("\033[1;32m %s, %u OID_802_11_HS_OSU_SSID [%s]\033[0m\n"
					, __FUNCTION__, __LINE__,pAd->ApCfg.MBSSID[pObj->ioctl_if].Ssid));  
				/* this is an osu ssid , disable 11U capability */
				pAd->ApCfg.MBSSID[pObj->ioctl_if].GASCtrl.b11U_enable = FALSE;
			}	
			break;	
#endif /* CONFIG_HOTSPOT_R2 */

#endif

#ifdef INTERNAL_CAPTURE_SUPPORT
        case OID_802_11_WIFISPECTRUM_GET_CAPTURE_BW:
            {
                UINT32 CaptureNode;
                UCHAR CaptureBw;

                CaptureNode = Get_Icap_WifiSpec_Capture_Node_Info(pAd);

                CaptureBw = Get_Icap_WifiSpec_Bw_Info(pAd, CaptureNode);
                
                wrq->u.data.length = sizeof(UCHAR);
                
                Status = copy_to_user(wrq->u.data.pointer, &CaptureBw, wrq->u.data.length);
                
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, 
                ("Query::OID_802_11_WIFISPECTRUM_GET_CAPTURE_BW CaptureBw = %d\n", CaptureBw));

            }
            break;
        case OID_802_11_WIFISPECTRUM_GET_CENTRAL_FREQ:
            {
                UINT32 CaptureNode; 
                USHORT CentralFreq;

                CaptureNode = Get_Icap_WifiSpec_Capture_Node_Info(pAd);
                
                CentralFreq = Get_Icap_WifiSpec_CentralFreq_Info(pAd, CaptureNode);

                wrq->u.data.length = sizeof(USHORT);
                
                Status = copy_to_user(wrq->u.data.pointer, &CentralFreq, wrq->u.data.length);

                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, 
                ("Query::OID_802_11_WIFISPECTRUM_GET_CENTRAL_FREQ CentralFreq = %d\n", CentralFreq));
            }
            break;
#endif/*INTERNAL_CAPTURE_SUPPORT*/

   		default:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::unknown IOCTL's subcmd = 0x%08x, apidx=%d\n", cmd, apidx));
			Status = -EOPNOTSUPP;
			break;
    }

	return Status;
}

/*
    ==========================================================================
    Description:
        Set Country Code.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_CountryCode_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{

#ifdef EXT_BUILD_CHANNEL_LIST
	/* reset temp table status */
	pAd->CommonCfg.pChDesp = NULL;
	pAd->CommonCfg.DfsType = MAX_RD_REGION;
#endif /* EXT_BUILD_CHANNEL_LIST */

	if(strlen(arg) == 2)
	{
		NdisMoveMemory(pAd->CommonCfg.CountryCode, arg, 2);
		pAd->CommonCfg.bCountryFlag = TRUE;
	}
	else
	{
		NdisZeroMemory(pAd->CommonCfg.CountryCode,
				sizeof(pAd->CommonCfg.CountryCode));
		pAd->CommonCfg.bCountryFlag = FALSE;
	}

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_CountryCode_Proc::(bCountryFlag=%d, CountryCode=%s)\n", pAd->CommonCfg.bCountryFlag, pAd->CommonCfg.CountryCode));

	return TRUE;
}

#ifdef EXT_BUILD_CHANNEL_LIST
INT Set_ChGeography_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	ULONG Geography;

	Geography = simple_strtol(arg, 0, 10);
	if (Geography <= BOTH)
		pAd->CommonCfg.Geography = Geography;
	else
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Set_ChannelGeography_Proc::(wrong setting. 0: Out-door, 1: in-door, 2: both)\n"));

	pAd->CommonCfg.CountryCode[2] =
		(pAd->CommonCfg.Geography == BOTH) ? ' ' : ((pAd->CommonCfg.Geography == IDOR) ? 'I' : 'O');

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Set_ChannelGeography_Proc:: Geography = %s\n", pAd->CommonCfg.Geography == ODOR ? "out-door" : (pAd->CommonCfg.Geography == IDOR ? "in-door" : "both")));

	/* After Set ChGeography need invoke SSID change procedural again for Beacon update. */
	/* it's no longer necessary since APStartUp will rebuild channel again. */
	/*BuildChannelListEx(pAd); */

	return TRUE;
}
#endif /* EXT_BUILD_CHANNEL_LIST */


/*
    ==========================================================================
    Description:
        Set Country String.
        This command will not work, if the field of CountryRegion in eeprom is programmed.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_CountryString_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	INT   index = 0;
	INT   success = TRUE;
	RTMP_STRING name_buffer[40] = {0};
	BOOLEAN IsSupport5G = HcIsRfSupport(pAd,RFIC_5GHZ);
	BOOLEAN IsSupport2G = HcIsRfSupport(pAd,RFIC_24GHZ);

#ifdef EXT_BUILD_CHANNEL_LIST
	return -EOPNOTSUPP;
#endif /* EXT_BUILD_CHANNEL_LIST */

	if(strlen(arg) <= 38)
	{
		if (strlen(arg) < 4)
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Set_CountryString_Proc::Parameter of CountryString are too short !\n"));
			return FALSE;
		}

		for (index = 0; index < strlen(arg); index++)
		{
			if ((arg[index] >= 'a') && (arg[index] <= 'z'))
				arg[index] = toupper(arg[index]);
		}

		for (index = 0; index < NUM_OF_COUNTRIES; index++)
		{
			NdisZeroMemory(name_buffer, 40);
			snprintf(name_buffer, sizeof(name_buffer), "\"%s\"", (RTMP_STRING *) allCountry[index].pCountryName);

			if (strncmp((RTMP_STRING *) allCountry[index].pCountryName, arg, strlen(arg)) == 0)
				break;
			else if (strncmp(name_buffer, arg, strlen(arg)) == 0)
				break;
		}

		if (index == NUM_OF_COUNTRIES)
			success = FALSE;
	}
	else
	{
		success = FALSE;
	}

	if (success == TRUE)
	{
		if (pAd->CommonCfg.CountryRegion & 0x80)
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Set_CountryString_Proc::parameter of CountryRegion in eeprom is programmed \n"));
			success = FALSE;
		}
		else
		{
			success = FALSE;


			if (IsSupport2G)
			{
				if (allCountry[index].SupportGBand == TRUE)
				{
					pAd->CommonCfg.CountryRegion = (UCHAR) allCountry[index].RegDomainNum11G;
					success = TRUE;
				}
				else
				{
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("The Country are not Support G Band Channel\n"));
				}
			}

			if (IsSupport5G)
			{
				if (allCountry[index].SupportABand == TRUE)
				{
					pAd->CommonCfg.CountryRegionForABand = (UCHAR) allCountry[index].RegDomainNum11A;
					success = TRUE;
				}
				else
				{
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("The Country are not Support A Band Channel\n"));
				}
			}
		}
	}

	if (success == TRUE)
	{
		os_zero_mem(pAd->CommonCfg.CountryCode, sizeof(pAd->CommonCfg.CountryCode));
		os_move_mem(pAd->CommonCfg.CountryCode, allCountry[index].IsoName, 2);
		pAd->CommonCfg.CountryCode[2] = ' ';
		/* After Set ChGeography need invoke SSID change procedural again for Beacon update. */
		/* it's no longer necessary since APStartUp will rebuild channel again. */
		/*BuildChannelList(pAd); */

		pAd->CommonCfg.bCountryFlag = TRUE;

		/* if set country string, driver needs to be reset */
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_CountryString_Proc::(CountryString=%s CountryRegin=%d CountryCode=%s)\n",
							allCountry[index].pCountryName, pAd->CommonCfg.CountryRegion, pAd->CommonCfg.CountryCode));
	}
	else
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Set_CountryString_Proc::Parameters out of range\n"));
	}

	return success;
}


/*
    ==========================================================================
    Description:
        Set SSID
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_AP_SSID_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	INT success = FALSE;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	BSS_STRUCT *mbss;

	if(((pObj->ioctl_if < HW_BEACON_MAX_NUM)) && (strlen(arg) <= MAX_LEN_OF_SSID))
	{
		struct wifi_dev *wdev = NULL;
		UCHAR RfIC = 0;
		
		mbss = &pAd->ApCfg.MBSSID[pObj->ioctl_if];
		wdev = &mbss->wdev;
		RfIC = wmode_2_rfic(wdev->PhyMode);

		NdisZeroMemory(mbss->Ssid, MAX_LEN_OF_SSID);
		NdisMoveMemory(mbss->Ssid, arg, strlen(arg));
		mbss->SsidLen = (UCHAR)strlen(arg);
		success = TRUE;

		{
			/* If in detection mode, need to stop detect first. */
			if (pAd->CommonCfg.bIEEE80211H == FALSE)
			{
				APStopByRf(pAd, RfIC);
				APStartUpByRf(pAd, RfIC);
			}
			else
			{
				/* each mode has different restart method */
				if (pAd->Dot11_H.RDMode == RD_SILENCE_MODE)
				{
					APStopByRf(pAd, RfIC);
					APStartUpByRf(pAd, RfIC);
				}
				else if (pAd->Dot11_H.RDMode == RD_SWITCHING_MODE)
				{
				}
				else if (pAd->Dot11_H.RDMode == RD_NORMAL_MODE)
				{
					APStopByRf(pAd, RfIC);
					APStartUpByRf(pAd, RfIC);
				}
			}

			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(ra%d) Set_SSID_Proc::(Len=%d,Ssid=%s)\n", pObj->ioctl_if,
				mbss->SsidLen, mbss->Ssid));
		}
	}
	else
		success = FALSE;

	return success;
}


/*
    ==========================================================================
    Description:
        Set TxRate
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_TxRate_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	NdisZeroMemory(pAd->ApCfg.MBSSID[pObj->ioctl_if].DesiredRates, MAX_LEN_OF_SUPPORTED_RATES);

	pAd->ApCfg.MBSSID[pObj->ioctl_if].DesiredRatesIndex = simple_strtol(arg, 0, 10);
	/* todo RTMPBuildDesireRate(pAd, pObj->ioctl_if, pAd->ApCfg.MBSSID[pObj->ioctl_if].DesiredRatesIndex); */

	/*todo MlmeUpdateTxRates(pAd); */

	return TRUE;
}


/*
    ==========================================================================
    Description:
        Set BasicRate
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_BasicRate_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	ULONG BasicRateBitmap;

	BasicRateBitmap = (ULONG) simple_strtol(arg, 0, 10);

	if (BasicRateBitmap > 4095) /* (2 ^ MAX_LEN_OF_SUPPORTED_RATES) -1 */
		return FALSE;

	pAd->CommonCfg.BasicRateBitmap = BasicRateBitmap;
	pAd->CommonCfg.BasicRateBitmapOld = BasicRateBitmap;

	MlmeUpdateTxRates(pAd, FALSE, (UCHAR)pObj->ioctl_if);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_BasicRate_Proc::(BasicRateBitmap=0x%08lx)\n", pAd->CommonCfg.BasicRateBitmap));

	return TRUE;
}


/*
    ==========================================================================
    Description:
        Set Beacon Period
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_BeaconPeriod_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	USHORT BeaconPeriod;
	INT   success = FALSE;

	BeaconPeriod = (USHORT) simple_strtol(arg, 0, 10);
	if((BeaconPeriod >= 20) && (BeaconPeriod < 1024))
	{
		pAd->CommonCfg.BeaconPeriod = BeaconPeriod;
		success = TRUE;

#ifdef AP_QLOAD_SUPPORT
		/* re-calculate QloadBusyTimeThreshold */
		QBSS_LoadAlarmReset(pAd);
#endif /* AP_QLOAD_SUPPORT */
	}
	else
		success = FALSE;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_BeaconPeriod_Proc::(BeaconPeriod=%d)\n", pAd->CommonCfg.BeaconPeriod));

	return success;
}

/*
    ==========================================================================
    Description:
        Set Dtim Period
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_DtimPeriod_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	USHORT DtimPeriod;
	INT   success = FALSE;

	DtimPeriod = (USHORT) simple_strtol(arg, 0, 10);
	if((DtimPeriod >= 1) && (DtimPeriod <= 255))
	{
		pAd->ApCfg.DtimPeriod = DtimPeriod;
		success = TRUE;
	}
	else
		success = FALSE;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_DtimPeriod_Proc::(DtimPeriod=%d)\n", pAd->ApCfg.DtimPeriod));

	return success;
}



/*
    ==========================================================================
    Description:
        Disable/enable OLBC detection manually
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_OLBCDetection_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	switch (simple_strtol(arg, 0, 10))
	{
		case 0: /*enable OLBC detect */
			pAd->CommonCfg.DisableOLBCDetect = 0;
			break;
		case 1: /*disable OLBC detect */
			pAd->CommonCfg.DisableOLBCDetect = 1;
			break;
		default:  /*Invalid argument */
			return FALSE;
	}

	return TRUE;
}


/*
    ==========================================================================
    Description:
        Set WmmCapable Enable or Disable
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_AP_WmmCapable_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	BOOLEAN	bWmmCapable;
	POS_COOKIE	pObj= (POS_COOKIE)pAd->OS_Cookie;
	struct wifi_dev *wdev ;

	bWmmCapable = simple_strtol(arg, 0, 10);

	wdev = &pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev ;

	if (bWmmCapable == 1)
		wdev->bWmmCapable = TRUE;
	else if (bWmmCapable == 0)
		wdev->bWmmCapable = FALSE;
	else
		return FALSE;  /*Invalid argument */

	pAd->ApCfg.MBSSID[pObj->ioctl_if].bWmmCapableOrg = \
								pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev.bWmmCapable;

#ifdef RTL865X_FAST_PATH
	if (!isFastPathCapable(pAd)) {
		rtlairgo_fast_tx_unregister();
		rtl865x_extDev_unregisterUcastTxDev(pAd->net_dev);
	}
#endif
#ifdef DOT11_N_SUPPORT
	/*Sync with the HT relate info. In N mode, we should re-enable it */
	SetCommonHtVht(pAd,wdev);
#endif /* DOT11_N_SUPPORT */

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WmmCapable_Proc::(bWmmCapable=%d)\n",
		pObj->ioctl_if, wdev->bWmmCapable));

	return TRUE;
}


INT	Set_AP_PerMbssMaxStaNum_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;
	INT			apidx = pObj->ioctl_if;

	return ApCfg_Set_PerMbssMaxStaNum_Proc(pAd, apidx, arg);
}

/*
    ==========================================================================
    Description:
        Set session idle timeout
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_AP_IdleTimeout_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	return ApCfg_Set_IdleTimeout_Proc(pAd, arg);
}
/*
    ==========================================================================
    Description:
        Set No Forwarding Enable or Disable
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_NoForwarding_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	ULONG NoForwarding;

	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;

	NoForwarding = simple_strtol(arg, 0, 10);

	if (NoForwarding == 1)
		pAd->ApCfg.MBSSID[pObj->ioctl_if].IsolateInterStaTraffic = TRUE;
	else if (NoForwarding == 0)
		pAd->ApCfg.MBSSID[pObj->ioctl_if].IsolateInterStaTraffic = FALSE;
	else
		return FALSE;  /*Invalid argument */

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_NoForwarding_Proc::(NoForwarding=%ld)\n",
		pObj->ioctl_if, pAd->ApCfg.MBSSID[pObj->ioctl_if].IsolateInterStaTraffic));

	return TRUE;
}


/*
    ==========================================================================
    Description:
        Set No Forwarding between each SSID
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_NoForwardingBTNSSID_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	ULONG NoForwarding;

	NoForwarding = simple_strtol(arg, 0, 10);

	if (NoForwarding == 1)
		pAd->ApCfg.IsolateInterStaTrafficBTNBSSID = TRUE;
	else if (NoForwarding == 0)
		pAd->ApCfg.IsolateInterStaTrafficBTNBSSID = FALSE;
	else
		return FALSE;  /*Invalid argument */

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_NoForwardingBTNSSID_Proc::(NoForwarding=%ld)\n", pAd->ApCfg.IsolateInterStaTrafficBTNBSSID));

	return TRUE;
}


/*
    ==========================================================================
    Description:
        Set Hide SSID Enable or Disable
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_HideSSID_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	BOOLEAN bHideSsid;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	bHideSsid = simple_strtol(arg, 0, 10);

	if (bHideSsid == 1)
		bHideSsid = TRUE;
	else if (bHideSsid == 0)
		bHideSsid = FALSE;
	else
		return FALSE;  /*Invalid argument */

	if (pAd->ApCfg.MBSSID[pObj->ioctl_if].bHideSsid != bHideSsid)
	{
		pAd->ApCfg.MBSSID[pObj->ioctl_if].bHideSsid = bHideSsid;
	}

#ifdef WSC_V2_SUPPORT
	if (pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.WscV2Info.bEnableWpsV2)
		WscOnOff(pAd, pObj->ioctl_if, pAd->ApCfg.MBSSID[pObj->ioctl_if].bHideSsid);
#endif /* WSC_V2_SUPPORT */

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_HideSSID_Proc::(HideSSID=%d)\n", pObj->ioctl_if, pAd->ApCfg.MBSSID[pObj->ioctl_if].bHideSsid));

	return TRUE;
}

/*
    ==========================================================================
    Description:
        Set VLAN's ID field
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_VLANID_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	struct wifi_dev *wdev;


	wdev = &pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev;
	wdev->VLAN_VID = simple_strtol(arg, 0, 10);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_VLANID_Proc::(VLAN_VID=%d)\n",
				pObj->ioctl_if, wdev->VLAN_VID));

	return TRUE;
}


/*
    ==========================================================================
    Description:
        Set VLAN's priority field
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_VLANPriority_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev;

	wdev->VLAN_Priority = simple_strtol(arg, 0, 10);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_VLANPriority_Proc::(VLAN_Priority=%d)\n", pObj->ioctl_if, wdev->VLAN_Priority));

	return TRUE;
}


/*
    ==========================================================================
    Description:
        Set enable or disable carry VLAN in the air
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_VLAN_TAG_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	BOOLEAN	bVLAN_Tag;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	struct wifi_dev *wdev;


	bVLAN_Tag = simple_strtol(arg, 0, 10);

	if (bVLAN_Tag == 1)
		bVLAN_Tag = TRUE;
	else if (bVLAN_Tag == 0)
		bVLAN_Tag = FALSE;
	else
		return FALSE;  //Invalid argument

	wdev = &pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev;
	wdev->bVLAN_Tag = bVLAN_Tag;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_VLAN_TAG_Proc::(VLAN_Tag=%d)\n",
				pObj->ioctl_if, wdev->bVLAN_Tag));

	return TRUE;
}


INT	Set_AP_WpaMixPairCipher_Proc (
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	/*
		In WPA-WPA2 mix mode, it provides a more flexible cipher combination.
		-	WPA-AES and WPA2-TKIP
		-	WPA-AES and WPA2-TKIPAES
		-	WPA-TKIP and WPA2-AES
		-	WPA-TKIP and WPA2-TKIPAES
		-	WPA-TKIPAES and WPA2-AES
		-	WPA-TKIPAES and WPA2-TKIP
		-	WPA-TKIPAES and WPA2-TKIPAES (default)
	 */
	Set_SecAuthMode_Proc(pAd, arg);
	Set_SecEncrypType_Proc(pAd, arg);

	return TRUE;
}


/*
    ==========================================================================
    Description:
        Set WPA rekey interval value
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_AP_RekeyInterval_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR apidx = pObj->ioctl_if;
    struct wifi_dev *wdev = NULL;
    struct _SECURITY_CONFIG *pSecConfig = NULL;
    ULONG value_interval;

    if (apidx >= pAd->ApCfg.BssidNum)
        return FALSE;

    wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
    pSecConfig = &wdev->SecConfig;

    value_interval = simple_strtol(arg, 0, 10);

    if((value_interval >= 10) && (value_interval < MAX_GROUP_REKEY_INTERVAL))
        pSecConfig->GroupReKeyInterval = value_interval;
    else /*Default*/
        pSecConfig->GroupReKeyInterval = DEFAULT_GROUP_REKEY_INTERVAL;

    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(%s%d) GroupKey ReKeyInterval=%ld seconds\n",
        INF_MBSSID_DEV_NAME, apidx, pSecConfig->GroupReKeyInterval));

    return TRUE;
}

#ifdef SPECIFIC_TX_POWER_SUPPORT
INT Set_AP_PKT_PWR(
    IN  PRTMP_ADAPTER    pAd,
    IN  RTMP_STRING *arg)
{
        POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
        UCHAR           apidx = pObj->ioctl_if;
        INT input;
        input = simple_strtol(arg, 0, 10);

	/*
	  Tx_PWR_ADJ[3:0] From 0 to 7 is Positive & add with Tx Power (dB),
          From 8 to 15 is minus with Tx Power mapping to -16 to -2 (step by 2),
          Default value: 0.

	  [0x13BC]TX_ALC_MONITOR, 13:8
		  TX_ALC_REQ_ADJ TX ALC Req Saturated[5:0], unit (0.5dB)
	*/

	if ((input >= 0) && (input <= 15))
        	pAd->ApCfg.MBSSID[apidx].TxPwrAdj = input;
	else
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("AP[%d]->PktPwr: Out of Range\n"));

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("AP[%d]->PktPwr: %d\n", apidx, pAd->ApCfg.MBSSID[apidx].TxPwrAdj));

	return TRUE;
}
#endif /* SPECIFIC_TX_POWER_SUPPORT */
/*
    ==========================================================================
    Description:
        Set WPA rekey method
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_AP_RekeyMethod_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR apidx = pObj->ioctl_if;
    struct wifi_dev *wdev = NULL;
    struct _SECURITY_CONFIG *pSecConfig = NULL;

    if (apidx >= pAd->ApCfg.BssidNum)
        return FALSE;

    wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
    pSecConfig = &wdev->SecConfig;

    if (rtstrcasecmp(arg, "TIME") == TRUE)
        pSecConfig->GroupReKeyMethod = SEC_GROUP_REKEY_TIME;
    else if (rtstrcasecmp(arg, "PKT") == TRUE)
        pSecConfig->GroupReKeyMethod = SEC_GROUP_REKEY_PACKET;

    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(%s%d) GroupKey ReKeyMethod=%x\n",
        INF_MBSSID_DEV_NAME, apidx, pSecConfig->GroupReKeyMethod));

	return TRUE;
}

/*
    ==========================================================================
    Description:
        Set PMK-cache period
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_AP_PMKCachePeriod_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE 	pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR		apidx = pObj->ioctl_if;
	UINT32 val = simple_strtol(arg, 0, 10);

	pAd->ApCfg.MBSSID[apidx].PMKCachePeriod = val * 60 * OS_HZ;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(ra%d) Set_AP_PMKCachePeriod_Proc=%ld\n",
									apidx, pAd->ApCfg.MBSSID[apidx].PMKCachePeriod));

	return TRUE;
}


/*
    ==========================================================================
    Description:
        Set AssocReq RSSI Threshold to reject STA with weak signal.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_AP_ASSOC_REQ_RSSI_THRESHOLD(
    IN  PRTMP_ADAPTER    pAd,
    IN  RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR           apidx = pObj->ioctl_if;
	UINT j;
        CHAR rssi;
        rssi = simple_strtol(arg, 0, 10);

        if (rssi == 0)
        {
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Disable AP_ASSOC_REQ_RSSI_THRESHOLD\n"));
        }
        else if (rssi > 0 || rssi < -100)
        {
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_AP_ASSOC_REQ_RSSI_THRESHOLD Value Error.\n"));
                return FALSE;
        }


        pAd->ApCfg.MBSSID[apidx].AssocReqRssiThreshold = rssi;

        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(ra%d) Set_AP_ASSOC_REQ_RSSI_THRESHOLD=%d\n", apidx,
                                                                        pAd->ApCfg.MBSSID[apidx].AssocReqRssiThreshold ));

	for(j = BSS0; j < pAd->ApCfg.BssidNum; j++)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%d. ==> %d\n", j, pAd->ApCfg.MBSSID[j].AssocReqRssiThreshold ));
	}


        return TRUE;
}

/*
    ==========================================================================
    Description:
        Set lower limit for AP kicking out a STA.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_AP_KickStaRssiLow_Proc(
    IN  PRTMP_ADAPTER    pAd,
    IN  RTMP_STRING *arg)
{
        POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
        UCHAR           apidx = pObj->ioctl_if;
        UINT j;
        CHAR rssi;
        rssi = simple_strtol(arg, 0, 10);

        if (rssi == 0)
        {
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Disable RssiLowForStaKickOut Function\n"));
        }
        else if (rssi > 0 || rssi < -100)
        {
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RssiLowForStaKickOut Value Error.\n"));
                return FALSE;
        }


        pAd->ApCfg.MBSSID[apidx].RssiLowForStaKickOut = rssi;

        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(ra%d) RssiLowForStaKickOut=%d\n", apidx,
                                                                        pAd->ApCfg.MBSSID[apidx].RssiLowForStaKickOut ));

        for(j = BSS0; j < pAd->ApCfg.BssidNum; j++)
        {
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%d. ==> %d\n", j, pAd->ApCfg.MBSSID[j].RssiLowForStaKickOut ));
        }


        return TRUE;
}


/*
    ==========================================================================
    Description:
        Set Access ctrol policy
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_AccessPolicy_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	switch (simple_strtol(arg, 0, 10))
	{
		case 0: /*Disable */
			pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Policy = 0;
			break;
		case 1: /* Allow All, and ACL is positive. */
			pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Policy = 1;
			break;
		case 2: /* Reject All, and ACL is negative. */
			pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Policy = 2;
			break;
		default: /*Invalid argument */
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Set_AccessPolicy_Proc::Invalid argument (=%s)\n", arg));
			return FALSE;
	}

	/* check if the change in ACL affects any existent association */
	ApUpdateAccessControlList(pAd, pObj->ioctl_if);
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_AccessPolicy_Proc::(AccessPolicy=%ld)\n", pObj->ioctl_if, pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Policy));

	return TRUE;
}


/* Replaced by Set_ACLAddEntry_Proc() and Set_ACLClearAll_Proc() */

/*
    ==========================================================================
    Description:
        Add one entry or several entries(if allowed to)
        	into Access control mac table list
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ACLAddEntry_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	UCHAR					macAddr[MAC_ADDR_LEN];
/*	RT_802_11_ACL			acl; */
	RT_802_11_ACL			*pacl = NULL;
	RTMP_STRING *this_char;
	RTMP_STRING *value;
	INT						i, j;
	BOOLEAN					isDuplicate=FALSE;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	if (pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Num >= (MAX_NUM_OF_ACL_LIST - 1))
    {
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("The AccessControlList is full, and no more entry can join the list!\n"));
		return FALSE;
	}

	/* allocate memory */
	os_alloc_mem(NULL, (UCHAR **)&pacl, sizeof(RT_802_11_ACL));
	if (pacl == NULL)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: Allocate memory fail!!!\n", __FUNCTION__));
		return FALSE;
	}

	NdisZeroMemory(pacl, sizeof(RT_802_11_ACL));
	NdisMoveMemory(pacl, &pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList, sizeof(RT_802_11_ACL));

	while ((this_char = strsep((char **)&arg, ";")) != NULL)
	{
		if (*this_char == '\0')
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("An unnecessary delimiter entered!\n"));
			continue;
		}
		if (strlen(this_char) != 17)  /*Mac address acceptable format 01:02:03:04:05:06 length 17 */
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("illegal MAC address length!\n"));
			continue;
		}
        for (i=0, value = rstrtok(this_char,":"); value; value = rstrtok(NULL,":"))
		{
			if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) )
			{
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("illegal MAC address format or octet!\n"));
				/* Do not use "continue" to replace "break" */
				break;
			}
			AtoH(value, &macAddr[i++], 1);
		}

		if (i != MAC_ADDR_LEN)
		{
			continue;
		}

		/* Check if this entry is duplicate. */
		isDuplicate = FALSE;
		for (j=0; j<pacl->Num; j++)
		{
			if (memcmp(pacl->Entry[j].Addr, &macAddr, 6) == 0)
			{
				isDuplicate = TRUE;
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("You have added an entry before :\n"));
	        	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("The duplicate entry is %02x:%02x:%02x:%02x:%02x:%02x\n",
	        		macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]));
			}
		}

		if (!isDuplicate)
		{
			NdisMoveMemory(pacl->Entry[pacl->Num++].Addr, &macAddr, MAC_ADDR_LEN);
		}

		if (pacl->Num == MAX_NUM_OF_ACL_LIST)
	    {
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("The AccessControlList is full, and no more entry can join the list!\n"));
        	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("The last entry of ACL is %02x:%02x:%02x:%02x:%02x:%02x\n",
        		macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]));
			break;
		}
	}

	ASSERT(pacl->Num < MAX_NUM_OF_ACL_LIST);

	NdisZeroMemory(&pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList, sizeof(RT_802_11_ACL));
	NdisMoveMemory(&pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList, pacl, sizeof(RT_802_11_ACL));

	/* check if the change in ACL affects any existent association */
	ApUpdateAccessControlList(pAd, pObj->ioctl_if);
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::%s(Policy=%ld, Entry#=%ld)\n",
		__FUNCTION__ , pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Policy, pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Num));

#ifdef DBG
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("=============== Entry ===============\n"));
	for (i=0; i<pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Num; i++)
	{
		printk("Entry #%02d: ", i+1);
		for (j=0; j<MAC_ADDR_LEN; j++)
		   printk("%02X ", pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Entry[i].Addr[j]);
		printk("\n");
	}
#endif

	if (pacl != NULL)
		os_free_mem( pacl);

	return TRUE;
}


/*
    ==========================================================================
    Description:
        Delete one entry or several entries(if allowed to)
        	from Access control mac table list
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ACLDelEntry_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	UCHAR					macAddr[MAC_ADDR_LEN];
	UCHAR					nullAddr[MAC_ADDR_LEN];
	RT_802_11_ACL			acl;
	RTMP_STRING *this_char;
	RTMP_STRING *value;
	INT						i, j;
	BOOLEAN					isFound=FALSE;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	NdisZeroMemory(&acl, sizeof(RT_802_11_ACL));
	NdisMoveMemory(&acl, &pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList, sizeof(RT_802_11_ACL));
	NdisZeroMemory(nullAddr, MAC_ADDR_LEN);

	while ((this_char = strsep((char **)&arg, ";")) != NULL)
	{
		if (*this_char == '\0')
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("An unnecessary delimiter entered!\n"));
			continue;
		}
		if (strlen(this_char) != 17)  /*Mac address acceptable format 01:02:03:04:05:06 length 17 */
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("illegal MAC address length!\n"));
			continue;
		}

		for (i=0, value = rstrtok(this_char,":"); value; value = rstrtok(NULL,":"))
		{
			if ((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) )
			{
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("illegal MAC address format or octet!\n"));
				/* Do not use "continue" to replace "break" */
				break;
			}
			AtoH(value, &macAddr[i++], 1);
		}

		if (i != MAC_ADDR_LEN)
		{
			continue;
		}

		/* Check if this entry existed. */
		isFound = FALSE;
		for (j=0; j<acl.Num; j++)
		{
			if (memcmp(acl.Entry[j].Addr, &macAddr, MAC_ADDR_LEN) == 0)
			{
				isFound = TRUE;
				NdisZeroMemory(acl.Entry[j].Addr, MAC_ADDR_LEN);
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("The entry %02x:%02x:%02x:%02x:%02x:%02x founded will be deleted!\n",
	        		macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]));
			}
		}

		if (!isFound)
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("The entry %02x:%02x:%02x:%02x:%02x:%02x is not in the list!\n",
        		macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]));
		}
	}

	NdisZeroMemory(&pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList, sizeof(RT_802_11_ACL));
	pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Policy = acl.Policy;
	ASSERT(pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Num == 0);
	i = 0;

	for (j=0; j<acl.Num; j++)
	{
		if (memcmp(acl.Entry[j].Addr, &nullAddr, MAC_ADDR_LEN) == 0)
		{
			continue;
		}
		else
		{
			NdisMoveMemory(&(pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Entry[i++]), acl.Entry[j].Addr, MAC_ADDR_LEN);
		}
	}

	pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Num = i;
	ASSERT(acl.Num >= pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Num);

	/* check if the change in ACL affects any existent association */
	ApUpdateAccessControlList(pAd, pObj->ioctl_if);
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::%s(Policy=%ld, Entry#=%ld)\n",
		__FUNCTION__ , pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Policy, pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Num));

#ifdef DBG
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("=============== Entry ===============\n"));
	for (i=0; i<pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Num; i++)
	{
		printk("Entry #%02d: ", i+1);
		for (j=0; j<MAC_ADDR_LEN; j++)
		   printk("%02X ", pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Entry[i].Addr[j]);
		printk("\n");
	}
#endif
	return TRUE;
}


/* for ACL policy message */
#define ACL_POLICY_TYPE_NUM	3
char const *pACL_PolicyMessage[ACL_POLICY_TYPE_NUM] = {
	"the Access Control feature is disabled",						/* 0 : Disable */
	"only the following entries are allowed to join this BSS",			/* 1 : Allow */
	"all the following entries are rejected to join this BSS",			/* 2 : Reject */
};


/*
    ==========================================================================
    Description:
        Dump all the entries in the Access control
        	mac table list of a specified BSS
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ACLShowAll_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	RT_802_11_ACL			acl;
	BOOLEAN					bDumpAll=FALSE;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	INT						i, j;

	bDumpAll = simple_strtol(arg, 0, 10);

	if (bDumpAll == 1)
	{
		bDumpAll = TRUE;
	}
	else if (bDumpAll == 0)
	{
		bDumpAll = FALSE;
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("Your input is 0!\n"));
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("The Access Control List will not be dumped!\n"));
		return TRUE;
	}
	else
	{
		return FALSE;  /* Invalid argument */
	}

	NdisZeroMemory(&acl, sizeof(RT_802_11_ACL));
	NdisMoveMemory(&acl, &pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList, sizeof(RT_802_11_ACL));

	/* Check if the list is already empty. */
	if (acl.Num == 0)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("The Access Control List is empty!\n"));
		return TRUE;
	}

	ASSERT(((bDumpAll == 1) && (acl.Num > 0)));

	/* Show the corresponding policy first. */
	printk("=============== Access Control Policy ===============\n");
	printk("Policy is %ld : ", acl.Policy);
	printk("%s\n", pACL_PolicyMessage[acl.Policy]);

	/* Dump the entry in the list one by one */
	printk("===============  Access Control List  ===============\n");
	for (i=0; i<acl.Num; i++)
	{
		printk("Entry #%02d: ", i+1);
		for (j=0; j<MAC_ADDR_LEN; j++)
		   printk("%02X ", acl.Entry[i].Addr[j]);
		printk("\n");
	}

	return TRUE;
}


/*
    ==========================================================================
    Description:
        Clear all the entries in the Access control
        	mac table list of a specified BSS
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ACLClearAll_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
/*	RT_802_11_ACL			acl; */
	RT_802_11_ACL			*pacl = NULL;
	BOOLEAN					bClearAll=FALSE;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	bClearAll = simple_strtol(arg, 0, 10);

	if (bClearAll == 1)
	{
		bClearAll = TRUE;
	}
	else if (bClearAll == 0)
	{
		bClearAll = FALSE;
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("Your input is 0!\n"));
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("The Access Control List will be kept unchanged!\n"));
		return TRUE;
	}
	else
	{
		return FALSE;  /* Invalid argument */
	}

	/* allocate memory */
	os_alloc_mem(NULL, (UCHAR **)&pacl, sizeof(RT_802_11_ACL));
	if (pacl == NULL)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: Allocate memory fail!!!\n", __FUNCTION__));
		return FALSE;
	}

	NdisZeroMemory(pacl, sizeof(RT_802_11_ACL));
	NdisMoveMemory(pacl, &pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList, sizeof(RT_802_11_ACL));

	/* Check if the list is already empty. */
	if (pacl->Num == 0)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("The Access Control List is empty!\n"));
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("No need to clear the Access Control List!\n"));

		if (pacl != NULL)
			os_free_mem( pacl);

		return TRUE;
	}

	ASSERT(((bClearAll == 1) && (pacl->Num > 0)));

	/* Clear the entry in the list one by one */
	/* Keep the corresponding policy unchanged. */
	do
	{
		NdisZeroMemory(pacl->Entry[pacl->Num - 1].Addr, MAC_ADDR_LEN);
		pacl->Num -= 1;
	}while (pacl->Num > 0);

	ASSERT(pacl->Num == 0);

	NdisZeroMemory(&(pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList), sizeof(RT_802_11_ACL));
	NdisMoveMemory(&(pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList), pacl, sizeof(RT_802_11_ACL));

	/* check if the change in ACL affects any existent association */
	ApUpdateAccessControlList(pAd, pObj->ioctl_if);

	if (pacl != NULL)
		os_free_mem(pacl);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set::%s(Policy=%ld, Entry#=%ld)\n",
		__FUNCTION__, pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Policy, pAd->ApCfg.MBSSID[pObj->ioctl_if].AccessControlList.Num));

	return TRUE;
}

#ifdef DBG
static void _rtmp_hexdump(int level, const char *title, const UINT8 *buf,
			 size_t len, int show)
{
	size_t i;
	if (level < DebugLevel)
		return;
	printk("%s - hexdump(len=%lu):", title, (unsigned long) len);
	if (show) {
		for (i = 0; i < len; i++)
			printk(" %02x", buf[i]);
	} else {
		printk(" [REMOVED]");
	}
	printk("\n");
}

void rtmp_hexdump(int level, const char *title, const UINT8 *buf, size_t len)
{
	_rtmp_hexdump(level, title, buf, len, 1);
}
#endif




/*
    ==========================================================================
    Description:
        Reset statistics counter

    Arguments:
        pAdapter            Pointer to our adapter
        arg

    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/

#ifdef AP_SCAN_SUPPORT
/*
    ==========================================================================
    Description:
        Issue a site survey command to driver
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
               1.) iwpriv ra0 set site_survey
    ==========================================================================
*/

/*
    ==========================================================================
    Description:
        Issue a Auto-Channel Selection command to driver
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
               1.) iwpriv ra0 set AutoChannelSel=1
                   Ues the number of AP to choose
               2.) iwpriv ra0 set AutoChannelSel=2
                   Ues the False CCA count to choose
    ==========================================================================
*/
INT Set_AutoChannelSel_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	NDIS_802_11_SSID Ssid;

	NdisZeroMemory(&Ssid, sizeof(NDIS_802_11_SSID));
	Ssid.SsidLength = 0;
	if (strlen(arg) <= MAX_LEN_OF_SSID)
	{
		if (strlen(arg) > 0)
		{
			NdisMoveMemory(Ssid.Ssid, arg, strlen(arg));
			Ssid.SsidLength = strlen(arg);
		}
		else   /*ANY ssid */
		{
			Ssid.SsidLength = 0;
			memcpy(Ssid.Ssid, "", 0);
		}
	}
	if (strcmp(arg,"1") == 0)
		pAd->ApCfg.AutoChannelAlg = ChannelAlgApCnt;
	else if (strcmp(arg,"2") == 0)
		pAd->ApCfg.AutoChannelAlg = ChannelAlgCCA;
	else if (strcmp(arg,"3") == 0)
		pAd->ApCfg.AutoChannelAlg = ChannelAlgBusyTime;
	else
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Set_AutoChannelSel_Proc Alg isn't defined\n"));
		return FALSE;
	}
    
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_AutoChannelSel_Proc Alg=%d \n", pAd->ApCfg.AutoChannelAlg));

    if(pAd->ApCfg.AutoChannelAlg == ChannelAlgBusyTime)
    {  
        if (pAd->AutoChSelCtrl.p2GScanwdev)
        {
            AutoChSelScanStart(pAd, pAd->AutoChSelCtrl.p2GScanwdev);
        }    

        if(pAd->AutoChSelCtrl.p5GScanwdev) 
        {
            AutoChSelScanStart(pAd, pAd->AutoChSelCtrl.p5GScanwdev);
        }  
    }    
	else if (Ssid.SsidLength == 0)
		ApSiteSurvey(pAd, &Ssid, SCAN_PASSIVE, TRUE);
	else
		ApSiteSurvey(pAd, &Ssid, SCAN_ACTIVE, TRUE);

    return TRUE;

}


/*
    ==========================================================================
    Description:
        Set a periodic check time for auto channel selection (unit: hour)
   Arguments:
       pAdapter                    Pointer to our adapter

    Return Value:
        TRUE if success, FALSE otherwise

    Note:
        Usage:
               iwpriv ra0 set ACSCheckTime=3  (unit: hour)

    ==========================================================================
*/
INT Set_AutoChannelSelCheckTime_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
   UINT8 Hour = simple_strtol(arg, 0, 10);

   pAd->ApCfg.ACSCheckTime = Hour*3600; /* Hour to second */
   pAd->ApCfg.ACSCheckCount = 0; /* Reset counter */
   MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,
        ("%s(): ACSCheckTime=%u seconds(%u hours)\n",
        __FUNCTION__, pAd->ApCfg.ACSCheckTime, Hour));
   return TRUE;
}
#endif /* AP_SCAN_SUPPORT */

INT Show_DriverInfo_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
        ("driver version: %s (%s %s) .\n", AP_DRIVER_VERSION, __DATE__, __TIME__));

#ifdef CONFIG_ANDES_SUPPORT
	if ((pAd->chipCap.MCUType & ANDES) == ANDES)
    {
		UINT32 loop = 0;
		RTMP_CHIP_CAP *cap = &pAd->chipCap;

		if (pAd->chipCap.need_load_fw)
        {
			USHORT fw_ver, build_ver;
			fw_ver = (*(cap->FwImgBuf + 11) << 8) | (*(cap->FwImgBuf + 10));
			build_ver = (*(cap->FwImgBuf + 9) << 8) | (*(cap->FwImgBuf + 8));

            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
                ("fw version:%d.%d.%02d ", (fw_ver & 0xf000) >> 8,
                (fw_ver & 0x0f00) >> 8, fw_ver & 0x00ff));
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
                                ("build:%x\n", build_ver));
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
                                        ("build time:"));

			for (loop = 0; loop < 16; loop++)
            {
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
                            ("%c", *(cap->FwImgBuf + 16 + loop)));
            }
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("\n"));
		}

		if (pAd->chipCap.need_load_rom_patch)
        {
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
                                ("rom patch version = \n"));

			for (loop = 0; loop < 4; loop++)
            {
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
                        ("%c", *(cap->rom_patch + 24 + loop)));
            }
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("\n"));

			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("build time = \n"));

			for (loop = 0; loop < 16; loop++)
            {
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
                            ("%c", *(cap->rom_patch + loop)));
            }
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("\n"));
		}
	}
#endif

#if defined(MT7603_FPGA) || defined(MT7628_FPGA)  || defined(MT7636_FPGA) || defined(MT7637_FPGA)
	if ((IS_MT7603(pAd) || IS_MT7628(pAd) || IS_MT76x6(pAd) || IS_MT7637(pAd))
                                            && pAd->chipCap.hif_type == HIF_MT)
    {
		UINT32 mac_val, ver, date_code, rev;

		RTMP_IO_READ32(pAd, 0x2700, &ver);
		RTMP_IO_READ32(pAd, 0x2704, &rev);
		RTMP_IO_READ32(pAd, 0x2708, &date_code);
		RTMP_IO_READ32(pAd, 0x21f8, &mac_val);
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
            ("##########################################\n"));
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
                                ("MT7603 FPGA Version:\n"));

		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
            ("\tFGPA1: Code[0x700]:0x%x, [0x704]:0x%x, [0x708]:0x%x\n",
					                        ver, rev, date_code));
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
                ("\tFPGA2: Version[0x21f8]:0x%x\n", mac_val));
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
            ("##########################################\n"));
	}
#endif /* MT7603_FPGA */

    return TRUE;
}

static MAC_TABLE_ENTRY debugContent[MAX_LEN_OF_MAC_TABLE];

INT Show_StaCount_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	INT i;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR	temp_str[20];
	INT		temp_len = sizeof(temp_str);
	struct wifi_dev *wdev = get_wdev_by_ioctl_idx_and_iftype(pAd,pObj->ioctl_if,pObj->ioctl_if_type);
	ADD_HT_INFO_IE *addht;

	if(!wdev)
		return FALSE;

	addht = wlan_operate_get_addht(wdev);

	printk("\n");

#ifdef DOT11_N_SUPPORT
	printk("HT Operating Mode : %d\n", addht->AddHtInfo2.OperaionMode);
	printk("\n");
#endif /* DOT11_N_SUPPORT */

	printk("\n%-19s%-4s%-12s%-12s%-12s%-12s%-12s%-12s\n",
		   "MAC", "AID","TxPackets","RxPackets","TxBytes","RxBytes", "TP(Tx)", "TP(Rx)");

	for (i=0; VALID_UCAST_ENTRY_WCID(pAd, i); i++)
	{
		PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];
		if ((IS_ENTRY_CLIENT(pEntry) || IS_ENTRY_APCLI(pEntry) || IS_ENTRY_REPEATER(pEntry))
			&& (pEntry->Sst == SST_ASSOC))
		{
			BOOLEAN	print_red = FALSE;
#ifdef MAC_REPEATER_SUPPORT
			BOOLEAN	print_orig_mac = FALSE;
			PREPEATER_CLIENT_ENTRY pReptCliEntry_tmp = NULL;
#endif
			if (IS_ENTRY_CLIENT(pEntry)) {
				if (memcmp(pAd->MonitorAddr,pEntry->Addr,MAC_ADDR_LEN) == 0) {
					print_red = TRUE;
				}
			}
#ifdef MAC_REPEATER_SUPPORT
			if (IS_ENTRY_REPEATER(pEntry)) {
				INT CliIdx;
				RTMP_CHIP_CAP *cap = &pAd->chipCap;
				for (CliIdx = 0; CliIdx < GET_MAX_REPEATER_ENTRY_NUM(cap); CliIdx++) {
					PREPEATER_CLIENT_ENTRY 		pReptCliEntry;
					pReptCliEntry = &pAd->ApCfg.pRepeaterCliPool[CliIdx];
					if (pReptCliEntry->MacTabWCID == pEntry->wcid) {
						if (memcmp(pAd->MonitorAddr,pReptCliEntry->OriginalAddress,MAC_ADDR_LEN) == 0) {
							print_red = TRUE;
							break;
						}
					}
				}
				for (CliIdx = 0; CliIdx < GET_MAX_REPEATER_ENTRY_NUM(cap); CliIdx++) {
					pReptCliEntry_tmp = &pAd->ApCfg.pRepeaterCliPool[CliIdx];
					if (pReptCliEntry_tmp->MacTabWCID == pEntry->wcid) {
						print_orig_mac = TRUE;
						break;
					}
				}
			}
#endif
#define RED(_text)  "\033[1;31m"_text"\033[0m"
			if (print_red == TRUE) {
				printk(RED("%02X:%02X:%02X:%02X:%02X:%02X  "),
					   pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
					   pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5]);
			} else {
			printk("%02X:%02X:%02X:%02X:%02X:%02X  ",
				pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
				pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5]);
			}
			printk("%-4d", (int)pEntry->Aid);
			if ((ULONG)debugContent[i].TxPackets.QuadPart != (ULONG)pEntry->TxPackets.QuadPart) {
				snprintf(temp_str,temp_len,"*%lu", (ULONG)pEntry->TxPackets.QuadPart);
				printk(RED("%-12s"), temp_str);
			} else {
			printk("%-12lu",(ULONG)pEntry->TxPackets.QuadPart);
			}
			if ((ULONG)debugContent[i].RxPackets.QuadPart != (ULONG)pEntry->RxPackets.QuadPart) {
				snprintf(temp_str,temp_len,"*%lu", (ULONG)pEntry->RxPackets.QuadPart);
				printk(RED("%-12s"), temp_str);
			} else {
			printk("%-12lu", (ULONG)pEntry->RxPackets.QuadPart);
			}
			printk("%-12lu", (ULONG)pEntry->TxBytes);
			printk("%-12lu", (ULONG)pEntry->RxBytes);
			snprintf(temp_str,temp_len,"%lu%s", (pEntry->AvgTxBytes >> 17), "Mbps");/* (n Bytes x 8) / (1024*1024) = (n >> 17) */
			printk("%-12s", temp_str);
			snprintf(temp_str,temp_len,"%lu%s", (pEntry->AvgRxBytes >> 17), "Mbps");/* (n Bytes x 8) / (1024*1024) = (n >> 17) */
			printk("%-12s", temp_str);
			if (IS_ENTRY_CLIENT(pEntry))
				printk("[S]%d ", HcGetBandByWdev(pEntry->wdev));
			if (IS_ENTRY_APCLI(pEntry))
				printk("[A]%d ", HcGetBandByWdev(pEntry->wdev));
			if (IS_ENTRY_REPEATER(pEntry))
				printk("[R]%d ", HcGetBandByWdev(pEntry->wdev));
#ifdef MAC_REPEATER_SUPPORT
			if ((print_orig_mac == TRUE) && (pReptCliEntry_tmp != NULL)) {
				printk("%02X:%02X:%02X:%02X:%02X:%02X (%d,%02d)",
					   PRINT_MAC(pReptCliEntry_tmp->OriginalAddress),
					   pReptCliEntry_tmp->MatchApCliIdx,
					   pReptCliEntry_tmp->MatchLinkIdx);
			}
#endif
			{/* add ba drop counter information*/
				INT	tid_idx;
				if (VALID_UCAST_ENTRY_WCID(pAd, pEntry->wcid)) {
					for (tid_idx=0; tid_idx < NUM_OF_TID; tid_idx++)
					{
						if (pEntry->BARecWcidArray[tid_idx] != 0) {
							PBA_REC_ENTRY pBAEntry = NULL;
							USHORT Idx;
							Idx = pEntry->BARecWcidArray[tid_idx];
							pBAEntry = &pAd->BATable.BARecEntry[Idx];
							printk("[%d]%lu ",tid_idx,pBAEntry->nDropPacket);
						}
					}
				}
			}
			printk("\n");
			debugContent[i] = *pEntry;
		}
	}

	return TRUE;
}


INT Show_RAInfo_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
#ifdef PRE_ANT_SWITCH
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("PreAntSwitch: %d\n", pAd->CommonCfg.PreAntSwitch));
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("PreAntSwitchRSSI: %d\n", pAd->CommonCfg.PreAntSwitchRSSI));
#endif /* PRE_ANT_SWITCH */

#ifdef CFO_TRACK
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("CFOTrack: %d\n", pAd->CommonCfg.CFOTrack));
#endif /* CFO_TRACK */


#ifdef NEW_RATE_ADAPT_SUPPORT
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("LowTrafficThrd: %d\n", pAd->CommonCfg.lowTrafficThrd));
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("TrainUpRule: %d\n", pAd->CommonCfg.TrainUpRule));
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("TrainUpRuleRSSI: %d\n", pAd->CommonCfg.TrainUpRuleRSSI));
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("TrainUpLowThrd: %d\n", pAd->CommonCfg.TrainUpLowThrd));
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("TrainUpHighThrd: %d\n", pAd->CommonCfg.TrainUpHighThrd));
#endif /* NEW_RATE_ADAPT_SUPPORT */

#ifdef STREAM_MODE_SUPPORT
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("StreamMode: %d\n", pAd->CommonCfg.StreamMode));
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("StreamModeMCS: 0x%04x\n", pAd->CommonCfg.StreamModeMCS));
#endif /* STREAM_MODE_SUPPORT */
#ifdef TXBF_SUPPORT
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("ITxBfEn: %d\n", pAd->CommonCfg.RegTransmitSetting.field.ITxBfEn));
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("ITxBfTimeout: %ld\n", pAd->CommonCfg.ITxBfTimeout));
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("ETxBfTimeout: %ld\n", pAd->CommonCfg.ETxBfTimeout));
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("ETxBfEnCond: %ld\n", pAd->CommonCfg.ETxBfEnCond));
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("ETxBfNoncompress: %d\n", pAd->CommonCfg.ETxBfNoncompress));
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("ETxBfIncapable: %d\n", pAd->CommonCfg.ETxBfIncapable));
#endif /* TXBF_SUPPORT */

#ifdef DBG_CTRL_SUPPORT
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("DebugFlags: 0x%lx\n", pAd->CommonCfg.DebugFlags));
#endif /* DBG_CTRL_SUPPORT */

	return TRUE;
}


#ifdef RTMP_MAC_PCI
#ifdef DBG_DIAGNOSE
INT Set_DiagOpt_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	ULONG diagOpt;
	/*POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie; */

	diagOpt = simple_strtol(arg, 0, 10);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("DiagOpt=%ld!\n", diagOpt));

	return TRUE;
}


INT Set_BDInfo_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	INT32 i, QueIdx=0;
	UINT32 RegValue;
	RXD_STRUC *pRxD;
	TXD_STRUC *pTxD;
#ifdef RT_BIG_ENDIAN
	RXD_STRUC *pDestRxD, *pDestTxD;
	RXD_STRUC RxD, TxD;
#endif /* RT_BIG_ENDIAN */
	RTMP_TX_RING *pTxRing = &pAd->PciHif.TxRing[QueIdx];
	RTMP_MGMT_RING *pMgmtRing = &pAd->MgmtRing;
	RTMP_RX_RING *pRxRing = &pAd->PciHif.RxRing[0];
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	struct wifi_dev *wdev = get_wdev_by_ioctl_idx_and_iftype(pAd,pObj->ioctl_if,pObj->ioctl_if_type);
	ADD_HT_INFO_IE *addht;
	if(!wdev)
		return FALSE;

	addht = wlan_operate_get_addht(wdev);

	printk("\n");

#ifdef DOT11_N_SUPPORT
	printk("HT Operating Mode : %d\n",addht->AddHtInfo2.OperaionMode);
#endif /* DOT11_N_SUPPORT */
	printk("\n");
	printk("[Tx]: SwFreeIdx=%d, CpuIdx=%d, DmaIdx=%d\n",
		pAd->PciHif.TxRing[QueIdx].TxSwFreeIdx,
		pAd->PciHif.TxRing[QueIdx].TxCpuIdx,
		pAd->PciHif.TxRing[QueIdx].TxDmaIdx);

#ifdef RT_BIG_ENDIAN
	pDestTxD = (TXD_STRUC *) pTxRing->Cell[pAd->TxRing[QueIdx].TxSwFreeIdx].AllocVa;
	TxD = *pDestTxD;
	pTxD = &TxD;
	RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#else
	pTxD = (TXD_STRUC *) pTxRing->Cell[pAd->TxRing[QueIdx].TxSwFreeIdx].AllocVa;
#endif /* RT_BIG_ENDIAN */
	hex_dump("Tx SwFreeIdx Descriptor", (char *)pTxD, 16);
	printk("pTxD->DMADONE = %x\n", pTxD->DMADONE);

#ifdef RT_BIG_ENDIAN
	pDestTxD = (TXD_STRUC *) pTxRing->Cell[pAd->TxRing[QueIdx].TxCpuIdx].AllocVa;
	TxD = *pDestTxD;
	pTxD = &TxD;
	RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#else
	pTxD = (TXD_STRUC *) pTxRing->Cell[pAd->TxRing[QueIdx].TxCpuIdx].AllocVa;
#endif /* RT_BIG_ENDIAN */
	hex_dump("Tx CpuIdx Descriptor", (char *)pTxD, 16);
	printk("pTxD->DMADONE = %x\n", pTxD->DMADONE);

#ifdef RT_BIG_ENDIAN
	pDestTxD = (TXD_STRUC *) pTxRing->Cell[pAd->TxRing[QueIdx].TxDmaIdx].AllocVa;
	TxD = *pDestTxD;
	pTxD = &TxD;
	RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#else
	pTxD = (TXD_STRUC *) pTxRing->Cell[pAd->TxRing[QueIdx].TxDmaIdx].AllocVa;
#endif /* RT_BIG_ENDIAN */
	hex_dump("Tx DmaIdx Descriptor", (char *)pTxD, 16);
	printk("pTxD->DMADONE = %x\n", pTxD->DMADONE);

	printk("[Mgmt]: SwFreeIdx=%d, CpuIdx=%d, DmaIdx=%d\n",
		pAd->MgmtRing.TxSwFreeIdx,
		pAd->MgmtRing.TxCpuIdx,
		pAd->MgmtRing.TxDmaIdx);

#ifdef RT_BIG_ENDIAN
	pDestTxD = (TXD_STRUC *) pMgmtRing->Cell[pAd->MgmtRing.TxSwFreeIdx].AllocVa;
	TxD = *pDestTxD;
	pTxD = &TxD;
	RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#else
	pTxD = (TXD_STRUC *) pMgmtRing->Cell[pAd->MgmtRing.TxSwFreeIdx].AllocVa;
#endif /* RT_BIG_ENDIAN */
	hex_dump("Mgmt SwFreeIdx Descriptor", (char *)pTxD, 16);

#ifdef RT_BIG_ENDIAN
	pDestTxD = (TXD_STRUC *) pMgmtRing->Cell[pAd->MgmtRing.TxCpuIdx].AllocVa;
	TxD = *pDestTxD;
	pTxD = &TxD;
	RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#else
	pTxD = (TXD_STRUC *) pMgmtRing->Cell[pAd->MgmtRing.TxCpuIdx].AllocVa;
#endif /* RT_BIG_ENDIAN */
	hex_dump("Mgmt CpuIdx Descriptor", (char *)pTxD, 16);

#ifdef RT_BIG_ENDIAN
	pDestTxD = (TXD_STRUC *) pMgmtRing->Cell[pAd->MgmtRing.TxDmaIdx].AllocVa;
	TxD = *pDestTxD;
	pTxD = &TxD;
	RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#else
	pTxD = (TXD_STRUC *) pMgmtRing->Cell[pAd->MgmtRing.TxDmaIdx].AllocVa;
#endif /* RT_BIG_ENDIAN */
	hex_dump("Mgmt DmaIdx Descriptor", (char *)pTxD, 16);
	printk("\n");
	printk("[Rx]:  SwRedIdx=%d, CpuIdx=%d, DmaIdx=%d\n",
		pAd->PciHif.RxRing[0].RxSwReadIdx,
		pAd->PciHif.RxRing[0].RxCpuIdx,
		pAd->PciHif.RxRing[0].RxDmaIdx);

#ifdef RT_BIG_ENDIAN
	pDestRxD = (RXD_STRUC *) pRxRing->Cell[pAd->RxRing[0].RxSwReadIdx].AllocVa;
	RxD = *pDestRxD;
	pRxD = &RxD;
	RTMPDescriptorEndianChange((PUCHAR)pRxD, TYPE_RXD);
#else
	pRxD = (RXD_STRUC *) pRxRing->Cell[pAd->RxRing[0].RxSwReadIdx].AllocVa;
#endif /* RT_BIG_ENDIAN */
	hex_dump("RX SwRedIdx Descriptor", (char *)pRxD, 16);

#ifdef RT_BIG_ENDIAN
	pDestRxD = (RXD_STRUC *) pRxRing->Cell[pAd->RxRing[0].RxCpuIdx].AllocVa;
	RxD = *pDestRxD;
	pRxD = &RxD;
	RTMPDescriptorEndianChange((PUCHAR)pRxD, TYPE_RXD);
#else
	pRxD = (RXD_STRUC *) pRxRing->Cell[pAd->RxRing[0].RxCpuIdx].AllocVa;
#endif /* RT_BIG_ENDIAN */
	hex_dump("RX RxCupIdx Descriptor", (char *)pRxD, 16);

#ifdef RT_BIG_ENDIAN
	pDestRxD = (RXD_STRUC *) pRxRing->Cell[pAd->RxRing[0].RxDmaIdx].AllocVa;
	RxD = *pDestRxD;
	pRxD = &RxD;
	RTMPDescriptorEndianChange((PUCHAR)pRxD, TYPE_RXD);
#else
	pRxD = (RXD_STRUC *) pRxRing->Cell[pAd->RxRing[0].RxDmaIdx].AllocVa;
#endif /* RT_BIG_ENDIAN */
	hex_dump("RX RxDmaIdx Descritpro", (char *)pRxD, 16);
	printk("\n%-19s%-4s%-4s%-7s%-7s%-7s%-10s%-6s%-6s%-6s%-6s\n",
		"MAC", "AID", "PSM", "RSSI0", "RSSI1", "RSSI2", "PhMd", "BW", "MCS", "SGI", "STBC");

	for (i=0; VALID_UCAST_ENTRY_WCID(pAd, i); i++)
	{
		PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];
		if (IS_ENTRY_CLIENT(pEntry) && (pEntry->Sst == SST_ASSOC))
		{
			printk("%02X:%02X:%02X:%02X:%02X:%02X  ",
				pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
				pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5]);
			printk("%-4d", (int)pEntry->Aid);
			printk("%-4d", (int)pEntry->PsMode);
			printk("%-7d", pEntry->RssiSample.AvgRssi[0]);
			printk("%-7d", pEntry->RssiSample.AvgRssi[1]);
			printk("%-7d", pEntry->RssiSample.AvgRssi[2]);


			printk("%-10s", get_phymode_str(pEntry->HTPhyMode.field.MODE));
			printk("%-6s", get_bw_str(pEntry->HTPhyMode.field.BW));
			printk("%-6d", pEntry->HTPhyMode.field.MCS);
			printk("%-6d", pEntry->HTPhyMode.field.ShortGI);
			printk("%-6d", pEntry->HTPhyMode.field.STBC);
			printk("%-10d, %d, %d\n", pEntry->FIFOCount, pEntry->DebugTxCount, pEntry->DebugTxCount-pEntry->FIFOCount);

			printk("\n");
		}
	}

	return TRUE;
}


INT Set_diag_cond_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
    UINT32 cond;

    cond = simple_strtol(arg, 0, 10);
    pAd->DiagStruct.diag_cond = cond;

    return TRUE;
}


INT Show_Diag_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	RtmpDiagStruct *pDiag = NULL;
	UCHAR i, start, stop, McsIdx, SwQNumLevel, TxDescNumLevel, que_idx;
	unsigned long irqFlags;
	UCHAR McsMaxIdx = MAX_MCS_SET;
#ifdef DOT11_VHT_AC
	UCHAR vht_mcs_max_idx = MAX_VHT_MCS_SET;
#endif /* DOT11_VHT_AC */

	os_alloc_mem(pAd, (UCHAR **)&pDiag, sizeof(RtmpDiagStruct));
	if (!pDiag) {
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s():AllocMem failed!\n", __FUNCTION__));
		return FALSE;
	}

	RTMP_IRQ_LOCK(&pAd->irq_lock, irqFlags);
	NdisMoveMemory(pDiag, &pAd->DiagStruct, sizeof(RtmpDiagStruct));
	RTMP_IRQ_UNLOCK(&pAd->irq_lock, irqFlags);

#ifdef DOT11N_SS3_SUPPORT
	if (IS_RT2883(pAd) || IS_RT3883(pAd))
		McsMaxIdx = 24;
#endif /* DOT11N_SS3_SUPPORT */

	if (pDiag->inited == FALSE)
		goto done;

	start = pDiag->ArrayStartIdx;
	stop = pDiag->ArrayCurIdx;
	printk("Start=%d, stop=%d!\n\n", start, stop);
	printk("    %-12s", "Time(Sec)");
	for(i=1; i< DIAGNOSE_TIME; i++)
	{
		printk("%-7d", i);
	}
	printk("\n    -------------------------------------------------------------------------------\n");
	printk("Tx Info:\n");
	printk("    %-12s", "TxDataCnt\n");
	for (que_idx = 0; que_idx < WMM_NUM_OF_AC; que_idx++) {
		printk("\tQueue[%d]:", que_idx);
		for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
		{
			printk("%-7d", pDiag->diag_info[i].TxDataCnt[que_idx]);
		}
		printk("\n");
	}
	printk("\n    %-12s", "TxFailCnt");
	for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
	{
		printk("%-7d", pDiag->diag_info[i].TxFailCnt);
	}

#ifdef DBG_TX_AGG_CNT
	printk("\n    %-12s", "TxAggCnt");
	for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
	{
		printk("%-7d", pDiag->diag_info[i].TxAggCnt);
	}
	printk("\n");
#endif /* DBG_TX_AGG_CNT */

#ifdef DBG_TXQ_DEPTH
	printk("DeQueue Info:\n");

	printk("\n    %-12s\n", "DeQueueFunc Called Distribution");
	printk("\t");
	for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
		printk("%-8d", pDiag->diag_info[i].deq_called);

	printk("\n    %-12s\n", "DeQueueRound(Per-Call) Distribution");
	printk("\t");
	for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
		printk("%-8d", pDiag->diag_info[i].deq_round);

	printk("\n    %-12s\n", "DeQueueCount(Per-Round) Distribution");
	for (SwQNumLevel = 0 ; SwQNumLevel < 9; SwQNumLevel++)
	{
		if (SwQNumLevel == 8)
			printk("\t>%-5d",  SwQNumLevel);
		else
			printk("\t%-6d", SwQNumLevel);
		for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
			printk("%-7d", pDiag->diag_info[i].deq_cnt[SwQNumLevel]);

		printk("\n");
	}

	printk("\n    %-12s%d", "Sw-Queued TxSwQCnt for WCID ", pDiag->wcid);
	for (que_idx = 0; que_idx < WMM_NUM_OF_AC; que_idx++) {
		printk("\n    %s[%d]\n", "Queue", que_idx);
		for (SwQNumLevel = 0 ; SwQNumLevel < 9; SwQNumLevel++)
		{
			if (SwQNumLevel == 8)
				printk("\t>%-5d",  SwQNumLevel);
			else
				printk("\t%-6d", SwQNumLevel);
			for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
				printk("%-7d", pDiag->diag_info[i].TxSWQueCnt[que_idx][SwQNumLevel]);

			printk("\n");
		}
		printk("	%-12s\n", "TxEnQFailCnt");
		for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
		{
			printk("\t%-7d", pDiag->diag_info[i].enq_fall_cnt[que_idx]);
		}
		printk("\n");
	}

	printk("\n	  %s\n", "DeQueFailedCnt:Reason NotTxResource");
	for (que_idx = 0; que_idx < WMM_NUM_OF_AC; que_idx++) {
		printk("\n    %s[%d]:", "Queue", que_idx);
		for (i = start; i != stop;	i = (i+1) % DIAGNOSE_TIME)
			printk("\t%-7d", pDiag->diag_info[i].deq_fail_no_resource_cnt[que_idx]);
	}
#endif /* DBG_TXQ_DEPTH */

#ifdef DBG_TX_RING_DEPTH
	printk("\n    %-12s\n", "DMA-Queued TxDescCnt");
	for (que_idx = 0; que_idx < WMM_NUM_OF_AC; que_idx++) {
		printk("\n Queue[%d]\n", que_idx);
		for(TxDescNumLevel = 0; TxDescNumLevel < 16; TxDescNumLevel++)
		{
			if (TxDescNumLevel == 15)
				printk("\t>%-5d",  TxDescNumLevel);
			else
				printk("\t%-6d",  TxDescNumLevel);
			for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
				printk("%-7d", pDiag->diag_info[i].TxDescCnt[que_idx][TxDescNumLevel]);

			printk("\n");
		}
	}
#endif /* DBG_TX_RING_DEPTH */

#ifdef MT_MAC
#ifdef DBG_PSE_DEPTH
	printk("\n    %-12s\n", "PSE-Queued TxPageCnt in unit of Packets (mod 13)");
	{
		UCHAR pg_level;
		for(pg_level = 0; pg_level < 50; pg_level++)
		{
			if (pg_level == 49)
				printk("\t>%-5d",  pg_level);
			else
				printk("\t%-6d",  pg_level);
			for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
				printk("%-7d", pDiag->diag_info[i].pse_pg_cnt[pg_level]);

			printk("\n");
		}
	}
#endif /* DBG_PSE_DEPTH */
#endif /* MT_MAC */

#ifdef DOT11_N_SUPPORT
#ifdef DBG_TX_AGG_CNT
	printk("\n    %-12s\n", "Tx-Agged AMPDUCnt");
	for (McsIdx =0 ; McsIdx < 16; McsIdx++)
	{
		printk("\t%-6d", (McsIdx+1));
		for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
		{
			printk("%d(%d%%)  ", pDiag->diag_info[i].TxAMPDUCnt[McsIdx],
								pDiag->diag_info[i].TxAMPDUCnt[McsIdx] ? (pDiag->diag_info[i].TxAMPDUCnt[McsIdx] * 100 / pDiag->diag_info[i].TxAggCnt) : 0);
		}
		printk("\n");
	}
#endif /* DBG_TX_AGG_CNT */
#endif /* DOT11_N_SUPPORT */

#ifdef DBG_TX_MCS
	printk("\n    %-12s\n", "TxMcsCnt_HT");
	for (McsIdx =0 ; McsIdx < McsMaxIdx; McsIdx++)
	{
		printk("\t%-6d", McsIdx);
		for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
			printk("%-7d", pDiag->diag_info[i].TxMcsCnt_HT[McsIdx]);
		printk("\n");
		}

#ifdef DOT11_VHT_AC
	printk("\n    %-12s\n", "TxMcsCnt_VHT");
	for (McsIdx =0 ; McsIdx < vht_mcs_max_idx; McsIdx++)
	{
		printk("\t%-6d", McsIdx);
		for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
			printk("%-7d", pDiag->diag_info[i].TxMcsCnt_VHT[McsIdx]);
		printk("\n");
	}
#endif /* DOT11_VHT_AC */
#endif /* DBG_TX_MCS */



	printk("Rx Info\n");
	printk("    %-12s", "RxDataCnt");
	for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
	{
		printk("%-7d", pDiag->diag_info[i].RxDataCnt);
	}
	printk("\n    %-12s", "RxCrcErrCnt");
	for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
	{
		printk("%-7d", pDiag->diag_info[i].RxCrcErrCnt);
	}

#ifdef DBG_RX_MCS
	printk("\n    %-12s\n", "RxMcsCnt_HT");
	for (McsIdx =0 ; McsIdx < McsMaxIdx; McsIdx++)
	{
		printk("\t%-6d", McsIdx);
		for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
		{
			printk("%-7d", pDiag->diag_info[i].RxMcsCnt_HT[McsIdx]);
		}
		printk("\n");
	}

#ifdef DOT11_VHT_AC
	printk("\n    %-12s\n", "RxMcsCnt_VHT");
	for (McsIdx =0 ; McsIdx < vht_mcs_max_idx; McsIdx++)
	{
		printk("\t%-6d", McsIdx);
		for (i = start; i != stop;  i = (i+1) % DIAGNOSE_TIME)
		{
			printk("%-7d", pDiag->diag_info[i].RxMcsCnt_VHT[McsIdx]);
		}
		printk("\n");
	}
#endif /* DOT11_VHT_AC */

#endif /* DBG_RX_MCS */

	printk("\n-------------\n");

done:
	os_free_mem( pDiag);
	return TRUE;
}
#endif /* DBG_DIAGNOSE */
#endif /* RTMP_MAC_PCI */


INT Show_Sat_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	/* Sanity check for calculation of sucessful count */
	printk("TransmitCountFromOS = %d\n", pAd->WlanCounters[0].TransmitCountFrmOs.u.LowPart);
	printk("TransmittedFragmentCount = %lld\n", (INT64)pAd->WlanCounters[0].TransmittedFragmentCount.u.LowPart + pAd->WlanCounters[0].MulticastTransmittedFrameCount.QuadPart);
	printk("MulticastTransmittedFrameCount = %d\n", pAd->WlanCounters[0].MulticastTransmittedFrameCount.u.LowPart);
	printk("FailedCount = %d\n", pAd->WlanCounters[0].FailedCount.u.LowPart);
	printk("RetryCount = %d\n", pAd->WlanCounters[0].RetryCount.u.LowPart);
	printk("MultipleRetryCount = %d\n", pAd->WlanCounters[0].MultipleRetryCount.u.LowPart);
	printk("RTSSuccessCount = %d\n", pAd->WlanCounters[0].RTSSuccessCount.u.LowPart);
	printk("RTSFailureCount = %d\n", pAd->WlanCounters[0].RTSFailureCount.u.LowPart);
	printk("ACKFailureCount = %d\n", pAd->WlanCounters[0].ACKFailureCount.u.LowPart);
	printk("FrameDuplicateCount = %d\n", pAd->WlanCounters[0].FrameDuplicateCount.u.LowPart);
	printk("ReceivedFragmentCount = %d\n", pAd->WlanCounters[0].ReceivedFragmentCount.u.LowPart);
	printk("MulticastReceivedFrameCount = %d\n", pAd->WlanCounters[0].MulticastReceivedFrameCount.u.LowPart);
	printk("Rx drop due to out of resource  = %ld\n", (ULONG)pAd->Counters8023.RxNoBuffer);
#ifdef DBG
	printk("RealFcsErrCount = %d\n", pAd->RalinkCounters.RealFcsErrCount.u.LowPart);
#else
	printk("FCSErrorCount = %d\n", pAd->WlanCounters[0].FCSErrorCount.u.LowPart);
	printk("FrameDuplicateCount.LowPart = %d\n", pAd->WlanCounters[0].FrameDuplicateCount.u.LowPart / 100);
#endif
	printk("TransmittedFrameCount = %d\n", pAd->WlanCounters[0].TransmittedFragmentCount.u.LowPart);
	printk("WEPUndecryptableCount = %d\n", pAd->WlanCounters[0].WEPUndecryptableCount.u.LowPart);
#ifdef OUI_CHECK_SUPPORT
#ifdef DBDC_MODE
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("RxHWLookupWcidMismatchCount = %ld\n", 
		(ULONG)pAd->WlanCounters[0].RxHWLookupWcidErrCount.u.LowPart + (ULONG)pAd->WlanCounters[1].RxHWLookupWcidErrCount.u.LowPart));
#else
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("RxHWLookupWcidMismatchCount = %ld\n", 
		(ULONG)pAd->WlanCounters[0].RxHWLookupWcidErrCount.u.LowPart));

#endif /*DBDC_MODE*/
#endif /*OUI_CHECK_SUPPORT*/

#ifdef DOT11_N_SUPPORT
	printk("\n===Some 11n statistics variables: \n");
	/* Some 11n statistics variables */
	printk("TxAMSDUCount = %ld\n", (ULONG)pAd->RalinkCounters.TxAMSDUCount.u.LowPart);
	printk("RxAMSDUCount = %ld\n", (ULONG)pAd->RalinkCounters.RxAMSDUCount.u.LowPart);
	printk("TransmittedAMPDUCount = %ld\n", (ULONG)pAd->RalinkCounters.TransmittedAMPDUCount.u.LowPart);
	printk("TransmittedMPDUsInAMPDUCount = %ld\n", (ULONG)pAd->RalinkCounters.TransmittedMPDUsInAMPDUCount.u.LowPart);
	printk("TransmittedOctetsInAMPDUCount = %ld\n", (ULONG)pAd->RalinkCounters.TransmittedOctetsInAMPDUCount.u.LowPart);
	printk("MPDUInReceivedAMPDUCount = %ld\n", (ULONG)pAd->RalinkCounters.MPDUInReceivedAMPDUCount.u.LowPart);
#ifdef DOT11N_DRAFT3
	printk("fAnyStaFortyIntolerant=%d\n", pAd->MacTab.fAnyStaFortyIntolerant);
#endif /* DOT11N_DRAFT3 */
#endif /* DOT11_N_SUPPORT */

{
	int apidx;

	for (apidx=0; apidx < pAd->ApCfg.BssidNum; apidx++)
	{
		printk("-- IF-ra%d -- \n", apidx);
		printk("Packets Received = %ld\n", (ULONG)pAd->ApCfg.MBSSID[apidx].RxCount);
		printk("Packets Sent = %ld\n", (ULONG)pAd->ApCfg.MBSSID[apidx].TxCount);
		printk("Bytes Received = %ld\n", (ULONG)pAd->ApCfg.MBSSID[apidx].ReceivedByteCount);
		printk("Byte Sent = %ld\n", (ULONG)pAd->ApCfg.MBSSID[apidx].TransmittedByteCount);
		printk("Error Packets Received = %ld\n", (ULONG)pAd->ApCfg.MBSSID[apidx].RxErrorCount);
		printk("Drop Received Packets = %ld\n", (ULONG)pAd->ApCfg.MBSSID[apidx].RxDropCount);

#ifdef WSC_INCLUDED
		if (pAd->ApCfg.MBSSID[apidx].WscControl.WscConfMode != WSC_DISABLE)
		{
			WSC_CTRL *pWscCtrl;

			pWscCtrl = &pAd->ApCfg.MBSSID[apidx].WscControl;
			printk("WscInfo:\n"
					"\tWscConfMode=%d\n"
					"\tWscMode=%s\n"
					"\tWscConfStatus=%d\n"
					"\tWscPinCode=%d\n"
					"\tWscState=0x%x\n"
					"\tWscStatus=0x%x\n",
					pWscCtrl->WscConfMode,
					((pWscCtrl->WscMode == WSC_PIN_MODE) ? "PIN" : "PBC"),
					pWscCtrl->WscConfStatus, pWscCtrl->WscEnrolleePinCode,
					pWscCtrl->WscState, pWscCtrl->WscStatus);
		}
#endif /* WSC_INCLUDED */

		printk("-- IF-ra%d end -- \n", apidx);
	}
}

{
	int i, j, k, maxMcs = MAX_MCS_SET -1;
	PMAC_TABLE_ENTRY pEntry;

#ifdef DOT11N_SS3_SUPPORT
	if (IS_RT2883(pAd) || IS_RT3883(pAd))
		maxMcs = 23;
#endif /* DOT11N_SS3_SUPPORT */

	for (i=0; VALID_UCAST_ENTRY_WCID(pAd, i); i++)
	{
		pEntry = &pAd->MacTab.Content[i];
		if (IS_ENTRY_CLIENT(pEntry) && (pEntry->Sst == SST_ASSOC))
		{
			printk("\n%02x:%02x:%02x:%02x:%02x:%02x - ", PRINT_MAC(pEntry->Addr));
			printk("%-4d\n", (int)pEntry->Aid);

			for (j=maxMcs; j>=0; j--)
			{
				if ((pEntry->TXMCSExpected[j] != 0) || (pEntry->TXMCSFailed[j] !=0))
				{
					printk("MCS[%02d]: Expected %u, Successful %u (%d%%), Failed %u\n",
						   j, pEntry->TXMCSExpected[j], pEntry->TXMCSSuccessful[j],
						   pEntry->TXMCSExpected[j] ? (100*pEntry->TXMCSSuccessful[j])/pEntry->TXMCSExpected[j] : 0,
						   pEntry->TXMCSFailed[j]);
					for(k=maxMcs; k>=0; k--)
					{
						if (pEntry->TXMCSAutoFallBack[j][k] != 0)
						{
							printk("\t\t\tAutoMCS[%02d]: %u (%d%%)\n", k, pEntry->TXMCSAutoFallBack[j][k],
								   (100*pEntry->TXMCSAutoFallBack[j][k])/pEntry->TXMCSExpected[j]);
						}
					}
				}
			}
		}
	}

}

#ifdef DOT11_N_SUPPORT
	/* Display Tx Aggregation statistics */
	DisplayTxAgg(pAd);
#endif /* DOT11_N_SUPPORT */

	return TRUE;
}



INT Show_Sat_Reset_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	/* Sanity check for calculation of sucessful count */

	printk("TransmittedFragmentCount = %lld\n", (INT64)pAd->WlanCounters[0].TransmittedFragmentCount.u.LowPart + pAd->WlanCounters[0].MulticastTransmittedFrameCount.QuadPart);
	printk("MulticastTransmittedFrameCount = %d\n", pAd->WlanCounters[0].MulticastTransmittedFrameCount.u.LowPart);
	printk("FailedCount = %d\n", pAd->WlanCounters[0].FailedCount.u.LowPart);
	printk("RetryCount = %d\n", pAd->WlanCounters[0].RetryCount.u.LowPart);
	printk("MultipleRetryCount = %d\n", pAd->WlanCounters[0].MultipleRetryCount.u.LowPart);
	printk("RTSSuccessCount = %d\n", pAd->WlanCounters[0].RTSSuccessCount.u.LowPart);
	printk("RTSFailureCount = %d\n", pAd->WlanCounters[0].RTSFailureCount.u.LowPart);
	printk("ACKFailureCount = %d\n", pAd->WlanCounters[0].ACKFailureCount.u.LowPart);
	printk("FrameDuplicateCount = %d\n", pAd->WlanCounters[0].FrameDuplicateCount.u.LowPart);
	printk("ReceivedFragmentCount = %d\n", pAd->WlanCounters[0].ReceivedFragmentCount.u.LowPart);
	printk("MulticastReceivedFrameCount = %d\n", pAd->WlanCounters[0].MulticastReceivedFrameCount.u.LowPart);
	printk("Rx drop due to out of resource  = %ld\n", (ULONG)pAd->Counters8023.RxNoBuffer);
#ifdef DBG
	printk("RealFcsErrCount = %d\n", pAd->RalinkCounters.RealFcsErrCount.u.LowPart);
#else
	printk("FCSErrorCount = %d\n", pAd->WlanCounters[0].FCSErrorCount.u.LowPart);
	printk("FrameDuplicateCount.LowPart = %d\n", pAd->WlanCounters[0].FrameDuplicateCount.u.LowPart / 100);
#endif
	printk("TransmittedFrameCount = %d\n", pAd->WlanCounters[0].TransmittedFrameCount.u.LowPart);
	printk("WEPUndecryptableCount = %d\n", pAd->WlanCounters[0].WEPUndecryptableCount.u.LowPart);

	pAd->WlanCounters[0].TransmittedFragmentCount.u.LowPart = 0;
	pAd->WlanCounters[0].MulticastTransmittedFrameCount.u.LowPart = 0;
	pAd->WlanCounters[0].FailedCount.u.LowPart = 0;
	pAd->WlanCounters[0].RetryCount.u.LowPart = 0;
	pAd->WlanCounters[0].MultipleRetryCount.u.LowPart = 0;
	pAd->WlanCounters[0].RTSSuccessCount.u.LowPart = 0;
	pAd->WlanCounters[0].RTSFailureCount.u.LowPart = 0;
	pAd->WlanCounters[0].ACKFailureCount.u.LowPart = 0;
	pAd->WlanCounters[0].FrameDuplicateCount.u.LowPart = 0;
	pAd->WlanCounters[0].ReceivedFragmentCount.u.LowPart = 0;
	pAd->WlanCounters[0].MulticastReceivedFrameCount.u.LowPart = 0;
	pAd->Counters8023.RxNoBuffer = 0;
#ifdef DBG
	pAd->RalinkCounters.RealFcsErrCount.u.LowPart = 0;
#else
	pAd->WlanCounters[0].FCSErrorCount.u.LowPart = 0;
	pAd->WlanCounters[0].FrameDuplicateCount.u.LowPart = 0;
#endif

	pAd->WlanCounters[0].TransmittedFrameCount.u.LowPart = 0;
	pAd->WlanCounters[0].WEPUndecryptableCount.u.LowPart = 0;

	{
		int i, j, k, maxMcs = 15;
		PMAC_TABLE_ENTRY pEntry;

#ifdef DOT11N_SS3_SUPPORT
		if (IS_RT2883(pAd) || IS_RT3883(pAd))
			maxMcs = 23;
#endif /* DOT11N_SS3_SUPPORT */

		for (i=0; VALID_UCAST_ENTRY_WCID(pAd, i); i++)
		{
			pEntry = &pAd->MacTab.Content[i];
			if (IS_ENTRY_CLIENT(pEntry) && (pEntry->Sst == SST_ASSOC))
			{

				printk("\n%02X:%02X:%02X:%02X:%02X:%02X - ",
					   pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
					   pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5]);
				printk("%-4d\n", (int)pEntry->Aid);

				for (j = maxMcs; j >= 0; j--)
				{
					if ((pEntry->TXMCSExpected[j] != 0) || (pEntry->TXMCSFailed[j] !=0))
					{
						printk("MCS[%02d]: Expected %u, Successful %u (%d%%), Failed %u\n",
							   j, pEntry->TXMCSExpected[j], pEntry->TXMCSSuccessful[j],
							   pEntry->TXMCSExpected[j] ? (100*pEntry->TXMCSSuccessful[j])/pEntry->TXMCSExpected[j] : 0,
							   pEntry->TXMCSFailed[j]
							   );
						for(k = maxMcs; k >= 0; k--)
						{
							if (pEntry->TXMCSAutoFallBack[j][k] != 0)
							{
								printk("\t\t\tAutoMCS[%02d]: %u (%d%%)\n", k, pEntry->TXMCSAutoFallBack[j][k],
									   (100*pEntry->TXMCSAutoFallBack[j][k])/pEntry->TXMCSExpected[j]);
							}
						}
					}
				}
			}
			for (j = 0; j < (maxMcs + 1); j++)
			{
				pEntry->TXMCSExpected[j] = 0;
				pEntry->TXMCSSuccessful[j] = 0;
				pEntry->TXMCSFailed[j] = 0;
				for(k = maxMcs; k >= 0; k--)
				{
					pEntry->TXMCSAutoFallBack[j][k] = 0;
				}
			}
		}
	}
#ifdef DOT11_N_SUPPORT
	/* Display Tx Aggregation statistics */
	DisplayTxAgg(pAd);
#endif /* DOT11_N_SUPPORT */

	return TRUE;
}


#ifdef MAT_SUPPORT
INT Show_MATTable_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	extern VOID dumpIPMacTb(MAT_STRUCT *pMatCfg, int index);
	extern NDIS_STATUS dumpSesMacTb(MAT_STRUCT *pMatCfg, int hashIdx);
	extern NDIS_STATUS dumpUidMacTb(MAT_STRUCT *pMatCfg, int hashIdx);
	extern NDIS_STATUS dumpIPv6MacTb(MAT_STRUCT *pMatCfg, int hashIdx);

	dumpIPMacTb(&pAd->MatCfg, -1);
	dumpSesMacTb(&pAd->MatCfg, -1);
	dumpUidMacTb(&pAd->MatCfg, -1);
	dumpIPv6MacTb(&pAd->MatCfg, -1);

	printk("Default BroadCast Address=%02x:%02x:%02x:%02x:%02x:%02x!\n", BROADCAST_ADDR[0], BROADCAST_ADDR[1],
			BROADCAST_ADDR[2], BROADCAST_ADDR[3], BROADCAST_ADDR[4], BROADCAST_ADDR[5]);
	return TRUE;
}
#endif /* MAT_SUPPORT */


#ifdef DOT1X_SUPPORT
/*
    ==========================================================================
    Description:
        UI should not call this function, it only used by 802.1x daemon
	Arguments:
	    pAd		Pointer to our adapter
	    wrq		Pointer to the ioctl argument
    ==========================================================================
*/
VOID RTMPIoctlAddPMKIDCache(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	UCHAR				apidx;
	NDIS_AP_802_11_KEY 	*pKey;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	apidx =	(UCHAR) pObj->ioctl_if;

	pKey = (PNDIS_AP_802_11_KEY) wrq->u.data.pointer;

	if (IS_AKM_WPA2_Entry(&pAd->ApCfg.MBSSID[apidx].wdev))
	{
		if(pKey->KeyLength == 32)
		{
			UCHAR	digest[80], PMK_key[20], macaddr[MAC_ADDR_LEN];

			/* Calculate PMKID */
			NdisMoveMemory(&PMK_key[0], "PMK Name", 8);
			NdisMoveMemory(&PMK_key[8], pAd->ApCfg.MBSSID[apidx].wdev.bssid, MAC_ADDR_LEN);
			NdisMoveMemory(&PMK_key[14], pKey->addr, MAC_ADDR_LEN);
			RT_HMAC_SHA1(pKey->KeyMaterial, PMK_LEN, PMK_key, 20, digest, SHA1_DIGEST_SIZE);

			NdisMoveMemory(macaddr, pKey->addr, MAC_ADDR_LEN);
			RTMPAddPMKIDCache(&pAd->ApCfg.PMKIDCache, apidx, macaddr, digest, pKey->KeyMaterial);

			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WPA2(pre-auth):(%02x:%02x:%02x:%02x:%02x:%02x)Calc PMKID=%02x:%02x:%02x:%02x:%02x:%02x\n",
				pKey->addr[0],pKey->addr[1],pKey->addr[2],pKey->addr[3],pKey->addr[4],pKey->addr[5],digest[0],digest[1],digest[2],digest[3],digest[4],digest[5]));
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("PMK =%02x:%02x:%02x:%02x-%02x:%02x:%02x:%02x\n",pKey->KeyMaterial[0],pKey->KeyMaterial[1],
				pKey->KeyMaterial[2],pKey->KeyMaterial[3],pKey->KeyMaterial[4],pKey->KeyMaterial[5],pKey->KeyMaterial[6],pKey->KeyMaterial[7]));
		}
		else
            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Set::RT_OID_802_11_WPA2_ADD_PMKID_CACHE ERROR or is wep key \n"));
	}

    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("<== RTMPIoctlAddPMKIDCache\n"));
}


/*
    ==========================================================================
    Description:
        UI should not call this function, it only used by 802.1x daemon
	Arguments:
	    pAd		Pointer to our adapter
	    wrq		Pointer to the ioctl argument
    ==========================================================================
*/
VOID RTMPIoctlStaticWepCopy(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	MAC_TABLE_ENTRY  *pEntry;
	UCHAR	MacAddr[MAC_ADDR_LEN];
	UCHAR			apidx;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	apidx =	(UCHAR) pObj->ioctl_if;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("RTMPIoctlStaticWepCopy-IF(ra%d)\n", apidx));

	if (wrq->u.data.length != sizeof(MacAddr))
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("RTMPIoctlStaticWepCopy: the length isn't match (%d)\n", wrq->u.data.length));
		return;
	}
	else
	{
		UINT32 len;

	        len = copy_from_user(&MacAddr, wrq->u.data.pointer, wrq->u.data.length);
	        pEntry = MacTableLookup(pAd, MacAddr);
	        if (!pEntry)
	        {
	            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("RTMPIoctlStaticWepCopy: the mac address isn't match\n"));
	            return;
	        }
	        else
	        {
	            UCHAR	KeyIdx;

	            KeyIdx = pAd->ApCfg.MBSSID[apidx].wdev.SecConfig.PairwiseKeyId;

	            /* need to copy the default shared-key to pairwise key table for this entry in 802.1x mode */
	            if (pAd->ApCfg.MBSSID[apidx].wdev.SecConfig.WepKey[KeyIdx].KeyLen == 0)
	            {
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("ERROR: Can not get Default shared-key (index-%d)\n", KeyIdx));
			return;
	            }
	            else
	            {
			ASIC_SEC_INFO Info = {0};

			pEntry->SecConfig.AKMMap = pAd->ApCfg.MBSSID[apidx].wdev.SecConfig.AKMMap;
			pEntry->SecConfig.PairwiseCipher = pAd->ApCfg.MBSSID[apidx].wdev.SecConfig.PairwiseCipher;
			pEntry->SecConfig.PairwiseKeyId = pAd->ApCfg.MBSSID[apidx].wdev.SecConfig.PairwiseKeyId;
			NdisMoveMemory(pEntry->SecConfig.WepKey, pAd->ApCfg.MBSSID[apidx].wdev.SecConfig.WepKey, sizeof(SEC_KEY_INFO)*SEC_KEY_NUM);

			/* Set key material to Asic */
			os_zero_mem(&Info, sizeof(ASIC_SEC_INFO));
			Info.Operation = SEC_ASIC_ADD_PAIRWISE_KEY;
			Info.Direction = SEC_ASIC_KEY_BOTH;
			Info.Wcid = pEntry->wcid;
			Info.BssIndex = pEntry->func_tb_idx;
			Info.Cipher = pEntry->SecConfig.PairwiseCipher;
			Info.KeyIdx = pEntry->SecConfig.PairwiseKeyId;
			os_move_mem(&Info.PeerAddr[0], pEntry->Addr, MAC_ADDR_LEN);

			HW_ADDREMOVE_KEYTABLE(pAd, &Info);

			/* Update WCID attribute table and IVEIV table */
			RTMPSetWcidSecurityInfo(pAd,
								pEntry->func_tb_idx,
								Info.KeyIdx,
								pEntry->SecConfig.PairwiseCipher,
								pEntry->wcid,
								PAIRWISEKEYTABLE);
	           }
            }
	}
    return;
}

/*
    ==========================================================================
    Description:
        UI should not call this function, it only used by 802.1x daemon
	Arguments:
	    pAd		Pointer to our adapter
	    wrq		Pointer to the ioctl argument
    ==========================================================================
*/
VOID RTMPIoctlSetIdleTimeout(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	MAC_TABLE_ENTRY  		*pEntry;
	PDOT1X_IDLE_TIMEOUT		pIdleTime;

	if (wrq->u.data.length != sizeof(DOT1X_IDLE_TIMEOUT))
	{
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s : the length is mis-match\n", __FUNCTION__));
        return;
    }

	pIdleTime = (PDOT1X_IDLE_TIMEOUT)wrq->u.data.pointer;

	if ((pEntry = MacTableLookup(pAd, pIdleTime->StaAddr)) == NULL)
    {
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s : the entry is empty\n", __FUNCTION__));
        return;
    }
	else
	{
		pEntry->NoDataIdleCount = 0;
		// TODO: shiang-usw,  remove upper setting becasue we need to migrate to tr_entry!
		pAd->MacTab.tr_entry[pEntry->wcid].NoDataIdleCount = 0;

		pEntry->StaIdleTimeout = pIdleTime->idle_timeout;
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s : Update Idle-Timeout(%d) from dot1x daemon\n",
									__FUNCTION__, pEntry->StaIdleTimeout));
	}

	return;
}


VOID RTMPIoctlQueryStaAid(
        IN      PRTMP_ADAPTER   pAd,
        IN      RTMP_IOCTL_INPUT_STRUCT *wrq)
{
	DOT1X_QUERY_STA_AID macBuf;
	MAC_TABLE_ENTRY *pEntry = NULL;

	if (wrq->u.data.length != sizeof(DOT1X_QUERY_STA_AID))
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s : the length is mis-match\n", __FUNCTION__));
        	return;
	}
	else
	{
		if (copy_from_user(&macBuf, wrq->u.data.pointer, wrq->u.data.length))
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: copy_to_user() fail\n", __FUNCTION__));
			return;
		}
		pEntry = MacTableLookup(pAd, macBuf.StaAddr);

		if (pEntry != NULL)
		{
			wrq->u.data.length = sizeof(DOT1X_QUERY_STA_AID);
			macBuf.aid = pEntry->Aid;
			if (copy_to_user(wrq->u.data.pointer, &macBuf, wrq->u.data.length))
			{
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: copy_to_user() fail\n", __FUNCTION__));
			}

			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_DOT1X_QUERY_STA_AID(%02x:%02x:%02x:%02x:%02x:%02x, AID=%d)\n",
						PRINT_MAC(macBuf.StaAddr), macBuf.aid));
		}
		else
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_DOT1X_QUERY_STA_AID(%02x:%02x:%02x:%02x:%02x:%02x, Not Found)\n",
					PRINT_MAC(macBuf.StaAddr)));
		}
	}
}

#ifdef RADIUS_ACCOUNTING_SUPPORT
VOID RTMPIoctlQueryStaData(
        IN      PRTMP_ADAPTER   pAd,
        IN      RTMP_IOCTL_INPUT_STRUCT *wrq)
{
	DOT1X_QUERY_STA_DATA macBuf;
	MAC_TABLE_ENTRY *pEntry = NULL;

	if (wrq->u.data.length != sizeof(DOT1X_QUERY_STA_DATA))
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s : the length is mis-match\n", __FUNCTION__));
        return;
	}
	else
	{
		copy_from_user(&macBuf, wrq->u.data.pointer, wrq->u.data.length);
		pEntry = MacTableLookup(pAd, macBuf.StaAddr);

		if (pEntry != NULL)
		{
			wrq->u.data.length = sizeof(DOT1X_QUERY_STA_DATA);
			macBuf.rx_bytes = pEntry->RxBytes;
			macBuf.tx_bytes = pEntry->TxBytes;
			macBuf.rx_packets = pEntry->RxPackets.u.LowPart;
			macBuf.tx_packets = pEntry->TxPackets.u.LowPart;
			
			if (copy_to_user(wrq->u.data.pointer, &macBuf, wrq->u.data.length))
			{
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: copy_to_user() fail\n", __FUNCTION__));				
			}
		}
		else
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::OID_802_DOT1X_QUERY_STA_AID(%02x:%02x:%02x:%02x:%02x:%02x, Not Found)\n",
					PRINT_MAC(macBuf.StaAddr)));			
		}
	}
}
#endif /*RADIUS_ACCOUNTING_SUPPORT*/

#endif /* DOT1X_SUPPORT */


#if defined(DBG) ||(defined(BB_SOC)&&defined(CONFIG_ATE))



/*
    ==========================================================================
    Description:
        Read / Write BBP
Arguments:
    pAdapter                    Pointer to our adapter
    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
               1.) iwpriv ra0 bbp               ==> read all BBP
               2.) iwpriv ra0 bbp 1             ==> read BBP where RegID=1
               3.) iwpriv ra0 bbp 1=10		    ==> write BBP R1=0x10
    ==========================================================================
*/
VOID RTMPAPIoctlBBP(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
#ifdef RTMP_BBP
	RTMP_STRING *this_char, *value;
	UCHAR regBBP = 0;
	RTMP_STRING *mpool, *msg; /*msg[2048]; */
	RTMP_STRING *arg; /*arg[255]; */
	RTMP_STRING *ptr;
	INT bbpId;
	LONG bbpValue;
	BOOLEAN bIsPrintAllBBP = FALSE, bAllowDump, bCopyMsg;
	INT argLen;
#endif /* RTMP_BBP */



#ifdef RTMP_BBP
	os_alloc_mem(NULL, (UCHAR **)&mpool, sizeof(CHAR)*(MAX_BBP_MSG_SIZE+256+12));
	if (mpool == NULL) {
		return;
	}

	NdisZeroMemory(mpool, MAX_BBP_MSG_SIZE+256+12);
	msg = (RTMP_STRING *)((ULONG)(mpool+3) & (ULONG)~0x03);
	arg = (RTMP_STRING *)((ULONG)(msg+MAX_BBP_MSG_SIZE+3) & (ULONG)~0x03);

	bAllowDump = ((wrq->u.data.flags & RTPRIV_IOCTL_FLAG_NODUMPMSG) == RTPRIV_IOCTL_FLAG_NODUMPMSG) ? FALSE : TRUE;
	bCopyMsg = ((wrq->u.data.flags & RTPRIV_IOCTL_FLAG_NOSPACE) == RTPRIV_IOCTL_FLAG_NOSPACE) ? FALSE : TRUE;
	argLen = strlen((char *)(wrq->u.data.pointer));


	if (argLen > 0)
	{
		NdisMoveMemory(arg, wrq->u.data.pointer, (argLen > 255) ? 255 : argLen);
		ptr = arg;
		sprintf(msg, "\n");
		/* Parsing Read or Write */
		while ((this_char = strsep((char **)&ptr, ",")) != NULL)
		{
			if (!*this_char)
				continue;

			if ((value = strchr(this_char, '=')) != NULL)
				*value++ = 0;

			if (!value || !*value)
			{ /*Read */
				if (sscanf(this_char, "%d", &(bbpId)) == 1)
				{
					if (bbpId <= pAdapter->chipCap.MaxNumOfBbpId)
					{
#ifdef CONFIG_ATE
						/*
							In RT2860 ATE mode, we do not load 8051 firmware.
							We must access BBP directly.
							For RT2870 ATE mode, ATE_BBP_IO_WRITE8(/READ8)_BY_REG_ID are redefined.
						*/
						if (ATE_ON(pAdapter))
						{
							ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP);
							/* Sync with QA for comparation */
							sprintf(msg+strlen(msg), "%03d = %02X\n", bbpId, regBBP);
						}
						else
#endif /* CONFIG_ATE */
						{
							/* according to Andy, Gary, David require. */
							/* the command bbp shall read BBP register directly for dubug. */
							BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP);
							sprintf(msg+strlen(msg), "R%02d[0x%02x]:%02X  ", bbpId, bbpId, regBBP);
						}
					}
					else
					{
						/*Invalid parametes, so default printk all bbp */
						bIsPrintAllBBP = TRUE;
						break;
					}
				}
				else
				{
					/*Invalid parametes, so default printk all bbp */
					bIsPrintAllBBP = TRUE;
					break;
				}
			}
			else
			{ /* Write */
				if ((sscanf(this_char, "%d", &(bbpId)) == 1) && (sscanf(value, "%lx", &(bbpValue)) == 1))
				{
					if (bbpId <= pAdapter->chipCap.MaxNumOfBbpId)
					{
#ifdef CONFIG_ATE
						/*
							In RT2860 ATE mode, we do not load 8051 firmware.
							We must access BBP directly.
							For RT2870 ATE mode, ATE_BBP_IO_WRITE8(/READ8)_BY_REG_ID are redefined.
						*/
						if (ATE_ON(pAdapter))
						{
							ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP);
							ATE_BBP_IO_WRITE8_BY_REG_ID(pAdapter, (UCHAR)bbpId,(UCHAR) bbpValue);

							/*Read it back for showing */
							ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP);
							/* Sync with QA for comparation */
							sprintf(msg+strlen(msg), "%03d = %02X\n", bbpId, regBBP);
						}
						else
#endif /* CONFIG_ATE */
						{
							/* according to Andy, Gary, David require. */
							/* the command bbp shall read/write BBP register directly for dubug. */
							BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP);
							BBP_IO_WRITE8_BY_REG_ID(pAdapter, (UCHAR)bbpId,(UCHAR) bbpValue);
							/*Read it back for showing */
							BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP);
							sprintf(msg+strlen(msg), "R%02d[0x%02X]:%02X\n", bbpId, bbpId, regBBP);
						}
					}
					else
					{
						/* Invalid parametes, so default printk all bbp */
						bIsPrintAllBBP = TRUE;
						break;
					}
				}
				else
				{
					/* Invalid parametes, so default printk all bbp */
					bIsPrintAllBBP = TRUE;
					break;
				}
			}
		}
	}
	else
		bIsPrintAllBBP = TRUE;

	if (bIsPrintAllBBP)
	{
		memset(msg, 0x00, MAX_BBP_MSG_SIZE);
		sprintf(msg, "\n");
		for (bbpId = 0; bbpId <= pAdapter->chipCap.MaxNumOfBbpId; bbpId++)
		{
#ifdef CONFIG_ATE
			/*
				In RT2860 ATE mode, we do not load 8051 firmware.
				We must access BBP directly.
				For RT2870 ATE mode, ATE_BBP_IO_WRITE8(/READ8)_BY_REG_ID are redefined.
			*/
			if (ATE_ON(pAdapter))
			{
				ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP);
				/* Sync with QA for comparation */
				sprintf(msg+strlen(msg), "%03d = %02X\n", bbpId, regBBP);
			}
			else
#endif /* CONFIG_ATE */
			{
				/* according to Andy, Gary, David require. */
				/* the command bbp shall read/write BBP register directly for dubug. */
				BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP);
				sprintf(msg+strlen(msg), "R%02d[0x%02X]:%02X    ", bbpId, bbpId, regBBP);
				if (bbpId%5 == 4)
					sprintf(msg+strlen(msg), "\n");
			}
		}
	}

#ifdef LINUX
	wrq->u.data.length = strlen(msg);
	if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s: copy_to_user() fail\n", __FUNCTION__));
	}
#endif /* LINUX */

	if (!bAllowDump)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Dump BBP msg[%d]=\n", (UINT32)strlen(msg)));
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s\n", msg));
	}

	os_free_mem( mpool);
	if (!bAllowDump)
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("<==RTMPIoctlBBP\n\n"));
#endif /* RTMP_BBP */
}



/* CFG TODO: double define in ap_cfg / sta_cfg */
#if defined(RT_CFG80211_P2P_SUPPORT) || defined(CFG80211_MULTI_STA)
#else
#endif

#ifdef RLT_RF
VOID RTMPAPIoctlRF_rlt(RTMP_ADAPTER *pAdapter, RTMP_IOCTL_INPUT_STRUCT *wrq)
{
	UCHAR				regRF = 0;
	RTMP_STRING *mpool, *msg;
	RTMP_STRING *arg;
	INT					rfId, maxRFIdx, bank_Id;
	BOOLEAN				bIsPrintAllRF = TRUE, bFromUI;
	INT					memLen = sizeof(CHAR) * (2048+256+12);
	INT					argLen;

	maxRFIdx = pAdapter->chipCap.MaxNumOfRfId;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("==>RTMPIoctlRF (maxRFIdx = %d)\n", maxRFIdx));

	memLen = 12*(maxRFIdx+1)*MAC_RF_BANK;
	os_alloc_mem(NULL, (UCHAR **)&mpool, memLen);
	if (mpool == NULL) {
		return;
	}

	bFromUI = ((wrq->u.data.flags & RTPRIV_IOCTL_FLAG_UI) == RTPRIV_IOCTL_FLAG_UI) ? TRUE : FALSE;

	NdisZeroMemory(mpool, memLen);
	msg = (RTMP_STRING *)((ULONG)(mpool+3) & (ULONG)~0x03);
	arg = (RTMP_STRING *)((ULONG)(msg+2048+3) & (ULONG)~0x03);
	argLen = strlen((char *)(wrq->u.data.pointer));
	if (bIsPrintAllRF)
	{
		RTMPZeroMemory(msg, memLen);
		sprintf(msg, "\n");
		for (bank_Id = 0; bank_Id <= MAC_RF_BANK; bank_Id++)
		{
			if (IS_MT76x0(pAdapter))
			{
				if ((bank_Id <=4) && (bank_Id >=1))
					continue;
			}
			for (rfId = 0; rfId <= maxRFIdx; rfId++)
			{
				rlt_rf_read(pAdapter, bank_Id, rfId, &regRF);
				sprintf(msg+strlen(msg), "%d %03d = %02X\n", bank_Id, rfId, regRF);
			}
		}
		RtmpDrvAllRFPrint(NULL, msg, strlen(msg));
		/* Copy the information into the user buffer */

#ifdef LINUX
		wrq->u.data.length = strlen("Dump to RFDump.txt");
		if (copy_to_user(wrq->u.data.pointer, "Dump to RFDump.txt", wrq->u.data.length))
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s: copy_to_user() fail\n", __FUNCTION__));
		}
#endif /* LINUX */
	}

	os_free_mem( mpool);
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("<==RTMPIoctlRF\n"));
}
#endif /* RLT_RF */


#ifdef RTMP_RF_RW_SUPPORT
/*
    ==========================================================================
    Description:
        Read / Write RF register
Arguments:
    pAdapter                    Pointer to our adapter
    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
               1.) iwpriv ra0 rf		==> read all RF registers
               2.) iwpriv ra0 rf 1		==> read RF where RegID=1
               3.) iwpriv ra0 rf 1=10	==> write RF R1=0x10
    ==========================================================================
*/
VOID RTMPAPIoctlRF(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
#ifdef RT_RF
	RTMP_STRING *this_char;
	RTMP_STRING *value;
	UCHAR				regRF = 0;
	RTMP_STRING *mpool, *msg; /*msg[2048]; */
	RTMP_STRING *arg; /*arg[255]; */
	RTMP_STRING *ptr;
	INT					rfId, maxRFIdx;
	LONG				rfValue;
	BOOLEAN				bIsPrintAllRF = FALSE, bFromUI;
	INT					memLen = sizeof(CHAR) * (2048+256+12);
#endif /* RT_RF */

#ifdef RLT_RF
	if (IS_MT7601(pAdapter) || IS_MT76x0(pAdapter)) {
		RTMPAPIoctlRF_rlt(pAdapter, wrq);
		return;
	}
#endif /* RLT_RF */


#if defined(RT_CFG80211_P2P_SUPPORT) || defined(CFG80211_MULTI_STA)
#else
#endif

#ifdef RT_RF
	maxRFIdx = pAdapter->chipCap.MaxNumOfRfId;

	os_alloc_mem(NULL, (UCHAR **)&mpool, memLen);
	if (mpool == NULL) {
		return;
	}

	bFromUI = ((wrq->u.data.flags & RTPRIV_IOCTL_FLAG_UI) == RTPRIV_IOCTL_FLAG_UI) ? TRUE : FALSE;

	NdisZeroMemory(mpool, memLen);
	msg = (RTMP_STRING *)((ULONG)(mpool+3) & (ULONG)~0x03);
	arg = (RTMP_STRING *)((ULONG)(msg+2048+3) & (ULONG)~0x03);

	if ((wrq->u.data.length > 1) /* No parameters. */
		)
	{
		NdisMoveMemory(arg, wrq->u.data.pointer, (wrq->u.data.length > 255) ? 255 : wrq->u.data.length);
		ptr = arg;
		sprintf(msg, "\n");
		/*Parsing Read or Write */
		while ((this_char = strsep((char **)&ptr, ",")) != NULL)
		{
			if (!*this_char)
				continue;

			if ((value = strchr(this_char, '=')) != NULL)
				*value++ = 0;

			if (!value || !*value)
			{ /*Read */
				if (sscanf(this_char, "%d", &(rfId)) == 1)
				{
					if (rfId <= pAdapter->chipCap.MaxNumOfRfId)
					{
#ifdef CONFIG_ATE
						if (ATE_ON(pAdapter))
						{
								RT30xxReadRFRegister(pAdapter, rfId, &regRF);

							/* Sync with QA for comparation */
							sprintf(msg+strlen(msg), "%03d = %02X\n", rfId, regRF);
						}
						else
#endif /* CONFIG_ATE */
						{
							/* according to Andy, Gary, David require. */
							/* the command rf shall read rf register directly for dubug. */
							/* BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP); */
							{
								RT30xxReadRFRegister(pAdapter, rfId, &regRF);
								sprintf(msg+strlen(msg), "R%02d[0x%02x]:%02X  ", rfId, rfId, regRF);
							}
						}
					}
					else
					{
						/* Invalid parametes, so default printk all RF */
						bIsPrintAllRF = TRUE;
						break;
					}
				}
				else
				{
					/* Invalid parametes, so default printk all RF */
					bIsPrintAllRF = TRUE;
					break;
				}
			}
			else
			{ /* Write */
				if ((sscanf(this_char, "%d", &(rfId)) == 1) && (sscanf(value, "%lx", &(rfValue)) == 1))
				{
					if (rfId <= pAdapter->chipCap.MaxNumOfRfId)
					{
#ifdef CONFIG_ATE
						if (ATE_ON(pAdapter))
						{
							{
								RT30xxReadRFRegister(pAdapter, rfId, &regRF);
								RT30xxWriteRFRegister(pAdapter, (UCHAR)rfId,(UCHAR) rfValue);
							}


							/* Read it back for showing. */
								RT30xxReadRFRegister(pAdapter, rfId, &regRF);

							/* Sync with QA for comparation */
							sprintf(msg+strlen(msg), "%03d = %02X\n", rfId, regRF);
						}
						else
#endif /* CONFIG_ATE */
						{
							/* according to Andy, Gary, David require. */
							/* the command RF shall read/write RF register directly for dubug. */
							/*BBP_IO_READ8_BY_REG_ID(pAdapter, bbpId, &regBBP); */
							/*BBP_IO_WRITE8_BY_REG_ID(pAdapter, (UCHAR)bbpId,(UCHAR) bbpValue); */
							{
								RT30xxReadRFRegister(pAdapter, rfId, &regRF);
								RT30xxWriteRFRegister(pAdapter, (UCHAR)rfId,(UCHAR) rfValue);
								/* Read it back for showing */
								RT30xxReadRFRegister(pAdapter, rfId, &regRF);
								sprintf(msg+strlen(msg), "R%02d[0x%02X]:%02X\n", rfId, rfId, regRF);
							}
						}
					}
					else
					{	/* Invalid parametes, so default printk all RF */
						bIsPrintAllRF = TRUE;
						break;
					}
				}
				else
				{	/* Invalid parametes, so default printk all RF */
					bIsPrintAllRF = TRUE;
					break;
				}
			}
		}
	}
	else
		bIsPrintAllRF = TRUE;

	if (bIsPrintAllRF)
	{
		memset(msg, 0x00, 2048);
		sprintf(msg, "\n");
		for (rfId = 0; rfId <= maxRFIdx; rfId++)
		{
#ifdef CONFIG_ATE
			/*
				In RT2860 ATE mode, we do not load 8051 firmware.
				We must access RF registers directly.
				For RT2870 ATE mode, ATE_RF_IO_WRITE8(/READ8)_BY_REG_ID are redefined.
			*/
			if (ATE_ON(pAdapter))
			{
					RT30xxReadRFRegister(pAdapter, rfId, &regRF);

				/* Sync with QA for comparation */
				sprintf(msg+strlen(msg), "%03d = %02X\n", rfId, regRF);
			}
			else
#endif /* CONFIG_ATE */
			{
				/* according to Andy, Gary, David require. */
				/* the command RF shall read/write RF register directly for dubug. */
				{
					RT30xxReadRFRegister(pAdapter, rfId, &regRF);
					sprintf(msg+strlen(msg), "R%02d[0x%02X]:%02X    ", rfId, rfId*2, regRF);
					if (rfId%5 == 4)
						sprintf(msg+strlen(msg), "\n");
				}
			}
		}
		/* Copy the information into the user buffer */

#ifdef LINUX
		wrq->u.data.length = strlen(msg);
		if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s: copy_to_user() fail\n", __FUNCTION__));
		}
#endif /* LINUX */
	}
	else
	{
#ifdef LINUX
		/* Copy the information into the user buffer */
		wrq->u.data.length = strlen(msg);
		if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s: copy_to_user() fail\n", __FUNCTION__));
		}
#endif /* LINUX */
	}

	if (!bFromUI)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Dump RF msg[%d]=\n", (UINT32)strlen(msg)));
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s\n", msg));
	}

	os_free_mem( mpool);
	if (!bFromUI)
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("<==RTMPIoctlRF\n\n"));
#endif /* RT_RF */

}
#endif /* RTMP_RF_RW_SUPPORT */
#endif /*#ifdef DBG */

/*
    ==========================================================================
    Description:
        Read / Write E2PROM
Arguments:
    pAdapter                    Pointer to our adapter
    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
               1.) iwpriv ra0 e2p 0     	==> read E2PROM where Addr=0x0
               2.) iwpriv ra0 e2p 0=1234    ==> write E2PROM where Addr=0x0, value=1234
    ==========================================================================
*/
VOID RTMPAPIoctlE2PROM(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	RTMP_STRING *this_char, *value;
	INT					j = 0, k = 0;
	RTMP_STRING *mpool, *msg;/*msg[1024]; */
	RTMP_STRING *arg, *ptr;
	USHORT				eepAddr = 0;
	UCHAR				temp[16];
	RTMP_STRING temp2[16];
	USHORT				eepValue;
	BOOLEAN				bIsPrintAllE2PROM = FALSE;

	os_alloc_mem(NULL, (UCHAR **)&mpool, sizeof(CHAR)*(4096+256+12));

	if (mpool == NULL) {
		return;
	}

	msg = (RTMP_STRING *)((ULONG)(mpool+3) & (ULONG)~0x03);
	arg = (RTMP_STRING *)((ULONG)(msg+4096+3) & (ULONG)~0x03);


	memset(msg, 0x00, 4096);
	memset(arg, 0x00, 256);

	if (
#ifdef LINUX
		(wrq->u.data.length > 1) /* If no parameter, dump all e2p. */
#endif /* LINUX */
		)
	{
		NdisMoveMemory(arg, wrq->u.data.pointer, (wrq->u.data.length > 255) ? 255 : wrq->u.data.length);
		ptr = arg;
		sprintf(msg, "\n");
		/*Parsing Read or Write */
		while ((this_char = strsep((char **)&ptr, ",")) != NULL)
		{
			if (!*this_char)
				continue;

			if ((value = strchr(this_char, '=')) != NULL)
				*value++ = 0;

			if (!value || !*value)
			{ /*Read */

				/* Sanity check */
				if(strlen(this_char) > 4)
					break;

				j = strlen(this_char);
				while(j-- > 0)
				{
					if(this_char[j] > 'f' || this_char[j] < '0')
						goto done; /*return; */
				}

				/* E2PROM addr */
				k = j = strlen(this_char);
				while(j-- > 0)
				{
					this_char[4-k+j] = this_char[j];
				}

				while(k < 4)
					this_char[3-k++]='0';
				this_char[4]='\0';

				if(strlen(this_char) == 4)
				{
					AtoH(this_char, temp, 2);
					eepAddr = *temp*256 + temp[1];
					if (eepAddr < 0xFFFF)
					{
						RT28xx_EEPROM_READ16(pAdapter, eepAddr, eepValue);
						sprintf(msg+strlen(msg), "[0x%04X]:0x%04X  ", eepAddr , eepValue);
					}
                    else
					{/*Invalid parametes, so default printk all bbp */
						break;
					}
				}
			}
            else
			{ /*Write */
				NdisMoveMemory(&temp2, value, strlen(value));
				temp2[strlen(value)] = '\0';

				/* Sanity check */
				if((strlen(this_char) > 4) || strlen(temp2) > 8)
					break;

				j = strlen(this_char);
				while(j-- > 0)
				{
					if(this_char[j] > 'f' || this_char[j] < '0')
						goto done; /* return; */
				}
				j = strlen(temp2) - 1;
				while(j >= 0)
				{
					if(temp2[j] > 'f' || temp2[j] < '0')
						goto done; /* return; */
					j --;
				}

				/* MAC Addr */
				k = j = strlen(this_char);
				while(j-- > 0)
				{
					this_char[4-k+j] = this_char[j];
				}

				while(k < 4)
					this_char[3-k++]='0';
				this_char[4]='\0';

				/* MAC value */
				k = strlen(temp2);
				j = strlen(temp2) - 1;
				while(j >= 0)
				{
                    if ((4-k+j) < 0)
                        break;
                    temp2[4-k+j] = temp2[j];
                    j --;
				}

				while(k < 4)
					temp2[3-k++]='0';
				temp2[4]='\0';

				AtoH(this_char, temp, 2);
				eepAddr = *temp*256 + temp[1];

				AtoH(temp2, temp, 2);
				eepValue = *temp*256 + temp[1];

				RT28xx_EEPROM_WRITE16(pAdapter, eepAddr, eepValue);
				sprintf(msg+strlen(msg), "[0x%02X]:%02X  ", eepAddr, eepValue);
			}
		}
	}
	else
	{
		bIsPrintAllE2PROM = TRUE;
	}

	if (bIsPrintAllE2PROM)
	{
		sprintf(msg, "\n");

		/* E2PROM Registers */
		for (eepAddr = 0x00; eepAddr < 0x400; eepAddr += 2)
		{
			RT28xx_EEPROM_READ16(pAdapter, eepAddr, eepValue);
			sprintf(msg+strlen(msg), "[0x%04X]:%04X  ", eepAddr , eepValue);
			if ((eepAddr & 0x6) == 0x6)
				sprintf(msg+strlen(msg), "\n");
		}
	}

	if(strlen(msg) == 1)
		sprintf(msg+strlen(msg), "===>Error command format!");

	/* Copy the information into the user buffer */

	AP_E2PROM_IOCTL_PostCtrl(wrq, msg);

done:
	os_free_mem( mpool);
    if (wrq->u.data.flags != RT_OID_802_11_HARDWARE_REGISTER)
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("<==RTMPIoctlE2PROM\n"));
}


//#define ENHANCED_STAT_DISPLAY	// Display PER and PLR statistics


/*
    ==========================================================================
    Description:
        Read statistics counter
Arguments:
    pAdapter                    Pointer to our adapter
    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
               1.) iwpriv ra0 stat 0     	==> Read statistics counter
    ==========================================================================
*/
static VOID ShowAmpduCounter(RTMP_ADAPTER *pAd,UCHAR BandIdx, RTMP_STRING *msg)
{
	COUNTER_802_11 *WlanCounter = &pAd->WlanCounters[BandIdx];
	ULONG per;

	sprintf(msg+strlen(msg), "BandIdx: %d\n",BandIdx);
	sprintf(msg+strlen(msg), "TX AGG Range 1 (1)              = %ld\n", (LONG)(WlanCounter->TxAggRange1Count.u.LowPart));
	sprintf(msg+strlen(msg), "TX AGG Range 2 (2~5)            = %ld\n", (LONG)(WlanCounter->TxAggRange2Count.u.LowPart));
	sprintf(msg+strlen(msg), "TX AGG Range 3 (6~15)           = %ld\n", (LONG)(WlanCounter->TxAggRange3Count.u.LowPart));
	sprintf(msg+strlen(msg), "TX AGG Range 4 (>15)            = %ld\n", (LONG)(WlanCounter->TxAggRange4Count.u.LowPart));
	{
		ULONG mpduTXCount;

		mpduTXCount = WlanCounter->AmpduSuccessCount.u.LowPart;

		sprintf(msg+strlen(msg), "AMPDU Tx success                = %ld\n", mpduTXCount);

		per = mpduTXCount==0? 0: 1000*(WlanCounter->AmpduFailCount.u.LowPart)/(WlanCounter->AmpduFailCount.u.LowPart+mpduTXCount);
		sprintf(msg+strlen(msg), "AMPDU Tx fail count             = %ld, PER=%ld.%1ld%%\n",
									(ULONG)WlanCounter->AmpduFailCount.u.LowPart,
									per/10, per % 10);
	}

}

VOID RTMPIoctlStatistics(RTMP_ADAPTER *pAd, RTMP_IOCTL_INPUT_STRUCT *wrq)
{
	INT Status;
	RTMP_STRING *msg;
	ULONG txCount = 0;
	UINT32 rxCount = 0;
#ifdef ENHANCED_STAT_DISPLAY
	ULONG per, plr;
	INT i;
#endif
#ifdef RTMP_EFUSE_SUPPORT
	UINT efusefreenum=0;
#endif /* RTMP_EFUSE_SUPPORT */

#ifdef BB_SOC
	ULONG txPackets=0, rxPackets=0, txBytes=0, rxBytes=0;
	UCHAR index=0;
#endif
	UINT32 temperature=0;

	os_alloc_mem(pAd, (UCHAR **)&msg, sizeof(CHAR)*(2048));
	if (msg == NULL) {
		return;
	}


    memset(msg, 0x00, 1600);
    sprintf(msg, "\n");

#ifdef RACTRL_FW_OFFLOAD_SUPPORT
    if (pAd->chipCap.fgRateAdaptFWOffload == TRUE )
    {
        EXT_EVENT_TX_STATISTIC_RESULT_T rTxStatResult;
        HTTRANSMIT_SETTING LastTxRate;

        MtCmdGetTxStatistic(pAd, GET_TX_STAT_TOTAL_TX_CNT | GET_TX_STAT_LAST_TX_RATE, 0, &rTxStatResult);
        pAd->WlanCounters[0].TransmittedFragmentCount.u.LowPart += (rTxStatResult.u4TotalTxCount - rTxStatResult.u4TotalTxFailCount);
        pAd->WlanCounters[0].FailedCount.u.LowPart += rTxStatResult.u4TotalTxFailCount;

        LastTxRate.field.MODE = rTxStatResult.rLastTxRate.MODE;
        LastTxRate.field.BW = rTxStatResult.rLastTxRate.BW;
        LastTxRate.field.ldpc = rTxStatResult.rLastTxRate.ldpc? 1:0;
        LastTxRate.field.ShortGI = rTxStatResult.rLastTxRate.ShortGI? 1:0;
        LastTxRate.field.STBC = rTxStatResult.rLastTxRate.STBC;
        if (LastTxRate.field.MODE == MODE_VHT)
        {
            LastTxRate.field.MCS = (((rTxStatResult.rLastTxRate.VhtNss - 1) & 0x3) << 4) + rTxStatResult.rLastTxRate.MCS;
        }
        else
        {
            LastTxRate.field.MCS = rTxStatResult.rLastTxRate.MCS;
        }

        pAd->LastTxRate = (USHORT)(LastTxRate.word);
    }
#endif /* RACTRL_FW_OFFLOAD_SUPPORT */

#ifdef CONFIG_ATE
	if(ATE_ON(pAd))
	{
        txCount = pAd->ATECtrl.TxDoneCount;
		rxCount = pAd->ATECtrl.rx_stat.RxTotalCnt[0];
	}
	else
#endif /* CONFIG_ATE */
	{
		txCount = pAd->WlanCounters[0].TransmittedFragmentCount.u.LowPart;
		rxCount = pAd->WlanCounters[0].ReceivedFragmentCount.QuadPart;
	}

	RTMP_GET_TEMPERATURE(pAd,&temperature);
	sprintf(msg+strlen(msg),"CurrentTemperature              = %d\n",temperature);

    sprintf(msg+strlen(msg), "Tx success                      = %ld\n", txCount);
#ifdef ENHANCED_STAT_DISPLAY
	if (pAd->chipCap.hif_type == HIF_MT) {
	per = txCount==0? 0: 1000*(pAd->WlanCounters[0].FailedCount.u.LowPart)/(pAd->WlanCounters[0].FailedCount.u.LowPart+txCount);
    sprintf(msg+strlen(msg), "Tx fail count                   = %ld, PER=%ld.%1ld%%\n",
									(ULONG)pAd->WlanCounters[0].FailedCount.u.LowPart,
									per/10, per % 10);
	} else {
	per = txCount==0? 0: 1000*(pAd->WlanCounters[0].RetryCount.u.LowPart+pAd->WlanCounters[0].FailedCount.u.LowPart)/(pAd->WlanCounters[0].RetryCount.u.LowPart+pAd->WlanCounters[0].FailedCount.u.LowPart+txCount);
    sprintf(msg+strlen(msg), "Tx retry count                  = %ld, PER=%ld.%1ld%%\n",
									(ULONG)pAd->WlanCounters[0].RetryCount.u.LowPart,
									per/10, per % 10);
	plr = txCount==0? 0: 10000*pAd->WlanCounters[0].FailedCount.u.LowPart/(pAd->WlanCounters[0].FailedCount.u.LowPart+txCount);
    sprintf(msg+strlen(msg), "Tx fail to Rcv ACK after retry  = %ld, PLR=%ld.%02ld%%\n",
									(ULONG)pAd->WlanCounters[0].FailedCount.u.LowPart, plr/100, plr%100);
	}
    sprintf(msg+strlen(msg), "Rx success                      = %ld\n", (ULONG)rxCount);
#ifdef DBDC_MODE
    sprintf(msg+strlen(msg), "Rx ICV Error                    = %ld\n", (ULONG)pAd->WlanCounters[0].RxICVErrorCount.u.LowPart + (ULONG)pAd->WlanCounters[1].RxICVErrorCount.u.LowPart);	
#else
    sprintf(msg+strlen(msg), "Rx ICV Error                    = %ld\n", (ULONG)pAd->WlanCounters[0].RxICVErrorCount.u.LowPart);
#endif /*DBDC_MODE*/
#ifdef CONFIG_QA
	if(ATE_ON(pAd))
	per = rxCount==0? 0: 1000*(pAd->WlanCounters[0].FCSErrorCount.u.LowPart)/(pAd->WlanCounters[0].FCSErrorCount.u.LowPart+rxCount);
	else
#endif /* CONFIG_QA */
	per = pAd->WlanCounters[0].ReceivedFragmentCount.u.LowPart==0? 0: 1000*(pAd->WlanCounters[0].FCSErrorCount.u.LowPart)/(pAd->WlanCounters[0].FCSErrorCount.u.LowPart+pAd->WlanCounters[0].ReceivedFragmentCount.u.LowPart);
	sprintf(msg+strlen(msg), "Rx with CRC                     = %ld, PER=%ld.%1ld%%\n",
									(ULONG)pAd->WlanCounters[0].FCSErrorCount.u.LowPart, per/10, per % 10);
	sprintf(msg+strlen(msg), "Rx with PhyErr                  = %ld\n",
									(ULONG)pAd->RalinkCounters.PhyErrCnt);
	sprintf(msg+strlen(msg), "Rx with PlcpErr                 = %ld\n",
									(ULONG)pAd->RalinkCounters.PlcpErrCnt);
	sprintf(msg+strlen(msg), "Rx drop due to out of resource  = %ld\n", (ULONG)pAd->Counters8023.RxNoBuffer);
	sprintf(msg+strlen(msg), "Rx duplicate frame              = %ld\n", (ULONG)pAd->WlanCounters[0].FrameDuplicateCount.u.LowPart);

	sprintf(msg+strlen(msg), "False CCA                       = %ld\n", (ULONG)pAd->RalinkCounters.FalseCCACnt);
#else
    sprintf(msg+strlen(msg), "Tx retry count                  = %ld\n", (ULONG)pAd->WlanCounters[0].RetryCount.u.LowPart);
    sprintf(msg+strlen(msg), "Tx fail to Rcv ACK after retry  = %ld\n", (ULONG)pAd->WlanCounters[0].FailedCount.u.LowPart);
    sprintf(msg+strlen(msg), "RTS Success Rcv CTS             = %ld\n", (ULONG)pAd->WlanCounters[0].RTSSuccessCount.u.LowPart);
    sprintf(msg+strlen(msg), "RTS Fail Rcv CTS                = %ld\n", (ULONG)pAd->WlanCounters[0].RTSFailureCount.u.LowPart);

    sprintf(msg+strlen(msg), "Rx success                      = %ld\n", (ULONG)pAd->WlanCounters[0].ReceivedFragmentCount.QuadPart);
    sprintf(msg+strlen(msg), "Rx with CRC                     = %ld\n", (ULONG)pAd->WlanCounters[0].FCSErrorCount.u.LowPart);
    sprintf(msg+strlen(msg), "Rx drop due to out of resource  = %ld\n", (ULONG)pAd->Counters8023.RxNoBuffer);
    sprintf(msg+strlen(msg), "Rx duplicate frame              = %ld\n", (ULONG)pAd->WlanCounters[0].FrameDuplicateCount.u.LowPart);

    sprintf(msg+strlen(msg), "False CCA (one second)          = %ld\n", (ULONG)pAd->RalinkCounters.OneSecFalseCCACnt);
#endif /* ENHANCED_STAT_DISPLAY */

#ifdef CONFIG_QA
	if(ATE_ON(pAd))
	{
		ATE_RX_STATISTIC rx_stat = pAd->ATECtrl.rx_stat;
		if (pAd->ATECtrl.RxAntennaSel == 0)
		{
    		sprintf(msg+strlen(msg), "RSSI-A                          = %ld\n", (LONG)(rx_stat.LastRssi[0] - pAd->BbpRssiToDbmDelta));
			sprintf(msg+strlen(msg), "RSSI-B (if available)           = %ld\n", (LONG)(rx_stat.LastRssi[1] - pAd->BbpRssiToDbmDelta));
			sprintf(msg+strlen(msg), "RSSI-C (if available)           = %ld\n\n", (LONG)(rx_stat.LastRssi[2] - pAd->BbpRssiToDbmDelta));
		}
		else
		{
    		sprintf(msg+strlen(msg), "RSSI                            = %ld\n", (LONG)(rx_stat.LastRssi[0] - pAd->BbpRssiToDbmDelta));
		}

	}
	else
#endif /* CONFIG_QA */
	{
#ifdef ENHANCED_STAT_DISPLAY
	sprintf(msg+strlen(msg), "RSSI                            = %ld %ld %ld %ld\n",
    			(LONG)(pAd->ApCfg.RssiSample.LastRssi[0] - pAd->BbpRssiToDbmDelta),
    			(LONG)(pAd->ApCfg.RssiSample.LastRssi[1] - pAd->BbpRssiToDbmDelta),
    			(LONG)(pAd->ApCfg.RssiSample.LastRssi[2] - pAd->BbpRssiToDbmDelta),
    			(LONG)(pAd->ApCfg.RssiSample.LastRssi[3] - pAd->BbpRssiToDbmDelta));

    	/* Display Last Rx Rate and BF SNR of first Associated entry in MAC table */
    	if (pAd->MacTab.Size > 0)
    	{
    		static char *phyMode[5] = {"CCK", "OFDM", "MM", "GF", "VHT"};

    		for (i=1; VALID_UCAST_ENTRY_WCID(pAd, i); i++)
			{
    			PMAC_TABLE_ENTRY pEntry = &(pAd->MacTab.Content[i]);
    			if ((IS_ENTRY_CLIENT(pEntry) && pEntry->Sst==SST_ASSOC) || IS_ENTRY_WDS(pEntry) || IS_ENTRY_APCLI(pEntry))
				{
					//sprintf(msg+strlen(msg), "sta mac: %02x:%02x:%02x:%02x:%02x:%02x\n", pEntry->wdev->if_addr[0], pEntry->wdev->if_addr[1],  pEntry->wdev->if_addr[2],  pEntry->wdev->if_addr[3],  pEntry->wdev->if_addr[4],  pEntry->wdev->if_addr[5]);
					UINT32 lastRxRate = pEntry->LastRxRate;
					UINT32 lastTxRate = pEntry->LastTxRate;

#ifdef RACTRL_FW_OFFLOAD_SUPPORT
                if (pAd->chipCap.fgRateAdaptFWOffload == TRUE )
                {
                        if (pEntry->bAutoTxRateSwitch == TRUE)
                        {
                            EXT_EVENT_TX_STATISTIC_RESULT_T rTxStatResult;
                            HTTRANSMIT_SETTING LastTxRate;

                            MtCmdGetTxStatistic(pAd, GET_TX_STAT_ENTRY_TX_RATE, pEntry->wcid, &rTxStatResult);

                            LastTxRate.field.MODE = rTxStatResult.rEntryTxRate.MODE;
                            LastTxRate.field.BW = rTxStatResult.rEntryTxRate.BW;
                            LastTxRate.field.ldpc = rTxStatResult.rEntryTxRate.ldpc ? 1:0;
                            LastTxRate.field.ShortGI = rTxStatResult.rEntryTxRate.ShortGI ? 1:0;
                            LastTxRate.field.STBC = rTxStatResult.rEntryTxRate.STBC;
                            if (LastTxRate.field.MODE == MODE_VHT)
                            {
                                LastTxRate.field.MCS = (((rTxStatResult.rEntryTxRate.VhtNss - 1) & 0x3) << 4) + rTxStatResult.rEntryTxRate.MCS;
                            }
                            else
                            {
                                LastTxRate.field.MCS = rTxStatResult.rEntryTxRate.MCS;
                            }

                            lastTxRate = (UINT32)(LastTxRate.word);
                        }
                }
#endif /* RACTRL_FW_OFFLOAD_SUPPORT */

#ifdef MT_MAC
					if (pAd->chipCap.hif_type == HIF_MT)  {
						StatRateToString(pAd, msg, 0, lastTxRate);
						StatRateToString(pAd, msg, 1, lastRxRate);
					}
					else
#endif /* MT_MAC */
					{
						sprintf(msg+strlen(msg), "Last TX Rate                    = MCS%d, %2dM, %cGI, %s%s\n",
							lastTxRate & 0x7F,  ((lastTxRate>>7) & 0x1)? 40: 20,
							((lastTxRate>>8) & 0x1)? 'S': 'L',
							phyMode[(lastTxRate>>14) & 0x3],
							((lastTxRate>>9) & 0x3)? ", STBC": " ");
						sprintf(msg+strlen(msg), "Last RX Rate                    = MCS%d, %2dM, %cGI, %s%s\n",
							lastRxRate & 0x7F,  ((lastRxRate>>7) & 0x1)? 40: 20,
							((lastRxRate>>8) & 0x1)? 'S': 'L',
							phyMode[(lastRxRate>>14) & 0x3],
							((lastRxRate>>9) & 0x3)? ", STBC": " ");
					}

					break;
				}
			}
    	}
#ifdef MT_MAC
	if (pAd->chipCap.hif_type == HIF_MT)
	{
		for(i=0;i<DBDC_BAND_NUM;i++)
		{
			ShowAmpduCounter(pAd,i,msg);
		}

        if (pAd->CommonCfg.bTXRX_RXV_ON) {
            sprintf(msg+strlen(msg), "/* Condition Number should enable mode4 of 0x6020_426c */\n");
            sprintf(msg+strlen(msg),
                    "--10 packets Condition Number   = [%d|%d|%d|%d|%d|%d|%d|%d|%d|%d]\n",
                    (UINT8)(pAd->rxv2_cyc3[0] & 0xff),
                    (UINT8)(pAd->rxv2_cyc3[1] & 0xff),
                    (UINT8)(pAd->rxv2_cyc3[2] & 0xff),
                    (UINT8)(pAd->rxv2_cyc3[3] & 0xff),
                    (UINT8)(pAd->rxv2_cyc3[4] & 0xff),
                    (UINT8)(pAd->rxv2_cyc3[5] & 0xff),
                    (UINT8)(pAd->rxv2_cyc3[6] & 0xff),
                    (UINT8)(pAd->rxv2_cyc3[7] & 0xff),
                    (UINT8)(pAd->rxv2_cyc3[8] & 0xff),
                    (UINT8)(pAd->rxv2_cyc3[9] & 0xff)
                   );
        }
	}
#endif /* MT_MAC */

 #else
    	sprintf(msg+strlen(msg), "RSSI-A                          = %ld\n", (LONG)(pAd->ApCfg.RssiSample.LastRssi[0] - pAd->BbpRssiToDbmDelta));
		sprintf(msg+strlen(msg), "RSSI-B (if available)           = %ld\n", (LONG)(pAd->ApCfg.RssiSample.LastRssi[1] - pAd->BbpRssiToDbmDelta));
		sprintf(msg+strlen(msg), "RSSI-C (if available)           = %ld\n\n", (LONG)(pAd->ApCfg.RssiSample.LastRssi[2] - pAd->BbpRssiToDbmDelta));
#endif /* ENHANCED_STAT_DISPLAY */
	}



/*
 * Let "iwpriv ra0 stat" can print out Tx/Rx Packet and Byte count.
 * Therefore, we can parse them out in cfg_manager. --Trey */
#ifdef BB_SOC

	for (index = 0; index < pAd->ApCfg.BssidNum; index++){
	//		sprintf(msg+strlen(msg), "Enrollee PinCode(ra%d)		  = %08u\n", idx, pAd->ApCfg.MBSSID[idx].WscControl.WscEnrolleePinCode);
	//		sprintf(msg+strlen(msg), "WPS Query Status(ra%d)		= %d\n", idx, pAd->ApCfg.MBSSID[idx].WscControl.WscStatus);
			sprintf(msg+strlen(msg), "WPS Wsc2MinsTimerRunning(rai%d)		= %d\n", index, pAd->ApCfg.MBSSID[index].WscControl.Wsc2MinsTimerRunning);
	}
	
	for (index = 0; index < pAd->ApCfg.BssidNum; index++){
		rxPackets += (ULONG)pAd->ApCfg.MBSSID[index].RxCount;
		txPackets += (ULONG)pAd->ApCfg.MBSSID[index].TxCount;
		rxBytes += (ULONG)pAd->ApCfg.MBSSID[index].ReceivedByteCount;
		txBytes += (ULONG)pAd->ApCfg.MBSSID[index].TransmittedByteCount;
	}
	sprintf(msg+strlen(msg), "Packets Received       = %lu\n", rxPackets);
	sprintf(msg+strlen(msg), "Packets Sent           = %lu\n", txPackets);
	sprintf(msg+strlen(msg), "Bytes Received         = %lu\n", rxBytes);
	sprintf(msg+strlen(msg), "Bytes Sent             = %lu\n", txBytes);
	sprintf(msg+strlen(msg), "\n");
#endif

#ifdef RTMP_EFUSE_SUPPORT
	if (pAd->bUseEfuse == TRUE)
	{
		eFuseGetFreeBlockCount(pAd, &efusefreenum);
		sprintf(msg+strlen(msg), "efuseFreeNumber                 = %d\n", efusefreenum);
	}
#endif /* RTMP_EFUSE_SUPPORT */

#ifdef CONFIG_HOTSPOT
    {
        PHOTSPOT_CTRL pHSCtrl;
        POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
        UCHAR apidx = pObj->ioctl_if;
        PGAS_CTRL pGASCtrl = &pAd->ApCfg.MBSSID[apidx].GASCtrl;
#ifdef CONFIG_DOT11V_WNM
        PWNM_CTRL pWNMCtrl = &pAd->ApCfg.MBSSID[apidx].WNMCtrl;
#endif
        pHSCtrl = &pAd->ApCfg.MBSSID[apidx].HotSpotCtrl;

        sprintf(msg+strlen(msg), "\n");
        sprintf(msg+strlen(msg), "hotspot enable                    = %d\n", pHSCtrl->HotSpotEnable);
        sprintf(msg+strlen(msg), "daemon ready                  = %d\n", pHSCtrl->HSDaemonReady);
        sprintf(msg+strlen(msg), "hotspot DGAFDisable               = %d\n", pHSCtrl->DGAFDisable);
        sprintf(msg+strlen(msg), "hotspot L2Filter              = %d\n", pHSCtrl->L2Filter);
        sprintf(msg+strlen(msg), "hotspot ICMPv4Deny                = %d\n", pHSCtrl->ICMPv4Deny);
        sprintf(msg+strlen(msg), "hotspot QosMapEnable              = %d\n", pHSCtrl->QosMapEnable);
#ifdef CONFIG_DOT11V_WNM
        sprintf(msg+strlen(msg), "proxy arp enable              = %d\n", pWNMCtrl->ProxyARPEnable);
        sprintf(msg+strlen(msg), "WNMNotify enable              = %d\n", pWNMCtrl->WNMNotifyEnable);
		sprintf(msg+strlen(msg), "WNM BSS Transition Management enable = %d\n", pWNMCtrl->WNMBTMEnable);
#endif
        sprintf(msg+strlen(msg), "hotspot OSEN enable                   = %d\n", pHSCtrl->bASANEnable);
        sprintf(msg+strlen(msg), "GAS come back delay                       = %d\n", pGASCtrl->cb_delay);
        sprintf(msg+strlen(msg), "\n");
    }
#endif /* CONFIG_HOTSPOT */

#ifdef CONFIG_DOT11U_INTERWORKING
	{		
		POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
        UCHAR apidx = pObj->ioctl_if;
		PGAS_CTRL pGASCtrl = &pAd->ApCfg.MBSSID[apidx].GASCtrl;
		sprintf(msg+strlen(msg), "GAS enable                       = %d\n", pGASCtrl->b11U_enable);
	}
#endif /* CONFIG_DOT11U_INTERWORKING */

    /* Copy the information into the user buffer */
    wrq->u.data.length = strlen(msg);
    Status = copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length);

	os_free_mem( msg);

#if defined(TXBF_SUPPORT) && defined(ENHANCED_STAT_DISPLAY)
#ifdef DBG_CTRL_SUPPORT
	/* Debug code to display BF statistics */
	if (pAd->CommonCfg.DebugFlags & DBF_SHOW_BF_STATS)
	{
		for (i=0; VALID_UCAST_ENTRY_WCID(pAd, i); i++) {
			PMAC_TABLE_ENTRY pEntry = &(pAd->MacTab.Content[i]);
			COUNTER_TXBF *pCnt;
			ULONG totalNBF, totalEBF, totalIBF, totalTx, totalRetry, totalSuccess;

			if (!IS_ENTRY_CLIENT(pEntry) || pEntry->Sst!=SST_ASSOC)
				continue;

			pCnt = &pEntry->TxBFCounters;

			totalNBF = pCnt->TxSuccessCount + pCnt->TxFailCount;
			totalEBF = pCnt->ETxSuccessCount + pCnt->ETxFailCount;
			totalIBF = pCnt->ITxSuccessCount + pCnt->ITxFailCount;

			totalTx = totalNBF + totalEBF + totalIBF;
			totalRetry = pCnt->TxRetryCount + pCnt->ETxRetryCount + pCnt->ITxRetryCount;
			totalSuccess = pCnt->TxSuccessCount + pCnt->ETxSuccessCount + pCnt->ITxSuccessCount;

			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("MacTable[%d]     Success    Retry/PER    Fail/PLR\n", i) );
			if (totalTx==0) {
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("   Total = 0\n") );
				continue;
			}

			if (totalNBF!=0) {
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("   NonBF (%3lu%%): %7lu  %7lu (%2lu%%) %5lu (%1lu%%)\n",
					100*totalNBF/totalTx, pCnt->TxSuccessCount,
					pCnt->TxRetryCount, 100*pCnt->TxRetryCount/(pCnt->TxSuccessCount+pCnt->TxRetryCount),
					pCnt->TxFailCount, 100*pCnt->TxFailCount/totalNBF) );
			}

			if (totalEBF!=0) {
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("   ETxBF (%3lu%%): %7lu  %7lu (%2lu%%) %5lu (%1lu%%)\n",
					 100*totalEBF/totalTx, pCnt->ETxSuccessCount,
					pCnt->ETxRetryCount, 100*pCnt->ETxRetryCount/(pCnt->ETxSuccessCount+pCnt->ETxRetryCount),
					pCnt->ETxFailCount, 100*pCnt->ETxFailCount/totalEBF) );
			}

			if (totalIBF!=0) {
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("   ITxBF (%3lu%%): %7lu  %7lu (%2lu%%) %5lu (%1lu%%)\n",
					100*totalIBF/totalTx, pCnt->ITxSuccessCount,
					pCnt->ITxRetryCount, 100*pCnt->ITxRetryCount/(pCnt->ITxSuccessCount+pCnt->ITxRetryCount),
					pCnt->ITxFailCount, 100*pCnt->ITxFailCount/totalIBF) );
			}

			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("   Total         %7lu  %7lu (%2lu%%) %5lu (%1lu%%)\n",
				totalSuccess, totalRetry, 100*totalRetry/(totalSuccess + totalRetry),
				pCnt->TxFailCount+pCnt->ETxFailCount+pCnt->ITxFailCount,
				100*(pCnt->TxFailCount+pCnt->ETxFailCount+pCnt->ITxFailCount)/totalTx) );
		}
	}
#endif /* DBG_CTRL_SUPPORT */
#endif /* defined(TXBF_SUPPORT) && defined(ENHANCED_STAT_DISPLAY) */

    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("<==RTMPIoctlStatistics\n"));
}

INT RTMPIoctlRXStatistics(
	IN RTMP_ADAPTER *pAd,
	IN RTMP_IOCTL_INPUT_STRUCT *wrq)
{
        RTMP_STRING *this_char, *value = NULL;
        INT Status = NDIS_STATUS_SUCCESS;


        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s----------------->\n",__FUNCTION__));    

        this_char = wrq->u.data.pointer;

        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s(): Before check, this_char = %s\n" 
        , __FUNCTION__, this_char));
        
        value = strchr(this_char, '=');
        if (value) 
        {  
               if (strlen(value) > 1) 
               {
                    *value=0;
                    value++;
               } 
               else
               {
                    value = NULL;
               }
        }

        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s(): After check, this_char = %s, value = %s\n" 
        , __FUNCTION__, this_char, (value == NULL ? "" : value)));

        for (PRTMP_PRIVATE_RX_PROC = RTMP_PRIVATE_RX_SUPPORT_PROC; PRTMP_PRIVATE_RX_PROC->name; PRTMP_PRIVATE_RX_PROC++)
        {
               if (!strcmp(this_char, PRTMP_PRIVATE_RX_PROC->name))
               {
	                if(!PRTMP_PRIVATE_RX_PROC->rx_proc(pAd, value, wrq))
	                {   /*FALSE:Set private failed then return Invalid argument */
	                        Status = -EINVAL;
	                }
	                break;  /*Exit for loop. */
               }
        }

        if(PRTMP_PRIVATE_RX_PROC->name == NULL)
        { 
               /*Not found argument */
               Status = -EINVAL;   
               MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, 
               ("IOCTL::(iwpriv) Command not Support [%s = %s]\n", this_char, value));
        }
       
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s<-----------------\n",__FUNCTION__));
               
        return Status;
}

#ifdef DOT11_N_SUPPORT
/*
    ==========================================================================
    Description:
        Get Block ACK Table
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
        		1.) iwpriv ra0 get_ba_table
        		3.) UI needs to prepare at least 4096bytes to get the results
    ==========================================================================
*/
VOID RTMPIoctlQueryBaTable(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	/*char *msg; */
	UCHAR	TotalEntry, i, j, index;
	QUERYBA_TABLE		*BAT;

	BAT = vmalloc(sizeof(QUERYBA_TABLE));

	if (BAT == NULL)
		return;

	RTMPZeroMemory(BAT, sizeof(QUERYBA_TABLE));

	TotalEntry = pAd->MacTab.Size;
	index = 0;
	for (i=0; ((VALID_UCAST_ENTRY_WCID(pAd, i)) && (TotalEntry > 0)); i++)
	{
		PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];

		if (IS_ENTRY_CLIENT(pEntry) && (pEntry->Sst == SST_ASSOC) && (pEntry->TXBAbitmap) && (index < 32))
		{
			NdisMoveMemory(BAT->BAOriEntry[index].MACAddr, pEntry->Addr, 6);
			for (j=0;j<8;j++)
			{
				if (pEntry->BAOriWcidArray[j] != 0)
					BAT->BAOriEntry[index].BufSize[j] = pAd->BATable.BAOriEntry[pEntry->BAOriWcidArray[j]].BAWinSize;
				else
					BAT->BAOriEntry[index].BufSize[j] = 0;
			}

			TotalEntry--;
			index++;
			BAT->OriNum++;
		}
	}

	TotalEntry = pAd->MacTab.Size;
	index = 0;
	for (i=0; ((VALID_UCAST_ENTRY_WCID(pAd, i)) && (TotalEntry > 0)); i++)
	{
		PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];

		if (IS_ENTRY_CLIENT(pEntry) && (pEntry->Sst == SST_ASSOC) && (pEntry->RXBAbitmap) && (index < 32))
		{
			NdisMoveMemory(BAT->BARecEntry[index].MACAddr, pEntry->Addr, 6);
			BAT->BARecEntry[index].BaBitmap = (UCHAR)pEntry->RXBAbitmap;
			for (j = 0; j < 8; j++)
			{
				if (pEntry->BARecWcidArray[j] != 0)
					BAT->BARecEntry[index].BufSize[j] = pAd->BATable.BARecEntry[pEntry->BARecWcidArray[j]].BAWinSize;
				else
					BAT->BARecEntry[index].BufSize[j] = 0;
			}

			TotalEntry--;
			index++;
			BAT->RecNum++;
		}
	}

	wrq->u.data.length = sizeof(QUERYBA_TABLE);

	if (copy_to_user(wrq->u.data.pointer, BAT, wrq->u.data.length))
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s: copy_to_user() fail\n", __FUNCTION__));
	}

	vfree(BAT);

}
#endif /* DOT11_N_SUPPORT */


#ifdef APCLI_SUPPORT
INT Set_ApCli_Enable_Proc(
	IN  PRTMP_ADAPTER pAd,
	IN  RTMP_STRING *arg)
{
	UINT Enable;
	POS_COOKIE pObj;
	UCHAR ifIndex;
    struct wifi_dev *wdev = NULL;

	pObj = (POS_COOKIE) pAd->OS_Cookie;
	if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	ifIndex = pObj->ioctl_if;
	Enable = simple_strtol(arg, 0, 16);

	pAd->ApCfg.ApCliTab[ifIndex].Enable = (Enable > 0) ? TRUE : FALSE;
	MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE,
                ("I/F(apcli%d) Set_ApCli_Enable_Proc::(enable = %d)\n",
                                ifIndex,
                                pAd->ApCfg.ApCliTab[ifIndex].Enable));

    wdev = &pAd->ApCfg.ApCliTab[ifIndex].wdev;
#ifdef APCLI_CONNECTION_TRIAL
	if (pAd->ApCfg.ApCliTab[ifIndex].TrialCh == 0)
#endif /* APCLI_CONNECTION_TRIAL */
	ApCliIfDown(pAd);

	return TRUE;
}


INT Set_ApCli_Ssid_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	POS_COOKIE pObj;
	UCHAR ifIndex;
	BOOLEAN apcliEn;
	INT success = FALSE;
	APCLI_STRUCT *apcli_entry;
	struct wifi_dev *wdev;

	pObj = (POS_COOKIE) pAd->OS_Cookie;
	if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	ifIndex = pObj->ioctl_if;

	if(strlen(arg) <= MAX_LEN_OF_SSID)
	{
		apcli_entry = &pAd->ApCfg.ApCliTab[ifIndex];
		wdev = &apcli_entry->wdev;

		/* bring apcli interface down first */
		apcliEn = apcli_entry->Enable;
#ifdef APCLI_CONNECTION_TRIAL
		if (pAd->ApCfg.ApCliTab[ifIndex].TrialCh == 0)
		{
#endif /* APCLI_CONNECTION_TRIAL */
		if(apcliEn == TRUE )
		{
			apcli_entry->Enable = FALSE;
			ApCliIfDown(pAd);
		}
#ifdef APCLI_CONNECTION_TRIAL
		}
#endif /* APCLI_CONNECTION_TRIAL */
		apcli_entry->bPeerExist = FALSE;
		NdisZeroMemory(apcli_entry->CfgSsid, MAX_LEN_OF_SSID);
		NdisMoveMemory(apcli_entry->CfgSsid, arg, strlen(arg));
		apcli_entry->CfgSsidLen = (UCHAR)strlen(arg);
		success = TRUE;

		MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(apcli%d) Set_ApCli_Ssid_Proc::(Len=%d,Ssid=%s)\n",
				ifIndex, apcli_entry->CfgSsidLen, apcli_entry->CfgSsid));

		apcli_entry->Enable = apcliEn;
	}
	else
		success = FALSE;

	return success;
}


INT Set_ApCli_Bssid_Proc(
	IN  PRTMP_ADAPTER pAd,
	IN  RTMP_STRING *arg)
{
	INT i;
	RTMP_STRING *value;
	UCHAR ifIndex;
	BOOLEAN apcliEn;
	POS_COOKIE pObj;

	pObj = (POS_COOKIE) pAd->OS_Cookie;
	if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	ifIndex = pObj->ioctl_if;

	apcliEn = pAd->ApCfg.ApCliTab[ifIndex].Enable;

	/* bring apcli interface down first */
	if(apcliEn == TRUE )
	{
		pAd->ApCfg.ApCliTab[ifIndex].Enable = FALSE;
#ifdef APCLI_CONNECTION_TRIAL
		if (pAd->ApCfg.ApCliTab[ifIndex].TrialCh == 0)
#endif /* APCLI_CONNECTION_TRIAL */
		ApCliIfDown(pAd);
	}

	NdisZeroMemory(pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid, MAC_ADDR_LEN);

	if(strlen(arg) == 17)  /* Mac address acceptable format 01:02:03:04:05:06 length 17 */
	{
		for (i=0, value = rstrtok(arg,":"); value; value = rstrtok(NULL,":"), i++)
		{
			if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) )
				return FALSE;  /* Invalid */

			AtoH(value, &pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid[i], 1);
		}

		if(i != 6)
			return FALSE;  /* Invalid */
	}

	MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_ApCli_Bssid_Proc (%2X:%2X:%2X:%2X:%2X:%2X)\n",
		pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid[0],
		pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid[1],
		pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid[2],
		pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid[3],
		pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid[4],
		pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid[5]));

	pAd->ApCfg.ApCliTab[ifIndex].Enable = apcliEn;

	return TRUE;
}


INT Set_ApCli_TxMode_Proc(
	IN  PRTMP_ADAPTER pAd,
	IN  RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie;
	struct wifi_dev *wdev;


	if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	wdev = &pAd->ApCfg.ApCliTab[pObj->ioctl_if].wdev;

	wdev->DesiredTransmitSetting.field.FixedTxMode = RT_CfgSetFixedTxPhyMode(arg);
	MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(apcli%d) Set_ApCli_TxMode_Proc = %d\n",
				pObj->ioctl_if, wdev->DesiredTransmitSetting.field.FixedTxMode));

	return TRUE;
}

INT Set_ApCli_Channel_Proc(
	IN  PRTMP_ADAPTER pAd,
	IN  RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	struct wifi_dev *wdev;
	UCHAR RFChannel;
	UCHAR Channel =  simple_strtol(arg, 0, 10);
	UCHAR op_ht_bw;

	if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	wdev = &pAd->ApCfg.ApCliTab[pObj->ioctl_if].wdev;
	op_ht_bw = wlan_operate_get_ht_bw(wdev);
	wdev->channel = Channel;

#ifdef DOT11_N_SUPPORT
	N_ChannelCheck(pAd,wdev->PhyMode,Channel);
	if (WMODE_CAP_N(wdev->PhyMode) &&
			op_ht_bw == BW_40)
		RFChannel = N_SetCenCh(pAd, Channel,op_ht_bw);
	else
#endif /* DOT11_N_SUPPORT */
		RFChannel = Channel;

	HcUpdateChannel( pAd,Channel);

	AsicSwitchChannel(pAd,RFChannel, FALSE);

	if(wdev->channel > 14)
		apcli_phy_rrm_init_byRf(pAd,RFIC_5GHZ);
	else
		apcli_phy_rrm_init_byRf(pAd,RFIC_24GHZ);

	MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(apcli%d) Set_ApCli_Channel_Proc = %d\n",
					pObj->ioctl_if, Channel));

	return TRUE;
}



INT Set_ApCli_WirelessMode_Proc(
	IN	PRTMP_ADAPTER pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	struct wifi_dev *wdev;
	struct dev_rate_info *rate;
	UCHAR WirelessMode =  simple_strtol(arg, 0, 10);
	UCHAR PhyMode;
	APCLI_STRUCT *apcli_entry;

	if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	apcli_entry = &pAd->ApCfg.ApCliTab[pObj->ioctl_if];

	wdev = &apcli_entry->wdev;
	rate = &wdev->rate;

	PhyMode = cfgmode_2_wmode(WirelessMode);

	if(!APCLI_IF_UP_CHECK(pAd,pObj->ioctl_if))
		return FALSE;

	/* apcli always follow per band's channel */
	if (WMODE_CAP_5G(PhyMode))
		wdev->channel = HcGetChannelByRf(pAd,RFIC_5GHZ);
	else
		wdev->channel = HcGetChannelByRf(pAd,RFIC_24GHZ);

	WifiSysApCliLinkDown(pAd,apcli_entry,0xFF);
	os_msec_delay(100);
	WifiSysClose(pAd,wdev);
	/* fixed race condition between hw control task */
	os_msec_delay(100);
	wdev->PhyMode = PhyMode;
	HcAcquireRadioForWdev(pAd,wdev);
	RTMPSetPhyMode( pAd,wdev,PhyMode);
	/*update rate info for wdev*/
	RTMPUpdateRateInfo(wdev->PhyMode,rate);
	os_msec_delay(100);

	/* Security initial  */
	if (wdev->SecConfig.AKMMap == 0x0)
		SET_AKM_OPEN(wdev->SecConfig.AKMMap);

	if (wdev->SecConfig.PairwiseCipher == 0x0)
	{
		SET_CIPHER_NONE(wdev->SecConfig.PairwiseCipher);
		SET_CIPHER_NONE(wdev->SecConfig.GroupCipher);
	}

	WifiSysOpen(pAd,wdev);
	os_msec_delay(100);

	MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(apcli%d) Set_ApCli_WirelessMode_Proc = %d\n",
					pObj->ioctl_if, PhyMode));

	printk("SupRate[0]=%x\n",rate->SupRate[0]);
	return TRUE;
}


INT Set_ApCli_TxMcs_Proc(
	IN  PRTMP_ADAPTER pAd,
	IN  RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	struct wifi_dev *wdev;


	if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	wdev = &pAd->ApCfg.ApCliTab[pObj->ioctl_if].wdev;

	wdev->DesiredTransmitSetting.field.MCS =
			RT_CfgSetTxMCSProc(arg, &wdev->bAutoTxRateSwitch);

	if (wdev->DesiredTransmitSetting.field.MCS == MCS_AUTO)
	{
		MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(apcli%d) Set_ApCli_TxMcs_Proc = AUTO\n", pObj->ioctl_if));
	}
	else
	{
		MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(apcli%d) Set_ApCli_TxMcs_Proc = %d\n",
					pObj->ioctl_if, wdev->DesiredTransmitSetting.field.MCS));
	}

	return TRUE;
}

#ifdef APCLI_CONNECTION_TRIAL
INT Set_ApCli_Trial_Ch_Proc(
	RTMP_ADAPTER *pAd, 
	RTMP_STRING *arg)
{
	POS_COOKIE 		pObj;
	UCHAR 			ifIndex;
	PAPCLI_STRUCT	pApCliEntry = NULL;

	pObj = (POS_COOKIE) pAd->OS_Cookie;
	if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	ifIndex = pObj->ioctl_if;
	if (ifIndex != (pAd->ApCfg.ApCliNum-1)) {
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, 
		("\n\rI/F(apcli%d) can not run connection trial, use apcli%d\n", 
		 ifIndex,(MAX_APCLI_NUM-1)));
		return FALSE;
	}
	
	if (pAd->CommonCfg.dbdc_mode == TRUE) {
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, 
				 ("\n\rI/F(apcli%d) can not run connection trial with DBDC mode\n", 
				  ifIndex));
		return FALSE;
	}
	pApCliEntry = &pAd->ApCfg.ApCliTab[ifIndex];

	pApCliEntry->TrialCh = simple_strtol(arg, 0, 10);
	
	if (pApCliEntry->TrialCh)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(apcli%d) pApCliEntry->TrialCh = %d\n", ifIndex, pApCliEntry->TrialCh));
	}
	else
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(apcli%d) pApCliEntry->TrialCh = %d\n", ifIndex, pApCliEntry->TrialCh));
	}
#ifdef DBDC_MODE
	if (HcGetBandByWdev(&pApCliEntry->wdev) != 
			HcGetBandByChannel(pAd,pApCliEntry->TrialCh)) {
		pApCliEntry->TrialCh = 0;
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, 
				("I/F(apcli%d) TrialCh = %d is not in this phy mode(%s)\n", 
				ifIndex, pApCliEntry->TrialCh,wmode_2_str(pApCliEntry->wdev.PhyMode)));
		return FALSE;
	}
#endif

	return TRUE;
}
#endif /* APCLI_CONNECTION_TRIAL */


#ifdef MAC_REPEATER_SUPPORT
INT Set_Cli_Link_Map_Proc(
    IN  PRTMP_ADAPTER pAd,
    IN  RTMP_STRING *arg)
{
    POS_COOKIE          pObj= (POS_COOKIE) pAd->OS_Cookie;
    UCHAR               ifIndex;
    UCHAR MbssIdx = 0;
    struct wifi_dev *cli_link_wdev = NULL;
    struct wifi_dev *mbss_link_wdev = NULL;

    if (pAd->ApCfg.bMACRepeaterEn != TRUE)
    {
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_ERROR,
                ("Rept has not been enabled yet.\n"));
        return FALSE;
    }

    if ((pObj->ioctl_if_type != INT_APCLI) &&
        (pObj->ioctl_if_type != INT_MSTA))
    {
        return FALSE;
    }
    ifIndex = pObj->ioctl_if;

    if (ifIndex >= MAX_APCLI_NUM) /* should < MAX_APCLI_NUM to prevent ApCliTab array out of bound*/
    {
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_ERROR,
                ("wrong cli link idx:%d to set link map.\n",
                    ifIndex));
        return FALSE;
    }

    cli_link_wdev = &pAd->ApCfg.ApCliTab[ifIndex].wdev;

    MbssIdx = simple_strtol(arg, 0, 10);
    if (MbssIdx >= HW_BEACON_MAX_NUM)  /* MbssIdx 16 will cause MBSSID array out of bound */
    {
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_ERROR,
                ("wrong mbss idx:%d to set link map.\n",
                    MbssIdx));
        return FALSE;
    }

    mbss_link_wdev = &pAd->ApCfg.MBSSID[MbssIdx].wdev;
    UpdateMbssCliLinkMap(pAd, MbssIdx, cli_link_wdev, mbss_link_wdev);

    return TRUE;
}

INT Set_ReptMode_Enable_Proc(
	IN  PRTMP_ADAPTER pAd,
	IN  RTMP_STRING *arg)
{
	UCHAR Enable = simple_strtol(arg, 0, 10);

	MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("MACRepeaterEn = %d, Repeater Mode (%s)\n",
				pAd->ApCfg.bMACRepeaterEn, (Enable ? "ON" : "OFF")));

    AsicSetReptFuncEnable(pAd, Enable);

	return TRUE;
}



#endif /* MAC_REPEATER_SUPPORT */

#ifdef APCLI_AUTO_CONNECT_SUPPORT
/*
    ==========================================================================
    Description:
        Trigger Apcli Auto connect to find the missed AP.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/

/*
0 : Disable apcli auto connect
1 : User Trigger Scan Mode 
2 : Partial Scan Mode
3 : Driver Trigger Scan Mode
*/

INT Set_ApCli_AutoConnect_Proc(
	IN PRTMP_ADAPTER pAd,
	IN RTMP_STRING *arg)
{
	POS_COOKIE  		pObj= (POS_COOKIE) pAd->OS_Cookie;
	UCHAR				ifIndex;
	AP_ADMIN_CONFIG *pApCfg;
	NDIS_802_11_SSID Ssid;

	long scan_mode = simple_strtol(arg, 0, 10);

	if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	if (ApScanRunning(pAd) == TRUE) {
		MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Set_ApCli_AutoConnect_Proc() is still running\n"));
		return TRUE;
	}
	pApCfg = &pAd->ApCfg;
	ifIndex = pObj->ioctl_if;
	NdisZeroMemory(&Ssid, sizeof(NDIS_802_11_SSID));

	if (scan_mode == 0) {//disable it
		pApCfg->ApCliTab[ifIndex].AutoConnectFlag = FALSE;
		pApCfg->ApCliAutoConnectRunning[ifIndex] = FALSE;
		pApCfg->ApCliAutoBWAdjustCnt[ifIndex] = 0;
		pApCfg->ApCliAutoConnectType[ifIndex] = 0;
		return TRUE;
	}
	pApCfg->ApCliTab[ifIndex].AutoConnectFlag = TRUE;

		Set_ApCli_Enable_Proc(pAd, "0");
	pApCfg->ApCliAutoConnectRunning[ifIndex] = TRUE;
	pApCfg->ApCliAutoBWAdjustCnt[ifIndex] = 0;
	MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(apcli%d) Set_ApCli_AutoConnect_Proc::(Len=%d,Ssid=%s)\n",
			ifIndex, pApCfg->ApCliTab[ifIndex].CfgSsidLen, pApCfg->ApCliTab[ifIndex].CfgSsid));
	/*
		use site survey function to trigger auto connecting (when pAd->ApCfg.ApAutoConnectRunning == TRUE)
	*/
	switch (scan_mode)
	{
		case 2:
			pAd->ScanCtrl.PartialScan.pwdev = &pApCfg->ApCliTab[ifIndex].wdev;
			pAd->ScanCtrl.PartialScan.bScanning = TRUE;
			pAd->ApCfg.ApCliAutoConnectType[ifIndex] = TRIGGER_SCAN_BY_USER;	
			break;
		case 3:
			pAd->ApCfg.ApCliAutoConnectType[ifIndex] = TRIGGER_SCAN_BY_DRIVER;
			ApSiteSurvey_by_wdev(pAd, &Ssid, SCAN_ACTIVE, FALSE, &pApCfg->ApCliTab[ifIndex].wdev);
			break;

		case 1:
			//FALL Through:
		default:
			pAd->ApCfg.ApCliAutoConnectType[ifIndex] = TRIGGER_SCAN_BY_USER;	
			ApSiteSurvey_by_wdev(pAd, &Ssid, SCAN_ACTIVE, FALSE, &pApCfg->ApCliTab[ifIndex].wdev);
			break;
	}

	return TRUE;
}
#endif  /* APCLI_AUTO_CONNECT_SUPPORT */


#ifdef WSC_AP_SUPPORT
INT Set_AP_WscSsid_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
    POS_COOKIE 		pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR			ifIndex = pObj->ioctl_if;
	PWSC_CTRL	    pWscControl = &pAd->ApCfg.ApCliTab[ifIndex].WscControl;

    if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	NdisZeroMemory(&pWscControl->WscSsid, sizeof(NDIS_802_11_SSID));

	if( (strlen(arg) > 0) && (strlen(arg) <= MAX_LEN_OF_SSID))
    {
		NdisMoveMemory(pWscControl->WscSsid.Ssid, arg, strlen(arg));
		pWscControl->WscSsid.SsidLength = strlen(arg);

		NdisZeroMemory(pAd->ApCfg.ApCliTab[ifIndex].CfgSsid, MAX_LEN_OF_SSID);
		NdisMoveMemory(pAd->ApCfg.ApCliTab[ifIndex].CfgSsid, arg, strlen(arg));
		pAd->ApCfg.ApCliTab[ifIndex].CfgSsidLen = (UCHAR)strlen(arg);

		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscSsid_Proc:: (Select SsidLen=%d,Ssid=%s)\n",
				pWscControl->WscSsid.SsidLength, pWscControl->WscSsid.Ssid));
	}
	else
		return FALSE;	/*Invalid argument */

	return TRUE;

}
#endif /* WSC_AP_SUPPORT */

#ifdef APCLI_CERT_SUPPORT
INT Set_ApCli_Cert_Enable_Proc(
	IN  RTMP_ADAPTER *pAd, 
	IN  RTMP_STRING *arg)
{
	UINT Enable;
	POS_COOKIE pObj;
	UCHAR ifIndex;
	
	pObj = (POS_COOKIE) pAd->OS_Cookie;
	if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	ifIndex = pObj->ioctl_if;
	
	Enable = simple_strtol(arg, 0, 16);

	pAd->bApCliCertTest = (Enable > 0) ? TRUE : FALSE;

	if (pAd->bApCliCertTest == TRUE)
	{
		pAd->CommonCfg.bEnableTxBurst = FALSE;
	}

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(apcli%d) Set_ApCli_Cert_Enable_Proc::(enable = %d)\n",
				ifIndex, pAd->bApCliCertTest));
	
	return TRUE;
}

//Add for APCLI PMF 5.3.3.3 option test item. (Only Tx De-auth Req. and make sure the pkt can be Encrypted)
INT ApCliTxDeAuth(
	IN PRTMP_ADAPTER pAd,
	IN  RTMP_STRING *arg)
{
	USHORT Reason = (USHORT)REASON_DEAUTH_STA_LEAVING;

#ifdef MAC_REPEATER_SUPPORT
	REPEATER_CLIENT_ENTRY *pReptEntry = NULL;
	UCHAR CliIdx = 0xFF;
#endif /* MAC_REPEATER_SUPPORT */

	HEADER_802_11 DeauthHdr;
	PUCHAR pOutBuffer = NULL;
	ULONG FrameLen = 0;
	NDIS_STATUS NStatus;
	POS_COOKIE pObj;
	UCHAR ifIndex;

	pObj = (POS_COOKIE) pAd->OS_Cookie;
	if (pObj->ioctl_if_type != INT_APCLI)
		return FALSE;

	ifIndex = pObj->ioctl_if;
		
	MTWF_LOG(DBG_CAT_CLIENT, CATCLIENT_APCLI, DBG_LVL_TRACE, ("%s : ifIndex=%u\n", __FUNCTION__,ifIndex));

	if ((ifIndex >= MAX_APCLI_NUM)
#ifdef MAC_REPEATER_SUPPORT
		&& (ifIndex < REPT_MLME_START_IDX)
#endif /* MAC_REPEATER_SUPPORT */
		)
		return FALSE;

#ifdef MAC_REPEATER_SUPPORT
	if (ifIndex >= REPT_MLME_START_IDX)
	{
		CliIdx = ifIndex - REPT_MLME_START_IDX;
		pReptEntry = &pAd->ApCfg.pRepeaterCliPool[CliIdx];
		ifIndex = pReptEntry->wdev->func_idx;
	}
#endif /* MAC_REPEATER_SUPPORT */

	NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);  /*Get an unused nonpaged memory */
	if (NStatus != NDIS_STATUS_SUCCESS)
	{
		return FALSE;
	}
	MTWF_LOG(DBG_CAT_CLIENT, CATCLIENT_APCLI, DBG_LVL_TRACE, ("%s: DE-AUTH request (Reason=%d)...\n", __FUNCTION__, Reason));

	ApCliMgtMacHeaderInit(pAd, &DeauthHdr, SUBTYPE_DEAUTH, 0, pAd->ApCfg.ApCliTab[ifIndex].MlmeAux.Bssid, pAd->ApCfg.ApCliTab[ifIndex].MlmeAux.Bssid, ifIndex);
#ifdef MAC_REPEATER_SUPPORT
	if (CliIdx != 0xFF)
        COPY_MAC_ADDR(DeauthHdr.Addr2, pAd->ApCfg.pRepeaterCliPool[CliIdx].CurrentAddress);
#endif /* MAC_REPEATER_SUPPORT */

	MakeOutgoingFrame(pOutBuffer,           &FrameLen,
		sizeof(HEADER_802_11),&DeauthHdr,
		2,                    &Reason,
		END_OF_ARGS);
	MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
	MlmeFreeMemory( pOutBuffer);

	return TRUE;
}

#endif /* APCLI_CERT_SUPPORT */
#endif /* APCLI_SUPPORT */


#ifdef WSC_AP_SUPPORT
#ifdef CON_WPS
static  INT WscPushConcurrentPBCAction(
        IN      PRTMP_ADAPTER   pAd,
        IN      PWSC_CTRL   pWscControl,
        IN      BOOLEAN     bIsApCli)
{
        POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
        UCHAR           apidx = pObj->ioctl_if;
        INT                 idx;

        if (bIsApCli)
                pWscControl->WscConfMode = WSC_ENROLLEE;
        else
                pWscControl->WscConfMode = WSC_REGISTRAR;

        WscInit(pAd, bIsApCli, apidx);
        pWscControl->WscMode = WSC_PBC_MODE;
        WscGetRegDataPIN(pAd, pWscControl->WscPinCode, pWscControl);

        WscStop(pAd, bIsApCli, pWscControl);
        pWscControl->RegData.ReComputePke = 1;
        WscInitRegistrarPair(pAd, pWscControl, apidx);
        for (idx = 0; idx < 192; idx++)
                pWscControl->RegData.EnrolleeRandom[idx] = RandomByte(pAd);

        pWscControl->bWscAutoTigeer = FALSE;

        if (bIsApCli)
        {
                pAd->ApCfg.ApCliTab[apidx].Enable = FALSE;
                ApCliIfDown(pAd);

                pWscControl->WscSsid.SsidLength = 0;
                NdisZeroMemory(&pWscControl->WscSsid, sizeof(NDIS_802_11_SSID));
                pWscControl->WscPBCBssCount = 0;
                /* WPS - SW PBC */
                WscPushPBCAction(pAd, pWscControl);
        }
        else
        {
		struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[apidx].wdev;

                WscBuildBeaconIE(pAd, pWscControl->WscConfStatus, TRUE, DEV_PASS_ID_PBC,
                                                                pWscControl->WscConfigMethods, apidx, NULL, 0, AP_MODE);

                WscBuildProbeRespIE(pAd, WSC_MSGTYPE_AP_WLAN_MGR, pWscControl->WscConfStatus, TRUE, DEV_PASS_ID_PBC,
                                                                pWscControl->WscConfigMethods, apidx, NULL, 0, AP_MODE);
		UpdateBeaconHandler(pAd, wdev, IE_CHANGE);

                RTMPSetTimer(&pWscControl->Wsc2MinsTimer, WSC_TWO_MINS_TIME_OUT);
                pWscControl->Wsc2MinsTimerRunning = TRUE;
                pWscControl->WscStatus = STATUS_WSC_LINK_UP;
                pWscControl->bWscTrigger = TRUE;

                RTMP_SEM_LOCK(&pWscControl->WscPeerListSemLock);
                WscClearPeerList(&pWscControl->WscPeerList);
                RTMP_SEM_UNLOCK(&pWscControl->WscPeerListSemLock);

        }

        return TRUE;
}

INT     Set_ConWpsApCliMode_Proc(
	RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
#ifdef MULTI_INF_SUPPORT
	UINT Mode = CON_WPS_APCLI_BAND_AUTO;
	UINT opposBandIdx = !multi_inf_get_idx(pAd);
	PRTMP_ADAPTER pOpposAd=NULL;

	Mode = simple_strtol(arg, 0, 10);
	
	if (Mode >= CON_WPS_APCLI_BAND_MAX)
		return FALSE;

	pOpposAd = (PRTMP_ADAPTER)adapt_list[opposBandIdx];

	pAd->ApCfg.ConWpsApCliMode = Mode;

	if (pOpposAd != NULL)
	{
	pOpposAd->ApCfg.ConWpsApCliMode = Mode;
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s Now: %s, Oppos: %s, Mode = %d\n", 
			__FUNCTION__, pAd->net_dev->name, pOpposAd->net_dev->name, Mode));
	} else {
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s Now: %s, Mode = %d\n", 
			__FUNCTION__, pAd->net_dev->name, Mode));		
	}
#else
	UINT Mode = CON_WPS_APCLI_BAND_AUTO;
	Mode = simple_strtol(arg, 0, 10);
	
	if (Mode >= CON_WPS_APCLI_BAND_MAX)
		return FALSE;
	
	pAd->ApCfg.ConWpsApCliMode = Mode;
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s Now: %s, Mode = %d\n", 
				__FUNCTION__, pAd->net_dev->name, Mode));		
#endif	
	return TRUE;
}

INT     Set_ConWpsApcliAutoPreferIface_Proc(
	RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
#ifdef MULTI_INF_SUPPORT
	UINT PreferIface = CON_WPS_APCLI_AUTO_PREFER_IFACE0;
	UINT opposBandIdx = !multi_inf_get_idx(pAd);
	PRTMP_ADAPTER pOpposAd = NULL;

	PreferIface = simple_strtol(arg, 0, 10);
	
	if (PreferIface >= CON_WPS_APCLI_AUTO_PREFER_IFACE_MAX)
		return FALSE;

	pOpposAd = (PRTMP_ADAPTER)adapt_list[opposBandIdx];
	pAd->ApCfg.ConWpsApcliAutoPreferIface = PreferIface;

	if (pOpposAd != NULL)
	{
	pOpposAd->ApCfg.ConWpsApcliAutoPreferIface = PreferIface;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s Now: %s, Oppos: %s, PreferIface = %d\n", 
			__FUNCTION__, pAd->net_dev->name, pOpposAd->net_dev->name, PreferIface));
	} else {
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s Now: %s, PreferIface = %d\n", 
				__FUNCTION__, pAd->net_dev->name, PreferIface));	
	}
#else
	UINT PreferIface = CON_WPS_APCLI_AUTO_PREFER_IFACE0;
	PreferIface = simple_strtol(arg, 0, 10);

	pAd->ApCfg.ConWpsApcliAutoPreferIface = PreferIface;
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s Now: %s, PreferIface = %d\n", 
				__FUNCTION__, pAd->net_dev->name, PreferIface));
		
#endif	
	return TRUE;
}

INT     Set_ConWpsApCliDisabled_Proc(
        RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
#ifdef MULTI_INF_SUPPORT
        UINT Disabled = FALSE;
        UINT opposBandIdx = !multi_inf_get_idx(pAd);
        PRTMP_ADAPTER pOpposAd=NULL;

        Disabled = simple_strtol(arg, 0, 10);
        pOpposAd = (PRTMP_ADAPTER)adapt_list[opposBandIdx];
      	pAd->ApCfg.ConWpsApCliDisableSetting = Disabled;

	if (pOpposAd != NULL)
	{		
        pOpposAd->ApCfg.ConWpsApCliDisableSetting = Disabled;

        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s Now: %s, Oppos: %s, ApClient Disabled = %d\n",
                        __FUNCTION__, pAd->net_dev->name, pOpposAd->net_dev->name, Disabled));
	} else {
	        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s Now: %s, ApClient Disabled = %d\n",
	                        __FUNCTION__, pAd->net_dev->name, Disabled));	
	}
#else
	UINT Disabled = FALSE;
	Disabled = simple_strtol(arg, 0, 10);
	pAd->ApCfg.ConWpsApCliDisableSetting = Disabled;
	        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s Now: %s, ApClient Disabled = %d\n",
	                        __FUNCTION__, pAd->net_dev->name, Disabled));	

#endif	
        return TRUE;
}
#endif /* CON_WPS */

INT	 Set_AP_WscConfMode_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	INT         ConfModeIdx;
	/*INT         IsAPConfigured; */
    POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR	    apidx = pObj->ioctl_if, mac_addr[MAC_ADDR_LEN];
    BOOLEAN     bFromApCli = FALSE;
    PWSC_CTRL   pWscControl;
#ifdef CON_WPS
	struct wifi_dev *pWdev=NULL;
#endif	

	ConfModeIdx = simple_strtol(arg, 0, 10);

#ifdef HOSTAPD_SUPPORT
	if (pAd->ApCfg.MBSSID[apidx].Hostapd == Hostapd_EXT)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WPS is control by hostapd now.\n"));
		return FALSE;
	}
#endif /*HOSTAPD_SUPPORT*/

#ifdef CON_WPS
        if (ConfModeIdx == WSC_ENROLLEE_REGISTRAR)
        {
		UINT ApClientWcid = 0; 
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("WPS is using concurrent WPS now apidx=%d\n",apidx));

	        if ((pAd->ApCfg.ConWpsApCliDisableSetting == TRUE) && 
	        	(apidx < pAd->ApCfg.ApCliNum) &&
		    (pAd->ApCfg.ApCliTab[apidx].CtrlCurrState == APCLI_CTRL_CONNECTED))
                {
                        ApClientWcid = pAd->ApCfg.ApCliTab[apidx].MacTabWCID;
                        if ((pAd->MacTab.Content[ApClientWcid].Sst == SST_ASSOC) &&
                           (pAd->MacTab.tr_entry[ApClientWcid].PortSecured == WPA_802_1X_PORT_SECURED))
			{
                        	pAd->ApCfg.ConWpsApCliDisabled = TRUE;
			}
                }

		if (pAd->ApCfg.ConWpsApCliDisabled == TRUE)
                {
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("Diabled the ApClient when using concurrent WPS now.\n"));
		}	
		else if ((apidx < pAd->ApCfg.ApCliNum) &&
				(pAd->ApCfg.ApCliTab[apidx].Enable))
		{
			pWdev=&(pAd->ApCfg.ApCliTab[apidx].wdev);

			if (pWdev != NULL)
		{			
                	pWscControl = &pAd->ApCfg.ApCliTab[apidx].WscControl;
				pWscControl->conWscStatus= CON_WPS_STATUS_APCLI_RUNNING;
				 MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("Set_AP_WscConfMode_Proc enter  WscPushConcurrentPBCAction(apcli%d) pWscControl=0x%p\n",apidx, pWscControl));
				RTMPZeroMemory(pAd->ApCfg.ApCliTab[apidx].WscControl.IfName, IFNAMSIZ);
				RTMPMoveMemory(pAd->ApCfg.ApCliTab[apidx].WscControl.IfName, pWdev->if_dev->name, IFNAMSIZ);					
                	WscPushConcurrentPBCAction(pAd, pWscControl, TRUE);
		}
		}
               
                pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;
		pWscControl->conWscStatus= CON_WPS_STATUS_AP_RUNNING;
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("Set_AP_WscConfMode_Proc enter  WscPushConcurrentPBCAction(ra%d)\n",apidx));		
                WscPushConcurrentPBCAction(pAd, pWscControl, FALSE);

                return TRUE;
        }
#endif /* CON_WPS */

#ifdef APCLI_SUPPORT
    if (pObj->ioctl_if_type == INT_APCLI)
    {
        bFromApCli = TRUE;
        pWscControl = &pAd->ApCfg.ApCliTab[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_WscConfMode_Proc:: This command is from apcli interface now.\n", apidx));
    }
    else
#endif /* APCLI_SUPPORT */
    {
        bFromApCli = FALSE;
        pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscConfMode_Proc:: This command is from ra interface now.\n", apidx));
    }

    pWscControl->bWscTrigger = FALSE;
    if ((ConfModeIdx & WSC_ENROLLEE_PROXY_REGISTRAR) == WSC_DISABLE)
    {
        pWscControl->WscConfMode = WSC_DISABLE;
		pWscControl->WscStatus = STATUS_WSC_NOTUSED;
        if (bFromApCli)
        {
            MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_WscConfMode_Proc:: WPS is disabled.\n", apidx));
        }
        else
        {
            struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[apidx].wdev;

            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscConfMode_Proc:: WPS is disabled.\n", apidx));
            /* Clear WPS IE in Beacon and ProbeResp */
            pAd->ApCfg.MBSSID[apidx].WscIEBeacon.ValueLen = 0;
        	pAd->ApCfg.MBSSID[apidx].WscIEProbeResp.ValueLen = 0;
			UpdateBeaconHandler(
                            pAd,
                            wdev,
                            IE_CHANGE);
        }
    }
    else
    {
#ifdef APCLI_SUPPORT
        if (bFromApCli)
        {
            if (ConfModeIdx == WSC_ENROLLEE)
            {
                pWscControl->WscConfMode = WSC_ENROLLEE;
                WscInit(pAd, TRUE, apidx);
            }
            else
            {
                MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_WscConfMode_Proc:: Ap Client only supports Enrollee mode.(ConfModeIdx=%d)\n",
						apidx, ConfModeIdx));
                return FALSE;
            }
        }
        else
#endif /* APCLI_SUPPORT */
        {
        	pWscControl->WscConfMode = (ConfModeIdx & WSC_ENROLLEE_PROXY_REGISTRAR);
            WscInit(pAd, FALSE, apidx);
        }
        pWscControl->WscStatus = STATUS_WSC_IDLE;
    }

#ifdef APCLI_SUPPORT
    if (bFromApCli)
    {
        memcpy(mac_addr, &pAd->ApCfg.ApCliTab[apidx].wdev.if_addr[0], MAC_ADDR_LEN);
    }
    else
#endif /* APCLI_SUPPORT */
    {
        memcpy(mac_addr, &pAd->ApCfg.MBSSID[apidx].wdev.bssid[0], MAC_ADDR_LEN);
    }

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(%02X:%02X:%02X:%02X:%02X:%02X) Set_WscConfMode_Proc::(WscConfMode(0~7)=%d)\n",
                            PRINT_MAC(mac_addr), pWscControl->WscConfMode));
	return TRUE;
}

INT	Set_AP_WscConfStatus_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	UCHAR       IsAPConfigured = 0;
	INT         IsSelectedRegistrar;
	USHORT      WscMode;
    POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR	    apidx = pObj->ioctl_if;

#ifdef HOSTAPD_SUPPORT
	if (pAd->ApCfg.MBSSID[apidx].Hostapd == Hostapd_EXT)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WPS is control by hostapd now.\n"));
		return FALSE;
	}
#endif /*HOSTAPD_SUPPORT*/


#ifdef APCLI_SUPPORT
    if (pObj->ioctl_if_type == INT_APCLI)
    {
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_WscConfStatus_Proc:: Ap Client doesn't need this command.\n", apidx));
        return FALSE;
    }
#endif /* APCLI_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
#ifdef WSC_V2_SUPPORT
	if ((pAd->ApCfg.MBSSID[apidx].WscControl.WscV2Info.bWpsEnable == FALSE) &&
		(pAd->ApCfg.MBSSID[apidx].WscControl.WscV2Info.bEnableWpsV2))
	{
		pAd->ApCfg.MBSSID[apidx].WscIEBeacon.ValueLen = 0;
       	pAd->ApCfg.MBSSID[apidx].WscIEProbeResp.ValueLen = 0;
		return FALSE;
	}
#endif /* WSC_V2_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */


	IsAPConfigured = (UCHAR)simple_strtol(arg, 0, 10);
	IsSelectedRegistrar = pAd->ApCfg.MBSSID[apidx].WscControl.WscSelReg;
    if (pAd->ApCfg.MBSSID[apidx].WscControl.WscMode == 1)
		WscMode = DEV_PASS_ID_PIN;
	else
		WscMode = DEV_PASS_ID_PBC;

	if ((IsAPConfigured  > 0) && (IsAPConfigured  <= 2))
    {
        struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[apidx].wdev;

        pAd->ApCfg.MBSSID[apidx].WscControl.WscConfStatus = IsAPConfigured;
        /* Change SC State of WPS IE in Beacon and ProbeResp */
        WscBuildBeaconIE(pAd, IsAPConfigured, IsSelectedRegistrar, WscMode, 0, apidx, NULL, 0, AP_MODE);
    	WscBuildProbeRespIE(pAd, WSC_MSGTYPE_AP_WLAN_MGR, IsAPConfigured, IsSelectedRegistrar, WscMode, 0, apidx, NULL, 0, AP_MODE);
		UpdateBeaconHandler(
                            pAd,
                            wdev,
                            IE_CHANGE);
    }
	else
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscConfStatus_Proc:: Set failed!!(WscConfStatus=%s), WscConfStatus is 1 or 2 \n", apidx, arg));
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscConfStatus_Proc:: WscConfStatus is not changed (%d) \n", apidx, pAd->ApCfg.MBSSID[apidx].WscControl.WscConfStatus));
		return FALSE;  /*Invalid argument */
	}

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(%02X:%02X:%02X:%02X:%02X:%02X) Set_WscConfStatus_Proc::(WscConfStatus=%d)\n",
				PRINT_MAC(pAd->ApCfg.MBSSID[apidx].wdev.bssid),
                               pAd->ApCfg.MBSSID[apidx].WscControl.WscConfStatus));

	return TRUE;
}

INT	Set_AP_WscMode_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	INT         WscMode;
    POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR	    apidx = pObj->ioctl_if, mac_addr[MAC_ADDR_LEN];
    PWSC_CTRL   pWscControl;
    BOOLEAN     bFromApCli = FALSE;

#ifdef HOSTAPD_SUPPORT
	if (pAd->ApCfg.MBSSID[apidx].Hostapd == Hostapd_EXT)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WPS is control by hostapd now.\n"));
		return FALSE;
	}
#endif /*HOSTAPD_SUPPORT*/

#ifdef APCLI_SUPPORT
    if (pObj->ioctl_if_type == INT_APCLI)
    {
        bFromApCli = TRUE;
        pWscControl = &pAd->ApCfg.ApCliTab[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_WscMode_Proc:: This command is from apcli interface now.\n", apidx));
    }
    else
#endif /* APCLI_SUPPORT */
    {
        bFromApCli = FALSE;
        pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscMode_Proc:: This command is from ra interface now.\n", apidx));
    }

	WscMode = simple_strtol(arg, 0, 10);

    if ((WscMode  > 0) && (WscMode  <= 2))
    {
        pWscControl->WscMode = WscMode;
        if (WscMode == WSC_PBC_MODE)
        {
	        WscGetRegDataPIN(pAd, pWscControl->WscPinCode, pWscControl);
        }
    }
	else
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscMode_Proc:: Set failed!!(Set_WscMode_Proc=%s), WscConfStatus is 1 or 2 \n", arg));
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscMode_Proc:: WscMode is not changed (%d) \n", pWscControl->WscMode));
		return FALSE;  /*Invalid argument */
	}

#ifdef APCLI_SUPPORT
    if (bFromApCli)
    {
        memcpy(mac_addr, pAd->ApCfg.ApCliTab[apidx].wdev.if_addr, MAC_ADDR_LEN);
    }
    else
#endif /* APCLI_SUPPORT */
    {
        memcpy(mac_addr, pAd->ApCfg.MBSSID[apidx].wdev.bssid, MAC_ADDR_LEN);
    }
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(%02X:%02X:%02X:%02X:%02X:%02X) Set_WscMode_Proc::(WscMode=%d)\n",
					PRINT_MAC(mac_addr), pWscControl->WscMode));

	return TRUE;
}

INT	Set_WscStatus_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
    POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR	    apidx = pObj->ioctl_if;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscStatus_Proc::(WscStatus=%d)\n", apidx, pAd->ApCfg.MBSSID[apidx].WscControl.WscStatus));
	return TRUE;
}

#define WSC_GET_CONF_MODE_EAP	1
#define WSC_GET_CONF_MODE_UPNP	2
INT	Set_AP_WscGetConf_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	INT                 WscMode, wscGetConfMode = 0;
	INT                 IsAPConfigured;
	PWSC_CTRL           pWscControl;
	PWSC_UPNP_NODE_INFO pWscUPnPNodeInfo;
    INT	                idx;
    POS_COOKIE          pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR	            apidx = pObj->ioctl_if, mac_addr[MAC_ADDR_LEN];
    BOOLEAN             bFromApCli = FALSE;
#ifdef APCLI_SUPPORT
	BOOLEAN 			apcliEn = pAd->ApCfg.ApCliTab[apidx].Enable;
#endif /* APCLI_SUPPORT */
#ifdef WSC_V2_SUPPORT
	PWSC_V2_INFO		pWscV2Info = NULL;
#endif /* WSC_V2_SUPPORT */
#ifdef WSC_LED_SUPPORT
	UCHAR WPSLEDStatus;
#endif /* WSC_LED_SUPPORT */

#ifdef HOSTAPD_SUPPORT
	if (pAd->ApCfg.MBSSID[apidx].Hostapd == Hostapd_EXT)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WPS is control by hostapd now.\n"));
		return FALSE;
	}
#endif /*HOSTAPD_SUPPORT*/

#ifdef APCLI_SUPPORT
    if (pObj->ioctl_if_type == INT_APCLI)
    {
    	if (pAd->ApCfg.ApCliTab[apidx].ApCliInit == FALSE)
    	{
    		MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_AP_WscGetConf_Proc:: ApCli is disabled.\n", apidx));
    		return FALSE;
    	}
        bFromApCli = TRUE;
		apidx &= (~MIN_NET_DEVICE_FOR_APCLI);
        pWscControl = &pAd->ApCfg.ApCliTab[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_AP_WscGetConf_Proc:: This command is from apcli interface now.\n", apidx));
    }
    else
#endif /* APCLI_SUPPORT */
    {
        bFromApCli = FALSE;
        pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_AP_WscGetConf_Proc:: This command is from ra interface now.\n", apidx));
    }

	NdisZeroMemory(mac_addr, MAC_ADDR_LEN);

#ifdef WSC_V2_SUPPORT
	pWscV2Info = &pWscControl->WscV2Info;
#endif /* WSC_V2_SUPPORT */
	wscGetConfMode = simple_strtol(arg, 0, 10);

    IsAPConfigured = pWscControl->WscConfStatus;
    pWscUPnPNodeInfo = &pWscControl->WscUPnPNodeInfo;

    if ((pWscControl->WscConfMode == WSC_DISABLE)
#ifdef WSC_V2_SUPPORT
		|| ((pWscV2Info->bWpsEnable == FALSE) && (pWscV2Info->bEnableWpsV2))
#endif /* WSC_V2_SUPPORT */
		)
    {
        pWscControl->bWscTrigger = FALSE;
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscGetConf_Proc: WPS is disabled.\n"));
		return FALSE;
    }

	WscStop(pAd, bFromApCli, pWscControl);

	/* trigger wsc re-generate public key */
    pWscControl->RegData.ReComputePke = 1;

	if (pWscControl->WscMode == 1)
		WscMode = DEV_PASS_ID_PIN;
	else
	{
		WscMode = DEV_PASS_ID_PBC;
	}
	WscInitRegistrarPair(pAd, pWscControl, apidx);
    /* Enrollee 192 random bytes for DH key generation */
	for (idx = 0; idx < 192; idx++)
		pWscControl->RegData.EnrolleeRandom[idx] = RandomByte(pAd);

#ifdef APCLI_SUPPORT
	if (bFromApCli)
	{
		/* bring apcli interface down first */
		pAd->ApCfg.ApCliTab[apidx].Enable = FALSE;
		ApCliIfDown(pAd);

		if (WscMode == DEV_PASS_ID_PIN)
		{
			NdisMoveMemory(pWscControl->RegData.SelfInfo.MacAddr,
				pAd->ApCfg.ApCliTab[apidx].wdev.if_addr,6);

			pAd->ApCfg.ApCliTab[apidx].Enable = apcliEn;			
		}
		else
		{
			pWscControl->WscSsid.SsidLength = 0;
			NdisZeroMemory(&pWscControl->WscSsid, sizeof(NDIS_802_11_SSID));
			pWscControl->WscPBCBssCount = 0;
			/* WPS - SW PBC */
			WscPushPBCAction(pAd, pWscControl);
		}
		NdisMoveMemory(mac_addr, pAd->ApCfg.ApCliTab[apidx].wdev.if_addr, MAC_ADDR_LEN);
	}
	else
#endif /* APCLI_SUPPORT */
	{
        struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[apidx].wdev;

		WscBuildBeaconIE(pAd, IsAPConfigured, TRUE, WscMode, pWscControl->WscConfigMethods, apidx, NULL, 0, AP_MODE);
		WscBuildProbeRespIE(pAd, WSC_MSGTYPE_AP_WLAN_MGR, IsAPConfigured, TRUE, WscMode, pWscControl->WscConfigMethods, apidx, NULL, 0, AP_MODE);
		UpdateBeaconHandler(
                            pAd,
                            wdev,
                            IE_CHANGE);
		NdisMoveMemory(mac_addr, pAd->ApCfg.MBSSID[apidx].wdev.bssid, MAC_ADDR_LEN);
	}

#ifdef APCLI_SUPPORT
	if (bFromApCli && (WscMode == DEV_PASS_ID_PBC))
		;
	else
#endif /* APCLI_SUPPORT */
	{
	    /* 2mins time-out timer */
	    RTMPSetTimer(&pWscControl->Wsc2MinsTimer, WSC_TWO_MINS_TIME_OUT);
	    pWscControl->Wsc2MinsTimerRunning = TRUE;
	    pWscControl->WscStatus = STATUS_WSC_LINK_UP;
		pWscControl->bWscTrigger = TRUE;
	}
	pWscControl->bWscAutoTigeer = FALSE;

    if (!bFromApCli)
    {
    	if (WscMode == DEV_PASS_ID_PIN)
    	{
    		WscAssignEntryMAC(pAd, pWscControl);
			WscSendUPnPConfReqMsg(pAd, pWscControl->EntryIfIdx,
	    							(PUCHAR)pAd->ApCfg.MBSSID[pWscControl->EntryIfIdx].Ssid,
	    							pAd->ApCfg.MBSSID[apidx].wdev.bssid, 3, 0, AP_MODE);
    	}
		else
		{
			RTMP_SEM_LOCK(&pWscControl->WscPeerListSemLock);
			WscClearPeerList(&pWscControl->WscPeerList);
			RTMP_SEM_UNLOCK(&pWscControl->WscPeerListSemLock);
		}
    }

#ifdef WSC_LED_SUPPORT
	WPSLEDStatus = LED_WPS_IN_PROCESS;
	RTMPSetLED(pAd, WPSLEDStatus);
#endif /* WSC_LED_SUPPORT */

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(%02X:%02X:%02X:%02X:%02X:%02X) Set_WscGetConf_Proc trigger WSC state machine, wscGetConfMode=%d\n",
				PRINT_MAC(mac_addr), wscGetConfMode));

	return TRUE;
}

INT	Set_AP_WscPinCode_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	UINT        PinCode = 0;
	BOOLEAN     validatePin, bFromApCli = FALSE;
    POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR       apidx = pObj->ioctl_if, mac_addr[MAC_ADDR_LEN];
    PWSC_CTRL   pWscControl;
#define IsZero(c) ('0' == (c) ? TRUE:FALSE)
	PinCode = simple_strtol(arg, 0, 10); /* When PinCode is 03571361, return value is 3571361. */

#ifdef HOSTAPD_SUPPORT
	if (pAd->ApCfg.MBSSID[apidx].Hostapd == Hostapd_EXT)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WPS is control by hostapd now.\n"));
		return FALSE;
	}
#endif /*HOSTAPD_SUPPORT*/

#ifdef APCLI_SUPPORT
    if (pObj->ioctl_if_type == INT_APCLI)
    {
        bFromApCli = TRUE;
        pWscControl = &pAd->ApCfg.ApCliTab[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_WscPinCode_Proc:: This command is from apcli interface now.\n", apidx));
    }
    else
#endif /* APCLI_SUPPORT */
    {
        bFromApCli = FALSE;
        pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscPinCode_Proc:: This command is from ra interface now.\n", apidx));
    }

	if (strlen(arg) == 4)
		validatePin = TRUE;
	else
	validatePin = ValidateChecksum(PinCode);

	if ( validatePin )
	{
	    if (pWscControl->WscRejectSamePinFromEnrollee &&
            (PinCode == pWscControl->WscLastPinFromEnrollee))
        {
            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("PIN authentication or communication error occurs!!\n"
                                      "Registrar does NOT accept the same PIN again!(PIN:%s)\n", arg));
            return FALSE;
        }
        else
        {
    		pWscControl->WscPinCode = PinCode;
            pWscControl->WscLastPinFromEnrollee = pWscControl->WscPinCode;
            pWscControl->WscRejectSamePinFromEnrollee = FALSE;
            /* PIN Code */
			if (strlen(arg) == 4)
			{
				pWscControl->WscPinCodeLen = 4;
				pWscControl->RegData.PinCodeLen = 4;
				NdisMoveMemory(pWscControl->RegData.PIN, arg, 4);
			}
			else
			{
				pWscControl->WscPinCodeLen = 8;

            if (IsZero(*arg))
				{
					pWscControl->RegData.PinCodeLen = 8;
                NdisMoveMemory(pWscControl->RegData.PIN, arg, 8);
				}
            else
    	        WscGetRegDataPIN(pAd, pWscControl->WscPinCode, pWscControl);
        }
	}
	}
	else
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set failed!!(Set_WscPinCode_Proc=%s), PinCode Checksum invalid \n", arg));
		return FALSE;  /*Invalid argument */
	}

#ifdef APCLI_SUPPORT
    if (bFromApCli)
    {
        memcpy(mac_addr, pAd->ApCfg.ApCliTab[apidx].wdev.if_addr, MAC_ADDR_LEN);
    }
    else
#endif /* APCLI_SUPPORT */
    {
        memcpy(mac_addr, pAd->ApCfg.MBSSID[apidx].wdev.bssid, MAC_ADDR_LEN);
    }
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(%02X:%02X:%02X:%02X:%02X:%02X) Set_WscPinCode_Proc::(PinCode=%d)\n",
					PRINT_MAC(mac_addr), pWscControl->WscPinCode));

	return TRUE;
}

INT	Set_WscOOB_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
    char        *pTempSsid = NULL;
    POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR       apidx = pObj->ioctl_if;

#ifdef HOSTAPD_SUPPORT
	if (pAd->ApCfg.MBSSID[apidx].Hostapd == Hostapd_EXT)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WPS is control by hostapd now.\n"));
		return FALSE;
	}
#endif /*HOSTAPD_SUPPORT*/

#ifdef APCLI_SUPPORT
    if (pObj->ioctl_if_type == INT_APCLI)
    {
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_WscPinCode_Proc:: Ap Client doesn't need this command.\n", apidx));
        return FALSE;
    }
#endif /* APCLI_SUPPORT */

    Set_WscSetupLock_Proc(pAd, "0");
    Set_AP_WscConfStatus_Proc(pAd, "1");
    Set_SecAuthMode_Proc(pAd, "WPA2PSK");
    Set_SecEncrypType_Proc(pAd, "AES");
    pTempSsid = vmalloc(33);
    if (pTempSsid)
    {
        memset(pTempSsid, 0, 33);
        snprintf(pTempSsid, 33,"RalinkInitialAP%02X%02X%02X",
						pAd->ApCfg.MBSSID[apidx].wdev.bssid[3],
                                          pAd->ApCfg.MBSSID[apidx].wdev.bssid[4],
                                          pAd->ApCfg.MBSSID[apidx].wdev.bssid[5]);
        Set_AP_SSID_Proc(pAd, pTempSsid);
        vfree(pTempSsid);
    }
	Set_SecWPAPSK_Proc(pAd, "RalinkInitialAPxx1234");

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscOOB_Proc\n", apidx));
	return TRUE;
}

INT	Set_WscStop_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
    POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR	    apidx = pObj->ioctl_if;
    PWSC_CTRL   pWscControl;
    BOOLEAN     bFromApCli = FALSE;

#ifdef HOSTAPD_SUPPORT
	if (pAd->ApCfg.MBSSID[apidx].Hostapd == Hostapd_EXT)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WPS is control by hostapd now.\n"));
		return FALSE;
	}
#endif /*HOSTAPD_SUPPORT*/


#ifdef APCLI_SUPPORT
    if (pObj->ioctl_if_type == INT_APCLI)
    {
        bFromApCli = TRUE;
        pWscControl = &pAd->ApCfg.ApCliTab[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_WscStop_Proc:: This command is from apcli interface now.\n", apidx));
    }
    else
#endif /* APCLI_SUPPORT */
    {
        bFromApCli = FALSE;
        pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscStop_Proc:: This command is from ra interface now.\n", apidx));
    }

#ifdef APCLI_SUPPORT
    if (bFromApCli)
    {
	    WscStop(pAd, TRUE, pWscControl);
		pWscControl->WscConfMode = WSC_DISABLE;
    }
    else
#endif /* APCLI_SUPPORT */
    {
        INT	 IsAPConfigured = pWscControl->WscConfStatus;
        struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[apidx].wdev;

        WscBuildBeaconIE(pAd, IsAPConfigured, FALSE, 0, 0, apidx, NULL, 0, AP_MODE);
		WscBuildProbeRespIE(pAd, WSC_MSGTYPE_AP_WLAN_MGR, IsAPConfigured, FALSE, 0, 0, apidx, NULL, 0, AP_MODE);
		UpdateBeaconHandler(
                            pAd,
                            wdev,
                            IE_CHANGE);
        WscStop(pAd, FALSE, pWscControl);
    }

    pWscControl->bWscTrigger = FALSE;
    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("<===== Set_WscStop_Proc"));
    return TRUE;
}

/*
    ==========================================================================
    Description:
        Get WSC Profile
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
        		1.) iwpriv ra0 get_wsc_profile
        		3.) UI needs to prepare at least 4096bytes to get the results
    ==========================================================================
*/
VOID RTMPIoctlWscProfile(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	WSC_CONFIGURED_VALUE Profile;
	RTMP_STRING *msg;
	POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR	    apidx = pObj->ioctl_if;
	BSS_STRUCT *pMbss;
	struct wifi_dev *wdev;


	pMbss = &pAd->ApCfg.MBSSID[apidx];
	wdev = &pMbss->wdev;
#ifdef HOSTAPD_SUPPORT
	if (pMbss->Hostapd == Hostapd_EXT)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("WPS is control by hostapd now.\n"));
		return;
	}
#endif /*HOSTAPD_SUPPORT*/


	memset(&Profile, 0x00, sizeof(WSC_CONFIGURED_VALUE));
	Profile.WscConfigured = pMbss->WscControl.WscConfStatus;
	NdisZeroMemory(Profile.WscSsid, 32 + 1);
	NdisMoveMemory(Profile.WscSsid, pMbss->Ssid, pMbss->SsidLen);
	Profile.WscSsid[pMbss->SsidLen] = '\0';
	Profile.WscAuthMode = WscGetAuthType(wdev->SecConfig.AKMMap);
	Profile.WscEncrypType = WscGetEncryType(wdev->SecConfig.PairwiseCipher);
	NdisZeroMemory(Profile.WscWPAKey, 64 + 1);

	if (Profile.WscEncrypType == 2)
	{
		Profile.DefaultKeyIdx = wdev->SecConfig.PairwiseKeyId+ 1;
		{
			int i;
			for (i=0; i< wdev->SecConfig.WepKey[Profile.DefaultKeyIdx].KeyLen; i++)
			{
				snprintf((RTMP_STRING *) Profile.WscWPAKey, sizeof(Profile.WscWPAKey),
						"%s%02x", Profile.WscWPAKey,
									wdev->SecConfig.WepKey[Profile.DefaultKeyIdx].Key[i]);
			}
			Profile.WscWPAKey[(wdev->SecConfig.WepKey[Profile.DefaultKeyIdx].KeyLen)*2] = '\0';
		}
	}
	else if (Profile.WscEncrypType >= 4)
	{
		Profile.DefaultKeyIdx = 2;
		NdisMoveMemory(Profile.WscWPAKey, pMbss->WscControl.WpaPsk,
						pMbss->WscControl.WpaPskLen);
		Profile.WscWPAKey[pMbss->WscControl.WpaPskLen] = '\0';
	}
	else
	{
		Profile.DefaultKeyIdx = 1;
	}

	wrq->u.data.length = sizeof(Profile);

	if (copy_to_user(wrq->u.data.pointer, &Profile, wrq->u.data.length))
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s: copy_to_user() fail\n", __FUNCTION__));
	}

	os_alloc_mem(pAd, (UCHAR **)&msg, sizeof(CHAR)*(2048));
	if (msg == NULL) {
		return;
	}

	memset(msg, 0x00, 2048);
	sprintf(msg,"%s","\n");

	if (Profile.WscEncrypType == 1)
	{
		sprintf(msg+strlen(msg),"%-12s%-33s%-12s%-12s\n", "Configured", "SSID", "AuthMode", "EncrypType");
	}
	else if (Profile.WscEncrypType == 2)
	{
		sprintf(msg+strlen(msg),"%-12s%-33s%-12s%-12s%-13s%-26s\n", "Configured", "SSID", "AuthMode", "EncrypType", "DefaultKeyID", "Key");
	}
	else
	{
		sprintf(msg+strlen(msg),"%-12s%-33s%-12s%-12s%-64s\n", "Configured", "SSID", "AuthMode", "EncrypType", "Key");
	}

	if (Profile.WscConfigured == 1)
		sprintf(msg+strlen(msg),"%-12s", "No");
	else
		sprintf(msg+strlen(msg),"%-12s", "Yes");
	sprintf(msg+strlen(msg), "%-33s", Profile.WscSsid);
	if (IS_AKM_WPA1PSK(wdev->SecConfig.AKMMap) || IS_AKM_WPA2PSK(wdev->SecConfig.AKMMap))
		sprintf(msg+strlen(msg), "%-12s", "WPAPSKWPA2PSK");
	else
		sprintf(msg+strlen(msg), "%-12s", WscGetAuthTypeStr(Profile.WscAuthMode));
	if (IS_CIPHER_TKIP(wdev->SecConfig.PairwiseCipher) || IS_CIPHER_CCMP128(wdev->SecConfig.PairwiseCipher))
		sprintf(msg+strlen(msg), "%-12s", "TKIPAES");
	else
		sprintf(msg+strlen(msg), "%-12s", WscGetEncryTypeStr(Profile.WscEncrypType));

	if (Profile.WscEncrypType == 1)
	{
		sprintf(msg+strlen(msg), "%s\n", "");
	}
	else if (Profile.WscEncrypType == 2)
	{
		sprintf(msg+strlen(msg), "%-13d",Profile.DefaultKeyIdx);
		sprintf(msg+strlen(msg), "%-26s\n",Profile.WscWPAKey);
	}
	else if (Profile.WscEncrypType >= 4)
	{
	    sprintf(msg+strlen(msg), "%-64s\n",Profile.WscWPAKey);
	}
#ifdef INF_AR9
	wrq->u.data.length = strlen(msg);
	copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length);
#endif/* INF_AR9 */

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s", msg));
	os_free_mem( msg);
}

#if defined(INF_AR9) || defined(BB_SOC)
#if defined(AR9_MAPI_SUPPORT) || defined(BB_SOC)

/*
    ==========================================================================
    Description:
        Get WSC Profile
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
        		1.) iwpriv ra0 ar9_show get_wsc_profile
        		3.) UI needs to prepare at least 4096bytes to get the results
    ==========================================================================
*/
VOID RTMPAR9IoctlWscProfile(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	WSC_CONFIGURED_VALUE Profile;
	RTMP_STRING *msg;
	POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR	    apidx = pObj->ioctl_if;
	BSS_STRUCT *pMbss;
	struct wifi_dev *wdev;

	pMbss = &pAd->ApCfg.MBSSID[apidx];
	wdev = &pMbss->wdev;

	memset(&Profile, 0x00, sizeof(WSC_CONFIGURED_VALUE));
	Profile.WscConfigured = pAd->ApCfg.MBSSID[apidx].WscControl.WscConfStatus;
	NdisZeroMemory(Profile.WscSsid, 32 + 1);
	NdisMoveMemory(Profile.WscSsid, pAd->ApCfg.MBSSID[apidx].Ssid,
								    pAd->ApCfg.MBSSID[apidx].SsidLen);
	Profile.WscSsid[pAd->ApCfg.MBSSID[apidx].SsidLen] = '\0';
	Profile.WscAuthMode = WscGetAuthType(wdev->SecConfig.AKMMap);
	Profile.WscEncrypType = WscGetEncryType(wdev->SecConfig.PairwiseCipher);
	NdisZeroMemory(Profile.WscWPAKey, 64 + 1);
	
	if (Profile.WscEncrypType == 2)
	{
		Profile.DefaultKeyIdx = wdev->SecConfig.PairwiseKeyId + 1;
		{
				int i;
				for (i=0; i< wdev->SecConfig.WepKey[Profile.DefaultKeyIdx].KeyLen; i++)
				{
					snprintf((RTMP_STRING *) Profile.WscWPAKey, sizeof(Profile.WscWPAKey),
							"%s%02x", Profile.WscWPAKey,
										wdev->SecConfig.WepKey[Profile.DefaultKeyIdx].Key[i]);
				}
				Profile.WscWPAKey[(wdev->SecConfig.WepKey[Profile.DefaultKeyIdx].KeyLen)*2] = '\0';
		}
	}	
	else if (Profile.WscEncrypType >= 4)
	{
		Profile.DefaultKeyIdx = 2;
		NdisMoveMemory(Profile.WscWPAKey, pAd->ApCfg.MBSSID[apidx].WscControl.WpaPsk,
						pAd->ApCfg.MBSSID[apidx].WscControl.WpaPskLen);
		Profile.WscWPAKey[pAd->ApCfg.MBSSID[apidx].WscControl.WpaPskLen] = '\0';
	}
	else
	{
		Profile.DefaultKeyIdx = 1;
	}

	os_alloc_mem(pAd, (UCHAR **)&msg, sizeof(CHAR)*(2048));
	if (msg == NULL) {
		return;
	}

	memset(msg, 0x00, 2048);
	sprintf(msg,"%s","\n");

	if (Profile.WscEncrypType == 1)
	{
		sprintf(msg+strlen(msg),"%-12s%-33s%-12s%-12s\n", "Configured", "SSID", "AuthMode", "EncrypType");
	}
	else if (Profile.WscEncrypType == 2)
	{
		sprintf(msg+strlen(msg),"%-12s%-33s%-12s%-12s%-13s%-26s\n", "Configured", "SSID", "AuthMode", "EncrypType", "DefaultKeyID", "Key");
	}
	else
	{
		sprintf(msg+strlen(msg),"%-12s%-33s%-12s%-12s%-64s\n", "Configured", "SSID", "AuthMode", "EncrypType", "Key");
	}

	if (Profile.WscConfigured == 1)
		sprintf(msg+strlen(msg),"%-12s", "No");
	else
		sprintf(msg+strlen(msg),"%-12s", "Yes");
	sprintf(msg+strlen(msg), "%-33s", Profile.WscSsid);
	if (IS_AKM_WPA1PSK(wdev->SecConfig.AKMMap) || IS_AKM_WPA2PSK(wdev->SecConfig.AKMMap))
		sprintf(msg+strlen(msg), "%-12s", "WPAPSKWPA2PSK");
	else
		sprintf(msg+strlen(msg), "%-12s", WscGetAuthTypeStr(Profile.WscAuthMode));
	if (IS_CIPHER_TKIP(wdev->SecConfig.PairwiseCipher) || IS_CIPHER_CCMP128(wdev->SecConfig.PairwiseCipher))
		sprintf(msg+strlen(msg), "%-12s", "TKIPAES");
	else
		sprintf(msg+strlen(msg), "%-12s", WscGetEncryTypeStr(Profile.WscEncrypType));

	if (Profile.WscEncrypType == 1)
	{
		sprintf(msg+strlen(msg), "%s\n", "");
	}
	else if (Profile.WscEncrypType == 2)
	{
		sprintf(msg+strlen(msg), "%-13d",Profile.DefaultKeyIdx);
		sprintf(msg+strlen(msg), "%-26s\n",Profile.WscWPAKey);
	}
	else if (Profile.WscEncrypType >= 4)
	{
	    sprintf(msg+strlen(msg), "%-64s\n",Profile.WscWPAKey);
	}

	wrq->u.data.length = strlen(msg);
	copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length);
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s", msg));
	}
	os_free_mem( msg);
}

VOID RTMPIoctlWscPINCode(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	RTMP_STRING *msg;
	POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR	    apidx = pObj->ioctl_if;
	UCHAR        tempPIN[9]={0};

	os_alloc_mem(NULL, (UCHAR **)&msg, sizeof(CHAR)*(128));
	if (msg == NULL) {
		return;
	}

	memset(msg, 0x00, 128);
	sprintf(msg,"%s","\n");
	sprintf(msg+strlen(msg),"WSC_PINCode=");
	if(pAd->ApCfg.MBSSID[apidx].WscControl.WscEnrolleePinCode)
	{
		if (pAd->ApCfg.MBSSID[apidx].WscControl.WscEnrolleePinCodeLen == 8)
			sprintf((RTMP_STRING *) tempPIN, "%08u", pAd->ApCfg.MBSSID[apidx].WscControl.WscEnrolleePinCode);
		else
			sprintf((RTMP_STRING *) tempPIN, "%04u", pAd->ApCfg.MBSSID[apidx].WscControl.WscEnrolleePinCode);
		sprintf(msg,"%s%s\n",msg,tempPIN);
	}
	wrq->u.data.length = strlen(msg);
	copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length);
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s", msg));
	}

	os_free_mem( msg);
}

VOID RTMPIoctlWscStatus(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	RTMP_STRING *msg;
	POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR	    apidx = pObj->ioctl_if;

	os_alloc_mem(NULL, (UCHAR **)&msg, sizeof(CHAR)*(128));
	if (msg == NULL) {
		return;
	}

	memset(msg, 0x00, 128);
	sprintf(msg,"%s","\n");
	sprintf(msg+strlen(msg),"WSC_Status=");
	sprintf(msg,"%s%d\n",msg,pAd->ApCfg.MBSSID[apidx].WscControl.WscStatus);
	wrq->u.data.length = strlen(msg);
	copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length);
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s", msg));
	}

	os_free_mem( msg);
}

VOID RTMPIoctlGetWscDynInfo(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	char *msg;
	BSS_STRUCT *pMbss;
	INT apidx,configstate;

	os_alloc_mem(NULL, (UCHAR **)&msg, sizeof(CHAR)*(pAd->ApCfg.BssidNum*(14*128)));
	if (msg == NULL)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s():Alloc memory failed\n", __FUNCTION__));
		return;
	}
	memset(msg, 0 ,pAd->ApCfg.BssidNum*(14*128));
	sprintf(msg,"%s","\n");

	for (apidx=0; apidx<pAd->ApCfg.BssidNum; apidx++)
	{
		pMbss=&pAd->ApCfg.MBSSID[apidx];

		if(pMbss->WscControl.WscConfStatus == WSC_SCSTATE_UNCONFIGURED)
			configstate = 0;
		else
			configstate = 1;

		sprintf(msg+strlen(msg),"ra%d\n",apidx);
#ifdef BB_SOC
		sprintf(msg+strlen(msg),"DeviceName = %s\n",(pMbss->WscControl.RegData.SelfInfo.DeviceName));
#endif
		sprintf(msg+strlen(msg),"UUID = %s\n",(pMbss->WscControl.Wsc_Uuid_Str));
		sprintf(msg+strlen(msg),"wpsVersion = 0x%x\n",WSC_VERSION);
		sprintf(msg+strlen(msg),"setuoLockedState = %d\n",0);
		sprintf(msg+strlen(msg),"configstate = %d\n",configstate);
		sprintf(msg+strlen(msg),"lastConfigError = %d\n",0);

	}

	wrq->u.data.length = strlen(msg);
	if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s", msg));
	}

	os_free_mem( msg);
}

VOID RTMPIoctlGetWscRegsDynInfo(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	char *msg;
	BSS_STRUCT *pMbss;
	INT apidx;

	os_alloc_mem(NULL, (UCHAR **)&msg, sizeof(CHAR)*(pAd->ApCfg.BssidNum*(14*128)));
	if (msg == NULL)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s():Alloc memory failed\n", __FUNCTION__));
		return;
	}
	memset(msg, 0 ,pAd->ApCfg.BssidNum*(14*128));
	sprintf(msg,"%s","\n");

	for (apidx=0; apidx<pAd->ApCfg.BssidNum; apidx++)
	{
		pMbss=&pAd->ApCfg.MBSSID[apidx];
		sprintf(msg+strlen(msg),"ra%d\n",apidx);
#ifdef BB_SOC
		sprintf(msg+strlen(msg),"DeviceName = %s\n",(pMbss->WscControl.RegData.SelfInfo.DeviceName));
#endif
		sprintf(msg+strlen(msg),"UUID_R = %s\n",(pMbss->WscControl.RegData.PeerInfo.Uuid));
	}

	wrq->u.data.length = strlen(msg);
	if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s", msg));
	}

	os_free_mem( msg);
}
#endif /* defined(AR9_MAPI_SUPPORT) || defined(BB_SOC) */
#endif /* defined(INF_AR9) || defined(BB_SOC) */

BOOLEAN WscCheckEnrolleeNonceFromUpnp(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	RTMP_STRING *pData,
	IN  USHORT			Length,
	IN  PWSC_CTRL       pWscControl)
{
	USHORT	WscType, WscLen;
    USHORT  WscId = WSC_ID_ENROLLEE_NONCE;

    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("check Enrollee Nonce\n"));

    /* We have to look for WSC_IE_MSG_TYPE to classify M2 ~ M8, the remain size must large than 4 */
	while (Length > 4)
	{
		WSC_IE	TLV_Recv;
        char ZeroNonce[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		memcpy((UINT8 *)&TLV_Recv, pData, 4);
		WscType = be2cpu16(TLV_Recv.Type);
		WscLen  = be2cpu16(TLV_Recv.Length);
		pData  += 4;
		Length -= 4;

		if (WscType == WscId)
		{
			if (RTMPCompareMemory(pWscControl->RegData.SelfNonce, pData, 16) == 0)
			{
			    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Nonce match!!\n"));
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("<----- WscCheckNonce\n"));
				return TRUE;
			}
            else if (NdisEqualMemory(pData, ZeroNonce, 16))
            {
                /* Intel external registrar will send WSC_NACK with enrollee nonce */
                /* "10 1A 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00" */
                /* when AP is configured and user selects not to configure AP. */
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Zero Enrollee Nonce!!\n"));
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("<----- WscCheckNonce\n"));
                return TRUE;
            }
		}

		/* Offset to net WSC Ie */
		pData  += WscLen;
		Length -= WscLen;
	}

    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Nonce mismatch!!\n"));
    return FALSE;
}

UCHAR	WscRxMsgTypeFromUpnp(
	IN	PRTMP_ADAPTER		pAdapter,
	IN  RTMP_STRING *	pData,
	IN	USHORT				Length)
{

	USHORT WscType, WscLen;

    {   /* Eap-Esp(Messages) */
        /* the first TLV item in EAP Messages must be WSC_IE_VERSION */
        NdisMoveMemory(&WscType, pData, 2);
        if (ntohs(WscType) != WSC_ID_VERSION)
            goto out;

        /* Not Wsc Start, We have to look for WSC_IE_MSG_TYPE to classify M2 ~ M8, the remain size must large than 4 */
		while (Length > 4)
		{
			/* arm-cpu has packet alignment issue, it's better to use memcpy to retrieve data */
			NdisMoveMemory(&WscType, pData, 2);
			NdisMoveMemory(&WscLen,  pData + 2, 2);
			WscLen = ntohs(WscLen);
			if (ntohs(WscType) == WSC_ID_MSG_TYPE)
			{
				return(*(pData + 4));	/* Found the message type */
			}
			else
			{
				pData  += (WscLen + 4);
				Length -= (WscLen + 4);
			}
		}
    }

out:
	return  WSC_MSG_UNKNOWN;
}

VOID RTMPIoctlSetWSCOOB(
	IN PRTMP_ADAPTER pAd)
{
    char        *pTempSsid = NULL;
    POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR       apidx = pObj->ioctl_if;
    pObj->pSecConfig = &pAd->ApCfg.MBSSID[apidx].wdev.SecConfig;

#ifdef APCLI_SUPPORT
    if (pObj->ioctl_if_type == INT_APCLI)
    {
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_WscPinCode_Proc:: Ap Client doesn't need this command.\n", apidx));
        return;
    }
#endif /* APCLI_SUPPORT */

    Set_WscSetupLock_Proc(pAd, "0");
    Set_AP_WscConfStatus_Proc(pAd, "1");
    Set_SecAuthMode_Proc(pAd, "WPAPSK");
    Set_SecEncrypType_Proc(pAd, "TKIP");
    pTempSsid = vmalloc(33);
    if (pTempSsid)
    {
        memset(pTempSsid, 0, 33);
        snprintf(pTempSsid, 33, "RalinkInitialAP%02X%02X%02X",
				pAd->ApCfg.MBSSID[apidx].wdev.bssid[3],
				pAd->ApCfg.MBSSID[apidx].wdev.bssid[4],
				pAd->ApCfg.MBSSID[apidx].wdev.bssid[5]);
        Set_AP_SSID_Proc(pAd, pTempSsid);
        vfree(pTempSsid);
    }
	Set_SecWPAPSK_Proc(pAd, "RalinkInitialAPxx1234");

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscOOB_Proc\n", apidx));
	return;
}

/*
	==========================================================================
	Description:
	Set Wsc Security Mode
	0 : WPA2PSK AES
	1 : WPA2PSK TKIP
	2 : WPAPSK AES
	3 : WPAPSK TKIP
	Return:
	TRUE if all parameters are OK, FALSE otherwise
	==========================================================================
*/
INT	Set_AP_WscSecurityMode_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR		apidx = pObj->ioctl_if;

	if (strcmp(arg, "0") == 0)
		pAd->ApCfg.MBSSID[apidx].WscSecurityMode = WPA2PSKAES;
	else if (strcmp(arg, "1") == 0)
		pAd->ApCfg.MBSSID[apidx].WscSecurityMode = WPA2PSKTKIP;
	else if (strcmp(arg, "2") == 0)
		pAd->ApCfg.MBSSID[apidx].WscSecurityMode = WPAPSKAES;
	else if (strcmp(arg, "3") == 0)
		pAd->ApCfg.MBSSID[apidx].WscSecurityMode = WPAPSKTKIP;
	else
		return FALSE;
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscSecurityMode_Proc::(WscSecurityMode=%d)\n",
		apidx, pAd->ApCfg.MBSSID[apidx].WscSecurityMode ));

	return TRUE;
}

INT Set_AP_WscMultiByteCheck_Proc(
	IN  PRTMP_ADAPTER   pAd,
	IN  RTMP_STRING *arg)
{
	POS_COOKIE 		pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR			apidx = pObj->ioctl_if;
	BOOLEAN			bEnable = FALSE;
	PWSC_CTRL		pWpsCtrl = NULL;
	BOOLEAN     	bFromApCli = FALSE;

#ifdef APCLI_SUPPORT
    if (pObj->ioctl_if_type == INT_APCLI)
    {
        bFromApCli = TRUE;
        pWpsCtrl = &pAd->ApCfg.ApCliTab[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CLIENT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(apcli%d) Set_WscConfMode_Proc:: This command is from apcli interface now.\n", apidx));
    }
    else
#endif /* APCLI_SUPPORT */
    {
        bFromApCli = FALSE;
        pWpsCtrl = &pAd->ApCfg.MBSSID[apidx].WscControl;
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_WscConfMode_Proc:: This command is from ra interface now.\n", apidx));
    }

	if (strcmp(arg, "0") == 0)
		bEnable = FALSE;
	else if (strcmp(arg, "1") == 0)
		bEnable = TRUE;
	else
		return FALSE;

	if (pWpsCtrl->bCheckMultiByte != bEnable)
	{
		pWpsCtrl->bCheckMultiByte = bEnable;
	}

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) Set_AP_WscMultiByteCheck_Proc::(bCheckMultiByte=%d)\n",
		apidx, pWpsCtrl->bCheckMultiByte));

	return TRUE;
}

INT	Set_WscVersion_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR	version = (UCHAR)simple_strtol(arg, 0, 16);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscVersion_Proc::(version=%x)\n",version));
	pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.RegData.SelfInfo.Version = version;
	return TRUE;
}

#ifdef WSC_V2_SUPPORT
INT	Set_WscFragment_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR	bool_flag = (UCHAR)simple_strtol(arg, 0, 16);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscFragment_Proc::(bool_flag=%d)\n",bool_flag));
	pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.bWscFragment = bool_flag;
	return TRUE;
}

INT	Set_WscFragmentSize_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;
	USHORT		WscFragSize = (USHORT)simple_strtol(arg, 0, 10);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscFragmentSize_Proc::(WscFragSize=%d)\n", WscFragSize));
	if ((WscFragSize >=128) && (WscFragSize <=300))
		pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.WscFragSize = WscFragSize;
	return TRUE;
}

INT	Set_WscSetupLock_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR		bEnable = (UCHAR)simple_strtol(arg, 0, 10);
	PWSC_CTRL	pWscControl = &pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl;

	if (bEnable == 0)
	{
		BOOLEAN bCancelled = FALSE;
		pWscControl->PinAttackCount = 0;
		if (pWscControl->WscSetupLockTimerRunning)
		{
			RTMPCancelTimer(&pWscControl->WscSetupLockTimer, &bCancelled);
		}
		WscSetupLockTimeout(NULL, pWscControl, NULL, NULL);
	}
	else
	{
        struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev;

		pWscControl->bSetupLock = TRUE;
		WscBuildBeaconIE(pAd,
						 pWscControl->WscConfStatus,
						 FALSE,
						 0,
						 0,
						 pObj->ioctl_if,
						 NULL,
						 0,
						 AP_MODE);
		WscBuildProbeRespIE(pAd,
							WSC_MSGTYPE_AP_WLAN_MGR,
							pWscControl->WscConfStatus,
							FALSE,
							0,
							0,
							pObj->ioctl_if,
							NULL,
							0,
							AP_MODE);
		UpdateBeaconHandler(
                            pAd,
                            wdev,
                            IE_CHANGE);
	}

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscSetupLock_Proc::(bSetupLock=%d)\n",
								pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.bSetupLock));
	return TRUE;
}

INT	Set_WscV2Support_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR bEnable = (UCHAR)simple_strtol(arg, 0, 10);
	PWSC_CTRL pWscControl = &pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl;

	if (bEnable == 0)
		pWscControl->WscV2Info.bEnableWpsV2 = FALSE;
	else
		pWscControl->WscV2Info.bEnableWpsV2 = TRUE;

	if (pWscControl->WscV2Info.bEnableWpsV2)
	{
        struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev;

		/*
			WPS V2 doesn't support Chiper WEP and TKIP.
		*/
		struct _SECURITY_CONFIG *pSecConfig = &wdev->SecConfig;
		if (IS_CIPHER_WEP_TKIP_ONLY(pSecConfig->PairwiseCipher)
			|| (pAd->ApCfg.MBSSID[pObj->ioctl_if].bHideSsid))
			WscOnOff(pAd, wdev->func_idx, TRUE);
		else
			WscOnOff(pAd, wdev->func_idx, FALSE);

		UpdateBeaconHandler(
                            pAd,
                            wdev,
                            IE_CHANGE);
	}
	else
	{
		WscInit(pAd, FALSE, pObj->ioctl_if);
	}

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscV2Support_Proc::(bEnableWpsV2=%d)\n",
								pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.WscV2Info.bEnableWpsV2));
	return TRUE;
}

INT	Set_WscVersion2_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR	version = (UCHAR)simple_strtol(arg, 0, 16);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscVersion2_Proc::(version=%x)\n",version));
	if (version >= 0x20)
		pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.RegData.SelfInfo.Version2 = version;
	else
		return FALSE;
	return TRUE;
}

INT	Set_WscExtraTlvTag_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;
	USHORT		new_tag = (USHORT)simple_strtol(arg, 0, 16);

	pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.WscV2Info.ExtraTlv.TlvTag = new_tag;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscExtraTlvTag_Proc::(new_tag=0x%04X)\n",new_tag));
	return TRUE;
}

INT	Set_WscExtraTlvType_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR		type = (UCHAR)simple_strtol(arg, 0, 10);

	pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.WscV2Info.ExtraTlv.TlvType = type;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscExtraTlvType_Proc::(type=%d)\n",type));
	return TRUE;
}

INT	Set_WscExtraTlvData_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE		pObj = (POS_COOKIE) pAd->OS_Cookie;
	UINT			DataLen = (UINT)strlen(arg);
	PWSC_TLV		pWscTLV = &pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.WscV2Info.ExtraTlv;
	INT				i;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscExtraTlvData_Proc::(DataLen = %d)\n", DataLen));

	if ((DataLen != 0) && (pWscTLV->TlvType == TLV_HEX))
	{
		for(i=0; i < DataLen; i++)
		{
			if( !isxdigit(*(arg+i)) )
				return FALSE;  /*Not Hex value; */
		}
	}

	if (pWscTLV->pTlvData)
	{
		os_free_mem( pWscTLV->pTlvData);
		pWscTLV->pTlvData = NULL;
	}

	if (DataLen == 0)
		return TRUE;

	pWscTLV->TlvLen = 0;
	os_alloc_mem(NULL, &pWscTLV->pTlvData, DataLen);
	if (pWscTLV->pTlvData)
	{
		if (pWscTLV->TlvType == TLV_ASCII)
		{
			NdisMoveMemory(pWscTLV->pTlvData, arg, DataLen);
			pWscTLV->TlvLen = DataLen;
		}
		else
		{
			pWscTLV->TlvLen = DataLen/2;
			AtoH(arg, pWscTLV->pTlvData, pWscTLV->TlvLen);
		}
		return TRUE;
	}
	else
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscExtraTlvData_Proc::os_alloc_mem fail\n"));

	return FALSE;
}

INT	Set_WscMaxPinAttack_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR		MaxPinAttack = (UCHAR)simple_strtol(arg, 0, 10);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscMaxPinAttack_Proc::(MaxPinAttack=%d)\n", MaxPinAttack));
	pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.MaxPinAttack = MaxPinAttack;
	return TRUE;
}


INT	Set_WscSetupLockTime_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;
	UINT		SetupLockTime = (UINT)simple_strtol(arg, 0, 10);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscSetupLockTime_Proc::(SetupLockTime=%d)\n", SetupLockTime));
	pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl.SetupLockTime = SetupLockTime;
	return TRUE;
}

#endif /* WSC_V2_SUPPORT */

INT	Set_WscAutoTriggerDisable_Proc(
	IN	RTMP_ADAPTER	*pAd,
	IN	RTMP_STRING		*arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR bEnable = (UCHAR)simple_strtol(arg, 0, 10);
	PWSC_CTRL pWscCtrl = &pAd->ApCfg.MBSSID[pObj->ioctl_if].WscControl;

	if (bEnable == 0)
		pWscCtrl->bWscAutoTriggerDisable = FALSE;
	else
		pWscCtrl->bWscAutoTriggerDisable = TRUE;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_WscAutoTriggerDisable_Proc::(bWscAutoTriggerDisable=%d)\n",
								pWscCtrl->bWscAutoTriggerDisable));
	return TRUE;
}

#endif /* WSC_AP_SUPPORT */



#ifdef IAPP_SUPPORT
INT	Set_IappPID_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	unsigned long IappPid;
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;

	IappPid = simple_strtol(arg, 0, 10);
	RTMP_GET_OS_PID(pObj->IappPid, IappPid);
	pObj->IappPid_nr = IappPid;

/*	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("pObj->IappPid = %d", GET_PID_NUMBER(pObj->IappPid))); */
	return TRUE;
} /* End of Set_IappPID_Proc */
#endif /* IAPP_SUPPORT */


INT	Set_DisConnectSta_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	UCHAR					macAddr[MAC_ADDR_LEN];
	RTMP_STRING *value;
	INT						i;
	MAC_TABLE_ENTRY *pEntry = NULL;

	if(strlen(arg) != 17)  /*Mac address acceptable format 01:02:03:04:05:06 length 17 */
		return FALSE;

	for (i=0, value = rstrtok(arg,":"); value; value = rstrtok(NULL,":"))
	{
		if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) )
			return FALSE;  /*Invalid */

		AtoH(value, (UCHAR *)&macAddr[i++], 1);
	}

	pEntry = MacTableLookup(pAd, macAddr);
	if (pEntry)
	{
		MlmeDeAuthAction(pAd, pEntry, REASON_DISASSOC_STA_LEAVING, FALSE);
/*		MacTableDeleteEntry(pAd, pEntry->wcid, Addr); */
	}

	return TRUE;
}

INT Set_DisConnectAllSta_Proc(
        IN PRTMP_ADAPTER pAd,
	IN RTMP_STRING *arg)
{
#ifdef DOT11W_PMF_SUPPORT
        CHAR value = simple_strtol(arg, 0, 10);

	if (value == 2)
        {
		POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
		INT i;

		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("[PMF]%s:: apidx=%d\n", __FUNCTION__, pObj->ioctl_if));
		APMlmeKickOutAllSta(pAd, pObj->ioctl_if, REASON_DEAUTH_STA_LEAVING);
		for (i=1; VALID_UCAST_ENTRY_WCID(pAd, i); i++)
		{
			MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[i];
			if (IS_ENTRY_CLIENT(pEntry)) {
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("[PMF]%s: MacTableDeleteEntry %x:%x:%x:%x:%x:%x\n",
						__FUNCTION__, PRINT_MAC(pEntry->Addr)));
				MacTableDeleteEntry(pAd, pEntry->wcid, pEntry->Addr);
			}
		}
        } else
#endif /* DOT11W_PMF_SUPPORT */
	{
		MacTableReset(pAd);
	}

	return TRUE;
}


#ifdef DOT1X_SUPPORT
/*
    ==========================================================================
    Description:
        Set IEEE8021X.
        This parameter is 1 when 802.1x-wep turn on, otherwise 0
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_IEEE8021X_Proc(
    IN PRTMP_ADAPTER	pAd,
    IN RTMP_STRING *arg)
{
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR apidx = pObj->ioctl_if;
    struct wifi_dev *wdev = NULL;
    struct _SECURITY_CONFIG *pSecConfig = NULL;

    if (apidx >= pAd->ApCfg.BssidNum)
        return FALSE;

    wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
    pSecConfig = &wdev->SecConfig;

    if(simple_strtol(arg, 0, 10) != 0)	/*Enable*/
        pSecConfig->IEEE8021X = TRUE;
    else /*Disable*/
        pSecConfig->IEEE8021X = FALSE;

    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(%s%d) IEEE8021X=%d\n",
        INF_MBSSID_DEV_NAME, apidx, pSecConfig->IEEE8021X));
    return TRUE;
}

/*
    ==========================================================================
    Description:
        Set pre-authentication enable or disable when WPA/WPA2 turn on
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_PreAuth_Proc(
    IN PRTMP_ADAPTER	pAd,
    IN RTMP_STRING *arg)
{
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR apidx = pObj->ioctl_if;
    struct wifi_dev *wdev = NULL;
    struct _SECURITY_CONFIG *pSecConfig = NULL;

    if (apidx >= pAd->ApCfg.BssidNum)
        return FALSE;

    wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
    pSecConfig = &wdev->SecConfig;

    if(simple_strtol(arg, 0, 10) != 0)  /*Enable*/
        pSecConfig->PreAuth = TRUE;
    else /*Disable*/
        pSecConfig->PreAuth = FALSE;

    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("I/F(%s%d) PreAuth=%d\n",
        INF_MBSSID_DEV_NAME, apidx, pSecConfig->PreAuth));

    return TRUE;
}

INT	Set_OwnIPAddr_Proc(
	IN PRTMP_ADAPTER	pAd,
	IN RTMP_STRING *arg)
{
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR apidx = pObj->ioctl_if;
    struct wifi_dev *wdev = NULL;
    struct _SECURITY_CONFIG *pSecConfig = NULL;

    if (apidx >= pAd->ApCfg.BssidNum)
        return FALSE;

    wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
    pSecConfig = &wdev->SecConfig;

    SetWdevOwnIPAddr(pSecConfig, arg);

    return TRUE;
}

INT	Set_EAPIfName_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR apidx = pObj->ioctl_if;
    struct wifi_dev *wdev = NULL;
    struct _SECURITY_CONFIG *pSecConfig = NULL;

    if (apidx >= pAd->ApCfg.BssidNum)
        return FALSE;

    wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
    pSecConfig = &wdev->SecConfig;

    if (strlen(arg) > 0 && strlen(arg) <= IFNAMSIZ)
    {
        pSecConfig->EAPifname_len = strlen(arg);
        NdisMoveMemory(pSecConfig->EAPifname, arg, strlen(arg));
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("EAPifname=%s, len=%d\n",
                pSecConfig->EAPifname, pSecConfig->EAPifname_len));
    }

    return TRUE;
}

INT	Set_PreAuthIfName_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR apidx = pObj->ioctl_if;
    struct wifi_dev *wdev = NULL;
    struct _SECURITY_CONFIG *pSecConfig = NULL;

    if (apidx >= pAd->ApCfg.BssidNum)
        return FALSE;

    wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
    pSecConfig = &wdev->SecConfig;

    if (strlen(arg) > 0 && strlen(arg) <= IFNAMSIZ)
    {
        pSecConfig->PreAuthifname_len = strlen(arg);
        NdisMoveMemory(pSecConfig->PreAuthifname, arg, strlen(arg));
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("PreAuthifname=%s, len=%d\n",
                pSecConfig->PreAuthifname, pSecConfig->PreAuthifname_len));
    }

    return TRUE;
}

INT	Set_RADIUS_Server_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR apidx = pObj->ioctl_if;
    struct wifi_dev *wdev = NULL;
    struct _SECURITY_CONFIG *pSecConfig = NULL;
    UINT32 ip_addr;
    INT count;
    RTMP_STRING *macptr;
    INT srv_cnt = 0;

    if (apidx >= pAd->ApCfg.BssidNum)
        return FALSE;

    wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
    pSecConfig = &wdev->SecConfig;

    for (count = 0, macptr = rstrtok(arg,";"); (macptr && count < MAX_RADIUS_SRV_NUM); macptr = rstrtok(NULL,";"), count++)
    {
        if (rtinet_aton(macptr, &ip_addr))
        {
            PRADIUS_SRV_INFO pSrvInfo = &pSecConfig->radius_srv_info[srv_cnt];

            pSrvInfo->radius_ip = ip_addr;
            srv_cnt++;
            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d), radius_ip(seq-%d)=%s\n",
                apidx, srv_cnt, arg));
	}

	if (srv_cnt > 0)
		pSecConfig->radius_srv_num = srv_cnt;

    }

    return TRUE;
}

INT	Set_RADIUS_Port_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR apidx = pObj->ioctl_if;
    struct wifi_dev *wdev = NULL;
    struct _SECURITY_CONFIG *pSecConfig = NULL;
    RTMP_STRING *macptr;
    INT count;
    INT srv_cnt = 0;

    if (apidx >= pAd->ApCfg.BssidNum)
        return FALSE;

    wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
    pSecConfig = &wdev->SecConfig;
    for (count = 0, macptr = rstrtok(arg,";"); (macptr && count < MAX_RADIUS_SRV_NUM); macptr = rstrtok(NULL,";"), count++)
    {
        if (srv_cnt < pSecConfig->radius_srv_num)
        {
            PRADIUS_SRV_INFO pSrvInfo = &pSecConfig->radius_srv_info[srv_cnt];

            pSrvInfo->radius_port = (UINT32) simple_strtol(macptr, 0, 10);
            srv_cnt ++;
            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d), radius_port(seq-%d)=%d\n",
                apidx, srv_cnt, pSrvInfo->radius_port));
        }
    }

    return TRUE;
}

INT	Set_RADIUS_Key_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR apidx = pObj->ioctl_if;
    struct wifi_dev *wdev = NULL;
    struct _SECURITY_CONFIG *pSecConfig = NULL;
    RTMP_STRING *macptr;
    INT count;
    INT srv_cnt = 0;

    if (apidx >= pAd->ApCfg.BssidNum)
        return FALSE;

    wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
    pSecConfig = &wdev->SecConfig;

    for (count = 0, macptr = rstrtok(arg,";"); (macptr && count < MAX_RADIUS_SRV_NUM); macptr = rstrtok(NULL,";"), count++)
    {
        if (strlen(macptr) > 0 && strlen(macptr) < 65 && srv_cnt < pSecConfig->radius_srv_num)
        {
            PRADIUS_SRV_INFO pSrvInfo = &pSecConfig->radius_srv_info[srv_cnt];

            pSrvInfo->radius_key_len = strlen(macptr);
            NdisMoveMemory(pSrvInfo->radius_key, macptr, pSrvInfo->radius_key_len);
            srv_cnt ++;
            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d), radius_key(seq-%d)=%s, len=%d\n",
                apidx, srv_cnt, pSrvInfo->radius_key, pSrvInfo->radius_key_len));

        }
    }

    return TRUE;
}

INT	Set_DeletePMKID_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg) //for testing sending deauth frame if PMKID not found
{
    UCHAR apidx = simple_strtol(arg, 0, 10);
    INT32 i = 0;

    for (i = 0; i < MAX_PMKID_COUNT; i++)
    	RTMPDeletePMKIDCache(&pAd->ApCfg.PMKIDCache, apidx, i);

    return TRUE;
}
INT	Set_DumpPMKID_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	UCHAR imput = simple_strtol(arg, 0, 10);
	INT32 i = 0;
	if (imput == 1) {
		for (i = 0; i < MAX_PMKID_COUNT; i++) {
			PAP_BSSID_INFO pBssInfo = &pAd->ApCfg.PMKIDCache.BSSIDInfo[i];

			if (pBssInfo->Valid) {
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
					("Cacheidx = %d, Mbssidx = %d, Mac = %02x:%02x:%02x:%02x:%02x:%02x\n",
						i, pBssInfo->Mbssidx, PRINT_MAC(pBssInfo->MAC)));
			}
		}
	}

    return TRUE;
}
#endif /* DOT1X_SUPPORT */

#ifdef UAPSD_SUPPORT
INT Set_UAPSD_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR IdMbss = pObj->ioctl_if;

	if (simple_strtol(arg, 0, 10) != 0)
		pAd->ApCfg.MBSSID[IdMbss].wdev.UapsdInfo.bAPSDCapable = TRUE;
	else
		pAd->ApCfg.MBSSID[IdMbss].wdev.UapsdInfo.bAPSDCapable = FALSE;

	return TRUE;
} /* End of Set_UAPSD_Proc */
#endif /* UAPSD_SUPPORT */



#ifdef MCAST_RATE_SPECIFIC
INT Set_McastPhyMode(
	IN PRTMP_ADAPTER pAd,
	IN RTMP_STRING *arg)
{
	BSS_INFO_ARGUMENT_T bss_info_argument;
	HTTRANSMIT_SETTING *pTransmit;
	BOOLEAN isband5g, tmp_band;
	UCHAR cfg_ht_bw;
	struct wifi_dev *wdev = NULL;
	INT i = 0;
	BOOLEAN	status = TRUE;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR apidx = pObj->ioctl_if;

	UCHAR PhyMode = simple_strtol(arg, 0, 10);

	if ((pObj->ioctl_if_type != INT_MBSSID) && (pObj->ioctl_if_type != INT_MAIN))
	{
		printk("Do nothing! This device interface is NOT AP mode!\n");
		return FALSE;
	}
	if (apidx >= pAd->ApCfg.BssidNum)
	{
		printk("Invalid device interface!\n");
		return FALSE;	
	}

	wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
	isband5g = (wdev->channel > 14) ? TRUE : FALSE;
	if (isband5g) {
		if (PhyMode == MCAST_CCK) {
			printk("We could not set CCK mode for multicast frames in 5G band!\n");
			return FALSE;
		}
		pTransmit = &pAd->CommonCfg.MCastPhyMode_5G;
	} else {
#ifdef DOT11_VHT_AC	
		if (PhyMode == MCAST_VHT) {
			printk("We could not set VHT mode for multicast frames in 2.4G band!\n");
			return FALSE;
		}
#endif /* DOT11_VHT_AC */
		pTransmit = &pAd->CommonCfg.MCastPhyMode;
	}
 
	cfg_ht_bw = wlan_config_get_ht_bw(wdev);
	pTransmit->field.BW = cfg_ht_bw;

	switch (PhyMode)
	{
		case MCAST_DISABLE: // disable
			NdisMoveMemory(pTransmit, &wdev->rate.MlmeTransmit, sizeof(HTTRANSMIT_SETTING));
			pTransmit->field.BW =  BW_20;
			//NdisMoveMemory(&pAd->CommonCfg.MCastPhyMode, &pAd->MacTab.Content[MCAST_WCID].HTPhyMode, sizeof(HTTRANSMIT_SETTING));
			break;

		case MCAST_CCK:	/* CCK */
			pTransmit->field.MODE = MODE_CCK;
			pTransmit->field.BW =  BW_20;
			if ((pTransmit->field.MCS > 11) || (pTransmit->field.MCS > 3 && pTransmit->field.MCS < 8))
				pTransmit->field.MCS = 3;
			break;

		case MCAST_OFDM:	/* OFDM */
			pTransmit->field.MODE = MODE_OFDM;
			pTransmit->field.BW =  BW_20;
			if (pTransmit->field.MCS > 7)
				pTransmit->field.MCS = 7;
			break;
			
#ifdef DOT11_N_SUPPORT
		case MCAST_HTMIX:	/* HTMIX */
			pTransmit->field.MODE = MODE_HTMIX;
			if ((isband5g) && (pAd->CommonCfg.BBPCurrentBW > BW_20))
				pTransmit->field.BW =  BW_40;
			else	
				pTransmit->field.BW =  BW_20;
			break;
#endif /* DOT11_N_SUPPORT */

#ifdef DOT11_VHT_AC
		case MCAST_VHT: /* VHT */
			pTransmit->field.MODE = MODE_VHT;
			pTransmit->field.BW = pAd->CommonCfg.BBPCurrentBW;
			break;
#endif /* DOT11_VHT_AC */

		default:
			printk("unknown Muticast PhyMode %d.\n", PhyMode);
			printk("0:Disable, 1:CCK, 2:OFDM, 3:HTMIX, 4:VHT.\n");
			status = FALSE;
			break;
	}

	if (!status)
		return FALSE;
	
	for (i = 0; i < pAd->ApCfg.BssidNum; i++) {	
		wdev = &pAd->ApCfg.MBSSID[i].wdev;
		tmp_band = (wdev->channel > 14) ? TRUE : FALSE;
		if (tmp_band != isband5g)
			continue;

		NdisZeroMemory(&bss_info_argument, sizeof(BSS_INFO_ARGUMENT_T));
		bss_info_argument.bss_state = BSS_ACTIVE;
		bss_info_argument.ucBssIndex = wdev->bss_info_argument.ucBssIndex;
		bss_info_argument.u4BssInfoFeature = BSS_INFO_BROADCAST_INFO_FEATURE;
		memmove(&bss_info_argument.BcTransmit, pTransmit, sizeof(HTTRANSMIT_SETTING));
		memmove(&bss_info_argument.McTransmit, pTransmit, sizeof(HTTRANSMIT_SETTING));
		
		if(AsicBssInfoUpdate(pAd, bss_info_argument) != NDIS_STATUS_SUCCESS)
			printk("Fail to apply the bssinfo, BSSID=%d!\n", i);
	}
	return TRUE;
}


INT Set_McastMcs(
	IN PRTMP_ADAPTER pAd,
	IN RTMP_STRING *arg)
{
	BSS_INFO_ARGUMENT_T bss_info_argument;
	BOOLEAN isband5g, tmp_band;
	HTTRANSMIT_SETTING *pTransmit;
	struct wifi_dev *wdev = NULL;
	INT i = 0;
	BOOLEAN	status = TRUE;	
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR apidx = pObj->ioctl_if;

	UCHAR Mcs = simple_strtol(arg, 0, 10);

	if ((pObj->ioctl_if_type != INT_MBSSID) && (pObj->ioctl_if_type != INT_MAIN))
	{
		printk("Do nothing! This device interface is NOT AP mode!\n");
		return FALSE;
	}
	if (apidx >= pAd->ApCfg.BssidNum)
	{
		printk("Invalid device interface!\n");
		return FALSE;
	}
	
	if (Mcs > 15)
	{
		printk("Mcs must be in range of 0 to 15\n");
		return FALSE;
	}

	wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
	isband5g = (wdev->channel > 14) ? TRUE : FALSE;
	pTransmit = (isband5g) ? (&pAd->CommonCfg.MCastPhyMode_5G) : (&pAd->CommonCfg.MCastPhyMode);

	switch(pTransmit->field.MODE)
	{
		case MODE_CCK:
			if ((Mcs <= 3) || (Mcs >= 8 && Mcs <= 11))
			{
				pTransmit->field.MCS = Mcs;
			} else {
				printk("MCS must in range of 0 ~ 3 and 8 ~ 11 for CCK Mode.\n");
				status = FALSE;
			}
			break;

		case MODE_OFDM:
			if (Mcs > 7)
			{
				printk("MCS must in range from 0 to 7 for OFDM Mode.\n");
				status = FALSE;
			} else {
				pTransmit->field.MCS = Mcs;
			}
			break;

		default:
			pTransmit->field.MCS = Mcs;
			break;
	}

	if (!status)
		return FALSE;
	
	for (i = 0; i < pAd->ApCfg.BssidNum; i++) {	
		wdev = &pAd->ApCfg.MBSSID[i].wdev;
		tmp_band = (wdev->channel > 14) ? TRUE : FALSE;
		if (tmp_band != isband5g)
			continue;

		NdisZeroMemory(&bss_info_argument, sizeof(BSS_INFO_ARGUMENT_T));
		bss_info_argument.bss_state = BSS_ACTIVE;
		bss_info_argument.ucBssIndex = wdev->bss_info_argument.ucBssIndex;
		bss_info_argument.u4BssInfoFeature = BSS_INFO_BROADCAST_INFO_FEATURE;
		memmove(&bss_info_argument.BcTransmit, pTransmit, sizeof(HTTRANSMIT_SETTING));
		memmove(&bss_info_argument.McTransmit, pTransmit, sizeof(HTTRANSMIT_SETTING));
		if(AsicBssInfoUpdate(pAd, bss_info_argument) != NDIS_STATUS_SUCCESS)
			printk("Fail to apply the bssinfo, BSSID=%d!\n", i);
	}
	
	return TRUE;
}

INT Show_McastRate(
	IN PRTMP_ADAPTER pAd,
	IN RTMP_STRING *arg)
{
	HTTRANSMIT_SETTING *pTransmit;
	struct wifi_dev *wdev = NULL;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR apidx = pObj->ioctl_if;

	if ((pObj->ioctl_if_type != INT_MBSSID) && (pObj->ioctl_if_type != INT_MAIN))
	{
		printk("Do nothing! This device interface is NOT AP mode!\n");
		return FALSE;
	}
	if (apidx >= pAd->ApCfg.BssidNum)
	{
		printk("Invalid device interface!\n");
		return FALSE;
	}

	wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
	pTransmit = (wdev->channel > 14) ? (&pAd->CommonCfg.MCastPhyMode_5G) : (&pAd->CommonCfg.MCastPhyMode);
	
	printk("Mcast PhyMode = %d\n", pTransmit->field.MODE);
	printk("Mcast Mcs = %d\n", pTransmit->field.MCS);
	printk("Mcast BW = %d\n", pTransmit->field.BW);
	return TRUE;
}
#endif /* MCAST_RATE_SPECIFIC */

#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
INT Set_OBSSScanParam_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	INT ObssScanValue;
	UINT Idx;
	RTMP_STRING *thisChar;

	Idx = 0;
	while ((thisChar = strsep((char **)&arg, "-")) != NULL)
	{
		ObssScanValue = (INT) simple_strtol(thisChar, 0, 10);
		switch (Idx)
		{
			case 0:
				if (ObssScanValue < 5 || ObssScanValue > 1000)
				{
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanPassiveDwell(%d), should in range 5~1000\n", ObssScanValue));
				}
				else
				{
					pAd->CommonCfg.Dot11OBssScanPassiveDwell = ObssScanValue;	/* Unit : TU. 5~1000 */
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OBSSScanParam for Dot11OBssScanPassiveDwell=%d\n", ObssScanValue));
				}
				break;
			case 1:
				if (ObssScanValue < 10 || ObssScanValue > 1000)
				{
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanActiveDwell(%d), should in range 10~1000\n", ObssScanValue));
				}
				else
				{
					pAd->CommonCfg.Dot11OBssScanActiveDwell = ObssScanValue;	/* Unit : TU. 10~1000 */
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OBSSScanParam for Dot11OBssScanActiveDwell=%d\n", ObssScanValue));
				}
				break;
			case 2:
				pAd->CommonCfg.Dot11BssWidthTriggerScanInt = ObssScanValue;	/* Unit : Second */
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OBSSScanParam for Dot11BssWidthTriggerScanInt=%d\n", ObssScanValue));
				break;
			case 3:
				if (ObssScanValue < 200 || ObssScanValue > 10000)
				{
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanPassiveTotalPerChannel(%d), should in range 200~10000\n", ObssScanValue));
				}
				else
				{
					pAd->CommonCfg.Dot11OBssScanPassiveTotalPerChannel = ObssScanValue;	/* Unit : TU. 200~10000 */
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OBSSScanParam for Dot11OBssScanPassiveTotalPerChannel=%d\n", ObssScanValue));
				}
				break;
			case 4:
				if (ObssScanValue < 20 || ObssScanValue > 10000)
				{
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanActiveTotalPerChannel(%d), should in range 20~10000\n", ObssScanValue));
				}
				else
				{
					pAd->CommonCfg.Dot11OBssScanActiveTotalPerChannel = ObssScanValue;	/* Unit : TU. 20~10000 */
					MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OBSSScanParam for Dot11OBssScanActiveTotalPerChannel=%d\n", ObssScanValue));
				}
				break;
			case 5:
				pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor = ObssScanValue;
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OBSSScanParam for Dot11BssWidthChanTranDelayFactor=%d\n", ObssScanValue));
				break;
			case 6:
				pAd->CommonCfg.Dot11OBssScanActivityThre = ObssScanValue;	/* Unit : percentage */
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OBSSScanParam for Dot11BssWidthChanTranDelayFactor=%d\n", ObssScanValue));
				break;
		}
		Idx++;
	}

	if (Idx != 7)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Wrong OBSSScanParamtetrs format in ioctl cmd!!!!! Use default value\n"));

		pAd->CommonCfg.Dot11OBssScanPassiveDwell = dot11OBSSScanPassiveDwell;	/* Unit : TU. 5~1000 */
		pAd->CommonCfg.Dot11OBssScanActiveDwell = dot11OBSSScanActiveDwell;	/* Unit : TU. 10~1000 */
		pAd->CommonCfg.Dot11BssWidthTriggerScanInt = dot11BSSWidthTriggerScanInterval;	/* Unit : Second */
		pAd->CommonCfg.Dot11OBssScanPassiveTotalPerChannel = dot11OBSSScanPassiveTotalPerChannel;	/* Unit : TU. 200~10000 */
		pAd->CommonCfg.Dot11OBssScanActiveTotalPerChannel = dot11OBSSScanActiveTotalPerChannel;	/* Unit : TU. 20~10000 */
		pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor = dot11BSSWidthChannelTransactionDelayFactor;
		pAd->CommonCfg.Dot11OBssScanActivityThre = dot11BSSScanActivityThreshold;	/* Unit : percentage */
	}
	pAd->CommonCfg.Dot11BssWidthChanTranDelay = (pAd->CommonCfg.Dot11BssWidthTriggerScanInt * pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor);
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("OBSSScanParam for Dot11BssWidthChanTranDelay=%ld\n", pAd->CommonCfg.Dot11BssWidthChanTranDelay));

	return TRUE;
}

static VOID SetAp2040ReScanByRf(RTMP_ADAPTER *pAd,UCHAR RfIC)
{
	UCHAR Channel = HcGetChannelByRf(pAd,RfIC);
	AsicBBPAdjust(pAd,Channel);
	AsicSwitchChannel(pAd, pAd->CommonCfg.CentralChannel, FALSE);
	AsicLockChannel(pAd, pAd->CommonCfg.CentralChannel);
}


INT	Set_AP2040ReScan_Proc(
	IN	PRTMP_ADAPTER pAd,
	IN	RTMP_STRING *arg)
{

	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	struct wifi_dev *wdev;
	wdev = &pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev;

	APOverlappingBSSScan(pAd,wdev);

	/* apply setting */
	SetCommonHtVht(pAd,wdev);

	SetAp2040ReScanByRf(pAd,RFIC_24GHZ);
	SetAp2040ReScanByRf(pAd,RFIC_5GHZ);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set_AP2040ReScan_Proc() Trigger AP ReScan !!!\n"));

	return TRUE;
}
#endif /* DOT11N_DRAFT3 */
#endif /* DOT11_N_SUPPORT */

INT Set_EntryLifeCheck_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	ULONG LifeCheckCnt = (ULONG) simple_strtol(arg, 0, 10);

	if (LifeCheckCnt <= 65535)
		pAd->ApCfg.EntryLifeCheck = LifeCheckCnt;
	else
		printk("LifeCheckCnt must in range of 0 to 65535\n");

	printk("EntryLifeCheck Cnt = %ld.\n", pAd->ApCfg.EntryLifeCheck);
	return TRUE;
}

INT	ApCfg_Set_PerMbssMaxStaNum_Proc(
	IN PRTMP_ADAPTER 	pAd,
	IN INT				apidx,
	IN RTMP_STRING *arg)
{
	pAd->ApCfg.MBSSID[apidx].MaxStaNum = (UCHAR)simple_strtol(arg, 0, 10);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IF(ra%d) %s::(MaxStaNum=%d)\n",
					apidx, __FUNCTION__, pAd->ApCfg.MBSSID[apidx].MaxStaNum));
	return TRUE;
}

INT	ApCfg_Set_IdleTimeout_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	LONG idle_time;

	idle_time = simple_strtol(arg, 0, 10);

	if (idle_time < MAC_TABLE_MIN_AGEOUT_TIME)
		pAd->ApCfg.StaIdleTimeout = MAC_TABLE_MIN_AGEOUT_TIME;
	else
		pAd->ApCfg.StaIdleTimeout = idle_time;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("%s : IdleTimeout=%d\n", __FUNCTION__, pAd->ApCfg.StaIdleTimeout));

	return TRUE;
}






INT	Set_MemDebug_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
#ifdef VENDOR_FEATURE2_SUPPORT
	printk("Number of Packet Allocated = %lu\n", OS_NumOfPktAlloc);
	printk("Number of Packet Freed = %lu\n", OS_NumOfPktFree);
	printk("Offset of Packet Allocated/Freed = %lu\n", OS_NumOfPktAlloc - OS_NumOfPktFree);
#endif /* VENDOR_FEATURE2_SUPPORT */
	return TRUE;
}


#ifdef APCLI_SUPPORT
#endif/*APCLI_SUPPORT*/

#ifdef CONFIG_AP_SUPPORT
/*
========================================================================
Routine Description:
	Set power save life time.

Arguments:
	pAd					- WLAN control block pointer
	Arg					- Input arguments

Return Value:
	None

Note:
========================================================================
*/
INT	Set_PowerSaveLifeTime_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	pAd->MacTab.MsduLifeTime = simple_strtol(arg, 0, 10);
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set new life time = %d\n", pAd->MacTab.MsduLifeTime));
	return TRUE;
}
#endif /* CONFIG_AP_SUPPORT */


#ifdef MBSS_SUPPORT
/*
========================================================================
Routine Description:
	Show MBSS information.

Arguments:
	pAd					- WLAN control block pointer
	Arg					- Input arguments

Return Value:
	None

Note:
========================================================================
*/
extern UCHAR *wmode_2_str(UCHAR wmode);


INT	Show_MbssInfo_Display_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	UINT32 IdBss;
	UCHAR PhyMode;
	CHAR *mod_str = NULL;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("\n\tBSS Idx\t\tPhy Mode\n"));


	PhyMode = HcGetPhyModeByRf(pAd,RFIC_24GHZ);
	if(PhyMode!=0)
	{
		mod_str = wmode_2_str(PhyMode);
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("\t2.4G Band MAX\t%s\n",mod_str));
	}

	PhyMode = HcGetPhyModeByRf(pAd,RFIC_5GHZ);
	if(PhyMode!=0)
	{
		mod_str = wmode_2_str(PhyMode);
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("\t2.4G Band MAX\t%s\n",mod_str));
	}

	if (mod_str)
                os_free_mem(mod_str);


	for(IdBss=0; IdBss< pAd->ApCfg.BssidNum; IdBss++)
	{

		PhyMode = pAd->ApCfg.MBSSID[IdBss].wdev.PhyMode;
		mod_str = wmode_2_str(PhyMode);
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("\t%d\t\t%s\n", IdBss,mod_str));
                if (mod_str)
                    os_free_mem(mod_str);
	}

	return TRUE;
} /* End of Show_MbssInfo_Display_Proc */
#endif /* MBSS_SUPPORT */


#ifdef HOSTAPD_SUPPORT
VOID RtmpHostapdSecuritySet(
	IN	RTMP_ADAPTER			*pAd,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrqin)
{
	if(wrqin->u.data.length > 20 && MAX_LEN_OF_RSNIE > wrqin->u.data.length && wrqin->u.data.pointer)
	{
		UCHAR RSNIE_Len[2];
		UCHAR RSNIe[2];
		int offset_next_ie=0;

		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("ioctl SIOCSIWGENIE pAd->IoctlIF=%d\n",apidx));

		RSNIe[0]=*(UINT8 *)wrqin->u.data.pointer;
		if(IE_WPA != RSNIe[0] && IE_RSN != RSNIe[0] )
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("IE %02x != 0x30/0xdd\n",RSNIe[0]));
			Status = -EINVAL;
			break;
		}
		RSNIE_Len[0]=*((UINT8 *)wrqin->u.data.pointer + 1);
		if(wrqin->u.data.length != RSNIE_Len[0]+2)
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("IE use WPA1 WPA2\n"));
			NdisZeroMemory(pAd->ApCfg.MBSSID[apidx].RSN_IE[1], MAX_LEN_OF_RSNIE);
			RSNIe[1]=*(UINT8 *)wrqin->u.data.pointer;
			RSNIE_Len[1]=*((UINT8 *)wrqin->u.data.pointer + 1);
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,( "IE1 %02x %02x\n",RSNIe[1],RSNIE_Len[1]));
			pAd->ApCfg.MBSSID[apidx].RSNIE_Len[1] = RSNIE_Len[1];
			NdisMoveMemory(pAd->ApCfg.MBSSID[apidx].RSN_IE[1], (UCHAR *)(wrqin->u.data.pointer)+2, RSNIE_Len[1]);
			offset_next_ie=RSNIE_Len[1]+2;
		}
		else
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,("IE use only %02x\n",RSNIe[0]));

		NdisZeroMemory(pAd->ApCfg.MBSSID[apidx].RSN_IE[0], MAX_LEN_OF_RSNIE);
		RSNIe[0]=*(((UINT8 *)wrqin->u.data.pointer)+offset_next_ie);
		RSNIE_Len[0]=*(((UINT8 *)wrqin->u.data.pointer) + offset_next_ie + 1);
		if(IE_WPA != RSNIe[0] && IE_RSN != RSNIe[0] )
		{
			Status = -EINVAL;
			break;
		}
		pAd->ApCfg.MBSSID[apidx].RSNIE_Len[0] = RSNIE_Len[0];
		NdisMoveMemory(pAd->ApCfg.MBSSID[apidx].RSN_IE[0], ((UCHAR *)(wrqin->u.data.pointer))+2+offset_next_ie, RSNIE_Len[0]);

        UpdateBeaconHandler(
        pAd,
        &pAd->ApCfg.MBSSID[apidx].wdev,
        INTERFACE_STATE_CHANGE);
	}
}
#endif /* HOSTAPD_SUPPORT */

#ifdef MT_MAC
/* Only for TimReq frame generating */
//TODO: Tim service establish.
/* used to mapping, tttt hwidx and  related CR */
static TTTT_CR_BSSID_IDX_MAP_T g_arTtttCrHwBssidMapTable[] =
{
    {HW_BSSID_0,    LPON_TT0TPCR,    LPON_TT4TPCR},
    {HW_BSSID_1,    LPON_TT1TPCR,    LPON_TT5TPCR},
    {HW_BSSID_2,    LPON_TT2TPCR,    LPON_TT6TPCR},
    {HW_BSSID_3,    LPON_TT2TPCR,    LPON_TT7TPCR},
};

INT	Set_AP_TimEnable_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	UINT32 interval = 0;
	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie;
	UCHAR APIndex = pObj->ioctl_if;
	BSS_STRUCT *pMbss = NULL;
	UINT32 Value = 0;
    struct wifi_dev *wdev = NULL;
    UINT8 OmacIdx = 0;
    UINT8 BssidIdxMapped = 0;
    PTTTT_CR_BSSID_IDX_MAP_T    pTtttCrMapTable = NULL;
    UINT32 u4TtttEnCr = 0, u4TtttIntervelCr = 0;
    BOOLEAN     fgEnable = FALSE;

	interval = simple_strtol(arg, 0, 10);

	/* if ((interval < 0) || (interval > 255)) {   // interval is UINT, no need to check <0 */
	if (interval > 255) {
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: interval is incorrect!!\n", __FUNCTION__));
		return FALSE;
	}

	pMbss = &pAd->ApCfg.MBSSID[APIndex];

    wdev = &pMbss->wdev;
    if (wdev != NULL)
    {
        OmacIdx = wdev->OmacIdx;
        wdev_tim_buf_init(pAd, &wdev->bcn_buf.tim_buf);

        /* ext mbss tttt, depends on MBSS_0 */
        if (OmacIdx > HW_BSSID_MAX)
            BssidIdxMapped = HW_BSSID_0;
        else
            BssidIdxMapped = OmacIdx;

        MTWF_LOG(DBG_CAT_CFG,
                DBG_SUBCAT_ALL,
                DBG_LVL_ERROR,
                ("%s: BssidIdxMapped = %d, OmacIdx = %x\n",
                    __FUNCTION__,
                    BssidIdxMapped,
                    OmacIdx));
    }
    else
    {
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: idx = %d, no wdev!?\n", __FUNCTION__, APIndex));
        return FALSE;
    }

    pTtttCrMapTable = &g_arTtttCrHwBssidMapTable[BssidIdxMapped];
    u4TtttEnCr = pTtttCrMapTable->u4TTTTEnableCr;
    u4TtttIntervelCr = pTtttCrMapTable->u4TTTTIntervalCr;

	if (interval > 0) //valid interval --> enable
	{
        fgEnable = TRUE;
        MAC_IO_WRITE32(pAd, LPON_PTTISR, IDX_DEFAULT_PRETTTT_INTERVAL(BssidIdxMapped));

		MAC_IO_READ32(pAd, u4TtttEnCr, &Value);
		Value &= ~TTTTn_OFFSET_OF_TBTTn_MASK;
		Value |= DEFAULT_TTTT_OFFSET_IN_MS;
		Value |= TTTTn_CAL_EN;
		MAC_IO_WRITE32(pAd, u4TtttEnCr, Value);

        printk("u4TtttEnCr = %x, Value = %x\n", u4TtttEnCr, Value);

		MAC_IO_WRITE32(pAd, u4TtttIntervelCr, SET_TTTT_PERIOD(pAd, interval));
        printk("u4TtttIntervelCr = %x, Value = %x\n", u4TtttIntervelCr, SET_TTTT_PERIOD(pAd, interval));

		MAC_IO_READ32(pAd, LPON_MPTCR4, &Value);
		Value |= TTTT0_PERIODTIMER_EN |
                TTTT0_TIMEUP_EN |
                PRETTTT0TIMEUP_EN |
                PRETTTT0_TRIG_EN |
                PRETTTT0INT_EN;
		MAC_IO_WRITE32(pAd, LPON_MPTCR4, Value);

		MAC_IO_READ32(pAd, AGG_ARCR, &Value);
		Value = Value | DUAL_BTIM_EN;
		MAC_IO_WRITE32(pAd, AGG_ARCR, Value);
        Value = 0 | AGG_BTIMRR_TX_MODE(Legacy_CCK) | AGG_BTIMRR_TX_RATE(LONG_PREAMBLE_1M);
		MAC_IO_WRITE32(pAd, AGG_BTIMRR0, Value);
        /*TODO: other omac idx rate setting.*/

        MAC_IO_READ32(pAd, ARB_BTIMCR0, &Value);
        Value |= (1 << OmacIdx);
		MAC_IO_WRITE32(pAd, ARB_BTIMCR0, Value);//enable BTIM_EN bit in ARB_BTIMCR0
		MAC_IO_WRITE32(pAd, ARB_BTIMCR1, Value);//enable 2 TIM broadcast

#ifndef BCN_OFFLOAD_SUPPORT
		MAC_IO_WRITE32(pAd, HWIER4, 0x80008000);//enable TTTT0/PRETTTT0 interrupt.
#endif
	}
	else //interval 0 --> disable
	{
        MAC_IO_READ32(pAd, ARB_BTIMCR0, &Value);
        Value &= ~(1 << OmacIdx);
        MAC_IO_WRITE32(pAd, ARB_BTIMCR0, Value);
        MAC_IO_WRITE32(pAd, ARB_BTIMCR1, Value);
	}

    AsicSetExtTTTTHwCRSetting(pAd, APIndex, fgEnable);

	return TRUE;
}

INT 
Show_TmrCalResult_Proc(
	IN RTMP_ADAPTER *pAd, 
	IN RTMP_STRING *arg
	)
{
	if (pAd->pTmrCtrlStruct)
	    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
			("%s(): TmrCalResult=0x%X\n", __FUNCTION__, pAd->pTmrCtrlStruct->TmrCalResult));
	else
	    MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
			("%s(): (X) pTmrCtrlStruct is NULL\n", __FUNCTION__));

	return TRUE;
}

#ifdef DBG
INT set_tim_update_proc(
    IN      PRTMP_ADAPTER   pAd,
    IN      RTMP_STRING     *arg)
{
    APMakeAllTimFrame(pAd);
    return TRUE;
}

INT Set_AP_DumpTime_Proc(
    IN      PRTMP_ADAPTER   pAd,
    IN      RTMP_STRING     *arg)
{
    int apidx = 0, i = 0;
    BSS_STRUCT *pMbss;

    printk("\n\t%-10s\t%-10s\n", "PreTBTTTime", "TBTTTime");
    printk("%-10lu\t%-10lu\n", pAd->HandlePreInterruptTime, pAd->HandleInterruptTime);

    for (apidx = 0; apidx < pAd->ApCfg.BssidNum; apidx++) {
        pMbss = &pAd->ApCfg.MBSSID[apidx];

        if(!BeaconTransmitRequired(pAd, &pMbss->wdev))
            continue;

        printk("\n%s:%d\tBcn_State:%d\t%-10s: %d\n", "Apidx", apidx, pMbss->wdev.bcn_buf.bcn_state, "recover", pMbss->bcn_recovery_num);
        printk("\t%-10s\t%-10s\t%-10s\t%-10s\n", "WriteBcnRing", "BcnDmaDone", "TXS_TSF", "TXS_SN");
        for (i = 0; i < MAX_TIME_RECORD; i++) {
            printk("Idx[%d]:\t%-10lu\t%-10lu\t%-10lu\t%-10lu\n", i, pMbss->WriteBcnDoneTime[i], pMbss->BcnDmaDoneTime[i], pMbss->TXS_TSF[i], pMbss->TXS_SN[i]);
        }
    }

    return TRUE;
}

INT Set_BcnStateCtrl_Proc(
    IN  PRTMP_ADAPTER   pAd,
    IN  RTMP_STRING *arg)
{
    UCHAR bcn_state;
    POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie;
    UCHAR apIndex = pObj->ioctl_if;
    BSS_STRUCT *pMbss = NULL;

    bcn_state = simple_strtol(arg, 0, 10);

    if (pAd->chipCap.hif_type != HIF_MT) {
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: chipCap is not HIF_MT\n", __FUNCTION__));
        return FALSE;
    }

    if ((bcn_state < BCN_TX_IDLE) || (bcn_state > BCN_TX_DMA_DONE)) {
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: bcn_state is incorrect!!\n", __FUNCTION__));
        return FALSE;
    }

    pMbss = &pAd->ApCfg.MBSSID[apIndex];

    ASSERT(pMbss);
    if (pMbss == NULL)
    {
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: pMbss == NULL!!\n", __FUNCTION__));
        return FALSE;
    }

    if (pMbss->wdev.bcn_buf.bcn_state != BCN_TX_IDLE) {
        printk("MBSSID[%d], bcn_buf.bcn_state = %d, will change to %d\n", apIndex, pMbss->wdev.bcn_buf.bcn_state, bcn_state);
        RTMP_SEM_LOCK(&pAd->BcnRingLock);
        pMbss->wdev.bcn_buf.bcn_state = bcn_state;
        RTMP_SEM_UNLOCK(&pAd->BcnRingLock);
    }

        return TRUE;
}

INT    set_dump_rx_ba_scoreboard_proc(
       IN      PRTMP_ADAPTER   pAd,
       IN      RTMP_STRING     *arg)
{
    UCHAR i;
    struct wifi_dev *wdev = NULL;
    RX_TRACKING_T *pTracking = NULL;
    RX_TA_TID_SEQ_MAPPING *pTaTidSeqMapEntry = NULL;

    for (i = 0; i < WDEV_NUM_MAX; i++)
    {
        wdev = pAd->wdev_list[i];
        if (wdev == NULL)
            continue;

        pTracking = &wdev->rx_tracking;
        pTaTidSeqMapEntry = &pTracking->LastRxWlanIdx;

        if (pTaTidSeqMapEntry->RxDWlanIdx != 0xff)
        {
            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR,
                ("wdev idx:%d, wdev_type:%x ,TriggerNum = %d\n",
                    wdev->wdev_idx, wdev->wdev_type, pTracking->TriggerNum));
        }
    }

    return TRUE;
}

INT    set_dump_sta_rec_history_proc(
       IN      PRTMP_ADAPTER   pAd,
       IN      RTMP_STRING     *arg)
{
    STA_REC_TRACKING *pTracking = &pAd->StaRecTracking;
    STA_REC_CFG_T *pStaCfg = NULL;
    int i, max = 0;

    if (pTracking->StaRecNum >= MAX_STA_REC_HISTORY_RECORD)
        max = MAX_STA_REC_HISTORY_RECORD;
    else
        max = pTracking->StaRecNum;

    printk("StaRecHistory: total send %d times\n", (UINT32)pTracking->StaRecNum);
    printk("BssIdx:\t\tWlanIdx:\tCnctStat:\tMuarIdx:\tCnctType\tFeature:\n");
    for (i = 0; i < max; i++)
    {
        pStaCfg = &pTracking->sta_rec_cfg_record[i];
        printk("%8d\t%8d\t%8d\t%8d\t%8d\t%8x\n",
                                        pStaCfg->ucBssIndex,
                                        pStaCfg->ucWlanIdx,
                                        pStaCfg->ConnectionState,
                                        pStaCfg->MuarIdx,
                                        pStaCfg->ConnectionType,
                                        pStaCfg->u4EnableFeature);
    }

    return TRUE;
}

#endif

INT setApTmrEnableProc(
    IN  PRTMP_ADAPTER   pAd,
    IN  RTMP_STRING *arg)
{
    LONG enable;
    POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie;
    UCHAR apIndex = pObj->ioctl_if;
    BSS_STRUCT *pMbss = NULL;
    UINT32  value = 0;

    enable = simple_strtol(arg, 0, 10);

    if (pAd->chipCap.hif_type != HIF_MT) {
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: chipCap is not HIF_MT\n", __FUNCTION__));
        return FALSE;
    }

    if ((enable < 0) || (enable > 2)) {
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: enable is incorrect!!\n", __FUNCTION__));
        return FALSE;
    }

    pMbss = &pAd->ApCfg.MBSSID[apIndex];

    ASSERT(pMbss);
    if (pMbss == NULL)
    {
        MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: pMbss == NULL!!\n", __FUNCTION__));
        return FALSE;
    }

    switch (enable)
    {
        case TMR_INITIATOR:
        {
            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s: enable TMR report, as Initialiter\n", __FUNCTION__));
            RTMP_IO_READ32(pAd, RMAC_TMR_PA, &value);
            value = value | BIT31;
            value = value & ~BIT30;
            RTMP_IO_WRITE32(pAd, RMAC_TMR_PA, value);
            pAd->chipCap.TmrEnable = 1;
	    MtCmdTmrCal(pAd,
			enable,
			(pMbss->wdev.channel > 14 ? _A_BAND : _G_BAND),
			pAd->CommonCfg.BBPCurrentBW,
			0,//Ant 0 at present
			TMR_INITIATOR);
        }
            break;
        case TMR_RESPONDER:
        {
            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s: enable TMR report, as Responser\n", __FUNCTION__));
            RTMP_IO_READ32(pAd, RMAC_TMR_PA, &value);
            value = value | BIT31;
            value = value | BIT30;
            value = value | 0x34;//Action frame register
            RTMP_IO_WRITE32(pAd, RMAC_TMR_PA, value);
            pAd->chipCap.TmrEnable = 2;
	    MtCmdTmrCal(pAd,
			enable,
			(pMbss->wdev.channel > 14 ? _A_BAND : _G_BAND),
			pAd->CommonCfg.BBPCurrentBW,
			0,//Ant 0 at present
			TMR_RESPONDER);
        }
            break;

        case TMR_DISABLE:
	default:
        {
            MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s: disable TMR report\n", __FUNCTION__));
            RTMP_IO_READ32(pAd, RMAC_TMR_PA, &value);
            value = value & ~BIT31;
            RTMP_IO_WRITE32(pAd, RMAC_TMR_PA, value);
            pAd->chipCap.TmrEnable = FALSE;
	    MtCmdTmrCal(pAd,
			enable,
			(pMbss->wdev.channel > 14 ? _A_BAND : _G_BAND),
			pAd->CommonCfg.BBPCurrentBW,
			0,//Ant 0 at present
			TMR_DISABLE);
        }
    }
	return TRUE;
}


#ifdef CUT_THROUGH
INT Set_CutThrough_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg)
{
	UINT mode;

	mode = simple_strtol(arg, 0, 10);
    cut_through_set_mode(pAd->PktTokenCb, mode);

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF,
			("%s():Set CutThroughType as %d\n",
			__FUNCTION__, cut_through_get_mode(pAd->PktTokenCb)));
    return TRUE;
}
#endif /* CUT_THROUGH */
#endif /* MT_MAC */


/*
========================================================================
Routine Description:
	Driver Ioctl for AP.

Arguments:
	pAdSrc			- WLAN control block pointer
	wrq				- the IOCTL parameters
	cmd				- the command ID
	subcmd			- the sub-command ID
	pData			- the IOCTL private data pointer
	Data			- the IOCTL private data

Return Value:
	NDIS_STATUS_SUCCESS	- IOCTL OK
	Otherwise			- IOCTL fail

Note:
========================================================================
*/
INT RTMP_AP_IoctlHandle(
	IN	VOID					*pAdSrc,
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq,
	IN	INT						cmd,
	IN	USHORT					subcmd,
	IN	VOID					*pData,
	IN	ULONG					Data)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie;
	INT Status = NDIS_STATUS_SUCCESS;


	switch(cmd)
	{
		case CMD_RTPRIV_IOCTL_SET:
			Status = RTMPAPPrivIoctlSet(pAd, wrq);
			break;

		case CMD_RT_PRIV_IOCTL:
			if (subcmd & OID_GET_SET_TOGGLE)
				Status = RTMPAPSetInformation(pAd, wrq,  (INT)subcmd);
			else
			{
#ifdef LLTD_SUPPORT
				if (subcmd == RT_OID_GET_PHY_MODE)
				{
					if(pData != NULL)
					{
						UINT modetmp = 0;
						MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Query::Get phy mode (%02X) \n", pAd->ApCfg.MBSSID[MAIN_MBSSID].wdev.PhyMode));
						modetmp = (UINT) pAd->ApCfg.MBSSID[MAIN_MBSSID].wdev.PhyMode;
						wrq->u.data.length = 1;
						/**(ULONG *)pData = (ULONG)pAd->ApCfg.MBSS[MAIN_MBSSID].wdev.PhyMode; */
						if (copy_to_user(pData, &modetmp, wrq->u.data.length))
							Status = -EFAULT;
					}
					else
						Status = -EFAULT;
				}
				else
#endif /* LLTD_SUPPORT */
					Status = RTMPAPQueryInformation(pAd, wrq, (INT)subcmd);
			}
			break;

		case CMD_RTPRIV_IOCTL_SHOW:
			Status = RTMPAPPrivIoctlShow(pAd, wrq);
			break;

#ifdef WSC_AP_SUPPORT
		case CMD_RTPRIV_IOCTL_SET_WSCOOB:
			RTMPIoctlSetWSCOOB(pAd);
		    break;
#endif/*WSC_AP_SUPPORT*/

		case CMD_RTPRIV_IOCTL_GET_MAC_TABLE:
			RTMPIoctlGetMacTable(pAd,wrq);
		    break;

#if defined (AP_SCAN_SUPPORT) || defined (CONFIG_STA_SUPPORT)
		case CMD_RTPRIV_IOCTL_GSITESURVEY:
			RTMPIoctlGetSiteSurvey(pAd,wrq);
			break;
#endif /* AP_SCAN_SUPPORT */

		case CMD_RTPRIV_IOCTL_STATISTICS:
			RTMPIoctlStatistics(pAd, wrq);
			break;

		case CMD_RTPRIV_IOCTL_RX_STATISTICS:
			Status = RTMPIoctlRXStatistics(pAd, wrq);
			break;
            
#ifdef WSC_AP_SUPPORT
		case CMD_RTPRIV_IOCTL_WSC_PROFILE:
		    RTMPIoctlWscProfile(pAd, wrq);
		    break;
#endif /* WSC_AP_SUPPORT */

#ifdef DOT11_N_SUPPORT
		case CMD_RTPRIV_IOCTL_QUERY_BATABLE:
		    RTMPIoctlQueryBaTable(pAd, wrq);
		    break;
#endif /* DOT11_N_SUPPORT */

		case CMD_RTPRIV_IOCTL_E2P:
			RTMPAPIoctlE2PROM(pAd, wrq);
			break;

#ifdef DBG
		case CMD_RTPRIV_IOCTL_BBP:
			RTMPAPIoctlBBP(pAd, wrq);
			break;

		case CMD_RTPRIV_IOCTL_MAC:
			RTMPIoctlMAC(pAd, wrq);
			break;

#ifdef RTMP_RF_RW_SUPPORT
		case CMD_RTPRIV_IOCTL_RF:
			RTMPAPIoctlRF(pAd, wrq);
			break;
#endif /* RTMP_RF_RW_SUPPORT */
#endif /* DBG */

		case CMD_RTPRIV_IOCTL_GET_MAC_TABLE_STRUCT:
			RTMPIoctlGetMacTableStaInfo(pAd, wrq);
			break;

		case CMD_RTPRIV_IOCTL_AP_SIOCGIFHWADDR:
			if (pObj->ioctl_if < MAX_MBSSID_NUM(pAd))
				NdisCopyMemory((RTMP_STRING *) wrq->u.name, (RTMP_STRING *) pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev.bssid, 6);
			break;

		case CMD_RTPRIV_IOCTL_AP_SIOCGIWESSID:
		{
			RT_CMD_AP_IOCTL_SSID *pSSID = (RT_CMD_AP_IOCTL_SSID *)pData;

#ifdef APCLI_SUPPORT
			if (pSSID->priv_flags == INT_APCLI)
			{
				if (pAd->ApCfg.ApCliTab[pObj->ioctl_if].Valid == TRUE)
				{
					pSSID->length = pAd->ApCfg.ApCliTab[pObj->ioctl_if].SsidLen;
					pSSID->pSsidStr = (char *)&pAd->ApCfg.ApCliTab[pObj->ioctl_if].Ssid;
				}
				else {
					pSSID->length = 0;
					pSSID->pSsidStr = NULL;
				}
			}
			else
#endif /* APCLI_SUPPORT */
			{
				pSSID->length = pAd->ApCfg.MBSSID[pSSID->apidx].SsidLen;
				pSSID->pSsidStr = (char *)pAd->ApCfg.MBSSID[pSSID->apidx].Ssid;
			}
		}
			break;

#ifdef MBSS_SUPPORT
		case CMD_RTPRIV_IOCTL_MBSS_BEACON_UPDATE:
            //Carter, TODO. check this oid.
            UpdateBeaconHandler(
                pAd,
                NULL,
                PRETBTT_UPDATE);
			break;

		case CMD_RTPRIV_IOCTL_MBSS_OPEN:
			if (MBSS_Open(pData) != 0)
				return NDIS_STATUS_FAILURE;
			break;

		case CMD_RTPRIV_IOCTL_MBSS_CLOSE:
			MBSS_Close(pData);
			break;

		case CMD_RTPRIV_IOCTL_MBSS_INIT:
			MBSS_Init(pAd, pData);
			break;

		case CMD_RTPRIV_IOCTL_MBSS_REMOVE:
			MBSS_Remove(pAd);
			break;

#ifdef MT_MAC
        case CMD_RTPRIV_IOCTL_MBSS_CR_ENABLE:
			if (ext_mbss_hw_cr_enable(pData) != 0)
				return NDIS_STATUS_FAILURE;
			break;

        case CMD_RTPRIV_IOCTL_MBSS_CR_DISABLE:
			if (ext_mbss_hw_cr_disable(pData) != 0)
				return NDIS_STATUS_FAILURE;
			break;
#endif /* MT_MAC */
#endif /* MBSS_SUPPORT */

		case CMD_RTPRIV_IOCTL_WSC_INIT:
		{
#ifdef APCLI_SUPPORT
#ifdef WSC_AP_SUPPORT
#ifdef WSC_V2_SUPPORT
			PWSC_V2_INFO	pWscV2Info;
#endif /* WSC_V2_SUPPORT */
			APCLI_STRUCT *pApCliEntry = (APCLI_STRUCT *)pData;
			WscGenerateUUID(pAd, &pApCliEntry->WscControl.Wsc_Uuid_E[0],
						&pApCliEntry->WscControl.Wsc_Uuid_Str[0], 0, FALSE, TRUE);
			pApCliEntry->WscControl.bWscFragment = FALSE;
			pApCliEntry->WscControl.WscFragSize = 128;
			pApCliEntry->WscControl.WscRxBufLen = 0;
			pApCliEntry->WscControl.pWscRxBuf = NULL;
			os_alloc_mem(pAd, &pApCliEntry->WscControl.pWscRxBuf, MGMT_DMA_BUFFER_SIZE);
			if (pApCliEntry->WscControl.pWscRxBuf)
				NdisZeroMemory(pApCliEntry->WscControl.pWscRxBuf, MGMT_DMA_BUFFER_SIZE);
			pApCliEntry->WscControl.WscTxBufLen = 0;
			pApCliEntry->WscControl.pWscTxBuf = NULL;
			os_alloc_mem(pAd, &pApCliEntry->WscControl.pWscTxBuf, MGMT_DMA_BUFFER_SIZE);
			if (pApCliEntry->WscControl.pWscTxBuf)
				NdisZeroMemory(pApCliEntry->WscControl.pWscTxBuf, MGMT_DMA_BUFFER_SIZE);
			initList(&pApCliEntry->WscControl.WscPeerList);
			NdisAllocateSpinLock(pAd, &pApCliEntry->WscControl.WscPeerListSemLock);
			pApCliEntry->WscControl.PinAttackCount = 0;
			pApCliEntry->WscControl.bSetupLock = FALSE;
#ifdef WSC_V2_SUPPORT
			pWscV2Info = &pApCliEntry->WscControl.WscV2Info;
			pWscV2Info->bWpsEnable = TRUE;
			pWscV2Info->ExtraTlv.TlvLen = 0;
			pWscV2Info->ExtraTlv.TlvTag = 0;
			pWscV2Info->ExtraTlv.pTlvData = NULL;
			pWscV2Info->ExtraTlv.TlvType = TLV_ASCII;
			pWscV2Info->bEnableWpsV2 = TRUE;
#endif /* WSC_V2_SUPPORT */
			WscInit(pAd, TRUE, Data);
#endif /* WSC_AP_SUPPORT */
#endif /* APCLI_SUPPORT */
		}
			break;

#ifdef APCLI_SUPPORT
		case CMD_RTPRIV_IOCTL_APC_UP:
			ApCliIfUp(pAd);
			break;

		case CMD_RTPRIV_IOCTL_APC_DISCONNECT:
			MlmeEnqueue(pAd, APCLI_CTRL_STATE_MACHINE, APCLI_CTRL_DISCONNECT_REQ, 0, NULL, Data);
			RTMP_MLME_HANDLER(pAd);
			break;

		case CMD_RTPRIV_IOCTL_APC_INIT:
			APCli_Init(pAd, pData);
			break;

		case CMD_RTPRIV_IOCTL_APC_OPEN:
			if (ApCli_Open(pAd, pData) != TRUE)
				return NDIS_STATUS_FAILURE;
			break;

		case CMD_RTPRIV_IOCTL_APC_CLOSE:
			if (ApCli_Close(pAd, pData) != TRUE)
				return NDIS_STATUS_FAILURE;
			break;

		case CMD_RTPRIV_IOCTL_APC_REMOVE:
			ApCli_Remove(pAd);
			break;
#endif /* APCLI_SUPPORT */

		case CMD_RTPRIV_IOCTL_MAIN_OPEN:
        {
            struct wifi_dev *wdev = &pAd->ApCfg.MBSSID[MAIN_MBSSID].wdev;
			wdev->bAllowBeaconing = TRUE;

			if (VIRTUAL_IF_NUM(pAd) != 0)
			{
				WifiSysOpen(pAd, wdev);
				pAd->ApCfg.MBSSID[MAIN_MBSSID].mbss_idx = MAIN_MBSSID;
				APStartUpForMbss(pAd,&pAd->ApCfg.MBSSID[MAIN_MBSSID]);
				APStartRekeyTimer(pAd,wdev);
			}
        }
        MTWF_LOG(DBG_CAT_INIT, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("AP interface up for ra_%x\n", MAIN_MBSSID));
		break;
		case CMD_RTPRIV_IOCTL_PREPARE:
		{
			RT_CMD_AP_IOCTL_CONFIG *pConfig = (RT_CMD_AP_IOCTL_CONFIG *)pData;
			pConfig->Status = RTMP_AP_IoctlPrepare(pAd, pData);
			if (pConfig->Status != 0)
				return NDIS_STATUS_FAILURE;
		}
			break;

		case CMD_RTPRIV_IOCTL_AP_SIOCGIWAP:
		{
			UCHAR *pBssidDest = (UCHAR *)pData;
			PCHAR pBssidStr;

#ifdef APCLI_SUPPORT
			if (Data == INT_APCLI)
			{
				if (pAd->ApCfg.ApCliTab[pObj->ioctl_if].Valid == TRUE)
					pBssidStr = (PCHAR)&APCLI_ROOT_BSSID_GET(pAd, pAd->ApCfg.ApCliTab[pObj->ioctl_if].MacTabWCID);
				else
					pBssidStr = NULL;
			}
			else
#endif /* APCLI_SUPPORT */
			{
				pBssidStr = (PCHAR) &pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev.bssid[0];
			}

			if (pBssidStr != NULL)
			{
				memcpy(pBssidDest, pBssidStr, ETH_ALEN);
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("IOCTL::SIOCGIWAP(=%02x:%02x:%02x:%02x:%02x:%02x)\n",
						PRINT_MAC(pBssidStr)));
			}
			else
			{
				memset(pBssidDest, 0, ETH_ALEN);
			}
		}
			break;

		case CMD_RTPRIV_IOCTL_AP_SIOCGIWRATEQ:
		/* handle for SIOCGIWRATEQ */
		{
			RT_CMD_IOCTL_RATE *pRate = (RT_CMD_IOCTL_RATE *)pData;
			HTTRANSMIT_SETTING HtPhyMode;

#ifdef APCLI_SUPPORT
			if (pRate->priv_flags == INT_APCLI)
				HtPhyMode = pAd->ApCfg.ApCliTab[pObj->ioctl_if].wdev.HTPhyMode;
			else
#endif /* APCLI_SUPPORT */
#ifdef WDS_SUPPORT
			if (pRate->priv_flags == INT_WDS)
				HtPhyMode = pAd->WdsTab.WdsEntry[pObj->ioctl_if].wdev.HTPhyMode;
			else
#endif /* WDS_SUPPORT */
			{
				HtPhyMode = pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev.HTPhyMode;


			}

			RtmpDrvMaxRateGet(pAd, HtPhyMode.field.MODE, HtPhyMode.field.ShortGI,
							HtPhyMode.field.BW, HtPhyMode.field.MCS,
							pAd->Antenna.field.TxPath,
							(UINT32 *)&pRate->BitRate);

		}
			break;

#ifdef HOSTAPD_SUPPORT
		case CMD_RTPRIV_IOCTL_AP_SIOCGIWRATEQ:
			RtmpHostapdSecuritySet(pAd, wrq);
			break;
#endif /* HOSTAPD_SUPPORT */

		default:
			Status = RTMP_COM_IoctlHandle(pAd, wrq, cmd, subcmd, pData, Data);
			break;
	}

	return Status;
}



#ifdef DYNAMIC_VGA_SUPPORT
INT Set_DyncVgaEnable_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	UINT Enable;

	Enable = simple_strtol(arg, 0, 10);

	pAd->CommonCfg.lna_vga_ctl.bDyncVgaEnable = (Enable > 0) ? TRUE : FALSE;

	if (pAd->CommonCfg.lna_vga_ctl.bDyncVgaEnable == TRUE)	{
		dynamic_vga_enable(pAd);
	} else {
		dynamic_vga_disable(pAd);
	}

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,
				("Set_DyncVgaEnable_Proc::(enable = %d)\n",
				pAd->CommonCfg.lna_vga_ctl.bDyncVgaEnable));

	return TRUE;
}


INT set_false_cca_hi_th(PRTMP_ADAPTER pAd, RTMP_STRING *arg)
{
	INT32 val = simple_strtol(arg, 0, 10);

	pAd->CommonCfg.lna_vga_ctl.nFalseCCATh = (val <= 0) ? 800 : val;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s::(false cca high threshould = %d)\n",
		__FUNCTION__, pAd->CommonCfg.lna_vga_ctl.nFalseCCATh));

	return TRUE;
}


INT set_false_cca_low_th(PRTMP_ADAPTER pAd, RTMP_STRING *arg)
{
	INT32 val = simple_strtol(arg, 0, 10);

	pAd->CommonCfg.lna_vga_ctl.nLowFalseCCATh = (val <= 0) ? 10 : val;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s::(false cca low threshould = %d)\n",
		__FUNCTION__, pAd->CommonCfg.lna_vga_ctl.nLowFalseCCATh));

	return TRUE;
}
#endif /* DYNAMIC_VGA_SUPPORT */

#ifdef CONFIG_HOTSPOT
static INT Set_AP_HS_IE(
	IN PRTMP_ADAPTER pAd,
	IN UINT8 EID,
	IN RTMP_STRING *IE,
	IN UINT32 IELen)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR apidx = pObj->ioctl_if;
	PHOTSPOT_CTRL pHSCtrl =  &pAd->ApCfg.MBSSID[apidx].HotSpotCtrl;

	switch (EID)
	{
		case IE_INTERWORKING:
			os_alloc_mem(NULL, &pHSCtrl->InterWorkingIE, IELen);
			NdisMoveMemory(pHSCtrl->InterWorkingIE, IE, IELen);
			pHSCtrl->InterWorkingIELen = IELen;
			pHSCtrl->AccessNetWorkType  = (*(IE + 2)) & 0x0F;
			if (IELen > 3)
			{
				pHSCtrl->IsHessid = TRUE;
				if (IELen == 7)
					NdisMoveMemory(pHSCtrl->Hessid, IE + 3, MAC_ADDR_LEN);
				else
					NdisMoveMemory(pHSCtrl->Hessid, IE + 5 , MAC_ADDR_LEN);
			}			
			break;
		case IE_ADVERTISEMENT_PROTO:
			os_alloc_mem(NULL, &pHSCtrl->AdvertisementProtoIE, IELen);
			NdisMoveMemory(pHSCtrl->AdvertisementProtoIE, IE, IELen);
			pHSCtrl->AdvertisementProtoIELen = IELen;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set Advertisement Protocol IE\n"));
			break;
		case IE_QOS_MAP_SET:
		{
			int tmp=0;
			char *pos=(char *)(IE+2);
			os_alloc_mem(NULL, &pHSCtrl->QosMapSetIE, IELen);
			NdisMoveMemory(pHSCtrl->QosMapSetIE, IE, IELen);
			pHSCtrl->QosMapSetIELen = IELen;
			for(tmp=0;tmp<(IELen-16-2)/2;tmp++)
			{
				pHSCtrl->DscpException[tmp] = *pos & 0xff;
				pHSCtrl->DscpException[tmp] |= (*(pos+1) & 0xff) << 8;
				pos += 2;
				MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("DscpException[%d]:0x%x\n",tmp,pHSCtrl->DscpException[tmp]));
			}
			for(tmp=0;tmp<8;tmp++)
            {
                pHSCtrl->DscpRange[tmp] = *pos & 0xff;
                pHSCtrl->DscpRange[tmp] |= (*(pos+1) & 0xff) << 8;
                pos += 2;
                MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("DscpRange[%d]:0x%x\n",tmp,pHSCtrl->DscpRange[tmp]));
            }

			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("=========================================== Set Qos MAP Set IE\n"));
			break;
		}
		case IE_ROAMING_CONSORTIUM:
			os_alloc_mem(NULL, &pHSCtrl->RoamingConsortiumIE, IELen);
			NdisMoveMemory(pHSCtrl->RoamingConsortiumIE, IE, IELen);
			pHSCtrl->RoamingConsortiumIELen = IELen;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set Roaming Consortium IE\n"));
			break;
		default:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: Unknown IE(EID = %d)\n", __FUNCTION__, EID));
			break;
	}

	return TRUE;
}
#endif

static INT Set_AP_VENDOR_SPECIFIC_IE(
	IN PRTMP_ADAPTER pAd,
	IN UINT8 OUIType,
	IN RTMP_STRING *IE,
	IN UINT32 IELen)
{
#ifdef CONFIG_HOTSPOT
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR apidx = pObj->ioctl_if;
	PHOTSPOT_CTRL pHSCtrl =  &pAd->ApCfg.MBSSID[apidx].HotSpotCtrl;
#endif

	switch (OUIType) {
#ifdef CONFIG_HOTSPOT
		case OUI_P2P:
			os_alloc_mem(NULL, &pHSCtrl->P2PIE, IELen);
			NdisMoveMemory(pHSCtrl->P2PIE, IE, IELen);
			pHSCtrl->P2PIELen = IELen;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set P2P IE\n"));
			break;
		case OUI_HS2_INDICATION:
			os_alloc_mem(NULL, &pHSCtrl->HSIndicationIE, IELen);
			NdisMoveMemory(pHSCtrl->HSIndicationIE, IE, IELen);
			pHSCtrl->HSIndicationIELen = IELen;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set HS2.0 Indication IE\n"));
			break;
#endif
		default:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: Unknown OUIType = %d\n", __FUNCTION__, OUIType));
		break;

	}

	return TRUE;
}

#ifdef CONFIG_DOT11V_WNM
static INT Set_AP_WNM_IE(
	IN PRTMP_ADAPTER pAd,
	IN UINT8 EID,
	IN RTMP_STRING *IE,
	IN UINT32 IELen)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR apidx = pObj->ioctl_if;
	PWNM_CTRL pWNMCtrl =  &pAd->ApCfg.MBSSID[apidx].WNMCtrl;

	switch (EID)
	{
		case IE_TIME_ADVERTISEMENT:
			os_alloc_mem(NULL, &pWNMCtrl->TimeadvertisementIE, IELen);
			NdisMoveMemory(pWNMCtrl->TimeadvertisementIE, IE, IELen);
			pWNMCtrl->TimeadvertisementIELen = IELen;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set Time Advertisement IE\n"));
			break;
		case IE_TIME_ZONE:
			os_alloc_mem(NULL, &pWNMCtrl->TimezoneIE, IELen);
			NdisMoveMemory(pWNMCtrl->TimezoneIE, IE, IELen);
			pWNMCtrl->TimezoneIELen = IELen;
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("Set Time Zone IE\n"));
			break;
		default:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: Unknown IE(EID = %d)\n", __FUNCTION__, EID));
			break;
	}

	return TRUE;
}
#endif

INT Set_AP_IE(
	IN PRTMP_ADAPTER pAd,
	IN RTMP_STRING *IE,
	IN UINT32 IELen)
{
	UINT8 EID;
	UINT8 OUIType;

	EID = *IE;

	switch (EID)
	{
#ifdef CONFIG_HOTSPOT
		case IE_INTERWORKING:
		case IE_ADVERTISEMENT_PROTO:
		case IE_QOS_MAP_SET:
		case IE_ROAMING_CONSORTIUM:
			Set_AP_HS_IE(pAd, EID, IE, IELen);
			break;
#endif

#ifdef CONFIG_DOT11V_WNM
		case IE_TIME_ADVERTISEMENT:
		case IE_TIME_ZONE:
			Set_AP_WNM_IE(pAd, EID, IE, IELen);
			break;
#endif
		case IE_VENDOR_SPECIFIC:
			OUIType = *(IE + 5);
			Set_AP_VENDOR_SPECIFIC_IE(pAd, OUIType, IE, IELen);
			break;
		default:
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s: Unknown IE(EID = %d)\n", __FUNCTION__, EID));
			break;
	}

	return TRUE;
}

#ifdef CONFIG_HOTSPOT
INT Send_ANQP_Rsp(
	IN PRTMP_ADAPTER pAd,
	IN RTMP_STRING *PeerMACAddr,
	IN RTMP_STRING *ANQPRsp,
	IN UINT32 ANQPRspLen)
{
	UCHAR *Buf;
	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie;
	UCHAR apidx = pObj->ioctl_if;
	PHOTSPOT_CTRL pHSCtrl = &pAd->ApCfg.MBSSID[apidx].HotSpotCtrl;
	PGAS_CTRL pGASCtrl = &pAd->ApCfg.MBSSID[apidx].GASCtrl;
	GAS_EVENT_DATA *Event;
	GAS_PEER_ENTRY *GASPeerEntry;
	GAS_QUERY_RSP_FRAGMENT *GASQueryRspFrag, *Tmp;
	UINT32 Len = 0, i, QueryRspOffset = 0;
	BOOLEAN Cancelled;
	BOOLEAN IsFound = FALSE;

	printk("%s\n", __FUNCTION__);


	RTMP_SEM_LOCK(&pGASCtrl->GASPeerListLock);
	/* Cancel PostReply timer after receiving daemon response */
	DlListForEach(GASPeerEntry, &pGASCtrl->GASPeerList, GAS_PEER_ENTRY, List)
	{
		if (MAC_ADDR_EQUAL(GASPeerEntry->PeerMACAddr, PeerMACAddr))
		{
			if (GASPeerEntry->PostReplyTimerRunning)
			{
				RTMPCancelTimer(&GASPeerEntry->PostReplyTimer, &Cancelled);
				GASPeerEntry->PostReplyTimerRunning = FALSE;
			}

			break;
		}
	}

	RTMP_SEM_UNLOCK(&pGASCtrl->GASPeerListLock);

	os_alloc_mem(NULL, (UCHAR **)&Buf, sizeof(*Event) + ANQPRspLen);

	if (!Buf)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s Not available memory\n", __FUNCTION__));
		goto error0;
	}

	NdisZeroMemory(Buf, sizeof(*Event) + ANQPRspLen);

	Event = (GAS_EVENT_DATA *)Buf;

	Event->ControlIndex = apidx;
	Len += 1;
	NdisMoveMemory(Event->PeerMACAddr, PeerMACAddr, MAC_ADDR_LEN);
	Len += MAC_ADDR_LEN;

	if (ANQPRspLen <= pHSCtrl->MMPDUSize)
	{
		Event->EventType = GAS_RSP;
		Len += 2;

		RTMP_SEM_LOCK(&pGASCtrl->GASPeerListLock);
		DlListForEach(GASPeerEntry, &pGASCtrl->GASPeerList, GAS_PEER_ENTRY, List)
		{
			if (MAC_ADDR_EQUAL(GASPeerEntry->PeerMACAddr, PeerMACAddr))
			{
				IsFound = TRUE;
				printk("GAS RSP DialogToken = %x\n", GASPeerEntry->DialogToken);
				Event->u.GAS_RSP_DATA.DialogToken = GASPeerEntry->DialogToken;
				Len += 1;
			}
			break;
		}
		RTMP_SEM_UNLOCK(&pGASCtrl->GASPeerListLock);

		if (!IsFound) {
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s Can not find peer address in GASPeerList\n", __FUNCTION__));
			goto error1;
		}

		Event->u.GAS_RSP_DATA.StatusCode = 0;
		Len += 2;
		Event->u.GAS_RSP_DATA.GASComebackDelay = 0;
		Len += 2;
		Event->u.GAS_RSP_DATA.AdvertisementProID = ACCESS_NETWORK_QUERY_PROTOCOL;
		Len += 1;
		Event->u.GAS_RSP_DATA.QueryRspLen = ANQPRspLen;
		Len += 2;

		NdisMoveMemory(Event->u.GAS_RSP_DATA.QueryRsp, ANQPRsp, ANQPRspLen);
		Len += ANQPRspLen;

		GASPeerEntry->CurrentState = WAIT_GAS_RSP;

		MlmeEnqueue(pAd, GAS_STATE_MACHINE, GAS_RSP, Len, Buf,0);
	}
	else
	{
		Event->EventType = GAS_RSP_MORE;
		Len += 2;

		RTMP_SEM_LOCK(&pGASCtrl->GASPeerListLock);
		DlListForEach(GASPeerEntry, &pGASCtrl->GASPeerList, GAS_PEER_ENTRY, List)
		{
			if (MAC_ADDR_EQUAL(GASPeerEntry->PeerMACAddr, PeerMACAddr))
			{
				IsFound = TRUE;

				Event->u.GAS_RSP_MORE_DATA.DialogToken = GASPeerEntry->DialogToken;
				Len += 1;

				if ((ANQPRspLen % pHSCtrl->MMPDUSize) == 0)
					GASPeerEntry->GASRspFragNum = ANQPRspLen / pHSCtrl->MMPDUSize;
				else
					GASPeerEntry->GASRspFragNum = (ANQPRspLen / pHSCtrl->MMPDUSize) + 1;

				GASPeerEntry->CurrentGASFragNum = 0;

				for (i = 0; i < GASPeerEntry->GASRspFragNum; i++)
				{
					os_alloc_mem(NULL, (UCHAR **)&GASQueryRspFrag, sizeof(*GASQueryRspFrag));

					if (!GASQueryRspFrag)
					{
						MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s Not available memory\n", __FUNCTION__));
						RTMP_SEM_UNLOCK(&pGASCtrl->GASPeerListLock);
						goto error1;
					}
					GASPeerEntry->AllocResource++;

					NdisZeroMemory(GASQueryRspFrag, sizeof(*GASQueryRspFrag));

					GASQueryRspFrag->GASRspFragID = i;

					if (i < (GASPeerEntry->GASRspFragNum - 1))
						GASQueryRspFrag->FragQueryRspLen = pHSCtrl->MMPDUSize;
					else
						GASQueryRspFrag->FragQueryRspLen = ANQPRspLen - (pHSCtrl->MMPDUSize * i);

					os_alloc_mem(NULL, (UCHAR **)&GASQueryRspFrag->FragQueryRsp,
										GASQueryRspFrag->FragQueryRspLen);
					GASPeerEntry->AllocResource++;

					if (!GASQueryRspFrag->FragQueryRsp)
					{
						MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s Not available memory\n", __FUNCTION__));
						RTMP_SEM_UNLOCK(&pGASCtrl->GASPeerListLock);
						goto error2;
					}

					NdisMoveMemory(GASQueryRspFrag->FragQueryRsp, &ANQPRsp[QueryRspOffset],
									GASQueryRspFrag->FragQueryRspLen);

					QueryRspOffset += GASQueryRspFrag->FragQueryRspLen;

					DlListAddTail(&GASPeerEntry->GASQueryRspFragList,
								  &GASQueryRspFrag->List);
				}
				break;
			}
		}
		RTMP_SEM_UNLOCK(&pGASCtrl->GASPeerListLock);

		if (!IsFound) {
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s Can not find peer address in GASPeerList\n", __FUNCTION__));
			goto error1;
		}

		Event->u.GAS_RSP_MORE_DATA.StatusCode = 0;
		Len += 2;
		//Event->u.GAS_RSP_MORE_DATA.GASComebackDelay = 1000;
		Event->u.GAS_RSP_MORE_DATA.GASComebackDelay = pGASCtrl->cb_delay;
		//printk("\033[1;32m %s, %u cb_delay : %d\033[0m\n", __FUNCTION__, __LINE__,Event->u.GAS_RSP_MORE_DATA.GASComebackDelay);  /* Kyle Debug Print */
		Len += 2;
		Event->u.GAS_RSP_MORE_DATA.AdvertisementProID = ACCESS_NETWORK_QUERY_PROTOCOL;
		Len += 1;

		GASPeerEntry->CurrentState = WAIT_GAS_RSP;
		MlmeEnqueue(pAd, GAS_STATE_MACHINE, GAS_RSP_MORE, Len, Buf, 0);
	}

	os_free_mem( Buf);

	return TRUE;

error2:
	RTMP_SEM_LOCK(&pGASCtrl->GASPeerListLock);
	DlListForEachSafe(GASQueryRspFrag, Tmp, &GASPeerEntry->GASQueryRspFragList,
											GAS_QUERY_RSP_FRAGMENT, List)
	{
		DlListDel(&GASQueryRspFrag->List);
		os_free_mem( GASQueryRspFrag);
	}

	DlListInit(&GASPeerEntry->GASQueryRspFragList);
	RTMP_SEM_UNLOCK(&pGASCtrl->GASPeerListLock);
error1:
	os_free_mem( Buf);
error0:
	return FALSE;
}

#endif

#ifdef CONFIG_DOT11V_WNM
INT Send_BTM_Req(
	IN PRTMP_ADAPTER pAd,
	IN RTMP_STRING *PeerMACAddr,
	IN RTMP_STRING *BTMReq,
	IN UINT32 BTMReqLen)
{
	UCHAR *Buf;
	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie;
	UCHAR APIndex = pObj->ioctl_if;
	PWNM_CTRL pWNMCtrl = &pAd->ApCfg.MBSSID[APIndex].WNMCtrl;
	BTM_EVENT_DATA *Event;
	BTM_PEER_ENTRY *BTMPeerEntry;
	UINT32 Len = 0;
	INT32 Ret;
	BOOLEAN IsFound = FALSE;
#ifdef DOT11K_RRM_SUPPORT	
#ifdef AP_SCAN_SUPPORT	
	UINT32 loop = 0;
	ULONG len_of_candidate_list = 0;
	RRM_BSSID_INFO BssidInfo;
	CHAR *candidate_list_included = NULL;
	struct wifi_dev *wdev = NULL;
	NDIS_802_11_SSID ssid;	
#endif /* AP_SCAN_SUPPORT */
#endif /* DOT11K_RRM_SUPPORT */

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s\n", __FUNCTION__));

	RTMP_SEM_EVENT_WAIT(&pWNMCtrl->BTMPeerListLock, Ret);
	DlListForEach(BTMPeerEntry, &pWNMCtrl->BTMPeerList, BTM_PEER_ENTRY, List)
	{
		if (MAC_ADDR_EQUAL(BTMPeerEntry->PeerMACAddr, PeerMACAddr))
		{
			IsFound = TRUE;
			break;
		}
	}
	RTMP_SEM_EVENT_UP(&pWNMCtrl->BTMPeerListLock);

	if (!IsFound)
	{
		os_alloc_mem(NULL, (UCHAR **)&BTMPeerEntry, sizeof(*BTMPeerEntry));

		if (!BTMPeerEntry)
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s Not available memory\n", __FUNCTION__));
			goto error0;
		}

		NdisZeroMemory(BTMPeerEntry, sizeof(*BTMPeerEntry));

		BTMPeerEntry->CurrentState = WAIT_BTM_REQ;
		BTMPeerEntry->ControlIndex = APIndex;
		NdisMoveMemory(BTMPeerEntry->PeerMACAddr, PeerMACAddr, MAC_ADDR_LEN);
		BTMPeerEntry->DialogToken = 1;
		BTMPeerEntry->Priv = pAd;

		RTMPInitTimer(pAd, &BTMPeerEntry->WaitPeerBTMRspTimer,
				GET_TIMER_FUNCTION(WaitPeerBTMRspTimeout), BTMPeerEntry, FALSE);
		RTMP_SEM_EVENT_WAIT(&pWNMCtrl->BTMPeerListLock, Ret);
		DlListAddTail(&pWNMCtrl->BTMPeerList, &BTMPeerEntry->List);
		RTMP_SEM_EVENT_UP(&pWNMCtrl->BTMPeerListLock);
	}

#ifdef DOT11K_RRM_SUPPORT	
	os_alloc_mem(NULL, (UCHAR **)&Buf, sizeof(*Event) + BTMReqLen + sizeof(RRM_NEIGHBOR_REP_INFO));
#else
	os_alloc_mem(NULL, (UCHAR **)&Buf, sizeof(*Event) + BTMReqLen);
#endif /* DOT11K_RRM_SUPPORT */

	if (!Buf)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s Not available memory\n", __FUNCTION__));
		goto error1;
	}

#ifdef DOT11K_RRM_SUPPORT	
	NdisZeroMemory(Buf, sizeof(*Event) + BTMReqLen + sizeof(RRM_NEIGHBOR_REP_INFO));
#else
	NdisZeroMemory(Buf, sizeof(*Event) + BTMReqLen);
#endif /* DOT11K_RRM_SUPPORT */

	Event = (BTM_EVENT_DATA *)Buf;

	Event->ControlIndex = APIndex;
	Len += 1;

	NdisMoveMemory(Event->PeerMACAddr, PeerMACAddr, MAC_ADDR_LEN);
	Len += MAC_ADDR_LEN;

	Event->EventType = BTM_REQ;
	Len += 2;

	Event->u.BTM_REQ_DATA.DialogToken = BTMPeerEntry->DialogToken;
	Len += 1;

	Event->u.BTM_REQ_DATA.BTMReqLen = BTMReqLen;
	Len += 2;

#ifdef DOT11K_RRM_SUPPORT	
#ifdef AP_SCAN_SUPPORT
	if (pObj->ioctl_if_type == INT_MBSSID)
		wdev = &pAd->ApCfg.MBSSID[APIndex].wdev;
	else
		wdev = &pAd->ApCfg.MBSSID[0].wdev;

	NdisZeroMemory(&ssid, sizeof(NDIS_802_11_SSID));
	ssid.SsidLength = 0;	
	ApSiteSurvey_by_wdev(pAd, &ssid, SCAN_PASSIVE, FALSE, wdev);

	OS_WAIT(4000);

	if (pAd->ScanTab.BssNr > 0) {
		candidate_list_included = &BTMReq[0];
		*candidate_list_included |= 0x01;
	}	
#endif /* AP_SCAN_SUPPORT */	
#endif /* DOT11K_RRM_SUPPORT */
		
	NdisMoveMemory(Event->u.BTM_REQ_DATA.BTMReq, BTMReq, BTMReqLen);
	Len += BTMReqLen;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, 
		("\nbefore adding BSS Transition Candidate List Entries::BTMReqLen=%d, Len=%d\n", 
		BTMReqLen, Len));
	
#ifdef DOT11K_RRM_SUPPORT	
#ifdef AP_SCAN_SUPPORT
	if (pAd->ScanTab.BssNr > 0) 
	{
		/* here to add the BSS Transition Candidate List Entries */
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, 
			("%s::adding BSS Transition Candidate List Entries\n", __FUNCTION__));

		BssTableSortByRssi(&pAd->ScanTab, FALSE);		

		/* for (loop=0; loop<pAd->ScanTab.BssNr; loop++) */
		for (loop=0; loop<1; loop++)
		{
			BSS_ENTRY *pBssEntry = &pAd->ScanTab.BssEntry[loop];

			len_of_candidate_list += sizeof(RRM_NEIGHBOR_REP_INFO);		
			if (len_of_candidate_list >= 2304)
				break;

			/* update Neighbor Report Information Elements */
			BssidInfo.word = 0;
			BssidInfo.field.APReachAble = 3;
			BssidInfo.field.Security = 0;
			BssidInfo.field.KeyScope = 0;
			BssidInfo.field.SepctrumMng = (pBssEntry->CapabilityInfo & (1 << 8)) ? 1:0;
			BssidInfo.field.Qos = (pBssEntry->CapabilityInfo & (1 << 9)) ? 1:0;
			BssidInfo.field.APSD = (pBssEntry->CapabilityInfo & (1 << 11)) ? 1:0;
			BssidInfo.field.RRM = (pBssEntry->CapabilityInfo & RRM_CAP_BIT) ? 1:0;
			BssidInfo.field.DelayBlockAck = (pBssEntry->CapabilityInfo & (1 << 14)) ? 1:0;
			BssidInfo.field.ImmediateBA = (pBssEntry->CapabilityInfo & (1 << 15)) ? 1:0;
			BssidInfo.field.MobilityDomain = (pBssEntry->bHasMDIE ) ? 1:0;
			BssidInfo.field.HT = (pBssEntry->HtCapabilityLen != 0) ? 1:0;
#ifdef DOT11_VHT_AC			
			BssidInfo.field.VHT = (pBssEntry->vht_cap_len != 0) ? 1:0;
#endif /* DOT11_VHT_AC */
			
			if (pBssEntry->Channel > 14)
			{
				if (pBssEntry->HtCapabilityLen != 0)
				{
#ifdef DOT11_VHT_AC				
					if (pBssEntry->vht_cap_len != 0)
						pBssEntry->CondensedPhyType = 9;
					else
#endif /* DOT11_VHT_AC */
						pBssEntry->CondensedPhyType = 7;
				}
				else
				{
					pBssEntry->CondensedPhyType = 4;
				}
			}
			else
			{
				if (pBssEntry->HtCapabilityLen != 0)
					pBssEntry->CondensedPhyType = 7;
				else if (ERP_IS_NON_ERP_PRESENT(pBssEntry->Erp))
					pBssEntry->CondensedPhyType = 6;
				else if (pBssEntry->SupRateLen > 4)
					pBssEntry->CondensedPhyType = 4;
			}

			RRM_InsertNeighborRepIE(pAd, 
				(Event->u.BTM_REQ_DATA.BTMReq + BTMReqLen), 
				(PULONG)&BTMReqLen,
				sizeof(RRM_NEIGHBOR_REP_INFO), 
				pBssEntry->Bssid,
				BssidInfo, 
				pBssEntry->RegulatoryClass,
				pBssEntry->Channel, 
				pBssEntry->CondensedPhyType);		

			Event->u.BTM_REQ_DATA.BTMReqLen = BTMReqLen;
			Len += sizeof(RRM_NEIGHBOR_REP_INFO);
			
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, 
				("after adding BSS Transition Candidate List Entries::(%d)BTMReqLen=%d, Len=%d\n", 
				loop, BTMReqLen, Len));
		}
	}
#endif /* AP_SCAN_SUPPORT */
#endif /* DOT11K_RRM_SUPPORT */

	MlmeEnqueue(pAd, BTM_STATE_MACHINE, BTM_REQ, Len, Buf, 0);

	os_free_mem( Buf);

	return TRUE;

error1:
	if (!IsFound)
		os_free_mem( BTMPeerEntry);
error0:
	return FALSE;
}

#ifdef CONFIG_HOTSPOT_R2
INT Send_WNM_Notify_Req(
	IN PRTMP_ADAPTER pAd,
	IN RTMP_STRING *PeerMACAddr,
	IN RTMP_STRING *WNMNotifyReq,
	IN UINT32 WNMNotifyReqLen,
	IN UINT32 type)
{
	UCHAR *Buf;
	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie;
	UCHAR APIndex = pObj->ioctl_if;
	PWNM_CTRL pWNMCtrl = &pAd->ApCfg.MBSSID[APIndex].WNMCtrl;
	WNM_NOTIFY_EVENT_DATA *Event;
	WNM_NOTIFY_PEER_ENTRY *WNMNotifyPeerEntry;
	UINT32 Len = 0;
	INT32 Ret;
	BOOLEAN IsFound = FALSE;

	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("%s\n", __FUNCTION__));

	RTMP_SEM_EVENT_WAIT(&pWNMCtrl->WNMNotifyPeerListLock, Ret);
	DlListForEach(WNMNotifyPeerEntry, &pWNMCtrl->WNMNotifyPeerList, WNM_NOTIFY_PEER_ENTRY, List)
	{
		if (MAC_ADDR_EQUAL(WNMNotifyPeerEntry->PeerMACAddr, PeerMACAddr))
		{
			IsFound = TRUE;
			break;
		}
	}
	RTMP_SEM_EVENT_UP(&pWNMCtrl->WNMNotifyPeerListLock);

	printk("*%d\n", IsFound);
	if (!IsFound)
	{
		os_alloc_mem(NULL, (UCHAR **)&WNMNotifyPeerEntry, sizeof(*WNMNotifyPeerEntry));

		if (!WNMNotifyPeerEntry)
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s Not available memory\n", __FUNCTION__));
			goto error0;
		}

		NdisZeroMemory(WNMNotifyPeerEntry, sizeof(*WNMNotifyPeerEntry));

		WNMNotifyPeerEntry->CurrentState = WNM_NOTIFY_REQ;
		WNMNotifyPeerEntry->ControlIndex = APIndex;
		NdisMoveMemory(WNMNotifyPeerEntry->PeerMACAddr, PeerMACAddr, MAC_ADDR_LEN);
		WNMNotifyPeerEntry->DialogToken = 1;
		WNMNotifyPeerEntry->Priv = pAd;

		RTMPInitTimer(pAd, &WNMNotifyPeerEntry->WaitPeerWNMNotifyRspTimer,
				GET_TIMER_FUNCTION(WaitPeerWNMNotifyRspTimeout), WNMNotifyPeerEntry, FALSE);
		RTMP_SEM_EVENT_WAIT(&pWNMCtrl->WNMNotifyPeerListLock, Ret);
		DlListAddTail(&pWNMCtrl->WNMNotifyPeerList, &WNMNotifyPeerEntry->List);
		RTMP_SEM_EVENT_UP(&pWNMCtrl->WNMNotifyPeerListLock);
	}

	os_alloc_mem(NULL, (UCHAR **)&Buf, sizeof(*Event) + WNMNotifyReqLen);

	if (!Buf)
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s Not available memory\n", __FUNCTION__));
		goto error1;
	}

	//RTMPusecDelay(200000); //200ms

	NdisZeroMemory(Buf, sizeof(*Event) + WNMNotifyReqLen);

	Event = (WNM_NOTIFY_EVENT_DATA *)Buf;

	Event->ControlIndex = APIndex;
	Len += 1;

	NdisMoveMemory(Event->PeerMACAddr, PeerMACAddr, MAC_ADDR_LEN);
	Len += MAC_ADDR_LEN;

	Event->EventType = type; //WNM_NOTIFY_REQ;
	Len += 2;

	Event->u.WNM_NOTIFY_REQ_DATA.DialogToken = WNMNotifyPeerEntry->DialogToken;
	Len += 1;

	Event->u.WNM_NOTIFY_REQ_DATA.WNMNotifyReqLen = WNMNotifyReqLen;
	Len += 2;

	NdisMoveMemory(Event->u.WNM_NOTIFY_REQ_DATA.WNMNotifyReq, WNMNotifyReq, WNMNotifyReqLen);
	Len += WNMNotifyReqLen;

	MlmeEnqueue(pAd, WNM_NOTIFY_STATE_MACHINE, WNM_NOTIFY_REQ, Len, Buf, 0);

	os_free_mem( Buf);

	return TRUE;

error1:
	if (!IsFound)
		os_free_mem( WNMNotifyPeerEntry);
error0:
	return FALSE;
}

INT Send_QOSMAP_Configure(
	IN PRTMP_ADAPTER pAd,
	IN RTMP_STRING *PeerMACAddr,
	IN RTMP_STRING *QosMapBuf,
	IN UINT32 	QosMapLen,
	IN UINT8	Apidx)
{
	MLME_QOS_ACTION_STRUCT QosMapConfig;
	QOSMAP_SET *pQOSMap = &QosMapConfig.QOSMap;

	NdisZeroMemory(&QosMapConfig, sizeof(MLME_QOS_ACTION_STRUCT));
	COPY_MAC_ADDR(QosMapConfig.Addr, PeerMACAddr);
	QosMapConfig.ActionField = ACTION_QOSMAP_CONFIG;
	QosMapConfig.apidx = Apidx;
	pQOSMap->DSCP_Field_Len = QosMapLen;
	NdisMoveMemory(pQOSMap->DSCP_Field, QosMapBuf, QosMapLen);
	MlmeEnqueue(pAd, ACTION_STATE_MACHINE, MT2_MLME_QOS_CATE, sizeof(MLME_QOS_ACTION_STRUCT), (PVOID)&QosMapConfig, 0);
	RTMP_MLME_HANDLER(pAd);

	return TRUE;
}

/* for debug */
INT Set_CR4_Hotspot_Flag(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	UINT8 Flag = simple_strtol(arg, 0, 10);
	PHOTSPOT_CTRL pHSCtrl;
	POS_COOKIE pObj = (POS_COOKIE)pAd->OS_Cookie;
	UCHAR APIndex = pObj->ioctl_if;
	PWNM_CTRL pWNMCtrl = &pAd->ApCfg.MBSSID[APIndex].WNMCtrl;
	EXT_CMD_ID_HOTSPOT_INFO_UPDATE_T HotspotInfoUpdateT;
	
	pHSCtrl = &pAd->ApCfg.MBSSID[APIndex].HotSpotCtrl;
	pHSCtrl->HotSpotEnable=IS_HOTSPOT_ENABLE(Flag);
	pHSCtrl->DGAFDisable=IS_DGAF_DISABLE(Flag);
	pHSCtrl->bASANEnable=IS_ASAN_ENABLE(Flag);
	pHSCtrl->QosMapEnable=IS_QOSMAP_ENABLE(Flag);
	pWNMCtrl->ProxyARPEnable=IS_PROXYARP_ENABLE(Flag);
	
	
	NdisZeroMemory(&HotspotInfoUpdateT,sizeof(HotspotInfoUpdateT));
	HotspotInfoUpdateT.ucUpdateType |= fgUpdateBssCapability;
	HotspotInfoUpdateT.ucHotspotBssFlags = Flag;
	HotspotInfoUpdateT.ucHotspotBssId = APIndex;	
	 
	MtCmdHotspotInfoUpdate(pAd,HotspotInfoUpdateT);
	MTWF_LOG(DBG_CAT_PROTO, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s ===> Update BSS:%d  HotspotFlag:0x%x\n"
		,__FUNCTION__,HotspotInfoUpdateT.ucHotspotBssId,HotspotInfoUpdateT.ucHotspotBssFlags));
	hotspot_bssflag_dump(Flag);
	return TRUE;
}

#endif /* CONFIG_HOTSPOT_R2 */
#endif /* CONFIG_DOT11V_WNM */
#ifdef CAL_TO_FLASH_SUPPORT	
INT Set_KtoFlash_Debug_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	UINT Enable;

	Enable = simple_strtol(arg, 0, 10);

	pAd->KtoFlashDebug = (Enable > 0) ? TRUE : FALSE;
	
	MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_TRACE,
				("Set_KtoFlash_Debug_Proc::(enable = %d)\n",
				pAd->KtoFlashDebug));

	return TRUE;
}
INT Set_RDCE_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	INT Type;
	INT BW;
	INT Band;
	INT i4Recv;

	if(arg)
	{
		i4Recv = sscanf(arg, "%d-%d-%d", &(Type),&(BW), &(Band));
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("%s():Type %d,BW %d,Band %d\n",
				__FUNCTION__,Type,BW,Band));

		if (i4Recv != 3)
		{
			MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Format Error!\n"));
			return FALSE;
		}

		MtCmdRDCE(pAd,Type,BW,Band);
	}
	else
	{
		MTWF_LOG(DBG_CAT_CFG, DBG_SUBCAT_ALL, DBG_LVL_ERROR, ("Format Error!\n"));
				return FALSE;
	}
	
	return TRUE;
}

#endif /* CAL_TO_FLASH_SUPPORT */

#ifdef RLM_CAL_CACHE_SUPPORT
INT Set_RLM_Cal_Cache_Debug_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
    rlmCalCacheStatus(pAd->rlmCalCache);
    rlmCalCacheDump(pAd->rlmCalCache);
	return TRUE;
}
#endif /* RLM_CAL_CACHE_SUPPORT */

INT Set_BWF_Enable_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{	
	UINT Enable;	
	Enable = simple_strtol(arg, 0, 10);
    MtCmdSetBWFEnable(pAd,(UINT8)Enable);
	return TRUE;
}

#ifdef TX_AGG_ADJUST_WKR
INT Set_AggAdjWkr_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	pAd->TxAggAdjsut = simple_strtol(arg, 0, 16);
	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF,
		    ("TxAggAdjsut = %u\n", pAd->TxAggAdjsut));
	return TRUE;
}
#endif /* TX_AGG_ADJUST_WKR */

#ifdef HTC_DECRYPT_IOT
INT Set_WTBL_AAD_OM_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{

	UINT Enable;	
	Enable = simple_strtol(arg, 0, 10);
	HW_SET_ASIC_WCID_AAD_OM(pAd,1,Enable);
	


	return TRUE;
}


INT Set_HTC_Err_TH_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)

{
	UINT value;
	value = simple_strtol(arg, 0, 10);
	if (value)
	{
		pAd->HTC_ICV_Err_TH = value;
	}

	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("@@@ pAd->HTC_ICV_Err_TH=%u\n",pAd->HTC_ICV_Err_TH));

	return TRUE;

}

INT Set_Entry_HTC_Err_Cnt_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)

{
		UINT value;
		PMAC_TABLE_ENTRY pEntry;
		value = simple_strtol(arg, 0, 10);
	
		pEntry = &pAd->MacTab.Content[1];

		if (pEntry && IS_ENTRY_CLIENT(pEntry))
		{

	        	ULONG Now32;
			NdisGetSystemUpTime(&Now32);

			pEntry->HTC_ICVErrCnt = value;
			
        	// Rx HTC and FAIL decryp case!
			//if (rxd_base->RxD2.IcvErr == 1)
			{
				if (pEntry->HTC_ICVErrCnt++ > pAd->HTC_ICV_Err_TH)
				{
					pEntry->HTC_ICVErrCnt = 0; //reset the history
					
					if (pEntry->HTC_AAD_OM_Force == 0)
					{
						pEntry->HTC_AAD_OM_Force = 1;
						HW_SET_ASIC_WCID_AAD_OM(pAd,1,1);
					}
					else
					{
						pEntry->HTC_AAD_OM_Force = 0;
						HW_SET_ASIC_WCID_AAD_OM(pAd,1,0);
					}
				}
			}

			MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("@@@ pEntry->HTC_ICVErrCnt=%u\n",pEntry->HTC_ICVErrCnt));
		}
		
	return TRUE;
}
#endif /* HTC_DECRYPT_IOT */

#ifdef DHCP_UC_SUPPORT
INT Set_DHCP_UC_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	RTMP_STRING *arg)
{
	UINT value;
	value = simple_strtol(arg, 0, 10);
	if (value)
		pAd->DhcpUcEnable = TRUE;
	else
		pAd->DhcpUcEnable = FALSE;

	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, ("@@@ pAd->DhcpUcEnable=%u\n",pAd->DhcpUcEnable));

	return TRUE;

}
#endif /* DHCP_UC_SUPPORT */
