/**
 ****************************************************************************************
 *
 * @file		rble_api_fmpl.c
 *
 * @brief	rBLE-Host Find Me Profile Locator I/F API
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

#if (PRF_SEL_FMPL)

/**************************** Define Declaration **************************************/


/**************************** Structure Declaration ***********************************/

/**************************** PUBLIC Memory Declaration *******************************/


/**************************** PRIVATE Memory Declaration ******************************/

/**************************** PRIVATE PROTOTYPE Declaration ***************************/

/******************************* Program Erea *********************************************/

/******************************************************************************************/
/**
 *  @brief Find Me Locator Enable
 *
 *  Find Me Profile Locator Role Enable@n
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
 *  @param[in] *ias			Immediate Alert service details container
 *  @param[in] *call_back	Call Back API Address
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_ERR			Profile Num Over
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_FMP_Locator_Enable(uint16_t conhdl, uint8_t con_type, RBLE_FMP_IAS_CONTENT *ias, RBLE_FMPL_EVENT_HANDLER call_back)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	if(RBLE_OK == ret){
		/* Parameter Check */
		if(NULL == call_back){
			ret = RBLE_PARAM_ERR;
		}else{
			/* Search Free Call Back Handler Memory */
			if ( NULL == RBLE_FMPL_Info.Fml_Handler ) {
				RBLE_FMPL_Info.conhdl      = conhdl;
				RBLE_FMPL_Info.Fml_Handler = call_back;
			} else {
				/* Free Memory None */
				ret = RBLE_ERR;
			}
		}
	
		if(NULL == ias){
			ret = RBLE_PARAM_ERR;
		}
	}
	
	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = ( sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t ) );	/* Add Reserved */
		head_pnt->opcode		= RBLE_CMD_FMP_LOCATOR_ENABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, con_type );
		RBLE_DATA_SET8( Data_p, 0 );			/* Reserved */

		RBLE_DATA_SET16( Data_p, ias->shdl );             /* Start Handle */
		RBLE_DATA_SET16( Data_p, ias->ehdl );             /* End Handle */
		RBLE_DATA_SET16( Data_p, ias->alert_char_hdl );   /* Alert Level Characteristic handle */
		RBLE_DATA_SET16( Data_p, ias->alert_val_hdl );    /* Alert Level Characteristic value handle */
		RBLE_DATA_SET8( Data_p, ias->alert_char_prop );  /* Alert Level Characteristic properties */
	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Find Me Locator Disable
 *
 *  Find Me Profile Locator Role Disable@n
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
RBLE_STATUS RBLE_FMP_Locator_Disable(uint16_t conhdl)
{
	RBLE_STATUS 						ret = RBLE_OK;
	RBLE_HEADER_t						*head_pnt;
	uint8_t								*Data_p;

	/* Status Check */
	if(RBLE_MODE_ACTIVE != rble_mode){
		ret = RBLE_STATUS_ERROR;
	}

	if(RBLE_OK == ret){
		/* Create Command Packet Header */
		head_pnt = (RBLE_HEADER_t *)RBLE_Get_CMD_Buffer();
		head_pnt->packet_type	= RBLE_PACKET_TYPE_COMMAND;
		head_pnt->packet_length = sizeof( uint16_t );
		head_pnt->opcode		= RBLE_CMD_FMP_LOCATOR_DISABLE;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
	}
	
	return ret;
}

/******************************************************************************************/
/**
 *  @brief Set Alert
 *
 *  Alert Setting @n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Create Send Data
 *    -# Send Message to rBLE Core Task
 *
 *  @param[in] conhdl		Connection Handle
 *  @param[in] alert_lvl	Alert Level
 *
 *  @retval		RBLE_OK				Success
 *  @retval		RBLE_PARAM_ERR		Parameter Error
 *  @retval		RBLE_STATUS_ERROR	Status ERROR
 */
/******************************************************************************************/
RBLE_STATUS RBLE_FMP_Locator_Set_Alert(uint16_t conhdl, uint8_t alert_lvl)
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
		head_pnt->packet_length = sizeof( uint16_t ) + sizeof( uint8_t ) + sizeof( uint8_t );	/* Add Reserved */
		head_pnt->opcode		= RBLE_CMD_FMP_LOCATOR_SET_ALERT;

		/* Create Command Payloard */
		Data_p = &head_pnt->data[ 0 ];
		RBLE_DATA_SET16( Data_p, conhdl );
		RBLE_DATA_SET8( Data_p, alert_lvl );
	}
	
	return ret;
}

#endif//#if (PRF_SEL_FMPL)
