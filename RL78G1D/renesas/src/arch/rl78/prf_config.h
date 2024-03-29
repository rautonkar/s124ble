/**
 ****************************************************************************************
 *
 * @file    prf_config.h
 *
 * @brief   Definition of Configuration Parameters for Profile
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2013-2017 Renesas Electronics Corporation
 *
 * This file is generated by Bluetooth Developer Studio plugin.
 *     BDS Version    : 1.1.3139.0
 *     Script Version : 2.0.1
 *
 ****************************************************************************************
 */

#ifndef _PRF_CONFIG_H_
#define _PRF_CONFIG_H_

/**
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "arch.h"
#include "rwble_config.h"

#include "co_bt.h"
#include "rble_api.h"

#include "prf_sel.h"

#include "rble_api_custom.h"

#include "sam/sams.h"

/**
 * TYPE DEFINES
 ****************************************************************************************
 */


/**
 * DEFINES
 ****************************************************************************************
 */
#define DB_TYPE_128BIT_UUID                 0xFFFFu /**< entry type 128bit UUID */

/**
 * DEFINES (initial value for GATT and GAP)
 ****************************************************************************************
 */

/*
 * GAP Device name Characteristic
 * Default device name
 */
#define GAP_DEV_NAME                    "Renesas-BLE"
#define GAP_DEV_NAME_SIZE               (sizeof(GAP_DEV_NAME) - 1)

/*
 * Scan interval/window for device search (performing Limited/General Discovery procedure)
 */
#define GAP_DEV_SEARCH_TIME             0x0300      /* 7.68sec = 768*10ms */
#define GAP_DEV_SEARCH_SCAN_INTV        0x0012
#define GAP_DEV_SEARCH_SCAN_WINDOW      0x0012

/*
 * Maximum time to remain advertising when in the limited discoverable mode
 * Required value : 180sec
 */
#define GAP_LIM_ADV_TIMEOUT             0x0C00      /* 30.72sec = 3072*10ms */

/*
 * Scan interval in any discovery or connection establishment
 * procedure when user initiated: TGAP(scan_fast_interval)
 * recommended value: 30 to 60 ms; N * 0.625
 */
#define GAP_SCAN_FAST_INTV              0x0030

/*
 * Scan window in any discovery or connection establishment
 * procedure when user initiated: TGAP(scan_fast_window)
 * recommended value: 30 ms; N * 0.625
 */
#define GAP_SCAN_FAST_WINDOW            0x0030

/*
 * Minimum to maximum connection interval upon any connection
 * establishment: TGAP(initial_conn_interval)
 * recommended value: 30 to 50 ms ; N * 1.25 ms
 */
#define GAP_INIT_CONN_MIN_INTV          0x0018
#define GAP_INIT_CONN_MAX_INTV          0x0028

/* 
 * Minimum/Maximum connection event length
 */
#define GAP_CONN_MIN_CE_LENGTH          0x0000
#define GAP_CONN_MAX_CE_LENGTH          0xFFFF

/* 
 * slave latency
 */
#define GAP_CONN_SLAVE_LATENCY          0x0000

/* 
 * supervision timeout
 * recommended value: 20s
 */
#define GAP_DEV_SUPERVISION_TIMEOUT     0x07D0

/*
 * Recommended value : 15min
 * 0x3A98 = 150sec(15000 * 10ms) = 2.5min
 */
#define GAP_RESOLVABLE_PRIVATE_ADDR_INTV    0x3A98

/*
 * Appearance or Icon value
 * see spec, https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml
 */
#define GAP_APPEARANCE_CHAR_VAL         0x0000

/** Slave Preferred Parameter value */
#define GAP_PPCP_CONN_INTV_MAX          0x0064u
#define GAP_PPCP_CONN_INTV_MIN          0x00C8u
#define GAP_PPCP_SLAVE_LATENCY          0x0000u
#define GAP_PPCP_STO_MULT               0x07D0u

/** Service Changed */
/* Start Handle */
#define GATT_SERVICE_CHANGED_START_HDL  0x0001
/* End Handle */
#define GATT_SERVICE_CHANGED_END_HDL    0xFFFF

/** Service Changed indication configuration */
#define GATT_SERVICE_CHANGED_IND_EN     0x0000


/** Attribute database index */
enum
{
    /* Invalid index*/
    ATT_INVALID_IDX = 0x0000,

    /* Generic Access Profile Service */
    GAP_IDX_PRIM_SVC,
    GAP_IDX_CHAR_DEVNAME,
    GAP_IDX_DEVNAME,
    GAP_IDX_CHAR_ICON,
    GAP_IDX_ICON,
    GAP_IDX_CHAR_SLAVE_PREF_PARAM,
    GAP_IDX_SLAVE_PREF_PARAM,

    /* Generic Attribute Profile Service */
    GATT_IDX_PRIM_SVC,
    GATT_IDX_CHAR_SVC_CHANGED,
    GATT_IDX_SVC_CHANGED,
    GATT_IDX_SVC_CHANGED_IND_CFG,
};

#if USE_SAMPLE_PROFILE
enum
{
    /* Sample Custom Service */
	ENVIRONMENTAL_SENSING_IDX_SVC_ENVIRONMENTAL_SENSING,
	ENVIRONMENTAL_SENSING_IDX_ES_DESCRIPTOR_VALUE_CHANGED_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_DESCRIPTOR_VALUE_CHANGED_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_DESCRIPTOR_VALUE_CHANGED_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_APPARENT_WIND_DIRECTION_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_APPARENT_WIND_DIRECTION_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_APPARENT_WIND_DIRECTION_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_APPARENT_WIND_SPEED_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_APPARENT_WIND_SPEED_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_APPARENT_WIND_SPEED_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_DEW_POINT_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_DEW_POINT_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_DEW_POINT_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_ELEVATION_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_ELEVATION_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_ELEVATION_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_GUST_FACTOR_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_GUST_FACTOR_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_GUST_FACTOR_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_HEAT_INDEX_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_HEAT_INDEX_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_HEAT_INDEX_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_HUMIDITY_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_HUMIDITY_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_HUMIDITY_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_IRRADIANCE_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_IRRADIANCE_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_IRRADIANCE_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_POLLEN_CONCENTRATION_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_POLLEN_CONCENTRATION_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_POLLEN_CONCENTRATION_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_RAINFALL_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_RAINFALL_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_RAINFALL_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_PRESSURE_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_PRESSURE_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_PRESSURE_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_TEMPERATURE_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_TEMPERATURE_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_TEMPERATURE_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_TRUE_WIND_DIRECTION_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_TRUE_WIND_DIRECTION_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_TRUE_WIND_DIRECTION_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_TRUE_WIND_SPEED_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_TRUE_WIND_SPEED_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_TRUE_WIND_SPEED_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_UV_INDEX_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_UV_INDEX_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_UV_INDEX_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_WIND_CHILL_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_WIND_CHILL_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_WIND_CHILL_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_BAROMETRIC_PRESSURE_TREND_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_BAROMETRIC_PRESSURE_TREND_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_BAROMETRIC_PRESSURE_TREND_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_MAGNETIC_DECLINATION_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_MAGNETIC_DECLINATION_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_MAGNETIC_DECLINATION_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_MAGNETIC_FLUX_DENSITY___2D_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_MAGNETIC_FLUX_DENSITY___2D_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_MAGNETIC_FLUX_DENSITY___2D_CCCD,
	ENVIRONMENTAL_SENSING_IDX_ES_MAGNETIC_FLUX_DENSITY___3D_CHAR,
	ENVIRONMENTAL_SENSING_IDX_ES_MAGNETIC_FLUX_DENSITY___3D_VAL,
	ENVIRONMENTAL_SENSING_IDX_ES_MAGNETIC_FLUX_DENSITY___3D_CCCD,

	ENVIRONMENTAL_SENSING_IDX_SVC_DEVICE_INFORMATION,
	ENVIRONMENTAL_SENSING_IDX_DI_MANUFACTURER_NAME_STRING_CHAR,
	ENVIRONMENTAL_SENSING_IDX_DI_MANUFACTURER_NAME_STRING_VAL,
	ENVIRONMENTAL_SENSING_IDX_DI_MODEL_NUMBER_STRING_CHAR,
	ENVIRONMENTAL_SENSING_IDX_DI_MODEL_NUMBER_STRING_VAL,
	ENVIRONMENTAL_SENSING_IDX_DI_SERIAL_NUMBER_STRING_CHAR,
	ENVIRONMENTAL_SENSING_IDX_DI_SERIAL_NUMBER_STRING_VAL,
	ENVIRONMENTAL_SENSING_IDX_DI_HARDWARE_REVISION_STRING_CHAR,
	ENVIRONMENTAL_SENSING_IDX_DI_HARDWARE_REVISION_STRING_VAL,
	ENVIRONMENTAL_SENSING_IDX_DI_FIRMWARE_REVISION_STRING_CHAR,
	ENVIRONMENTAL_SENSING_IDX_DI_FIRMWARE_REVISION_STRING_VAL,
	ENVIRONMENTAL_SENSING_IDX_DI_SOFTWARE_REVISION_STRING_CHAR,
	ENVIRONMENTAL_SENSING_IDX_DI_SOFTWARE_REVISION_STRING_VAL,
	ENVIRONMENTAL_SENSING_IDX_DI_SYSTEM_ID_CHAR,
	ENVIRONMENTAL_SENSING_IDX_DI_SYSTEM_ID_VAL,
	ENVIRONMENTAL_SENSING_IDX_DI_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST_CHAR,
	ENVIRONMENTAL_SENSING_IDX_DI_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST_VAL,
	ENVIRONMENTAL_SENSING_IDX_DI_PNP_ID_CHAR,
	ENVIRONMENTAL_SENSING_IDX_DI_PNP_ID_VAL,

	ENVIRONMENTAL_SENSING_IDX_SVC_BATTERY_SERVICE,
	ENVIRONMENTAL_SENSING_IDX_BS_BATTERY_LEVEL_CHAR,
	ENVIRONMENTAL_SENSING_IDX_BS_BATTERY_LEVEL_VAL,
	ENVIRONMENTAL_SENSING_IDX_BS_BATTERY_LEVEL_CCCD,

};
#endif /* #if USE_SAMPLE_PROFILE */

/**
 * STRUCTURE DECLARATIONS
 ****************************************************************************************
 */

/** Devaice Name variable */
struct device_name
{
    /** length for name */
    uint8_t     namelen;
    /** array of bytes for name */
    uint8_t     name[RBLE_BD_NAME_SIZE];
};


/** Scan interval window characteristic variable */
struct svc_scans_intv_window
{
    /** scan interval value */
    uint16_t le_scan_interval;
    /** scan window value */
    uint16_t le_scan_window;
};

/** Attribute data server entry */
struct atts_desc
{
    /** Element UUID */
    uint16_t type;
    /** Maximum length of the element */
    uint8_t maxlength;
    /** Current length of the element */
    uint8_t length;
    /** TaskId and Attribute Idx of the upper profile */
    ke_task_id_t taskid;
    /** Attribute permission */
    uint16_t perm;
    /** Pointer to the element value */
    void *value;
};

struct atts_elmt_128
{
    /** attribute UUID */
    uint8_t attr_uuid[RBLE_GATT_128BIT_UUID_OCTET];
    /** length of the UUID */
    uint8_t uuid_len;
    /** Pointer to the element value */
    void *value;
};


/**
 * DEFINES
 ****************************************************************************************
 */
#define ATTS_CHAR(prop, handle, type)   { (uint8_t)((prop)),                                \
                                        { (uint8_t)((handle)),  (uint8_t)((handle) >> 8)},  \
                                        { (uint8_t)((type)),    (uint8_t)((type) >> 8)}}
#define ATTS_INCL(start, end, uuid)     { { (uint8_t)((start)), (uint8_t)((start) >> 8)},   \
                                          { (uint8_t)((end)),   (uint8_t)((end) >> 8)},     \
                                          { (uint8_t)((uuid)),  (uint8_t)((uuid) >> 8)} }

#define TASK_ATTID(taskid, idx)         (uint16_t)((taskid << 10) | idx)

#define ATTS_32BIT_UUID(uuid)           { 0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00,       \
                                         (uint8_t)(0x000000FF & uuid), (uint8_t)((0x0000FF00 & uuid) >> 8),             \
                                         (uint8_t)((0x00FF0000 & uuid) >> 16), (uint8_t)((0xFF000000 & uuid) >> 24) }

/**
 * STRUCTURE DECLARATIONS
 ****************************************************************************************
 */

/** Characteristic Value Descriptor */
struct atts_char_desc
{
    /** properties */
    uint8_t prop;
    /** attribute handle */
    uint8_t attr_hdl[ sizeof(uint16_t) ];
    /** attribute type */
    uint8_t attr_type[ RBLE_GATT_16BIT_UUID_OCTET ];
};

/** 128bit Characteristic Value Descriptor */
struct atts_char128_desc
{
    /** properties */
    uint8_t prop;
    /** attribute handle */
    uint8_t attr_hdl[ sizeof(uint16_t) ];
    /** attribute type */
    uint8_t attr_type[ RBLE_GATT_128BIT_UUID_OCTET ];
};

/** include service entry element */
struct att_incl_desc
{
    /** start handle value of included service */
    uint8_t start_hdl[ sizeof(uint16_t) ];
    /** end handle value of included service */
    uint8_t end_hdl[ sizeof(uint16_t) ];
    /** attribute value UUID */
    uint8_t uuid[ RBLE_GATT_16BIT_UUID_OCTET ];
};

/** Slave preferred connection parameters */
struct atts_slv_pref
{
    /** Connection interval maximum */
    uint8_t con_intv_max[ sizeof(uint16_t) ];
    /** Connection interval minimum */
    uint8_t con_intv_min[ sizeof(uint16_t) ];
    /** Slave latency */
    uint8_t slave_latency[ sizeof(uint16_t) ];
    /** Connection supervision timeout multiplier */
    uint8_t conn_timeout[ sizeof(uint16_t) ];
};

/**
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
extern struct atts_desc *atts_desc_list;
extern const struct atts_desc atts_desc_list_prf[];
extern const struct atts_desc atts_desc_list_host[];
extern uint16_t ATT_LAST_IDX;
extern const uint16_t ATT_HOST_DB_SIZE;
extern const uint16_t ATT_PRF_DB_SIZE;

extern uint16_t gap_discovery_scan_time;
extern uint16_t gap_dev_search_scan_intv;
extern uint16_t gap_dev_search_scan_window;
extern uint16_t gap_lim_adv_timeout;
extern uint16_t gap_scan_fast_intv;
extern uint16_t gap_scan_fast_window;
extern uint16_t gap_init_conn_min_intv;
extern uint16_t gap_init_conn_max_intv;
extern uint16_t gap_conn_min_ce_length;
extern uint16_t gap_conn_max_ce_length;
extern uint16_t gap_conn_slave_latency;
extern uint16_t gap_dev_supervision_timeout;
extern uint16_t gap_resolvable_private_addr_intv;

extern uint8_t gatt_service_changed[4];
extern uint16_t gatt_service_changed_ind_en;

extern struct device_name gap_dev_name;
extern uint16_t gap_appearance_char_val;
#endif /* _PRF_CONFIG_H_ */
