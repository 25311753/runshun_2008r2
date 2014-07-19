//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "DicZhengMian.h"
#include "DataModule.h"
#include "BaseCode.h"
#include "LdyInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDicZhengMianForm *DicZhengMianForm;
void DicZhengMian(int nAuth)
{
  TDicZhengMianForm *pForm;
	try
	{
  	        pForm=new TDicZhengMianForm(Application);
        }
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
                return;
	}
        pForm->ShowModal();
        delete pForm;
}
__fastcall TDicZhengMianForm::TDicZhengMianForm(TComponent* Owner)
        : TForm(Owner)
{
  m_enWorkState=EN_IDLE;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TDicZhengMianForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==VK_RETURN)
    KeyReturnHandle(ActiveControl);        
}
//---------------------------------------------------------------------------
void __fastcall TDicZhengMianForm::FormShow(TObject *Sender)
{
  edtName->Text="";
  RefreshData();
  m_enWorkState=EN_IDLE;
  ResetCtrl();        
}
//---------------------------------------------------------------------------
void __fastcall TDicZhengMianForm::ListView1Click(TObject *Sender)
{
        Row2Editor();
	ResetCtrl();        
}
//---------------------------------------------------------------------------
void __fastcall TDicZhengMianForm::btnOK0Click(TObject *Sender)
{
  char strName[80],*ptr,strTemp[80];
  int nState;

  edtName->Text=edtName->Text.Trim();

  if(edtName->Text.IsEmpty())
  {
  	ShowMessage("����������");
    if(edtName->CanFocus())	edtName->SetFocus();
    return;
  }

  strcpy(strName,edtName->Text.c_str());

  char strSQL[1024];

  switch(m_enWorkState)
  {
    case EN_ADDNEW:
      sprintf(strSQL,"insert into DicZhengMian values('%s')",strName);
      break;
    case EN_EDIT:
      {
        TListItem *pItem = ListView1->Selected;
              sprintf(strSQL,"update DicZhengMian set zmname='%s' where zmname='%s'",strName,pItem->Caption.c_str());
      }
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
      sprintf(strAddSQL,"select * from DicZhengMian where zmname='%s'",strName);
      RunSQL(strAddSQL,true);
      if(dm1->Query1->RecordCount>0)
      {
        ShowMessage("���ݿ������иñ�ŵļ�¼!");
        edtName->SetFocus();
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
    pItem->Caption=edtName->Text;
    ListView1->Selected=pItem;
  }
  else if(m_enWorkState==EN_EDIT)
  {
  	pItem=ListView1->Selected;
    if(pItem!=NULL)
    {
    	pItem->Caption=edtName->Text;
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
void __fastcall TDicZhengMianForm::btnCancel0Click(TObject *Sender)
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
void __fastcall TDicZhengMianForm::btnAddNewClick(TObject *Sender)
{
  edtName->Text="";
  m_enWorkState=EN_ADDNEW;
  ResetCtrl();
  msgState->Caption="����״̬������";
  edtName->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TDicZhengMianForm::btnEditClick(TObject *Sender)
{
  m_enWorkState=EN_EDIT;
  ResetCtrl();
  msgState->Caption="����״̬���޸�";
  edtName->SetFocus();         
}
//---------------------------------------------------------------------------
void __fastcall TDicZhengMianForm::btnDeleteClick(TObject *Sender)
{
  char strMsg[256],strSQL[512];
  sprintf(strMsg,"\n  ��Ҫɾ����%s���ļ�¼��  \n",edtName->Text.c_str());
  if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
   return;

  sprintf(strSQL,"delete from DicZhengMian where zmname='%s'",edtName->Text.c_str());
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
void __fastcall TDicZhengMianForm::btnRefreshClick(TObject *Sender)
{
  RefreshData();
  ResetCtrl(); 
}
//---------------------------------------------------------------------------
void __fastcall TDicZhengMianForm::btnExitClick(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------
void TDicZhengMianForm::Row2Editor()
{
  TListItem *pItem;

  pItem=ListView1->Selected;
  if(pItem==NULL)	return;
  edtName->Text=pItem->Caption;
}

void TDicZhengMianForm::RefreshData()
{
  char strBuff[512];
  TListItem *pItem;

  if(!dm1->OpenDatabase())  return;
  try
  {
    dm1->Query1->SQL->Clear();
    dm1->Query1->SQL->Add("select * from DicZhengMian");

    dm1->Query1->Prepare();
    dm1->Query1->Open();
    dm1->Query1->First();
    ListView1->Items->Clear();
    while(!dm1->Query1->Eof)
    {
    	pItem=ListView1->Items->Add();
      pItem->Caption=dm1->Query1->FieldByName("zmname")->AsString;
      dm1->Query1->Next();
    }
  }
  catch(...)
  {
    ShowMessage("���ݿ��������");
    return;
  }
}

void TDicZhengMianForm::ResetCtrl()
{
  //bool bAuth = m_nAuthor>=AUTH_FULL;
  bool bAuth = true;
  if(m_enWorkState==EN_IDLE)
  {
    EnableEdit(edtName,false);
    btnOK0->Enabled=false;
    btnCancel0->Enabled=false;

    btnAddNew->Enabled=true && bAuth;
    btnRefresh->Enabled=true;
    btnExit->Enabled=true;
    ListView1->Enabled=true;
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
    ListView1->Enabled=false;
    btnEdit->Enabled=false;
    btnDelete->Enabled=false;

    EnableEdit(edtName,true);
    btnOK0->Enabled=true;
    btnCancel0->Enabled=true;
    if(m_enWorkState==EN_ADDNEW)
	    EnableEdit(edtName,true);
    else //�޸�ʱ
	    EnableEdit(edtName,true);
  }
}