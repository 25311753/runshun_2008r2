#ifndef  __LAODY_LFC_HONGXINERP_BASECODE__
#define  __LAODY_LFC_HONGXINERP_BASECODE__

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <comctrls.hpp>

#include <vcl.h>
#include "utilcls.h"
#include <stdarg.h>

#include "LfcString.h"
#include "LfcArray_s.h"
#include "LfcTempl.h"

#include "GSLfcBase.h"

#include "GlobalVal.h"

typedef const char* LPCSTR;

#define FLOW_CUT2STORE_IDX		1  //�ɿⵥ��ˮ�ŵ�������
#define FLOW_ORDER_IDX				7	 //������ˮ�ŵ�������
#define FLOW_BURSARMONTH_IDX	8  //����·ݵ�������

#define ITEMSIZE(p) (sizeof(p)/sizeof(p[0]))

int GetFlowNumber(int nIdx,bool bAddFlow=true);
CString GetFlowString(int nIdx);

bool BeginTransact();
bool CommitTransact();
bool RollBackTransact();
bool RunTransactSQL(LPCSTR lpszSQL);

bool OpenQuery(TQuery *pQuery,LPCSTR lpszSQL);
bool OpenTable(TTable *pTable,LPCSTR lpszName,LPCSTR lpszFilter=NULL);
bool RunSQL(LPCSTR lpszSQL,bool bSelect=false);
bool RunSQL(TQuery *pQuery,LPCSTR lpszSQL,bool bSelect=false);
bool RunSQL2(LPCSTR lpszSQL,bool bSelect=false);


bool Get1Field(LPCSTR lpszSQL,LPCSTR lpszField,CStringArray &lstResult);
bool Get1Field(LPCSTR lpszSQL,LPCSTR lpszField,char *pszResult);

void GetSysDate(int &nYear,int &nMonth,int &nDay,bool bCloseDb=true);
TDateTime GetSysTime(bool bCloseDb=true);
//ע�ⷵ�ص�������������Ϊ��yyyy-mm-dd
CString GetSysDate(bool bCloseDb=true);
CString GetSysTimeStr();
//��ȡһ����ʱ�������
CString GetTmpTableName();

#endif  //__LAODY_LFC_TAISHUERP_BASECODE__
