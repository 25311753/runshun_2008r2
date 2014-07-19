#ifndef  __LAODY_LFC_GRACESTOCK_GSLFCLIB__
#define  __LAODY_LFC_GRACESTOCK_GSLFCLIB__

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

typedef const char* LPCSTR;

bool IsDateNULL(TDateTime &tDate);
bool IsDateFieldNULL(TField *pField);

//���ַ����еĿո��滻Ϊ0���������������Ϊsprintf���ܲ�0
CString ReplaceSpaceTo0(LPCSTR lpszString);

void EnableEdit(TEdit *pEdit,bool bEnable=true);
void EnableCombo(TComboBox *pBox,bool bEnable=true);

//�� yyyymmdd ���͵�����ת��Ϊ yyyy-mm-dd ���͵����ݿ�����
void Date2Dbdate(LPCSTR lpszIn,char *pszOut);
//�� yyyymmdd ���͵�����ת��Ϊ����������
void Date2YYMMDD(LPCSTR lpszIn,int &nYear,int &nMon,int &nDay);
TDateTime YYMMDD2Date(LPCSTR lpszDate);
//�� TDateTimeת��Ϊyyyymmdd�����ַ���
CString Date2YYMMDD(TDateTime tDate);

//�� TDateTime ����ת�������ݿ�ġ�yyyy-mm-dd�������ַ�����ע������û�е�����
CString Time2DBDateString(const TDateTime &tTime);
//�� TDateTime ����ת�������ݿ�ġ�yyyy-mm-dd hh:MM:ss�������ַ�����ע������û�е�����
CString Time2DBTimeString(const TDateTime &tTime);

void TrimLeft(char *pszString);
void TrimRight(char *pszString);
bool HasSpaceInString(LPCSTR lpszString);
bool StringIsInt(LPCSTR lpszString);
bool StringIsFloat(LPCSTR lpszString);

//��������,������Kg��=���(u)*���(mm)*����(M)*10^-6*0.907*֧��
//������������(u)�����(mm)������(M)��֧��
//��������(Kg)��������һλС��
double CalcWeight(double fThick,int nWidth,int nLength,int nBunch=1);

//����֧����֧��=����/(���(u)*���(mm)*����(M)*1e-6*0.907)
//������������(u)�����(mm)������(M)������(Kg)
//����֧����������������������룬���С�1�򷵻�1
int CalcBunch(double fThick,int nWidth,int nLength,double fWeight);

void KeyReturnHandle(TWinControl *ActiveControl);
void EditorKeyMaxLenHandle(TWinControl *ActiveControl,WORD Key);
void EditorArrowHandle(TWinControl *ActiveControl,WORD Key);

//�����ּ��϶���
CString AddComma(LPCSTR lpszSrc);

// ת Excel �����ӿ�
bool OpenExcel(LPCSTR lpszFName,Variant &vExcel,Variant &vSheet);
void InsertExcelItem(Variant &vSheet,int nLine,...);
void CloseExcel(Variant &vExcel);
void SetExcelColumnWidth(Variant &vExcel,int nColumn,int nWidth);
void ListHead2Excel(TListView *pList,Variant &vSheet,int nLine);

//�������ַ����еĵ����Ŵ�������ݿ��ܽ��ܵ���ʽ
CString ReplaceSingleQuote(LPCSTR lpszSrc);

AnsiString Text2DBString(AnsiString szText);
AnsiString Text2DBDate(AnsiString szText);
AnsiString Text2DBInt(AnsiString szText);
AnsiString Text2DBFloat(AnsiString szText,int nPrec=1);

CString Str2DBString	(LPCSTR lpszText);
CString Int2DBString	(int nVal);
CString Float2DBString(double fVal,int nPrec=1);

AnsiString StringField2Text(TField *pField);
AnsiString DateField2Text(TField *pField);
AnsiString IntField2Text(TField *pField);
AnsiString FloatField2Text(TField *pField);

AnsiString Field2DateString(TField *pField);
AnsiString Field2TimeString(TField *pField);

// ����ת����ʽ
double LB2KG(double fLB);
double Metre2KG(double fThick,double fWidth,double fLength,double fProp=0);
double YD2KG(double fThick,double fWidth,double fYD,double fProp=0);
double MM2KG(double fThick,double fWidth,double fMM,double fProp=0);

AnsiString IntField2String(TField *pField);
AnsiString FloatField2String(TField *pField,int nPrec=1);

typedef struct _ntime_row
{
	char 	dn_code[80];
  int		dn_type;
  char  dn_name[80];
} NTIME_ROW;
//���ö��ַ����ٲ��ҷ�����ʱ������
int GetNTimePos(LPCSTR lpszID,CArray<NTIME_ROW,NTIME_ROW>&lstData);

typedef struct _base_2coldata
{
	char strCode[80];
  char strName[256];
} BASE2COLDATA;

typedef struct _base_4coldata
{
	char strCode[80];
  char strName[256];
	char strCol2[256];
	char strCol3[256];
} BASE4COLDATA;

//���ö��ַ����ٲ���
int SearchBaseDataPos(LPCSTR lpszID,CArray<BASE2COLDATA,BASE2COLDATA>&lstData);

//�������ַ������ûس����ָ����ַ�����ȡ���������浽lstResult��
void SplitLine(LPCSTR lpszData,CStringArray &lstResult);
void SplitString(LPCSTR lpszSrc,CStringArray &lstResult,char cSplit);

CString SimpleEncode(LPCSTR lpszSrc);
CString SimpleDecode(LPCSTR lpszSrc);

//��һ�ַ����ֽ�ɶ���,width Ϊÿ������
int SplitStrToLines(LPCSTR lpszSrc,int nWidth,CStringArray &lstLine);

bool CheckDateEditor(TEdit *pEdit);
bool CheckFloatEditor(TEdit *pEdit,bool bAllowEmpty=false);
bool CheckIntEditor(TEdit *pEdit,bool bAllowEmpty=false);

void HandleListboxExit(TComboBox *pBox,CStringArray *plstID);

#endif  //__LAODY_LFC_GRACESTOCK_GSLFCLIB__