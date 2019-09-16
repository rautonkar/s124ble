/**
 ****************************************************************************************
 *
 * @file		rble_api_lnpc.c
 *
 * @brief	rBLE-Host LNP Collector I/F API
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: v2.00.00 $
 *
 ****************************************************************************************
 */

#include	<string.h>

#include	"rble_api.h"
#include	"rble_host.h"
#include	"rble_trans.h"

#if (PRF_SEL_LNPC)

/**************************** Define Declaration **********************************/
/**************************** Structure Declaration *******************************/
/**************************** PUBLIC Memory Declaration ***************************/
/**************************** PRIVATE Memory Declaration **************************/
/**************************** Function definition *********************************/

/******************************* Program Erea *************************************/
/******************************************************************************************/
/**
 *  @brief LNP Collector xxx 
 *
 *  LNP Collector Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Store Call Back
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	con_type		
 *  @param[in]	*lns			
 *  @param[in]	*dis			
 *  @param[in]	*bas			
 *  @param[in]	call_back		
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_LNP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_LNS_CONTENT *lns, RBLE_DIS_CONTENT *dis, RBLE_BATS_CONTENT *bas, RBLE_LNPC_EVENT_HANDLER call_back)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */
	if(NULL == call_back){
		ret = RBLE_PARAM_ERR;
	}else{
		if ( NULL == RBLE_LNPC_Info.Lnc_Handler ) {
			RBLE_LNPC_Info.conhdl		= conhdl;
			RBLE_LNPC_Info.Lnc_Handler	= call_back;
		} else {
			/* Free Memory None */
			ret = RBLE_ERR;
		}
	}

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )		/* reserved */
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t )
								   + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
								   + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t )
								   + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t )
								   + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t )
								   + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_LNP_COLLECTOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, con_type );
		RBLE_DATA_SET8(  Data_p, 0 );		/*reserved*/
		
		RBLE_DATA_SET16( Data_p, lns->shdl );
		RBLE_DATA_SET16( Data_p, lns->ehdl );
		RBLE_DATA_SET16( Data_p, lns->ln_feature_char_hdl );
		RBLE_DATA_SET16( Data_p, lns->ln_feature_val_hdl );
		RBLE_DATA_SET8(  Data_p, lns->ln_feature_prop );
		RBLE_DATA_SET8(  Data_p, lns->reserved1 );
		RBLE_DATA_SET16( Data_p, lns->location_speed_char_hdl );
		RBLE_DATA_SET16( Data_p, lns->location_speed_val_hdl );
		RBLE_DATA_SET16( Data_p, lns->location_speed_cfg_hdl );
		RBLE_DATA_SET8(  Data_p, lns->location_speed_prop );
		RBLE_DATA_SET8(  Data_p, lns->reserved2 );
		RBLE_DATA_SET16( Data_p, lns->position_quality_char_hdl );
		RBLE_DATA_SET16( Data_p, lns->position_quality_val_hdl );
		RBLE_DATA_SET8(  Data_p, lns->position_quality_prop );
		RBLE_DATA_SET8(  Data_p, lns->reserved3 );
		RBLE_DATA_SET16( Data_p, lns->ln_cp_char_hdl );
		RBLE_DATA_SET16( Data_p, lns->ln_cp_val_hdl );
		RBLE_DATA_SET16( Data_p, lns->ln_cp_cfg_hdl );
		RBLE_DATA_SET8(  Data_p, lns->ln_cp_prop );
		RBLE_DATA_SET8(  Data_p, lns->reserved4 );
		RBLE_DATA_SET16( Data_p, lns->navigation_char_hdl );
		RBLE_DATA_SET16( Data_p, lns->navigation_val_hdl );
		RBLE_DATA_SET16( Data_p, lns->navigation_cfg_hdl );
		RBLE_DATA_SET8(  Data_p, lns->navigation_prop );
		RBLE_DATA_SET8(  Data_p, lns->reserved5 );
		
		RBLE_DATA_SET16( Data_p, dis->shdl );
		RBLE_DATA_SET16( Data_p, dis->ehdl );
		RBLE_DATA_SET16( Data_p, dis->sys_id_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->sys_id_val_hdl );
		RBLE_DATA_SET8(  Data_p, dis->sys_id_prop );
		RBLE_DATA_SET8(  Data_p, dis->reserved );
		RBLE_DATA_SET16( Data_p, dis->model_nb_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->model_nb_val_hdl );
		RBLE_DATA_SET8(  Data_p, dis->model_nb_prop );
		RBLE_DATA_SET8(  Data_p, dis->reserved2 );
		RBLE_DATA_SET16( Data_p, dis->serial_nb_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->serial_nb_val_hdl );
		RBLE_DATA_SET8(  Data_p, dis->serial_nb_prop );
		RBLE_DATA_SET8(  Data_p, dis->reserved3 );
		RBLE_DATA_SET16( Data_p, dis->fw_rev_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->fw_rev_val_hdl );
		RBLE_DATA_SET8(  Data_p, dis->fw_rev_prop );
		RBLE_DATA_SET8(  Data_p, dis->reserved4 );
		RBLE_DATA_SET16( Data_p, dis->hw_rev_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->hw_rev_val_hdl );
		RBLE_DATA_SET8(  Data_p, dis->hw_rev_prop );
		RBLE_DATA_SET8(  Data_p, dis->reserved5 );
		RBLE_DATA_SET16( Data_p, dis->sw_rev_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->sw_rev_val_hdl );
		RBLE_DATA_SET8(  Data_p, dis->sw_rev_prop );
		RBLE_DATA_SET8(  Data_p, dis->reserved6 );
		RBLE_DATA_SET16( Data_p, dis->manuf_name_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->manuf_name_val_hdl );
		RBLE_DATA_SET8(  Data_p, dis->manuf_name_prop );
		RBLE_DATA_SET8(  Data_p, dis->reserved7 );
		RBLE_DATA_SET16( Data_p, dis->ieee_certif_char_hdl );
		RBLE_DATA_SET16( Data_p, dis->ieee_certif_val_hdl );
		RBLE_DATA_SET8(  Data_p, dis->ieee_certif_prop );
		RBLE_DATA_SET8(  Data_p, dis->reserved8 );
		
		RBLE_DATA_SET16( Data_p, bas->shdl );
		RBLE_DATA_SET16( Data_p, bas->ehdl );
		RBLE_DATA_SET16( Data_p, bas->battery_lvl_char_hdl );
		RBLE_DATA_SET16( Data_p, bas->battery_lvl_val_hdl );
		RBLE_DATA_SET16( Data_p, bas->battery_lvl_cfg_hdl );
		RBLE_DATA_SET8(  Data_p, bas->battery_lvl_prop );
		RBLE_DATA_SET8(  Data_p, bas->reserved );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief LNP Collector xxx 
 *
 *  LNP Collector Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_LNP_Collector_Disable(uint16_t conhdl)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */
	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_LNP_COLLECTOR_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Read Characteristic Info
 *
 *  Read Characteristic Information @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	char_code		
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_LNP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */
	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_LNP_COLLECTOR_READ_CHAR;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, char_code );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief LNP Collector xxx 
 *
 *  LNP Collector Role xxx @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	*wr_cp_info		
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_LNP_Collector_Write_LN_Control_Point(uint16_t conhdl, RBLE_LNP_WR_CONTROL_POINT_INFO *wr_cp_info)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */
	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( uint32_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
								   + sizeof( int32_t );
		head_pnt->opcode		= RBLE_CMD_LNP_COLLECTOR_WRITE_LN_CONTROL_POINT;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		
		RBLE_DATA_SET8(  Data_p, wr_cp_info->OpCode );
		RBLE_DATA_SET8(  Data_p, wr_cp_info->reserved );
		RBLE_DATA_SET32( Data_p, wr_cp_info->cumulative_value );
		RBLE_DATA_SET16( Data_p, wr_cp_info->content_mask );
		RBLE_DATA_SET16( Data_p, wr_cp_info->route_num );
		RBLE_DATA_SET8(  Data_p, wr_cp_info->control_val );
		RBLE_DATA_SET8(  Data_p, wr_cp_info->fix_rate );
		RBLE_DATA_SET32( Data_p, wr_cp_info->elevation );
	}

	return ret;
}

/******************************************************************************************/
/**
 *  @brief Write Characteristic Info
 *
 *  Write Characteristic Information @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Status Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in]	conhdl			Connection Handle
 *  @param[in]	char_code		
 *  @param[in]	cfg_val			
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_LNP_Collector_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	/* Parameter Check */
	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )		/* reserved */
								   + sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_LNP_COLLECTOR_WRITE_CHAR;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8(  Data_p, char_code );
		RBLE_DATA_SET8(  Data_p, 0 );		/*reserved*/
		RBLE_DATA_SET16( Data_p, cfg_val );
	}

	return ret;
}

#endif//#if (PRF_SEL_LNPC)
