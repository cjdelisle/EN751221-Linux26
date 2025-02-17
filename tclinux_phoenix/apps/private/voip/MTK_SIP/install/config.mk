CONFIG = -D OSBN_VOIP -D BIG_ENDIAN -D PORK_MODIFY

ifneq ($(strip $(TCSUPPORT_VOIP_IMSSIP)),)
CONFIG += -D TCSUPPORT_VOIP_IMSSIP
endif

ifneq ($(strip $(TCSUPPORT_CT_VOIP_SIP)),)
export TCSUPPORT_CT_VOIP_SIP
CONFIG += -D TCSUPPORT_CT_VOIP_SIP# -D VOICE_QUALITY_IMPROVE
endif
# -DSERENA_DO_CFG -D PORK_MODIFY -D SERENA_FXO_MODIFY -D OSBN_VOIP 

ifneq ($(strip $(TCSUPPORT_CT_PON_C9)),)
export TCSUPPORT_CT_PON_C9
CONFIG += -D TCSUPPORT_CT_PON_C9
endif
ifneq ($(strip $(TCSUPPORT_CT_PON_GD)),)
export TCSUPPORT_CT_PON_GD
CONFIG += -D TCSUPPORT_CT_PON_GD
endif
ifneq ($(strip $(TCSUPPORT_CT_PON_CY)),)
export TCSUPPORT_CT_PON_CY
CONFIG += -D TCSUPPORT_CT_PON_CY
endif
ifneq ($(strip $(TCSUPPORT_CUC)),)
export TCSUPPORT_CUC
CONFIG += -D TCSUPPORT_CUC
endif

ifneq ($(strip $(CZ_DSP_SUPPORT)),)
export CZ_DSP_SUPPORT
CONFIG += -D CZ_DSP_SUPPORT
endif

ifneq ($(strip $(TCSUPPORT_CT_PON_JS_IP)),)
export TCSUPPORT_CT_PON_JS_IP
CONFIG += -D TCSUPPORT_CT_PON_JS_IP
endif

ifneq ($(strip $(TCSUPPORT_VOIP_LED_APPCTRL)),)
export TCSUPPORT_VOIP_LED_APPCTRL
CONFIG += -D TCSUPPORT_VOIP_LED_APPCTRL
endif

ifneq ($(strip $(TCSUPPORT_CT_VOIP_CRYPT)),)
export TCSUPPORT_CT_VOIP_CRYPT
CONFIG += -D TCSUPPORT_CT_VOIP_CRYPT
endif

ifneq ($(strip $(TCSUPPORT_CT_PON_SK)),)
export TCSUPPORT_CT_PON_SK=1
CONFIG += -D TCSUPPORT_CT_PON_SK
endif

ifneq ($(strip $(TCSUPPORT_VOIP_SIP_DNS)),)
export TCSUPPORT_VOIP_SIP_DNS
CONFIG += -D TCSUPPORT_VOIP_SIP_DNS
endif

ifneq ($(strip $(TCSUPPORT_VOIP_SIP_T_TIMER)),)
export TCSUPPORT_VOIP_SIP_T_TIMER
CONFIG += -D TCSUPPORT_VOIP_SIP_T_TIMER
endif

ifneq ($(strip $(TCSUPPORT_SYSLOG)),)
export TCSUPPORT_SYSLOG
CONFIG += -D TCSUPPORT_SYSLOG
endif

ifneq ($(strip $(TCSUPPORT_VOIP_FLASH2_HOLD)),)
export TCSUPPORT_VOIP_FLASH2_HOLD
CONFIG += -D TCSUPPORT_VOIP_FLASH2_HOLD
endif
