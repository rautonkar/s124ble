/**
 ****************************************************************************************
 *
 * @file		rble_app.h
 *
 * @brief	rBLE Sample Program
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 9368 $
 *
 ****************************************************************************************
 */

#ifndef _rBLE_APP_H
#define _rBLE_APP_H

/**
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rble.h"
#include "rble_api.h"
#include "rble_api_custom.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * DEFINES
 ****************************************************************************************
 */
#define ILLEGAL_NO                          -1
#define RBLE_PARAMETER_MAX                  5

#define ILLEGAL_CONNECTION_HANDLE           0xffff


typedef struct RBLE_GAP_Observation_Enable_t {
    uint16_t            mode;
    RBLE_SCANNING_INFO  scan_info;
} RBLE_OBSERVATION_ENABLE_PARAM;

typedef struct RBLE_GAP_Broadcast_Enable_t {
    uint16_t            disc_mode;
    uint16_t            conn_mode;
    RBLE_ADV_INFO       adv_info;
} RBLE_BROADCAST_ENABLE_PARAM;

typedef struct RBLE_GAP_Privacy_Feature_Param_t {
    uint8_t             priv_flag;
    uint8_t             set_to_ll;
} RBLE_GAP_PRIV_FEAT_PRAM;

/*
 * EXTERN DECLARATIONS
 ****************************************************************************************
 */
extern uint32_t             RBLE_Parameter_Arg[ RBLE_PARAMETER_MAX ];
extern uint_t               RBLE_Parameter_Cnt;
extern uint16_t             Connection_Handle;
extern const RBLE_OBSERVATION_ENABLE_PARAM      RBLE_Observation_Enable_Param[4];
extern const RBLE_BROADCAST_ENABLE_PARAM        RBLE_Broadcast_Enable_Param[22];
extern const RBLE_BOND_PARAM                    RBLE_Start_Bond_Param[14];
extern const RBLE_BOND_RESP_PARAM               RBLE_Bond_Resp_Param[17];

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
BOOL RBLE_App_Init( void );

void BdAddress_Disp( RBLE_BD_ADDR *Addr_p );
void AdvRepot_Disp( RBLE_ADV_REPORT *Report_p );
void Uuid128_Disp( uint8_t *p );
void RBLE_Status_Disp( RBLE_STATUS Status );
void KeyDisp( struct RBLE_EVT_SM_Key_t *Key_p );
void RandomDataDisp( RBLE_RAND_NB *p );
void KeyDataDisp( RBLE_KEY_VALUE *p );
char *Get_Status_Str( RBLE_STATUS Status );
void ATT_Info_Disp( RBLE_ATT_INFO_DATA *p );
void Print_Dump( uint8_t *p, uint16_t len );
char  *Get_Att_Code_Str( uint8_t att_code );
void Select_Param_No_Disp(uint32_t no);
void Connection_Handle_Disp( uint16_t conhdl );
void Illegal_Connection_Print( void );
int_t RBLE_Get_Random( void );

float FLOAT2float( uint32_t float_val );
float SFLOAT2float( uint16_t sfloat_val );
void DIS_handle_Info_Disp(RBLE_DIS_CONTENT *dis);
void CPP_Broadcast_Rcv(RBLE_ADV_REPORT *adv_rep_p );
void BAS_handle_Info_Disp(RBLE_BATS_CONTENT *bas);
void TIP_Time_Disp(RBLE_DATE_TIME *stamp);


#ifdef __cplusplus
}
#endif
#endif /* _rBLE_APP_H */
