/**
 ****************************************************************************************
 *
 * @file		rble_api_pxpm.c
 *
 * @brief	rBLE-Host Proximity Profile Monitor I/F API
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

#if (PRF_SEL_PXPM)

/**************************** Define Declaration **************************************/


/**************************** Structure Declaration ***********************************/

/**************************** PUBLIC Memory Declaration *******************************/


/**************************** PRIVATE Memory Declaration ******************************/


/**************************** PRIVATE PROTOTYPE Declaration ***************************/

/******************************* Program Erea *********************************************/

/******************************************************************************************/
/**
 *  @brief Proximity Monitor Enable
 *
 *  Proximity Profile Monitor Role Enable@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Store Call Back
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in] conhdl		Connection Handle
 *  @param[in] con_type		Connection Type
 *  @param[in] param		Proximity Monitor Parameter
 *  @param[in] *call_back	Call Back API Address
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_ERR			Profile Num Over
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_PXP_Monitor_Enable(uint16_t conhdl, uint8_t con_type, RBLE_PROXI_MON_PARAM *param, RBLE_PXPM_EVENT_HANDLER call_back)
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
		/* Search Free Call Back Handler Memory */
		if ( NULL == RBLE_PXPM_Info.Pmm_Handler ) {
			RBLE_PXPM_Info.conhdl      = conhdl;
			RBLE_PXPM_Info.Pmm_Handler = call_back;
		} else {
			/* Free Memory None */
			ret = RBLE_ERR;
		}
	}

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = ( sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t )
									+ sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) );
		head_pnt->opcode		= RBLE_CMD_PXP_MONITOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, con_type );
		RBLE_DATA_SET8( Data_p, 0 );			/* Reserved */

		RBLE_DATA_SET16( Data_p, param->lls.shdl );		/* Service start handle */
		RBLE_DATA_SET16( Data_p, param->lls.ehdl );		/* Service end handle */
		RBLE_DATA_SET16( Data_p, param->lls.alert_lvl.char_hdl ); /* Alert level char handle */
		RBLE_DATA_SET16( Data_p, param->lls.alert_lvl.val_hdl );  /* alert Level value handle */
		RBLE_DATA_SET8( Data_p, param->lls.alert_lvl.prop );     /* Alert level properties */
		RBLE_DATA_SET8( Data_p, param->lls.alert_lvl.value );    /* Alert value */

		RBLE_DATA_SET16( Data_p, param->ias.shdl );		/* Service start handle */
		RBLE_DATA_SET16( Data_p, param->ias.ehdl );		/* Service end handle */
		RBLE_DATA_SET16( Data_p, param->ias.alert_lvl.char_hdl ); /* Alert level char handle */
		RBLE_DATA_SET16( Data_p, param->ias.alert_lvl.val_hdl );  /* alert Level value handle */
		RBLE_DATA_SET8( Data_p, param->ias.alert_lvl.prop );     /* Alert level properties */
		RBLE_DATA_SET8( Data_p, param->ias.alert_lvl.value );    /* Alert value */

		RBLE_DATA_SET16( Data_p, param->tps.shdl );              /* Service start handle */
		RBLE_DATA_SET16( Data_p, param->tps.ehdl );              /* Service end handle */
		RBLE_DATA_SET16( Data_p, param->tps.txpw_lvl_char_hdl ); /* TX Power level char handle */
		RBLE_DATA_SET16( Data_p, param->tps.txpw_lvl_val_hdl );  /* TX Power Level value handle */
		RBLE_DATA_SET16( Data_p, param->tps.txpw_lvl_cfg_hdl );  /* TX Power Level configuration characteristic value handle */
		RBLE_DATA_SET8( Data_p, param->tps.txpw_lvl_prop );     /* TX Power level properties */
		RBLE_DATA_SET8( Data_p, param->tps.txpw_lvl );          /* TX Power level */

	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Proximity Monitor Disable
 *
 *  Proximity Profile Monitor Role disable@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in] conhdl		Connection Handle
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_PXP_Monitor_Disable(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_PXP_MONITOR_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );

	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Get Alert Level
 *
 *  Get Alert Level @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_PXP_Monitor_Get_Alert_Level(uint16_t conhdl)
{
	RBLE_STATUS 							ret = RBLE_OK;
	RBLE_HEADER_t							*head_pnt;
	uint8_t									*Data_p;

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
		head_pnt->opcode		= RBLE_CMD_PXP_MONITOR_GET_ALERT_LEVEL;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );

	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Set Alert Level
 *
 *  Set Alert Level @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *  @param[in]	svc_code	Svc Code
 *  @param[in]	lvl			Alert Level
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_PXP_Monitor_Set_Alert_Level(uint16_t conhdl,uint8_t svc_code,uint8_t lvl)
{
	RBLE_STATUS 							ret = RBLE_OK;
	RBLE_HEADER_t							*head_pnt;
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t );
		head_pnt->opcode		= RBLE_CMD_PXP_MONITOR_SET_ALERT_LEVEL;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, svc_code );
		RBLE_DATA_SET8( Data_p, lvl );

	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Get Tx Power
 *
 *  Get Tx Power @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Create Message
 *    -# Send Message to Bluetooth protocol Stack
 *
 *  @param[in]	conhdl		Connection Handle
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_PXP_Monitor_Get_Tx_Power(uint16_t conhdl)
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
		head_pnt->opcode		= RBLE_CMD_PXP_MONITOR_GET_TX_POWER;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );

	}
	
	return ret;
}

#endif//#if (PRF_SEL_PXPM)
