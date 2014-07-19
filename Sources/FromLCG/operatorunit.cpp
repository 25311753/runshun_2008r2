//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <string.h>
#pragma hdrstop
#include "operatorunit.h"
#include "DataModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Toperatorform *operatorform;
//---------------------------------------------------------------------------
__fastcall Toperatorform::Toperatorform(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void  Toperatorform::SetDBedit(bool btnvalue)
{
   dbedoptcode->Enabled=btnvalue;
   dbedoptname->Enabled=btnvalue;
   dbedpassword->Enabled=btnvalue;
   dbecorp->Enabled=btnvalue;
   confirmpass->Enabled=btnvalue;
   cmbclass->Enabled=btnvalue;
}
//---------------------------------------------------------------------------

void  Toperatorform::SetButton(bool btnstate)
{
   btnclose->Enabled=btnstate;
   btnadd->Enabled=btnstate;
   btndelete->Enabled=btnstate;
   btnedit->Enabled=btnstate;
   btnprior->Enabled=btnstate;
   btnnext->Enabled=btnstate;
   btnlast->Enabled=btnstate;
   btnfirst->Enabled=btnstate;
   btnquery->Enabled=btnstate;
   btnsave->Enabled=!btnstate;
   btncancel->Enabled=!btnstate;
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::FormShow(TObject *Sender)
{
   if (dm1->ConnectDB())
   {
      sclass = new TStringList;
      sclass->Clear();
      try
      {
         dm1->Query1->Close();
         dm1->Query1->SQL->Clear();
         dm1->Query1->SQL->Add("select * from operclass");
         dm1->Query1->Prepare();
         dm1->Query1->Open();
         dm1->Query1->First();
         cmbclass->Items->Clear();
         while(!dm1->Query1->Eof)
         {
             sclass->Add(dm1->Query1->Fields->Fields[0]->AsString);
             cmbclass->Items->Add(dm1->Query1->Fields->Fields[1]->AsString);
             dm1->Query1->Next();
         }
         dm1->Query1->Close();
         dm1->OperTab->Open();
         btnfirstClick(Sender);
      }
      catch(...)
      {
         Application->MessageBox("�򿪱�ʧ��,��������!","����",MB_OK+MB_ICONERROR);
         SetButton(false);
         btnsave->Enabled=false;
         btncancel->Enabled=false;
         btnclose->Enabled=true;
         SetDBedit(false);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::btnaddClick(TObject *Sender)
{
   SetButton(false);
   SetDBedit(true);
   dbedpassword->Text="";
   confirmpass->Text="";
   dm1->DSOper->Enabled = true;
   dm1->OperTab->Append();
   dbedoptcode->Enabled=true;
   dbedoptcode->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::btneditClick(TObject *Sender)
{
   SetButton(false);
   SetDBedit(true);
   dm1->DSOper->Enabled = true;
   dm1->OperTab->Edit();
   dbedoptcode->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::btndeleteClick(TObject *Sender)
{
    if (dbedoptcode->Text=="root")
    {
       Application->MessageBox("����ɾ�������ܹ���Ա��¼!","����",MB_OK+MB_ICONERROR);
       return;
    }
    if (dm1->OperTab->RecordCount >0)
    {
       if (Application->MessageBox("�����ɾ��������¼��?","ϵͳ��Ϣ",MB_YESNO) == IDYES)
       {
          dm1->OperTab->Delete();
          if (dm1->OperTab->RecordCount <=0) btndelete->Enabled = false;
       }
    }
}
//---------------------------------------------------------------------------

AnsiString  Toperatorform::DecodePwd()
{
   AnsiString  password="";
   char *pbuf, buff[7];
   pbuf=&buff[0];
   strcpy(pbuf,(dbepwd->Text).c_str());
   for (int i=0; i<(dbepwd->Text).Length();i++)     //�������
      password=password+Char(buff[i]^73+9);
   return password;
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::btnsaveClick(TObject *Sender)
{
   dbedoptcode->Text=Trim(dbedoptcode->Text);
   dbedoptname->Text=Trim(dbedoptname->Text);
   if (dbedoptcode->Text=="")
   {
      Application->MessageBox("����Ա���Ų���Ϊ��!","ϵͳ��Ϣ",MB_OK);
      dbedoptcode->SetFocus();
      return;
   }
   if (dbedoptname->Text=="")
   {
      Application->MessageBox("����Ա��������Ϊ��!","ϵͳ��Ϣ",MB_OK);
      dbedoptname->SetFocus();
      return;
   }
   if (dbedpassword->Text=="")
   {
      Application->MessageBox("���벻��Ϊ��!","ϵͳ��Ϣ",MB_OK);
      dbedpassword->SetFocus();
      return;
   }
   if (dbedpassword->Text!= confirmpass->Text)
   {
      Application->MessageBox ("����������벻һ�£������䣡","ϵͳ��Ϣ",MB_OK);
      confirmpass->SetFocus();
      return;
   }
   else
   {
      AnsiString  password="";
      char *pbuf, buff[7];
      pbuf=&buff[0];
      strcpy(pbuf,(confirmpass->Text).c_str());
      for (int i=0; i<(confirmpass->Text).Length();i++)     //�������
        password=password+Char(buff[i]^73+9);
      dm1->OperTab->FieldByName("op_password")->AsString=password ;
   }
   if (cmbclass->Text=="")
   {
      Application->MessageBox("������Ϊ��!","ϵͳ��Ϣ",MB_OK);
      cmbclass->SetFocus();
      return;
   }
   else
      dm1->OperTab->FieldByName("op_class")->AsInteger=StrToInt(sclass->Strings[cmbclass->ItemIndex]);

   if (dm1->OperTab->State == dsInsert)
   {
      dm1->Query1->Close();
      dm1->Query1->SQL->Clear();
      dm1->Query1->SQL->Add(" select * from operator where op_code = :code");
      dm1->Query1->ParamByName("code")->AsString = Trim(dbedoptcode->Text);
      dm1->Query1->Prepare();
      dm1->Query1->Open();
      if (! dm1->Query1->IsEmpty())
      {
         Application->MessageBox("�ò���Ա�����Ѵ���,����������һ��!", "ϵͳ��Ϣ", MB_OK);
         dm1->Query1->Close();
         return;
      }
   }
   if (dm1->OperTab->State == dsEdit)
   {
      if ((dbedoptcode->Text=="root")&&(cmbclass->Text!="�ܹ���Ա"))
      {
         Application->MessageBox("�����޸�root�Ĺ���Ա�ȼ�!", "����", MB_OK+MB_ICONERROR);
         cmbclass->SetFocus();
         return;
      }
   }

   //Save record
   dm1->OperTab->DisableControls();
   try
   {
      dm1->OperTab->Post();
      dm1->OperTab->Refresh();
      //dbepwd->Text=dm1->OperTab->FieldByName("op_password")->AsString;
      //dbedpassword->Text=DecodePwd();
      //confirmpass->Text=dbedpassword->Text;
   }
   catch(...)
   {
      Application->MessageBox("����ʧ��!","����",MB_OK+MB_ICONERROR);
   }
   dm1->OperTab->EnableControls();
   SetButton(true);
   SetDBedit(false);
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::btncancelClick(TObject *Sender)
{
   dm1->OperTab->DisableControls();
   dm1->OperTab->Cancel();
   dm1->OperTab->Refresh();
   cmbclass->ItemIndex=sclass->IndexOf(dm1->OperTab->FieldByName("op_class")->AsInteger);
   dbepwd->Text=dm1->OperTab->FieldByName("op_password")->AsString;
   dbedpassword->Text=DecodePwd();
   confirmpass->Text=dbedpassword->Text;
   dm1->OperTab->EnableControls();
   SetButton(true);
   SetDBedit(false);
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::btnfirstClick(TObject *Sender)
{
    dm1->OperTab->DisableControls();
    dm1->OperTab->First();
    cmbclass->ItemIndex=sclass->IndexOf(dm1->OperTab->FieldByName("op_class")->AsInteger);
    rank = dm1->OperTab->FieldByName("op_class")->AsInteger;
    dm1->OperTab->EnableControls();
    dbedpassword->Text=DecodePwd();
    confirmpass->Text=dbedpassword->Text;
    if (dm1->OperTab->Bof)
    {
       btnfirst->Enabled = false;
       btnprior->Enabled = false;
    }
    btnnext->Enabled = true;
    btnlast->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::btnpriorClick(TObject *Sender)
{
    dm1->OperTab->DisableControls();
    dm1->OperTab->Prior();
    cmbclass->ItemIndex=sclass->IndexOf(dm1->OperTab->FieldByName("op_class")->AsInteger);
    rank = dm1->OperTab->FieldByName("op_class")->AsInteger;
    dm1->OperTab->EnableControls();
    dbedpassword->Text=DecodePwd();
    confirmpass->Text=dbedpassword->Text;
    if (dm1->OperTab->Bof)
    {
       btnfirst->Enabled = false;
       btnprior->Enabled = false;
    }
    btnnext->Enabled = true;
    btnlast->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::btnnextClick(TObject *Sender)
{
   dm1->OperTab->DisableControls();
   dm1->OperTab->Next();
   cmbclass->ItemIndex=sclass->IndexOf(dm1->OperTab->FieldByName("op_class")->AsInteger);
   rank = dm1->OperTab->FieldByName("op_class")->AsInteger;
   dm1->OperTab->EnableControls();
   dbedpassword->Text=DecodePwd();
   confirmpass->Text=dbedpassword->Text;
   if (dm1->OperTab->Eof)
   {
      btnnext->Enabled = false;
      btnlast->Enabled = false;
   }
   btnfirst->Enabled = true;
   btnprior->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::btnlastClick(TObject *Sender)
{
    dm1->OperTab->DisableControls();
    dm1->OperTab->Last();
    cmbclass->ItemIndex=sclass->IndexOf(dm1->OperTab->FieldByName("op_class")->AsInteger);
    rank = dm1->OperTab->FieldByName("op_class")->AsInteger;
    dm1->OperTab->EnableControls();
    dbedpassword->Text=DecodePwd();
    confirmpass->Text=dbedpassword->Text;
    if (dm1->OperTab->Eof)
    {
       btnnext->Enabled = false;
       btnlast->Enabled = false;
    }
    btnfirst->Enabled = true;
    btnprior->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::btnqueryClick(TObject *Sender)
{
    AnsiString  strcode;
    TLocateOptions Opts;

    Opts.Clear();
    Opts <<loCaseInsensitive;
    strcode= "123456";
    if (!InputQuery("���������Ա����:","����Ա����  ",strcode))
       return;
    dm1->OperTab->First();
    dm1->OperTab->DisableControls();
    if (!dm1->OperTab->Locate("op_code",strcode,Opts))
       Application->MessageBox("û�д˲���Ա!","ϵͳ��Ϣ",MB_OK);
    cmbclass->ItemIndex=sclass->IndexOf(dm1->OperTab->FieldByName("op_class")->AsInteger);
    rank = dm1->OperTab->FieldByName("op_class")->AsInteger;
    dm1->OperTab->EnableControls();
    dbedpassword->Text=DecodePwd();
    confirmpass->Text=dbedpassword->Text;
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   dm1->db1->Connected=false;
   if (sclass)
      delete sclass;
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::dbedoptnameKeyPress(TObject *Sender,
      char &Key)
{
   if (Key==13)
   {
      Key=0;
      SelectNext(ActiveControl,true,true);
   }
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::dbedoptcodeKeyPress(TObject *Sender,
      char &Key)
{
   if (Key==13)
   {
      Key=0;
      SelectNext(ActiveControl,true,true);
   }
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::dbedpasswordKeyPress(TObject *Sender,
      char &Key)
{
   if (Key==13)
   {
      Key=0;
      SelectNext(ActiveControl,true,true);
   }
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::confirmpassKeyPress(TObject *Sender,
      char &Key)
{
   if (Key==13)
   {
      Key=0;
      SelectNext(ActiveControl,true,true);
   }
}
//---------------------------------------------------------------------------

void __fastcall Toperatorform::cmbclassKeyPress(TObject *Sender, char &Key)
{
   if (Key==13)
   {
      Key=0;
      SelectNext(ActiveControl,true,true);
   }
}
//---------------------------------------------------------------------------




