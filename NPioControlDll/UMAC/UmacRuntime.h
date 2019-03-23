

#ifndef RUNTIME_H
#define RUNTIME_H

#include <pmacu.h>

#define DRIVERNAME          TEXT("Pcomm32.dll") //name of the user-dll driver 


//************************************************************************
// COMM Type Defines
//************************************************************************
typedef BOOL   (CALLBACK* PMACCONFIGURE)(HANDLE hwnd,DWORD dwDevice);
typedef BOOL   (CALLBACK* OPENPMACDEVICE)(DWORD dwDevice);
typedef BOOL   (CALLBACK* CLOSEPMACDEVICE)(DWORD dwDevice);
typedef LPSTR  (CALLBACK* SZLOADSTRINGA)(HANDLE hInst,int iID);
typedef PCHAR  (CALLBACK* SZSTRIPCONTROLCHARA)(PCHAR str);
typedef PCHAR  (CALLBACK* SZSTRIPWHITESPACEA)(PCHAR str);
typedef void   (CALLBACK* CHECKFORPERCENTCHARANDMODIFY)(PCHAR orig,PCHAR mod,int size_mod);
typedef int    (CALLBACK* GETERROR)(DWORD dwDevice);
typedef ASCIIMODE (CALLBACK* GETASCIICOMM)(DWORD dwDevice);
typedef BOOL   (CALLBACK* SETASCIICOMM)(DWORD dwDevice,ASCIIMODE m);
typedef BOOL   (CALLBACK* MOTIONBUFOPEN)(DWORD dwDevice);
typedef BOOL   (CALLBACK* ROTBUFOPEN)(DWORD dwDevice);
typedef short int (CALLBACK* GETVARIABLE)(DWORD dwDevice,char ch,UINT num,short int def);
typedef short int (CALLBACK* GETIVARIABLE)(DWORD dwDevice,UINT num,short int def);
typedef long   (CALLBACK* GETIVARIABLELONG)(DWORD dwDevice,UINT num,long def);
typedef double (CALLBACK* GETVARIABLEDOUBLE)(DWORD dwDevice,char ch,UINT num,double def);
typedef double (CALLBACK* GETIVARIABLEDOUBLE)(DWORD dwDevice,UINT num,double def);
typedef void (CALLBACK* SETVARIABLE)(DWORD dwDevice, char ch, UINT num, short int val);
typedef void (CALLBACK* SETVARIABLEDOUBLE)(DWORD dwDevice, char ch, UINT num, double val);
typedef void   (CALLBACK* SETIVARIABLE)(DWORD dwDevice,UINT num,short int val);
typedef void   (CALLBACK* SETIVARIABLELONG)(DWORD dwDevice,UINT num,long val);
typedef void   (CALLBACK* SETIVARIABLEDOUBLE)(DWORD dwDevice,UINT num,double val);
typedef int    (CALLBACK* GETProgramInfo)(DWORD dwDevice,BOOL plc,int num,UINT *sadr,UINT *fadr);
typedef PUSER_HANDLE (CALLBACK* GETUSERHANDLE)(DWORD dwDevice);
typedef BOOL   (CALLBACK* CONFIGURE)(HWND hwnd,DWORD dwDevice);
typedef BOOL   (CALLBACK* INBOOTSTRAP)(DWORD dwDevice);

// ASCII string exported functions
typedef PCHAR (CALLBACK* GETROMDATEA)(DWORD dwDevice,LPSTR s,int maxchar);
typedef PCHAR (CALLBACK* GETROMVERSIONA)(DWORD dwDevice,LPSTR s,int maxchar);
typedef int   (CALLBACK* GETPMACTYPE)(DWORD dwDevice);
typedef BOOL  (CALLBACK* GETIVARIABLESTRA)(DWORD dwDevice,LPSTR str,UINT num);
typedef int   (CALLBACK* MULTIDOWNLOADA)(DWORD dwDevice,DOWNLOADMSGPROC msgp,LPCSTR outfile,
                LPCSTR inifile,LPCSTR szUserId,BOOL macro,BOOL map,BOOL log,BOOL dnld);
typedef int   (CALLBACK* ADDDOWNLOADFILEA)(DWORD dwDevice,LPCSTR inifile,LPCSTR szUserId,LPCSTR szDLFile);
typedef int   (CALLBACK* REMOVEDOWNLOADFILEA)(DWORD dwDevice,LPCSTR inifile,LPCSTR szUserId,LPCSTR szDLFile);
typedef void  (CALLBACK* RENUMBERFILESA)(DWORD dwDevice,int file_num,LPCSTR szIniFile);
typedef int   (CALLBACK* GETERRORSTRA)(DWORD dwDevice,LPSTR str,int maxchar);

// Unicode string exported functions
typedef BOOL (CALLBACK* GETIVARIABLESTRW)(DWORD dwDevice,LPWSTR str,UINT num);
typedef WORD (CALLBACK* GETPLCSTATUS)(DWORD dwDevice,struct total_plc_status_struct *plc_stat);
typedef int  (CALLBACK* MULTIDOWNLOADW)(DWORD dwDevice,DOWNLOADMSGPROC msgp,PWCHAR outfile,
                PWCHAR inifile,PWCHAR szUserId,BOOL macro,BOOL map,BOOL log,BOOL dnld);
typedef int  (CALLBACK* ADDDOWNLOADFILEW)(DWORD dwDevice,PWCHAR inifile,PWCHAR szUserId,PWCHAR szDLFile);
typedef int  (CALLBACK* REMOVEDOWNLOADFILEW)(DWORD dwDevice,PWCHAR inifile,PWCHAR szUserId,PWCHAR szDLFile);
typedef void (CALLBACK* RENUMBERFILESW)(DWORD dwDevice,int file_num,PWCHAR szIniFile);
typedef int  (CALLBACK* GETERRORSTRW)(DWORD dwDevice,PWCHAR str,int maxchar);
 

typedef BOOL  (CALLBACK* READREADY)(DWORD dwDevice);
typedef int   (CALLBACK* SENDCHARA)(DWORD dwDevice,CHAR outch);
typedef int   (CALLBACK* SENDCHARW)(DWORD dwDevice,WCHAR outch);
typedef int   (CALLBACK* SENDLINEA)(DWORD dwDevice,LPCTSTR outstr);
typedef int   (CALLBACK* SENDLINEW)(DWORD dwDevice,PWCHAR outstr);
typedef int 	(CALLBACK* GETLINEA)(DWORD dwDevice,LPTSTR s,UINT maxchar);
typedef int   (CALLBACK* GETLINEW)(DWORD dwDevice,PWCHAR s,UINT maxchar);
typedef int 	(CALLBACK* GETBUFFERA)(DWORD dwDevice,LPTSTR s,UINT maxchar);
typedef int 	(CALLBACK* GETBUFFERW)(DWORD dwDevice,PWCHAR s,UINT maxchar);
typedef int 	(CALLBACK* GETRESPONSEA)(DWORD dwDevice,LPTSTR s,UINT maxchar,LPCTSTR outstr);
typedef int 	(CALLBACK* GETRESPONSEW)(DWORD dwDevice,PWCHAR s,UINT maxchar,PWCHAR outstr);
typedef int   (CALLBACK* GETCONTROLRESPONSEA)(DWORD dwDevice,LPTSTR s,UINT maxchar,CHAR outchar);
typedef int   (CALLBACK* GETCONTROLRESPONSEW)(DWORD dwDevice,PWCHAR s,UINT maxchar,WCHAR outchar);
typedef void 	(CALLBACK* FLUSH)(DWORD dwDevice);
typedef void  (CALLBACK* SENDCOMMANDA)(DWORD dwDevice,LPCTSTR outchar);
typedef void  (CALLBACK* SENDCOMMANDW)(DWORD dwDevice,PWCHAR outstr);
typedef int   (CALLBACK* SENDCTRLCHARA)(DWORD dwDevice,CHAR outstr);
typedef int   (CALLBACK* SENDCTRLCHARW)(DWORD dwDevice,WCHAR outstr);

typedef int   (CALLBACK* DOWNLOADA)(DWORD dwDevice,DOWNLOADMSGPROC msgp,DOWNLOADGETPROC getp,
                            DOWNLOADPROGRESS ppgr,PCHAR filename,BOOL macro,BOOL map,BOOL log,BOOL dnld);
typedef int   (CALLBACK* DOWNLOADW)(DWORD dwDevice,DOWNLOADMSGPROC msgp,DOWNLOADGETPROC getp,
                            DOWNLOADPROGRESS ppgr,PWCHAR fname,BOOL macro,BOOL map,BOOL log,BOOL dnld);
typedef void  (CALLBACK* DOWNLOADFILE)(DWORD dwDevice,char *fname);
typedef BOOL  (CALLBACK* COMPILEPLCC)(DWORD dwDevice,char *plccName,char *outName);
typedef BOOL  (CALLBACK* WRITEDICTIONARY)(const char *tblName,PMACRO *root);
typedef BOOL  (CALLBACK* READDICTIONARY)(const char *tblName,PMACRO *root);
typedef int   (CALLBACK* DOWNLOADFIRMWAREFILE)(DWORD dwDevice,DOWNLOADMSGPROC msgp,
                                      DOWNLOADPROGRESS prgp,PCHAR filename);
typedef void  (CALLBACK* ABORTDOWNLOAD)(DWORD dwDevice);
typedef void  (CALLBACK* SETMAXDOWNLOADERRORS)(UINT max);

typedef void   (CALLBACK* DPRSTATUS)(DWORD dwDevice,UINT *comm,UINT *bg,UINT * bgv,
                UINT *rt,UINT *cp, UINT *rot);

// Numeric read/write functions
typedef LONG	 (CALLBACK* TESTDPRAM)(DWORD dwDevice, DPRTESTMSGPROC msgp,DPRTESTPROGRESS prgp);
typedef void	 (CALLBACK* TESTDPRABORT)(void);
typedef BOOL 	 (CALLBACK* DPRDWORDBITSET)(DWORD dwDevice,UINT offset,UINT bit);
typedef void 	 (CALLBACK* DPRSETDWORDBIT)(DWORD dwDevice,UINT offset,UINT bit);
typedef void   (CALLBACK* DPRRESETDWORDBIT)(DWORD dwDevice,UINT offset,UINT bit);
typedef WORD 	 (CALLBACK* DPRGETWORD)(DWORD dwDevice,UINT offset);
typedef void	 (CALLBACK* DPRSETWORD)(DWORD dwDevice,UINT offset, WORD val);
typedef DWORD  (CALLBACK* DPRGETDWORD)(DWORD dwDevice,UINT offset);
typedef void	 (CALLBACK* DPRSETDWORD)(DWORD dwDevice,UINT offset, DWORD val);
typedef float	 (CALLBACK* DPRGETFLOAT)(DWORD dwDevice,UINT offset);
typedef void	 (CALLBACK* DPRSETFLOAT)(DWORD dwDevice,UINT offset,double val);
typedef void 	 (CALLBACK* DPRSETDWORDMASK)(DWORD dwDevice,UINT offset,DWORD val,BOOL onoff);
typedef DWORD  (CALLBACK* DPRGETDWORDMASK)(DWORD dwDevice,UINT offset,DWORD val);

typedef double (CALLBACK* DPRFLOAT)(long d[],double scale);
typedef double (CALLBACK* DPRLFIXED)(long d[],double scale);


typedef double (CALLBACK* DPRCOMMANDED)(DWORD dwDevice,int coord,char axchar);
typedef double (CALLBACK* DPRVELOCITY)(DWORD dwDevice,int mtr,double units);
typedef double (CALLBACK* DPRVECTORVELOCITY)(DWORD dwDevice,int num,int mtr[],double units[]);
typedef BOOL   (CALLBACK* DPRSETBACKGROUND)(DWORD dwDevice);
typedef BOOL   (CALLBACK* DPRBACKGROUND)(DWORD dwDevice,int on);

// Functions pertaining to coord systems
typedef long   (CALLBACK* DPRPE)(DWORD dwDevice,int cs);
typedef BOOL   (CALLBACK* DPRROTBUFFULL)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSINPOSITION)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSWARNFERROR)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSFATALFERROR)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSRUNTIMEERROR)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSCIRCLERADERROR)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSAMPFAULTERROR)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRPROGRUNNING)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRPROGSTEPPING)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRPROGCONTMOTION)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRPROGCONTREQUEST)(DWORD dwDevice,int crd);
typedef int	   (CALLBACK* DPRPROGREMAINING)(DWORD dwDevice,int crd);

// Functions pertaining to individual motors
// Background-Functions pertaining to individual motors
typedef BOOL   (CALLBACK* DPRAMPENABLED)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRWARNFERROR)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRFATALFERROR)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRAMPFAULT)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRONPOSITIONLIMIT)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRHOMECOMPLETE)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRINPOSITION)(DWORD dwDevice,int mtr);
typedef double (CALLBACK* DPRGETTARGETPOS)(DWORD dwDevice,int motor, double posscale);
typedef double (CALLBACK* DPRGETBIASPOS)(DWORD dwDevice,int motor, double posscale);
typedef long   (CALLBACK* DPRTIMEREMINMOVE)(DWORD dwDevice,int cs);
typedef long   (CALLBACK* DPRTIMEREMINTATS)(DWORD dwDevice,int cs);


// Logical query functions
typedef PROGRAM    (CALLBACK* DPRGETPROGRAMMODE)(DWORD dwDevice,int csn);
typedef MOTIONMODE (CALLBACK* DPRGETMOTIONMODE)(DWORD dwDevice,int csn);

////////////////////////////////////////////////////////////////////////////
// DPR Control Panel functions
////////////////////////////////////////////////////////////////////////////
typedef BOOL  (CALLBACK* DPRCONTROLPANEL)(DWORD dwDevice,long on);
typedef void  (CALLBACK* DPRSETJOGPOSBIT)(DWORD dwDevice,long motor,long onoff);
typedef long  (CALLBACK* DPRGETJOGPOSBIT)(DWORD dwDevice,long motor);
typedef void  (CALLBACK* DPRSETJOGNEGBIT)(DWORD dwDevice,long motor,long onoff);
typedef long  (CALLBACK* DPRGETJOGNEGBIT)(DWORD dwDevice,long motor);
typedef void  (CALLBACK* DPRSETJOGRETURNBIT)(DWORD dwDevice,long motor,long onoff);
typedef long  (CALLBACK* DPRGETJOGRETURNBIT)(DWORD dwDevice,long motor);
typedef void  (CALLBACK* DPRSETRUNBIT)(DWORD dwDevice,long cs,long onoff);
typedef long  (CALLBACK* DPRGETRUNBIT)(DWORD dwDevice,long cs);
typedef void  (CALLBACK* DPRSETSTOPBIT)(DWORD dwDevice,long cs,long onoff);
typedef long  (CALLBACK* DPRGETSTOPBIT)(DWORD dwDevice,long cs);
typedef void  (CALLBACK* DPRSETHOMEBIT)(DWORD dwDevice,long cs,long onoff);
typedef long  (CALLBACK* DPRGETHOMEBIT)(DWORD dwDevice,long cs);
typedef void  (CALLBACK* DPRSETHOLDBIT)(DWORD dwDevice,long cs,long onoff);
typedef long  (CALLBACK* DPRGETHOLDBIT)(DWORD dwDevice,long cs);
typedef long  (CALLBACK* DPRGETSTEPBIT)(DWORD dwDevice,long cs);
typedef void  (CALLBACK* DPRSETSTEPBIT)(DWORD dwDevice,long cs,long onoff);
typedef long  (CALLBACK* DPRGETREQUESTBIT)(DWORD dwDevice,long mtrcrd);
typedef void  (CALLBACK* DPRSETREQUESTBIT)(DWORD dwDevice,long mtrcrd,long onoff);
typedef long  (CALLBACK* DPRGETFOENABLEBIT)(DWORD dwDevice,long cs);
typedef void  (CALLBACK* DPRSETFOENABLEBIT)(DWORD dwDevice,long cs, long on_off);
typedef void  (CALLBACK* DPRSETFOVALUE)(DWORD dwDevice,long cs, long value);
typedef long  (CALLBACK* DPRGETFOVALUE)(DWORD dwDevice,long cs);


////////////////////////////////////////////////////////////////////////////
// DPR Real Time Data Buffer functions
////////////////////////////////////////////////////////////////////////////
typedef BOOL (CALLBACK* DPRREALTIME)(DWORD dwDevice,UINT period,int on);
typedef void (CALLBACK* DPRSETHOSTBUSYBIT)(DWORD dwDevice,int value);
typedef int  (CALLBACK* DPRGETHOSTBUSYBIT)(DWORD dwDevice);
typedef int  (CALLBACK* DPRGETPMACBUSYBIT)(DWORD dwDevice);
typedef int  (CALLBACK* DPRGETSERVOTIMER)(DWORD dwDevice);
typedef void (CALLBACK* DPRSETMOTORS)(DWORD dwDevice,UINT n);

typedef double (CALLBACK* DPRGETCOMMANDEDPOS)(DWORD dwDevice,int mtr, double units);
typedef double (CALLBACK* DPRPOSITION)(DWORD dwDevice,int i,double units);
typedef double (CALLBACK* DPRFOLLOWERROR)(DWORD dwDevice,int mtr,double units);
typedef double (CALLBACK* DPRGETVEL)(DWORD dwDevice,int mtr,double units);
typedef void   (CALLBACK* DPRGETMASTERPOS)(DWORD dwDevice,int mtr,double units,double *the_double);
typedef void   (CALLBACK* DPRGETCOMPENSATIONPOS)(DWORD dwDevice,int mtr,double units,double *the_double);

typedef DWORD  (CALLBACK* DPRGETPREVDAC)(DWORD dwDevice,int mtr);
typedef DWORD  (CALLBACK* DPRGETMOVETIME)(DWORD dwDevice,int mtr);


//Gather Time Buffer-Functions pertaining to individual motors
typedef struct ss (CALLBACK* DPRMOTORSERVOSTATUS)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRDATABLOCK)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRPHASEDMOTOR)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRMOTORENABLED)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRHANDWHEELENABLED)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPROPENLOOP)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRONNEGATIVELIMIT)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRONPOSITIVELIMIT)(DWORD dwDevice,int mtr);
typedef void (CALLBACK* DPRSETJOGRETURN)(DWORD dwDevice,int mtr);

// Logical query functions
typedef MOTION (CALLBACK* DPRGETMOTORMOTION)(DWORD dwDevice,int mtr);

// Functions pertaining	to coord systems
typedef BOOL (CALLBACK* DPRMOTIONBUFOPEN)(DWORD dwDevice);
typedef BOOL (CALLBACK* DPRROTBUFOPEN)(DWORD dwDevice);
typedef double (CALLBACK* DPRGETFEEDRATEMODE)(DWORD dwDevice,int csn, BOOL	*mode);

// Function	pertaining to global
typedef BOOL (CALLBACK* DPRSYSSERVOERROR)(DWORD dwDevice);
typedef BOOL (CALLBACK* DPRSYSREENTRYERROR)(DWORD dwDevice);
typedef BOOL (CALLBACK* DPRSYSMEMCHECKSUMERROR)(DWORD dwDevice);
typedef BOOL (CALLBACK* DPRSYSPROMCHECKSUMERROR)(DWORD dwDevice);
typedef struct gs (CALLBACK* DPRGETGLOBALSTATUS)(DWORD dwDevice);

typedef BOOL  (CALLBACK* INTRINIT)(DWORD dwDevice,DWORD dwCallback,DWORD dwFlags,
                            DWORD dwUser,ULONG mask);
typedef BOOL  (CALLBACK* INTRTERMINATE)(DWORD dwDevice);
typedef BOOL  (CALLBACK* INTRWNDMSGINIT)(DWORD dwDevice,HWND hWnd,UINT msg,ULONG ulMask);
typedef DWORD (CALLBACK* INTRTHREADINIT)(DWORD *dwDev);
typedef VOID  (CALLBACK* INTRCALLBACK)(DWORD dwDevice);
typedef VOID  (CALLBACK* INTRQUEUE)(DWORD dwDevice,DWORD dwEvent);
typedef VOID  (CALLBACK* INTRCOMPLETE)(DWORD dwDevice,DWORD dwEvent);
typedef BOOL  (CALLBACK* INTRPROCESSFUNCTION)(DWORD dwDevice,PMACINTRPROC pFunc,DWORD Param,
                  ULONG ulMask);

/*****************************************************************************/



typedef void   (CALLBACK* DPRSTATUS)(DWORD dwDevice,UINT *comm,UINT *bg,UINT * bgv,
                UINT *rt,UINT *cp, UINT *rot);


typedef int    (CALLBACK* GETPROGRAMINFO)(DWORD dwDevice,BOOL plc,int num,UINT *sadr,UINT *fadr);

// Numeric read/write functions
typedef BOOL   (CALLBACK* DPRDWORDBITSET)(DWORD dwDevice,UINT offset,UINT bit);
typedef void   (CALLBACK* DPRSETDWORDBIT)(DWORD dwDevice,UINT offset,UINT bit);
typedef void   (CALLBACK* DPRRESETDWORDBIT)(DWORD dwDevice,UINT offset,UINT bit);
typedef WORD   (CALLBACK* DPRGETWORD)(DWORD dwDevice,UINT offset);
typedef void   (CALLBACK* DPRSETWORD)(DWORD dwDevice,UINT offset, WORD val);
typedef DWORD  (CALLBACK* DPRGETDWORD)(DWORD dwDevice,UINT offset);
typedef void   (CALLBACK* DPRSETDWORD)(DWORD dwDevice,UINT offset, DWORD val);
typedef float  (CALLBACK* DPRGETFLOAT)(DWORD dwDevice,UINT offset);
typedef void   (CALLBACK* DPRSETFLOAT)(DWORD dwDevice,UINT offset,double val);
typedef void   (CALLBACK* DPRSETDWORDMASK)(DWORD dwDevice,UINT offset,DWORD val,BOOL onoff);
typedef DWORD  (CALLBACK* DPRGETDWORDMASK)(DWORD dwDevice,UINT offset,DWORD val);

typedef double (CALLBACK* DPRFLOAT)(long d[],double scale);
typedef double (CALLBACK* DPRLFIXED)(long d[],double scale);


typedef double (CALLBACK* DPRCOMMANDED)(DWORD dwDevice,int coord,char axchar);
typedef double (CALLBACK* DPRVELOCITY)(DWORD dwDevice,int mtr,double units);
typedef double (CALLBACK* DPRVECTORVELOCITY)(DWORD dwDevice,int num,int mtr[],double units[]);
typedef BOOL   (CALLBACK* DPRSETBACKGROUND)(DWORD dwDevice);
typedef BOOL   (CALLBACK* DPRBACKGROUND)(DWORD dwDevice,int on);

// Functions pertaining to coord systems
typedef long   (CALLBACK* DPRPE)(DWORD dwDevice,int cs);
typedef BOOL   (CALLBACK* DPRROTBUFFULL)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSINPOSITION)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSWARNFERROR)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSFATALFERROR)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSRUNTIMEERROR)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSCIRCLERADERROR)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRSYSAMPFAULTERROR)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRPROGRUNNING)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRPROGSTEPPING)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRPROGCONTMOTION)(DWORD dwDevice,int crd);
typedef BOOL   (CALLBACK* DPRPROGCONTREQUEST)(DWORD dwDevice,int crd);
typedef int	   (CALLBACK* DPRPROGREMAINING)(DWORD dwDevice,int crd);

// Functions pertaining to individual motors
// Background-Functions pertaining to individual motors
typedef BOOL   (CALLBACK* DPRAMPENABLED)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRWARNFERROR)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRFATALFERROR)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRAMPFAULT)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRONPOSITIONLIMIT)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRHOMECOMPLETE)(DWORD dwDevice,int mtr);
typedef BOOL   (CALLBACK* DPRINPOSITION)(DWORD dwDevice,int mtr);
typedef double (CALLBACK* DPRGETTARGETPOS)(DWORD dwDevice,int motor, double posscale);
typedef double (CALLBACK* DPRGETBIASPOS)(DWORD dwDevice,int motor, double posscale);
typedef long   (CALLBACK* DPRTIMEREMINMOVE)(DWORD dwDevice,int cs);
typedef long   (CALLBACK* DPRTIMEREMINTATS)(DWORD dwDevice,int cs);


// Logical query functions
typedef PROGRAM    (CALLBACK* DPRGETPROGRAMMODE)(DWORD dwDevice,int csn);
typedef MOTIONMODE (CALLBACK* DPRGETMOTIONMODE)(DWORD dwDevice,int csn);

////////////////////////////////////////////////////////////////////////////
// DPR Control Panel functions
////////////////////////////////////////////////////////////////////////////
typedef BOOL  (CALLBACK* DPRCONTROLPANEL)(DWORD dwDevice,long on);
typedef void  (CALLBACK* DPRSETJOGPOSBIT)(DWORD dwDevice,long motor,long onoff);
typedef long  (CALLBACK* DPRGETJOGPOSBIT)(DWORD dwDevice,long motor);
typedef void  (CALLBACK* DPRSETJOGNEGBIT)(DWORD dwDevice,long motor,long onoff);
typedef long  (CALLBACK* DPRGETJOGNEGBIT)(DWORD dwDevice,long motor);
typedef void  (CALLBACK* DPRSETJOGRETURNBIT)(DWORD dwDevice,long motor,long onoff);
typedef long  (CALLBACK* DPRGETJOGRETURNBIT)(DWORD dwDevice,long motor);
typedef void  (CALLBACK* DPRSETRUNBIT)(DWORD dwDevice,long cs,long onoff);
typedef long  (CALLBACK* DPRGETRUNBIT)(DWORD dwDevice,long cs);
typedef void  (CALLBACK* DPRSETSTOPBIT)(DWORD dwDevice,long cs,long onoff);
typedef long  (CALLBACK* DPRGETSTOPBIT)(DWORD dwDevice,long cs);
typedef void  (CALLBACK* DPRSETHOMEBIT)(DWORD dwDevice,long cs,long onoff);
typedef long  (CALLBACK* DPRGETHOMEBIT)(DWORD dwDevice,long cs);
typedef void  (CALLBACK* DPRSETHOLDBIT)(DWORD dwDevice,long cs,long onoff);
typedef long  (CALLBACK* DPRGETHOLDBIT)(DWORD dwDevice,long cs);
typedef long  (CALLBACK* DPRGETSTEPBIT)(DWORD dwDevice,long cs);
typedef void  (CALLBACK* DPRSETSTEPBIT)(DWORD dwDevice,long cs,long onoff);
typedef long  (CALLBACK* DPRGETREQUESTBIT)(DWORD dwDevice,long mtrcrd);
typedef void  (CALLBACK* DPRSETREQUESTBIT)(DWORD dwDevice,long mtrcrd,long onoff);
typedef long  (CALLBACK* DPRGETFOENABLEBIT)(DWORD dwDevice,long cs);
typedef void  (CALLBACK* DPRSETFOENABLEBIT)(DWORD dwDevice,long cs, long on_off);
typedef void  (CALLBACK* DPRSETFOVALUE)(DWORD dwDevice,long cs, long value);
typedef long  (CALLBACK* DPRGETFOVALUE)(DWORD dwDevice,long cs);


////////////////////////////////////////////////////////////////////////////
// DPR Real Time Data Buffer functions
////////////////////////////////////////////////////////////////////////////
typedef BOOL (CALLBACK* DPRREALTIME)(DWORD dwDevice,UINT period,int on);
typedef void (CALLBACK* DPRSETHOSTBUSYBIT)(DWORD dwDevice,int value);
typedef int  (CALLBACK* DPRGETHOSTBUSYBIT)(DWORD dwDevice);
typedef int  (CALLBACK* DPRGETPMACBUSYBIT)(DWORD dwDevice);
typedef int  (CALLBACK* DPRGETSERVOTIMER)(DWORD dwDevice);
typedef void (CALLBACK* DPRSETMOTORS)(DWORD dwDevice,UINT n);

typedef double (CALLBACK* DPRGETCOMMANDEDPOS)(DWORD dwDevice,int mtr, double units);
typedef double (CALLBACK* DPRPOSITION)(DWORD dwDevice,int i,double units);
typedef double (CALLBACK* DPRFOLLOWERROR)(DWORD dwDevice,int mtr,double units);
typedef double (CALLBACK* DPRGETVEL)(DWORD dwDevice,int mtr,double units);
typedef void   (CALLBACK* DPRGETMASTERPOS)(DWORD dwDevice,int mtr,double units,double *the_double);
typedef void   (CALLBACK* DPRGETCOMPENSATIONPOS)(DWORD dwDevice,int mtr,double units,double *the_double);

typedef DWORD  (CALLBACK* DPRGETPREVDAC)(DWORD dwDevice,int mtr);
typedef DWORD  (CALLBACK* DPRGETMOVETIME)(DWORD dwDevice,int mtr);

//Gather Time Buffer-Functions pertaining to individual motors
typedef struct ss (CALLBACK* DPRMOTORSERVOSTATUS)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRDATABLOCK)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRPHASEDMOTOR)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRMOTORENABLED)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRHANDWHEELENABLED)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPROPENLOOP)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRONNEGATIVELIMIT)(DWORD dwDevice,int mtr);
typedef BOOL (CALLBACK* DPRONPOSITIVELIMIT)(DWORD dwDevice,int mtr);
typedef void (CALLBACK* DPRSETJOGRETURN)(DWORD dwDevice,int mtr);

    // Logical query functions
typedef MOTION (CALLBACK* DPRGETMOTORMOTION)(DWORD dwDevice,int mtr);

// Functions pertaining	to coord systems
typedef BOOL (CALLBACK* DPRMOTIONBUFOPEN)(DWORD dwDevice);
typedef BOOL (CALLBACK* DPRROTBUFOPEN)(DWORD dwDevice);
typedef double (CALLBACK* DPRGETFEEDRATEMODE)(DWORD dwDevice,int csn, BOOL	*mode);

// Function	pertaining to global
typedef BOOL (CALLBACK* DPRSYSSERVOERROR)(DWORD dwDevice);
typedef BOOL (CALLBACK* DPRSYSREENTRYERROR)(DWORD dwDevice);
typedef BOOL (CALLBACK* DPRSYSMEMCHECKSUMERROR)(DWORD dwDevice);
typedef BOOL (CALLBACK* DPRSYSPROMCHECKSUMERROR)(DWORD dwDevice);
typedef struct gs (CALLBACK* DPRGETGLOBALSTATUS)(DWORD dwDevice);

//**************************************
// DPR Binary rotary buffer functions
// Initialization/shutdown
typedef SHORT (CALLBACK* DPRROTBUFINIT)(DWORD dwDevice,USHORT bufnum);
typedef SHORT (CALLBACK* DPRROTBUFREMOVE)(DWORD dwDevice,USHORT bufnum);
typedef SHORT (CALLBACK* DPRROTBUFCHANGE)(DWORD dwDevice,USHORT bufnum,USHORT new_size) ;
typedef void  (CALLBACK* DPRROTBUFCLR)(DWORD dwDevice,USHORT bufnum);
typedef SHORT (CALLBACK* DPRROTBUF)(DWORD dwDevice,BOOL on);
typedef int   (CALLBACK* DPRBUFLAST)(DWORD dwDevice);

// Transfer functions
typedef SHORT (CALLBACK* DPRASCIISTRTOROTA)(DWORD dwDevice,PCHAR inpstr, USHORT bufnum);
typedef SHORT (CALLBACK* DPRSENDBINARYCOMMANDA)(DWORD dwDevice,PCHAR inpstr, USHORT bufnum);
typedef SHORT (CALLBACK* DPRASCIISTRTOBINARYFILEA)(PCHAR inpstr, FILE *outfile);

typedef SHORT (CALLBACK* DPRASCIISTRTOROTW)(DWORD dwDevice,PWCHAR inpstr, USHORT bufnum);
typedef SHORT (CALLBACK* DPRSSNDBINARYCOMMANDW)(DWORD dwDevice,PWCHAR inpstr, USHORT bufnum);
typedef SHORT (CALLBACK* DPRASCIISTRTOBINARYFILEW)(PWCHAR inpstr, FILE *outfile);


typedef SHORT (CALLBACK* DPRASCIIFILETOROT)(DWORD dwDevice,FILE *inpfile, USHORT bufnum);
typedef SHORT (CALLBACK* DPRBINARYFILETOROT)(DWORD dwDevice,FILE *inpfile, USHORT bufnum);
typedef SHORT (CALLBACK* DPRASCIITOBINARYFILE)(FILE *inpfile,FILE *outfile);
typedef SHORT (CALLBACK* DPRBINARYTOROT)(DWORD dwDevice,WORD *inpbinptr,WORD **outbinptr,WORD bufnum);

// Variable Background Buffer Functions
    // Initialization
typedef long  (CALLBACK* DPRVARBUFINIT)(DWORD dwDevice,long new_num_entries,PLONG addrarray);
typedef long  (CALLBACK* DPRVARBUFREMOVE)(DWORD dwDevice,long h);
typedef long  (CALLBACK* DPRVARBUFCHANGE)(DWORD dwDevice,long handle, long new_num_entries,
                  long *addrarray);
typedef long  (CALLBACK* DPRVARBUFREAD)(DWORD dwDevice,long h,long entry_num,PLONG long_2);
typedef long  (CALLBACK* DPRGETVBGADDRESS)(DWORD dwDevice,long h,long entry_num);

typedef long  (CALLBACK* DPRGETVBGNUMENTRIES)(DWORD dwDevice,long h);
typedef long  (CALLBACK* DPRGETVBGDATAOFFSET)(DWORD dwDevice,long h);
typedef long  (CALLBACK* DPRGETVBGADDROFFSET)(DWORD dwDevice,long h);

// Both
typedef UINT  (CALLBACK* DPRGETVBGSERVOTIMER)(DWORD dwDevice);
typedef UINT  (CALLBACK* DPRGETVBGSTARTADDR)(DWORD dwDevice);
typedef int   (CALLBACK* DPRGETVBGTOTALENTRIES)(DWORD dwDevice);

typedef int   (CALLBACK* DPRWRITEBUFFER)(DWORD dwDevice,int num_entries,
                struct VBGWFormat *the_data);


///////////////////////////////////////////////////////////////////////////
// Functions




#ifdef __cplusplus
extern "C" {
#endif

//extern HINSTANCE hPmacLib;

HINSTANCE RuntimeLink();

// Comm functions
#ifdef _UNICODE
  extern SENDLINEW DeviceSendLine;
  extern SENDCTRLCHARW DeviceSendCtrlChar;
  extern SENDCOMMANDW DeviceSendCommand;
  extern GETLINEW DeviceGetLine;
  extern SENDCHARW DeviceSendChar;
  extern GETBUFFERW DeviceGetBuffer;
  extern GETRESPONSEW DeviceGetResponse;
  extern DOWNLOADW DeviceDownload;
//  extern GETROMDATEW DeviceGetRomDate;
//  extern GETROMVERSIONW DeviceGetRomVersion;
  extern TESTDPRAM DeviceTestDPRAM;
#else
  extern SZLOADSTRINGA DeviceLoadString;
  extern SENDLINEA DeviceSendLine;
  extern SENDCTRLCHARA DeviceSendCtrlChar;
  extern SENDCOMMANDA DeviceSendCommand;
  extern GETLINEA DeviceGetLine;
  extern SENDCHARA DeviceSendChar;
  extern GETBUFFERA DeviceGetBuffer;
  extern GETRESPONSEA DeviceGetResponse;
  extern DOWNLOADA DeviceDownload;
  extern GETROMDATEA DeviceGetRomDate;
  extern GETROMVERSIONA DeviceGetRomVersion;
  extern TESTDPRAM DeviceTestDPRAM;
  extern TESTDPRABORT DeviceTestDPRAMAbort;
  extern DPRASCIISTRTOROTA	DeviceDPRAsciiStrToRot;
#endif

extern INBOOTSTRAP DeviceInBootStrapMode;
extern PMACCONFIGURE DevicePmacConfigure;
extern READREADY DeviceReadReady;
extern OPENPMACDEVICE DeviceOpen;
extern CLOSEPMACDEVICE DeviceClose;
extern GETASCIICOMM DeviceGetAsciiComm;
extern SETASCIICOMM DeviceSetAsciiComm;
extern FLUSH DeviceFlush;
extern GETERROR DeviceGetError;
extern GETERRORSTRA DeviceGetErrorStr;
extern GETPMACTYPE DeviceGetType;
extern INTRTERMINATE DeviceINTRTerminate;
extern INTRWNDMSGINIT  DeviceINTRWndMsgInit;
extern INTRPROCESSFUNCTION DeviceINTRFuncInit;
extern GETVARIABLE DeviceGetVariable;
extern GETIVARIABLELONG DeviceGetIVariableLong;
extern GETVARIABLEDOUBLE DeviceGetVariableDouble;
extern SETIVARIABLELONG DeviceSetIVariableLong;
extern SETVARIABLE DeviceSetVariable;
extern SETVARIABLEDOUBLE DeviceSetVariableDouble;
extern GETUSERHANDLE DeviceGetUserHandle;
extern CONFIGURE DeviceConfigure;;
extern DPRVARBUFINIT DeviceDPRVarBufInit;
extern DPRVARBUFREMOVE DeviceDPRVarBufRemove;
extern DPRVELOCITY DeviceDPRVelocity;
extern DPRVARBUFREAD DeviceDPRVarBufRead;
extern DOWNLOADFIRMWAREFILE DeviceLoadFirmwareFile;
extern ABORTDOWNLOAD DeviceAbortDownload;
extern SETMAXDOWNLOADERRORS DeviceSetMaxDownloadErrors;
extern DPRGETMOTORMOTION DeviceDPRGetMotorMotion;
extern DPRGETPROGRAMMODE DeviceDPRGetProgramMode;
extern DPRSYSINPOSITION DeviceDPRSysInposition;
extern DPRROTBUFOPEN DeviceDPRRotBufOpen;
extern DPRPROGREMAINING DeviceDPRProgRemaining;
extern DPRGETFEEDRATEMODE DeviceDPRGetFeedRateMode;

// Binary Rotary Buffer 
extern DPRROTBUFINIT	DeviceDPRRotBufInit;
extern DPRROTBUF	DeviceDPRRotBuf;
extern DPRBUFLAST	DeviceDPRBufLast;
extern DPRROTBUFREMOVE DeviceDPRRotBufRemove;
extern DPRROTBUFCHANGE DeviceDPRRotBufChange;

// Real Time Data Buffer
extern DPRREALTIME DeviceDPRRealTime;
extern DPRSETHOSTBUSYBIT DeviceDPRSetHostBusyBit;
extern DPRGETHOSTBUSYBIT DeviceDPRGetHostBusyBit;
extern DPRGETPMACBUSYBIT DeviceDPRGetPMACBusyBit;
extern DPRGETSERVOTIMER DeviceDPRGetServoTimer;
extern DPRSETMOTORS DeviceDPRSetMotors;
extern DPRGETCOMMANDEDPOS DeviceDPRGetCommandedPos;
extern DPRPOSITION DeviceDPRPosition;
extern DPRFOLLOWERROR DeviceDPRFollowError;
extern DPRGETVEL DeviceDPRGetVel;
extern DPRGETMASTERPOS DeviceDPRGetMasterPos;
extern DPRGETCOMPENSATIONPOS DeviceDPRGetCompensationPos;
extern DPRGETPREVDAC DeviceDPRGetPrevDAC;
extern DPRGETMOVETIME DeviceDPRGetMoveTime;


#ifdef __cplusplus
}
#endif

#endif