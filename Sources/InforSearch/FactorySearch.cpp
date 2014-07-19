//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <assert.h>
#pragma hdrstop

#include "DataModule.h"
#include "BaseCode.h"
#include "FactorySearch.h"
#include "TConst.h"
#include "LdyInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFactorySearchForm *FactorySearchForm;

//===============================================================================
//�������ϲ�ѯ�ӿڣ��ļ���FactorySearch.*
void FactorySearch(int nAuth)
{
	CALL_FORM(TFactorySearchForm);
}

//---------------------------------------------------------------------------
__fastcall TFactorySearchForm::TFactorySearchForm(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TFactorySearchForm::btnExitClick(TObject *Sender)
{
	Close();	
}

//---------------------------------------------------------------------------
void __fastcall TFactorySearchForm::btnQueryClick(TObject *Sender)
{
	CString szSQL;
	char strWhereAnd[80],strTemp[256];
	TListItem *pItem;

	lstView->Items->Clear();
	strcpy(strWhereAnd,"where");
	szSQL="select * from Manufactur";
	edtID->Text=edtID->Text.Trim();
	if(!edtID->Text.IsEmpty())
	{
		sprintf(strTemp," %s mf_code like '%s%%'",strWhereAnd,edtID->Text.c_str());
		szSQL+=strTemp;
		strcpy(strWhereAnd,"and");
	}
	edtShort->Text=edtShort->Text.Trim();
	if(!edtShort->Text.IsEmpty())
	{
		sprintf(strTemp," %s mf_shortname like '%%%s%%'",strWhereAnd,edtShort->Text.c_str());
		szSQL+=strTemp;
		strcpy(strWhereAnd,"and");
	}
	edtName->Text=edtName->Text.Trim();
	if(!edtName->Text.IsEmpty())
	{
		sprintf(strTemp," %s mf_name like '%%%s%%'",strWhereAnd,edtName->Text.c_str());
		szSQL+=strTemp;
		strcpy(strWhereAnd,"and");
	}
	szSQL+=" order by mf_code,mf_shortname";
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{
		pItem=lstView->Items->Add();
		assert(pItem!=NULL);
		pItem->Caption=dm1->Query1->FieldByName("mf_code")->AsString;
		pItem->SubItems->Add(dm1->Query1->FieldByName("mf_shortname")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("mf_name")->AsString);

		dm1->Query1->Next();
	}
}

//---------------------------------------------------------------------------
void __fastcall TFactorySearchForm::btnDetailClick(TObject *Sender)
{
	TListItem *pItem;

	if((pItem=lstView->Selected)==NULL)	return;
	int nRight=GetOperatorRight(operatorcode.c_str(),"IDM_FACTORY");
	//�������ϵ��ӿڣ��ļ���Factory.*
	FactoryInput(pItem->Caption.c_str(),nRight);
}
//---------------------------------------------------------------------------
void __fastcall TFactorySearchForm::FormShow(TObject *Sender)
{
	edtID->Text="";
	edtShort->Text="";
	edtName->Text="";
  ResetCtrl();
}
//---------------------------------------------------------------------------

void __fastcall TFactorySearchForm::lstViewClick(TObject *Sender)
{
	ResetCtrl();
}
//---------------------------------------------------------------------------

void TFactorySearchForm::ResetCtrl()
{
	btnDetail->Enabled = lstView->Selected!=NULL;
  btnOK->Enabled = lstView->Selected!=NULL;
}

void __fastcall TFactorySearchForm::lstViewSelectItem(TObject *Sender,TListItem *Item, bool Selected)
{
	if(!Selected)	return;
	ResetCtrl();
}
//---------------------------------------------------------------------------

void __fastcall TFactorySearchForm::ApplicationEvents1Idle(TObject *Sender,bool &Done)
{
	if(ActiveControl==edtID)
		txtTips->Caption="��ʾ���������ŵ�ǰ��λ";
	else if(ActiveControl==edtShort)
		txtTips->Caption="��ʾ���������Ƶ�ǰ��λ";
	else if(ActiveControl==edtName)
		txtTips->Caption="��ʾ��������ȫ�ư������ַ�";
	else txtTips->Caption="";
}
//---------------------------------------------------------------------------

void __fastcall TFactorySearchForm::lstViewDblClick(TObject *Sender)
{
	if(btnOK->Enabled)
		btnOKClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFactorySearchForm::btnOKClick(TObject *Sender)
{
	TListItem *pItem;
  if((pItem=lstView->Selected)!=NULL)
  {
  	m_szID=pItem->Caption.c_str();
  }
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------

