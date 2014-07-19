//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <assert.h>
#include "DataModule.h"
#include "BaseCode.h"
#include "LdyInterface.h"
#include "TConst.h"
#include <string.h>
#include <stdio.h>

#include "ApFilePage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TApFilePageForm *ApFilePageForm;
//---------------------------------------------------------------------------

void ApFilePage(int nAuth)
{
	if(nAuth==AUTH_NONE)	return;
	TApFilePageForm *pForm=new TApFilePageForm(Application);
	assert(pForm!=NULL);
	pForm->m_nAuthor=nAuth;
	pForm->ShowModal();
	delete pForm;
}


__fastcall TApFilePageForm::TApFilePageForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TApFilePageForm::btnExitClick(TObject *Sender)
{
	int nState=m_enState;
	if(m_enState==EN_STATE_IDLE)
		Close();
	else
	{
		m_enState=EN_STATE_IDLE;
		ResetCtrl();
        if(nState!=EN_STATE_NEW)
			btnQueryClick(Sender);
	}
}

void __fastcall TApFilePageForm::FormShow(TObject *Sender)
{
	CString szSQL;
        edtAPC->Text="";
        edtFC->Text="";
        edtPC->Text="";
	EnableEdit(edtFC,false);
	EnableEdit(edtPC,false);

/*	szSQL="select * from DepartCorp order by dc_code";
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{
		m_lstReDepID.Add(dm1->Query1->FieldByName("dc_code")->AsString.c_str());
		lstReDep->Items->Add(dm1->Query1->FieldByName("dc_name")->AsString);
		dm1->Query1->Next();
	}
	lstReDep->Text="";
	lstReDep->ItemIndex=-1;
*/
	m_enState=EN_STATE_IDLE;
	ResetCtrl();
	if(edtAPC->CanFocus())	edtAPC->SetFocus();
}

void TApFilePageForm::ResetCtrl()
{
	bool bIdle,bAuth;

	bIdle = m_enState==EN_STATE_IDLE;
	bAuth = m_nAuthor==AUTH_FULL;

	EnableEdit(edtAPC,m_enState!=EN_STATE_EDIT);
	EnableEdit(edtFC,(m_enState==EN_STATE_NEW || m_enState==EN_STATE_EDIT));
	EnableEdit(edtPC,(m_enState==EN_STATE_NEW || m_enState==EN_STATE_EDIT));
	EnableCombo(lstRem,(m_enState==EN_STATE_NEW || m_enState==EN_STATE_EDIT));
	btnQuery->Enabled = bIdle;
	btnNew	->Enabled = bIdle && bAuth;
	btnEdit	->Enabled = bIdle && bAuth && !m_szActiveID.IsEmpty();
	btnMove	->Enabled = bIdle && bAuth && !m_szActiveID.IsEmpty();

	btnOK	->Enabled = !bIdle;
	btnExit->Caption = bIdle ? "��    ��" : "��    ��";
}


void __fastcall TApFilePageForm::btnNewClick(TObject *Sender)
{
	m_enState=EN_STATE_NEW;
	ResetCtrl();
//	edtFC->Text=GenerateID();
	edtFC->Text="";
        edtAPC->Text="";
        edtPC->Text="";
        lstRem->ItemIndex=-1;
	if(edtAPC->CanFocus())	edtAPC->SetFocus();
}
//---------------------------------------------------------------------------


void __fastcall TApFilePageForm::btnEditClick(TObject *Sender)
{
	int nSel;
	if(m_szActiveID.IsEmpty())	return;
        
        m_enState=EN_STATE_EDIT;
        ResetCtrl();

        if(edtFC->CanFocus())	edtFC->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TApFilePageForm::btnMoveClick(TObject *Sender)
{
	int nSel;
	CString szSQL;
        char strTemp[2048];

	if(m_szActiveID.IsEmpty())	return;

	if(m_szActiveID!=edtAPC->Text.c_str())
	{
		ShowMessage("������ĵ���������ڲ�ѯ�Ĳ�ͬ");
		if(edtAPC->CanFocus())	edtAPC->SetFocus();
		return;
	}

	if(Application->MessageBox("��Ҫɾ������������","����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)	return;
	szSQL.Format("select * from FilePage where fp_apcode='%s'",m_szActiveID);
	RunSQL(dm1->Query1,szSQL,true);
	if(dm1->Query1->Eof)
	{
		ShowMessage("�������ݲ����ڣ������ѱ�������ɾ����");
		return;
	}

        try
	{
		szSQL.Format("delete FilePage where fp_apcode=%s",Str2DBString(m_szActiveID));
		RunSQL(dm1->Query1,szSQL);
	}
	catch(...)
	{
		sprintf(strTemp,"���ݿ����ʧ�ܣ�SQL=\n%s",szSQL);
		ShowMessage(strTemp);
		return;
	}

	m_szActiveID="";
        edtAPC->Text="";
        edtFC->Text="";
        edtPC->Text="";
        lstRem->ItemIndex=-1;
	ResetCtrl();
	return;

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


void __fastcall TApFilePageForm::btnQueryClick(TObject *Sender)
{
	CString szSQL;
	int i,nSel,j;
	char strID[80],strTemp[80],str1[80],str2[80],str3[80],str4[8];
       	m_szActiveID="";

        if(edtAPC->Text.IsEmpty())
	{
		ShowMessage("�������빺�����");
		if(edtAPC->CanFocus())	edtAPC->SetFocus();
		ResetCtrl();
		return;
	}

	szSQL.Format("select * from FilePage where fp_apcode='%s'",edtAPC->Text.c_str());
	RunSQL(dm1->Query1,szSQL,true);
	if(dm1->Query1->Eof)
	{
		ShowMessage("û�и�������");
		ResetCtrl();
		return;
	}
	edtFC->Text=dm1->Query1->FieldByName("fp_fno")->AsString;
	edtPC->Text=dm1->Query1->FieldByName("fp_pno")->AsString;
        strcpy(strTemp,dm1->Query1->FieldByName("fp_rem")->AsString.c_str());

        for(i=0;i<4;i++)
	{
  	        if(lstRem->Items->Strings[i]==dm1->Query1->FieldByName("fp_rem")->AsString.c_str())
  	        {
    	                lstRem->ItemIndex=i;
                        break;
                }
        }


/*        strcpy(str1,"����");
        strcpy(str2,"�˵�");
        strcpy(str3,"ȡ��");

        if(strcmp(strTemp,str1)==0)           strcpy(str4,'A');
        else if(strcmp(strTemp,str2)==0)      strcpy(str4,'B');
        else if(strcmp(strTemp,str3)==0)      strcpy(str4,'C');


        switch(strTemp)
	{
  	        case '����':	lstRem->ItemIndex=1;	break;
                case '�˵�':	lstRem->ItemIndex=2;	break;
                case 'ȡ��':	lstRem->ItemIndex=3;	break;
                default:	lstRem->ItemIndex=0;	break;
        }
*/
	m_szActiveID=edtAPC->Text.c_str();
	ResetCtrl();
}
//---------------------------------------------------------------------------
//����ʱ�Զ����������ĵ���
AnsiString TApFilePageForm::GenerateID()
{ //��Ÿ�ʽ��GR-S-XXXXXX

	int nYear,nMonth,nDay,nFlow;
  char strID[80],chMon,strSQL[1024],strKey[80];

  if(!dm1->OpenDatabase())	return "";
	GetSysDate(nYear,nMonth,nDay,false);

	if(nMonth<10)	chMon=(char)('0'+nMonth);
  else chMon=(char)('A'-10+nMonth);

  //�鿴�����Ƿ��Ѿ�����ˮ��
  sprintf(strKey,"FilePage_%04d%02d",nYear,nMonth);
  sprintf(strSQL,"select * from RuntimeData where rd_key='%s'",strKey);
	RunSQL(strSQL,true);
  if(dm1->Query1->Eof)
  { //���»�û����ˮ�ţ���ʼ��������ˮ��
  	sprintf(strSQL,"insert into RuntimeData values('%s',1,NULL,'����ҳ��%d��%d����ˮ��')",
    	strKey,nYear,nMonth);
    RunSQL(strSQL);
    nFlow=1;
  }

  else
  {
  	nFlow=dm1->Query1->FieldByName("rd_int")->AsInteger;
  }

  while(1)
  {
    sprintf(strID,"A%02d%02d%03d",nYear%10,nMonth,nFlow);                       //�Զ����ɵĵ���Ĭ����A��ͷ
    sprintf(strSQL,"select * from FilePage where fp_fno='%s'",strID);
    RunSQL(strSQL,true);
    if(dm1->Query1->Eof)	break;
		nFlow++;
  }
  sprintf(strSQL,"update RuntimeData set rd_int=%d where rd_key='%s'",nFlow,strKey);
  RunSQL(strSQL);

  return AnsiString(strID);

}




void __fastcall TApFilePageForm::btnOKClick(TObject *Sender)
{
	CString szSQL;
	char strID[80],strTemp[2048],strPC[80],strFC[80],strTemp1[80],strTemp2[80],strTemp11[80],strFC1[80],strTemp0[80],strList[256];
        int index;
        String str1;
	sprintf(strID,"%s",edtAPC->Text.c_str());
        sprintf(strFC,"%s",edtFC->Text.c_str());
        sprintf(strPC,"%s",edtPC->Text.c_str());
        if(edtAPC->Text.IsEmpty())
	{
		ShowMessage("�������빺����");
		if(edtAPC->CanFocus())	edtAPC->SetFocus();
		return;
	}
        if(edtFC->Text.IsEmpty())
	{
		ShowMessage("������鵵��");
		if(edtFC->CanFocus())	edtFC->SetFocus();
		return;
	}
        if(edtPC->Text.IsEmpty())
	{
		ShowMessage("������ҳ��");
		if(edtPC->CanFocus())	edtPC->SetFocus();
		return;
	}

	szSQL.Format("select * from FilePage where fp_apcode=%s",Str2DBString(strID));
        RunSQL(dm1->Query1,szSQL,true);
        strcpy(strTemp1,dm1->Query1->FieldByName("fp_fno")->AsString.c_str());
        strncpy(strTemp11,dm1->Query1->FieldByName("fp_fno")->AsString.c_str(),1);
        strTemp11[1] = '\0';
        strcpy(strTemp2,dm1->Query1->FieldByName("fp_pno")->AsString.c_str());
        strncpy(strFC1,strFC,1);
        strFC1[1] = '\0';


        if(m_enState==EN_STATE_NEW && !dm1->Query1->Eof)
	{
		ShowMessage("���ݿ������иõ��Ľ������ϣ����������뵥��");
		if(edtAPC->CanFocus())	edtAPC->SetFocus();
		return;
	}
        else if(m_enState==EN_STATE_EDIT && dm1->Query1->Eof)
	{
		ShowMessage("���ݿ���û�иõ��Ľ�������!");
		if(edtAPC->CanFocus())	edtAPC->SetFocus();
		return;
	}
        else if(m_enState==EN_STATE_EDIT && !dm1->Query1->Eof)
        {
	        szSQL.Format("select * from FilePage where fp_fno<>'%s' and fp_fno=%s",strTemp1,Str2DBString(strFC));
                RunSQL(dm1->Query1,szSQL,true);
                if(!dm1->Query1->Eof)
                {
                        ShowMessage("�õ��������н������ϣ���������һ���鵵��!");
		if(edtFC->CanFocus())	edtFC->SetFocus();
                        return;
                }
                //ͬ����Ҫ���ҳ���Ƿ��Ѿ�����
                szSQL.Format("select * from FilePage where substring(fp_fno,1,1)='%s' and fp_pno<>'%s' and fp_pno=%s",strFC1,strTemp2,Str2DBString(strPC));
                RunSQL(dm1->Query1,szSQL,true);
                if(!dm1->Query1->Eof)
                {
                        ShowMessage("��ҳ���н������ϣ���������һ��ҳ��!");
		if(edtPC->CanFocus())	edtPC->SetFocus();
                        return;
                }

        }
        else if(m_enState==EN_STATE_NEW)
        {
	        szSQL.Format("select * from FilePage where fp_fno=%s",Str2DBString(strFC));
                RunSQL(dm1->Query1,szSQL,true);
                if(!dm1->Query1->Eof)
                {
                        ShowMessage("�õ��������н������ϣ���������һ���鵵��!");
		if(edtFC->CanFocus())	edtFC->SetFocus();
                        return;
                }
                //ͬ����Ҫ���ҳ���Ƿ��Ѿ�����
                szSQL.Format("select * from FilePage where substring(fp_fno,1,1)='%s' and fp_pno=%s",strFC1,Str2DBString(strPC));
                RunSQL(dm1->Query1,szSQL,true);
                if(!dm1->Query1->Eof)
                {
                        ShowMessage("��ҳ���н������ϣ���������һ��ҳ��!");
		if(edtPC->CanFocus())	edtPC->SetFocus();
                        return;
                }
        }


	if(m_enState==EN_STATE_NEW)
	{
 		szSQL="insert into FilePage (fp_apcode,fp_fno,fp_pno,fp_rem) values(";
		szSQL+=Str2DBString(strID);
		szSQL+=",";	szSQL+=Str2DBString(edtFC->Text.c_str());
		szSQL+=",";	szSQL+=Str2DBString(edtPC->Text.c_str());

                if(lstRem->ItemIndex<0)
	        {
                	szSQL+=",";	szSQL+="NULL";
                }
                else
                {
                        index=lstRem->ItemIndex;
//                        strcpy(strList,lstRem->Items->Strings[index]->AsString.c_str());
                        str1=lstRem->Items->Strings[index];
                        szSQL+=",";	szSQL+=Str2DBString(str1.c_str());
                }
		szSQL+=")";
	}
        else if(m_enState==EN_STATE_EDIT)
	{
                szSQL="update FilePage set";
		szSQL+=" fp_fno=";           szSQL+=Str2DBString(edtFC->Text.c_str());
		szSQL+=",fp_pno=";	     szSQL+=Str2DBString(edtPC->Text.c_str());

                if(lstRem->ItemIndex<0)
	        {
                	szSQL+=",fp_rem=";	szSQL+="NULL";
                }
                else
                {
                        index=lstRem->ItemIndex;
//                        strcpy(strList,lstRem->Items->Strings[index]->AsString.c_str());
                        str1=lstRem->Items->Strings[index];
                        szSQL+=",fp_rem=";	szSQL+=Str2DBString(str1.c_str());
                }
                szSQL+=" where fp_apcode=";  szSQL+=Str2DBString(strID);
        }

	try
	{
		RunSQL(dm1->Query1,szSQL);
	}
	catch(...)
	{
		sprintf(strTemp,"���ݿ����ʧ��!SQL=\n%s",szSQL);
		ShowMessage(strTemp);
		return;
	}

        m_enState=EN_STATE_IDLE;
	ResetCtrl();
	if(btnNew->CanFocus())	btnNew->SetFocus();

}
//---------------------------------------------------------------------------

void __fastcall TApFilePageForm::btnAPQueryClick(TObject *Sender)
{
        if(edtAPC->Text.IsEmpty())	return;

	int nRight=GetOperatorRight(operatorcode.c_str(),"IDM_ASKPURCHASE");
	//�빺������ӿڣ��ļ���AskPurchase.*
	AskPurchase(edtAPC->Text.c_str(),nRight);
}
//---------------------------------------------------------------------------




