//---------------------------------------------------------------------------

#include <vcl.h>  
#include <stdio.h>
#pragma hdrstop

#include "DicCurrency.h"
#include "DataModule.h"
#include "BaseCode.h"
#include "LdyInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDicCurrencyForm *DicCurrencyForm;
//---------------------------------------------------------------------------
void DicDicCurrency(int nAuth)
{
	if(nAuth==AUTH_NONE)
  {
  	ShowMessage("��Ǹ����û�����Ȩ��");
    return;
  }

  TDicCurrencyForm *pForm;
	try
	{
  	pForm=new TDicCurrencyForm(Application);
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
__fastcall TDicCurrencyForm::TDicCurrencyForm(TComponent* Owner)
        : TForm(Owner)
{
  m_enWorkState=EN_IDLE;
}
//---------------------------------------------------------------------------
void TDicCurrencyForm::Row2Editor()
{
  TListItem *pItem;

  pItem=ListView1->Selected;
  if(pItem==NULL)	return;
  edtID->Text=pItem->Caption;
  edtName->Text=pItem->SubItems->Strings[0];
}

void TDicCurrencyForm::ResetCtrl()
{
	bool bAuth = m_nAuthor>=AUTH_FULL;
  if(m_enWorkState==EN_IDLE)
  {
    EnableEdit(edtID	,false);
    EnableEdit(edtName,false);
    btnOK0->Enabled=false;
    btnCancel0->Enabled=false;

    btnAddNew->Enabled=true && bAuth;
    btnRefresh->Enabled=true;
    btnExit->Enabled=true;
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

    EnableEdit(edtName,true);
    btnOK0->Enabled=true;
    btnCancel0->Enabled=true;
    if(m_enWorkState==EN_ADDNEW)
	    EnableEdit(edtID,true);
    else //�޸�ʱ�����޸Ĳ�Ʒ���
	    EnableEdit(edtID,false);
  }
}
void __fastcall TDicCurrencyForm::ListView1Click(TObject *Sender)
{
  Row2Editor();
	ResetCtrl();        
}
//---------------------------------------------------------------------------
void __fastcall TDicCurrencyForm::btnOK0Click(TObject *Sender)
{
  char strID[80],strName[80],*ptr,strTemp[80];
  int nState;

  edtID->Text=edtID->Text.Trim();
  edtName->Text=edtName->Text.Trim();

  if(edtID->Text.IsEmpty())
  {
  	ShowMessage("��������д");
    if(edtID->CanFocus())	edtID->SetFocus();
    return;
  }
  if(edtName->Text.IsEmpty())
  {
  	ShowMessage("���������");
    if(edtName->CanFocus())	edtName->SetFocus();
    return;
  }

  strcpy(strID,edtID->Text.c_str());
  strcpy(strName,edtName->Text.c_str());

  char strSQL[1024];

  switch(m_enWorkState)
  {
    case EN_ADDNEW:
      sprintf(strSQL,"insert into DicCurrency values('%s','%s')",strID,strName);
      break;
    case EN_EDIT:
      sprintf(strSQL,"update DicCurrency set crname='%s' where crid='%s'",strName,strID);
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
      sprintf(strAddSQL,"select * from DicCurrency where crid='%s'",strID);
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
    ListView1->Selected=pItem;
  }
  else if(m_enWorkState==EN_EDIT)
  {
  	pItem=ListView1->Selected;
    if(pItem!=NULL)
    {
    	pItem->Caption=edtID->Text;
      pItem->SubItems->Strings[0]=edtName->Text;
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
void __fastcall TDicCurrencyForm::btnCancel0Click(TObject *Sender)
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
void __fastcall TDicCurrencyForm::btnAddNewClick(TObject *Sender)
{
  edtID->Text="";
  edtName->Text="";
  m_enWorkState=EN_ADDNEW;
  ResetCtrl();
  msgState->Caption="����״̬������";
  edtID->SetFocus();        
}
//---------------------------------------------------------------------------
void __fastcall TDicCurrencyForm::btnEditClick(TObject *Sender)
{
  m_enWorkState=EN_EDIT;
  ResetCtrl();
  msgState->Caption="����״̬���޸�";
  edtName->SetFocus();        
}
//---------------------------------------------------------------------------
void __fastcall TDicCurrencyForm::btnDeleteClick(TObject *Sender)
{
 char strMsg[256],strSQL[512];
  sprintf(strMsg,"\n  ��Ҫɾ�����Ϊ��%s���ļ�¼��  \n",edtID->Text.c_str());
  if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
   return;

  sprintf(strSQL,"delete from DicCurrency where crid='%s'",edtID->Text.c_str());
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
void __fastcall TDicCurrencyForm::btnRefreshClick(TObject *Sender)
{
  RefreshData();
  ResetCtrl();
}
//---------------------------------------------------------------------------
void __fastcall TDicCurrencyForm::btnExitClick(TObject *Sender)
{
Close();
}

void TDicCurrencyForm::RefreshData()
{
  char strBuff[512];
  TListItem *pItem;

  if(!dm1->OpenDatabase())  return;
  try
  {
    dm1->Query1->SQL->Clear();
    dm1->Query1->SQL->Add("select * from DicCurrency");

    dm1->Query1->Prepare();
    dm1->Query1->Open();
    dm1->Query1->First();
    ListView1->Items->Clear();
    while(!dm1->Query1->Eof)
    {
    	pItem=ListView1->Items->Add();
      pItem->Caption=dm1->Query1->FieldByName("crid")->AsString;
      pItem->SubItems->Add(dm1->Query1->FieldByName("crname")->AsString);
      dm1->Query1->Next();
    }
  }
  catch(...)
  {
    ShowMessage("���ݿ��������");
    return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDicCurrencyForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==VK_RETURN)
    KeyReturnHandle(ActiveControl);
}
//---------------------------------------------------------------------------

void __fastcall TDicCurrencyForm::FormShow(TObject *Sender)
{
  edtName->Text="";
  edtID->Text = "";
  RefreshData();
  m_enWorkState=EN_IDLE;
  ResetCtrl();
}
//---------------------------------------------------------------------------

