//---------------------------------------------------------------------------
#include <map>
#include <vcl.h>
#pragma hdrstop

#include "CostAccounting.h"
#include "common.hpp"

#include "DataModule.h"
#include "BaseCode.h"
#include "LdyInterface.h"

#include "OleExcelCom.h"
#include "Excel_2K_SRVR2.h"
#include <OleServer.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCostAccountingForm *CostAccountingForm;
//#define CNT_TOTAL_COL 46
//#define CNT_FIXED_COL 11

enum E_COLUMN_NAME{
	COL_CID = 0,
        COL_CLIWORKID,
        COL_ACCEPTDATE,
        COL_OPUNIT,
        COL_CLIENT,
        COL_GOODSPERF,
        COL_DECLAREID,
        COL_CUSTFREE,
        COL_BASECOST,
        COL_LADINGID,
        COL_CONTAINER,
        COL_BEIZHU,
};
//�����ᵥ�š���š���ע���ͻ�������
int charge_start_column = COL_BEIZHU+1; // ��������ʼ��ƫ��. +0 ->��һ�����ã� +1 �ڶ�������
void CostAccounting(int nAuth)
{
        CALL_FORM(TCostAccountingForm);
}
//same as financeform !!!
int TCostAccountingForm::getChidByChname(CString chname){
        for (std::map<int, CString>::const_iterator it=m_mChargeInfo.begin(); it!=m_mChargeInfo.end(); ++it){
                if (it->second == chname){
                        return it->first;
                }
        }
        return 0;//error
}
//---------------------------------------------------------------------------
__fastcall TCostAccountingForm::TCostAccountingForm(TComponent* Owner)
        : TForm(Owner)
{
  m_enWorkState=EN_IDLE;
//  isHasResult = false;
}
//---------------------------------------------------------------------------
void __fastcall TCostAccountingForm::FormShow(TObject *Sender)
{
        m_mChargeInfo.clear();
        cbb1->Items->Add("");
        cbb2->Items->Add("");
        cbb3->Items->Add("");
        cbb4->Items->Add("");
        cbb5->Items->Add("");
        cbb6->Items->Add("");
        cbb7->Items->Add("");
        cbb8->Items->Add("");
        cbb9->Items->Add("");
        cbb10->Items->Add("");
        CString szSQL;
        szSQL.Format("select * from charges order by chid");
        RunSQL(dm1->Query1,szSQL,true);

        int column_charge = 0;
        while(!dm1->Query1->Eof)
        {
                AnsiString chname = dm1->Query1->FieldByName("chname")->AsString;
                int chid = dm1->Query1->FieldByName("chid")->AsInteger;                
                cbb1->Items->Add(chname);
                cbb2->Items->Add(chname);
                cbb3->Items->Add(chname);
                cbb4->Items->Add(chname);
                cbb5->Items->Add(chname);
                cbb6->Items->Add(chname);
                cbb7->Items->Add(chname);
                cbb8->Items->Add(chname);
                cbb9->Items->Add(chname);
                cbb10->Items->Add(chname);
                lstView->Columns->Items[charge_start_column+(++column_charge)]->Caption = chname;

                m_mChargeInfo.insert(std::make_pair(chid, chname.c_str()));
                dm1->Query1->Next();
        }
        lstView->Columns->Items[charge_start_column+(++column_charge)]->Caption = "�ϼ�";
        cbb1->ItemIndex = 1;
        cbb2->ItemIndex = 2;
        cbb3->ItemIndex = 3;
        cbb4->ItemIndex = 4;
        cbb5->ItemIndex = 5;
        cbb6->ItemIndex = 6;
        cbb7->ItemIndex = 7;
        cbb8->ItemIndex = 8;
        cbb9->ItemIndex = 9;
        cbb10->ItemIndex = 10;

        //draw lstview column tital , after "��������" ��list all charge type, order by chid

        TDateTime tDate=GetSysTime(false);
        dtpQryAcceptDate->DateTime=tDate;
        dtpQryAcceptDateEnd->DateTime=tDate;

        edtIn->Text = "0";
        edtBase->Text = "0";
        edtPure->Text = "0";
        m_enWorkState=EN_IDLE;
        ResetCtrl();
        
}
//---------------------------------------------------------------------------
void __fastcall TCostAccountingForm::lstViewClick(TObject *Sender)
{

      if(lstView->Selected == NULL){
//        btnAddDetail->Enabled = true;
         btnMod->Enabled = false;
                  clearQryInput();
      }        
}
//---------------------------------------------------------------------------
void __fastcall TCostAccountingForm::lstViewSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{

    if (Selected!=NULL){
        ResetCharges();
        edtCid->Text = Item->SubItems->Strings[COL_CID];
        edtClient->Text = Item->SubItems->Strings[COL_CLIENT];
        edtDeclareId->Text = Item->SubItems->Strings[COL_DECLAREID];
        edtBaseCost->Text = Item->SubItems->Strings[COL_BASECOST];
        edtRemark->Text = Item->SubItems->Strings[COL_BEIZHU];
        int cbb_order = 0;

        int pos_charge = 0;

        for (std::map<int, CString>::const_iterator it=m_mChargeInfo.begin(); it!=m_mChargeInfo.end(); ++it){
                int pos = charge_start_column + pos_charge++;
                if (Item->SubItems->Strings[pos] != ""){
                        cbb_order++;
                        TComboBox *cbb = getCbb(cbb_order);
                        TEdit *edt = getEdt(cbb_order);
                        cbb->ItemIndex = cbb->Items->IndexOf(AnsiString(it->second));
                        edt->Text = Item->SubItems->Strings[pos];
                }
        }

        btnMod->Enabled = true;
     }
}
//---------------------------------------------------------------------------
void __fastcall TCostAccountingForm::btnModClick(TObject *Sender)
{
  m_enWorkState=EN_EDIT;
  ResetCtrl();
  return;        
}
//---------------------------------------------------------------------------
void __fastcall TCostAccountingForm::btnOKClick(TObject *Sender)
{
  char strName[80],*ptr,strTemp[80];
  int nState;

  switch(m_enWorkState)
  {
    case EN_EDIT:
      if (-1 == ModCharge())
        return;
      break;
    default:
      ShowMessage("Work State not AddNew or Edit");
      return;
  }

  m_enWorkState=EN_IDLE;
  ResetCtrl();          
}
//---------------------------------------------------------------------------
void __fastcall TCostAccountingForm::btnCancelClick(TObject *Sender)
{
  int nState;

  lstView->ItemIndex = -1;
  lstView->Selected = NULL; 
  nState=m_enWorkState;
  m_enWorkState=EN_IDLE;
  ResetCtrl();
  clearQryInput();
}
//---------------------------------------------------------------------------

void TCostAccountingForm::InsertValue(TComboBox *cbb, TEdit *edt){
        if (cbb->ItemIndex == 0) {
                return;
        } else if(edt->Text.IsEmpty()){
                ShowMessage("����д��Ӧ����");
                if(edt->CanFocus())
                edt->SetFocus();
                return;
        }

        char strChid[80];
   	sprintf(strChid,"%s%02d",edtCid->Text.c_str(), getChidByChname(cbb->Text.c_str()));

        CString szSQL;
        szSQL = "insert into costaccounting(caid,value) values(";
        szSQL += Str2DBString(strChid);
        szSQL +=","; szSQL += Str2DBString(edt->Text.c_str());
        szSQL +=")";
        if(!RunSQL(dm1->Query1,szSQL))
        {
                ShowMessage("insert fail!") ;
                return;
        }
}

void TCostAccountingForm::InsertCustFreeToListView2(int row, TComboBox *pcbb,AnsiString col_value){
    if (pcbb->ItemIndex == 0){
        return;
    }

    lstView->Items->Item[row]->SubItems->Strings[charge_start_column+pcbb->ItemIndex - 1] = col_value;
}

void TCostAccountingForm::InsertCustFreeToListViewOri(int row, int pos, AnsiString col_value){
    lstView->Items->Item[row]->SubItems->Strings[pos] = col_value;
}


bool TCostAccountingForm::IsDupFree()
{

        int idx1, idx2,idx3,idx4,idx5,idx6,idx7,idx8,idx9,idx10;
        idx1 = idx2 = idx3 = idx4 = idx5 = idx6 = idx7 = idx8 = idx9 = idx10 =0;

        idx1 = cbb1->ItemIndex;
        idx2 = cbb2->ItemIndex;
        idx3 = cbb3->ItemIndex;
        idx4 = cbb4->ItemIndex;
        idx5 = cbb5->ItemIndex;
        idx6 = cbb6->ItemIndex;
        idx7 = cbb7->ItemIndex;
        idx8 = cbb8->ItemIndex;
        idx9 = cbb9->ItemIndex;
        idx10 = cbb10->ItemIndex;

        if (idx1!=0 && (idx1 == idx2 || idx1 == idx3 || idx1 == idx4 || idx1 == idx5 || idx1 == idx6 || idx1 == idx7 || idx1 == idx8 || idx1 == idx9 || idx1 == idx10) ){
             if(cbb1->CanFocus())
                cbb1->SetFocus();
             return true;
        }                                                               
        if (idx2!=0 &&(idx2 == idx3 || idx2 == idx4 || idx2 == idx5 || idx2 == idx6 || idx2 == idx7 || idx2 == idx8 || idx2 == idx9 || idx2 == idx10) ){
             if(cbb2->CanFocus())
                cbb2->SetFocus();
             return true;
        }
        if (idx3!=0 &&(idx3 == idx4 || idx3 == idx5 || idx3 == idx6 || idx3 == idx7 || idx3 == idx8 || idx3 == idx9 || idx3 == idx10)){
             if(cbb3->CanFocus())
                cbb3->SetFocus();
             return true;
        }
        if (idx4!=0 &&(idx4 == idx5 || idx4 == idx6 || idx4 == idx7 || idx4 == idx8 || idx4 == idx9 || idx4 == idx10)){
             if(cbb4->CanFocus())
                cbb4->SetFocus();
             return true;
        }
         if (idx5!=0 &&(idx5 == idx6 || idx5 == idx7 || idx5 == idx8 || idx5 == idx9 || idx5 == idx10)){
             if(cbb5->CanFocus())
                cbb5->SetFocus();
             return true;
         }
          if (idx6!=0 &&( idx6 == idx7 || idx6 == idx8 || idx6 == idx9 || idx6 == idx10)){
             if(cbb6->CanFocus())
                cbb6->SetFocus();
             return true;
          }
           if (idx7!=0 &&(idx7 == idx8 || idx7 == idx9 || idx7 == idx10)){
             if(cbb7->CanFocus())
                cbb7->SetFocus();
             return true;
           }
           if (idx8!=0 &&(idx8 == idx9 || idx8 == idx10)){
             if(cbb8->CanFocus())
                cbb8->SetFocus();
             return true;
           }
            if (idx9!=0 &&(idx9 == idx10)) {
             if(cbb9->CanFocus())
                cbb9->SetFocus();
             return true;
            }


        return false;
}
float TCostAccountingForm::getCharge(TEdit *edt)
{
        return StrToFloat(edt->Text.IsEmpty()?"0":edt->Text.c_str());
}
TComboBox *TCostAccountingForm::getCbb(int i)
{
        TComboBox *rt = NULL;
        switch(i){
                case 1:
                        rt=cbb1;
                        break;
                case 2:
                        rt=cbb2;
                        break;
                case 3:
                        rt=cbb3;
                        break;
                case 4:
                        rt=cbb4;
                        break;
                case 5:
                        rt=cbb5;
                        break;
                case 6:
                        rt=cbb6;
                        break;
                case 7:
                        rt=cbb7;
                        break;
                case 8:
                        rt=cbb8;
                        break;
                case 9:
                        rt=cbb9;
                        break;
                case 10:
                        rt=cbb10;
                        break;
                default:
                        break;

        };
        return rt;
}

TEdit *TCostAccountingForm::getEdt(int i)
{
        TEdit *rt = NULL;
        switch(i){
                case 1:
                        rt=edtCharge1;
                        break;
                case 2:
                        rt=edtCharge2;
                        break;
                case 3:
                        rt=edtCharge3;
                        break;
                case 4:
                        rt=edtCharge4;
                        break;
                case 5:
                        rt=edtCharge5;
                        break;
                case 6:
                        rt=edtCharge6;
                        break;
                case 7:
                        rt=edtCharge7;
                        break;
                case 8:
                        rt=edtCharge8;
                        break;
                case 9:
                        rt=edtCharge9;
                        break;
                case 10:
                        rt=edtCharge10;
                        break;
                default:
                        break;

        };
        return rt;
}

void TCostAccountingForm::ResetCharges(){
        cbb1->ItemIndex = 0;
        cbb2->ItemIndex = 0;
        cbb3->ItemIndex = 0;
        cbb4->ItemIndex = 0;
        cbb5->ItemIndex = 0;
        cbb6->ItemIndex =  0;
        cbb7->ItemIndex =  0;
        cbb8->ItemIndex =  0;
        cbb9->ItemIndex =  0;
        cbb10->ItemIndex =  0;

        edtCharge1->Text = "";
        edtCharge2->Text = "";
        edtCharge3->Text = "";
        edtCharge4->Text = "";
        edtCharge5->Text = "";
        edtCharge6->Text = "";
        edtCharge7->Text = "";
        edtCharge8->Text = "";
        edtCharge9->Text = "";
        edtCharge10->Text = "";
}

int TCostAccountingForm::ModCharge(){
        int rt = -1;

        if (lstView->Selected == NULL) {
                ShowMessage("δѡ�м�¼");
                return rt;
        }
        char strMsg[256],strSQL[512];
        sprintf(strMsg,"\n  ȷ����������������  \n",edtCid->Text.c_str());
        if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
                return rt;

        if (edtCid->Text.IsEmpty()){
                ShowMessage("������д������");
                return rt;
        }
        //valid check
        if (IsDupFree()) {
                ShowMessage("���ظ�������Ŀ,��������");
                return rt;
        }
        //del all free
        CString szSQL;
        szSQL.Format("delete from costaccounting where caid like '%s__'",edtCid->Text.c_str());
        if(!RunSQL(dm1->Query1,szSQL))
        {
                ShowMessage("delete fail!") ;
                return rt;
        }

        //then insert new free
        InsertValue(cbb1,edtCharge1);
        InsertValue(cbb2,edtCharge2);
        InsertValue(cbb3,edtCharge3);
        InsertValue(cbb4,edtCharge4);
        InsertValue(cbb5,edtCharge5);
        InsertValue(cbb6,edtCharge6);
        InsertValue(cbb7,edtCharge7);
        InsertValue(cbb8,edtCharge8);
        InsertValue(cbb9,edtCharge9);
        InsertValue(cbb10,edtCharge10);

        //update custfree
        if (!edtBaseCost->Text.IsEmpty()) {
                szSQL.Format("update customs set basecost='%s' where cid='%s'",edtBaseCost->Text.c_str(), edtCid->Text.c_str());
                if(!RunSQL(dm1->Query1,szSQL))
                {
                        ShowMessage("update custfree fail!") ;
                        return rt;
                }
        }
        //add listview
        TListItem *pItem = lstView->Items->Add();
        pItem->Caption = 0;//flush after
        pItem->SubItems->Add(edtCid->Text);
        pItem->SubItems->Add(lstView->Selected->SubItems->Strings[COL_CLIWORKID]);
        pItem->SubItems->Add(lstView->Selected->SubItems->Strings[COL_ACCEPTDATE]);
        pItem->SubItems->Add(lstView->Selected->SubItems->Strings[COL_OPUNIT]);
        pItem->SubItems->Add(lstView->Selected->SubItems->Strings[COL_CLIENT]);
        pItem->SubItems->Add(lstView->Selected->SubItems->Strings[COL_GOODSPERF]);
        pItem->SubItems->Add(lstView->Selected->SubItems->Strings[COL_DECLAREID]);
        pItem->SubItems->Add(lstView->Selected->SubItems->Strings[COL_CUSTFREE]);
        pItem->SubItems->Add(lstView->Selected->SubItems->Strings[COL_BASECOST]);
        pItem->SubItems->Add(lstView->Selected->SubItems->Strings[COL_LADINGID]);
        pItem->SubItems->Add(lstView->Selected->SubItems->Strings[COL_CONTAINER]);
        pItem->SubItems->Add(lstView->Selected->SubItems->Strings[COL_BEIZHU]);




        pItem->SubItems->Add(edtBaseCost->Text);

        //init var charge column first, otherwise core in InsertCustFreeToListView2
        for(int i=0; i<m_mChargeInfo.size(); ++i) {
                pItem->SubItems->Add("");
        }
        pItem->SubItems->Add("");

        int row = pItem->Index;        
        InsertCustFreeToListView2(row, cbb1, edtCharge1->Text);
        InsertCustFreeToListView2(row, cbb2, edtCharge2->Text);
        InsertCustFreeToListView2(row, cbb3, edtCharge3->Text);
        InsertCustFreeToListView2(row, cbb4, edtCharge4->Text);
        InsertCustFreeToListView2(row, cbb5, edtCharge5->Text);
        InsertCustFreeToListView2(row, cbb6, edtCharge6->Text);
        InsertCustFreeToListView2(row, cbb7, edtCharge7->Text);
        InsertCustFreeToListView2(row, cbb8, edtCharge8->Text);
        InsertCustFreeToListView2(row, cbb9, edtCharge9->Text);
        InsertCustFreeToListView2(row, cbb10, edtCharge10->Text);
        float total_var_charges =  getCharge(edtCharge1) + getCharge(edtCharge2) + getCharge(edtCharge3) +
            getCharge(edtCharge4) + getCharge(edtCharge5) + getCharge(edtCharge6) +
            getCharge(edtCharge7) + getCharge(edtCharge8) + getCharge(edtCharge9) +
            getCharge(edtCharge10) ;
        float total = StrToFloat(lstView->Selected->SubItems->Strings[COL_CUSTFREE].c_str()) -
                        StrToFloat(lstView->Selected->SubItems->Strings[COL_BASECOST].c_str()) -
                        total_var_charges;

        lstView->Items->Item[row]->SubItems->Strings[charge_start_column+m_mChargeInfo.size()] = FloatToStr(total);
        //delete after add
        lstView->Selected->Delete();

        //clear input
        clearQryInput();
        lstView->ItemIndex = -1;
        lstView->Selected = NULL;
        //~
        //flush
        flushSum();
        rt = 0;
        flushColumnNo();
        return rt;
}

void TCostAccountingForm::ResetCtrl()
{
  if(m_enWorkState==EN_IDLE)
  {
    //lstview
    lstView->Enabled = true;
    //btn
//    btnMod->Enabled=isHasResult;
    btnMod->Enabled=false;
    btnOK->Enabled =false;
    btnCancel->Enabled=false;
    //qry panel
    for(int   i=0;i <plQry->ControlCount;i++)
    {
              plQry->Controls[i]->Enabled = true;
    }
    //charge panel
    for(int   i=0;i <plMod->ControlCount;i++)
    {
              plMod->Controls[i]->Enabled = false;
    }
  }
  else
  {
    //lstview
    lstView->Enabled = false;
    //btn
    btnMod->Enabled=false;
    btnOK->Enabled =true;
    btnCancel->Enabled=true;
    //qry panel
    for(int   i=0;i <plQry->ControlCount;i++)
    {
              plQry->Controls[i]->Enabled = false;
    }
    //charge panel
    for(int   i=0;i <plMod->ControlCount;i++)
    {
              plMod->Controls[i]->Enabled = true;
    }
  }

}
void __fastcall TCostAccountingForm::btnQueryClick(TObject *Sender)
{
        //gen date str
        unsigned short year0,month0,day0;
	dtpQryAcceptDate->DateTime.DecodeDate(&year0,&month0,&day0);
        char strDate0[80];
   	sprintf(strDate0,"%04d%02d%02d",year0,month0,day0);
        unsigned short year1,month1,day1;
	dtpQryAcceptDateEnd->DateTime.DecodeDate(&year1,&month1,&day1);
        char strDate1[80];
   	sprintf(strDate1,"%04d%02d%02d 23:59:59",year1,month1,day1);
        //~

        CString szSQL;
        //custcoms
        szSQL="select cid, cliworkid, acceptdate, client, goodsperf, declareid, ladingid, containerinfo, beizhu, custfree, basecost, \
                        operunit,CONVERT(varchar(10), acceptdate, 23) as ad \
                        from customs where 1=1 ";
        if (!edtCid->Text.IsEmpty()){
                szSQL +=" and cid="; szSQL += Str2DBString(edtCid->Text.c_str());
        }
        if (!edtDeclareId->Text.IsEmpty()){
                szSQL +=" and declareid="; szSQL += Str2DBString(edtDeclareId->Text.c_str());
        }
        if (cbIsQryByDate->Checked) {
                szSQL += " and acceptdate between "; szSQL += Str2DBString(strDate0);
                szSQL += " and ";
                szSQL += Str2DBString(strDate1);
        }        
        if (!edtClient->Text.IsEmpty()){
                szSQL +=" and client like '%"; szSQL += edtClient->Text.c_str(); szSQL += "%'";
        }
        if (!edtContainerinfo->Text.IsEmpty()){
                szSQL +=" and containerinfo like '%"; szSQL += edtContainerinfo->Text.c_str(); szSQL += "%'";
        }

        szSQL += " order by acceptdate";
        RunSQL(dm1->Query1,szSQL,true);
        lstView->Clear();
        if (dm1->Query1->Eof){
                m_enWorkState=EN_IDLE;
                ResetCtrl();
        }

        int column_no=0;
	while(!dm1->Query1->Eof)
	{
                column_no++;
                TListItem *pItem = lstView->Items->Add();
                pItem->Caption = column_no;
                pItem->SubItems->Add(dm1->Query1->FieldByName("cid")->AsString);
                pItem->SubItems->Add(dm1->Query1->FieldByName("cliworkid")->AsString);
                pItem->SubItems->Add(dm1->Query1->FieldByName("ad")->AsString);
                pItem->SubItems->Add(dm1->Query1->FieldByName("operunit")->AsString);
                pItem->SubItems->Add(dm1->Query1->FieldByName("client")->AsString);
                pItem->SubItems->Add(dm1->Query1->FieldByName("goodsperf")->AsString);

                pItem->SubItems->Add(dm1->Query1->FieldByName("declareid")->AsString);

                //qry sum_finance
                float sum_finance = 0;
                {
                        CString szSQL_Finance;
                        szSQL_Finance.Format("select sum(value) as sum_finance \
                                from finance where fid like '%s__'", dm1->Query1->FieldByName("cid")->AsString);
                        RunSQL(dm1->Query2,szSQL_Finance,true);
                        if (!dm1->Query2->Eof){
                                sum_finance = dm1->Query2->FieldByName("sum_finance")->AsFloat;
                        }
                }
                //���� = ���ط� + ���úϼ� (finance)
                float income = dm1->Query1->FieldByName("custfree")->AsFloat + sum_finance;
                //~
                pItem->SubItems->Add(FloatToStr(income));
                pItem->SubItems->Add(dm1->Query1->FieldByName("basecost")->AsString);   //���ػ����ɱ�
                pItem->SubItems->Add(dm1->Query1->FieldByName("ladingid")->AsString);
                pItem->SubItems->Add(AnsiString(getAllContainerNo(dm1->Query1->FieldByName("containerinfo")->AsString)));
                pItem->SubItems->Add(dm1->Query1->FieldByName("beizhu")->AsString);
                //init var charge column first, otherwise core in InsertCustFreeToListView2
                for(int i=0; i<m_mChargeInfo.size(); ++i) {
                        pItem->SubItems->Add("");
                }
                //for last column-total
                pItem->SubItems->Add("");

                //qry costaccounting
                int row = pItem->Index;
                //�ɱ���ϸ costaccounting
                float total_costaccounting = 0;
                {
                        CString szSQL2;
                        szSQL2.Format("select  * \
                                        from costaccounting, charges \
                                        where chid=CAST(substring(caid, len(caid)-1,2) as int) and caid like '%s__'" \
                                        , dm1->Query1->FieldByName("cid")->AsString);
                        RunSQL(dm1->Query3,szSQL2,true);


                        for (int i=1; i<=10 && !dm1->Query3->Eof; ++i){
                                int pos = charge_start_column + cbb1->Items->IndexOf(dm1->Query3->FieldByName("chname")->AsString.c_str()) -1;  //��cbb1��
                                InsertCustFreeToListViewOri(row, pos, dm1->Query3->FieldByName("value")->AsString);
                                total_costaccounting += dm1->Query3->FieldByName("value")->AsFloat;
                                dm1->Query3->Next();
                        }
//                        lstView->Items->Item[row]->SubItems->Strings[charge_start_column+gCHARGES.GetSize()] = FloatToStr(total);

                }
                
                //���� = ���� - ���ػ����ɱ� - �ɱ��ϼ�
                float total = income - dm1->Query1->FieldByName("basecost")->AsFloat - total_costaccounting;
                lstView->Items->Item[row]->SubItems->Strings[charge_start_column+m_mChargeInfo.size()] = FloatToStr(total);
		dm1->Query1->Next();
        }
        flushSum();
        m_enWorkState=EN_IDLE;
        ResetCtrl();        
}
//---------------------------------------------------------------------------



CString TCostAccountingForm::getFirstContainerNo(AnsiString c)
{
        CString rt = "";
        char cnt[10];memset(cnt,sizeof(cnt),0x00);
        char body[2048];memset(body,sizeof(body),0x00);
        sscanf(c.c_str(),"%[^|]|",cnt);
        sscanf(c.c_str(),"%*[^|]|%[^@]",body);

        char strTmp1[2048];
        memset(strTmp1,sizeof(strTmp1),0x00);
        strcpy(strTmp1,body);
        int num = StrToInt(cnt);
        for (int i =0; i<num && i<1; ++i){
                char str1[100],str2[100],str3[100],str4[100],strTmp[2048],strSealId[100];
                memset(str1,sizeof(str1),0x00);
                memset(str2,sizeof(str2),0x00);
                memset(str3,sizeof(str3),0x00);
                memset(str4,sizeof(str4),0x00);
                memset(strSealId,sizeof(strSealId),0x00);

                memset(strTmp,sizeof(strTmp),0x00);
                strcpy(strTmp,strTmp1);
                sscanf(strTmp,"%[^#]#",str1);
                sscanf(str1,"%s %s %s",str3,str4,strSealId);
                sscanf(strTmp,"%*[^#]#%[^@]",strTmp1);
//			printf("head:[%s] tail:[%s]\n", str1, strTmp1);
//			printf("bno:[%s] type:[%s]\n\n", str3, str4);
                rt = str3;
        }
        return rt;
}
//---------------------------------------------------------------------------



void TCostAccountingForm::clearQryInput(){
        edtRemark->Text = "";
        edtCid->Text = "";
        edtDeclareId->Text = "";
        edtClient->Text = "";
        cbb1->ItemIndex = -1;
        cbb2->ItemIndex = -1;
        cbb3->ItemIndex = -1;
        cbb4->ItemIndex = -1;
        cbb5->ItemIndex = -1;
        cbb6->ItemIndex = -1;
        cbb7->ItemIndex = -1;
        cbb8->ItemIndex = -1;
        cbb9->ItemIndex = -1;
        cbb10->ItemIndex = -1;
        edtCharge1->Text = "";
        edtCharge2->Text = "";
        edtCharge3->Text = "";
        edtCharge4->Text = "";
        edtCharge5->Text = "";
        edtCharge6->Text = "";
        edtCharge7->Text = "";
        edtCharge8->Text = "";
        edtCharge9->Text = "";
        edtCharge10->Text = "";
}
CString TCostAccountingForm::getAllContainerNo(AnsiString c){
        CString rt = "";
        char cnt[10];memset(cnt,sizeof(cnt),0x00);
        char body[2048];memset(body,sizeof(body),0x00);
        sscanf(c.c_str(),"%[^|]|",cnt);
        sscanf(c.c_str(),"%*[^|]|%[^@]",body);

        char strTmp1[2048];
        memset(strTmp1,sizeof(strTmp1),0x00);
        strcpy(strTmp1,body);
        int num = StrToInt(cnt);
        for (int i =0; i<num; ){
                char str1[100],str2[100],str3[100],str4[100],strTmp[2048],strSealId[100];
                memset(str1,sizeof(str1),0x00);
                memset(str2,sizeof(str2),0x00);
                memset(str3,sizeof(str3),0x00);
                memset(str4,sizeof(str4),0x00);
                memset(strSealId,sizeof(strSealId),0x00);

                memset(strTmp,sizeof(strTmp),0x00);
                strcpy(strTmp,strTmp1);
                sscanf(strTmp,"%[^#]#",str1);
                sscanf(str1,"%s %s %s",str3,str4,strSealId);
                sscanf(strTmp,"%*[^#]#%[^@]",strTmp1);
//			printf("head:[%s] tail:[%s]\n", str1, strTmp1);
//			printf("bno:[%s] type:[%s]\n\n", str3, str4);
                rt += str3;
//                rt += " | ";
                if (++i<num){
                        rt += "\r\n ";
                }
        }
        return rt;
}


void __fastcall TCostAccountingForm::Button1Click(TObject *Sender)
{
        AnsiString sfile ="";
 if(SaveDialog1->Execute()){
        sfile = SaveDialog1->FileName;
//        ShowMessage(sfile);
 }else{
        ShowMessage("����δ���");
        return;
 }
        std::map<int, int> map_charge_exist; // �������ж�û��ĳ�����ã�����0��������1��

        //��ʼ��ȫ��Ϊ0
        for (int i=0; i<m_mChargeInfo.size(); ++i){
                map_charge_exist.insert(std::make_pair(i, 0));
        }
        //init listViewPure's column header names by listView's
        int pos_pure = 0;
        lstViewPure->Columns->Items[0]->Caption = "���";
        for (pos_pure=0; pos_pure<charge_start_column-1-1; pos_pure++){//������ע�����
                lstViewPure->Columns->Items[pos_pure+1]->Caption = lstView->Columns->Items[pos_pure+1+1]->Caption;
        }
        //~
        //����Ǹ����÷ǿ�
        for (int row=0; row<lstView->Items->Count; ++row) {
                for (int col=0; col<m_mChargeInfo.size(); ++col) {
                        int pos_real = charge_start_column + col;
                        if (lstView->Items->Item[row]->SubItems->Strings[pos_real] != ""){
                                map_charge_exist[col] = 1 ;
                        }
                }
        }
        //����ǿշ������� - ����map_charge_exist�����������
       for (int j=0; j<m_mChargeInfo.size(); ++j) {
               int pos_real = charge_start_column+j;
                if (map_charge_exist[j] == 1){
                    lstViewPure->Columns->Items[1+pos_pure++]->Caption = lstView->Columns->Items[pos_real+1]->Caption;
                }
       }
        lstViewPure->Columns->Items[1+pos_pure++]->Caption = "�ϼ�";
        lstViewPure->Columns->Items[1+pos_pure++]->Caption = "��ע";

        //mv data
        lstViewPure->Clear();
        edtMockDeclareid->Text = "";
        for(int i=0;i<lstView->Items->Count;i++)
        {
                 TListItem *pItem = lstViewPure->Items->Add();
//                 pItem->Caption = lstView->Items->Item[i]->Caption;
                 pItem->Caption = AnsiString(i+1);
                 pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[COL_CLIWORKID]);
                 pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[COL_ACCEPTDATE]);
                 pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[COL_OPUNIT]);
                 pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[COL_CLIENT]);
                 pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[COL_GOODSPERF]);
                 edtMockDeclareid->Text = lstView->Items->Item[i]->SubItems->Strings[COL_DECLAREID];
                 pItem->SubItems->Add((edtMockDeclareid->Text.Length() > 9)? edtMockDeclareid->Text.SubString(edtMockDeclareid->Text.Length()-9+1,9):edtMockDeclareid->Text);

                 pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[COL_CUSTFREE]);
                 pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[COL_BASECOST]);
                 pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[COL_LADINGID]);
                 pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[COL_CONTAINER]);
                 int j=0;
                 int cnt=0;
                 for (j=0; j<m_mChargeInfo.size(); ++j) {
                         int pos_real = charge_start_column+j;
                        if (map_charge_exist[j] == 0){
                               continue;
                        }
                        pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[pos_real]);
                        cnt++;
                 }
                 pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[charge_start_column+j]);
                 pItem->SubItems->Add(lstView->Items->Item[i]->SubItems->Strings[COL_BEIZHU]);
                 //add tail column for gen excel not to core
                 //���룬�����쳣�����µ���1����¼���ж�
                 for (int t=cnt; t<40; ++t)
                        pItem->SubItems->Add("");
        }                 
       //gen excel
        Variant vExcel,vWorkBook,vSheet,vRange,vBorders, vPicture;
        int iCols,iRows;
        iCols = iRows = 0;
        try{

                vExcel = Variant::CreateObject("Excel.Application");      //��excel
//                vExcel.OlePropertySet("Visible", false);
                vExcel.OlePropertyGet("Workbooks").OleFunction("Add", 1); // ����������

                vSheet = vExcel.OlePropertyGet("ActiveWorkbook").OlePropertyGet("Sheets", 1);//�������������
                vWorkBook = vExcel.OlePropertyGet("ActiveWorkbook");

                //��������
//                vExcel.OlePropertySet("Visible",true);
                vExcel.OlePropertySet("DisplayAlerts",false);
//                vExcel.OlePropertySet("Visible",false);
                vSheet.OlePropertyGet("Rows",++iRows).OlePropertySet("RowHeight",72);//����ָ���еĸ߶�Ϊ28
                vSheet.OlePropertyGet("Cells").OlePropertySet("WrapText", true);//�������е�Ԫ����ı��Զ�����
                vSheet.OlePropertyGet("Columns").OlePropertySet("ColumnWidth",15);//���������е��п�Ϊ28


                //��ӱ���ͼƬ
                AnsiString strPic = "d:\\p1.jpg";
                vPicture = vSheet.OlePropertyGet( "Pictures").OleFunction( "Insert",strPic.c_str());
                vPicture.OlePropertySet("Left", 50);
                vPicture.OlePropertySet("Top", 10);
                
                //����
//                Insert2Excel(vSheet, ++iRows, );
                ++iRows;
                for (int i=0; i<lstViewPure->Columns->Count; ++i){
                //lstViewPure->Columns->Items[i]->Caption
                        vSheet.OlePropertyGet("Cells",iRows,i+1).OlePropertySet("Value",lstViewPure->Columns->Items[i]->Caption.c_str());
                }                        

                //����
            TListItem *pItem;
            for(int i=0;i<lstViewPure->Items->Count;i++)
            {
                ++iRows;
                iCols = 0;
    	        	pItem=lstViewPure->Items->Item[i];
//                if (lbGoodsPerf->Caption != "������"){					
                vSheet.OlePropertyGet("Cells",iRows,1).OlePropertySet("Value",pItem->Caption.c_str());
                for (int i=1; i<lstViewPure->Columns->Count; ++i){
                        vSheet.OlePropertyGet("Cells",iRows,i+1).OlePropertySet("Value",pItem->SubItems->Strings[i-1].c_str());
                }
           }        
                //�ӿ�
//                AnsiString strRange = "A"+IntToStr(2)+":"+AnsiString(NumbertoString(lstViewPure->Columns->Count+1))+IntToStr(iRows);
                AnsiString strRange = "A"+IntToStr(2)+":"+AnsiString(NumbertoString(1+pos_pure))+IntToStr(iRows);
                vRange = vSheet.OlePropertyGet("Range",strRange.c_str());
                vRange.OlePropertySet("VerticalAlignment", 3);
                vRange.OlePropertySet("HorizontalAlignment", 3);
                vBorders = vRange.OlePropertyGet("Borders");
                vBorders.OlePropertySet("linestyle",xlContinuous);
                vBorders.OlePropertySet("weight",xlThin);
                vBorders.OlePropertySet("colorindex",xlAutomatic);

                //�˺���Ϣ
                /////////////////////////////////////////////////////////////////////////////////////
                ++iRows;
                ++iRows;

                AnsiString strRange1="A"+AnsiString(iRows)+":D"+AnsiString(iRows);
                vRange=vSheet.OlePropertyGet( "Range", strRange1.c_str());
                vRange.OleProcedure("Select");
                vRange.OleFunction( "Merge",   true);
                 vSheet.OlePropertyGet("Rows",iRows).OlePropertySet("RowHeight",72);//����ָ���еĸ߶�Ϊ28
                AnsiString strAccount1 = "�˺ţ�4758 5020 1880 8049 23\r\n \
�����У��й����й����в��֧��\r\n \
���������ǲ�";

                vSheet.OlePropertyGet("Cells",iRows,1).OlePropertySet("Value",strAccount1.c_str());
                /////////////////////////////////////////////////////////////////////////////////////
                ++iRows;
                ++iRows;

                strRange1="A"+AnsiString(iRows)+":D"+AnsiString(iRows);
                vRange=vSheet.OlePropertyGet( "Range", strRange1.c_str());
                vRange.OleProcedure("Select");
                vRange.OleFunction( "Merge",   true);
                 vSheet.OlePropertyGet("Rows",iRows).OlePropertySet("RowHeight",72);//����ָ���еĸ߶�Ϊ28
                strAccount1 = "�˺ţ�6228 4800 8389 9556 910\r\n \
�����У�ũ�з�خ�°�֧��\r\n \
����������Ǫ";
                vSheet.OlePropertyGet("Cells",iRows,1).OlePropertySet("Value",strAccount1.c_str());
                
                /////////////////////////////////////////////////////////////////////////////////////
                ++iRows;
                ++iRows;

                strRange1="A"+AnsiString(iRows)+":D"+AnsiString(iRows);
                vRange=vSheet.OlePropertyGet( "Range", strRange1.c_str());
                vRange.OleProcedure("Select");
                vRange.OleFunction( "Merge",   true);
                 vSheet.OlePropertyGet("Rows",iRows).OlePropertySet("RowHeight",72);//����ָ���еĸ߶�Ϊ28
                strAccount1 ="�˺ţ�3602 0242 0100 0738 803\r\n \
�����У��������֧�� \r\n \
�������ɳ���";
                vSheet.OlePropertyGet("Cells",iRows,1).OlePropertySet("Value",strAccount1.c_str());
                /////////////////////////////////////////////////////////////////////////////////////

//                vWorkBook.OleProcedure("Save");

//                char strFile[80];
//                memset(strFile, 0x00, sizeof(strFile));
//                sprintf(strFile,"d:\\runshun_�ɱ�_%s.xlsx",GetSysTime2());

//                AnsiString sfile = "d:\\runshun_�ɱ�_"+AnsiString(GetSysTime2())+".xlsx";
//                ShowMessage(sfile);
                vSheet.OleProcedure("SaveAs", sfile.c_str());

                vWorkBook.OleProcedure("Close");

                vExcel.OleFunction("Quit");
                vWorkBook = Unassigned;
                vExcel = Unassigned;
                ShowMessage("�������");
        }catch(...){
                ShowMessage("����ʧ��");
        }           
                                
}
//---------------------------------------------------------------------------

void TCostAccountingForm::flushSum(){
      TListItem *pItem;
        char  strTemp[80];
        double totalIn = 0;
        double totalBase=0;
        double totalPure=0;

        int i=0;
        for(i=0;i<lstView->Items->Count;i++)
        {
                pItem=lstView->Items->Item[i];
                totalIn += StrToFloat(pItem->SubItems->Strings[COL_CUSTFREE]);
                totalPure += StrToFloat(pItem->SubItems->Strings[charge_start_column+m_mChargeInfo.size()]);
        }

        edtIn->Text = totalIn;
        edtBase->Text = totalIn -totalPure;
        edtPure->Text = totalPure;
}

void TCostAccountingForm::flushColumnNo(){
        int cnt_row = lstView->Items->Count;
        int column_no=0;
        if (cnt_row>0){
                for (int row=0; row<(cnt_row-1); ++row) {
                        column_no++;
//                        lstView->Items->Item[row]->SubItems->Strings[0] = column_no;
                        lstView->Items->Item[row]->Caption = column_no;
                }
        }
}




