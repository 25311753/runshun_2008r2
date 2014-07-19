//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "DataModule.h"
#include "BaseCode.h"
#include "DicDepart.h"
#include "LdyInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDicDepartForm *DicDepartForm;

// ==================================================================
//���ű�����ӿڣ��ļ�DicDepart.*
void DicDepart(int nAuth)
{
	if(nAuth==AUTH_NONE)
  {
  	ShowMessage("��Ǹ����û�����Ȩ��");
    return;
  }

  TDicDepartForm *pForm;
	try
	{
  	pForm=new TDicDepartForm(Application);
  }
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
    return;
	}
  pForm->m_nAuthor=nAuth;
  pForm->ShowModal();
  delete pForm;
}
// ==================================================================


__fastcall TDicDepartForm::TDicDepartForm(TComponent* Owner)
  : TForm(Owner)
{
  m_enWorkState=EN_IDLE;
}
//---------------------------------------------------------------------------

void __fastcall TDicDepartForm::btnRefreshClick(TObject *Sender)
{
  RefreshData();
  ResetCtrl();
}
//---------------------------------------------------------------------------


// --------------------------------------------------------------------------
void TDicDepartForm::Row2Editor()
{
  TListItem *pItem;

  pItem=ListView1->Selected;
  if(pItem==NULL)	return;
  edtID->Text=pItem->Caption;
  edtName->Text=pItem->SubItems->Strings[0];
	edtDepartCorp->Text=pItem->SubItems->Strings[1];
}


//---------------------------------------------------------------------------

void TDicDepartForm::RefreshData()
{
  char strBuff[512];
  TListItem *pItem;

  if(!dm1->OpenDatabase())  return;
  try
  {
    dm1->Query1->SQL->Clear();
    dm1->Query1->SQL->Add("select * from DicDepart,DepartCorp where substring(dd_code,1,2)*=dc_code order by dd_code");

    dm1->Query1->Prepare();
    dm1->Query1->Open();
    dm1->Query1->First();
    ListView1->Items->Clear();
    while(!dm1->Query1->Eof)
    {
    	pItem=ListView1->Items->Add();
      pItem->Caption=dm1->Query1->FieldByName("dd_code")->AsString;
      pItem->SubItems->Add(dm1->Query1->FieldByName("dd_name")->AsString);
			pItem->SubItems->Add(dm1->Query1->FieldByName("dc_name")->AsString);
      dm1->Query1->Next();
    }
  }
  catch(...)
  {
    ShowMessage("���ݿ��������!");
    return;
  }
}

void __fastcall TDicDepartForm::btnAddNewClick(TObject *Sender)
{
  edtID->Text="";
  edtName->Text="";
  m_enWorkState=EN_ADDNEW;
  ResetCtrl();
  msgState->Caption="����״̬������";
  edtID->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TDicDepartForm::btnEditClick(TObject *Sender)
{
  m_enWorkState=EN_EDIT;
  ResetCtrl();
  msgState->Caption="����״̬���޸�";
  edtName->SetFocus();
}
//---------------------------------------------------------------------------


void __fastcall TDicDepartForm::btnCancel0Click(TObject *Sender)
{
  int nState;

  nState=m_enWorkState;
  m_enWorkState=EN_IDLE;
  ResetCtrl();
  msgState->Caption="����״̬����ѯ";
  Row2Editor();
  switch(nState)
  { //����ȷ����Ĺ��λ��
    case EN_ADDNEW: btnAddNew->SetFocus();  break;
    case EN_EDIT:   btnEdit->SetFocus();    break;
    default:  break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDicDepartForm::btnOK0Click(TObject *Sender)
{
  char strID[80],strName[80],*ptr,strTemp[256],strCorp[80];
  char strSQL[1024];
  int nState;

  edtID->Text=edtID->Text.Trim();
  edtName->Text=edtName->Text.Trim();

  if(edtID->Text.IsEmpty())
  {
  	ShowMessage("��������");
    if(edtID->CanFocus())	edtID->SetFocus();
    return;
  }
  if(edtName->Text.IsEmpty())
  {
  	ShowMessage("����������");
    if(edtName->CanFocus())	edtName->SetFocus();
    return;
  }
  strcpy(strID,edtID->Text.c_str());
  strcpy(strName,edtName->Text.c_str());

	strcpy(strCorp,strID);
	strCorp[2]='\0';
	sprintf(strSQL,"select * from DepartCorp where dc_code='%s'",strCorp);
	RunSQL(dm1->Query1,strSQL,true);
	if(dm1->Query1->Eof)
	{
		sprintf(strTemp,"���ǰ��λ��%s��ָ���Ĳ��Ź�˾������",strCorp);
		ShowMessage(strTemp);
		return;
	}

  switch(m_enWorkState)
  {
    case EN_ADDNEW:
      sprintf(strSQL,"insert into DicDepart values('%s','%s')",strID,strName);
      break;
    case EN_EDIT:
      sprintf(strSQL,"update DicDepart set dd_name='%s' where dd_code='%s'",strName,strID);
      break;
    default:
      ShowMessage("Work State not AddNew or Edit");
      return;
  }

  if(!dm1->OpenDatabase())  return;

  try
  {
    if(m_enWorkState==EN_ADDNEW)
    {
      char strAddSQL[256];
      sprintf(strAddSQL,"select * from DicDepart where dd_code='%s'",strID);
      RunSQL(strAddSQL,true);
      if(dm1->Query1->RecordCount>0)
      {
        ShowMessage("���ݿ������иñ�ŵļ�¼!");
        edtID->SetFocus();
        return;
      }
    }
    RunSQL(strSQL);
  }
  catch(...)
  {
    ShowMessage("���ݿ��������!");
    return;
  }

  TListItem *pItem;
  if(m_enWorkState==EN_ADDNEW)
  {
  	pItem=ListView1->Items->Add();
    pItem->Caption=edtID->Text;
    pItem->SubItems->Add(edtName->Text);
		pItem->SubItems->Add(edtDepartCorp->Text);
    ListView1->Selected=pItem;
  }
  else if(m_enWorkState==EN_EDIT)
  {
  	pItem=ListView1->Selected;
    if(pItem!=NULL)
    {
    	pItem->Caption=edtID->Text;
      pItem->SubItems->Strings[0]=edtName->Text;
			pItem->SubItems->Strings[1]=edtDepartCorp->Text;
    }
  }

  nState=m_enWorkState;
  m_enWorkState=EN_IDLE;
  ResetCtrl();
  msgState->Caption="����״̬����ѯ";
  switch(nState)
  { //����ȷ����Ĺ��λ��
    case EN_ADDNEW: btnAddNew->SetFocus();  break;
    case EN_EDIT:   btnEdit->SetFocus();    break;
    default:  break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDicDepartForm::btnDeleteClick(TObject *Sender)
{
  char strMsg[256],strSQL[512];
  sprintf(strMsg,"\n  ��Ҫɾ�����Ϊ��%s���ļ�¼��  \n",edtID->Text.c_str());
  if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
   return;

  sprintf(strSQL,"delete from DicDepart where dd_code='%s'",edtID->Text.c_str());
  if(!dm1->OpenDatabase())  return;

  if(!RunSQL(strSQL))	return;

  TListItem *pItem;
  pItem=ListView1->Selected;
  if(pItem!=NULL)
  {
  	int nSel=ListView1->Items->IndexOf(pItem);
    ListView1->Items->Delete(nSel);
    ListView1->Selected=NULL;
  }
  m_enWorkState=EN_IDLE;
  ResetCtrl();
}
//---------------------------------------------------------------------------

void TDicDepartForm::ResetCtrl()
{
	bool bAuth = m_nAuthor>=AUTH_FULL;
	EnableEdit(edtDepartCorp,false);
  if(m_enWorkState==EN_IDLE)
  {
    EnableEdit(edtID	,false);
    EnableEdit(edtName,false);
    btnOK0->Enabled=false;
    btnCancel0->Enabled=false;

    btnAddNew->Enabled=true && bAuth;
    btnRefresh->Enabled=true;
    btnExit->Enabled=true;
    btnPrint->Enabled=true;
    ListView1->Enabled=true;
//    ctlListBox->Enabled=true;
    if(ListView1->Selected!=NULL)
    {
      btnEdit->Enabled=true && bAuth;
      btnDelete->Enabled=true && bAuth;
    }
    else
    {
      btnEdit->Enabled=false;
      btnDelete->Enabled=false;
    }
  }
  else
  {
    btnAddNew->Enabled=false;
    btnRefresh->Enabled=false;
    btnExit->Enabled=false;
//    ctlListBox->Enabled=false;
    ListView1->Enabled=false;
    btnEdit->Enabled=false;
    btnDelete->Enabled=false;
    btnPrint->Enabled=false;

    EnableEdit(edtName,true);
    btnOK0->Enabled=true;
    btnCancel0->Enabled=true;
    if(m_enWorkState==EN_ADDNEW)
	    EnableEdit(edtID,true);
    else //�޸�ʱ�����޸Ĳ�Ʒ���
	    EnableEdit(edtID,false);
  }
}

void __fastcall TDicDepartForm::btnExitClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------


void __fastcall TDicDepartForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==VK_RETURN)
    KeyReturnHandle(ActiveControl);
}
//---------------------------------------------------------------------------


#include "PrnBaseData.h"
void __fastcall TDicDepartForm::btnPrintClick(TObject *Sender)
{
  if(!dm1->OpenDatabase())  return;
  AnsiString szSQL="select dd_code as prnID,dd_name as prnName,dc_name as prnText2"
		" from DicDepart,DepartCorp where substring(dd_code,1,2)*=dc_code order by dd_code";
  if(!OpenQuery(dm1->sqlPrint,szSQL.c_str()))
  	return;

  try
  {
	  TPrnBaseDataForm *pForm;
  	pForm=new TPrnBaseDataForm(this);

    pForm->txtTitle->Caption="���ű��ֵ��";
    pForm->txtText2->Caption="���Ź�˾";
    pForm->txtText3->Caption="";

    pForm->PrnView->PreviewModal() ;
    delete pForm;
  }
  catch(...)
  {
    ShowMessage("��ӡ�����ش�����Ϣ�������Ƿ�װ��������Ĭ�ϴ�ӡ��");
    return;
  }
}
//---------------------------------------------------------------------------


void __fastcall TDicDepartForm::FormShow(TObject *Sender)
{
	edtID->Text="";
  edtName->Text="";
	edtDepartCorp->Text="";
  RefreshData();
  m_enWorkState=EN_IDLE;
  ResetCtrl();
}
//---------------------------------------------------------------------------

void __fastcall TDicDepartForm::ListView1Click(TObject *Sender)
{
  Row2Editor();
	ResetCtrl();
}
//---------------------------------------------------------------------------


void __fastcall TDicDepartForm::edtIDExit(TObject *Sender)
{
	CString szSQL;
	char strDep[80];

	edtID->Text=edtID->Text.Trim();
	if(edtID->Text.IsEmpty())	return;
	strcpy(strDep,edtID->Text.c_str());
	strDep[2]='\0';
	szSQL.Format("select * from DepartCorp where dc_code='%s'",strDep);
	RunSQL(dm1->Query1,szSQL,true);
	if(!dm1->Query1->Eof)
		edtDepartCorp->Text=dm1->Query1->FieldByName("dc_name")->AsString;
	else
		edtDepartCorp->Text="";
}
//---------------------------------------------------------------------------

