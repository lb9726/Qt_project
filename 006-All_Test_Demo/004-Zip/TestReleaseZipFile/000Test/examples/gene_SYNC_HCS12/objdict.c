
/* File generated by gen_cfile.py. Should not be modified. */

#include "objdict.h"

/**************************************************************************/
/* Declaration of the mapped variables                                    */
/**************************************************************************/
UNS16 acceptanceFilter1 = 0x0;		/* Mapped at index 0x2015, subindex 0x00 */
UNS16 acceptanceFilter2 = 0x0;		/* Mapped at index 0x2016, subindex 0x00 */
UNS16 acceptanceFilter3 = 0x0;		/* Mapped at index 0x2017, subindex 0x00 */
UNS16 acceptanceFilter4 = 0x0;		/* Mapped at index 0x2018, subindex 0x00 */
UNS16 mask1 = 0x0;		/* Mapped at index 0x2019, subindex 0x00 */
UNS16 mask2 = 0x0;		/* Mapped at index 0x2020, subindex 0x00 */
UNS16 mask3 = 0x0;		/* Mapped at index 0x2021, subindex 0x00 */
UNS16 mask4 = 0x0;		/* Mapped at index 0x2022, subindex 0x00 */
UNS8 applyDownloadedFilters = 0x0;		/* Mapped at index 0x2023, subindex 0x00 */

/**************************************************************************/
/* Declaration of the value range types                                   */
/**************************************************************************/

UNS32 gene_SYNC_valueRangeTest (UNS8 typeValue, void * value)
{
  switch (typeValue) {
  }
  return 0;
}

/**************************************************************************/
/* The node id                                                            */
/**************************************************************************/
/* node_id default value.*/
UNS8 gene_SYNC_bDeviceNodeId = 0x03;

/**************************************************************************/
/* Array of message processing information */

const UNS8 gene_SYNC_iam_a_slave = 1;

TIMER_HANDLE gene_SYNC_heartBeatTimers[1];

/*
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

                               OBJECT DICTIONARY

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/

/* index 0x1000 :   Device Type. */
                    UNS32 gene_SYNC_obj1000 = 0x0;	/* 0 */
                    subindex gene_SYNC_Index1000[] = 
                     {
                       { RO, uint32, sizeof (UNS32), (void*)&gene_SYNC_obj1000 }
                     };

/* index 0x1001 :   Error Register. */
                    UNS8 gene_SYNC_obj1001 = 0x0;	/* 0 */
                    subindex gene_SYNC_Index1001[] = 
                     {
                       { RO, uint8, sizeof (UNS8), (void*)&gene_SYNC_obj1001 }
                     };

/* index 0x1005 :   SYNC COB ID. */
                    UNS32 gene_SYNC_obj1005 = 0x40000080;	/* 1073741952 */
                    ODCallback_t gene_SYNC_Index1005_callbacks[] = 
                     {
                       NULL,
                     };
                    subindex gene_SYNC_Index1005[] = 
                     {
                       { RW, uint32, sizeof (UNS32), (void*)&gene_SYNC_obj1005 }
                     };

/* index 0x1006 :   Communication / Cycle Period. */
                    UNS32 gene_SYNC_obj1006 = 0x2710;	/* 10000 */
                    ODCallback_t gene_SYNC_Index1006_callbacks[] = 
                     {
                       NULL,
                     };
                    subindex gene_SYNC_Index1006[] = 
                     {
                       { RW, uint32, sizeof (UNS32), (void*)&gene_SYNC_obj1006 }
                     };

/* index 0x1008 :   Manufacturer Device Name. */
                    UNS8 gene_SYNC_obj1008[10] = "GENE_SYNC";
                    subindex gene_SYNC_Index1008[] = 
                     {
                       { RO, visible_string, sizeof (gene_SYNC_obj1008), (void*)&gene_SYNC_obj1008 }
                     };

/* index 0x1016 :   Consumer Heartbeat Time */
                    UNS8 gene_SYNC_highestSubIndex_obj1016 = 0;
                    UNS32 gene_SYNC_obj1016[];
                    subindex gene_SYNC_Index1016[];

/* index 0x1017 :   Producer Heartbeat Time */ 
                    UNS16 gene_SYNC_obj1017 = 0x0;   /* 0 */

/* index 0x1018 :   Identity. */
                    UNS8 gene_SYNC_highestSubIndex_obj1018 = 4; /* number of subindex - 1*/
                    UNS32 gene_SYNC_obj1018_Vendor_ID = 0x0;	/* 0 */
                    UNS32 gene_SYNC_obj1018_Product_Code = 0x0;	/* 0 */
                    UNS32 gene_SYNC_obj1018_Revision_Number = 0x0;	/* 0 */
                    UNS32 gene_SYNC_obj1018_Serial_Number = 0x0;	/* 0 */
                    subindex gene_SYNC_Index1018[] = 
                     {
                       { RO, uint8, sizeof (UNS8), (void*)&gene_SYNC_highestSubIndex_obj1018 },
                       { RO, uint32, sizeof (UNS32), (void*)&gene_SYNC_obj1018_Vendor_ID },
                       { RO, uint32, sizeof (UNS32), (void*)&gene_SYNC_obj1018_Product_Code },
                       { RO, uint32, sizeof (UNS32), (void*)&gene_SYNC_obj1018_Revision_Number },
                       { RO, uint32, sizeof (UNS32), (void*)&gene_SYNC_obj1018_Serial_Number }
                     };

/* index 0x1200 :   Server SDO Parameter. */
                    UNS8 gene_SYNC_highestSubIndex_obj1200 = 2; /* number of subindex - 1*/
                    UNS32 gene_SYNC_obj1200_COB_ID_Client_to_Server_Receive_SDO = 0x0;	/* 0 */
                    UNS32 gene_SYNC_obj1200_COB_ID_Server_to_Client_Transmit_SDO = 0x0;	/* 0 */
                    subindex gene_SYNC_Index1200[] = 
                     {
                       { RO, uint8, sizeof (UNS8), (void*)&gene_SYNC_highestSubIndex_obj1200 },
                       { RO, uint32, sizeof (UNS32), (void*)&gene_SYNC_obj1200_COB_ID_Client_to_Server_Receive_SDO },
                       { RO, uint32, sizeof (UNS32), (void*)&gene_SYNC_obj1200_COB_ID_Server_to_Client_Transmit_SDO }
                     };

/* index 0x2015 :   Mapped variable acceptanceFilter1 */
                    subindex gene_SYNC_Index2015[] = 
                     {
                       { RW, uint16, sizeof (UNS16), (void*)&acceptanceFilter1 }
                     };

/* index 0x2016 :   Mapped variable acceptanceFilter2 */
                    subindex gene_SYNC_Index2016[] = 
                     {
                       { RW, uint16, sizeof (UNS16), (void*)&acceptanceFilter2 }
                     };

/* index 0x2017 :   Mapped variable acceptanceFilter3 */
                    subindex gene_SYNC_Index2017[] = 
                     {
                       { RW, uint16, sizeof (UNS16), (void*)&acceptanceFilter3 }
                     };

/* index 0x2018 :   Mapped variable acceptanceFilter4 */
                    subindex gene_SYNC_Index2018[] = 
                     {
                       { RW, uint16, sizeof (UNS16), (void*)&acceptanceFilter4 }
                     };

/* index 0x2019 :   Mapped variable mask1 */
                    subindex gene_SYNC_Index2019[] = 
                     {
                       { RW, uint16, sizeof (UNS16), (void*)&mask1 }
                     };

/* index 0x2020 :   Mapped variable mask2 */
                    subindex gene_SYNC_Index2020[] = 
                     {
                       { RW, uint16, sizeof (UNS16), (void*)&mask2 }
                     };

/* index 0x2021 :   Mapped variable mask3 */
                    subindex gene_SYNC_Index2021[] = 
                     {
                       { RW, uint16, sizeof (UNS16), (void*)&mask3 }
                     };

/* index 0x2022 :   Mapped variable mask4 */
                    subindex gene_SYNC_Index2022[] = 
                     {
                       { RW, uint16, sizeof (UNS16), (void*)&mask4 }
                     };

/* index 0x2023 :   Mapped variable applyDownloadedFilters */
                    subindex gene_SYNC_Index2023[] = 
                     {
                       { RW, uint8, sizeof (UNS8), (void*)&applyDownloadedFilters }
                     };

const indextable gene_SYNC_objdict[] = 
{
  { (subindex*)gene_SYNC_Index1000,sizeof(gene_SYNC_Index1000)/sizeof(gene_SYNC_Index1000[0]), 0x1000},
  { (subindex*)gene_SYNC_Index1001,sizeof(gene_SYNC_Index1001)/sizeof(gene_SYNC_Index1001[0]), 0x1001},
  { (subindex*)gene_SYNC_Index1005,sizeof(gene_SYNC_Index1005)/sizeof(gene_SYNC_Index1005[0]), 0x1005},
  { (subindex*)gene_SYNC_Index1006,sizeof(gene_SYNC_Index1006)/sizeof(gene_SYNC_Index1006[0]), 0x1006},
  { (subindex*)gene_SYNC_Index1008,sizeof(gene_SYNC_Index1008)/sizeof(gene_SYNC_Index1008[0]), 0x1008},
  { (subindex*)gene_SYNC_Index1018,sizeof(gene_SYNC_Index1018)/sizeof(gene_SYNC_Index1018[0]), 0x1018},
  { (subindex*)gene_SYNC_Index1200,sizeof(gene_SYNC_Index1200)/sizeof(gene_SYNC_Index1200[0]), 0x1200},
  { (subindex*)gene_SYNC_Index2015,sizeof(gene_SYNC_Index2015)/sizeof(gene_SYNC_Index2015[0]), 0x2015},
  { (subindex*)gene_SYNC_Index2016,sizeof(gene_SYNC_Index2016)/sizeof(gene_SYNC_Index2016[0]), 0x2016},
  { (subindex*)gene_SYNC_Index2017,sizeof(gene_SYNC_Index2017)/sizeof(gene_SYNC_Index2017[0]), 0x2017},
  { (subindex*)gene_SYNC_Index2018,sizeof(gene_SYNC_Index2018)/sizeof(gene_SYNC_Index2018[0]), 0x2018},
  { (subindex*)gene_SYNC_Index2019,sizeof(gene_SYNC_Index2019)/sizeof(gene_SYNC_Index2019[0]), 0x2019},
  { (subindex*)gene_SYNC_Index2020,sizeof(gene_SYNC_Index2020)/sizeof(gene_SYNC_Index2020[0]), 0x2020},
  { (subindex*)gene_SYNC_Index2021,sizeof(gene_SYNC_Index2021)/sizeof(gene_SYNC_Index2021[0]), 0x2021},
  { (subindex*)gene_SYNC_Index2022,sizeof(gene_SYNC_Index2022)/sizeof(gene_SYNC_Index2022[0]), 0x2022},
  { (subindex*)gene_SYNC_Index2023,sizeof(gene_SYNC_Index2023)/sizeof(gene_SYNC_Index2023[0]), 0x2023},
};

const indextable * gene_SYNC_scanIndexOD (UNS16 wIndex, UNS32 * errorCode, ODCallback_t **callbacks)
{
	int i;
	*callbacks = NULL;
	switch(wIndex){
		case 0x1000: i = 0;break;
		case 0x1001: i = 1;break;
		case 0x1005: i = 2;*callbacks = gene_SYNC_Index1005_callbacks; break;
		case 0x1006: i = 3;*callbacks = gene_SYNC_Index1006_callbacks; break;
		case 0x1008: i = 4;break;
		case 0x1018: i = 5;break;
		case 0x1200: i = 6;break;
		case 0x2015: i = 7;break;
		case 0x2016: i = 8;break;
		case 0x2017: i = 9;break;
		case 0x2018: i = 10;break;
		case 0x2019: i = 11;break;
		case 0x2020: i = 12;break;
		case 0x2021: i = 13;break;
		case 0x2022: i = 14;break;
		case 0x2023: i = 15;break;
		default:
			*errorCode = OD_NO_SUCH_OBJECT;
			return NULL;
	}
	*errorCode = OD_SUCCESSFUL;
	return &gene_SYNC_objdict[i];
}

/* To count at which received SYNC a PDO must be sent.
 * Even if no pdoTransmit are defined, at least one entry is computed
 * for compilations issues.
 */
UNS8 gene_SYNC_count_sync[1] = {0,};

quick_index gene_SYNC_firstIndex = {
  6, /* SDO_SVR */
  0, /* SDO_CLT */
  0, /* PDO_RCV */
  0, /* PDO_RCV_MAP */
  0, /* PDO_TRS */
  0 /* PDO_TRS_MAP */
};

quick_index gene_SYNC_lastIndex = {
  6, /* SDO_SVR */
  0, /* SDO_CLT */
  0, /* PDO_RCV */
  0, /* PDO_RCV_MAP */
  0, /* PDO_TRS */
  0 /* PDO_TRS_MAP */
};

UNS16 gene_SYNC_ObjdictSize = sizeof(gene_SYNC_objdict)/sizeof(gene_SYNC_objdict[0]); 

CO_Data gene_SYNC_Data = CANOPEN_NODE_DATA_INITIALIZER(gene_SYNC);
