//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <assert.h>
#pragma hdrstop

#include "DataModule.h"
#include "BaseCode.h"
#include "SumEffect.h"
#include "TConst.h"
#include "LdyInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSumEffectForm *SumEffectForm;
//--------------------------------------------------------------------------
//�ճ���Чͳ�Ʊ���ӿڣ��ļ���SumEffect*
void SumEffect(int nAuth)
{
	CALL_FORM(TSumEffectForm);
}
//---------------------------------------------------------------------------
__fastcall TSumEffectForm::TSumEffectForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSumEffectForm::FormShow(TObject *Sender)
{
 CString szSQL;
 unsigned short year,month,day;
 int i;
 char strTemp[256];

 edtSNum->Text="";
 edtSPrice->Text="";

 m_lstHandleID.Add("");                                        //�ɹ�����
 lstHandle->Items->Add("ȫ��");
 szSQL="select * from StockHandleman order by sh_code";
 RunSQL(dm1->Query1,szSQL,true);
 while(!dm1->Query1->Eof)
  {
   m_lstHandleID.Add(dm1->Query1->FieldByName("sh_code")->AsString.c_str());
   lstHandle->Items->Add(dm1->Query1->FieldByName("sh_name")->AsString);
   dm1->Query1->Next();
  }
 lstHandle->ItemIndex=0;


 m_lstStockDepartID.Add("");                                        //�ɹ�����
 lstStockDepart->Items->Add("ȫ��");
 szSQL="select * from StockDepart order by sd_code";
 RunSQL(dm1->Query1,szSQL,true);
 while(!dm1->Query1->Eof)
  {
    m_lstStockDepartID.Add(dm1->Query1->FieldByName("sd_code")->AsString.c_str());
    lstStockDepart->Items->Add(dm1->Query1->FieldByName("sd_name")->AsString);
    dm1->Query1->Next();
  }
 lstStockDepart->ItemIndex=0;




/*
 if(g_theOperator.op_cpcode[0]=='A')
      { //���û�Ϊ�������ģ��Զ�Ѱ���������ĵĲɹ�������ɹ�����
                     szSQL="select * from StockDepart where sd_code='X' or sd_code='11' order by sd_code DESC";
                     RunSQL(dm1->Query1,szSQL,true);
                     while(!dm1->Query1->Eof)
                       {
                         m_lstStockDepartID.Add(dm1->Query1->FieldByName("sd_code")->AsString.c_str());
                         lstStockDepart->Items->Add(dm1->Query1->FieldByName("sd_name")->AsString);
                         dm1->Query1->Next();
                        }
                    lstStockDepart->ItemIndex=0;
        //�ɹ�����
                    m_lstHandleID.Add("");
                    lstHandle->Items->Add("�ɹ���ȫ���ɹ�����");
                    szSQL="select * from StockHandleman where sh_sdcode='X' or sh_sdcode='11'order by sh_code";
                    RunSQL(dm1->Query1,szSQL,true);
                    while(!dm1->Query1->Eof)
                       {
                         m_lstHandleID.Add(dm1->Query1->FieldByName("sh_code")->AsString.c_str());
                         lstHandle->Items->Add(dm1->Query1->FieldByName("sh_name")->AsString);
                         dm1->Query1->Next();
                        }
                     lstHandle->ItemIndex=0;

      }

 if(g_theOperator.op_cpcode[0]=='D')
      { //���û�Ϊ������˾���Զ�Ѱ�Һ����Ĳɹ�������ɹ�����
              szSQL="select * from StockDepart where sd_code='Y' or sd_code='1' order by sd_code";
                     RunSQL(dm1->Query1,szSQL,true);
                     while(!dm1->Query1->Eof)
                       {
                         m_lstStockDepartID.Add(dm1->Query1->FieldByName("sd_code")->AsString.c_str());
                         lstStockDepart->Items->Add(dm1->Query1->FieldByName("sd_name")->AsString);
                         dm1->Query1->Next();
                        }
                    lstStockDepart->ItemIndex=0;

        //�ɹ�����
                    m_lstHandleID.Add("");
                    lstHandle->Items->Add("ȫ���ɹ�����");
                    szSQL="select * from StockHandleman where sh_sdcode='Y' or sh_sdcode='1' order by sh_code";
                    RunSQL(dm1->Query1,szSQL,true);
                    while(!dm1->Query1->Eof)
                       {
                         m_lstHandleID.Add(dm1->Query1->FieldByName("sh_code")->AsString.c_str());
                         lstHandle->Items->Add(dm1->Query1->FieldByName("sh_name")->AsString);
                         dm1->Query1->Next();
                        }
                     lstHandle->ItemIndex=0;
      }
 if(g_theOperator.op_cpcode[0]=='B')
      { //���û�Ϊ���ʹ�˾���Զ�Ѱ�Һ��ʵĲɹ�����
              szSQL="select * from StockDepart where sd_code='9' order by sd_code";
                     RunSQL(dm1->Query1,szSQL,true);
                     while(!dm1->Query1->Eof)
                       {
                         m_lstStockDepartID.Add(dm1->Query1->FieldByName("sd_code")->AsString.c_str());
                         lstStockDepart->Items->Add(dm1->Query1->FieldByName("sd_name")->AsString);
                         dm1->Query1->Next();
                        }
                    lstStockDepart->ItemIndex=0;

        //�ɹ�����
                    m_lstHandleID.Add("");
                    lstHandle->Items->Add("ȫ���ɹ�����");
                    szSQL="select * from StockHandleman where sh_sdcode='9' order by sh_code";
                    RunSQL(dm1->Query1,szSQL,true);
                    while(!dm1->Query1->Eof)
                       {
                         m_lstHandleID.Add(dm1->Query1->FieldByName("sh_code")->AsString.c_str());
                         lstHandle->Items->Add(dm1->Query1->FieldByName("sh_name")->AsString);
                         dm1->Query1->Next();
                        }
                     lstHandle->ItemIndex=0;
      }
 if(g_theOperator.op_cpcode[0]=='C')
      { //���û�Ϊ������˾���Զ�Ѱ�Һ����Ĳɹ�����
              szSQL="select * from StockDepart where sd_code='4' or sd_code='5' order by sd_code";
                     RunSQL(dm1->Query1,szSQL,true);
                     while(!dm1->Query1->Eof)
                       {
                         m_lstStockDepartID.Add(dm1->Query1->FieldByName("sd_code")->AsString.c_str());
                         lstStockDepart->Items->Add(dm1->Query1->FieldByName("sd_name")->AsString);
                         dm1->Query1->Next();
                        }
                    lstStockDepart->ItemIndex=0;

        //�ɹ�����
                    m_lstHandleID.Add("");
                    lstHandle->Items->Add("ȫ���ɹ�����");
                    szSQL="select * from StockHandleman where sh_sdcode='4' or sh_sdcode='5' order by sh_code";
                    RunSQL(dm1->Query1,szSQL,true);
                    while(!dm1->Query1->Eof)
                       {
                         m_lstHandleID.Add(dm1->Query1->FieldByName("sh_code")->AsString.c_str());
                         lstHandle->Items->Add(dm1->Query1->FieldByName("sh_name")->AsString);
                         dm1->Query1->Next();
                        }
                     lstHandle->ItemIndex=0;
      }
 if(g_theOperator.op_cpcode[0]=='E')
      { //���û�Ϊ���Ź�˾���Զ�Ѱ�Һ��ŵĲɹ�����
              szSQL="select * from StockDepart where sd_code='2' or sd_code='3' order by sd_code";
                     RunSQL(dm1->Query1,szSQL,true);
                     while(!dm1->Query1->Eof)
                       {
                         m_lstStockDepartID.Add(dm1->Query1->FieldByName("sd_code")->AsString.c_str());
                         lstStockDepart->Items->Add(dm1->Query1->FieldByName("sd_name")->AsString);
                         dm1->Query1->Next();
                        }
                    lstStockDepart->ItemIndex=0;

        //�ɹ�����
                    m_lstHandleID.Add("");
                    lstHandle->Items->Add("ȫ���ɹ�����");
                    szSQL="select * from StockHandleman where sh_sdcode='2' or sh_sdcode='3' order by sh_code";
                    RunSQL(dm1->Query1,szSQL,true);
                    while(!dm1->Query1->Eof)
                       {
                         m_lstHandleID.Add(dm1->Query1->FieldByName("sh_code")->AsString.c_str());
                         lstHandle->Items->Add(dm1->Query1->FieldByName("sh_name")->AsString);
                         dm1->Query1->Next();
                        }
                     lstHandle->ItemIndex=0;
      }
if(g_theOperator.op_cpcode[0]=='F')
      { //���û�Ϊ�����˾���Զ�Ѱ�Һ���Ĳɹ�����
              szSQL="select * from StockDepart where sd_code='6' or sd_code='7' or sd_code='8' order by sd_code";
                     RunSQL(dm1->Query1,szSQL,true);
                     while(!dm1->Query1->Eof)
                       {
                         m_lstStockDepartID.Add(dm1->Query1->FieldByName("sd_code")->AsString.c_str());
                         lstStockDepart->Items->Add(dm1->Query1->FieldByName("sd_name")->AsString);
                         dm1->Query1->Next();
                        }
                    lstStockDepart->ItemIndex=0;

        //�ɹ�����
                    m_lstHandleID.Add("");
                    lstHandle->Items->Add("ȫ���ɹ�����");
                    szSQL="select * from StockHandleman where sh_sdcode='6' or sh_sdcode='7' order by sh_code";
                    RunSQL(dm1->Query1,szSQL,true);
                    while(!dm1->Query1->Eof)
                       {
                         m_lstHandleID.Add(dm1->Query1->FieldByName("sh_code")->AsString.c_str());
                         lstHandle->Items->Add(dm1->Query1->FieldByName("sh_name")->AsString);
                         dm1->Query1->Next();
                        }
                     lstHandle->ItemIndex=0;
      }
if(g_theOperator.op_cpcode[0]=='G')
      { //���û�Ϊ�����˾���Զ�Ѱ�Һ���Ĳɹ�����
              szSQL="select * from StockDepart where sd_code='10' order by sd_code";
                     RunSQL(dm1->Query1,szSQL,true);
                     while(!dm1->Query1->Eof)
                       {
                         m_lstStockDepartID.Add(dm1->Query1->FieldByName("sd_code")->AsString.c_str());
                         lstStockDepart->Items->Add(dm1->Query1->FieldByName("sd_name")->AsString);
                         dm1->Query1->Next();
                        }
                    lstStockDepart->ItemIndex=0;

        //�ɹ�����
                    m_lstHandleID.Add("");
                    lstHandle->Items->Add("ȫ���ɹ�����");
                    szSQL="select * from StockHandleman where sh_sdcode='10' order by sh_code";
                    RunSQL(dm1->Query1,szSQL,true);
                    while(!dm1->Query1->Eof)
                       {
                         m_lstHandleID.Add(dm1->Query1->FieldByName("sh_code")->AsString.c_str());
                         lstHandle->Items->Add(dm1->Query1->FieldByName("sh_name")->AsString);
                         dm1->Query1->Next();
                        }
                     lstHandle->ItemIndex=0;
      }
*/
 m_lstCorpID.Add("");
 lstCorp->Items->Add("ȫ��");
 szSQL="select * from DepartCorp order by dc_code";
 RunSQL(dm1->Query1,szSQL,true);
 while(!dm1->Query1->Eof)
  {
    m_lstCorpID.Add(dm1->Query1->FieldByName("dc_code")->AsString.c_str());
    lstCorp->Items->Add(dm1->Query1->FieldByName("dc_name")->AsString);
    dm1->Query1->Next();
  }
 lstCorp->ItemIndex=0;

 m_lstTypeID.Add("");
 lstType->Items->Add("ȫ��");
 szSQL="select * from diclargetype where lt_code!='M' order by lt_code";        //��ͳ��ԭ�ϲɹ�
 RunSQL(dm1->Query1,szSQL,true);
 while(!dm1->Query1->Eof)
  {
    m_lstTypeID.Add(dm1->Query1->FieldByName("lt_code")->AsString.c_str());
    lstType->Items->Add(dm1->Query1->FieldByName("lt_name")->AsString);
    dm1->Query1->Next();
  }
 lstType->ItemIndex=0;

 TDateTime tNow;
 tNow=GetSysTime(false);
 tNow.DecodeDate(&year,&month,&day);
 DateTimePicker1->Date=tNow;
 DateTimePicker2->Date=tNow;
}
//---------------------------------------------------------------------------

void __fastcall TSumEffectForm::btnQuitClick(TObject *Sender)
{
  Close();        
}
//---------------------------------------------------------------------------

void __fastcall TSumEffectForm::btntoExcelClick(TObject *Sender)
{
  Variant vExcel,vSheet;
  int nRow,i;
  TListItem *pItem;

  try
  {
	  nRow=1;
		if(!OpenExcel("�ճ���Чͳ�Ʊ���.xls",vExcel,vSheet))	return;
		ListHead2Excel(lstView,vSheet,nRow++);
		for(i=0;i<lstView->Items->Count;i++)
    {
    	pItem=lstView->Items->Item[i];
      InsertExcelItem(vSheet,nRow++,
      	pItem->Caption.c_str(),
        pItem->SubItems->Strings[0].c_str(),
        pItem->SubItems->Strings[1].c_str(),
        pItem->SubItems->Strings[2].c_str(),
        pItem->SubItems->Strings[3].c_str(),
        pItem->SubItems->Strings[4].c_str(),
        pItem->SubItems->Strings[5].c_str(),
        pItem->SubItems->Strings[6].c_str(),
        pItem->SubItems->Strings[7].c_str(),
        pItem->SubItems->Strings[8].c_str(),
        pItem->SubItems->Strings[9].c_str(),
        pItem->SubItems->Strings[10].c_str(),
        pItem->SubItems->Strings[11].c_str(),
				NULL);
    }
  }
	catch(...)
	{
	}
}
//---------------------------------------------------------------------------

void __fastcall TSumEffectForm::btnQueryClick(TObject *Sender)
{
  int nDays,nYear,nMonth,nDay;
  CString szSQL;
  TListItem *pItem;
  char strTemp[256],strSHID[256],strID[80];
  int nSel,i;
  char strDate0[80],strDate1[80],strTemp2[256];
  unsigned short year0,month0,day0,year1,month1,day1;
  int nRightSH,nRightRP;
  char sNumc[2046];
  float sNum[2046];
//---------- new 0711---ȡ�ò����߶�Ӧ�Ĳɹ����ű�(��StockHandleman�еĹ�˾��) ����ӦȨ��-srtSHID-------
        szSQL.Format("select * from StockHandleman where sh_code='%s'",operatorcode);
        RunSQL(dm1->Query1,szSQL,true);
        strcpy(strSHID,dm1->Query1->FieldByName("sh_sdcode")->AsString.c_str());
        nRightSH=GetOperatorRight(operatorcode.c_str(),"IDM_PRSHLPRICE");            //ȡ��Ȩ��
        nRightRP=GetOperatorRight(operatorcode.c_str(),"IDM_RAWPRICERIGHT");            //ȡ��Ȩ��
//------------------------------------------------------------------------------

  lstView->Items->Clear();

 szSQL.Format("select distinct * from AskPurchase,CompareFactory,DecideFactory,StockHandleman,StockDepart,"
              "diclargetype,PurchaseDetail,materialno,DepartCorp,DicDepart,diccorp,DicUnits"
              " where ap_code=substring(cf_apcode,1,7) and ap_code=substring(df_apcode,1,7)"
              " and cf_mfcode=df_mfplan and ap_shcode=sh_code and ap_sdcode=sd_code and cf_apcode=df_apcode"
              " and cf_apcode=pd_code and lt_code=substring(pd_mncode,2,1) and du_code=cf_quoteunit"
              " and pd_mncode=mn_code and cp_code=substring(ap_reqdepart,1,1)"
              " and ap_reqdepart=dd_code and dc_code=substring(ap_reqdepart,1,2)"
              );
        DateTimePicker1->DateTime.DecodeDate(&year0,&month0,&day0);
        sprintf(strDate0,"%04d-%02d-%02d",year0,month0,day0);
        DateTimePicker2->DateTime.DecodeDate(&year1,&month1,&day1);
        sprintf(strDate1,"%04d-%02d-%02d",year1,month1,day1);
        sprintf(strTemp," and df_plandate between '%s' and '%s'",strDate0,strDate1);
        szSQL+=strTemp;

 if((nSel=lstHandle->ItemIndex)>0 && nSel<m_lstHandleID.GetSize())
  {
    sprintf(strTemp," and ap_shcode='%s'",m_lstHandleID[nSel]);
    szSQL+=strTemp;
  }
// if((nSel=lstStockDepart->ItemIndex)>=0 && nSel<m_lstStockDepartID.GetSize())
 if((nSel=lstStockDepart->ItemIndex)>0 && nSel<m_lstStockDepartID.GetSize())
  {
    sprintf(strTemp," and ap_sdcode='%s'",m_lstStockDepartID[nSel]);
    szSQL+=strTemp;
  }
 if((nSel=lstCorp->ItemIndex)>0 && nSel<m_lstCorpID.GetSize())
  {
//    sprintf(strTemp," and substring(pd_mncode,1,1)='%s'",m_lstCorpID[nSel]);    //�ò��ϱ�ŵ���λ������˾��
    sprintf(strTemp," and substring(ap_reqdepart,1,2)='%s'",m_lstCorpID[nSel]);
    szSQL+=strTemp;
  }
  if((nSel=lstType->ItemIndex)>0 && nSel<m_lstTypeID.GetSize())
  {
    sprintf(strTemp," and substring(pd_mncode,2,1)='%s'",m_lstTypeID[nSel]);
    szSQL+=strTemp;
  }
  if(lstType->ItemIndex==0)                                                     //ѡ��ȫ����ʱ������ѯԭ�ϲɹ�
  {
    sprintf(strTemp," and substring(pd_mncode,2,1)!='M'");
    szSQL+=strTemp;
  }

 szSQL+=" order by cf_apcode,df_plandate";
 RunSQL(dm1->Query1,szSQL,true);
 while(!dm1->Query1->Eof)
  {
       pItem=lstView->Items->Add();
       assert(pItem!=NULL);
       pItem->Caption=dm1->Query1->FieldByName("cf_apcode")->AsString;
       pItem->SubItems->Add(DateField2Text(dm1->Query1->FieldByName("ap_reqdate")));
       pItem->SubItems->Add(dm1->Query1->FieldByName("cp_name")->AsString);
       pItem->SubItems->Add(dm1->Query1->FieldByName("lt_name")->AsString);
       pItem->SubItems->Add(dm1->Query1->FieldByName("mn_name")->AsString);
       pItem->SubItems->Add(FloatField2String(dm1->Query1->FieldByName("cf_quotenum"),2));



       pItem->SubItems->Add(dm1->Query1->FieldByName("du_name")->AsString);
//       pItem->SubItems->Add(dm1->Query1->FieldByName("cf_quoteprice")->AsString);
//       pItem->SubItems->Add(dm1->Query1->FieldByName("cf_higgleprice")->AsString);
//---new 0706----ֻ�е�����Ա�Ǳ�������ʱ���ſɲ鿴����-------------------------
//        strcpy(strTemp3,dm1->Query1->FieldByName("ap_shcode")->AsString.c_str());
/*���ǡ��������Բ鿴�� ��
���������ź��ڵİ�����Ϊԭ�ϲɹ�����ԭ�ϵ���Ϊ3��Ϊͬ�ɹ����ž��죬����Բ鿴�õ����ۣ�
����İ���(�����Ǻ��ź��ڻ��ԭ�ϲɹ�)���������Բ鿴���ۣ����಻����

�����ǡ��������Բ鿴�� ��
���������ź��ڵİ�����Ϊԭ�ϲɹ� ����ԭ�ϵ���Ϊ3����Ϊ�õ����죬����Բ鿴�õ����ۣ�
����İ���(�����Ǻ��ź��ڻ��ԭ�ϲɹ�)��Ҫ�����Լ��ſ��Բ鿴����
*/

	strcpy(strID,dm1->Query1->FieldByName("ap_sdcode")->AsString.c_str());
        strcpy(strTemp,dm1->Query1->FieldByName("mn_code")->AsString.c_str());

             if(operatorcode=="admin")
             {
                       pItem->SubItems->Add(dm1->Query1->FieldByName("cf_quoteprice")->AsString);
                       pItem->SubItems->Add(dm1->Query1->FieldByName("cf_higgleprice")->AsString);
             }
             else if(nRightSH==3)
             {
                if((dm1->Query1->FieldByName("ap_sdcode")->AsString=='2' || dm1->Query1->FieldByName("ap_sdcode")->AsString=='3') && strTemp[1]=='M')     //���ź��ڵ�ԭ�ϲɹ� //����"�ɹ������ɻ���鿴"��Ȩ�ޣ��򾭰����Ի���鿴
                {
                        if(nRightRP!=3 && strcmp(strID,strSHID)!=0)
                        {
                                pItem->SubItems->Add("");
                                pItem->SubItems->Add("");
                        }
                        else
                        {
                               pItem->SubItems->Add(dm1->Query1->FieldByName("cf_quoteprice")->AsString);
                               pItem->SubItems->Add(dm1->Query1->FieldByName("cf_higgleprice")->AsString);
                        }

                }
                else
                {
                        if(strcmp(strID,strSHID)!=0)
                        {
                                pItem->SubItems->Add("");
                                pItem->SubItems->Add("");
                        }
                        else
                        {
                               pItem->SubItems->Add(dm1->Query1->FieldByName("cf_quoteprice")->AsString);
                               pItem->SubItems->Add(dm1->Query1->FieldByName("cf_higgleprice")->AsString);
                        }
                }
             }
             else
             {
                if((dm1->Query1->FieldByName("ap_sdcode")->AsString=='2' || dm1->Query1->FieldByName("ap_sdcode")->AsString=='3') && strTemp[1]=='M')                                                    //����"�ɹ������ɻ���鿴"��Ȩ�ޣ��򾭰����Ի���鿴
                {
                        if(nRightRP!=3 && operatorcode!=dm1->Query1->FieldByName("ap_shcode")->AsString)
                        {
                                pItem->SubItems->Add("");
                                pItem->SubItems->Add("");
                        }
                        else
                        {
                               pItem->SubItems->Add(dm1->Query1->FieldByName("cf_quoteprice")->AsString);
                               pItem->SubItems->Add(dm1->Query1->FieldByName("cf_higgleprice")->AsString);
                        }

                }
                else
                {
                        if(operatorcode!=dm1->Query1->FieldByName("ap_shcode")->AsString)
                        {
                                pItem->SubItems->Add("");
                                pItem->SubItems->Add("");
                        }
                        else
                        {
                               pItem->SubItems->Add(dm1->Query1->FieldByName("cf_quoteprice")->AsString);
                               pItem->SubItems->Add(dm1->Query1->FieldByName("cf_higgleprice")->AsString);
                        }
                }
             }
/*             else
             {
                if(nRightSH==3)                                                    //����"�ɹ������ɻ���鿴"��Ȩ�ޣ��򾭰����Ի���鿴
                {
                        if(strcmp(strID,strSHID)!=0)
                        {
                                pItem->SubItems->Add("");
                                pItem->SubItems->Add("");
                        }
                        else
                        {
                               pItem->SubItems->Add(dm1->Query1->FieldByName("cf_quoteprice")->AsString);
                               pItem->SubItems->Add(dm1->Query1->FieldByName("cf_higgleprice")->AsString);
                        }
                }
                else                                                               //����ֻ�о����Լ��ſ��Բ鿴
                {       //��������ڵİ���
                        if(dm1->Query1->FieldByName("ap_sdcode")->AsString=='2' || dm1->Query1->FieldByName("ap_sdcode")->AsString=='3')
                        {
                                if(nRightRP!=3 && operatorcode!=dm1->Query1->FieldByName("ap_shcode")->AsString)
                                {
                                        pItem->SubItems->Add("");
                                        pItem->SubItems->Add("");
                                }
                                else
                                {
                                        pItem->SubItems->Add(dm1->Query1->FieldByName("cf_quoteprice")->AsString);
                                        pItem->SubItems->Add(dm1->Query1->FieldByName("cf_higgleprice")->AsString);
                                }
                        }
                        //���������糣
                        else
                        {
                                if(operatorcode!=dm1->Query1->FieldByName("ap_shcode")->AsString)
                                {
                                        pItem->SubItems->Add("");
                                        pItem->SubItems->Add("");
                                }
                                else
                                {
                                        pItem->SubItems->Add(dm1->Query1->FieldByName("cf_quoteprice")->AsString);
                                        pItem->SubItems->Add(dm1->Query1->FieldByName("cf_higgleprice")->AsString);
                                }
                        }
                }
             }
*/
//------------------------------------------------------------------------------
       pItem->SubItems->Add(dm1->Query1->FieldByName("cf_quotermb")->AsString);
       pItem->SubItems->Add(DateField2Text(dm1->Query1->FieldByName("df_plandate")));
       pItem->SubItems->Add(dm1->Query1->FieldByName("sh_name")->AsString);
       pItem->SubItems->Add(dm1->Query1->FieldByName("sd_name")->AsString);
       //pItem->SubItems->Add(FloatField2String(dm1->Query1->FieldByName("bf_count"),2));
       //pItem->SubItems->Add(dm1->Query1->FieldByName("mf_shortname")->AsString);
       //pItem->SubItems->Add(dm1->Query1->FieldByName("pd_code")->AsString);
      // pItem->SubItems->Add(dm1->Query1->FieldByName("sh_name")->AsString);
		dm1->Query1->Next();
      // strcpy(strTemp,dm1->Query1->FieldByName("ap_reqdepart")->AsString.c_str());
  }
//-----------ͳ����������-----------------------------------------------------
 szSQL.Format("select count(*), sum(cf_higgleprice*cf_quotenum)  from AskPurchase,CompareFactory,DecideFactory,StockHandleman,StockDepart,"
              "diclargetype,PurchaseDetail,materialno,DepartCorp,DicDepart,diccorp,DicUnits"
              " where ap_code=substring(cf_apcode,1,7) and ap_code=substring(df_apcode,1,7)"
              " and cf_mfcode=df_mfplan and ap_shcode=sh_code and ap_sdcode=sd_code and cf_apcode=df_apcode"
              " and cf_apcode=pd_code and lt_code=substring(pd_mncode,2,1) and du_code=cf_quoteunit"
              " and pd_mncode=mn_code and cp_code=substring(ap_reqdepart,1,1)"
              " and ap_reqdepart=dd_code and dc_code=substring(ap_reqdepart,1,2)"
              );
        DateTimePicker1->DateTime.DecodeDate(&year0,&month0,&day0);
        sprintf(strDate0,"%04d-%02d-%02d",year0,month0,day0);
        DateTimePicker2->DateTime.DecodeDate(&year1,&month1,&day1);
        sprintf(strDate1,"%04d-%02d-%02d",year1,month1,day1);
        sprintf(strTemp," and df_plandate between '%s' and '%s'",strDate0,strDate1);
        szSQL+=strTemp;

 if((nSel=lstHandle->ItemIndex)>0 && nSel<m_lstHandleID.GetSize())
  {
    sprintf(strTemp," and ap_shcode='%s'",m_lstHandleID[nSel]);
    szSQL+=strTemp;
  }
// if((nSel=lstStockDepart->ItemIndex)>=0 && nSel<m_lstStockDepartID.GetSize())
 if((nSel=lstStockDepart->ItemIndex)>0 && nSel<m_lstStockDepartID.GetSize())
  {
    sprintf(strTemp," and ap_sdcode='%s'",m_lstStockDepartID[nSel]);
    szSQL+=strTemp;
  }
 if((nSel=lstCorp->ItemIndex)>0 && nSel<m_lstCorpID.GetSize())
  {
//    sprintf(strTemp," and substring(pd_mncode,1,1)='%s'",m_lstCorpID[nSel]);    //�ò��ϱ�ŵ���λ������˾��
    sprintf(strTemp," and substring(ap_reqdepart,1,2)='%s'",m_lstCorpID[nSel]);
    szSQL+=strTemp;
  }
  if((nSel=lstType->ItemIndex)>0 && nSel<m_lstTypeID.GetSize())
  {
    sprintf(strTemp," and substring(pd_mncode,2,1)='%s'",m_lstTypeID[nSel]);
    szSQL+=strTemp;
  }

 RunSQL(dm1->Query1,szSQL,true);
 while(!dm1->Query1->Eof)
  {
//                ShowMessage(FloatField2String(dm1->Query1->FieldByName("sum(cf_quotenum)"));
                edtSNum->Text=dm1->Query1->Fields->Fields[0]->AsString;
                
                edtSPrice->Text=dm1->Query1->Fields->Fields[1]->AsString;
		dm1->Query1->Next();

  }

}
//---------------------------------------------------------------------------



void __fastcall TSumEffectForm::RadioButton2Click(TObject *Sender)
{
        m_sType=true;
        lstHandle->Enabled=true;
        DateTimePicker2->Enabled=false;
        lstStockDepart->Enabled=false;
        lstStockDepart->ItemIndex=0;
        lstCorp->Enabled=false;
        lstCorp->ItemIndex=0;
        lstType->Enabled=false;
        lstType->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TSumEffectForm::RadioButton1Click(TObject *Sender)
{
        m_sType=false;
        DateTimePicker2->Enabled=true;
        lstHandle->Enabled=true;
        lstStockDepart->Enabled=true;
        lstCorp->Enabled=true;
        lstType->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TSumEffectForm::DateTimePicker1Change(TObject *Sender)
{
        if(m_sType)
        DateTimePicker2->Date=DateTimePicker1->Date;             
}
//---------------------------------------------------------------------------

void __fastcall TSumEffectForm::RadioButton3Click(TObject *Sender)
{
        m_sType=true;
        lstHandle->Enabled=false;
        lstHandle->ItemIndex=0;
        DateTimePicker2->Enabled=false;
        lstStockDepart->Enabled=false;
        lstStockDepart->ItemIndex=0;
        lstCorp->Enabled=false;
        lstCorp->ItemIndex=0;
        lstType->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TSumEffectForm::RadioButton4Click(TObject *Sender)
{
        m_sType=true;
        lstHandle->Enabled=false;
        lstHandle->ItemIndex=0;
        DateTimePicker2->Enabled=false;
        lstStockDepart->Enabled=false;
        lstStockDepart->ItemIndex=0;
        lstCorp->Enabled=true;
        lstType->Enabled=false;
        lstType->ItemIndex=0;
}
//---------------------------------------------------------------------------

