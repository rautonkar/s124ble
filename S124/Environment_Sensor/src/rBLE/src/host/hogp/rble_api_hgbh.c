/**
 ****************************************************************************************
 *
 * @file	rble_api_hgbh.c
 *
 * @brief	rBLE-Host HOGP Boot Host Role I/F API
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 2156 $
 *
 ****************************************************************************************
 */

#include	<string.h>

#include	"rble_api.h"
#include	"rble_host.h"
#include	"rble_trans.h"

#if (PRF_SEL_HGBH)

/**************************** Define Declaration ****************************************/
/**************************** Structure Declaration *************************************/
/**************************** PUBLIC Memory Declaration *********************************/
/**************************** PRIVATE Memory Declaration ********************************/
/**************************** PRIVATE PROTOTYPE Declaration *****************************/
/******************************* Program Erea *************************************/

/******************************************************************************************/
/**
 *  @brief HOGP Boot Host Role Enable Command
 *
 *  HOGP Boot Host Role Enable @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# check parameter
 *    -# register callback function
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	con_type	Connection Type
 *  @param[in]	*hids		HID Service Infomation
 *  @param[in]	*dis		Device Information Service Infomation
 *  @param[in]	*bas		Battery Service Infomation
 *  @param[in]	call_back	call_back Address
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_BHost_Enable(uint16_t conhdl, uint8_t con_type, uint8_t hids_inst_num, uint8_t bas_inst_num, RBLE_HIDS_CONTENT *hids, RBLE_DIS11_CONTENT *dis, RBLE_BAS_CONTENT *bas, RBLE_HGBH_EVENT_HANDLER call_back)
{
	RBLE_STATUS			ret = RBLE_OK;
	RBLE_HEADER_t		*head_pnt;
	int_t				cnt;

	/* Parameter Check */
	if (NULL != call_back) {
		/* Status Check */
		if (RBLE_MODE_ACTIVE != rble_mode) {
			ret = RBLE_STATUS_ERROR;
		} else {
			/* Search Free Call Back Handler Memory */
			if ( NULL != RBLE_HGBH_Info.Hgbh_Handler ) {
				/* Free Memory None */
				ret = RBLE_ERR;
			} else {
				RBLE_HGBH_Info.conhdl      = conhdl;
				RBLE_HGBH_Info.Hgbh_Handler = call_back;
				
				RBLE_HGBH_Info.Hids_Temp_Memory_Top		= hids;		/* Handle attribute information of HID Service */
				RBLE_HGBH_Info.Bas_Temp_Memory_Top		= bas;		/* Handle attribute information of Battery Service */

				/* Create Command Packet Header */
				head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
				head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
				head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) * 4
										 + ( ( sizeof( uint16_t ) * 33 ) + ( sizeof( uint8_t ) * 20 ) ) * RBLE_HIDS_INST_MAX
										 + sizeof( uint16_t ) * 4 + sizeof( uint8_t ) * 2
										 + ( ( sizeof( uint16_t ) * 6 ) + ( sizeof( uint8_t ) * 2 ) ) * RBLE_BAS_INST_MAX;

				head_pnt->opcode		= RBLE_CMD_HGP_BHOST_ENABLE;

				/* Create Command Payloard */
				RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
				
				RBLE_Data_Set16( conhdl );
				RBLE_Data_Set8( con_type );
				RBLE_Data_Set8( hids_inst_num );
				RBLE_Data_Set8( bas_inst_num );
				RBLE_Data_Set8( 0 );
				for (cnt = 0; cnt < RBLE_HIDS_INST_MAX; cnt++) {
					RBLE_Data_Set16( hids[cnt].shdl );
					RBLE_Data_Set16( hids[cnt].ehdl );
					RBLE_Data_Set16( hids[cnt].protocol_md_char_hdl );
					RBLE_Data_Set16( hids[cnt].protocol_md_val_hdl );
					RBLE_Data_Set8( hids[cnt].protocol_md_prop );
					RBLE_Data_Set8( hids[cnt].reserved );
					RBLE_Data_Set16( hids[cnt].report_input_char_hdl );
					RBLE_Data_Set16( hids[cnt].report_input_val_hdl );
					RBLE_Data_Set16( hids[cnt].report_input_cfg_hdl );
					RBLE_Data_Set16( hids[cnt].input_rep_ref_hdl );
					RBLE_Data_Set8( hids[cnt].report_input_prop );
					RBLE_Data_Set8( hids[cnt].reserved1 );
					RBLE_Data_Set16( hids[cnt].report_output_char_hdl );
					RBLE_Data_Set16( hids[cnt].report_output_val_hdl );
					RBLE_Data_Set16( hids[cnt].output_rep_ref_hdl );
					RBLE_Data_Set8( hids[cnt].report_output_prop );
					RBLE_Data_Set8( hids[cnt].reserved2 );
					RBLE_Data_Set16( hids[cnt].report_feature_char_hdl );
					RBLE_Data_Set16( hids[cnt].report_feature_val_hdl );
					RBLE_Data_Set16( hids[cnt].feature_rep_ref_hdl );
					RBLE_Data_Set8( hids[cnt].report_feature_prop );
					RBLE_Data_Set8( hids[cnt].reserved3 );
					RBLE_Data_Set16( hids[cnt].report_map_char_hdl );
					RBLE_Data_Set16( hids[cnt].report_map_val_hdl );
					RBLE_Data_Set16( hids[cnt].external_rep_ref_hdl );
					RBLE_Data_Set8( hids[cnt].report_map_prop );
					RBLE_Data_Set8( hids[cnt].reserved4 );
					RBLE_Data_Set16( hids[cnt].bootkb_input_char_hdl );
					RBLE_Data_Set16( hids[cnt].bootkb_input_val_hdl );
					RBLE_Data_Set16( hids[cnt].bootkb_input_cfg_hdl );
					RBLE_Data_Set8( hids[cnt].bootkb_input_prop );
					RBLE_Data_Set8( hids[cnt].reserved5 );
					RBLE_Data_Set16( hids[cnt].bootkb_output_char_hdl );
					RBLE_Data_Set16( hids[cnt].bootkb_output_val_hdl );
					RBLE_Data_Set8( hids[cnt].bootkb_output_prop );
					RBLE_Data_Set8( hids[cnt].reserved6 );
					RBLE_Data_Set16( hids[cnt].bootmo_input_char_hdl );
					RBLE_Data_Set16( hids[cnt].bootmo_input_val_hdl );
					RBLE_Data_Set16( hids[cnt].bootmo_input_cfg_hdl );
					RBLE_Data_Set8( hids[cnt].bootmo_input_prop );
					RBLE_Data_Set8( hids[cnt].reserved7 );
					RBLE_Data_Set16( hids[cnt].hid_info_char_hdl );
					RBLE_Data_Set16( hids[cnt].hid_info_val_hdl );
					RBLE_Data_Set8( hids[cnt].hid_info_prop );
					RBLE_Data_Set8( hids[cnt].reserved8 );
					RBLE_Data_Set16( hids[cnt].hid_cp_char_hdl );
					RBLE_Data_Set16( hids[cnt].hid_cp_val_hdl );
					RBLE_Data_Set8( hids[cnt].hid_cp_prop );
					RBLE_Data_Set8( hids[cnt].reserved9 );
					RBLE_Data_Set16( hids[cnt].include_svc_hdl );
					RBLE_Data_Set16( hids[cnt].include_svc_uuid );
					RBLE_Data_Set16( hids[cnt].incl_shdl );
					RBLE_Data_Set16( hids[cnt].incl_ehdl );
				}
				RBLE_Data_Set16( dis->shdl );
				RBLE_Data_Set16( dis->ehdl );
				RBLE_Data_Set16( dis->pnp_id_char_hdl );
				RBLE_Data_Set16( dis->pnp_id_val_hdl );
				RBLE_Data_Set8( dis->pnp_id_prop );
				RBLE_Data_Set8( dis->reserved );
				for (cnt = 0; cnt < RBLE_BAS_INST_MAX; cnt++) {
					RBLE_Data_Set16( bas[cnt].shdl );
					RBLE_Data_Set16( bas[cnt].ehdl );
					RBLE_Data_Set16( bas[cnt].battery_lvl_char_hdl );
					RBLE_Data_Set16( bas[cnt].battery_lvl_val_hdl );
					RBLE_Data_Set16( bas[cnt].battery_lvl_cfg_hdl );
					RBLE_Data_Set16( bas[cnt].battery_lvl_rep_ref_hdl );
					RBLE_Data_Set8( bas[cnt].battery_lvl_prop );
					RBLE_Data_Set8( bas[cnt].reserved );
				}
				
				RBLE_Data_Set_End( );		/* End Packet Data Setting */
			}
		}
	} else {
		ret = RBLE_PARAM_ERR;
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief HID over GATT Boot Host Role Disable Command
 *
 *  HID over GATT Boot Host Role Disable @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# check mode
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl		Connection Handle
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_BHost_Disable(uint16_t conhdl)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_HGP_BHOST_DISABLE;

		/* Create Command Payloard */
		RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
		
		RBLE_Data_Set16( conhdl );
		
		RBLE_Data_Set_End( );		/* End Packet Data Setting */
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Read Characteristic Command
 *
 *  Read Characteristic Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# check mode
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	inst_idx	Service Instance Index
 *  @param[in]	char_code	Characteristic Code
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_BHost_Read_Char(uint16_t conhdl, uint8_t inst_idx, uint8_t char_code)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) * 2;
		head_pnt->opcode		= RBLE_CMD_HGP_BHOST_READ_CHAR;

		/* Create Command Payloard */
		RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
		
		RBLE_Data_Set16( conhdl );
		RBLE_Data_Set8( inst_idx );
		RBLE_Data_Set8( char_code );
		
		RBLE_Data_Set_End( );		/* End Packet Data Setting */
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Read Characteristic by UUID Command
 *
 *  Read Characteristic Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# check mode
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	inst_idx	Service Instance Index
 *  @param[in]	char_code	Characteristic Code
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_BHost_Read_By_UUID_Char(uint16_t conhdl, uint8_t inst_idx, uint8_t char_code)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) * 2;
		head_pnt->opcode		= RBLE_CMD_HGP_BHOST_READ_CHAR_BY_UUID;

		/* Create Command Payloard */
		RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
		
		RBLE_Data_Set16( conhdl );
		RBLE_Data_Set8( inst_idx );
		RBLE_Data_Set8( char_code );
		
		RBLE_Data_Set_End( );		/* End Packet Data Setting */
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Write Characteristic Command
 *
 *  Write Characteristic Infomation @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# check mode
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	inst_idx	Service Instance Index
 *  @param[in]	char_code	Characteristic Code
 *  @param[in]	cfg_val		Configuration Value
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_BHost_Write_Char(uint16_t conhdl, uint8_t inst_idx, uint8_t char_code, uint16_t cfg_val)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) * 2 + sizeof( uint8_t ) * 2;
		head_pnt->opcode		= RBLE_CMD_HGP_BHOST_WRITE_CHAR;

		/* Create Command Payloard */
		RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
		
		RBLE_Data_Set16( conhdl );
		RBLE_Data_Set8( inst_idx );
		RBLE_Data_Set8( char_code );
		RBLE_Data_Set16( cfg_val );
		
		RBLE_Data_Set_End( );		/* End Packet Data Setting */
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Set Report Command
 *
 *  Set Report Command @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# check mode
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	inst_idx	Service Instance Index
 *  @param[in]	report		Report Value
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_BHost_Set_Report(uint16_t conhdl, uint8_t inst_idx, RBLE_HGP_REPORT_DESC *report)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								cnt;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) * 4 + sizeof( uint8_t ) * RBLE_HIDS_REPORT_MAX + sizeof( uint16_t );
#if ((RBLE_HIDS_REPORT_MAX % 2) != 0)
		head_pnt->packet_length += sizeof( uint8_t );
#endif
		head_pnt->opcode		= RBLE_CMD_HGP_BHOST_SET_REPORT;

		/* Create Command Payloard */
		RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
		
		RBLE_Data_Set16( conhdl );
		RBLE_Data_Set8( inst_idx );
		RBLE_Data_Set8( 0 );
		RBLE_Data_Set8( report->device_type );
		RBLE_Data_Set8( report->report_type );
		for (cnt = 0; cnt < RBLE_HIDS_REPORT_MAX; cnt++) {
			RBLE_Data_Set8( report->value[cnt] );
		}
#if ((RBLE_HIDS_REPORT_MAX % 2) != 0)
		RBLE_Data_Set8( report->reserved );
#endif
		RBLE_Data_Set16( report->value_size );
		
		RBLE_Data_Set_End( );		/* End Packet Data Setting */
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Set Protocol Mode Command
 *
 *  Set Protocol Mode Command @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# check mode
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl		        Connection Handle
 *  @param[in]	inst_idx	        Service Instance Index
 *  @param[in]	protocol_mode_val   Protocol Mode Value
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_BHost_Write_Protocol_Mode(uint16_t conhdl, uint8_t inst_idx, uint8_t protocol_mode_val)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) * 2;
		head_pnt->opcode		= RBLE_CMD_HGP_BHOST_SET_PROTOCOL_MODE;

		/* Create Command Payloard */
		RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
		
		RBLE_Data_Set16( conhdl );
		RBLE_Data_Set8( inst_idx );
		RBLE_Data_Set8( protocol_mode_val );
		
		RBLE_Data_Set_End( );		/* End Packet Data Setting */
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Data Output Command
 *
 *  Data Output Command @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# check mode
 *    -# Create Send Data
 *    -# Send Message to HOST rBLE Task
 *
 *  @param[in]	conhdl	    Connection Handle
 *  @param[in]	inst_idx    Service Instance Index
 *  @param[in]	report      Report Value
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status Error
 */
/******************************************************************************************/
RBLE_STATUS RBLE_HGP_BHost_Data_Output(uint16_t conhdl, uint8_t inst_idx, RBLE_HGP_REPORT_DESC *report)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								cnt;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	} else {
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) * 4 + sizeof( uint8_t ) * RBLE_HIDS_REPORT_MAX + sizeof( uint16_t );
#if ((RBLE_HIDS_REPORT_MAX % 2) != 0)
		head_pnt->packet_length += sizeof( uint8_t );
#endif
		head_pnt->opcode		= RBLE_CMD_HGP_BHOST_DATA_OUTPUT;

		/* Create Command Payloard */
		RBLE_Data_Set_Start( (RBLE_HEADER_t *)head_pnt );		/* Start Packet Data Setting */
		
		RBLE_Data_Set16( conhdl );
		RBLE_Data_Set8( inst_idx );
		RBLE_Data_Set8( 0 );
		RBLE_Data_Set8( report->device_type );
		RBLE_Data_Set8( report->report_type );
		for (cnt = 0; cnt < RBLE_HIDS_REPORT_MAX; cnt++) {
			RBLE_Data_Set8( report->value[cnt] );
		}
#if ((RBLE_HIDS_REPORT_MAX % 2) != 0)
		RBLE_Data_Set8( report->reserved );
#endif
		RBLE_Data_Set16( report->value_size );
		
		RBLE_Data_Set_End( );		/* End Packet Data Setting */
	}
	return ret;
}

#endif//#if (PRF_SEL_HGBH)
