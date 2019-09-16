/**
 ****************************************************************************************
 *
 * @file		rble_fw_up_sender_app.c
 *
 * @brief	rBLE FW Update Sample Application(Sender)
 *
 * Copyright(C) 2014 Renesas Electronics Corporation
 *
 * $Rev: 3867 $
 *
 ****************************************************************************************
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef USE_FW_UPDATE_PROFILE
#include <windows.h>
#include "rble.h"
#include "rble_api.h"
#include "rble_app.h"

#include "prf_sel.h"
#include "rble_api_fwup.h"

#include "timer.h"

/**************************** Define Declaration **************************************/

#define ILLEGAL_CONNECTION_HANDLE			0xffff
#define FW_UP_BLOCK_SIZE	1024
#define FW_UPDATE_INTERVAL	18

enum {
	FW_UP_STATE_IDLE,					/* idle */
	FW_UP_STATE_SET_DATA_SEND_START,	/* write data control(DATA_SEND_START) */
	FW_UP_STATE_SEND_DATA,				/* write data */
	FW_UP_STATE_SET_DATA_SEND_COMP,		/* write data control(DATA_SEND_COMP) */
	FW_UP_STATE_CHECK_WRITE,			/* write check */
	FW_UP_STATE_COMP_DISCONNECT,		/* disconnect(after data send comp) */
	FW_UP_STATE_CHECK_FINISH,			/* fw update check */
	FW_UP_STATE_FINISH_DISCONNECT		/* disconnect(after data send finish) */
};

/**************************** Function definition **************************************/
static void fw_update_callback(RBLE_FWUPS_EVENT *event);
static void RBLE_FWUP_GAP_CallBack( RBLE_GAP_EVENT *event );
static void RBLE_FWUP_SM_CallBack( RBLE_SM_EVENT *event );
static void RBLE_FWUP_FWUPS_Callback( RBLE_FWUPS_EVENT *event);

extern BOOL RBLE_Is_Cmd_Buff_Available(void);
/**************************** PRIVATE Memory Declaration ******************************/
uint8_t update_area_emb[][2] = 
{
	/* embededde */
	/* start block, end block */
	{  0,  3},		/* 0x00000-0x00FFF( 4block) */
	{ 17, 42},		/* 0x04400-0x0ABFF(26block) */
	{ 44, 47},		/* 0x0B000-0x0BFFF( 4block) */
	{197,239}		/* 0x31400-0x3BFFF(43block) */
};

uint8_t update_area_iar[][2] =
{
	/* iar */
	/* start block, end block */
	{  0,  3},		/* 0x00000-0x00FFF( 4block) */
	{ 17, 42},		/* 0x04400-0x0ABFF(26block) */
	{ 44, 47},		/* 0x0B000-0x0BFFF( 4block) */
	{176,225},		/* 0x2C000-0x387FF(50block) */
	{168,173}		/* 0x2A000-0x2B7FF( 6block) */
};

uint8_t update_area_mdm[][2] =
{
	/* modem */
	/* start block, end block */
	{  0,  3},		/* 0x00000-0x00FFF( 4block) */
	{ 17, 42},		/* 0x04400-0x0ABFF(27block) */
	{ 44, 47},		/* 0x0B000-0x0BFFF( 4block) */
	{207,235}		/* 0x33C00-0x3AFFF(29block) */
};

uint8_t update_area_iar_mdm[][2] =
{
	/* iar */
	/* start block, end block */
	{  0,  3},		/* 0x00000-0x00FFF( 4block) */
	{ 17, 42},		/* 0x04400-0x0ABFF(26block) */
	{ 44, 47},		/* 0x0B000-0x0BFFF( 4block) */
	{176,210},		/* 0x2C000-0x34BFF(35block) */
	{168,173}		/* 0x2A000-0x2B7FF( 6block) */
};

uint8_t update_area_ccrl_emb[][2] =
{
	/* embededde */
	/* start block, end block */
	{  0,  3},		/* 0x00000-0x00FFF( 4block) */
	{ 17, 42},		/* 0x04400-0x0ABFF(26block) */
	{ 44, 47},		/* 0x0B000-0x0BFFF( 4block) */
	{192,235}		/* 0x30000-0x3AFFF(44block) */
};

uint8_t update_area_ccrl_mdm[][2] =
{
	/* modem */
	/* start block, end block */
	{  0,  3},		/* 0x00000-0x00FFF( 4block) */
	{ 17, 42},		/* 0x04400-0x0ABFF(26block) */
	{ 44, 47},		/* 0x0B000-0x0BFFF( 4block) */
	{192,235}		/* 0x30000-0x3AFFF(44block) */
};



uint8_t update_area[10][2] = {0};
uint8_t last_area_count;

char input_file_name[][50] = 
{
#if 0
	"BLE_Emb_CS_HOGP_ScPP.bin",
	"BLE_Emb_CS_LNP_RSCP.bin",
	"BLE_Emb_CS_HTP_BLP_HRP.bin",
	"BLE_Emb_CS_CPP_PASP.bin",
	"BLE_Emb_CS_GLP_TIP.bin",
	"BLE_Emb_CS_CSCP_FMP.bin",
	"BLE_Emb_CS_ANP_PXP.bin",
	"BLE_Emb_IAR_HOGP_ScPP.bin",
	"BLE_Emb_IAR_LNP_RSCP.bin",
	"BLE_Emb_IAR_HTP_BLP_HRP.bin",
	"BLE_Emb_IAR_CPP_PASP.bin",
	"BLE_Emb_IAR_GLP_TIP.bin",
	"BLE_Emb_IAR_CSCP_FMP.bin",
	"BLE_Emb_IAR_ANP_PXP.bin",
	"BLE_Emb_CC_HOGP_ScPP.bin",
	"BLE_Emb_CC_LNP_RSCP.bin",
	"BLE_Emb_CC_HTP_BLP_HRP.bin",
	"BLE_Emb_CC_CPP_PASP.bin",
	"BLE_Emb_CC_GLP_TIP.bin",
	"BLE_Emb_CC_CSCP_FMP.bin",
	"BLE_Emb_CC_ANP_PXP.bin",
	"BLE_Mdm_CS_HOGP_ScPP.bin",
	"BLE_Mdm_CS_LNP_RSCP.bin",
	"BLE_Mdm_CS_HTP_BLP_HRP.bin",
	"BLE_Mdm_CS_CPP_PASP.bin",
	"BLE_Mdm_CS_GLP_TIP.bin",
	"BLE_Mdm_CS_CSCP_FMP.bin",
	"BLE_Mdm_CS_ANP_PXP.bin",
	"BLE_Mdm_IAR_HOGP_ScPP.bin",
	"BLE_Mdm_IAR_LNP_RSCP.bin",
	"BLE_Mdm_IAR_HTP_BLP_HRP.bin",
	"BLE_Mdm_IAR_CPP_PASP.bin",
	"BLE_Mdm_IAR_GLP_TIP.bin",
	"BLE_Mdm_IAR_CSCP_FMP.bin",
	"BLE_Mdm_IAR_ANP_PXP.bin",
	"BLE_Mdm_CC_HOGP_ScPP.bin",
	"BLE_Mdm_CC_LNP_RSCP.bin",
	"BLE_Mdm_CC_HTP_BLP_HRP.bin",
	"BLE_Mdm_CC_CPP_PASP.bin",
	"BLE_Mdm_CC_GLP_TIP.bin",
	"BLE_Mdm_CC_CSCP_FMP.bin",
	"BLE_Mdm_CC_ANP_PXP.bin",
#else
	"RL78_G1D_CE(PXP,FMP,ANP).bin",
	"RL78_G1D_CE(HTP,BLP,HRP).bin",
	"RL78_G1D_IE(PXP,FMP,ANP).bin",
	"RL78_G1D_IE(HTP,BLP,HRP).bin",
	"RL78_G1D_CCE(PXP,FMP,ANP).bin",
	"RL78_G1D_CCE(HTP,BLP,HRP).bin"
#endif
};

#if 0
#define CS_EMB_FILE_LAST_IDX	7
#define IAR_EMB_FILE_LAST_IDX	14
#define CCRL_EMB_FILE_LAST_IDX	21
#define CS_MDM_FILE_LAST_IDX	28
#define IAR_MDM_FILE_LAST_IDX	35
#define CCRL_MDM_FILE_LAST_IDX	42
#else
#define CS_EMB_FILE_LAST_IDX	2
#define IAR_EMB_FILE_LAST_IDX	4
#define CCRL_EMB_FILE_LAST_IDX	6
#define CS_MDM_FILE_LAST_IDX	15
#define IAR_MDM_FILE_LAST_IDX	20
#define CCRL_MDM_FILE_LAST_IDX	25
#endif

uint8_t fw_update_interval = FW_UPDATE_INTERVAL;

uint8_t area_count;
uint8_t block_count;
uint8_t preb_block_count;
uint16_t data_ptr;
FILE *binFp;

extern RBLE_BD_ADDR Remote_Device;
extern uint8_t Peer_Addr_Type;
extern uint8_t Own_Addr_Type ;

RBLE_FWUS_CONTENT fw_up_fwus;

uint16_t fw_up_conhdl = ILLEGAL_CONNECTION_HANDLE;

uint8_t fw_up_state = FW_UP_STATE_IDLE;

UINT FW_Update_TimerID = 0;

/******************************************************************************************/
/**
 *  @brief FW Update  Application Test Start
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
RBLE_STATUS FW_Update_Sender_Start( uint8_t file_num, uint8_t start_area )
{
	RBLE_STATUS ret;
	
	/* open send data */
	printf("send bin file %s\n", input_file_name[file_num]);
	fopen_s(&binFp, input_file_name[file_num], "rb");
	
	memset(&fw_up_fwus, 0, sizeof(fw_up_fwus));
	
	if(binFp != NULL) {
		if( file_num < CS_EMB_FILE_LAST_IDX ) {
			memcpy(&update_area[0][0], update_area_emb, sizeof(update_area_emb));
			last_area_count = sizeof(update_area_emb) / sizeof(update_area_emb[0]) - 1;
		}
		else if( file_num < IAR_EMB_FILE_LAST_IDX ){
			memcpy(&update_area[0][0], update_area_iar, sizeof(update_area_iar));
			last_area_count = sizeof(update_area_iar) / sizeof(update_area_iar[0]) - 1;
		}
		else if (file_num < CCRL_EMB_FILE_LAST_IDX){
			memcpy(&update_area[0][0], update_area_ccrl_emb, sizeof(update_area_ccrl_emb));
			last_area_count = sizeof(update_area_ccrl_emb) / sizeof(update_area_ccrl_emb[0]) - 1;
		}
		else if (file_num < CS_MDM_FILE_LAST_IDX){
			memcpy(&update_area[0][0], update_area_mdm, sizeof(update_area_mdm));
			last_area_count = sizeof(update_area_mdm) / sizeof(update_area_mdm[0]) - 1;
		}
		else if (file_num < IAR_MDM_FILE_LAST_IDX){
			memcpy(&update_area[0][0], update_area_iar_mdm, sizeof(update_area_iar_mdm));
			last_area_count = sizeof(update_area_iar_mdm) / sizeof(update_area_iar_mdm[0]) - 1;
		}
		else {
			memcpy(&update_area[0][0], update_area_ccrl_mdm, sizeof(update_area_ccrl_mdm));
			last_area_count = sizeof(update_area_ccrl_mdm) / sizeof(update_area_ccrl_mdm[0]) - 1;
		}

		fw_up_state = FW_UP_STATE_IDLE;
		
		if(start_area < (sizeof(update_area) / 2)) {
			area_count = start_area;
		}
		else {
			area_count = 0;
		}
		block_count = update_area[area_count][0];
		preb_block_count = 0xff;
		data_ptr = 0;
		
		/* GAP Reset */
		RBLE_GAP_Reset( &RBLE_FWUP_GAP_CallBack, &RBLE_FWUP_SM_CallBack );
		
		ret = RBLE_OK;
	}
	else {
		printf(" cant't open bin file\n");
		ret = RBLE_STATUS_ERROR;
	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Create Connection
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
static void FW_Update_Sender_Create_Connection( void )
{
	RBLE_STATUS Ret_Status;
	RBLE_CREATE_CONNECT_PARAM param;

	/* Connection Parameter Set*/
	param.scan_intv			= 48;								/* Scan interval    ( 30msec = 48 * 0.625msec ) Range:0x0004-0x4000 */
	param.scan_window		= 48;								/* Scan window size ( 30msec = 48 * 0.625msec ) Range:0x0004-0x4000 */
	param.init_filt_policy	= RBLE_GAP_INIT_FILT_IGNORE_WLST;	/* Initiator filter policy */
	param.peer_addr_type	= Peer_Addr_Type;					/* Peer address type */

	param.peer_addr = Remote_Device;							/* Peer BD address */

	param.own_addr_type		= Own_Addr_Type;					/* Own address type - 0=public/1=random */
	param.con_intv_min		= 16;								/* Minimum of connection interval ( 20msec = 16 * 1.25msec ) Range:0x0006-0x0c80 */
	param.con_intv_max		= 16;								/* Maximum of connection interval ( 20msec = 16 * 1.25msec ) Range:0x0006-0x0c80 */
	param.con_latency		= 0x00;								/* Connection latency Range:0x0000-0x01f4 */
	param.superv_to			= 0x14;								/* Link supervision timeout ( 200msec = 0x14 * 10msec ) Range:0x000a-0x0c80 */
	param.ce_len_min		= 0x00;								/* Minimum CE length ( 00msec = 0x00 * 0.625msec ) Range:0x0000-0xffff */
	param.ce_len_max		= 80;								/* Maximum CE length ( 50msec = 80 * 0.625msec ) Range:0x0000-0xffff */

	/* API Call */
	printf("create");
	Ret_Status = RBLE_GAP_Create_Connection(&param);
}

/******************************************************************************************/
/**
 *  @brief FW Update Profile Enable
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
static void FW_Update_Sender_Enable( void )
{
	uint8_t con_type;
	RBLE_STATUS Ret_Status;
	
	if( fw_up_fwus.shdl != 0 ) {
		con_type = RBLE_FWUP_CON_NORMAL;
	}
	else {
		con_type = RBLE_FWUP_CON_CFG;
	}
	Ret_Status = RBLE_FWUP_Sender_Enable(fw_up_conhdl, con_type, &fw_up_fwus, RBLE_FWUP_FWUPS_Callback);
}

/******************************************************************************************/
/**
 *  @brief Send FW Update data
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
static void CALLBACK FW_Update_Data_Send( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
	static BOOL data_end = FALSE;
	RBLE_STATUS Ret_Status;
	uint8_t data[20];
	uint8_t data_size;
	uint8_t i;
	
	if (fw_up_conhdl == ILLEGAL_CONNECTION_HANDLE) {
		KillTimer(NULL, FW_Update_TimerID);
		printf("stop\n");
	}
	else if (TRUE == RBLE_Is_Cmd_Buff_Available()) {
		if (data_end == FALSE) {
			if ((FW_UP_BLOCK_SIZE - data_ptr) > 19) {
				data_size = 19;
			}
			else {
				data_size = (uint8_t)(FW_UP_BLOCK_SIZE - data_ptr);
				data_end = TRUE;
			}

			fseek(binFp, block_count * 0x400 + data_ptr, SEEK_SET);
			fread(data, sizeof(char), data_size, binFp);

			data_ptr += data_size;
			data[data_size] = 0;
			for (i = 0; i < data_size; i++) {
				data[data_size] += data[i];
			}


			Ret_Status = RBLE_FWUP_Sender_Write_Data(fw_up_conhdl, data, data_size + 1);
		}
		else {
			KillTimer(NULL, FW_Update_TimerID);

			data_end = FALSE;

			printf(" comp\n");
			/* 1kbyte data sended */
			Ret_Status = RBLE_FWUP_Sender_Write_Data_Cntl(fw_up_conhdl, RBLE_FWUP_DATA_SEND_COMP, 0, 0);
			fw_up_state = FW_UP_STATE_SET_DATA_SEND_COMP;
		}
	}
}

/******************************************************************************************/
/**
 *  @brief Callback of FW Update profile.
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_FWUP_FWUPS_Callback( RBLE_FWUPS_EVENT *event)
{
	RBLE_STATUS Ret_Status;

	switch( event->type){
		case RBLE_FWUP_EVENT_SENDER_ENABLE_COMP:
			if( event->param.sender_enable.status == RBLE_OK ) {
				Sleep(100);
				fw_up_fwus = event->param.sender_enable.fwus;
				
				if( (fw_up_state == FW_UP_STATE_COMP_DISCONNECT) || (fw_up_state == FW_UP_STATE_CHECK_WRITE) ) {
					/* check write code result */
					RBLE_FWUP_Sender_Write_Data_Cntl( fw_up_conhdl, RBLE_FWUP_DATA_CHECK_WRITE, 0, 0 );
					fw_up_state = FW_UP_STATE_CHECK_WRITE;
				}
				else if( fw_up_state == FW_UP_STATE_FINISH_DISCONNECT ) {
					RBLE_FWUP_Sender_Write_Data_Cntl(fw_up_conhdl, RBLE_FWUP_DATA_SEND_FINISH, 0, 0);
					fw_up_state = FW_UP_STATE_CHECK_FINISH;
				}
				else {
					/* set config comp */
					RBLE_FWUP_Sender_Write_Data_Cntl( fw_up_conhdl, RBLE_FWUP_DATA_SEND_START, block_count, 1024 );
					fw_up_state = FW_UP_STATE_SET_DATA_SEND_START;
				}
			}
			else {
				/* update end */
				printf("enable error!!  update end\n");
				fclose(binFp);
			}
			break;
		case RBLE_FWUP_EVENT_SENDER_WRITE_CHAR_RES:
			if( event->param.sender_enable.status == RBLE_OK ) {
				switch(fw_up_state)
				{
					case FW_UP_STATE_SET_DATA_SEND_START:
						/* set DATA_SEND_START comp */
						data_ptr = 0;
						fw_up_state = FW_UP_STATE_SEND_DATA;
						
						/* timer start */
						FW_Update_TimerID = SetTimer( NULL, 0, fw_update_interval, FW_Update_Data_Send );

						printf("send block%d", block_count);
						break;
					case FW_UP_STATE_SET_DATA_SEND_COMP:
						Ret_Status = RBLE_FWUP_Sender_Disable( fw_up_conhdl );
						
						Ret_Status = RBLE_GAP_Disconnect( fw_up_conhdl );
						
						preb_block_count = block_count;
						if( (block_count == update_area[area_count][1]) && (area_count == last_area_count) ) {
							fw_up_state = FW_UP_STATE_FINISH_DISCONNECT;
						}
						else {
							if(block_count == update_area[area_count][1]) {
								area_count++;
								block_count = update_area[area_count][0];
								data_ptr = 0;
							}
							else {
								block_count++;
							}
							fw_up_state = FW_UP_STATE_COMP_DISCONNECT;
						}
						break;
					case FW_UP_STATE_CHECK_WRITE:
						/* set config comp */
						RBLE_FWUP_Sender_Write_Data_Cntl( fw_up_conhdl, RBLE_FWUP_DATA_SEND_START, block_count, 1024 );
						fw_up_state = FW_UP_STATE_SET_DATA_SEND_START;
						break;
					case FW_UP_STATE_CHECK_FINISH:
						Ret_Status = RBLE_FWUP_Sender_Disable( fw_up_conhdl );

						/* write cfg */
						Ret_Status = RBLE_GAP_Disconnect( fw_up_conhdl );
						
						fw_up_state = FW_UP_STATE_IDLE;
						
						fclose(binFp);
						printf("fw update finish\n");
						break;
					default:
						break;
				}
			}
			else {
				if( (fw_up_state == FW_UP_STATE_CHECK_WRITE) ) {
					/* resend data */
					if( (block_count == update_area[area_count][0]) && (area_count > 0) ) {
						area_count--;
					}
					block_count = preb_block_count;
					
					RBLE_FWUP_Sender_Write_Data_Cntl( fw_up_conhdl, RBLE_FWUP_DATA_SEND_START, block_count, 1024 );
					fw_up_state = FW_UP_STATE_SET_DATA_SEND_START;
				}
				else if( fw_up_state == FW_UP_STATE_CHECK_FINISH ) {
					/* resend data */
					RBLE_FWUP_Sender_Write_Data_Cntl( fw_up_conhdl, RBLE_FWUP_DATA_SEND_START, block_count, 1024 );
					fw_up_state = FW_UP_STATE_SET_DATA_SEND_START;
				}
				else if( fw_up_state == FW_UP_STATE_SET_DATA_SEND_COMP ) {
					/* set DATA_SEND_START comp */
					data_ptr = 0;
					fw_up_state = FW_UP_STATE_SET_DATA_SEND_START;
					
					RBLE_FWUP_Sender_Write_Data_Cntl( fw_up_conhdl, RBLE_FWUP_DATA_SEND_START, block_count, 1024 );
					
				}
				else {
					/* update end */
					printf("update end(%d)\n", fw_up_state);
					fclose(binFp);
				}
			}
			break;
	}
}

/******************************************************************************************/
/**
 *  @brief Callback of GAP.
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_FWUP_GAP_CallBack( RBLE_GAP_EVENT *event )
{
	switch( event->type ) {
		case RBLE_GAP_EVENT_RESET_RESULT:
			/* create connection */
			FW_Update_Sender_Create_Connection();
			break;
		case RBLE_GAP_EVENT_CONNECTION_COMP:
			printf("connection\n");
			fw_up_conhdl = event->param.conn_comp.connect_info.conhdl;
			
			/* prepare FW update profile */
			FW_Update_Sender_Enable();
			break;
		case RBLE_GAP_EVENT_DISCONNECT_COMP:
			fw_up_conhdl = ILLEGAL_CONNECTION_HANDLE;
			switch(fw_up_state)
			{
				case FW_UP_STATE_IDLE:
					printf("Disconnect! please gap reset\n");
					break;
				case FW_UP_STATE_FINISH_DISCONNECT:
				case FW_UP_STATE_COMP_DISCONNECT:
					printf("Disconnect!\n");
					FW_Update_Sender_Create_Connection();
					break;
				default:
					RBLE_FWUP_Sender_Disable( fw_up_conhdl );
					printf("Disconnect!? %x\n", event->param.conn_comp.connect_info.status);

					RBLE_GAP_Reset( &RBLE_FWUP_GAP_CallBack, &RBLE_FWUP_SM_CallBack );
					break;
			}
			break;
		case RBLE_GAP_EVENT_ADVERTISING_REPORT_IND:
			printf("adv data\n");
			RBLE_GAP_Observation_Disable();
			break;
		case RBLE_GAP_EVENT_OBSERVATION_DISABLE_COMP:
			FW_Update_Sender_Create_Connection();
			break;
		default:
			printf("??");
			break;
	}
}

/******************************************************************************************/
/**
 *  @brief Callback of SM.
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_FWUP_SM_CallBack( RBLE_SM_EVENT *event )
{
	;
}

#endif /* USE_FW_UPDATE_PROFILE */

