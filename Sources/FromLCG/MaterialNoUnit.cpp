//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MaterialNoUnit.h"
#include "DataModule.h"
#include "CommUnit.h"
#include "TConst.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMaterialNoForm *MaterialNoForm;
//---------------------------------------------------------------------------

__fastcall TMaterialNoForm::TMaterialNoForm(TComponent* Owner)
   : TForm(Owner)
{

}
//---------------------------------------------------------------------------

bool  TMaterialNoForm::CheckEdit()
{
   edtcode->Text=Trim(edtcode->Text);
   if (edtcode->Text=="")
   {
       InfoDlg("��������15λ����!");
       edtcode->SetFocus();
       return false;
   }
   
   edtname->Text=Trim(edtname->Text);
   if (edtname->Text=="")
   {
       InfoDlg("���������ƹ��!");
       edtname->SetFocus();
       return false;
   }
   edtusage->Text=Trim(edtusage->Text);
   return true;
}
//---------------------------------------------------------------------------

bool  TMaterialNoForm::CheckCode()
{
   edtscode->Text=Trim(edtscode->Text);
   if (edtscode->Text.Length()<4)
   {
      ErrorDlg("С����볤�ȱ�������λ!");
      edtscode->SetFocus();
      return false;
   }
   else
   {  //�Ƿ����С�����
      strsql =AnsiString("select st_code from dicsmalltype where st_code='")
             +edtscode->Text+"'";
      if (DoQuery(dm1->Query1,strsql))
      {
         if (dm1->Query1->IsEmpty())
         {
            ErrorDlg("ǰ��λС����벻����,����������!");
            edtscode->SetFocus();
            return false;
         }
      }
      else
         return false;
   }
   return true;
}
//---------------------------------------------------------------------------

void  TMaterialNoForm::SetEdits(bool v0,bool v1,bool v2,bool v3,bool v4,bool v5,
      bool v6,bool v7,bool v8,bool v9)
{
    edtlcode->Enabled=v0;
    cbltype->Enabled=v1;
    edtmcode->Enabled=v2;
    cbmtype->Enabled=v3;
    edtstype->Enabled=v4;
    cbstype->Enabled=v5;
    edtscode->Enabled=v6;
    edtcode->Enabled=v7;
    edtname->Enabled=v8;
    edtusage->Enabled=v9;
}
//---------------------------------------------------------------------------

void  TMaterialNoForm::SetButton(bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6)
{
   btnadd->Enabled=v0;
   btnref->Enabled=v1;
   btnmod->Enabled=v2;
   btndel->Enabled=v3;
   btnok->Enabled=v4;
   btncan->Enabled=v5;
   btnprint->Enabled=v6;
}
//---------------------------------------------------------------------------

void  TMaterialNoForm::LocateRow(AnsiString str)
{
   for (int i=0;i<ListView1->Items->Count;i++)
   {
      info=ListView1->Items->Item[i]->SubItems->Strings[0];
      if (info.AnsiPos(str)>0)
      {
         ListView1->Items->Item[i]->Selected=true;
         ListView1->SetFocus();
         ListView1Click(MaterialNoForm);
         break;
      }
   }
}
//---------------------------------------------------------------------------

void  TMaterialNoForm::ClearEdit()
{
    edtscode->Text="";
    edtcode->Text="";
    edtname->Text="";
}
//---------------------------------------------------------------------------

void  TMaterialNoForm::SetDefault()
{
    btnclear->Click();
    ListView1->Items->Clear();
    edtcorp->Enabled=false;
    SetEdits(0,0,0,0,0,0,0,0,0,0);
    btnidx=5;
    SetButton(1,1,0,0,0,0,0);
    PanelHint->Caption="";
}
//---------------------------------------------------------------------------

bool  TMaterialNoForm::GetMiddleType()
{
   strsql =AnsiString("select * from dicmiddletype where substring(mt_code,1,1)='")
          +edtlcode->Text+"' order by mt_code ";
   if (DoQuery(dm1->Query1,strsql))
   {
      if (!dm1->Query1->IsEmpty())
      {
         cbmtype->Items->Clear();
         smtype->Clear();
         while (!dm1->Query1->Eof)
         {
            smtype->Add(dm1->Query1->FieldByName("mt_code")->AsString);
            cbmtype->Items->Add(dm1->Query1->FieldByName("mt_name")->AsString);
            dm1->Query1->Next();
         }
      }
      else
      {
         info = AnsiString("����")+cbltype->Text+"û����������,\n�޷���������,���Ƚ�����������!";
         ErrorDlg(info.c_str());
         return false;
      }
   }
   else
   {
      ErrorDlg("ȡ�������ݴ���,�޷���������,����������!");
      return false;
   }
   return true;
}
//---------------------------------------------------------------------------

bool  TMaterialNoForm::GetSmallType()
{
   strsql =AnsiString("select * from dicsmalltype where substring(st_code,1,2)='")
          +edtmcode->Text+"' order by st_code ";
   if (DoQuery(dm1->Query1,strsql))
   {
      if (!dm1->Query1->IsEmpty())
      {
         cbstype->Items->Clear();
         sstype->Clear();
         while (!dm1->Query1->Eof)
         {
            sstype->Add(dm1->Query1->FieldByName("st_code")->AsString);
            cbstype->Items->Add(dm1->Query1->FieldByName("st_name")->AsString);
            dm1->Query1->Next();
         }
      }
      else
      {
         info = AnsiString("����")+cbmtype->Text+"û��С������,\n�޷���������,���Ƚ���С������!";
         ErrorDlg(info.c_str());
         return false;
      }
   }
   else
   {
      ErrorDlg("ȡ�������ݴ���,�޷���������,����������!");
      return false;
   }
   return true;
}
//---------------------------------------------------------------------------

int   TMaterialNoForm::DoSelect(bool closeds, bool addtolv)
{
   int rn=-1;
   strsql =AnsiString("select * from materialno ");
   if (edtscode->Text!="")
      strsql+=" where mn_code like '"+edtscode->Text+"%'";
   else if (edtmcode->Text!="")
      strsql+=" where mn_code like '"+edtcorp->Text+edtmcode->Text+"%'";
   else if (edtlcode->Text!="")
      strsql+=" where mn_code like '"+edtcorp->Text+edtlcode->Text+"%'";
   else if (edtname->Text!="")
      strsql+=" where mn_name like '%"+edtname->Text+"%'";
   else
   {
      InfoDlg("����������������ࡢ���ࡢС�����������ƹ���е�һ������!");
      return rn;
   }
   strsql =strsql+" order by mn_code";
   if (DoQuery(dm1->Query1,strsql))
   {
      ListView1->Items->Clear();
      if (!dm1->Query1->IsEmpty())
      {
         rn=0;
         if (btnidx==1 && (edtcorp->Text==corpcode))
         {
            btnmod->Enabled=true;
            btndel->Enabled=true;
         }
         btnprint->Enabled=true;
      }
      else
      {
         btnmod->Enabled=false;
         btndel->Enabled=false;
         btnprint->Enabled=false;
         rn=1;
      }
      if (addtolv && rn==0)
      {
         TListItem  *ListItem;
         while (!dm1->Query1->Eof)
         {
            ListItem=ListView1->Items->Add();
            ListItem->SubItems->Add(dm1->Query1->Fields->Fields[0]->AsString);
            ListItem->SubItems->Add(dm1->Query1->Fields->Fields[1]->AsString);
            ListItem->SubItems->Add(dm1->Query1->Fields->Fields[2]->AsString);
            dm1->Query1->Next();
         }
         dm1->Query1->First();
      }
   }
   if (closeds)
      dm1->db1->Connected=false;
   return rn;
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::FormShow(TObject *Sender)
{
   SetButton(0,0,0,0,0,0,0);
   SetEdits(0,0,0,0,0,0,0,0,0,0);
   ListView1->Items->Clear();
   Screen->Cursor= crHourGlass;
   if (!prtform)
   {
      try
      {
         prtform= new TMaterialPrtForm(Application);
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
      else
      {
         ErrorDlg("û�д�������,�޷���������,���Ƚ�����������!");
         return;
      }
   }
   else
   {
      ErrorDlg("ȡ�������ݴ���,�޷���������,����������!");
      return;
   }
   smtype = new TStringList;
   sstype = new TStringList;
   SetDefault();
   edtcorp->Text=corpcode;
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::FormShortCut(TWMKey &Msg, bool &Handled)
{
    switch(Msg.CharCode)
    {
       case VK_F1:
            if (btnadd->Enabled)
               btnadd->Click();
            Handled=true;
            break;
       case VK_F2:
            if (btnref->Enabled)
               btnref->Click();
            Handled=true;
            break;
       case VK_F3:
            if (btnmod->Enabled)
               btnmod->Click();
            Handled=true;
            break;
       case VK_F4:
            if (btndel->Enabled)
               btndel->Click();
            Handled=true;
            break;
       case VK_F5:
            if (btnok->Enabled)
               btnok->Click();
            Handled=true;
            break;
       case VK_F6:
            if (btncan->Enabled)
               btncan->Click();
            Handled=true;
            break;
       case VK_F7:
            if (btnprint->Enabled)
               btnprint->Click();
            Handled=true;
            break;
       case VK_F8:
            if (btnclear->Enabled)
               btnclear->Click();
            Handled=true;
            break;
       case VK_F9:
            if (btnfind->Enabled)
               btnfind->Click();
            Handled=true;
            break;
       case VK_ESCAPE:
            if (btnexit->Enabled)
               btnexit->Click();
            Handled=true;
            break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if (Key==13)
   {
      //Key=0;
      SelectNext(ActiveControl,true,true);
   }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::FormClose(TObject *Sender,
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
    if (smtype)
       delete smtype;
    if (sstype)
       delete sstype;
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::btnaddClick(TObject *Sender)
{
    btnidx=0;
    ClearEdit();
    SetEdits(1,1,0,0,0,0,0,0,0,0);
    SetButton(0,0,0,0,0,1,0);
    //edtscode->SetFocus();
    PanelHint->Caption=" �������������ѡ���������";
}
//---------------------------------------------------------------------------


void __fastcall TMaterialNoForm::btnmodClick(TObject *Sender)
{
    btnidx=2;
    edtscode->Enabled=false;
    edtcode->Enabled=false;
    edtname->Enabled=true;
    edtusage->Enabled=true;
    SetButton(0,0,0,0,1,1,0);
    PanelHint->Caption=" �����б���ѡ��Ҫ�޸ĵ����,ʹ�ò��ҿ��Կ������б��ж�λ!";
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::btndelClick(TObject *Sender)
{
    btnidx=3;
    SetButton(0,0,0,0,1,1,0);
    PanelHint->Caption=" �����б���ѡ��Ҫɾ�������";
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::btnokClick(TObject *Sender)
{
   AnsiString  materialno;
   materialno=edtscode->Text+"-"+edtcode->Text;
   switch(btnidx)
   {
      case 0://do add
           if (!CheckEdit())
              break;
           //if (!CheckCode())
           //   break;
           strsql=AnsiString("select mn_code from materialno where mn_code='")+materialno+"'";
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
              ErrorDlg("���ݿ����!");
              break;
           }
           strsql =AnsiString("insert into materialno values('")+materialno+"','"
                  +edtname->Text+"','"+edtusage->Text+"')";
           if (DoQuery(dm1->Query1,strsql,1))
           {
              DoSelect(false,true);
              //ClearEdit();
              edtcode->SetFocus();
           }
           break;
      case 1:
           if (DoSelect(false,true)==1)
              InfoDlg("û���ҵ���������������!");
           edtcorp->Enabled=false;
           break;
      case 2://do modify
           if (ListView1->SelCount==0)
           {
              InfoDlg("�������б���ѡ��Ҫ�޸ĵ����!");
              break;
           }
           if (!CheckEdit())
              break;
           //if (!CheckCode())
           //   break;
           strsql =AnsiString("update materialno set mn_name='")
                  +edtname->Text+"', mn_usage='"+edtusage->Text+"' where mn_code='"
                  +ListView1->Selected->SubItems->Strings[0]+"'";
           if (DoQuery(dm1->Query1,strsql,1))
           {
              ClearEdit();
              DoSelect(false,true);
              InfoDlg("�޸����!");
              btnprint->Enabled=true;
           }
           else
              ErrorDlg("���ݿ����");
           break;
      case 3://do delete
           if (ListView1->SelCount==0)
           {
              InfoDlg("�������б���ѡ��Ҫɾ�������!");
              break;
           }
           if (AskDlg("ȷʵҪɾ���������")==IDNO)
              break;
           strsql =AnsiString("delete materialno where mn_code='")
                  +ListView1->Selected->SubItems->Strings[0]+"'";
           if (DoQuery(dm1->Query1,strsql,1))
           {
              ClearEdit();
              DoSelect(false,true);
              InfoDlg("ɾ�����!");
              if (ListView1->Items->Count>0)
                 btnprint->Enabled=true;
           }
           else
              ErrorDlg("���ݿ����");
           break;
   }
   //if (btnidx!=6)
   //   dm1->db1->Connected=false;
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::btncanClick(TObject *Sender)
{
    SetDefault();
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::btnprintClick(TObject *Sender)
{
   if (!prtform)
   {
      ErrorDlg("������ӡ,����ӡ�������!");
      return;
   }
   //if (DoSelect(false,false)!=0)
   //   return;
   prtform->QuickRep1->PreviewModal();
   //dm1->db1->Connected=false;
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::btnrefClick(TObject *Sender)
{
    btnidx=1;
    ClearEdit();
    edtcorp->Enabled=true;
    SetEdits(1,1,0,0,0,0,1,1,1,1);
    SetButton(0,0,0,0,1,1,0);
    //edtscode->SetFocus();
    PanelHint->Caption=" �����԰�����𼶲�ѯ��ֱ��������ϱ�Ż����ƹ���ѯ";
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::ListView1Click(TObject *Sender)
{
    if (ListView1->SelCount>0 && btnidx>0)
    {
       edtscode->Text=ListView1->Selected->SubItems->Strings[0].SubString(1,5);
       edtcode->Text=ListView1->Selected->SubItems->Strings[0].SubString(7,15);
       edtname->Text=ListView1->Selected->SubItems->Strings[1];
       edtusage->Text=ListView1->Selected->SubItems->Strings[2];
       edtscodeExit(Sender);
    }
}
//---------------------------------------------------------------------------


void __fastcall TMaterialNoForm::edtlcodeKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
   if (Key==13)
   {
      Key=0;
      if (sltype->IndexOf(edtlcode->Text)>=0)
      {
         cbltype->ItemIndex=sltype->IndexOf(edtlcode->Text);
         if (GetMiddleType())
         {
            SetEdits(1,1,1,1,0,0,0,0,0,0);
            if (btnidx==0)
            {
               PanelHint->Caption=" ������������ѡ����������";
               edtmcode->SetFocus();
            }
            //else if (btnidx==1)
            //   PanelHint->Caption=" ������������ѡ����������������ϸ��ѯ";
         }
      }
      else
         ErrorDlg("�����˴���Ĵ������");
   }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::cbltypeChange(TObject *Sender)
{
    if (cbltype->ItemIndex>=0)
    {
       edtlcode->Text=sltype->Strings[cbltype->ItemIndex];
       if (GetMiddleType())
       {
          SetEdits(1,1,1,1,0,0,0,0,0,0);
          if (btnidx==0)
          {
             PanelHint->Caption=" ������������ѡ����������";
             edtmcode->SetFocus();
          }
          //else if (btnidx==1)
          //   PanelHint->Caption=" ������������ѡ����������������ϸ��ѯ";
       }
    }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::edtmcodeKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
   if (Key==13)
   {
      Key=0;
      if (smtype->IndexOf(edtmcode->Text)>=0)
      {
         cbmtype->ItemIndex=smtype->IndexOf(edtmcode->Text);
         if (GetSmallType())
         {
            SetEdits(1,1,1,1,1,1,0,0,0,0);
            if (btnidx==0)
            {
               PanelHint->Caption=" ����С������ѡ��С������";
               edtstype->SetFocus();
            }
         }
      }
      else
         ErrorDlg("�����˴�����������");
   }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::cbmtypeChange(TObject *Sender)
{
   if (cbmtype->ItemIndex>=0)
   {
      edtmcode->Text=smtype->Strings[cbmtype->ItemIndex];
      if (GetSmallType())
      {
         SetEdits(1,1,1,1,1,1,0,0,0,0);
         PanelHint->Caption=" ����С������ѡ��С������";
         if (btnidx==0)
            edtstype->SetFocus();
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::edtstypeKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
   if (Key==13)
   {
      Key=0;
      if (sstype->IndexOf(edtstype->Text)>=0)
      {
         cbstype->ItemIndex=sstype->IndexOf(edtstype->Text);
         SetEdits(1,1,1,1,1,1,0,1,1,1);
         edtscode->Text=corpcode+edtstype->Text;
         if (btnidx==0)
         {
            btnok->Enabled=true;
            PanelHint->Caption=" ������15λ���ϱ�ź����ƹ��";
            edtcode->SetFocus();
         }
      }
      else
         ErrorDlg("�����˴����С�����");
   }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::cbstypeChange(TObject *Sender)
{
   if (cbstype->ItemIndex>=0)
   {
      edtstype->Text=sstype->Strings[cbstype->ItemIndex];
      SetEdits(1,1,1,1,1,1,0,1,1,1);
      edtscode->Text=corpcode+edtstype->Text;
      if (btnidx==0)
      {
         btnok->Enabled=true;
         PanelHint->Caption=" ������15λ���ϱ�ź����ƹ��";
         edtcode->SetFocus();
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::btnexitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::btnclearClick(TObject *Sender)
{
    edtlcode->Text="";
    cbltype->ItemIndex=-1;
    edtmcode->Text="";
    cbmtype->Items->Clear();
    edtstype->Text="";
    cbstype->Items->Clear();
    edtusage->Text="";
    ClearEdit();
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::btnfindClick(TObject *Sender)
{
   if (ListView1->Items->Count<1)
   {
      InfoDlg("��ǰ�б���û�п��Բ��ҵ�����!");
      return;
   }
   if (!InputQuery("���Ҷ�λ","��������ϱ��(����ֻ���벿��)          ",info))
      return;
   LocateRow(info);
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::edtscodeExit(TObject *Sender)
{
    edtscode->Text=Trim(edtscode->Text);
    if (sltype->IndexOf(edtscode->Text.SubString(2,1))>0)
    {
       cbltype->ItemIndex=sltype->IndexOf(edtscode->Text.SubString(2,1));
       edtlcode->Text=edtscode->Text.SubString(2,1);
       if (edtscode->Text.Length()>=3)
       {
          edtmcode->Text=edtscode->Text.SubString(2,2);
          //cbltypeChange(Sender);
          //cbmtype->ItemIndex=smtype->IndexOf(edtmcode->Text);
       }
       if (edtscode->Text.Length()==5)
       {
          edtstype->Text=edtscode->Text.SubString(2,4);
          //cbmtypeChange(Sender);
          //cbstype->ItemIndex=sstype->IndexOf(edtstype->Text);
       }
    }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialNoForm::edtusageKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
   if (Key==13)
   {
      Key=0;
      if (btnidx==0||btnidx==2)
         btnok->Click();
   }
}
//---------------------------------------------------------------------------

