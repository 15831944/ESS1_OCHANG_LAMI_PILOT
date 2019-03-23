#ifndef __AppDefine_H__
#define __AppDefine_H__

#pragma comment(lib, "../../VLib/General/NAppDll.lib")
#pragma comment(lib, "../../VLib/General/NGuiDll.lib")
#pragma comment(lib, "../../VLib/General/NPioControlDll.lib")
#pragma comment(lib, "../../VLib/General/NPlotDll.lib")
#pragma comment(lib, "../../VLib/General/NVisionDll.lib")
#pragma comment(lib, "../../VLib/General/NMultiFuncDll.lib")

#pragma comment(lib, "../../VLib/HW/IO_Board/AXL.lib")

#pragma comment(lib, "../../VLib/HW/Grabber/Euresys/MultiCam/Lib/MultiCam.lib")

// ���� ���� ����
//#define MAIN_WINDOW_TITLE      _T(" CNA NLaminatorApp Ver 3.0(17.02.10) ")
#define MAIN_WINDOW_TITLE      _T("NLaminatorApp Ver3.91 for ESS in Korea")
// ���� ���� ����

// ���� �ε�, ���� ���� �н��� ��ġ ����
#define SAVE_DATA_PATH         _T("D:\\DATABASE\\DATA\\")
#define LOG_DATA_FILE             "C:\\DATABASE\\LOG\\"
#define SAVE_DATA_PATH_CSV     _T("C:\\DATABASE\\DATA_CSV\\")
#define SAVE_SHORTDATA_PATH    _T("C:\\DATABASE\\SHORTDATA\\")
#define SAVE_EXCEL_PATH        _T("C:\\DATABASE\\EXCEL\\")
#define SAVE_PLOT_PATH         _T("C:\\DATABASE\\PLOT\\")
#define SAVE_IMAGE_PATH        _T("DATABASE\\IMAGE\\")

#define MODEL_FILE_PATH        _T("..\\MODEL\\")
#define SETTING_DATA_FILE      _T("..\\Config.cfg")
#define ETC_DATA_FILE          _T("..\\Count.cfg")
#define MODEL_IMAGE_PATH       _T("..\\MODELIMAGE\\")
#define DEF_SHORTPLOT_PATHNAME _T("..\\PLOT\\TestShortage.plt")
#define DEF_GAUSPLOT_PATHNAME  _T("..\\PLOT\\Gausian.plt")
#define DEF_LISTPLOT_PATHNAME  _T("..\\PLOT\\List.plt")	
#define DEF_MINPLOT_PATHNAME   _T("..\\PLOT\\Min.plt")
#define DEF_MAXPLOT_PATHNAME   _T("..\\PLOT\\Max.plt")
#define DEF_RESULT_PATHNAME    _T("..\\PLOT\\")
#define DEF_ETCDATA_FILE       _T("..\\SensorLimit.txt")  

// ���� �ε�, ���� ���� �н��� ��ġ ����

// ���� �޼���, ���� ������ ����
#define DEF_MSG_READY		   _T("READY")
#define DEF_MSG_IMAGE_GRAB     _T("Image Grabbing ...")
#define DEF_MSG_INSPECT        _T("On Inspection ...")
#define DEF_MSG_COMPLETE 	   _T("Inspection has been completed .")
#define DEF_MSG_UMAC_ERROR     _T("UMac. Ethenet communication open Error  !")
#define DEF_MSG_PLC_ERROR      _T("PLC. Ethenet communication open Error  !")
#define DEF_MSG_RS232_ERROR    _T("RS232C communication open Error !")
#define DEF_SOCKET_INIT_FAIL   _T(" Can not initialize windows socket ") 
// ���� �޼���, ���� ������ ����

// ������ Option ���� 
#define _DUO_CELL                        1   // 0:ī�޶�� 1�� CELL�� �˻�, 1:ī�޶�� 2�� CELL�� �˻�
#define _EURESIS_CAM                     1   // 0:ī�޶� ������� ����, 1:EURESIS ���带 ����Ͽ� ������ ȹ��
#define _UMAC                            1   // 0:UMAC��� ������� ����, 1:UMAC�� �����Ͽ� Ethernet ��� 
#define _OMRON                           0
#define _MELSEC                          1
#define _DUO_SHORT                       1   // 0:SHORT TESTER�� �ϳ��� �����, 1:SHORT TESTER�� �� �� ����Ͽ� ����͸� ��
#define _PCI7230                         0   // 1:PCI-7230 DIO���带 �����
#define _AJIN_EXT                        1   // 1:AJIN_EXT DIO���带 �����
#define _GAUGE_RNR                       0   // 0:�������� ���� ����·� ��������, 1:�ݺ����� ����Ÿ ��½� �����
#define _CNA_LAMI                        0
#define _RS232C_LVS_TESTER               0
#define _RS232C_CHROMA_TESTER            0
#define _RS232C_CHROMA_TESTER2           0
#define _NG_DISP                         1

#define _OCHANG_NO3						 1   // ��â ESS��̿� 3ȣ�⸸ MONO DECA�� �ƴ϶� MONO�̴�.
#define _SHORTAGE_USE				     0

//#define _LAMI_PC1                        0 // shsong : �̰� ������ ���� ���ð� �Է¹޴� �ɷ� ������.
// ������ Option ���� 

// DCF, ����ũ�� �� ������ ZOOM ���� 
/*
#define CAMERA_DCF_FILE        "C:\\APPL\\NLaminateApp\\EXE\\HV-B2535x_RG_10TAP"
#define IMAGE_WIDTH		    	     4096
#define IMAGE_HEIGHT			     3072
#define DEF_INIT_ZOOM_SCALE          0.23f
#define DEF_MINI_ZOOM_SCALE          0.14f
*/

//#define CAMERA_DCF_FILE        "C:\\APPL\\NLaminateApp\\EXE\\Falcon2-12M_8bit8TAP 4096x3072_RG"
#define CAMERA_DCF_FILE          "S-25A30_P32RG"
//#define CAMERA_DCF_FILE2		 "HV-M2535C_RG_10TAP"

#define IMAGE_WIDTH		    	     5120
#define IMAGE_HEIGHT			     5120
//#define IMAGE_WIDTH		   	     2592
//#define IMAGE_HEIGHT			     1944

#define DEF_INIT_ZOOM_SCALE          0.175f
#define DEF_MINI_ZOOM_SCALE          0.10f
// DCF, ����ũ�� �� ������ ZOOM ���� 

// ���� ���α׷� ȭ�� ũ�� ���� 
#define APP_WIDTH                     1280
#define APP_HEIGHT                    1024
// ���� ���α׷� ȭ�� ũ�� ���� 

// �ҷ����� ����� �ҷ������� ũ�� ���� 
#define SMALL_IMAGE_WIDTH              932
#define SMALL_IMAGE_HEIGHT             780
// �ҷ����� ����� �ҷ������� ũ�� ���� 

// ������� ȭ���� ũ�⸦ ���� 
#define FRAME_WIDTH					   932
#define FRAME_HEIGHT				   860
// ������� ȭ���� ũ�⸦ ���� 

// ī�޶��� ��, ī�޶� ID���� 
#define VIEW_MAX_NUM                     2
#define CAM_MAX_NUM						 1
#define CAM_UP                           0
#define CAM_DN                           1
// ī�޶��� ��, ī�޶� ID���� 

// �˻��׸��� �ִ� ��, �׸񿬰� �˻��׸��� �ִ� �� ���� 
#define GOM_MAX_NUM					    100
#define LINK_MAX_NUM                    50
#define SAMPLE_MAX_NUM                  20
// �˻��׸��� �ִ� ��, �׸񿬰� �˻��׸��� �ִ� �� ���� 

#define LIGHT_MAX_NUM                   10
#define MODEL_MAX_NUM                  100
#define PROD_MAXDATA_NUM               500
#define OFFSET_MAX_NUM                 200
#define SHORT_BUFFER_MAX_NUM            10
#define DEF_LIGHT_MAX_NUM              500
#define DEF_LIGHT_CHANNEL_NUM           10

// MESSAGE EVENT ID
#define WM_SENDDATA_PLC         (WM_USER+1001)
#define UM_SPLASH_INIT			(WM_USER+1002)
#define WM_DELETE_DATA          (WM_USER+1003)
#define WM_LEFTMOUSEDOWN        (WM_USER+1004)
// MESSAGE EVENT ID

// SPLASH ������ PROCESS ID
#define ID_LOADING_FILE			 		 0
#define ID_INITIAL_CONTROL				 1
#define ID_INITIAL_REGIST				 2
#define ID_INITIAL_FINISH				 3
// SPLASH ������ PROCESS ID

// TIMER ID
#define TIMER_TEMP_INSP                  0
#define TIMER_TOTAL_INSP                 3
#define TIMER_ON_PROCESS                 4
#define TIMER_SHORT_PROC                 5

#define DEF_PROC_TIMEOUT               0.5

#define TIMEMSG_GRAB_CAM_UP              0
#define TIMEMSG_GRAB_CAM_DN              1
#define TIMEMSG_COMM_ERROR               2
#define TIMEMSG_PIO_INSP_CLEAR           3
#define TIMEMSG_SHORT_DATA               4
// TIMER ID

// MAIN DIALOG �޴� ID 
#define DEF_BUTTON_AUTO 		         0
#define DEF_BUTTON_RESULT                1
#define DEF_BUTTON_SETUP		         2
#define DEF_BUTTON_PLOT                  3
#define DEF_BUTTON_SEARCH			     4
#define DEF_BUTTON_EXIT			         5
// MAIN DIALOG �޴� ID 

// ��� ���� ID
#define DEF_LANG_KOREA                   0
#define DEF_LANG_ENGLISH                 1
#define DEF_LANG_CHANA                   2
// ��� ���� ID

// SHORT DIALOG �޴� ID 
#define DEF_BUTTON_SHORT_MAIN            0
#define DEF_BUTTON_SHORT_SETUP           1
#define DEF_BUTTON_SHORT_QUERY           2
#define DEF_BUTTON_SHORT_EXIT            3
// SHORT DIALOG �޴� ID 

#define DEF_V1_SMALL_LEFT    		     0
#define DEF_V1_SMALL_TOP			     0
#define DEF_V1_SMALL_WIDTH			   600
#define DEF_V1_SMALL_HEIGHT			   424

#define DEF_V2_SMALL_LEFT    		     0
#define DEF_V2_SMALL_TOP			   426
#define DEF_V2_SMALL_WIDTH			   600
#define DEF_V2_SMALL_HEIGHT			   424


// MAIN DIALOG ��ġ 
#define DEF_DIALOG_LEFT				     2
#define DEF_DIALOG_TOP				    58
#define DEF_DIALOG_WIDTH			  1280
#define DEF_DIALOG_HEIGHT			   892
// MAIN DIALOG ��ġ   

// SUB BAR ��ġ
#define DEF_SUBBAR_LEFT		             1
#define DEF_SUBBAR_TOP	 			   952
#define DEF_SUBBAR_WIDTH			  1288
#define DEF_SUBBAR_HEIGHT			    66
// SUB BAR ��ġ  

// MAIN BAR ��ġ 
#define DEF_MAINBAR_LEFT			     0
#define DEF_MAINBAR_TOP				     0
#define DEF_MAINBAR_WIDTH			  1288
#define DEF_MAINBAR_HEIGHT			    57
// MAIN BAR ��ġ   

// ����Ʈ�� ��ġ
#define DEF_LISTLOC_LEFT               944
#define DEF_LISTLOC_TOP                 41
#define DEF_LISTLOC_WIDTH              336
#define DEF_LISTLOC_HEIGHT             620
// ����Ʈ�� ��ġ

// TEST SHORT DIALOG ��ġ
#define DEF_TESTSHORT_LEFT            1280
#define DEF_TESTSHORT_TOP                0
#define DEF_TESTSHORT_WIDTH           1280
#define DEF_TESTSHORT_HEIGHT          1024
// TEST SHORT DIALOG ��ġ

// PLOT�� �ּ�/�ִ� ũ�� 
#define PLOT_MIN_WIDTH                 625
#define PLOT_MIN_HEIGHT                105
//#define PLOT_MIN_HEIGHT               93
#define PLOT_MAX_WIDTH                1260
#define PLOT_MAX_HEIGHT                845
// PLOT�� �ּ�/�ִ� ũ�� 

#define DEF_SHORT_PLOT_NUM              56
#define DEF_GAUSE_PLOT_NUM              57
#define DEF_SHORT_GAUSE_PLOT_NUM        58
#define DEF_LIST_PLOT_NUM               59
#define DEF_SHORT_LIST_PLOT_NUM         60

#define DEF_IMAGE_MAX_NUM               10
#define DEF_IMAGE_REDUCE_RATE            4
#define DEF_QUEUE_MAX_NUM               10
//#define DEF_EXPOSURE_TIME              300
#define DEF_EXPOSURE_TIME              500

#define DEF_LIGHT_NUM                   10
#define DEF_REMOTE_LIGHT_COMPORT         8
#define DEF_CONT_EXIT_SETUP              0
#define DEF_CONT_LIGHT                   1

// PLOT�� �ּ�/�ִ� ũ�� 
#define PLOT_PAGE_MAX_NUM                2
#define PLOT_MAX_NUM                    18
#define PLOT_ONE_MAX_NUM                 3
#define PLOT_MAX_DATA_NUM              100
#define PLOT_MIN_WIDTH                 625
#define PLOT_MIN_HEIGHT                105
#define PLOT_MAX_WIDTH                1260
#define PLOT_MAX_HEIGHT                845

#define PLOT_MODE_XY                     0
#define PLOT_MODE_TY                     1
#define PLOT_MODE_STICK                  2
#define PLOT_MODE_GAUSIAN                3
// PLOT�� �ּ�/�ִ� ũ�� 

#define DEF_DIO_PORTNUM                 16
#define PIO_IN_READ_RECIPE       		 0
#define PIO_IN_READ_SHORT			     1
#define PIO_IN_READ_RESET                3
#define PIO_IN_READ_YALIGN_PORT          5
#define PIO_IN_READ_CAM1			     8
#define PIO_IN_READ_CAM2                 9
#define PIO_IN_READ_IS_FRONT            11
#define PIO_IN_READ_IS_REAR             12

#define PIO_OUT_READY_PORT               0
#define PIO_OUT_COMM_PORT   	 	     1
#define PIO_OUT_REAR_OK_PORT             4
#define PIO_OUT_REAR_NG_PORT             5
#define PIO_OUT_FRONT_OK_PORT            6
#define PIO_OUT_FRONT_NG_PORT            7
#define PIO_OUT_EPC_PORT                 8
#define PIO_OUT_REAR_MISMATCH_PORT      10
#define PIO_OUT_FRONT_MISMATCH_PORT     11

#define DEF_IP_ADDRESS           "192.168.251.5" 

#define OMRON_IN_LOT_ID			  "PLC_Lot_ID[00]"
#define OMRON_IN_PROD_MODEL       "PLC_Prod_Model"
#define OMRON_IN_RECIPI_NUM       "PLC_Recipe_Number"
#define OMRON_IN_STOP_PORT        "PLC_Machine_Status"
#define OMRON_IN_COMM_PORT        "PLC_Comm_Status"
#define OMRON_OUT_VISION          "PLC_VISION_Host_Data[000]"

// PC<-UMAC ���� 
#define UMAC_IN_COMM                  5000
#define UMAC_IN_ERROR_CNT             5150
// PC<-UMAC ���� 

// PC->UMAC ���� 
//#define UMAC_OUT_TRIGGER_CAM1       4259    
//#define UMAC_OUT_TRIGGER_CAM2       4260
#define UMAC_OUT_TRIGGER_CAM1         4362    
#define UMAC_OUT_TRIGGER_CAM2         4363
#define UMAC_OUT_REJECT_FRONT         5102   // ESS������ �˻�Ϸ�:0, �Ϲݺҷ�:1, �Ǻҷ�:2 ��ǰ:3, �˻���:11 �� ���ǵǾ� �־���.
#define UMAC_OUT_REJECT_REAR          5101
#define UMAC_OUT_FINAL_CUT            5109
#define UMAC_OUT_EPC                  5104
#define UMAC_OUT_T2B                  5105
// PC->UMAC ���� 

#define UMAC_OUT_MOVE_TRIG            1251
#define UMAC_OUT_MOVE_TARG            1252
#define UMAC_OUT_CAM_SEL              1253
#define UMAC_OUT_VELLO                1255
#define UMAC_OUT_ACCEL                1256

#if _CNA_LAMI
	#define PLC_IN_EPC_N1           _T("D3020")
	#define PLC_IN_EPC_N2           _T("D3021")
	#define PLC_IN_EPC_N3           _T("D3022")
	#define PLC_IN_EPC_COMP         _T("D3023")

	#define PLC_IN_PROD_MODEL       _T("D3000")
	#define PLC_IN_RECIPI_NUM       _T("D3001")
	#define PLC_IN_RECIPI_NAME      _T("D3002")
	#define PLC_IN_PROD_MODE        _T("D3012")
	#define PLC_IN_STOP_PORT        _T("D3013")
	#define PLC_IN_COMM_PORT        _T("D3270")

	#define PLC_IN_TEST_VOLT1       _T("D3102")	//SHORT1 200
	#define PLC_IN_LIMIT_CURR1      _T("D3103")	//SHORT1 5
	#define PLC_IN_START_TIME1      _T("D3104")	//SHORT1 0
	#define PLC_IN_RAMP_TIME1       _T("D3105")	//SHORT1 3 //20 modify
	#define PLC_IN_HOLD_TIME1       _T("D3106")	//SHORT1 2 //20 modify
	#define PLC_IN_CONTACT_CURR1    _T("D3107")	//SHORT1 0

	#define PLC_IN_TEST_VOLT2       _T("D3108")	//SHORT2 200
	#define PLC_IN_LIMIT_CURR2      _T("D3109")	//SHORT2 5
	#define PLC_IN_START_TIME2      _T("D3110")	//SHORT2 0
	#define PLC_IN_RAMP_TIME2       _T("D3111")	//SHORT2 3
	#define PLC_IN_HOLD_TIME2       _T("D3112")	//SHORT2 2
	#define PLC_IN_CONTACT_CURR2    _T("D3113")	//SHORT2 0

	#define PLC_OUT_VISION_DATA     _T("D3100")
	#define PLC_OUT_SHORT_DATA      _T("D3300")
	#define PLC_OUT_COMM_DATA       _T("D3270")
	#define PLC_OUT_SHORT_SETUP     _T("D3250")
	#define PLC_OUT_EPC_CAMUP       _T("D3024")
	#define PLC_OUT_EPC_CAMDN       _T("D3025")
#else
///////////////////////////////////////////////////////
//////////////    �ű�  �޸� ����   /////////////////
///////////////////////////////////////////////////////
	#define PLC_IN_PROD_MODEL       _T("D2400")
	#define PLC_IN_RECIPI_NUM       _T("D2401")
	#define PLC_IN_RECIPI_NAME      _T("D2402")
	#define PLC_IN_PROD_MODE        _T("D2412")
	#define PLC_IN_STOP_PORT        _T("D2413")
	#define PLC_IN_COMM_PORT        _T("D2414")

	#define PLC_IN_UNWINDER_UP_POS			_T("D2478")
	#define PLC_IN_UNWINDER_DN_POS			_T("D2479")
	#define PLC_IN_UNWINDER_UP_BARCODE    _T("D2480")
	#define PLC_IN_UNWINDER_DN_BARCODE    _T("D2490")

	#define PLC_OUT_VISION_DATA     _T("D2500")
	#define PLC_OUT_SHORT_DATA      _T("D2570")
	#define PLC_OUT_EPC_DATA        _T("D2600")
///////////////////////////////////////////////////////
//////////////    �ű�  �޸� ����   /////////////////
///////////////////////////////////////////////////////
#endif

// LIST UI ID ����
#define LIST_MAIN_ID                     0
#define LIST_INFO_ID                     1
#define LIST_RESULT_ID                   2
#define LIST_SETUP_ID                    3
#define LIST_SHORT_ID                    4

// ���� MACRO ����
#define PI               3.141592653589793
#define RAD                   (PI / 180.0)
#define DEG                   (180.0 / PI)
// ���� MACRO ����

typedef enum
{
	UNW_UPPER_POS = 0,
	UNW_DOWN_POS  = 1,
	UNW_UPPER_BARCODE = 2,
	UNW_DOWN_BARCODE  = 12,
	UNW_INFO_END = 22
};

typedef struct
{
	float   x;
	float   y;
} POINT2;

typedef struct 
{
	TCHAR   m_strModelName[100];
	TCHAR   m_strPassWord[50];
	TCHAR   m_strImgPath[50];
	TCHAR 	m_strSelDriver[50];

	BOOL	m_bOKImageSave[VIEW_MAX_NUM];
	BOOL	m_bNGImageSave[VIEW_MAX_NUM];
	BOOL    m_bExecInsp[VIEW_MAX_NUM];
	BOOL	m_bSaveImageType[VIEW_MAX_NUM];

	long	m_nDataPeriod;
	long	m_nSaveTypeImage;
	float   m_fCamScale[VIEW_MAX_NUM];

	long    m_nCamSel;
	long    m_nPortNo;
	long    m_nCOM1;
	long    m_nCOM2;
	long    m_nSelLanguage;

	long    m_nEpcN1;
	long    m_nEpcN2;
	long    m_nEpcN3;
	long    m_nCompensRate;

	long    m_nOldDataHour;
	long    m_nOldShortHour;
	long    m_nSpace[12];
	long    m_OrderNum[MODEL_MAX_NUM];
	TCHAR   m_ModelName[MODEL_MAX_NUM][100];

	BOOL    m_bExecCutOffset;
	BOOL    m_bExecEPCOffset;
	BOOL    m_bExecT2BOffset;
	BOOL    m_bExecDataTrans;
	BOOL    m_bRecipeOption;

	long    m_nSkipCnt;
	long    m_nDaqUnitCnt;
	long    m_nTotalShortCount;

	float   m_fTestVolt;
	float   m_fLimitCurr;
	float   m_fStartDelayTime;
	float   m_fRampingTime;
	float   m_fHoldingTime;
	float   m_fContactCurr;

	long    m_nOldHour;
	long    m_nSelUpDn;
	long    m_nLightV[DEF_LIGHT_MAX_NUM];
	long    m_nExposureTime[CAM_MAX_NUM];

	SYSTEMTIME  m_sysOldTime;
	
	BOOL    m_bSWGrab;
	long    m_nDelayTime;
//	BOOL    m_bWayLefttoRight;                     // shsong ���� ���� ���⿡ ���� FRONT CELL�� REAR CELL�� NG ��ȣ ���� ����ġ
} SETTING_DATA;

typedef struct 
{
	long    m_nTotalInspCount;
	long    m_nTotalGoodCount;
	long    m_nTotalBadCount;
	long    m_nUpBadCount;
	long    m_nDnBadCount;
	long    m_nUpFBadCount;
	long    m_nUpRBadCount;
	long    m_nDnFBadCount;
	long    m_nDnRBadCount;
	long    m_CntROI[CAM_MAX_NUM][GOM_MAX_NUM];
	long    m_CntLink[CAM_MAX_NUM][LINK_MAX_NUM];
	long    m_nPriorityNum;
	long    m_nSpace[18];
} ETC_DATA;

typedef struct
{
	long    m_nTotalCnt;
	long    m_nIndexPnt;
	long    m_InspType[CAM_MAX_NUM][GOM_MAX_NUM];
	BOOL   *m_bDec[CAM_MAX_NUM][GOM_MAX_NUM];
	float  *m_fValue[CAM_MAX_NUM][GOM_MAX_NUM];
	float  *m_fValueS[CAM_MAX_NUM][GOM_MAX_NUM];
} PRODUCT_DATA;

typedef struct
{
	TCHAR   m_strModelName[100];
	TCHAR   m_strLotID[100];

	BOOL    m_bCam1;
	BOOL    m_bCam2;
	BOOL    m_bGood;
	BOOL    m_bBad;
	BOOL    m_bPeriod;
	BOOL    m_bModel;
	BOOL    m_bInspItem;

	long 	m_tmFromYY;
	long 	m_tmFromMM;
	long 	m_tmFromDD;
	long 	m_tmFromTT;
	long    m_tmFromMT;
	long 	m_tmToYY;
	long    m_tmToMM;
	long 	m_tmToDD;
	long 	m_tmToTT;
	long    m_tmToMT;
} SEARCH_DATA;

typedef struct
{
	long    m_nCamID;
	BOOL    m_bDecision;
	BOOL    m_bStatus;
	BOOL    m_bFDecision;
	BOOL    m_bRDecision;
	BOOL    m_bFDataDec;
	BOOL    m_bRDataDec;

	BOOL    m_bMisMatchDecF;
	BOOL    m_bMisMatchDecR;

	float   m_dSensorDataF;
	float   m_dSensorDataR;
	float   m_dOffsetEPC;

	TCHAR   m_strTitle[GOM_MAX_NUM][100];
	BOOL    m_bDecisionROI[GOM_MAX_NUM];
	BOOL    m_bLocROI[GOM_MAX_NUM];
	BOOL    m_bExecROI[GOM_MAX_NUM];
	BOOL    m_bEjectROI[GOM_MAX_NUM];
	long    m_nPlotNum[GOM_MAX_NUM];
	long    m_nOutNo[GOM_MAX_NUM];
    long    m_nInspType[GOM_MAX_NUM];

#if _NG_DISP
	float   m_dPosRoiX1[GOM_MAX_NUM];
	float   m_dPosRoiY1[GOM_MAX_NUM];
	float   m_dPosRoiX2[GOM_MAX_NUM];
	float   m_dPosRoiY2[GOM_MAX_NUM];

	long    m_nSampleNum[GOM_MAX_NUM];
	POINT2  m_ptFirstEdge[GOM_MAX_NUM][SAMPLE_MAX_NUM];
	POINT2  m_ptSecondEdge[GOM_MAX_NUM][SAMPLE_MAX_NUM];
#endif
	
	float   m_dValue[GOM_MAX_NUM];
	float   m_dLowLimit[GOM_MAX_NUM];
	float   m_dHighLimit[GOM_MAX_NUM];
	float   m_dStdSpec[GOM_MAX_NUM];
	float   m_dToleSpec[GOM_MAX_NUM];
	long    m_nLinkNum;
	TCHAR   m_strLinkTitle[LINK_MAX_NUM][100];
	BOOL    m_bLinkDecROI[LINK_MAX_NUM];
	BOOL    m_bLinkExecROI[LINK_MAX_NUM];
	BOOL    m_bLinkEjectROI[LINK_MAX_NUM];
	BOOL    m_bLinkLocROI[LINK_MAX_NUM];
	long    m_nLinkPlotPos[LINK_MAX_NUM];
	long    m_nLinkOutNo[LINK_MAX_NUM];
	long    m_nLinkPlotNum[LINK_MAX_NUM];
    long    m_nLinkType[LINK_MAX_NUM];
	float   m_dLinkValue[LINK_MAX_NUM];
	float   m_dLinkValueS[LINK_MAX_NUM];
	float   m_dLinkLowLimit[LINK_MAX_NUM];
	float   m_dLinkHighLimit[LINK_MAX_NUM];
	float   m_dLinkStdSpec[LINK_MAX_NUM];
	float   m_dLinkToleSpec[LINK_MAX_NUM];
} INSP_RESULT_DATA;

typedef struct 
{
	float   m_A1;
	float   m_A2;
	float   m_A3;
	float   m_A4;
	float   m_T2B;
	float   m_TE;
} OFFSET_DATA;

typedef struct
{
	long    m_nImgHeadPnt;
	long    m_nImgTailPnt;

	long    m_nWidth;
	long    m_nHeight;

	long    m_nImgIndex;
	long    m_ImgIndexP[DEF_QUEUE_MAX_NUM];
	long    m_ImgCam[DEF_QUEUE_MAX_NUM];
    long    m_nBadIndex[CAM_MAX_NUM];

	long    m_nMelHeadPnt;
	long    m_nMelTailPnt;

	INSP_RESULT_DATA m_ImgRes[DEF_QUEUE_MAX_NUM];
	INSP_RESULT_DATA m_BadRes[CAM_MAX_NUM][DEF_QUEUE_MAX_NUM];
	INSP_RESULT_DATA m_MelRes[CAM_MAX_NUM][DEF_QUEUE_MAX_NUM];

	long    m_InspCnt[GOM_MAX_NUM];
	float   m_BadCnt[GOM_MAX_NUM];
	float   m_YieldRate[GOM_MAX_NUM];
} REALTIME_DATA;

// INSPECT TYPE ����
typedef enum
{
	INSP_MEASURE_SIDE_ONE=400,          // �� ���� EDGE ����(������Ʈ ���)
	INSP_MEASURE_SIDE_TWO,              // �� ���� EDGE ����(������Ʈ ���)
	INSP_SLINE,                         // ������ �˻�
	INSP_STRAIGHTNESS,            
	INSP_DATUM_C,
	INSP_DATUM_B,
	INSP_DEFECT_VARI,                   // Defect �ٸ� �˻�
	INSP_MARK,
	INSP_PATTERN_MATCH,                 // ���� ��Ī 
	INSP_CAMERA_CAL,
	INSP_BLOB_ANALYSIS,                 // BLOB �ؼ� 
	INSP_MEASURE_CIRCLE,                // ���� �˻�
	INSP_BLOB_CIRCLE,
	INSP_DEFECT_NORMAL,                 // Defect �˻� Type  
	INSP_DEFECT_CIRCLE,
	INSP_GRAY_VALUE,                    // Gray Value�� �˻� Type 
	INSP_SHARPNESS_VALUE,               // Sharpness Value
	INSP_IS_OBJECT,
	INSP_SCALE_FACTOR
};
// INSPECT TYPE ����

// TEACHING TYPE ����
typedef enum
{
	TEACH_NONE = 500,                  // �˻� ���°� ����  
	TEACH_DIST_PNT2PNT,                // �� ���� �Ÿ� ���� TYPE
	TEACH_DIST_PNT2LINE,
	TEACH_DIST_LINE2LINE,              // �� ������ �Ÿ� ���� TYPE
	TEACH_DIST_1STLINE2LINE,
	TEACH_DIST_2STLINE2LINE,
	TEACH_NADIST_LINE2LINE,
	TEACH_DIFF_GAP2GAP,
	TEACH_DIST_CIRCLE2LINE,
	TEACH_ANG_PNT2PNT,                 // �� ���� ���� ���� Type  
	TEACH_ANG_LINE4PNT,                // 4������ ����� 2������ ���� ���� Type 
	TEACH_ANG_LINE2LINE,               // �� ������ ���� ���� TYPE
	TEACH_ANG_3PNT,                    // �� ���� ���� ���� TYPE
	TEACH_DIFF_PNT2PNT,                // �� ���� OFFSET ���� TYPE 
	TEACH_CIR_ROI2ROI
};
// TEACHING TYPE ���� 

typedef struct
{
	char   m_sLogTitle1[50];
	char   m_sLogTitle2[50];
	char   m_sLogTitle3[50];
	char   m_sLogTitle4[50];
	char   m_sLogTitle5[50];
	char   m_sLogTitle6[50];
	char   m_sLogTitle7[50];
	char   m_sLogTitle8[50];
	char   m_sLogTitle9[50];
	char   m_sLogTitle10[50];
	char   m_sLogTitle11[50];
	char   m_sLogTitle12[50];
	char   m_sLogTitle13[50];
	char   m_sLogTitle14[50];
	char   m_sLogTitle15[50];
	char   m_sLogTitle16[50];
} LOG_LIST;
// INSPECT TYPE ����

typedef struct
{
    long    m_nCamID;
	long    m_nPosX;
	long    m_nPosY;
	long    m_nGrayLevel;

	BOOL    m_bModify;
	BOOL    m_bContNo;
	long    m_nCh;

	long    m_nExposureTime;
	long    m_LightV[LIGHT_MAX_NUM];
} NAppDll_CallDATA;

typedef struct
{
    long    m_nPlotID;
	long    m_nPosX;
	long    m_nPosY;
	long    m_nGrayLevel;
} NPlotDll_CallDATA;

#endif __AppDefine_H__
