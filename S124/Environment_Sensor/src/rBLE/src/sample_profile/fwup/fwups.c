/**
 ****************************************************************************************
 *
 * @file fwups.c
 *
 * @brief Software Update Profile Sender implementation.
 *
 * Copyright(C) 2014 Renesas Electronics Corporation
 *
 * $Rev: 2311 $
 *
 ****************************************************************************************
 */
 
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include	"rble.h"
#include	"rble_app.h"

#include	"db_handle.h"
#include	"rble_api_fwup.h"


#include	<stdio.h>
#include	<string.h>


#ifdef USE_FW_UPDATE_PROFILE

#define RBLE_ATT_SVC_FW_UPDATE		0xFFFF
#define RBLE_MTU_MAX_SIZE			23

typedef enum FWUPS_STATUS_enum {
	FWUPS_STATUS_IDLE,
	FWUPS_STATUS_DISCOVERING,
	FWUPS_STATUS_CONNECTED
}FWUPS_STATUS;

enum
{
	FWUPS_DSC_NONE = 0x00,
	FWUPS_DSC_EVENT_CFG,
	FWUPS_DSC_DISCOVERY_DONE = 0xFF
};

enum
{
	FWUPS_CHAR_DATA_CNTL = 0x00,
	FWUPS_CHAR_DATA,
	FWUPS_CHAR_EVENT,
	FWUPS_CHAR_OTHER = 0xFF
};

typedef struct FWUPS_INFO_t {
	uint16_t					conhdl;
	FWUPS_STATUS				status;
	uint8_t						cfg_dsc_idx;
	uint16_t					last_uuid_req;
	RBLE_FWUPS_EVENT_HANDLER	callback;
	RBLE_FWUS_CONTENT			fwus;
}FWUPS_INFO;

/* internal function definitions */
static void gatt_cmp_evt_handler( RBLE_GATT_EVENT *event );
static void gatt_disc_svc_by_uuid_evt_handler( RBLE_GATT_EVENT *event );
static void gatt_disc_char_all_evt_handler( RBLE_GATT_EVENT *event);
static void gatt_callback( RBLE_GATT_EVENT *event );

static void fwups_disc_svc_send( void );
static void fwups_disc_char_all_send( uint16_t shdl, uint16_t ehdl );
static void fwups_disc_char_desc_send( uint16_t shdl );
static uint16_t fwups_get_next_char_hdl( uint16_t shdl );
static uint8_t fwups_get_char_index( uint8_t *uuid );

/* global variables definitions */
FWUPS_INFO fwups_info;

/* 128bit UUID definitions */
const uint8_t fwups_service_uuid[16]		= RBLE_SVC_FW_UPDATE;
const uint8_t fwups_data_cntl_char_uuid[16]	= RBLE_CHAR_FWUP_DATA_CTRL;
const uint8_t fwups_data_char_uuid[16]		= RBLE_CHAR_FWUP_DATA;

/*
 * IF FUNCTION DEFINITIONS
 ****************************************************************************************
 */
RBLE_STATUS RBLE_FWUP_Sender_Enable( uint16_t conhdl,
									 uint8_t  con_type,
									 RBLE_FWUS_CONTENT *fwus,
									 RBLE_FWUPS_EVENT_HANDLER call_back )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_FWUPS_EVENT	fwup_event;
	
	memset(&fwups_info, 0, sizeof(fwups_info));
	
	/* parameter check */
	if(call_back != NULL) {
		/* initialise GATT */
		result = RBLE_GATT_Enable(gatt_callback);
		if (result == RBLE_OK) {
			/* regist callback function */
			fwups_info.callback = call_back;
			
			/* set conhdl */
			fwups_info.conhdl = conhdl;

			/* config connection, start discovering */
			if(con_type == RBLE_FWUP_CON_CFG) {
				/* start discovering FWUS on peer */
				fwups_disc_svc_send();
			}
			/* normal connection, get saved att details */
			else {
				fwups_info.fwus = *fwus;
				
				/* enable complete callback */
				fwup_event.type = RBLE_FWUP_EVENT_SENDER_ENABLE_COMP;
				fwup_event.param.sender_enable.conhdl = conhdl;
				fwup_event.param.sender_enable.status = RBLE_OK;
				fwup_event.param.sender_enable.fwus   = fwups_info.fwus;
				
				fwups_info.status = FWUPS_STATUS_CONNECTED;
				fwups_info.callback(&fwup_event);
			}
		}
	}
	else {
		/* parameter error */
		result = RBLE_PARAM_ERR;
	}
	return result;
}

RBLE_STATUS RBLE_FWUP_Sender_Disable( uint16_t conhdl )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_FWUPS_EVENT	fwup_event;
	
	if(fwups_info.status == FWUPS_STATUS_CONNECTED) {
		if(conhdl == fwups_info.conhdl) {
			fwups_info.status = FWUPS_STATUS_IDLE;
			
			/* disable complete callback */
			fwup_event.type = RBLE_FWUP_EVENT_SENDER_DISABLE_COMP;
			fwup_event.param.sender_disable.conhdl = conhdl;
			fwup_event.param.sender_disable.status = RBLE_OK;
			
			fwups_info.callback(&fwup_event);
		}
		else {
			/* parameter error */
			result = RBLE_PARAM_ERR;
		}
	}
	else {
		/* status error */
		result = RBLE_STATUS_ERROR;
	}
	
	return result;
}

RBLE_STATUS RBLE_FWUP_Sender_Write_Data_Cntl( uint16_t conhdl,
											  uint8_t type,
											  uint8_t block_num,
											  uint16_t data_size )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_GATT_WRITE_CHAR_REQ wr_char;
	
	if(fwups_info.status == FWUPS_STATUS_CONNECTED) {
		if(conhdl == fwups_info.conhdl) {
			switch(type)
			{
				case RBLE_FWUP_DATA_SEND_START:
					wr_char.val_len	= 4;
					wr_char.value[0] = type;
					wr_char.value[1] = block_num;
					if( (data_size > 1024) || (data_size == 0) ) {
						result = RBLE_PARAM_ERR;
					}
					else {
						memcpy(&wr_char.value[2], &data_size, 2);
					}
					break;
				case RBLE_FWUP_DATA_SEND_COMP:
				case RBLE_FWUP_DATA_SEND_FINISH:
				case RBLE_FWUP_DATA_CHECK_WRITE:
				case RBLE_FWUP_DATA_CHECK_UPDATE:
					wr_char.val_len	= 1;
					wr_char.value[0] = type;
					break;
				default:
					result = RBLE_PARAM_ERR;
					break;
			}
			if( result == RBLE_OK ) {
				wr_char.charhdl		= fwups_info.fwus.data_cntl_val_hdl;
				wr_char.req_type	= RBLE_GATT_WRITE_CHAR;
				
				wr_char.conhdl		 = conhdl;
				wr_char.wr_offset	 = 0;
				wr_char.auto_execute = FALSE;
				
				result = RBLE_GATT_Write_Char_Request(&wr_char);
			}
		}
		else {
			/* parameter error */
			result = RBLE_PARAM_ERR;
		}
	}
	else {
		/* status error */
		result = RBLE_STATUS_ERROR;
	}
	
	return result;
}

RBLE_STATUS RBLE_FWUP_Sender_Write_Data( uint16_t conhdl,
											 uint8_t *data,
											 uint8_t data_size )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_GATT_WRITE_CHAR_REQ wr_char;
	
	if(fwups_info.status == FWUPS_STATUS_CONNECTED) {
		if(conhdl == fwups_info.conhdl) {
			if( (data_size > 0) && (data_size <= (RBLE_MTU_MAX_SIZE - 3)) ) {
				wr_char.val_len	= data_size;
				memcpy(&wr_char.value[0], data, data_size);
				
				wr_char.charhdl		= fwups_info.fwus.data_val_hdl;
				wr_char.req_type	= RBLE_GATT_WRITE_NO_RESPONSE;
				
				wr_char.conhdl		 = conhdl;
				wr_char.wr_offset	 = 0;
				wr_char.auto_execute = FALSE;
				
				result = RBLE_GATT_Write_Char_Request(&wr_char);
			}
			else {
				/* parameter error */
				result = RBLE_PARAM_ERR;
			}
		}
		else {
			/* parameter error */
			result = RBLE_PARAM_ERR;
		}
	}
	else {
		/* status error */
		result = RBLE_STATUS_ERROR;
	}
	
	return result;
}

/*
 * Internal FUNCTION DEFINITIONS
 ****************************************************************************************
 */
static void gatt_cmp_evt_handler( RBLE_GATT_EVENT *event )
{
	if( (event->param.complete.att_code == RBLE_ATT_ERR_ATTRIBUTE_NOT_FOUND) ||
		(event->param.complete.att_code == RBLE_ATT_ERR_NO_ERROR)) {
		switch(fwups_info.last_uuid_req)
		{
			case RBLE_ATT_SVC_FW_UPDATE:
				fwups_disc_char_all_send(fwups_info.fwus.shdl, fwups_info.fwus.ehdl);
				break;
			case RBLE_DECL_CHARACTERISTIC:
			{
					RBLE_FWUPS_EVENT	fwup_event;
					
					fwups_info.cfg_dsc_idx = FWUPS_DSC_DISCOVERY_DONE;
					fwups_info.status = FWUPS_STATUS_CONNECTED;
					
					/* enable complete callback */
					fwup_event.type = RBLE_FWUP_EVENT_SENDER_ENABLE_COMP;
					fwup_event.param.sender_enable.conhdl = fwups_info.conhdl;
					fwup_event.param.sender_enable.status = RBLE_OK;
					fwup_event.param.sender_enable.fwus   = fwups_info.fwus;
					
					fwups_info.callback(&fwup_event);
				}
				break;
			default:
				break;
		}
	}
}

static void gatt_disc_svc_by_uuid_evt_handler( RBLE_GATT_EVENT *event )
{
	if(event->param.disc_svc_by_uuid_cmp.att_code == RBLE_OK) {
		fwups_info.fwus.shdl = event->param.disc_svc_by_uuid_cmp.list[0].start_hdl;
		fwups_info.fwus.ehdl = event->param.disc_svc_by_uuid_cmp.list[0].end_hdl;
	}
}

static void gatt_disc_char_all_evt_handler( RBLE_GATT_EVENT *event)
{
	uint8_t char_idx;
	
	if(event->param.disc_char_all_128_cmp.att_code == RBLE_OK) {
		char_idx = fwups_get_char_index(&event->param.disc_char_all_128_cmp.list.uuid[0]);
		
		switch(char_idx)
		{
			case FWUPS_CHAR_DATA_CNTL:
				fwups_info.fwus.data_cntl_char_hdl	= event->param.disc_char_all_128_cmp.list.attr_hdl;
				fwups_info.fwus.data_cntl_prop		= event->param.disc_char_all_128_cmp.list.prop;
				fwups_info.fwus.data_cntl_val_hdl	= event->param.disc_char_all_128_cmp.list.pointer_hdl;
				break;
			case FWUPS_CHAR_DATA:
				fwups_info.fwus.data_char_hdl		= event->param.disc_char_all_128_cmp.list.attr_hdl;
				fwups_info.fwus.data_prop			= event->param.disc_char_all_128_cmp.list.prop;
				fwups_info.fwus.data_val_hdl		= event->param.disc_char_all_128_cmp.list.pointer_hdl;
				break;
				default:
				break;
		}
	}
}

static void gatt_callback( RBLE_GATT_EVENT *event )
{
	RBLE_FWUPS_EVENT		 fwup_event;
	
	switch(event->type)
	{
		case RBLE_GATT_EVENT_COMPLETE:
			gatt_cmp_evt_handler(event);
			break;
		case RBLE_GATT_EVENT_DISC_SVC_BY_UUID_CMP:
			gatt_disc_svc_by_uuid_evt_handler(event);
			break;
		case RBLE_GATT_EVENT_DISC_CHAR_ALL_128_CMP:
			gatt_disc_char_all_evt_handler(event);
			break;
		case RBLE_GATT_EVENT_WRITE_CHAR_RESP:
			fwup_event.type = RBLE_FWUP_EVENT_SENDER_WRITE_CHAR_RES;
			fwup_event.param.wr_char_resp.conhdl   = fwups_info.conhdl;
			fwup_event.param.wr_char_resp.att_code = event->param.write_char_resp.att_code;
			
			fwups_info.callback(&fwup_event);
			break;
		case RBLE_GATT_EVENT_RESP_TIMEOUT:
		case RBLE_GATT_EVENT_COMMAND_DISALLOWED_IND:
		default:
			break;
	}
}

static void fwups_disc_svc_send( void )
{
	RBLE_GATT_DISC_SVC_REQ disc_svc;
	
	disc_svc.req_type	= RBLE_GATT_DISC_BY_UUID_SVC;
	disc_svc.conhdl		= fwups_info.conhdl;
	disc_svc.start_hdl	= 0x0001u;
	disc_svc.end_hdl	= 0xFFFFu;
	disc_svc.desired_svc.value_size = 16;
	memcpy(&disc_svc.desired_svc.value[0], &fwups_service_uuid, sizeof(fwups_service_uuid));
	
	(void)RBLE_GATT_Discovery_Service_Request(&disc_svc);
	
	fwups_info.last_uuid_req = RBLE_ATT_SVC_FW_UPDATE;
}

static void fwups_disc_char_all_send( uint16_t shdl, uint16_t ehdl )
{
	RBLE_GATT_DISC_CHAR_REQ disc_char = {0};
	
	disc_char.req_type	= RBLE_GATT_DISC_ALL_CHAR;
	disc_char.conhdl	= fwups_info.conhdl;
	disc_char.start_hdl	= shdl;
	disc_char.end_hdl	= ehdl;
	
	(void)RBLE_GATT_Discovery_Char_Request(&disc_char);
	
	fwups_info.last_uuid_req = RBLE_DECL_CHARACTERISTIC;
}

static uint16_t fwups_get_next_char_hdl( uint16_t shdl )
{
	uint16_t hdlarr[2];
	uint16_t ret_hdl = fwups_info.fwus.ehdl;
	uint8_t  i;
	
	hdlarr[0]= fwups_info.fwus.data_cntl_char_hdl;
	hdlarr[1]= fwups_info.fwus.data_char_hdl;
	
	for(i = 0; i < 2; i++)
	{
		if( (shdl < hdlarr[i]) && (hdlarr[i] < ret_hdl) ) {
			ret_hdl = hdlarr[i];
		}
	}
	
	return ret_hdl;
}

static uint8_t fwups_get_char_index( uint8_t *uuid )
{
	uint8_t idx = FWUPS_CHAR_OTHER;
	
	if(0 == memcmp(uuid, &fwups_data_cntl_char_uuid[0], sizeof(fwups_data_cntl_char_uuid))) {
		idx = FWUPS_CHAR_DATA_CNTL;
	}
	else if(0 == memcmp(uuid, &fwups_data_char_uuid[0], sizeof(fwups_data_char_uuid))) {
		idx = FWUPS_CHAR_DATA;
	}
	else {
		idx = FWUPS_CHAR_OTHER;
	}
	
	return idx;
}

#endif /* #ifdef USE_FW_UPDATE_PROFILE */
