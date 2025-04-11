/***************************  Filename: DpCfg.c ******************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0                              */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      Einsteinstraße 6        */
/* 0000  000  0   0 000   0 0     00000 0 0000       91074 Herzogenaurach    */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Phone: ++499132744200   */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax:   ++4991327442164  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Technical support:       eMail: support@profichip.com                     */
/*                          Phone: ++49-9132-744-2150                        */
/*                          Fax. : ++49-9132-744-29-2150                     */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/* contents:


*/
/*****************************************************************************/
/** @file
 * Copyright (C) by profichip GmbH   All Rights Reserved. Confidential
 *
 * @brief Handling of PROFIBUS-DP configuration telegram.
 *
 * @author Peter Fredehorst
 * @version $Rev$
 */

/* include hierarchy */
#define F_CPU 16000000UL
#include <string.h>
#include <util/delay.h>
#include "platform.h"
#include "spi_slave.h"
#include "DpAppl.h"

/*---------------------------------------------------------------------------*/
/* defines, structures                                                       */
/*---------------------------------------------------------------------------*/
#define DpApplCfgDataLength ((uint8_t)0x01)     /**< Length of configuration data. */
//default configuration data for startup
//ROMCONST__ 
uint8_t DpApplDefCfg[1] = { 0x11 };  /**< Default configuration data. */

/*---------------------------------------------------------------------------*/
/* local user data definitions                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
/*!
  \brief Init profibus configuration.
*/
void DpCfg_Init( void )
{
   //todo:
   //insert your real configuration data here
   sDpAppl.sCfgData.bLength = DpApplCfgDataLength; // length of configuration data
   memcpy( &sDpAppl.sCfgData.abData[0], &DpApplDefCfg[0], sDpAppl.sCfgData.bLength );
}//void DpCfg_Init( void )

/*---------------------------------------------------------------------------*/
/* function: DpCfg_ChkNewCfgData                                             */
/*---------------------------------------------------------------------------*/
/**
 * @brief Checking configuration data.
 * The function VPC3_Isr() or VPC3_Poll() calls this function if the VPC3+
 * has received a Check_Cfg message and has made the data available in the Cfg buffer.
 *
 * The user has to program the function for checking the received configuration data.
 *
 * @param[in] pbCfgData - address of check configuration data
 * @param[in] bCfgLength - length of configuration data
 *
 * @return see E_DP_CFG_ERROR @see E_DP_CFG_ERROR
 */
E_DP_CFG_ERROR DpCfg_ChkNewCfgData( MEM_UNSIGNED8_PTR pbCfgData, uint8_t bCfgLength )
{
E_DP_CFG_ERROR eRetValue;
uint8_t        i;

   eRetValue = DP_CFG_OK;
   
  /*start_spis();
   //write_spis(bCfgLength);
   //_delay_ms(20);
   
   write_spis(*pbCfgData);
   //write_spis(*pbCfgData+1);
   close_spis();
  */

   if( bCfgLength == sDpAppl.sCfgData.bLength )
   {
   
	/*	if(eRetValue==DP_CFG_OK){
	start_spis();
    write_spis(3);
    close_spis();}
   */
      for( i = 0; i < bCfgLength; i++ )
      {
         if( sDpAppl.sCfgData.abData[ i ] != *pbCfgData )
         {
			/*start_spis();
			write_spis(4);
			close_spis();
			 */
            eRetValue = DP_CFG_FAULT;
			}
         //if( sDpAppl.sCfgData.abData[ i ] != *pbCfgData )

         pbCfgData++;
      }//for( i = 0; i < bCfgLength; i++ )
   }//if( bCfgLength == sDpAppl.sCfgData.bLength )
   else
   {
      eRetValue = DP_CFG_FAULT;
   }//else of if( bCfgLength == sDpAppl.sCfgData.bLength )

   if( ( eRetValue == DP_CFG_OK ) || ( eRetValue == DP_CFG_UPDATE ) )
   { /*start_spis();
    write_spis(7);
    close_spis();*/
      eRetValue = DpDiag_SetCfgOk( eRetValue );
      if( eRetValue != DP_CFG_FAULT )
      {
	 /* start_spis();
    write_spis(8);
    close_spis();
	  */
         VPC3_SetDpState( eDpStateCfgOkStatDiag );
      }
   }//if( ( eRetValue == DP_CFG_OK ) || ( eRetValue == DP_CFG_UPDATE ) )

   DpDiag_AlarmInit();
   
	/*if(eRetValue==DP_CFG_OK){
	start_spis();
    write_spis(6);
    close_spis();}

*/
   return eRetValue;
}//E_DP_CFG_ERROR DpCfg_ChkNewCfgData( MEM_UNSIGNED8_PTR pbCfgData, uint8_t bCfgLength )

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

