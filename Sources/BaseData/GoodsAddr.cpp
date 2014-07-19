//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "DataModule.h"
#include "BaseCode.h"
#include "GoodsAddr.h"
#include "LdyInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGoodsAddrForm *GoodsAddrForm;
// ==================================================================
//�����ص�����ӿڣ��ļ�GoodsAddr.*
void GoodsAddr(int nAuth)
{
	if(nAuth==AUTH_NONE)
  {
  	ShowMessage("��Ǹ����û�����Ȩ��");
  	return;
  }

  TGoodsAddrForm *pForm;
	try
	{
  	pForm=new TGoodsAddrForm(Application);
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


__fastcall TGoodsAddrForm::TGoodsAddrForm(TComponent* Owner)
  : TForm(Owner)
{
  m_enWorkState=EN_IDLE;
}
//---------------------------------------------------------------------------

void __fastcall TGoodsAddrForm::btnRefreshClick(TObject *Sender)
{
  RefreshData();
  ResetCtrl();
}
//---------------------------------------------------------------------------


// --------------------------------------------------------------------------
void TGoodsAddrForm::Row2Editor()
{
  TListItem *pItem;

  pItem=ListView1->Selected;
  if(pItem==NULL)	return;
  edtID->Text=pItem->Caption;
  edtName->Text=pItem->SubItems->Strings[0];
  edtType->Text=pItem->SubItems->Strings[1];
}


//---------------------------------------------------------------------------

void TGoodsAddrForm::RefreshData()
{
  char strBuff[512];
  TListItem *pItem;

  if(!dm1->OpenDatabase())  return;
  try
  {
    dm1->Query1->SQL->Clear();
    dm1->Query1->SQL->Add("select * from GoodsAddr");

    dm1->Query1->Prepare();
    dm1->Query1->Open();
    dm1->Query1->First();
    ListView1->Items->Clear();
    while(!dm1->Query1->Eof)
    {
    	pItem=ListView1->Items->Add();
      pItem->Caption=dm1->Query1->FieldByName("ga_code")->AsString;
      pItem->SubItems->Add(dm1->Query1->FieldByName("ga_area")->AsString);
      pItem->SubItems->Add(dm1->Query1->FieldByName("ga_name")->AsString);
      dm1->Query1->Next();
    }
  }
  catch(...)
  {
    ShowMessage("���ݿ��������!");
    return;
  }
}

void __fastcall TGoodsAddrForm::btnAddNewClick(TObject *Sender)
{
  edtID->Text="";
  edtName->Text="";
  edtType->Text="";
  m_enWorkState=EN_ADDNEW;
  ResetCtrl();
  msgState->Caption="����״̬������";
  edtID->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TGoodsAddrForm::btnEditClick(TObject *Sender)
{
  m_enWorkState=EN_EDIT;
  ResetCtrl();
  msgState->Caption="����״̬���޸�";
  edtName->SetFocus();
}
//---------------------------------------------------------------------------


void __fastcall TGoodsAddrForm::btnCancel0Click(TObject *Sender)
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

void __fastcall TGoodsAddrForm::btnOK0Click(TObject *Sender)
{
  char strSQL[1024];
  char strID[80],strName[80],strType[80];
  int nState;

  edtID->Text=edtID->Text.Trim();
  edtName->Text=edtName->Text.Trim();
  edtType->Text=edtType->Text.Trim();

  if(edtID->Text.IsEmpty())
  {
  	ShowMessage("��������");
    if(edtID->CanFocus())	edtID->SetFocus();
    return;
  }
  if(edtName->Text.IsEmpty())
  {
  	ShowMessage("�����뽻����˾������");
    if(edtName->CanFocus())	edtName->SetFocus();
    return;
  }
  if(edtType->Text.IsEmpty())
  {
  	ShowMessage("�����뽻���ص�");
    if(edtType->CanFocus())	edtType->SetFocus();
    return;
  }

  strcpy(strID,edtID->Text.c_str());
  strcpy(strName,edtName->Text.c_str());
  strcpy(strType,edtType->Text.c_str());

  if(!dm1->OpenDatabase())  return;

  switch(m_enWorkState)
  {
    case EN_ADDNEW:
      sprintf(strSQL,"insert into GoodsAddr values('%s','%s','%s')",strID,strName,strType);
      break;
    case EN_EDIT:
      sprintf(strSQL,"update GoodsAddr set ga_area='%s',ga_name='%s' where ga_code='%s'",strName,strType,strID);
      break;
    default:
      ShowMessage("Work State not AddNew or Edit");
      return;
  }

  try
  {
    if(m_enWorkState==EN_ADDNEW)
    {
      char strAddSQL[256];
      sprintf(strAddSQL,"select * from GoodsAddr where ga_code='%s'",strID);
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
    pItem->SubItems->Add(edtType->Text);
    ListView1->Selected=pItem;
  }
  else if(m_enWorkState==EN_EDIT)
  {
  	pItem=ListView1->Selected;
    if(pItem!=NULL)
    {
    	pItem->Caption=edtID->Text;
      pItem->SubItems->Strings[0]=edtName->Text;
      pItem->SubItems->Strings[1]=edtType->Text;
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

void __fastcall TGoodsAddrForm::btnDeleteClick(TObject *Sender)
{
  char strMsg[256],strSQL[512];
  sprintf(strMsg,"\n  ��Ҫɾ�����Ϊ��%s���ļ�¼��  \n",edtID->Text.c_str());
  if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL)!=IDYES)
   return;

  sprintf(strSQL,"delete from GoodsAddr where ga_code='%s'",edtID->Text.c_str());
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
//  if(nSel>=ctlListBox->Items->Count)
//    nSel=ctlListBox->Items->Count-1;
//  ctlListBox->ItemIndex=nSel;
//  Row2Editor();
  m_enWorkState=EN_IDLE;
  ResetCtrl();
}
//---------------------------------------------------------------------------

void TGoodsAddrForm::ResetCtrl()
{
	bool bAuth = m_nAuthor>=AUTH_FULL;
  if(m_enWorkState==EN_IDLE)
  {
    EnableEdit(edtID	,false);
    EnableEdit(edtName,false);
    EnableEdit(edtType,false);
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
    EnableEdit(edtType,true);
    btnOK0->Enabled=true;
    btnCancel0->Enabled=true;
    if(m_enWorkState==EN_ADDNEW)
	    EnableEdit(edtID,true);
    else //�޸�ʱ�����޸Ĳ�Ʒ���
	    EnableEdit(edtID,false);
  }
}

void __fastcall TGoodsAddrForm::btnExitClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------


void __fastcall TGoodsAddrForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==VK_RETURN)
    KeyReturnHandle(ActiveControl);
}
//---------------------------------------------------------------------------


#include "PrnBaseData.h"
void __fastcall TGoodsAddrForm::btnPrintClick(TObject *Sender)
{
  if(!dm1->OpenDatabase())  return;
  AnsiString szSQL="select ga_code as prnID,ga_area as prnName,ga_name as prnText2 from GoodsAddr order by ga_code";
  if(!OpenQuery(dm1->sqlPrint,szSQL.c_str()))
  	return;

  try
  {
	  TPrnBaseDataForm *pForm;
  	pForm=new TPrnBaseDataForm(this);

    pForm->txtTitle->Caption="�����ص��ֵ��";
		pForm->txtText1->Caption="������˾������";
    pForm->txtText2->Caption="�����ص�";
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


void __fastcall TGoodsAddrForm::FormShow(TObject *Sender)
{
	edtID->Text="";
  edtName->Text="";
  edtType->Text="";
  RefreshData();
  m_enWorkState=EN_IDLE;
  ResetCtrl();
}
//---------------------------------------------------------------------------



void __fastcall TGoodsAddrForm::ListView1Click(TObject *Sender)
{
  Row2Editor();
	ResetCtrl();
}
//---------------------------------------------------------------------------

