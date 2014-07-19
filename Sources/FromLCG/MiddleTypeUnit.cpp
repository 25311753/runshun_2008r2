//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MiddleTypeUnit.h"
#include "DataModule.h"
#include "CommUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseFormUnit"
#pragma resource "*.dfm"
TMiddleTypeForm *MiddleTypeForm;
//---------------------------------------------------------------------------

__fastcall TMiddleTypeForm::TMiddleTypeForm(TComponent* Owner)
   : TBaseForm(Owner)
{
}
//---------------------------------------------------------------------------

bool  TMiddleTypeForm::CheckCode()
{
   if (edtcode->Text.Length()<2)
   {
      ErrorDlg("������볤�ȱ�������λ!");
      edtcode->SetFocus();
      return false;
   }
   else
   {  //�Ƿ���ڴ������
      strsql =AnsiString("select lt_code from diclargetype where lt_code='")
             +edtcode->Text.SubString(1,1)+"'";
      if (DoQuery(dm1->Query1,strsql))
      {
         if (dm1->Query1->IsEmpty())
         {
            ErrorDlg("��һλ������벻����,����������!");
            edtcode->SetFocus();
            return false;
         }
      }
      else
         return false;
   }
   return true;
}
//---------------------------------------------------------------------------

void __fastcall TMiddleTypeForm::FormShow(TObject *Sender)
{
   SetDefault();
   ListView1->Items->Clear();
   if (rights==0)
      SetButton(0,0,0,0,0,0,1,1);
   Screen->Cursor= crHourGlass;
   if (!prtform)
   {
      try
      {
         prtform= new TTypePrtForm(Application);
      }
      catch(...)
      {
         prtform=NULL;
      }
   }
   Screen->Cursor=crDefault;
   sltype = new TStringList;
   strsql =AnsiString("select * from diclargetype order by lt_code ");
   if (DoQuery(dm1->Query1,strsql))
   {
      if (!dm1->Query1->IsEmpty())
      {
         cbltype->Items->Clear();
         while (!dm1->Query1->Eof)
         {
            sltype->Add(dm1->Query1->FieldByName("lt_code")->AsString);
            cbltype->Items->Add(dm1->Query1->FieldByName("lt_name")->AsString);
            dm1->Query1->Next();
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TMiddleTypeForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    if (dm1->db1->Connected)
       dm1->db1->Connected=false;
    if (prtform)
    {
       delete prtform;
       prtform=NULL;
    }
    if (sltype)
       delete sltype;
}
//---------------------------------------------------------------------------

void __fastcall TMiddleTypeForm::btnokClick(TObject *Sender)
{
   if (!dm1->ConnectDB())
      return;
   switch(btnidx)
   {
      case 0://do add
           if (!CheckEdit())
              break;
           if (!CheckCode())
              break;
           strsql =AnsiString("select mt_code from dicmiddletype where mt_code='")
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
           strsql =AnsiString("insert into dicmiddletype values('")+edtcode->Text+"','"
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
           if (!CheckCode())
              break;
           strsql =AnsiString("update dicmiddletype set mt_name='")
                  +edtname->Text+"' where mt_code='"
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
           strsql =AnsiString("delete dicmiddletype where mt_code='")
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

void  TMiddleTypeForm::LocateRow()
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
      InfoDlg("��������������!");
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
         ListView1Click(MiddleTypeForm);
         break;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TMiddleTypeForm::btnrefClick(TObject *Sender)
{
   if (!dm1->ConnectDB())
      return;
   strsql =AnsiString("select * from dicmiddletype order by mt_code");
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

void __fastcall TMiddleTypeForm::btnprintClick(TObject *Sender)
{
   if (!prtform)
   {
      ErrorDlg("������ӡ,����ӡ�������!");
      return;
   }
   if (!dm1->ConnectDB())
      return;
   strsql =AnsiString("select * from dicmiddletype order by mt_code ");
   if (!DoQuery(dm1->Query1,strsql))
      return;
   if (dm1->Query1->IsEmpty())
   {
      InfoDlg("û�п�����ӡ������!");
      return;
   }
   prtform->LabTitle->Caption="�������ϵ�";
   prtform->LabCode->Caption="�������";
   prtform->LabName->Caption="��������";
   prtform->QRDBText1->DataField="mt_code";
   prtform->QRDBText2->DataField="mt_name";
   prtform->QuickRep1->PreviewModal();
   dm1->db1->Connected=false;
}
//---------------------------------------------------------------------------

void __fastcall TMiddleTypeForm::ListView1Click(TObject *Sender)
{
    if (ListView1->SelCount>0)
    {
       edtcode->Text=ListView1->Selected->SubItems->Strings[0];
       edtname->Text=ListView1->Selected->SubItems->Strings[1];
       edtcodeExit(Sender);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMiddleTypeForm::edtnameKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if (Key==13)
   {
      Key=0;
      if (btnidx==0||btnidx==2)
         btnok->Click();
   }
}
//---------------------------------------------------------------------------

void __fastcall TMiddleTypeForm::cbltypeChange(TObject *Sender)
{
   if (cbltype->ItemIndex>=0)
      edtcode->Text=sltype->Strings[cbltype->ItemIndex];
}
//---------------------------------------------------------------------------

void __fastcall TMiddleTypeForm::edtcodeExit(TObject *Sender)
{
    edtcode->Text=Trim(edtcode->Text);
    if (edtcode->Text=="")
       return;
    if (sltype->IndexOf(edtcode->Text.SubString(1,1))>=0)
       cbltype->ItemIndex=sltype->IndexOf(edtcode->Text.SubString(1,1));
}
//---------------------------------------------------------------------------

void __fastcall TMiddleTypeForm::FormActivate(TObject *Sender)
{
    if (ListView1->Items->Count<1)
       btnref->Click();
}
//---------------------------------------------------------------------------

