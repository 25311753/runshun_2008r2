//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <assert.h>
#pragma hdrstop

#include "DataModule.h"
#include "BaseCode.h"
#include "SumDecideNotEnd.h"
#include "TConst.h"
#include "LdyInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSumDecideNotEndForm *SumDecideNotEndForm;
//---------------------------------------------------------------------------

//--------------------------------------------------------------------------
//�ʺ�����δ��ɰ���ͳ�Ʊ�ӿڣ��ļ���SumDecideNotEnd.*
void SumDecideNotEnd(int nAuth)
{
	CALL_FORM(TSumDecideNotEndForm);
}
//-------------------------------------------------------------------------

__fastcall TSumDecideNotEndForm::TSumDecideNotEndForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TSumDecideNotEndForm::btnExitClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------

void __fastcall TSumDecideNotEndForm::btnToExcelClick(TObject *Sender)
{
	Variant vExcel,vSheet;
  int nRow,i;
  TListItem *pItem;

  try
  {
	  nRow=1;
		if(!OpenExcel("�б�.xls",vExcel,vSheet))	return;
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
				NULL);
		}
	}
	catch(...)
	{
	}
}
//---------------------------------------------------------------------------

#include "PrnSumNotEnd.h"
void __fastcall TSumDecideNotEndForm::btnPrintClick(TObject *Sender)
{
	CString szSQL,szTable;
	TPrnSumNotEndForm *pPrn;
	TListItem *pItem;
	int i;
	TDateTime tNow;

	szTable=GetTmpTableName();
	try
	{
		BeginTransact();
		szSQL.Format("create table dbo.%s ("
                        "m_id   varchar(80),"
			"m_sh   varchar(80),"
			"m_corp varchar(80),"
			"m_ap   varchar(80),"
                        "m_afd  varchar(80),"
			"m_type varchar(80),"
			"m_mnname varchar(80),"
                        "m_mfsname varchar(80),"
			"m_count varchar(80),"
			"m_hm   varchar(80),"
			"m_date varchar(80),"
			"m_money varchar(80)"
//			"m_mfname varchar(80),"
			")",szTable);
		RunTransactSQL(szSQL);

		for(i=0;i<lstView->Items->Count;i++)
		{
			pItem=lstView->Items->Item[i];
			szSQL.Format("insert into %s values(",szTable);
      			szSQL+=Int2DBString(i+1);
                        szSQL+=",";	szSQL+=Str2DBString(pItem->Caption.c_str());
			szSQL+=",";	szSQL+=Str2DBString(pItem->SubItems->Strings[0].c_str());
			szSQL+=",";	szSQL+=Str2DBString(pItem->SubItems->Strings[1].c_str());
			szSQL+=",";	szSQL+=Str2DBString(pItem->SubItems->Strings[2].c_str());
			szSQL+=",";	szSQL+=Str2DBString(pItem->SubItems->Strings[3].c_str());
			szSQL+=",";	szSQL+=Str2DBString(pItem->SubItems->Strings[4].c_str());
			szSQL+=",";	szSQL+=Str2DBString(pItem->SubItems->Strings[5].c_str());
			szSQL+=",";	szSQL+=Str2DBString(pItem->SubItems->Strings[6].c_str());
			szSQL+=",";	szSQL+=Str2DBString(pItem->SubItems->Strings[7].c_str());
			szSQL+=",";	szSQL+=Str2DBString(pItem->SubItems->Strings[8].c_str());
			szSQL+=",";	szSQL+=Str2DBString(pItem->SubItems->Strings[9].c_str());                        
			szSQL+=")";
			RunTransactSQL(szSQL);
		}
		CommitTransact();
	}
	catch(...)
	{
		RollBackTransact();
		ShowMessage("���ݿ�����쳣");
		return;
	}

	pPrn=new TPrnSumNotEndForm(this);
	assert(pPrn!=NULL);

	tNow=GetSysTime(false);

	pPrn->txtPrnTime->Caption=(LPCSTR)Time2DBTimeString(tNow);
	pPrn->txtDate->Caption="�ʺ���";
        if(!m_bMonth)
        {
                pPrn->txtTitle->Caption="�ʺ�����δ��ɰ����ܱ�ͳ��(�߰쵥)";
                pPrn->QRLabel12->Caption="��ʱ˵��";
                pPrn->QRLabel17->Caption="˵��: �ʺ�������������ͳ�Ʒ�Χ֮�ھͱ���Ϊ����";
                pPrn->QRLabel19->Caption="      һ���:�ʺ���������=�Ӽ���+7  ����:�ʺ���������=�Ӽ���+3";
        }
        else
        {
                pPrn->txtTitle->Caption="�ʺ����ڰ����±���ϸ";
                pPrn->QRLabel12->Caption="��ע";
                pPrn->QRLabel17->Caption="˵��: ���ڰ�����ָ �ʺ�����������ͳ�Ʒ�Χ֮�ڲ��ҳʺ���������<ʵ�ʳʺ���";
                pPrn->QRLabel19->Caption="      һ���:�ʺ���������=�Ӽ���+7  ����:�ʺ���������=�Ӽ���+3";
        }
	szSQL.Format("select * from %s",szTable);
	OpenQuery(dm1->sqlPrint,szSQL);
	pPrn->PrnView->PreviewModal();
	delete pPrn;

DROP_TABLE:
	szSQL.Format("drop table %s",szTable);
	RunSQL(dm1->Query1,szSQL);
}
//---------------------------------------------------------------------------
void __fastcall TSumDecideNotEndForm::btnQueryClick(TObject *Sender)            //�����ܱ������±�,����ͳ��ͳ������
{
	CString szSQL;
	TListItem *pItem;
	TDateTime tNow;
	char strCorp[80],strFact[80],strTemp[256],strTemp2[80],strDate0[80],strDate1[80],strSHID[256],strID[80];
	CArray<BASE2COLDATA,BASE2COLDATA>lstHM,lstSH;
	BASE2COLDATA rwData;
	int i,nSel;
        int nRightSH,nRightRP;
        unsigned short year0,month0,day0,year1,month1,day1;
//---------- new 0711---ȡ�ò����߶�Ӧ�Ĳɹ����ű�(��StockHandleman�еĹ�˾��) ����ӦȨ��-srtSHID-------
        szSQL.Format("select * from StockHandleman where sh_code='%s'",operatorcode);
        RunSQL(dm1->Query1,szSQL,true);
        strcpy(strSHID,dm1->Query1->FieldByName("sh_sdcode")->AsString.c_str());
        nRightSH=GetOperatorRight(operatorcode.c_str(),"IDM_PRSHLPRICE");            //ȡ��Ȩ��:�����鿴����
        nRightRP=GetOperatorRight(operatorcode.c_str(),"IDM_RAWPRICERIGHT");            //ȡ��Ȩ�ޣ��鿴ԭ�ϵ���
//------------------------------------------------------------------------------

//	tNow=GetSysTime(false);
//	tNow -= atoi(edtDays->Text.c_str());                                    //�ʺ���+�ſ��գ�>��ǰʱ��

	szSQL="select * from HandleMan order by hm_code";
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{
		strcpy(rwData.strCode,dm1->Query1->FieldByName("hm_code")->AsString.c_str());
		strcpy(rwData.strName,dm1->Query1->FieldByName("hm_name")->AsString.c_str());
		lstHM.Add(rwData);
		dm1->Query1->Next();
	}

	szSQL="select * from StockHandleman order by sh_code";
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{
		strcpy(rwData.strCode,dm1->Query1->FieldByName("sh_code")->AsString.c_str());
		strcpy(rwData.strName,dm1->Query1->FieldByName("sh_name")->AsString.c_str());
		lstSH.Add(rwData);
		dm1->Query1->Next();
	}

	lstView->Items->Clear();

//------�ܱ�--------------------------------------------------------------------
  if(!m_bMonth)
  {
	szSQL.Format("select * from AskPurchase,PurchaseDetail,materialno"
		" where ap_code=substring(pd_code,1,7)"
                " and substring(ap_code,1,1)!='Q'"                              //��ͳ��ͳ������
		" and mn_code=pd_mncode"
                " and ap_code not in ( select substring(df_apcode,1,7) from DecideFactory)"     //��δ�ʺ�
                );
        DateTimePicker1->DateTime.DecodeDate(&year0,&month0,&day0);
        sprintf(strDate0,"%04d-%02d-%02d",year0,month0,day0);
        DateTimePicker2->DateTime.DecodeDate(&year1,&month1,&day1);
        sprintf(strDate1,"%04d-%02d-%02d",year1,month1,day1);
//      sprintf(strTemp," and ap_affirmdate between '%s' and '%s'",strDate0,strDate1);
//        szSQL+=strTemp;
//      sprintf(strTemp," and((ap_affirmdate+7<'%s' and ap_type='I') or (ap_affirmdate+3<'%s' and ap_type='N'))",strDate1,strDate1);    //��ǰ�����ѳ����Ӽ����ڼ��Ϸſ���������İ�������Ϊ����
        sprintf(strTemp," and(((ap_affirmdate+7 between '%s' and '%s') and ap_type='I') or ((ap_affirmdate+3 between '%s' and '%s') and ap_type='N'))",strDate0,strDate1,strDate0,strDate1);    //�ʺ�����(ap_affirmdate+7��3)��ͳ�Ʒ�Χ�ڣ���İ�������Ϊ����
        szSQL+=strTemp;                         //��Ӧ�ĳʺ�������������ͳ�Ʒ�Χ֮�ھͱ���Ϊ����

	if((nSel=lstHandle->ItemIndex)>0 && nSel<m_lstHandleID.GetSize())
	{
		sprintf(strTemp," and ap_shcode='%s'",m_lstHandleID[nSel]);
		szSQL+=strTemp;
	}
	if((nSel=lstCorp->ItemIndex)>0 && nSel<m_lstCorpID.GetSize())
	{
		sprintf(strTemp," and substring(ap_reqdepart,1,2)='%s'",m_lstCorpID[nSel]);
		szSQL+=strTemp;
	}
/*	if((nSel=lstFactory->ItemIndex)>0 && nSel<m_lstFactID.GetSize())
	{
		sprintf(strTemp," and df_mfplan='%s'",m_lstFactID[nSel]);
		szSQL+=strTemp;
	}       */
	if((nSel=lstStockDepart->ItemIndex)>=0 && nSel<m_lstStockDepartID.GetSize())
	{
		sprintf(strTemp," and ap_sdcode='%s'",m_lstStockDepartID[nSel]);
		szSQL+=strTemp;
	}
	szSQL+=" order by ap_shcode,ap_affirmdate";
   }
//------�±�--------------------------------------------------------------------
  else
  {
//	tNow=GetSysTime(false);
//	tNow -= atoi(edtDays->Text.c_str());                                    //�ʺ���+�ſ��գ�>��ǰʱ��

	szSQL.Format("select * from DecideFactory,CompareFactory,AskPurchase,PurchaseDetail,materialno,Manufactur"
		" where pd_code=df_apcode"
		" and cf_apcode=pd_code"
                " and cf_mfcode=df_mfplan"                                      //new 0729�ҳ��⹺����
		" and ap_code=substring(pd_code,1,7)"
                " and substring(ap_code,1,1)!='Q'"                              //��ͳ��ͳ������
		" and mn_code=pd_mncode"
		" and cf_mfcode=mf_code"
		" and df_decidedate is null"                                    //�ѳʺ˻�δ�¶���(δ����"����"��һ��)
                " and((ap_affirmdate+7<df_plandate  and ap_type='I') or (ap_affirmdate+3<df_plandate and ap_type='N'))"   //�ʺ���������<ʵ�ʳʺ���(df_plandate)
//--------------����:�ʺ���-�Ӽ���>7 ��Ϊ���ڣ�һ���:�ʺ���-�Ӽ���>3 ��Ϊ����--
//		" and ((df_plandate-ap_affirmdate>7 and ap_type='I') or (df_plandate-ap_affirmdate>3 and ap_type='N'))"
                );
//		" and df_plandate<'%s'",Time2DBDateString(tNow -= atoi(ap_type=="I" ? "7" : "3")));
//		" and df_plandate<'%s'",Time2DBDateString(tNow));
        DateTimePicker1->DateTime.DecodeDate(&year0,&month0,&day0);
        sprintf(strDate0,"%04d-%02d-%02d",year0,month0,day0);
        DateTimePicker2->DateTime.DecodeDate(&year1,&month1,&day1);
        sprintf(strDate1,"%04d-%02d-%02d",year1,month1,day1);
//        sprintf(strTemp," and ap_affirmdate between '%s' and '%s'",strDate0,strDate1);
//        szSQL+=strTemp;
        sprintf(strTemp," and(((ap_affirmdate+7 between '%s' and '%s') and ap_type='I') or ((ap_affirmdate+3 between '%s' and '%s') and ap_type='N'))",strDate0,strDate1,strDate0,strDate1);    //�ʺ�����(ap_affirmdate+7��3)��ͳ�Ʒ�Χ�ڣ���İ�������Ϊ����
        szSQL+=strTemp;                //��Ӧ�ĳʺ�������������ͳ�Ʒ�Χ֮��

	if((nSel=lstHandle->ItemIndex)>0 && nSel<m_lstHandleID.GetSize())
	{
		sprintf(strTemp," and ap_shcode='%s'",m_lstHandleID[nSel]);
		szSQL+=strTemp;
	}
	if((nSel=lstCorp->ItemIndex)>0 && nSel<m_lstCorpID.GetSize())
	{
		sprintf(strTemp," and substring(ap_reqdepart,1,2)='%s'",m_lstCorpID[nSel]);
		szSQL+=strTemp;
	}
	if((nSel=lstFactory->ItemIndex)>0 && nSel<m_lstFactID.GetSize())
	{
		sprintf(strTemp," and df_mfplan='%s'",m_lstFactID[nSel]);
		szSQL+=strTemp;
	}
	if((nSel=lstStockDepart->ItemIndex)>=0 && nSel<m_lstStockDepartID.GetSize())
	{
		sprintf(strTemp," and ap_sdcode='%s'",m_lstStockDepartID[nSel]);
		szSQL+=strTemp;
	}
	szSQL+=" order by ap_shcode,ap_affirmdate";
  }
/*
	szSQL.Format("select * from DecideFactory,CompareFactory,AskPurchase,PurchaseDetail,materialno"
		" where pd_code=df_apcode"
		" and cf_apcode=pd_code"
		" and ap_code=substring(pd_code,1,7)"
		" and mn_code=pd_mncode"
		" and df_decidedate is null"
		" and df_plandate<'%s'",Time2DBDateString(tNow));
	if((nSel=lstHandle->ItemIndex)>0 && nSel<m_lstHandleID.GetSize())
	{
		sprintf(strTemp," and ap_shcode='%s'",m_lstHandleID[nSel]);
		szSQL+=strTemp;
	}
	if((nSel=lstCorp->ItemIndex)>0 && nSel<m_lstCorpID.GetSize())
	{
		sprintf(strTemp," and substring(ap_reqdepart,1,2)='%s'",m_lstCorpID[nSel]);
		szSQL+=strTemp;
	}
	if((nSel=lstFactory->ItemIndex)>0 && nSel<m_lstFactID.GetSize())
	{
		sprintf(strTemp," and df_mfplan='%s'",m_lstFactID[nSel]);
		szSQL+=strTemp;
	}
	if((nSel=lstStockDepart->ItemIndex)>=0 && nSel<m_lstStockDepartID.GetSize())
	{
		sprintf(strTemp," and ap_sdcode='%s'",m_lstStockDepartID[nSel]);
		szSQL+=strTemp;
	}
	szSQL+=" order by ap_shcode,ap_affirmdate";
*/
//------------------------------------------------------------------------------
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{
		pItem=lstView->Items->Add();
		assert(pItem!=NULL);
		strcpy(strTemp,dm1->Query1->FieldByName("ap_shcode")->AsString.c_str());
		for(i=0;i<lstSH.GetSize();i++)
		{
			if(strcmp(lstSH[i].strCode,strTemp)==0)
			{
				pItem->Caption=lstSH[i].strName;
				break;
			}
		}
//		pItem->Caption=dm1->Query1->FieldByName("sh_name")->AsString;
		strcpy(strCorp,dm1->Query1->FieldByName("ap_reqdepart")->AsString.c_str());
		strCorp[2]='\0';
		for(i=1;i<m_lstCorpID.GetSize();i++)
		{
			if(m_lstCorpID[i]==strCorp)
			{
				pItem->SubItems->Add(lstCorp->Items->Strings[i]);
				break;
			}
		}
		if(i==m_lstCorpID.GetSize())	pItem->SubItems->Add("");
		pItem->SubItems->Add(dm1->Query1->FieldByName("pd_code")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("ap_affirmdate")->AsString);      //�Ӽ���
		pItem->SubItems->Add(dm1->Query1->FieldByName("ap_type")->AsString=="I" ? "����" : "��ͨ");
		pItem->SubItems->Add(dm1->Query1->FieldByName("mn_name")->AsString);
                if(!m_bMonth)   pItem->SubItems->Add("");
                else            pItem->SubItems->Add(dm1->Query1->FieldByName("mf_shortname")->AsString);
//--------------�ܱ�------------------------------------------------------------
                if(!m_bMonth)   pItem->SubItems->Add(dm1->Query1->FieldByName("pd_count")->AsString);
                else
		pItem->SubItems->Add(FloatField2String(dm1->Query1->FieldByName("cf_quotenum")));
//------------------------------------------------------------------------------                
		strcpy(strTemp,dm1->Query1->FieldByName("ap_hmcode")->AsString.c_str());
		for(i=0;i<lstHM.GetSize();i++)
		{
			if(strcmp(lstHM[i].strCode,strTemp)==0)
			{
				pItem->SubItems->Add(lstHM[i].strName);
				break;
			}
		}
		if(i==lstHM.GetSize())	pItem->SubItems->Add("");
//--------------�ܱ�ʱ�޳ʺ���--------------------------------------------------
                if(!m_bMonth)   pItem->SubItems->Add("");
                else
                pItem->SubItems->Add(DateField2Text(dm1->Query1->FieldByName("df_plandate")));

	strcpy(strID,dm1->Query1->FieldByName("ap_sdcode")->AsString.c_str());
        strcpy(strTemp,dm1->Query1->FieldByName("mn_code")->AsString.c_str());
        if(!m_bMonth)     pItem->SubItems->Add("");                  //�ܱ�,���޼�Ǯ
        else
        {
             if(operatorcode=="admin")
             {
		pItem->SubItems->Add((LPCSTR)Float2DBString(dm1->Query1->FieldByName("cf_quotenum")->AsFloat*dm1->Query1->FieldByName("cf_higgleprice")->AsFloat,2));
             }
             else if(nRightSH==3)
             {
                if((dm1->Query1->FieldByName("ap_sdcode")->AsString=='2' || dm1->Query1->FieldByName("ap_sdcode")->AsString=='3') && strTemp[1]=='M')     //���ź��ڵ�ԭ�ϲɹ� //����"�ɹ������ɻ���鿴"��Ȩ�ޣ��򾭰����Ի���鿴
                {
                        if(nRightRP!=3 && strcmp(strID,strSHID)!=0)
                        {
                                pItem->SubItems->Add("");
                        }
                        else
                        {
                		pItem->SubItems->Add((LPCSTR)Float2DBString(dm1->Query1->FieldByName("cf_quotenum")->AsFloat*dm1->Query1->FieldByName("cf_higgleprice")->AsFloat,2));

                        }

                }
                else
                {
                        if(strcmp(strID,strSHID)!=0)
                        {
                                pItem->SubItems->Add("");
                        }
                        else
                        {
                		pItem->SubItems->Add((LPCSTR)Float2DBString(dm1->Query1->FieldByName("cf_quotenum")->AsFloat*dm1->Query1->FieldByName("cf_higgleprice")->AsFloat,2));
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
                        }
                        else
                        {
        		pItem->SubItems->Add((LPCSTR)Float2DBString(dm1->Query1->FieldByName("cf_quotenum")->AsFloat*dm1->Query1->FieldByName("cf_higgleprice")->AsFloat,2));
                        }

                }
                else
                {
                        if(operatorcode!=dm1->Query1->FieldByName("ap_shcode")->AsString)
                        {
                                pItem->SubItems->Add("");
                        }
                        else
                        {
        		pItem->SubItems->Add((LPCSTR)Float2DBString(dm1->Query1->FieldByName("cf_quotenum")->AsFloat*dm1->Query1->FieldByName("cf_higgleprice")->AsFloat,2));
                        }
                }
             }
        }
//--------------new 0704--------------------------------------------------------
/*                strcpy(strTemp2,dm1->Query1->FieldByName("ap_sdcode")->AsString.c_str());
                if(g_theOperator.op_cpcode[0]!='E' && (strcmp(strTemp2,"2")==0 || strcmp(strTemp2,"3")==0))
                {
                        pItem->SubItems->Add("");
                }
                else if(!m_bMonth)   pItem->SubItems->Add("");                  //�ܱ�,�޼�Ǯ
                else
                {
		pItem->SubItems->Add((LPCSTR)Float2DBString(dm1->Query1->FieldByName("cf_quotenum")->AsFloat*dm1->Query1->FieldByName("cf_higgleprice")->AsFloat,2));
                }       */
//------------------------------------------------------------------------------
/*		strcpy(strFact,dm1->Query1->FieldByName("df_mfplan")->AsString.c_str());
		for(i=1;i<m_lstFactID.GetSize();i++)
		{
			if(m_lstFactID[i]==strFact)
			{
				pItem->SubItems->Add(lstFactory->Items->Strings[i]);
				break;
			}
		}
		if(i==m_lstFactID.GetSize())	pItem->SubItems->Add("");
*/

//		pItem->SubItems->Add((LPCSTR)Float2DBString(dm1->Query1->FieldByName("cf_quotenum")->AsFloat*dm1->Query1->FieldByName("cf_quoteprice")->AsFloat,2));

//		pItem->SubItems->Add(dm1->Query1->FieldByName("sh_name")->AsString);
/*		strcpy(strTemp,dm1->Query1->FieldByName("ap_sdcode")->AsString.c_str());
		for(i=0;i<m_lstStockDepartID.GetSize();i++)
		{
			if(m_lstStockDepartID[i]==strTemp)
			{
				pItem->SubItems->Add(lstStockDepart->Items->Strings[i]);
				break;
			}
		}
		if(i==m_lstStockDepartID.GetSize())	pItem->SubItems->Add("");
*/
                pItem->SubItems->Add("");
		dm1->Query1->Next();
	}
}
//---------------------------------------------------------------------------
void __fastcall TSumDecideNotEndForm::FormShow(TObject *Sender)
{
	CString szSQL;
	char strTemp[256],strCorp2[80];
	int i;
	bool bFind;
//------new 0704----------ȡ�ò����߹�˾��--------------------------------------
/*        szSQL.Format("select op_cpcode from operator where op_code='%s'",operatorcode.c_str());
	RunSQL(dm1->Query1,szSQL,true);
        strcpy(strCorp2,dm1->Query1->FieldByName("op_cpcode")->AsString.c_str());       */
//------------------------------------------------------------------------------
/*	m_lstHandleID.Add("");
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

	m_lstFactID.Add("");
	lstFactory->Items->Add("ȫ��");
	szSQL="select * from Manufactur order by mf_code";
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{
		m_lstFactID.Add(dm1->Query1->FieldByName("mf_code")->AsString.c_str());
		lstFactory->Items->Add(dm1->Query1->FieldByName("mf_shortname")->AsString);
		dm1->Query1->Next();
	}
	lstFactory->ItemIndex=0;

/*	m_lstHandleID.Add("");
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

	m_lstFactID.Add("");
	lstFactory->Items->Add("ȫ��");
	szSQL="select * from Manufactur order by mf_code";
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{
		m_lstFactID.Add(dm1->Query1->FieldByName("mf_code")->AsString.c_str());
		lstFactory->Items->Add(dm1->Query1->FieldByName("mf_shortname")->AsString);
		dm1->Query1->Next();
	}
	lstFactory->ItemIndex=0;
*/
//------new 0704----------------------------------------------------------------
/*        if(strcmp(strCorp2,"E")==0 || operatorcode=="admin")
	 {
          szSQL="select * from StockDepart order by sd_code";
         }
        else
         {
          szSQL="select * from StockDepart where sd_code<>'2' and sd_code<>'3' order by sd_code";
         }      */
//------------------------------------------------------------------------------
/*	szSQL="select * from StockDepart order by sd_code";
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{
		m_lstStockDepartID.Add(dm1->Query1->FieldByName("sd_code")->AsString.c_str());
		lstStockDepart->Items->Add(dm1->Query1->FieldByName("sd_name")->AsString);
		dm1->Query1->Next();
	}
*/
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
//                    lstStockDepart->ItemIndex=0;
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
//                    lstStockDepart->ItemIndex=0;

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
//                    lstStockDepart->ItemIndex=0;

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
//                    lstStockDepart->ItemIndex=0;

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
//                    lstStockDepart->ItemIndex=0;

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
//                    lstStockDepart->ItemIndex=0;

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
//                    lstStockDepart->ItemIndex=0;

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

	szSQL.Format("select * from StockHandleman where sh_code='%s'",operatorcode.c_str());
	RunSQL(dm1->Query1,szSQL,true);
	bFind=false;
	if(!dm1->Query1->Eof)           //�ҵ���֤���������Ǿ����е�һλ
	{
		strcpy(strTemp,dm1->Query1->FieldByName("sh_sdcode")->AsString.c_str());
		for(i=0;i<m_lstStockDepartID.GetSize();i++)
		{
			if(m_lstStockDepartID[i]==strTemp)
			{
				lstStockDepart->ItemIndex=i;                    //�Զ�ץȡ��Ӧ�ɹ����ŵ�����
				bFind=true;
				break;
			}
		}
	}
	if(!bFind)	lstStockDepart->ItemIndex=0;
   //	edtDays->Text="2";
}
//---------------------------------------------------------------------------
void __fastcall TSumDecideNotEndForm::lstFactoryExit(TObject *Sender)
{
	HandleListboxExit(lstFactory,&m_lstFactID);
}
//---------------------------------------------------------------------------


void __fastcall TSumDecideNotEndForm::RadioButton1Click(TObject *Sender)
{
        m_bMonth=false;
        lstFactory->Enabled=false;       //�ܱ�ʱ����δ�ж���������Ϣ������ס
}
//---------------------------------------------------------------------------

void __fastcall TSumDecideNotEndForm::RadioButton2Click(TObject *Sender)
{
        m_bMonth=true;
        lstFactory->Enabled=true;        //�±�ʱ�����гʺ˵���Ϣ
}
//---------------------------------------------------------------------------


