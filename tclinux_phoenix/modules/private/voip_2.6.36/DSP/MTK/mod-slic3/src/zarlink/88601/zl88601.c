/*
 * zl88601.c
 *
 *  Created on: Nov 30, 2010
 *      Author: pork
 */
/*
 * zl88601drv.c
 *
 *  Created on: Nov 11, 2010
 *      Author: pork
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <asm/atomic.h>      //dyma_20140218
#include <linux/spinlock.h>  //dyma_20140218
#include <linux/jiffies.h>     //dyma_140619

#include "vp_api.h"
#include "ZLR88621L_SM2_DEF.h"
#include "vp_api_cfg.h"
#include "vp886_api.h"
#include "vp886_api_int.h"
#include "_TDI.h"
#include "TDI.h"
#include "zl88601.h"
#include "sdk_qs_utils.h"
#include "ddr_slic3.h"
#include <linux/libcompileoption.h>


/************************************************************************
 *				GLOBE VARIABLES
 *************************************************************************
 */
extern slicFuncTbl* slicFunc[];
static VpDevCtxType            devCtx_ptr;
static Vp886DeviceObjectType   devObj_ptr;   // 1 Device should have only 1 DeviceObject
static VpLineCtxType           lineCtx_ptr[ZL88601_CHAN_NUM];
static Vp886LineObjectType     lineObj_ptr[ZL88601_CHAN_NUM];
VpEventType   zl88601legerityEventObj; // a temp var need not duplicate  [ZL88601_CHAN_NUM]; // 1=>2
zl88601LineStat_t zl88601LineStat[ZL88601_CHAN_NUM]; // 1=>2

int zl88601_Init_Cmp = _TDI_FALSE;
int zl88601_Init_Line_Cmp[ZL88601_CHAN_NUM]= {_TDI_FALSE,_TDI_FALSE};
int zl88601_Get_Mask_Cmp = _TDI_FALSE;
int zl88601_Line_Cal_Cmp[ZL88601_CHAN_NUM]= {_TDI_FALSE,_TDI_FALSE};
static int zl88601_Init_Cnt = 0;
static atomic_t zl88601_init_ongoing;  //dyma_20140218
static spinlock_t zl88601_event_process_lock;  //dyma_20140218
unsigned int zl88601_event_process_flags; //dyma_20140218

/*20110714 PTChen add for slic country*/
const VpProfileDataType *DEVICE_PRO_zl88601;
const VpProfileDataType *AC_PRO_FXS_zl88601;
const VpProfileDataType *DC_PRO_FXS_zl88601;
const VpProfileDataType *RINGING_PRO_FXS_zl88601;
const VpProfileDataType *AC_PRO_FXO_zl88601;
const VpProfileDataType *CONFIG_PRO_FXO_zl88601;
const VpProfileDataType *RINGING_CADENCE_PRO_zl88601;
const VpProfileDataType *CALL_ID_PRO_zl88601;
const VpProfileDataType *METERING_PRO_zl88601; //dyma_20140218

static char ret_event_process_chan_id;
//dyma_131106  static uint8 zl88601_ChanLocation[3]={0,0,0};  //default value 0 means no physical channel exist 
static uint8 zl88601_DevPhyLocation=0;

#ifdef RECOVER_ORIGINAL_CODE
/* Number of lines supported by Zl88601 */
//#define NB_LINE     2  // replace by ZL88601_CHAN_NUM

/* FXS on device ID, FXO on device ID */
//#define FXS_LINE    0  // related to ZL88601_FXS_NUM
//#define FXO_LINE    1  // related to ZL88601_FXO_NUM
#endif

slicFuncTbl zl88601Func[]={
		zl88601_deviceVerify,
		zl88601_deviceInit,
		zl88601_shutdownHw,
		zl88601_paramReset,
		zl88601_phoneRingStart,
		zl88601_phoneRingStop,
		zl88601_eventProcess,
		zl88601_fxsSetLineState,
		zl88601_fxsGetLineState,
		zl88601_fxsSetBatteryPol,
		zl88601_fxsGetBatteryPol,
		zl88601_fxsGetHookState,
		zl88601_ringState,
		zl88601_fxsGetPulseStatus,
		zl88601_fxsSetGainLv,
		slicFunc_dummy, // zl88601_fxoGetRingCadence,
		slicFunc_dummy, // zl88601_fxoGetHookState,
		slicFunc_dummy, // zl88601_fxoSetHookState,
		slicFunc_dummy, // zl88601_fxoGetLineStats,
		slicFunc_dummy, // zl88601_fxoGetPSTNStats,
		slicFunc_dummy, // zl88601_fxoSetMonitorMode,
		zl88601_dumpregister,
		slicFunc_dummy,
		zl88601_LtTestStart,//Roger_lt
		zl88601_getLtTestResult,//Roger_lt
		zl88601_fxsSetMetering,  //dyma_20140428 metering
		zl88601_SetLoopback,
		zl88601_SetRingParam,	//mtk07059_20150423
		zl88601_SetDcFeedParam,	//mtk07059_20150616
};

/* Termination type: VP_TERM_FXS_GENERIC or VP_TERM_FXS_LOW_PWR */
#define TERM_TYPE   VP_TERM_FXS_GENERIC
#define TERM_TYPE_T "VP_TERM_FXS_GENERIC"
#define OSBN_VOIP_SLIC_TEST 1

#ifdef RECOVER_ORIGINAL_CODE

char *vpStatusString[] = {
    "VP_STATUS_SUCCESS",
    "VP_STATUS_FAILURE",
    "VP_STATUS_FUNC_NOT_SUPPORTED",
    "VP_STATUS_INVALID_ARG",
    "VP_STATUS_MAILBOX_BUSY",
    "VP_STATUS_ERR_VTD_CODE",
    "VP_STATUS_OPTION_NOT_SUPPORTED",
    "VP_STATUS_ERR_VERIFY",
    "VP_STATUS_DEVICE_BUSY",
    "VP_STATUS_MAILBOX_EMPTY",
    "VP_STATUS_ERR_MAILBOX_DATA",
    "VP_STATUS_ERR_HBI",
    "VP_STATUS_ERR_IMAGE",
    "VP_STATUS_IN_CRTCL_SECTN",
    "VP_STATUS_DEV_NOT_INITIALIZED",
    "VP_STATUS_ERR_PROFILE",
    "VP_STATUS_INVALID_VOICE_STREAM"
};
#endif

/*20110714 PTChen add for slic country*/ //dyma_20140428 
hwProfList_t zl88601_country_mapping[] =
{
	{"Default"    	  , C_DEF, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Australia"  	  ,  C_AU, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Austria"   	  ,  C_AT, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"BEL-Belgium"    ,  C_BE, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"BRA-Brazil"     ,  C_BR, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Canada"    	  ,  C_CA, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"CHN-China"      ,  C_CN, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Czech"       	  ,  C_CZ, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"DNK-Denmark"    ,  C_DK, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"ETS-ETSI"       ,  C_EU, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"FIN-Finland"    ,  C_FI, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"FRA-France"     ,  C_FR, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"DEU-Germany"    ,  C_DE, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Greece"     	  ,  C_GR, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"HongKong"  	  ,  C_HK, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"HUN-Hungary"    ,  C_HU, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Ireland"   	  ,  C_IE, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Israel"    	  ,  C_IL, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"India"      	  ,  C_IN, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"ITA-Italy"      ,  C_IT, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"JPN-Japan"      ,  C_JP, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Singapore" 	  ,  C_SG, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"S.Korea"   	  ,  C_KR, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Mexico"     	  ,  C_MX, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"NLD-Netherlands",  C_NL, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"NewZealand" 	  ,  C_NZ, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Norway"     	  ,  C_NO, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Poland"   	  ,  C_PL, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Portugal"   	  ,  C_PT, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Slovenia"   	  ,  C_SI, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Taiwan"     	  ,  C_TW, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Spain"     	  ,  C_ES, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"SWE-Sweden"     ,  C_SE, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"GBR-Great Britain",C_GB, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"USA-United States",C_US, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"S.Africa"   	  ,  C_ZA, 	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Vietnam"	 	  ,C_VN,  ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"RSV1"			  ,C_RSV1,  ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"RSV2"			  ,C_RSV2,	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"RSV3"			  ,C_RSV3,	ZLR88621L_ABS81V_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL }
};

hwProfList_t zl88801_country_mapping[] =
{
	{"Default"    	  , C_DEF, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Australia"  	  ,  C_AU, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Austria"   	  ,  C_AT, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"BEL-Belgium"    ,  C_BE, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"BRA-Brazil"     ,  C_BR, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Canada"    	  ,  C_CA, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"CHN-China"      ,  C_CN, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Czech"       	  ,  C_CZ, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"DNK-Denmark"    ,  C_DK, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"ETS-ETSI"       ,  C_EU, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"FIN-Finland"    ,  C_FI, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"FRA-France"     ,  C_FR, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"DEU-Germany"    ,  C_DE, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Greece"     	  ,  C_GR, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"HongKong"  	  ,  C_HK, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"HUN-Hungary"    ,  C_HU, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Ireland"   	  ,  C_IE, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Israel"    	  ,  C_IL, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"India"      	  ,  C_IN, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"ITA-Italy"      ,  C_IT, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"JPN-Japan"      ,  C_JP, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Singapore" 	  ,  C_SG, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"S.Korea"   	  ,  C_KR, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Mexico"     	  ,  C_MX, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"NLD-Netherlands",  C_NL, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"NewZealand" 	  ,  C_NZ, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Norway"     	  ,  C_NO, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Poland"   	  ,  C_PL, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Portugal"   	  ,  C_PT, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Slovenia"   	  ,  C_SI, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Taiwan"     	  ,  C_TW, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Spain"     	  ,  C_ES, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"SWE-Sweden"     ,  C_SE, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"GBR-Great Britain",C_GB, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"USA-United States",C_US, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"S.Africa"   	  ,  C_ZA, 	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"Vietnam"	 	  ,C_VN,  ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"RSV1"			  ,C_RSV1,  ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"RSV2"			  ,C_RSV2,	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
	{"RSV3"			  ,C_RSV3,	ZLR88801_100UL_DEVICE,	AC_FXS_RF14_600R_DEF, 	DC_FXS_ZL880_ABS100V_DEF, 	RING_ZL880_ABS100V_DEF, 	VP_PTABLE_NULL, 	VP_PTABLE_NULL, 	NULL, 	NULL },
};

/*20110714 PTChen add for slic country here get country name from rcs file*/

/******************************************************************************
*	Function:	 slicParamReset
*
*	Description: Reset the parameters used to init slic 
*
*	Parameters:
*
*	Returns:
*
*******************************************************************************/
void zl88601_slicParamReset(int idx){
	/*====== decide profile according to Country code =====*/
	/*Reset by default table*/
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "TCSUPPORT_SLIC_ZL88801_VAL=%d\n", TCSUPPORT_SLIC_ZL88801_VAL);
	if (TCSUPPORT_SLIC_ZL88801_VAL)
		DEVICE_PRO_zl88601 = ZLR88801_100UL_DEVICE;
	else
		DEVICE_PRO_zl88601 = ZLR88621L_ABS81V_DEVICE;
	AC_PRO_FXS_zl88601 = AC_FXS_RF14_600R_DEF;
	DC_PRO_FXS_zl88601 = DC_FXS_ZL880_ABS100V_DEF;
	RINGING_PRO_FXS_zl88601 = RING_ZL880_ABS100V_DEF;
	AC_PRO_FXO_zl88601 = VP_PTABLE_NULL;
	CONFIG_PRO_FXO_zl88601 = VP_PTABLE_NULL;
	CALL_ID_PRO_zl88601 = VP_PTABLE_NULL;
	METERING_PRO_zl88601 = NULL; //dyma_20140428

	if((idx >= C_DEF) && (idx < C_NUM)){
		if (TCSUPPORT_SLIC_ZL88801_VAL)
			DEVICE_PRO_zl88601 = zl88801_country_mapping[idx].pDevice;
		else
			DEVICE_PRO_zl88601 = zl88601_country_mapping[idx].pDevice;
		AC_PRO_FXS_zl88601 = zl88601_country_mapping[idx].pAcFXS;
		DC_PRO_FXS_zl88601 = zl88601_country_mapping[idx].pDcFXS;
		RINGING_PRO_FXS_zl88601 = zl88601_country_mapping[idx].pRingingFXS;
		AC_PRO_FXO_zl88601 = zl88601_country_mapping[idx].pAcFXO;
		CONFIG_PRO_FXO_zl88601 = zl88601_country_mapping[idx].pConfigFXO;
		CALL_ID_PRO_zl88601 = zl88601_country_mapping[idx].pCallID;
		METERING_PRO_zl88601 = zl88601_country_mapping[idx].pMetering; //dyma_20140428 
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "%s Successful(CountryCode=%s) for %s\n",__func__,\
				zl88601_country_mapping[idx].name, TCSUPPORT_SLIC_ZL88801_VAL?"zl88801":"zl88601");
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "err: slicParamReset UNKNOW\n");
	}
}

#ifdef OSBN_VOIP
static int test = 0x61;
static int testCh = 0;
static int16 testMpiData;
static int8 testWMpiData = 0xbf;
static int8 testRMpiData = 0;
static int testCheck_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data){
	int len = 0;

	//VpSetLineState(&(lineCtx_ptr[FXO_LINE]),VP_LINE_FXO_LOOP_OPEN);
	//VpMpiCmdWrapper(0, testCh, test,
	//				VP890_NO_UL_SIGREG_LEN, &testMpiData);
	int16 testdata = 0;
	/*VpMpiCmd(0, 0x1, 0x61,0x1, &testdata);
	VpMpiCmd(0, 0x2, 0x61,0x1, &testdata);*/
	VpMpiCmd(0, 0x1, test,0x2, &testdata);

	printk("data:%x \n",testdata);
	VpMpiCmd(0, 0x2, test,0x2, &testdata);

	printk("data:%x \n",testdata);
	//printk("data:%x \n",mpiData);
	//len += sprintf(page+len, "mpi: %x\n",testMpiData);

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	return len;
}
static int testCheck_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	char val_string[40];
	int devIdx;
	int16 tmpValue;
	if (count > sizeof(val_string) - 1)
    		return -EINVAL;
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;
	val_string[count] = '\0';
	sscanf(val_string,"%x %d %hd\n",&test,&testCh,&testMpiData);
	printk("-->write %d ch:%d mipdata:%hd\n",test,testCh,testMpiData);

	return count;
}
#endif


/************************************************************************
 *				FUNCTION DECLARATION
 *************************************************************************
 */
/*_____________________________________________________________________________
 **      function name: _TDI_paramReset
 **      descriptions:
 **            allocate no side effect K1 buffer and record what had been allocated
 **      parameters:
 **				size: buffer size
 **				gfpFlag: used to indicate GFP flags
 **				type: what have been allocated
 **      global:
 **
 **      return:
 **	      buffer address
 **      call:
 **      revision:
 **      		1.1 2008/08/13 18:00  pork
 **      		1.2 2008/08/14 18:00  pork
 **____________________________________________________________________________
 */
int zl88601_paramReset(char* buf){
//	int country = *(int *)buf;
	uint8 i=0;
	uint8 ChanId = *(uint8*)buf;

	return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: zl88601_deviceVerify
 **      descriptions:
 **            driver verify
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	retVal
 **      call:
 **      revision:
 **____________________________________________________________________________
 */
int zl88601_deviceVerify(char* buf){
    VpStatusType          status;
	deviceInfo_t* devInfo = (deviceInfo_t*)buf;
	uint8	res_arr[2] = {0};
	uint8 lineNum = 0;
	static uint8 RCNPCN_rd_cnt=0;

    /* Verify the Device */
	RCNPCN_rd_cnt=0;
	if(devInfo->deviceNum>0)
	{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING, "zl88601_deviceVerify already! Bypass 2nd verify!\n");
		return _TDI_FAIL;
	}
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "a zl88601_deviceVerify Start\r\n");
	
	while((res_arr[0]!=VP886_R_RCNPCN_RCN_BAA) ||((res_arr[1]!=VP886_R_RCNPCN_PCN_ZL88601))){
		VpMpiCmd(devInfo->deviceNum, VP886_EC_1, VP886_R_RCNPCN_RD, VP886_R_RCNPCN_LEN, res_arr);

		RCNPCN_rd_cnt++;
		if(RCNPCN_rd_cnt>100)
		{
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "zl88601_deviceVerify RCNPCN mismatch!\n");
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "devInfo->deviceNum=%d res_arr[0]=%x res_arr[0]=%x\n",devInfo->deviceNum,res_arr[0],res_arr[1]);
			return _TDI_FAIL;
		}
	}
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "devInfo->deviceNum=%d res_arr[0]=%x res_arr[0]=%x\n",devInfo->deviceNum,res_arr[0],res_arr[1]);

		zl88601_DevPhyLocation = devInfo->deviceNum;
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "zl88601_deviceVerify Success! \r\nrd_cnt=%d\r\n",RCNPCN_rd_cnt);

    status = VpMakeDeviceObject(VP_DEV_886_SERIES, devInfo->deviceNum, &devCtx_ptr, &devObj_ptr);

    if (VP_STATUS_SUCCESS != status) {
        dbg_printf("Error making the device object: %s\n", MapStatus(status));
        return _TDI_FAIL;
    }
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "Microsemi VP_API V%d.%d.%d\n",VP_API_VERSION_MAJOR_NUM,VP_API_VERSION_MINOR_NUM,VP_API_VERSION_MINI_NUM);

	#if ZL88601_SELF_TEST
	uint8 temp_val=0x47;

	/* printing the SLIC state */		
	VpMpiCmd( devInfo->deviceNum, VP886_EC_1, VP886_R_OPCOND_RD, 1, res_arr );		
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "SLIC_SELF_TEST VP886_R_OPCOND_RD = 0x%x\n",res_arr[0]);
	
	/* changing the SLIC state */		
	VpMpiCmd( devInfo->deviceNum, VP886_EC_1, VP886_R_OPCOND_WRT, 1, &temp_val);		

	/* printing the SLIC state */		
	VpMpiCmd( devInfo->deviceNum, VP886_EC_1, VP886_R_OPCOND_RD, 1, res_arr );		
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "SLIC_SELF_TEST WRT VAL = 0x%x\nSLIC_SELF_TEST RD BACK = 0x%x\n", temp_val, res_arr[0]);
	if(res_arr[0] != temp_val){ 		
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING, "   !!! WRT RD ERROR !!!\n");		
	}else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "SLIC WRT RD test Pass\n");
	}
	
	/* changing the SLIC state again*/
	temp_val=0x0;
	VpMpiCmd( devInfo->deviceNum, VP886_EC_1, VP886_R_OPCOND_WRT, 1, &temp_val);		

	/* printing the SLIC state again*/		
	VpMpiCmd( devInfo->deviceNum, VP886_EC_1, VP886_R_OPCOND_RD, 1, res_arr );		
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "SLIC_SELF_TEST WRT VAL = 0x%x\nSLIC_SELF_TEST RD BACK = 0x%x\n", temp_val, res_arr[0]);
	if(res_arr[0] != temp_val){ 		
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING, "   !!! WRT RD ERROR !!!\n");		
	}
	else{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "SLIC WRT RD test Pass\n");
	}
	#endif

	devInfo->deviceNum += 1;
	devInfo->fxsNum += ZL88601_FXS_NUM;
	devInfo->fxoNum += ZL88601_FXO_NUM;
	devInfo->slicType = ZARLINK_88601;
	devInfo->pcm1ChNum += ZL88601_FXS_NUM+ZL88601_FXO_NUM;
	devInfo->pcm2ChNum += 0;
	return _TDI_SUCCESS;

}


/*_____________________________________________________________________________
 **      function name: zl88601_deviceInit
 **      descriptions:
 **            driver init
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	retVal
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
#if 1//def C7_DSP_SUPPORT//compile err
extern void VpSysSemaphoreInit(int semaphoreId);
#endif

int zl88601_deviceInit(char* buf){
	int                  devCnt;
	int                  lineCnt;
	unsigned char eventRetChan=0;
    VpStatusType          status;
    VpOptionCodecType     codecOption;
    VpOptionPulseModeType pulseMode;
    VpOptionTimeslotType  timeSlots;
    VpOptionEventMaskType eventMask;
    int timeSlotIdx;

    _TDI_initSetting_t* pinitSetting = ( _TDI_initSetting_t*)buf;
    devCnt = pinitSetting->chan;
    timeSlotIdx = devCnt;
	
        //dyma_20140218
	zl88601_Init_Line_Cmp[0] = _TDI_FALSE;
	zl88601_Init_Line_Cmp[1] = _TDI_FALSE;
	zl88601_Line_Cal_Cmp[0]	 = _TDI_FALSE;
	zl88601_Line_Cal_Cmp[1]	 = _TDI_FALSE;
	
	zl88601_Init_Cmp = _TDI_FALSE;
	zl88601_Init_Cnt = 0;
	
	atomic_set(&zl88601_init_ongoing,1); //dyma_20140218
	spin_lock_init(&zl88601_event_process_lock); /* added by mtk07059_20140717 */
	
	if(devCnt!=zl88601_DevPhyLocation){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "Bypass zl88601_deviceInit on Device %d!\n",devCnt);  //dyma_130118
		atomic_set(&zl88601_init_ongoing,0); //dyma_20140218
		return _TDI_SUCCESS;
	}

	VpSysSemaphoreInit(0);

	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "zl88601_deviceInit %d\n",devCnt);  //dyma_130118 

    /* Initialize the Device */
    status = VpMakeDeviceObject(VP_DEV_886_SERIES, devCnt, &devCtx_ptr, &devObj_ptr);
    if (VP_STATUS_SUCCESS != status) {
		
        SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "Error making device object: %s\n", MapStatus(status));
		atomic_set(&zl88601_init_ongoing,0); //dyma_20140218
        return _TDI_FAIL;
    }

    /* VP_TERM_FXS_GENERIC / VP_TERM_FXS_LOW_PWR / VP_TERM_FXS_SPLITTER_LP */
	for (lineCnt=0; lineCnt < ZL88601_CHAN_NUM; lineCnt++) {
		status = VpMakeLineObject(VP_TERM_FXS_GENERIC, lineCnt, &(lineCtx_ptr[lineCnt]), &(lineObj_ptr[lineCnt]), &devCtx_ptr);
		if (VP_STATUS_SUCCESS != status) {
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "Error making FXS line object 0, Status: %s\n", MapStatus(status));
			atomic_set(&zl88601_init_ongoing,0); //dyma_20140218
			return _TDI_FAIL;
		}
	}


    /*20110714 PTChen modified for slic country*/
    zl88601_slicParamReset(pinitSetting->hwProfId);
    status = VpInitDevice(&devCtx_ptr , DEVICE_PRO_zl88601, AC_PRO_FXS_zl88601, DC_PRO_FXS_zl88601, RINGING_PRO_FXS_zl88601, AC_PRO_FXO_zl88601, CONFIG_PRO_FXO_zl88601);
    if (VP_STATUS_SUCCESS != status) {
    	SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "Device not properly initialized, Status: %s\n", MapStatus(status));
		atomic_set(&zl88601_init_ongoing,0); //dyma_20140218
    	return _TDI_FAIL;
    }
	
	/* Do line init inside eventProcess */
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "zl88601 Init "); //dyma_20140903  

    while (zl88601_Init_Cmp != _TDI_TRUE) {
		zl88601_Init_Cnt++;
		if(zl88601_Init_Cnt>6000)
		{
			break;
		}
		if(zl88601_Init_Cnt%100==1)
		{
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "."); //dyma_20140903  
		}
		mdelay(10);
		zl88601_eventProcess((char *)&devCnt);
    }
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "\r\nzl88601_Init_Cnt = %d\n",zl88601_Init_Cnt);

	if(zl88601_Init_Cmp == FALSE)
	{
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR,"zl88601_Init_Cnt=%d, TIMEOUT !!! \n",zl88601_Init_Cnt);
		atomic_set(&zl88601_init_ongoing,0); //dyma_20140218
		return _TDI_FAIL;
	}

    /*
     * Set event mask for Legerity device.
     * Configure to detect ONHOOK and OFFHOOK only. TIC software will debounce
     * these signals, and detect FLASH. In this way, TIC has access to the
     * immediate state of the line, as required by the Caller ID Send (CIDS)
     * module.
     */
    /*
     * XXX This should be done by using VpGetOption() to unmask only the
     * options used by this application.
     * For an example, see qs_le71he0824_dat.c in
     * C:\Zarlink\VoicePath API-II 2.13.0\apps\qs_le71he0824_dat
     */

#if 0
	eventMask.faults = VP_EVCAT_FAULT_UNMASK_ALL;
    eventMask.signaling = VP_EVCAT_SIGNALING_UNMASK_ALL | VP_DEV_EVID_TS_ROLLOVER;
    eventMask.response = VP_EVCAT_RESPONSE_UNMASK_ALL;
    eventMask.test = VP_EVCAT_TEST_UNMASK_ALL;
    eventMask.process = VP_EVCAT_PROCESS_UNMASK_ALL;
#else
    eventMask.signaling = 0xffff;
    eventMask.signaling &= ~(VP_LINE_EVID_HOOK_OFF | VP_LINE_EVID_HOOK_ON);	/*SERENA_MODIFY*/
	eventMask.response &= ~(VP_EVID_CAL_CMP);
	eventMask.process &= ~(VP_LINE_EVID_SIGNAL_CMP);
#endif
//	eventMask.fxo &= ~(VP_LINE_EVID_RING_ON | VP_LINE_EVID_RING_OFF);
//	eventMask.fxo &= ~(VP_LINE_EVID_DISCONNECT | VP_LINE_EVID_RECONNECT);
//	eventMask.fxo &= ~(VP_LINE_EVID_FEED_DIS | VP_LINE_EVID_FEED_EN);
//	eventMask.fxo &= ~(VP_LINE_EVID_LIU | VP_LINE_EVID_LNIU);
//	eventMask.fxo &= ~VP_LINE_EVID_POLREV;

    if (VP_STATUS_SUCCESS != VpSetOption(VP_NULL, &devCtx_ptr,
            VP_OPTION_ID_EVENT_MASK, &eventMask)) {
    	dbg_printf("%s:%d VpSetOption: FXS_LINE VP_OPTION_ID_EVENT_MASK failed\n",
                __FILE__, __LINE__);
		atomic_set(&zl88601_init_ongoing,0); //dyma_20140218
        return _TDI_FAIL;
    }

	/*20110509 PTChen add for AC, now D2 use CODEC_LINEAR, AC use CODEC_ALAW*/
	if(pinitSetting->slicCodecOption == CODEC_LINEAR){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "CODEC_LINEAR setting\n");
		codecOption = VP_OPTION_LINEAR;
	}
	else if(pinitSetting->slicCodecOption == CODEC_ALAW){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "CODEC_ALAW setting\n");
		codecOption = VP_OPTION_ALAW;

	}
	else if(pinitSetting->slicCodecOption == CODEC_ULAW){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "CODEC_ULAW setting\n");
		codecOption = VP_OPTION_MLAW;

	}

	if (VP_STATUS_SUCCESS != VpSetOption(&(lineCtx_ptr[0]),NULL,
				VP_OPTION_ID_CODEC, &codecOption)) {
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, " failed to set codec type!\n");
		atomic_set(&zl88601_init_ongoing,0); //dyma_20140218
		return _TDI_FAIL;
	}

	if (VP_STATUS_SUCCESS != VpSetOption(&(lineCtx_ptr[1]),NULL,
				VP_OPTION_ID_CODEC, &codecOption)) {
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, " failed to set codec type!\n");
		atomic_set(&zl88601_init_ongoing,0); //dyma_20140218
		return _TDI_FAIL;
	}

    /*
     * Disable Pulse Dial Detection
     */
   // pulseMode = VP_OPTION_PULSE_DECODE_ON;
   // VpSetOption(VP_NULL, &devCtx_ptr, VP_OPTION_ID_PULSE_MODE, &pulseMode);

    /*
     * Set timeslot line option, and activate lines
     */
	/*20110509 PTChen add for 16 bit linear and 8 bit ALAW ULAW setting*/
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "FXS-0 timeslot:%d \n",timeSlotIdx);

	if(pinitSetting->slicCodecOption == CODEC_LINEAR){
		timeSlots.tx = ((timeSlotIdx) << 1);
		timeSlots.rx = ((timeSlotIdx) << 1);
	}
	else{
		timeSlots.tx = ((timeSlotIdx));
		timeSlots.rx = ((timeSlotIdx));
	}
	
	VpSetOption(&(lineCtx_ptr[0]), VP_NULL, VP_OPTION_ID_TIMESLOT,&timeSlots);

	timeSlotIdx += 1;
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "FXS-1 timeslot:%d \n",timeSlotIdx);

	if(pinitSetting->slicCodecOption == CODEC_LINEAR){
		timeSlots.tx = ((timeSlotIdx) << 1);
		timeSlots.rx = ((timeSlotIdx) << 1);
	}
	else{
		timeSlots.tx = ((timeSlotIdx));
		timeSlots.rx = ((timeSlotIdx));
	}
	VpSetOption(&(lineCtx_ptr[1]), VP_NULL, VP_OPTION_ID_TIMESLOT,&timeSlots);

	/*
	 * Adjust Tx/Rx relative gain levels.
	 */
	for (lineCnt=0; lineCnt < ZL88601_CHAN_NUM; lineCnt++) {
	    //VpSetLineState(&lineCtx_ptr[lineCnt], VP_LINE_ACTIVE);/*for COC by jq.zhu , OSBNB00030567, 20130327*/
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "fxs%d gain--TX :%x RX: %x \n",lineCnt,pinitSetting->speakSlicVal,pinitSetting->listenSlicVal);
		VpSetRelGain(&(lineCtx_ptr[lineCnt]),pinitSetting->speakSlicVal, pinitSetting->listenSlicVal, 0);
	}
	//VpSetRelGain(&(lineCtx_ptr[FXO_LINE]), 0x2013, 0x2013, 0);

    zl88601LineStat[0].fxsHookStat = TIC_IMMEDIATE_ONHOOK;
	zl88601LineStat[1].fxsHookStat = TIC_IMMEDIATE_ONHOOK;
	
	//dyma_20140428 metering
	for (lineCnt=0; lineCnt < ZL88601_CHAN_NUM; lineCnt++) {
		VpInitMeter(&(lineCtx_ptr[lineCnt]),METERING_PRO_zl88601);
	}
		
	/*loopback test*/
	#if 0  
	loopback = VP_OPTION_LB_TIMESLOT;//VP_OPTION_LB_DIGITAL
	status=VpSetOption(&(lineCtx_ptr[0]), VP_NULL, VP_OPTION_ID_LOOPBACK,&loopback);
	if (VP_STATUS_SUCCESS != status) {
		printk("loopback0 not properly initialized, Status: %s\n", MapStatus(status));		
	//	return _TDI_FAIL;
	}
	status=VpSetOption(&(lineCtx_ptr[1]), VP_NULL, VP_OPTION_ID_LOOPBACK,&loopback);
	if (VP_STATUS_SUCCESS != status) {
		printk("loopback1 not properly initialized, Status: %s\n", MapStatus(status));		
	//	return _TDI_FAIL;
	}	
	#endif
		
	atomic_set(&zl88601_init_ongoing,0); //dyma_20140218

    return _TDI_SUCCESS;
}


/*_____________________________________________________________________________
 **      function name: _TDI_shutdownHw
 **      descriptions:
 **            driver shutdown
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	void
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int zl88601_shutdownHw(char* buf){
    int    lineCnt;

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;

    dbg_printf("%s",__func__);
    for(lineCnt = 0; lineCnt < ZL88601_CHAN_NUM;lineCnt++){
		VpSetLineState(&lineCtx_ptr[lineCnt], VP_LINE_DISCONNECT);
		VpFreeLineCtx(&lineCtx_ptr[lineCnt]);
    }
    return _TDI_SUCCESS;
}
/*_____________________________________________________________________________
 **      function name: zl88601_fxsSetLineState
 **      descriptions:
 **            set ring state
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	void
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int zl88601_fxsSetLineState(char* buf){
    uint8 stateType = 0;
    _TDI_lineInfo_t* plineInfo = (_TDI_lineInfo_t*)buf;

	uint32 apiWaitCnt=0;

	if(atomic_read(&zl88601_init_ongoing))  //dyma_20140218
	{
		return _TDI_FAIL;
	}
	
    /*
     * Map to Legerity State Types
     */
    //dbg_printf("%s",__func__);

    switch (plineInfo->state) {
       /*
        * Codec deactivate
        */
        case TIC_STATE_POWER_DOWN:
            stateType = VP_LINE_DISCONNECT;
            break;
     /*
        * Codec activate or ring stop.
        */
        case TIC_STATE_ACTIVE:
			/* fall through */
        case TIC_STATE_RING_PAUSE:
            stateType = VP_LINE_TALK;
            break;
        case TIC_STATE_REV_OHT:
            stateType = VP_LINE_OHT_POLREV;
            break;
        case TIC_STATE_OHT:
			stateType = VP_LINE_OHT;
			break;
       /*
        * Start Ring
        */
        case TIC_STATE_RINGING:
			stateType = VP_LINE_RINGING;
            break;
	/*add for COC by mtk69086  #OSBNB00030711*/		
	case TIC_STATE_STAND_BY:
		stateType = VP_LINE_STANDBY;
		break;
	/*end of add for COC by mtk69086*/
        default:
            event_printf("%s:%d error, bad state! %d\n",
                    __FILE__, __LINE__,plineInfo->state);
            return _TDI_FAIL;
    }
    /*
     * Set Line State
     */
    //dbg_printf("---ch:%d status: %d \n",plineInfo->chan,stateType);
	
    VpSetLineState(&(lineCtx_ptr[plineInfo->chan]), stateType);

    return _TDI_SUCCESS;

}

/*_____________________________________________________________________________
 **      function name: zl88601_fxsGetLineState
 **      descriptions:
 **            set ring state
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	tic state
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int zl88601_fxsGetLineState(char* buf){
    VpLineStateType  state;
    _TDI_lineInfo_t* plineInfo = (_TDI_lineInfo_t*)buf;
	
	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
	{
		plineInfo->retTicStat = TIC_STATUS_ERROR;
		return _TDI_FAIL;
	}
	
    VpGetLineState(&(lineCtx_ptr[plineInfo->chan]), &state);
 	
    event_printf("%s - %d \n",__func__,state);
    switch (state) {
        case VP_LINE_DISCONNECT:
        	plineInfo->retTicStat = TIC_STATE_POWER_DOWN;
        	break;

        case VP_LINE_STANDBY:
            /* fall through */
        case VP_LINE_STANDBY_POLREV:
            /* fall through */
        case VP_LINE_ACTIVE:
            /* fall through */
        case VP_LINE_ACTIVE_POLREV:
            /* fall through */
        case VP_LINE_TALK:
            /* fall through */
        case VP_LINE_TALK_POLREV:
            /* fall through */
        case VP_LINE_OHT:
        	plineInfo->retTicStat = TIC_STATE_OHT;
        	break;
            /* fall through */
        case VP_LINE_OHT_POLREV:
        	plineInfo->retTicStat = TIC_STATE_REV_OHT;
        	break;

        case VP_LINE_RINGING:
            /* fall through */
        case VP_LINE_RINGING_POLREV:
        	plineInfo->retTicStat = TIC_STATE_RINGING;
        	break;

        case VP_LINE_TIP_OPEN:
        	plineInfo->retTicStat = TIC_STATE_GROUND_START;
            break;
        default:
        	plineInfo->retTicStat = TIC_STATUS_ERROR;
        	break;
    }
    return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: zl88601_fxsGetBatteryPol
 **      descriptions:
 **            set ring state
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	tic state
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int zl88601_fxsGetBatteryPol(char* buf){
    VpLineStateType  polarity;
    VpStatusType     status;
    _TDI_batteryPolInfo_t* pBatteryPolInfo = (_TDI_batteryPolInfo_t* )buf;

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
	{
		pBatteryPolInfo->retTicStat = TIC_STATUS_ERROR;
		return _TDI_FAIL;
	}

    event_printf("%s",__func__);
	
    status = VpGetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]), &polarity);
	
    if (VP_STATUS_SUCCESS != status) {
        dbg_printf("%s:%d Error!\n", __FILE__, __LINE__);
        return (-1);
    }

    switch (polarity) {
        case VP_LINE_ACTIVE:
            /* fall through */
        case VP_LINE_OHT:
            /* fall through */
        case VP_LINE_TALK:
            /* fall through */
        case VP_LINE_STANDBY:
            /* fall through */
        case VP_LINE_RINGING:
        	pBatteryPolInfo->retTicStat = TIC_BATT_POL_FORWARD;
            return _TDI_SUCCESS;
        case VP_LINE_ACTIVE_POLREV:
            /* fall through */
        case VP_LINE_OHT_POLREV:
            /* fall through */
        case VP_LINE_TALK_POLREV:
            /* fall through */
        case VP_LINE_STANDBY_POLREV:
            /* fall through */
        case VP_LINE_RINGING_POLREV:
        	pBatteryPolInfo->retTicStat = TIC_BATT_POL_REVERSE;
            return _TDI_SUCCESS;
        default:
            dbg_printf("%s:%d Bad polarity... %d\n",
                    __FILE__, __LINE__, polarity);
            pBatteryPolInfo->retTicStat = TIC_STATUS_ERROR;
            return _TDI_SUCCESS;
    }
}
/*_____________________________________________________________________________
 **      function name: zl88601_fxsSetBatteryPol
 **      descriptions:
 **            set battery polarity
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	tic state
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int zl88601_fxsSetBatteryPol(char* buf){
    VpLineStateType  curState;
    VpStatusType     getStatus;
    VpStatusType     setStatus;
    _TDI_batteryPolInfo_t* pBatteryPolInfo = (_TDI_batteryPolInfo_t* )buf;

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
	{
		return _TDI_FAIL;
	}

	event_printf("%s",__func__);
	
    getStatus = VpGetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]), &curState);
	
    if (VP_STATUS_SUCCESS != getStatus) {
        dbg_printf("%s:%d Error!\n", __FILE__, __LINE__);
        return _TDI_FAIL;
    }

    switch (curState) {
        case VP_LINE_ACTIVE:
            /* fall through */
        case VP_LINE_ACTIVE_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]), VP_LINE_ACTIVE);
            }
            else {
                setStatus = VpSetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]),
                        VP_LINE_ACTIVE_POLREV);
            }
            break;
        case VP_LINE_OHT:
            /* fall through */
        case VP_LINE_OHT_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]), VP_LINE_OHT);
            }
            else {
                setStatus = VpSetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]),
                        VP_LINE_OHT_POLREV);
            }
            break;
        case VP_LINE_STANDBY:
            /* fall through */
        case VP_LINE_STANDBY_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]), VP_LINE_STANDBY);
            }
            else {
                setStatus = VpSetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]),
                        VP_LINE_STANDBY_POLREV);
            }
            break;
        case VP_LINE_RINGING:
            /* fall through */
        case VP_LINE_RINGING_POLREV:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]), VP_LINE_RINGING);
            }
            else {
                setStatus = VpSetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]),
                        VP_LINE_RINGING_POLREV);
            }
            break;

        case VP_LINE_TALK:
            /* fall through */
        case VP_LINE_TALK_POLREV:
            /* fall through */
        default:
            if (TIC_BATT_POL_FORWARD == pBatteryPolInfo->polarity) {
                setStatus = VpSetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]), VP_LINE_TALK);
            }
            else {
                setStatus = VpSetLineState(&(lineCtx_ptr[pBatteryPolInfo->chan]),
                        VP_LINE_TALK_POLREV);
            }
            break;
	}

	if (VP_STATUS_SUCCESS != setStatus) {
        dbg_printf("%s:%d Error!\n", __FILE__, __LINE__);
        return _TDI_FAIL;
    }
    return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: zl88601_fxsGetHookState
 **      descriptions:
 **            get hook polarity
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	tic state
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int zl88601_fxsGetHookState(char* buf){
	_TDI_hookStat_t* pHookStat = (_TDI_hookStat_t*)buf;

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
	{
		return _TDI_FAIL;
	}

	//dbg_printf("%s",__func__);
	pHookStat->hookStat = zl88601LineStat[pHookStat->chan].fxsHookStat;
	return _TDI_SUCCESS;
}

/*_____________________________________________________________________________
 **      function name: zl88601_ringState
 **      descriptions:
 **            get ring state
 **      parameters:
 **				void
 **      global:
 **
 **      return:
 **	      	ring state
 **      call:
 **      revision:
 **      		1.1 2010/11/10 17:00  pork
 **____________________________________________________________________________
 */
int zl88601_ringState(char* buf){
    int state;
    int count;
    int ret;
    _TDI_ringSetting_t *pRingSetting = (_TDI_ringSetting_t *) buf;
	static unsigned long pre_jiffies;

    state = pRingSetting->state;
    count = pRingSetting->count;
    ret   = _TDI_FAIL;

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
		return (ret);

    event_printf("%s---%d %d",__func__,state,pRingSetting->count);
    /*
     * Return immediately if done.
     */
    if (state == _TDI_RING_STATE_DONE) {
        return (ret);
    }

ZL88601_RING_STATE_RUN:

    /*
     * State machine
     */
    switch (state) {

        /*
         * Init state. When new ring starts. Always transitions to make1 state.
         */
        case _TDI_RING_STATE_INIT:
            count = pRingSetting->make1s;
            state = _TDI_RING_STATE_MAK1;
            ret   = _TDI_SUCCESS;
			pre_jiffies=jiffies;   //dyma_140619
			
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"\n|chan=%d,state=%d,count=%d\n|make=%d	%d	%d\n|break=%d	%d	%d\n|rep=%d,numRing=%d\n",
			pRingSetting->chan,
			pRingSetting->state,
			pRingSetting->count,
			pRingSetting->make1s,
			pRingSetting->make2s,
			pRingSetting->make3s,
			pRingSetting->break1s,
			pRingSetting->break2s,
			pRingSetting->break3s,
			pRingSetting->rep,
			pRingSetting->numRings);

        /*
         * Make state always transitions to break state.
         */
        case _TDI_RING_STATE_MAK1:
            /*
             * If time is negative, generate forever
             */
        	//dbg_printf("%s---%d %d",__func__,state,pRingSetting->make1s);
            if (pRingSetting->make1s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->break1s;
            state = _TDI_RING_STATE_BRK1;
            ret   = _TDI_SUCCESS ;

        /*
         * Break state always transitions to make state
         */
        case _TDI_RING_STATE_BRK1:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->break1s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->make2s;
            state = _TDI_RING_STATE_MAK2;
            ret   = _TDI_SUCCESS;

        /*
         * Make state always transitions to break state.
         */
        case _TDI_RING_STATE_MAK2:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->make2s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->break2s;
            state = _TDI_RING_STATE_BRK2;
            ret   = _TDI_SUCCESS ;

        /*
         * Break state always transitions to make state
         */
        case _TDI_RING_STATE_BRK2:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->break2s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->make3s;
            state = _TDI_RING_STATE_MAK3;
            ret   = _TDI_SUCCESS ;

        /*
         * Make state always transitions to break state.
         */
        case _TDI_RING_STATE_MAK3:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->make3s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            count = pRingSetting->break3s;
            state = _TDI_RING_STATE_BRK3;
            ret   = _TDI_SUCCESS ;

        /*
         * Break state always transitions to make state
         */
        case _TDI_RING_STATE_BRK3:
            /*
             * If time is negative, generate forever
             */
            if (pRingSetting->break3s < 0) {
                count = _TDI_RING_INFINITY;
                break;
            }

            /*
             * Not finished with this state.
             */
            if (count > 0) {
                break;
            }

            /*
             * Finsihed with this state, go to next state.
             */
            pRingSetting->numRings++;

            /*
             * Check number of repeats.
             *  <  0 means infinite repeats.
             *  >  0 means repeats not done.
             *  == 0 means repeats done.
             */
            count = pRingSetting->make1s;
            state = _TDI_RING_STATE_MAK1;
            ret   = _TDI_SUCCESS ;
            if (pRingSetting->rep < 0) {
                /*
                 * Run state machine again.
                 */
                goto ZL88601_RING_STATE_RUN;
            }
            else if (pRingSetting->rep > 0) {
                /*
                 * Run state machine again.
                 */
                pRingSetting->rep--;
                goto ZL88601_RING_STATE_RUN;
            }

        default:
            count = _TDI_RING_INFINITY;
            state = _TDI_RING_STATE_DONE;
            ret   = _TDI_SUCCESS ;
    }

    count=count-((int)(jiffies-pre_jiffies)>0?((int)(jiffies-pre_jiffies)):1); //dyma_140619
	pre_jiffies=jiffies;   //dyma_140619
    pRingSetting->state = state;
    pRingSetting->count = count;
    event_printf("over: %s---%d %d %d",__func__,state,pRingSetting->count,ret);

    return (ret);

}
int zl88601_fxsGetPulseStatus(char* buf){

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;

	dbg_printf("%s",__func__);
	return _TDI_SUCCESS;
}

int zl88601_fxsSetGainLv(char* buf){
	_TDI_gainSetting_t* gainSetting = (_TDI_gainSetting_t *)buf;
	VpStatusType gainSetResult;

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;

	gainSetResult = VpSetRelGain(&lineCtx_ptr[gainSetting->chan],gainSetting->speakSlicVal,gainSetting->listenSlicVal, 0);

	dbg_printf("%s - %x / %x \n",__func__,gainSetting->speakSlicVal,gainSetting->listenSlicVal);

	if(gainSetResult != VP_STATUS_SUCCESS){
		return _TDI_FAIL;
	}

	return _TDI_SUCCESS;
}
int zl88601_phoneRingStart(char* buf){
	//int channel = *(int*) buf;

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;

	dbg_printf("%s",__func__);
	return _TDI_SUCCESS;
}

int zl88601_phoneRingStop(char* buf){
	//int channel = *(int*) buf;

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;

	dbg_printf("%s",__func__);
	return _TDI_SUCCESS;
}

int zl88601_eventProcess(char *buf){
    VpDevCtxType *legerityDev_ptr;
    bool          legerityEventFlag = FALSE;
//    bool          legerityEventStatus = FALSE;
//    int          chan;
	int devCnt = *(int*)buf;
//	int chan=0;
    uint16        mpiData;      /* Workaround for VE890 delayed on-hook event */
    uint8         ecVal[] =  { VP886_R_EC_EC1, VP886_R_EC_EC2 };
	uint8 lineNum = 0;
	static uint8 cmpCounter = 0;
	VpStatusType status;
	VpOptionEventMaskType eventMask;
	
	if(devCnt!=zl88601_DevPhyLocation){
		//printk("Bypass zl88601_eventProcess on Device %d!\n",devCnt);  //dyma_130118
		return _TDI_SUCCESS;
	}

	spin_lock_irqsave(&zl88601_event_process_lock, zl88601_event_process_flags); //dyma_20140218

    /*
     * Call the VP API tick
     * and then get status.
     */
    legerityDev_ptr = &devCtx_ptr;
	ret_event_process_chan_id=0;
    //VpApiTick(legerityDev_ptr, &legerityEventFlag);

    /*
     * If the legerity EventFlag is active, then parse the legerityEventObj
     * for the events of interest to this TIC driver.
     * For now, do some error checking, to help debug problems.
     */

    // if (TRUE == legerityEventFlag) {
    	//event_printf("||||%d \n",devCnt);

    if(VpGetEvent(legerityDev_ptr,&zl88601legerityEventObj)){
		
		ret_event_process_chan_id = zl88601legerityEventObj.channelId;
	
        //if ((TRUE == legerityEventStatus) && (VP_STATUS_SUCCESS == zl88601legerityEventObj[devCnt].status)) {
        if ((VP_STATUS_SUCCESS == zl88601legerityEventObj.status)
			&&(VP_STATUS_SUCCESS==zl88601legerityEventObj.eventData)){
			
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"Chk[%d]:Dev[%d]chan[%d][%d]:st0=%d,ed0=%d: Cat=0x%x Id=0x%x\n",
			devCnt,
			zl88601legerityEventObj.deviceId,
			zl88601legerityEventObj.channelId,
			ret_event_process_chan_id,
			zl88601legerityEventObj.status,
			zl88601legerityEventObj.eventData,
			zl88601legerityEventObj.eventCategory,
			zl88601legerityEventObj.eventId); 
			
            /*
             * Valid Status received from Legerity,
             * check which line has status, and do some error checking.
             */

            /*if (_TDI_FXS_NUM < chan) {
                dbg_printf("%s: %d Legerity line number out of range %d\n",
                        __FILE__, __LINE__, chan);
                return _TDI_FAIL;
            }*/

            if (VP_EVCAT_RESPONSE == zl88601legerityEventObj.eventCategory) {
                switch (zl88601legerityEventObj.eventId) {
                    case VP_DEV_EVID_DEV_INIT_CMP:
						SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"Dev[%d] Init Cmp!\n",devCnt);

					    /* mask everything */
					    VpMemSet(&eventMask, 0xFF, sizeof(VpOptionEventMaskType));

					    /* unmask only the events the application is interested in */
					    eventMask.faults = VP_EVCAT_FAULT_UNMASK_ALL;
					    eventMask.signaling = VP_EVCAT_SIGNALING_UNMASK_ALL | VP_DEV_EVID_TS_ROLLOVER;
					    eventMask.response = VP_EVCAT_RESPONSE_UNMASK_ALL;
					    eventMask.test = VP_EVCAT_TEST_UNMASK_ALL;
					    eventMask.process = VP_EVCAT_PROCESS_UNMASK_ALL;

					    /* inform the API of the mask */
					    VpSetOption(VP_NULL, &devCtx_ptr, VP_OPTION_ID_EVENT_MASK, &eventMask);

						cmpCounter=0;  //dyma_20140218

		                for (lineNum=0; lineNum < ZL88601_CHAN_NUM; lineNum++) {   //line init
		                    status = VpSetLineState(&lineCtx_ptr[lineNum], VP_LINE_STANDBY);
							SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"Start Line Standby on lineNum %d\n", lineNum);
		                    if (status != VP_STATUS_SUCCESS) {
								SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR,"Error setting lineNum %i to VP_LINE_STANDBY: %s\n", lineNum, MapStatus(status));
		                    }

		                    status = VpCalLine(&lineCtx_ptr[lineNum]);
							SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"Start Calibration on lineNum %d\n", lineNum);
		                    if (status != VP_STATUS_SUCCESS) {
		                        SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR,"Error calling VpCalLine on lineNum %i:: %s\n", lineNum, MapStatus(status));
		                    }
		                }
						break;

					case VP_EVID_CAL_CMP:
						cmpCounter++;  //dyma_20140218
						zl88601_Line_Cal_Cmp[ret_event_process_chan_id] = _TDI_TRUE;
						SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"Calibration done on lineNum %d (%d/%d)\n", ret_event_process_chan_id,cmpCounter,ZL88601_CHAN_NUM);
						if (cmpCounter == ZL88601_CHAN_NUM) {
							/* Device Initialization is complete */
							zl88601_Init_Cmp = _TDI_TRUE;
							SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, "Calibration completed on both lines\n\n");
						}
						break;

                    case VP_LINE_EVID_RD_OPTION:
						SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"VP_LINE_EVID_RD_OPTION!\n");
                        /* Read the Event Mask */
                        zl88601_Get_Mask_Cmp = TRUE;
                        break;
						
                    case VP_LINE_EVID_LINE_INIT_CMP:
						SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"Line[%d] Init Cmp!\n",ret_event_process_chan_id);
                    	zl88601_Init_Line_Cmp[ret_event_process_chan_id] = TRUE;
                    	break;
						
                    case VP_EVID_CAL_BUSY:
                        /* Line Calibration failed */
                        SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, " Legerity line calibration Failed %d\n",ret_event_process_chan_id);
                        break;
						
                    default:
                        /* Do nothing */
                        break;
                }
            }
            else if (VP_EVCAT_SIGNALING == zl88601legerityEventObj.eventCategory) {
                switch (zl88601legerityEventObj.eventId) {
                    case VP_LINE_EVID_HOOK_OFF:
						SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"chan %d HOOK_OFF\n\n",ret_event_process_chan_id);
                    	zl88601LineStat[ret_event_process_chan_id].fxsHookStat = TIC_IMMEDIATE_OFFHOOK;
                        break;
                    case VP_LINE_EVID_HOOK_ON:
						SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG,"chan %d HOOK_ON\n\n",ret_event_process_chan_id);
                    	zl88601LineStat[ret_event_process_chan_id].fxsHookStat = TIC_IMMEDIATE_ONHOOK;
                        break;
                    default:
                        /* Do nothing */
                        break;
                }
            }
            else if(VP_EVCAT_FXO == zl88601legerityEventObj.eventCategory){
				SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING,"#zl88601 not support FXO\n\n");
            }
        }
		else{
			SLIC_DBG_MSG(_SLIC_DBG_TYPE_WARNING,"#EventErrorDueTo:st0=%d,ed0=%d!Chk[%d]:Dev[%d]chan[%d][%d]:Cat=0x%x Id=0x%x\n",
			zl88601legerityEventObj.status,
			zl88601legerityEventObj.eventData,
			devCnt,
			zl88601legerityEventObj.deviceId,
			zl88601legerityEventObj.channelId,
			ret_event_process_chan_id,
			zl88601legerityEventObj.eventCategory,
			zl88601legerityEventObj.eventId);
		}
    }

	spin_unlock_irqrestore(&zl88601_event_process_lock, zl88601_event_process_flags); //dyma_20140218

    /*
     *  Workaround for VE890 delayed on-hook event.
     *  Check the 0x4D Signaling Register directly to determine on-hook event.
     */

    /*VpMpiCmdWrapper(0, 2, 0x60,0x1, 0xbf);
     * VpMpiCmdWrapper(0, 1, 0x60,0x1, 0xbf);*/
	/*VpMpiCmd(devCnt, VP890_EC_CH1, VP890_NO_UL_SIGREG_RD,
					VP890_NO_UL_SIGREG_LEN, &mpiData);
	if (_TDI_VP890_SIGREG_LIU & mpiData ) {     //off-hook state
		if (zl88601LineStat.fxsHookStat != TIC_IMMEDIATE_OFFHOOK) {
			zl88601LineStat.fxsHookStat  = TIC_IMMEDIATE_OFFHOOK;
		}
		else { }
	} else {                                    // on-hook state
		if (zl88601LineStat.fxsHookStat != TIC_IMMEDIATE_ONHOOK) {
			zl88601LineStat.fxsHookStat  = TIC_IMMEDIATE_ONHOOK;
		}
	}*/
	/*VpMpiCmdWrapper(0, 0x2, 0x60,0x1, &testRMpiData);
	if(testRMpiData != 0xb7 || testRMpiData != 0xbf){
		testWMpiData = 0xbf;
		VpMpiCmdWrapper(0, 0x1, 0x60,0x1, &testWMpiData);
		testWMpiData = 0xb7;
		VpMpiCmdWrapper(0, 0x2, 0x60,0x1, &testWMpiData);
	}*/
	//VpMpiCmdWrapper(0, 0x3, 0x60,0x1, &testRMpiData);
	//printk("mpi data:%x \n",testRMpiData);

	return _TDI_SUCCESS;
}

int zl88601_dumpregister(char* buf){

	#define PRINTK_BUF_SIZE 	 1024	// It is the static buf size in function vprintk() which called by printk() of /linux/kernel/printk.c
	#define DUMP_PRINTK_BUF_NUM 8
	#define DUMP_PRINTK_BUF_SIZE (PRINTK_BUF_SIZE*DUMP_PRINTK_BUF_NUM)

	Vp886DeviceObjectType *pDevObj = devCtx_ptr.pDevObj;
	uint8 channelId, registerIndex, registerNumber;
	uint32 apiWaitCnt=0;
	uint8 dumpPrintkBufCnt=0;
	char *print_buffer= MALLOC(DUMP_PRINTK_BUF_SIZE);
	int print_buffer_len=0;

	//dbg_printf("%s \n",__func__);
	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;
	

	  /* Sufficient size to print all reg data */
	if(print_buffer==NULL){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "print_buffer alloc fail\r\n");
		return _TDI_FAIL;
	}
	else{
		memset(print_buffer,0,DUMP_PRINTK_BUF_SIZE);
	}

	 /* Sufficient size to hold a single MPI Read is all that is needed. */
	 uint8 registerBuffer[25];

	 /* Define buffers containing MPI read commands with lengths */
 	 #define VP886_DEVICE_REGISTER_COUNT	12  //bypass dump on event process
	 uint8 deviceRegs[VP886_DEVICE_REGISTER_COUNT][2] = {
		 {VP886_R_RCNPCN_RD, VP886_R_RCNPCN_LEN},
		 {VP886_R_DEVCFG_RD, VP886_R_DEVCFG_LEN},
		 {VP886_R_DEVMODE_RD, VP886_R_DEVMODE_LEN},
		 {VP886_R_CLKSLOTS_RD, VP886_R_CLKSLOTS_LEN},
		 {VP886_R_INTMASK_RD, VP886_R_INTMASK_LEN},
		 {VP886_R_SIGREG_NO_UL_RD, VP886_R_SIGREG_LEN},  //bypass dump on event process
		 {VP886_R_SWCTRL_RD, VP886_R_SWCTRL_LEN},
		 {VP886_R_SWTIMING_RD, VP886_R_SWTIMING_LEN},
		 {VP886_R_GTIMER_RD, VP886_R_GTIMER_LEN},
		 {VP886_R_TIMESTAMP_RD, VP886_R_TIMESTAMP_LEN},
		 {VP886_R_CPCYCLES_RD, VP886_R_CPCYCLES_LEN},
		 {VP886_R_EC_RD, VP886_R_EC_LEN},
	 };
	
	 char *deviceRegsName[VP886_DEVICE_REGISTER_COUNT] = {
		 "VP886_R_RCNPCN_RD",		 "VP886_R_DEVCFG_RD",
		 "VP886_R_DEVMODE_RD",		 "VP886_R_CLKSLOTS_RD",
		 "VP886_R_INTMASK_RD",		 "VP886_R_SIGREG_NO_UL_RD",   //bypass dump on event process
		 "VP886_R_SWCTRL_RD",		 "VP886_R_SWTIMING_RD",
		 "VP886_R_GTIMER_RD",		 "VP886_R_TIMESTAMP_RD",
		 "VP886_R_CPCYCLES_RD", 	 "VP886_R_EC_RD"
	 };
	
 	#define VP886_CHANNEL_REGISTER_COUNT	 56
	 uint8 channelRegs[VP886_CHANNEL_REGISTER_COUNT][2] = {
		 {VP886_R_TXSLOT_RD, VP886_R_TXSLOT_LEN},
		 {VP886_R_RXSLOT_RD, VP886_R_RXSLOT_LEN},
		 {VP886_R_VPGAIN_RD, VP886_R_VPGAIN_LEN},
		 {VP886_R_IODATA_RD, VP886_R_IODATA_LEN},
		 {VP886_R_IODIR_RD, VP886_R_IODIR_LEN},
		 {VP886_R_STATE_RD, VP886_R_STATE_LEN},
		 {VP886_R_SW_OC_RD, VP886_R_SW_OC_LEN},
		 {VP886_R_HOOKFREEZE_RD, VP886_R_HOOKFREEZE_LEN},
		 {VP886_R_OPFUNC_RD, VP886_R_OPFUNC_LEN},
		 {VP886_R_SSCFG_RD, VP886_R_SSCFG_LEN},
		 {VP886_R_OPCOND_RD, VP886_R_OPCOND_LEN},
		 {VP886_R_GX_RD, VP886_R_GX_LEN},
		 {VP886_R_GR_RD, VP886_R_GR_LEN},
		 {VP886_R_B_FIR_FILT_RD, VP886_R_B_FIR_FILT_LEN},
		 {VP886_R_X_FILT_RD, VP886_R_X_FILT_LEN},
		 {VP886_R_R_FILT_RD, VP886_R_R_FILT_LEN},
		 {VP886_R_B_IIR_FILT_RD, VP886_R_B_IIR_FILT_LEN},
		 {VP886_R_Z_FIR_FILT_RD, VP886_R_Z_FIR_FILT_LEN},
		 {VP886_R_Z_IIR_FILT_RD, VP886_R_Z_IIR_FILT_LEN},
		 {VP886_R_SADC_RD, VP886_R_SADC_LEN},
		 {VP886_R_VADC_RD, VP886_R_VADC_LEN},
		 {VP886_R_DCANCEL_RD, VP886_R_DCANCEL_LEN},
		 {VP886_R_CALCTRL_RD, VP886_R_CALCTRL_LEN},
		 {VP886_R_INDCAL_RD, VP886_R_INDCAL_LEN},
		 {VP886_R_NORMCAL_RD, VP886_R_NORMCAL_LEN},
		 {VP886_R_REVCAL_RD, VP886_R_REVCAL_LEN},
		 {VP886_R_RINGCAL_RD, VP886_R_RINGCAL_LEN},
		 {VP886_R_RINGGEN_RD, VP886_R_RINGGEN_LEN},
		 {VP886_R_LOOPSUP_RD, VP886_R_LOOPSUP_LEN},
		 {VP886_R_DCFEED_RD, VP886_R_DCFEED_LEN},
		 {VP886_R_HOOKBUF_RD, VP886_R_HOOKBUF_LEN},
		 {VP886_R_DISN_RD, VP886_R_DISN_LEN},
		 {VP886_R_VBUFFER_RD, VP886_R_VBUFFER_LEN},
		 {VP886_R_METER_RD, VP886_R_METER_LEN},
		 {VP886_R_SIGAB_RD, VP886_R_SIGAB_LEN},
		 {VP886_R_SIGCD_RD, VP886_R_SIGCD_LEN},
		 {VP886_R_CHTIMER_RD, VP886_R_CHTIMER_LEN},
		 {VP886_R_SIGCTRL_RD, VP886_R_SIGCTRL_LEN},
		 {VP886_R_CADENCE_RD, VP886_R_CADENCE_LEN},
		 {VP886_R_CIDDATA_RD, VP886_R_CIDDATA_LEN},
		 {VP886_R_SWPARAM_RD, VP886_R_SWPARAM_LEN},
		 {VP886_R_BATCAL_RD, VP886_R_BATCAL_LEN},
		 {VP886_R_CIDPARAM_RD, VP886_R_CIDPARAM_LEN},
		 {VP886_R_ICR1_RD, VP886_R_ICR1_LEN},
		 {VP886_R_ICR2_RD, VP886_R_ICR2_LEN},
		 {VP886_R_ICR3_RD, VP886_R_ICR3_LEN},
		 {VP886_R_ICR4_RD, VP886_R_ICR4_LEN},
		 {VP886_R_ICR5_RD, VP886_R_ICR5_LEN},
		 {VP886_R_ICR6_RD, VP886_R_ICR6_LEN},
		 {VP886_R_SW_ON_TIME_RD, VP886_R_SW_ON_TIME_LEN},
		 {VP886_R_B1_RD, VP886_R_B1_LEN},
		 {VP886_R_B2_RD, VP886_R_B2_LEN},
		 {VP886_R_B3_RD, VP886_R_B3_LEN},
		 {VP886_R_B4_RD, VP886_R_B4_LEN},
		 {VP886_R_B5_RD, VP886_R_B5_LEN},
		 {VP886_R_B6_RD, VP886_R_B6_LEN}
	 };
	
	 char *registerName[VP886_CHANNEL_REGISTER_COUNT] = {
		 "VP886_R_TXSLOT_RD",		 "VP886_R_RXSLOT_RD",
		 "VP886_R_VPGAIN_RD",		 "VP886_R_IODATA_RD",
		 "VP886_R_IODIR_RD",		 "VP886_R_STATE_RD",
		 "VP886_R_SW_OC_RD",		 "VP886_R_HOOKFREEZE_RD",
		 "VP886_R_OPFUNC_RD",		 "VP886_R_SSCFG_RD",
		 "VP886_R_OPCOND_RD",		 "VP886_R_GX_RD",
		 "VP886_R_GR_RD",			 "VP886_R_B_FIR_FILT_RD",
		 "VP886_R_X_FILT_RD",		 "VP886_R_R_FILT_RD",
		 "VP886_R_B_IIR_FILT_RD",	 "VP886_R_Z_FIR_FILT_RD",
		 "VP886_R_Z_IIR_FILT_RD",	 "VP886_R_SADC_RD",
		 "VP886_R_VADC_RD", 		 "VP886_R_DCANCEL_RD",
		 "VP886_R_CALCTRL_RD",		 "VP886_R_INDCAL_RD",
		 "VP886_R_NORMCAL_RD",		 "VP886_R_REVCAL_RD",
		 "VP886_R_RINGCAL_RD",		 "VP886_R_RINGGEN_RD",
		 "VP886_R_LOOPSUP_RD",		 "VP886_R_DCFEED_RD",
		 "VP886_R_HOOKBUF_RD",		 "VP886_R_DISN_RD",
		 "VP886_R_VBUFFER_RD",		 "VP886_R_METER_RD",
		 "VP886_R_SIGAB_RD",		 "VP886_R_SIGCD_RD",
		 "VP886_R_CHTIMER_RD",		 "VP886_R_SIGCTRL_RD",
		 "VP886_R_CADENCE_RD",		 "VP886_R_CIDDATA_RD",
		 "VP886_R_SWPARAM_RD",		 "VP886_R_BATCAL_RD",
		 "VP886_R_CIDPARAM_RD", 	 "VP886_R_ICR1_RD",
		 "VP886_R_ICR2_RD", 		 "VP886_R_ICR3_RD",
		 "VP886_R_ICR4_RD", 		 "VP886_R_ICR5_RD",
		 "VP886_R_ICR6_RD", 		 "VP886_R_SW_ON_TIME_RD",
		 "VP886_R_B1_RD",			 "VP886_R_B2_RD",
		 "VP886_R_B3_RD",			 "VP886_R_B4_RD",
		 "VP886_R_B5_RD",			 "VP886_R_B6_RD"
	 };
	
	 /* Read all of the Global registers and print their contents */
	 print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\n\rZL88601DEVICE");

	 for (registerNumber = 0; registerNumber < VP886_DEVICE_REGISTER_COUNT; registerNumber++) {

		 VpSlacRegRead(&devCtx_ptr, NULL, deviceRegs[registerNumber][0], deviceRegs[registerNumber][1], registerBuffer);

		 print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\n\r%s (0x%02X) ",deviceRegsName[registerNumber], deviceRegs[registerNumber][0]);

		 for (registerIndex = 0;
			  registerIndex < deviceRegs[registerNumber][1];
			  registerIndex++) {
			 print_buffer_len+=sprintf(print_buffer+print_buffer_len, "0x%02X ", registerBuffer[registerIndex]);

		 }
	 }

	 /* Read all of the channel specific registers and print their contents */
	 #if 1
	 for (channelId = 0; channelId < pDevObj->staticInfo.maxChannels; channelId++) {
		 VpLineCtxType *pLineCtx = devCtx_ptr.pLineCtx[channelId];
		 print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\n\n\rCHANNEL %d", channelId);

		 for (registerNumber = 0; registerNumber < VP886_CHANNEL_REGISTER_COUNT; registerNumber++) {

			 VpSlacRegRead(NULL, pLineCtx, channelRegs[registerNumber][0], channelRegs[registerNumber][1], registerBuffer);

			 print_buffer_len+=sprintf(print_buffer+print_buffer_len, "\n\r%s (0x%02X) ",registerName[registerNumber], channelRegs[registerNumber][0]);

			 for (registerIndex = 0;
				  registerIndex < channelRegs[registerNumber][1];
				  registerIndex++) {
				 print_buffer_len+=sprintf(print_buffer+print_buffer_len,"0x%02X ", registerBuffer[registerIndex]);

			 }
		 }
	 }
	#else
	channelId=0;

 	 VpLineCtxType *pLineCtx = devCtx_ptr.pLineCtx[channelId];
	 print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\n\n\rCHANNEL %d", channelId);

	 for (registerNumber = 0; registerNumber < VP886_CHANNEL_REGISTER_COUNT; registerNumber++) {

		 VpSlacRegRead(NULL, pLineCtx, channelRegs[registerNumber][0], channelRegs[registerNumber][1], registerBuffer);

		 print_buffer_len+=sprintf(print_buffer+print_buffer_len, "\n\r%s (0x%02X) ",registerName[registerNumber], channelRegs[registerNumber][0]);

		 for (registerIndex = 0;
			  registerIndex < channelRegs[registerNumber][1];
			  registerIndex++) {
			 print_buffer_len+=sprintf(print_buffer+print_buffer_len,"0x%02X ", registerBuffer[registerIndex]);

		 }
	 }
	 channelId=1;

	 pLineCtx = devCtx_ptr.pLineCtx[channelId];
	 print_buffer_len+=sprintf(print_buffer+print_buffer_len,"\n\n\rCHANNEL %d", channelId);
	 
	 for (registerNumber = 0; registerNumber < VP886_CHANNEL_REGISTER_COUNT; registerNumber++) {
	 
		 VpSlacRegRead(NULL, pLineCtx, channelRegs[registerNumber][0], channelRegs[registerNumber][1], registerBuffer);
	 
		 print_buffer_len+=sprintf(print_buffer+print_buffer_len, "\n\r%s (0x%02X) ",registerName[registerNumber], channelRegs[registerNumber][0]);
	 
		 for (registerIndex = 0;
			  registerIndex < channelRegs[registerNumber][1];
			  registerIndex++) {
			 print_buffer_len+=sprintf(print_buffer+print_buffer_len,"0x%02X ", registerBuffer[registerIndex]);
	 
		 }
	 }
	#endif
		 
	 print_buffer_len+=sprintf(print_buffer+print_buffer_len, "\r\n");

	 if(print_buffer_len>=DUMP_PRINTK_BUF_SIZE){
	 	printk("Error! buf overflow! print_buffer_len=%d > bufSize=%d \r\n",print_buffer_len,PRINTK_BUF_SIZE);
	 	// print_buffer_len+=sprintf(print_buffer+print_buffer_len,
	 }
	 for(dumpPrintkBufCnt=0;dumpPrintkBufCnt<DUMP_PRINTK_BUF_NUM;dumpPrintkBufCnt++){
		 printk(print_buffer+dumpPrintkBufCnt*PRINTK_BUF_SIZE);
     }

	 if(print_buffer){
	 	FREE(print_buffer);
		print_buffer=NULL;
	 }

	return _TDI_SUCCESS;
}
//Roger_lt
int zl88601_LtTestStart(char* buf) {
	_TDI_devicelinetest_t* linetest=(_TDI_devicelinetest_t*)buf;

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;

	if(linetest->chan<ZL88601_CHAN_NUM)	
		return LtTestStart(&lineCtx_ptr[linetest->chan],&devCtx_ptr,buf);
	else
		return _TDI_FALSE;
}


int* zl88601_getLtTestResult(void)
{

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;

	return getLtTestResult();
}

int zl88601_fxsSetMetering(char* buf) //dyma_20140428 metering
{
	_TDI_fxsSetMetering_t* meteringSetting=(_TDI_fxsSetMetering_t*)buf;

	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;

	/* VpStartMeter()
	  Implements VpStartMeter() to begin a metering signal a line or stop metering
	  
	  If numMeters is 0, abort any ongoing metering
	  Any currently running metering sequence must be aborted to start the new one 
	  If onTime is 0, turn metering on indefinitely
	  Otherwise, start metering by using the timer handler
	  
	  See the VP-API-II Reference Guide for more details on VpStartMeter().
	*/
	VpStartMeter(&(lineCtx_ptr[meteringSetting->chan]),meteringSetting->onTime,meteringSetting->offTime,meteringSetting->numMeters);

	return _TDI_SUCCESS;
}

/*[OSBNB00041138] ADD by peter.jiang@20141126, support dsp loopback*/
int zl88601_SetLoopback(char *buf)
{
	_TDI_fxsSetLoopback_t* loopbackSetting=(_TDI_fxsSetLoopback_t*)buf;
	VpOptionLoopbackType loopBack;
	
	if(atomic_read(&zl88601_init_ongoing))	//dyma_20140218
		return _TDI_FAIL;
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_DEBUG, " %s loopback\n", loopbackSetting->enable?"Enable":"Disable");

	if (loopbackSetting->enable)
		loopBack = VP_OPTION_LB_TIMESLOT;
	else
		loopBack = VP_OPTION_LB_OFF;
	if (VP_STATUS_SUCCESS != VpSetOption(&(lineCtx_ptr[loopbackSetting->chan]), VP_NULL, VP_OPTION_ID_LOOPBACK, &loopBack)) {

		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, " VpSetOption failed\n");
		return _TDI_FAIL;
	}

	return _TDI_SUCCESS;

}

/*[OSBNB00045371] ADD by mtk07059_20150423 , support ring parameters set. */
int zl88601_SetRingParam(char *buf)
{
	_TDI_fxsSetRingParams_s* ringParams=(_TDI_fxsSetRingParams_s*)buf;
	VpOptionRingingParamsType vpParams;
	int ret=0;

	//printk("zl88601_SetRingParam test :\n type:%d;\nfrequency:%d;\namplitude:%d;\ndc:%d;\n",
	//	ringParams->type, ringParams->frequency, ringParams->amplitude, ringParams->dcBias);

	/* check current setting */
	if(0 == ringParams->type){
		ret = VpConfigLine(&(lineCtx_ptr[ringParams->chanId]), VP_NULL, VP_NULL, RINGING_PRO_FXS_zl88601);
	}
	else if(1 == ringParams->type){
		ret = VpConfigLine(&(lineCtx_ptr[ringParams->chanId]), VP_NULL, VP_NULL, RING_ZL880_ABS100V_TRAP);
	}
	else{
		return _TDI_FAIL;
	}
	if(ret != VP_STATUS_SUCCESS){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERROR ret:%d\n", ret);
		return _TDI_FAIL;
	}
	
	vpParams.validMask = VP_OPTION_CFG_FREQUENCY | VP_OPTION_CFG_AMPLITUDE | VP_OPTION_CFG_DC_BIAS	|VP_OPTION_CFG_RINGTRIP_THRESHOLD;
	vpParams.amplitude = ringParams->amplitude;
	vpParams.frequency = ringParams->frequency;
	vpParams.dcBias = ringParams->dcBias;
	vpParams.ringTripThreshold = ringParams->ringTripThreshold;	 /* added by mtk08176_20150605  */
	ret = VpSetOption(&(lineCtx_ptr[ringParams->chanId]), VP_NULL, VP_OPTION_ID_RINGING_PARAMS, &vpParams);
	if(ret != VP_STATUS_SUCCESS){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, " ret:%d\n", ret);
		return _TDI_FAIL;
	}
	return _TDI_SUCCESS;

}

/*[OSBNB00047036] ADD by mtk07059_20150616 , support dc feed parameters set. */
int zl88601_SetDcFeedParam(char* buf)
{
	_TDI_fxsSetDcFeedParams_s* dcFeedParams=(_TDI_fxsSetDcFeedParams_s*)buf;
	VpOptionDcFeedParamsType vpParams;
	int ret=0;

	//printk("zl88601_SetRingParam test :\n type:%d;\nfrequency:%d;\namplitude:%d;\ndc:%d;\n",
	//	ringParams->type, ringParams->frequency, ringParams->amplitude, ringParams->dcBias);
	vpParams.validMask = VP_OPTION_CFG_ILA;
	vpParams.ila = dcFeedParams->ila;
	ret = VpSetOption(&(lineCtx_ptr), VP_NULL, VP_OPTION_ID_DCFEED_PARAMS, &vpParams);
	if(ret != VP_STATUS_SUCCESS){
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_ERROR, "ERROR ret:%d\n", ret);
		return _TDI_FAIL;
	}
	return _TDI_SUCCESS;

}

#if 0

int zl88601_fxoGetRingCadence(char* buf){
	event_printf("%s - %d\n",__func__,zl88601LineStat.fxoRingStat);
	return zl88601LineStat.fxoRingStat;
}
int zl88601_fxoGetHookState(char* buf){
	dbg_printf("%s - %d \n",__func__,zl88601LineStat.fxoHookStat);
    return zl88601LineStat.fxoHookStat;
}
int zl88601_fxoSetHookState(char* buf){
	int state = *(int *)buf;

	dbg_printf("%s - %d \n",__func__,state);
    switch (state) {
        case TIC_SEIZE: /*off hook*/
        	VpSetLineState(&(lineCtx_ptr[FXO_LINE]),VP_LINE_FXO_TALK);

            return _TDI_SUCCESS;
        case TIC_RELEASE:
        	VpSetLineState(&(lineCtx_ptr[FXO_LINE]),VP_LINE_FXO_OHT);
            return _TDI_SUCCESS;
    }

}
int zl88601_fxoGetLineStats(char* buf){
	_TDI_fxoLineStat_t* fxoLineStat = (_TDI_fxoLineStat_t*)buf;

	event_printf("%s - chan:%d \n",__func__,fxoLineStat->chan);
	if(zl88601LineStat.polRev){
		fxoLineStat->lc = 1;
		fxoLineStat->lv = 1;
	}
	else{
		fxoLineStat->lc = -1;
		fxoLineStat->lv = -1;
	}
	return _TDI_SUCCESS;

}
int zl88601_fxoGetPSTNStats(char* buf){
	//printk("%s  \n",__func__);
	return zl88601LineStat.fxoLineConn;

}

int zl88601_fxoSetMonitorMode(char* buf){
	return _TDI_SUCCESS;
}

#endif

