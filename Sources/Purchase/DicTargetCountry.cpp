//---------------------------------------------------------------------------

#include <vcl.h>   
#include <stdio.h>
#pragma hdrstop

#include "DicTargetCountry.h"
#include "DataModule.h"
#include "BaseCode.h"
#include "LdyInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDicTargetCountryForm *DicTargetCountryForm;
//---------------------------------------------------------------------------
void DicTargetCountry(int nAuth)
{
  TDicTargetCountryForm *pForm;
	try
	{
  	        pForm=new TDicTargetCountryForm(Application);
        }
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
                return;
	}
        pForm->ShowModal();
        delete pForm;
}
__fastcall TDicTargetCountryForm::TDicTargetCountryForm(TComponent* Owner)
        : TForm(Owner)
{
  m_enWorkState=EN_IDLE;
}
//---------------------------------------------------------------------------
void TDicTargetCountryForm::Row2Editor()
{
  TListItem *pItem;

  pItem=ListView1->Selected;
  if(pItem==NULL)	return;
  edtName->Text=pItem->Caption;
edtPort->Text=pItem->SubItems->Strings[0];;

}

void TDicTargetCountryForm::RefreshData()
{
  char strBuff[512];
  TListItem *pItem;

  if(!dm1->OpenDatabase())  return;
  try
  {
    dm1->Query1->SQL->Clear();
    dm1->Query1->SQL->Add("select * from DicTargetCountry");

    dm1->Query1->Prepare();
    dm1->Query1->Open();
    dm1->Query1->First();
    ListView1->Items->Clear();
    while(!dm1->Query1->Eof)
    {
    	pItem=ListView1->Items->Add();
      pItem->Caption=dm1->Query1->FieldByName("tcname")->AsString;
      pItem->SubItems->Add(dm1->Query1->FieldByName("tcport")->AsString);
      dm1->Query1->Next();
    }
  }
  catch(...)
  {
    ShowMessage("���ݿ��������");
    return;
  }
}
void TDicTargetCountryForm::ResetCtrl()
{
  //bool bAuth = m_nAuthor>=AUTH_FULL;
  bool bAuth = true;
  if(m_enWorkState==EN_IDLE)
  {
    EnableEdit(edtName,false);
    EnableEdit(edtPort,false);
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
    EnableEdit(edtPort,true);

    btnOK0->Enabled=true;
    btnCancel0->Enabled=true;
    if(m_enWorkState==EN_ADDNEW){
	    EnableEdit(edtName,true);
                EnableEdit(edtPort,true);

    }
    else {//�޸�ʱ
	    EnableEdit(edtName,true);
                EnableEdit(edtPort,true);

    }
  }
}
void __fastcall TDicTargetCountryForm::FormKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if (Key==VK_RETURN)
    KeyReturnHandle(ActiveControl);        
}
//---------------------------------------------------------------------------
void __fastcall TDicTargetCountryForm::FormShow(TObject *Sender)
{
  edtName->Text="";
  edtPort->Text="";
  RefreshData();
  m_enWorkState=EN_IDLE;
  ResetCtrl();        
}
//---------------------------------------------------------------------------
void __fastcall TDicTargetCountryForm::ListView1Click(TObject *Sender)
{
        Row2Editor();
	ResetCtrl();        
}
//---------------------------------------------------------------------------
void __fastcall TDicTargetCountryForm::btnOK0Click(TObject *Sender)
{
  char strName[80],*ptr,strTemp[80],strPort[80];
  int nState;

  edtName->Text=edtName->Text.Trim();
  edtPort->Text=edtPort->Text.Trim();

  if(edtName->Text.IsEmpty())
  {
  	ShowMessage("����������");
    if(edtName->CanFocus())	edtName->SetFocus();
    return;
  }

  strcpy(strName,edtName->Text.c_str());
  strcpy(strPort,edtPort->Text.IsEmpty()?edtName->Text.c_str():edtPort->Text.c_str());

  char strSQL[1024];

  switch(m_enWorkState)
  {
    case EN_ADDNEW:
      sprintf(strSQL,"insert into DicTargetCountry(tcname,tcport) values('%s','%s')",strName,strPort);
      break;
    case EN_EDIT:
      {TListItem *pItem = ListView1->Selected;
      sprintf(strSQL,"update DicTargetCountry set tcname='%s',tcport='%s' where tcname='%s'",strName,strPort,pItem->Caption.c_str());
      }break;
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
      sprintf(strAddSQL,"select * from DicTargetCountry where tcname='%s'",strName);
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
    pItem->SubItems->Add(strPort);
    ListView1->Selected=pItem;
  }
  else if(m_enWorkState==EN_EDIT)
  {
  	pItem=ListView1->Selected;
    if(pItem!=NULL)
    {
    	pItem->Caption=edtName->Text;
        pItem->SubItems->Strings[0]=strPort;
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
void __fastcall TDicTargetCountryForm::btnCancel0Click(TObject *Sender)
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
void __fastcall TDicTargetCountryForm::btnAddNewClick(TObject *Sender)
{
  edtName->Text="";
  edtPort->Text="";
  m_enWorkState=EN_ADDNEW;
  ResetCtrl();
  msgState->Caption="����״̬������";
  edtName->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TDicTargetCountryForm::btnEditClick(TObject *Sender)
{
  m_enWorkState=EN_EDIT;
  ResetCtrl();
  msgState->Caption="����״̬���޸�";
  edtName->SetFocus();          
}
//---------------------------------------------------------------------------
void __fastcall TDicTargetCountryForm::btnDeleteClick(TObject *Sender)
{
  char strMsg[256],strSQL[512];
  sprintf(strMsg,"\n  ��Ҫɾ����%s���ļ�¼��  \n",edtName->Text.c_str());
  if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
   return;

  sprintf(strSQL,"delete from DicTargetCountry where tcname='%s'",edtName->Text.c_str());
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
void __fastcall TDicTargetCountryForm::btnRefreshClick(TObject *Sender)
{
  RefreshData();
  ResetCtrl();            
}
//---------------------------------------------------------------------------
void __fastcall TDicTargetCountryForm::btnExitClick(TObject *Sender)
{
  Close();                
}
//---------------------------------------------------------------------------

