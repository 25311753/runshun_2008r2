//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "InputCorpUnit.h"
#include "DataModule.h"
#include "CommUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseFormUnit"
#pragma resource "*.dfm"
TInputCorpForm *InputCorpForm;
//---------------------------------------------------------------------------

__fastcall TInputCorpForm::TInputCorpForm(TComponent* Owner)
   : TBaseForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TInputCorpForm::FormShow(TObject *Sender)
{
   SetDefault();
   ListView1->Items->Clear();
   Screen->Cursor= crHourGlass;
   if (!prtform)
   {
      try
      {
         prtform= new TTypePrt2Form(Application);
      }
      catch(...)
      {
         prtform=NULL;
      }
   }
   Screen->Cursor=crDefault;
   btnref->Click();
}
//---------------------------------------------------------------------------

void __fastcall TInputCorpForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    if (dm1->db1->Connected)
       dm1->db1->Connected=false;
    if (prtform)
    {
       delete prtform;
       prtform=NULL;
    }
}
//---------------------------------------------------------------------------

void __fastcall TInputCorpForm::btnokClick(TObject *Sender)
{
   if (!dm1->ConnectDB())
      return;
   switch(btnidx)
   {
      case 0://do add
           if (!CheckEdit())
              break;
           strsql =AnsiString("select cp_code from diccorp where cp_code='")
                  +edtcode->Text+"'";
           if (DoQuery(dm1->Query1,strsql))
           {
              if (!dm1->Query1->IsEmpty())
              {
                 ErrorDlg("�������ظ��Ĵ���!");
                 break;
              }
           }
           else
           {
              SetButton(1,1,1,1,0,0,1,1);
              break;
           }
           strsql =AnsiString("insert into diccorp values('")+edtcode->Text+"','"
                  +edtname->Text+"')";
           if (DoQuery(dm1->Query1,strsql,1))
           {
              btnref->Click();
              ClearEdit();
              edtcode->SetFocus();
           }
           break;
      case 1:
           LocateRow();
           break;
      case 2://do modify
           if (ListView1->SelCount==0)
           {
              InfoDlg("�������б���ѡ��Ҫ�޸ĵ����!");
              break;
           }
           if (!CheckEdit())
              break;
           strsql =AnsiString("update diccorp set cp_name='")
                  +edtname->Text+"' where cp_code='"
                  +ListView1->Selected->SubItems->Strings[0]+"'";
           if (DoQuery(dm1->Query1,strsql,1))
           {
              btnref->Click();
              ClearEdit();
              InfoDlg("�޸����!");
           }
           else
              SetButton(1,1,1,1,0,0,1,1);
           break;
      case 3://do delete
           if (ListView1->SelCount==0)
           {
              InfoDlg("�������б���ѡ��Ҫɾ�������!");
              break;
           }
           if (AskDlg("ȷʵҪɾ���������")==IDNO)
              break;
           strsql =AnsiString("delete diccorp where cp_code='")
                  +ListView1->Selected->SubItems->Strings[0]+"'";
           if (DoQuery(dm1->Query1,strsql,1))
           {
              ClearEdit();
              btnref->Click();
              InfoDlg("ɾ�����!");
           }
           else
              SetButton(1,1,1,1,0,0,1,1);
           break;
   }
   dm1->db1->Connected=false;
}
//---------------------------------------------------------------------------

void  TInputCorpForm::LocateRow()
{
   AnsiString  fieldsval;
   char  idx;
   if (edtcode->Text!="")
   {
      fieldsval=edtcode->Text;
      idx=0;
   }
   else if (edtname->Text!="")
   {
      fieldsval=edtname->Text;
      idx=1;
   }
   else
   {
      InfoDlg("�����빫˾����������!");
      edtcode->Enabled=true;
      edtname->Enabled=true;
      edtcode->SetFocus();
      return;
   }
   for (int i=0;i<ListView1->Items->Count;i++)
   {
      if (ListView1->Items->Item[i]->SubItems->Strings[idx]==fieldsval)
      {
         ListView1->Items->Item[i]->Selected=true;
         ListView1->SetFocus();
         ListView1Click(InputCorpForm);
         break;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TInputCorpForm::btnrefClick(TObject *Sender)
{
   if (!dm1->ConnectDB())
      return;
   strsql =AnsiString("select * from diccorp order by cp_code");
   if (DoQuery(dm1->Query1,strsql))
   {
      ListView1->Items->Clear();
      TListItem  *ListItem;
      while (!dm1->Query1->Eof)
      {
         ListItem=ListView1->Items->Add();
         ListItem->SubItems->Add(dm1->Query1->Fields->Fields[0]->AsString);
         ListItem->SubItems->Add(dm1->Query1->Fields->Fields[1]->AsString);
         dm1->Query1->Next();
      }
   }
   dm1->db1->Connected=false;
}
//---------------------------------------------------------------------------

void __fastcall TInputCorpForm::btnprintClick(TObject *Sender)
{
    if (!prtform)
   {
      ErrorDlg("������ӡ,����ӡ�������!");
      return;
   }
   if (!dm1->ConnectDB())
      return;
   strsql =AnsiString("select * from diccorp order by cp_code");
   if (!DoQuery(dm1->Query1,strsql))
      return;
   if (dm1->Query1->IsEmpty())
   {
      InfoDlg("û�п�����ӡ������!");
      return;
   }
   prtform->LabTitle->Caption="��˾�����ϵ�";
   prtform->LabCode->Caption="��˾����";
   prtform->LabName->Caption="��˾����";
   prtform->QRDBText1->DataField="cp_code";
   prtform->QRDBText2->DataField="cp_name";
   prtform->QuickRep1->PreviewModal();
   dm1->db1->Connected=false;
}
//---------------------------------------------------------------------------

void __fastcall TInputCorpForm::ListView1Click(TObject *Sender)
{
    if (ListView1->SelCount>0)
    {
       edtcode->Text=ListView1->Selected->SubItems->Strings[0];
       edtname->Text=ListView1->Selected->SubItems->Strings[1];
    }
}
//---------------------------------------------------------------------------

void __fastcall TInputCorpForm::edtnameKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if (Key==13)
   {
      Key=0;
      if (btnidx==0||btnidx==2)
         btnok->Click();
   }
}
//--------------------------------------------------------------------------- _al
