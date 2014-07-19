#ifndef  _common_util_
#define  _common_util_

#include <vcl.h>
#include <math.h>
#include "DataModule.h"
#include "LdyInterface.h"
#include "BaseCode.h"
#include <vector>
//excel include
#include "OleExcelCom.h"
#include "Excel_2K_SRVR2.h"
#include <OleServer.hpp>
//~

typedef std::vector<CString> sql_t;
//////////////////////////////////////////////////////////////////////////////////////
//control
//�ж�tedit�����Ƿ�����
bool isNum(AnsiString s){

        try{
                StrToInt(s);
        }catch(...){
                return false;
        }
        return true;
}
//�ж��Ƿ����ֽ�֧��С��λ���ж�(���(��)����λС��)
bool isMoney(AnsiString s, int decimal_place = 0){
        try{
                if (s.IsEmpty()){
                        return true;
                }

                double money = 0;
                if(!TryStrToFloat(s, money)){
//                        ShowMessage("It's not a double!");
                        return false;
                }

                float money2 = money*pow(10,2);
                int money3   = (int)(money2);
                if ( money3!=money2 ){
                        return false;
                }
        }catch(...){
                return false;
        }
        return true;
}
bool isMoneyUnsign(AnsiString s, int decimal_place = 0){

     AnsiString s_unsign;
     if (s.Length()>0){
             if (s.SubString(1,1)=="-"){
                if (s.Length()>1){
                        s_unsign = s.SubString(2,s.Length()-1);
                }
             }
     }
     return isMoney(s_unsign, decimal_place);
}
//����panle������Ԫ��enabled����
void enablePlCtrl(TPanel *pl, bool enabled = true){
	for(int i=0; i <pl->ControlCount; i++)
	{
		TControl *p = dynamic_cast<TControl*>(pl->Controls[i]);
		pl->Controls[i]->Enabled = enabled;
	}
}
void locateCbbByValue(TComboBox *cbb, AnsiString value){
        cbb->ItemIndex = cbb->Items->IndexOf(value);
}
void setDtp(TDateTimePicker *dtp, AnsiString value)
{
        TDateTime tDate;
        tDate=StrToDateTime(value);
        dtp->DateTime=tDate;
//        dtpEndDateHHMM->Time=tDate;
}
void delLvItem(TListView *lv){
        TListItem *pItem = lv->Selected;
        if(pItem!=NULL)
        {
                int nSel=lv->Items->IndexOf(pItem);
                lv->Items->Delete(nSel);
                lv->Selected=NULL;
        }
}

//////////////////////////////////////////////////////////////////////////////////////
//time
CString GetSysTime2(){
  unsigned short year,month,day,h,m,s,u;

  char strDate[80];
  TDateTime tDate=Now();
  tDate.DecodeDate(&year,&month,&day);
  tDate.DecodeTime(&h, &m, &s, &u);
  sprintf(strDate,"%04d%02d%02d%02d%02d%02d",year,month,day,h,m,s);
  return CString(strDate);
}

CString GetDate(TDateTimePicker *dtp)
{
        unsigned short year,month,day;
	dtp->DateTime.DecodeDate(&year,&month,&day);;
        char strDate[80];
        memset(strDate, 0x00, sizeof(strDate));
        sprintf(strDate,"%04d-%02d-%02d",year,month,day);
        return CString(strDate);
}
//@return int type of YYYYMM
int GetYYYYMM(TDateTimePicker *dtp)
{
        unsigned short year,month,day;
	dtp->DateTime.DecodeDate(&year,&month,&day);
        return year*100+month;
}
CString GetTime(TDateTimePicker *dtp){
  unsigned short y,m,d,h,mi,s,u;
  char strDate[80];
        memset(strDate, 0x00, sizeof(strDate));
  dtp->DateTime.DecodeDate(&y,&m,&d);
  dtp->DateTime.DecodeTime(&h,&mi,&s,&u);
  sprintf(strDate,"%04d-%02d-%02d %02d:%02d:%02d",y,m,d,h,mi,s);
  return CString(strDate);

}
CString GetTimeBy2Dtp(TDateTimePicker *dtp_ymh, TDateTimePicker *dtp_hms){
  unsigned short y,m,d,h,mi,s,u;
  char strDate[80];
        memset(strDate, 0x00, sizeof(strDate));
  dtp_ymh->DateTime.DecodeDate(&y,&m,&d);
  dtp_hms->DateTime.DecodeTime(&h,&mi,&s,&u);
  sprintf(strDate,"%04d-%02d-%02d %02d:%02d:%02d",y,m,d,h,mi,s);
  return CString(strDate);

}
CString GetTimeBy2DtpYMD(TDateTimePicker *dtp_ymh, TDateTimePicker *dtp_hms){
  unsigned short y,m,d,h,mi,s,u;
  char strDate[80];
        memset(strDate, 0x00, sizeof(strDate));
  dtp_ymh->DateTime.DecodeDate(&y,&m,&d);
  dtp_hms->DateTime.DecodeTime(&h,&mi,&s,&u);
  sprintf(strDate,"%04d-%02d-%02d",y,m,d);
  return CString(strDate);

}
CString GetMonthBegin(TDateTimePicker *dtp){
        unsigned short year,month,day;
	dtp->DateTime.DecodeDate(&year,&month,&day);;
        char strDate[80];
        memset(strDate, 0x00, sizeof(strDate));
        sprintf(strDate,"%04d-%02d-1 00:00:00",year,month);
        return CString(strDate);
}
CString GetMonthEnd(TDateTimePicker *dtp){
        unsigned short year,month,day;
	dtp->DateTime.DecodeDate(&year,&month,&day);;
        char strDate[80];
        memset(strDate, 0x00, sizeof(strDate));
        sprintf(strDate,"%04d-%02d-25 00:00:00",year,month);
        return CString(strDate);
}
CString GetTimeEnd(TDateTimePicker *dtp){
        unsigned short year,month,day;
	dtp->DateTime.DecodeDate(&year,&month,&day);;
        char strDate[80];
        memset(strDate, 0x00, sizeof(strDate));
        sprintf(strDate,"%04d-%02d-%02d 23:59:59",year,month,day);
        return CString(strDate);
}
CString GetYearMonth(TDateTimePicker *dtp)
{
        unsigned short year,month,day;
	dtp->DateTime.DecodeDate(&year,&month,&day);;
        char strDate[80];
        memset(strDate, 0x00, sizeof(strDate));
        sprintf(strDate,"%04d-%02d",year,month);
        return CString(strDate);
}

CString GetYear(TDateTimePicker *dtp)
{
        unsigned short year,month,day;
	dtp->DateTime.DecodeDate(&year,&month,&day);;
        char strDate[80];
        memset(strDate, 0x00, sizeof(strDate));
        sprintf(strDate,"%04d",year);
        return CString(strDate);
}

CString GetMonth(TDateTimePicker *dtp)
{
        unsigned short year,month,day;
	dtp->DateTime.DecodeDate(&year,&month,&day);;
        char strDate[80];
        memset(strDate, 0x00, sizeof(strDate));
        sprintf(strDate,"%d",month);
        return CString(strDate);
}

void SetDateTimePicker(TDateTimePicker *dtp, AnsiString strDate){

        TDateTime tDate;
        tDate=StrToDateTime(strDate);
        dtp->DateTime=tDate;
}

void SetDateTimePicketNow(TDateTimePicker *dtp){
        TDateTime tNow=TDateTime::CurrentDateTime();
        dtp->DateTime=tNow;
        dtp->Time = EncodeTime(00,00,00,0);
}

//////////////////////////////////////////////////////////////////////////////////
//sql
bool checkDataExist(CString sql){
      RunSQL(sql,true);
//      return dm1->Query1->RecordCount>0?true:false;
      return !dm1->Query1->Eof;
}
//batch sql by trans
int batchRunSQL(const sql_t &sqls){
        int rt = 0;
        //start tran
        BeginTransact();
        try {
                for(sql_t::const_iterator it=sqls.begin(); it!=sqls.end(); ++it){
#if 0
                        ShowMessage(AnsiString(*it));
#endif
                        RunTransactSQL(*it);
                }
                CommitTransact();
        } catch (...) {
                ShowMessage("����ʧ��");
                RollBackTransact();
                rt = -1;
        }
        //~end tran
        return rt;
}

///////////////////////////////////////////////////////////////////////////////////
//string
AnsiString getByEdt(TEdit *edt)
{
        return edt->Text.IsEmpty()?AnsiString(""):edt->Text;
}
CString getByEdt2(TEdit *edt)
{
        return edt->Text.IsEmpty()?AnsiString("").c_str():edt->Text.c_str();
}


//excel
class OleHelper{
public:
        explicit OleHelper(Variant &vSheet, AnsiString begin_column_type="", int begin_column_num=1, AnsiString end_column_type="", int end_column_num=1);
        ~OleHelper(){}
        //merge cells --//merge begin_column_type:begin_column_num - end_column_type:end_column_num to a cell
        OleHelper &merge(bool culumn=true);
        //����ָ���еĸ߶�
        OleHelper &set_row_height(int height, int row=0);
        //���õ�row�е�column�е�Ԫ��������ʽ        
        OleHelper &set_font_type(AnsiString type, int row=0, int column=0);
        OleHelper &set_font_style(AnsiString style, int row=0, int column=0);
        OleHelper &set_font_size(int size, int row=0, int column=0);

        OleHelper &set_alignment(int alignment_type, int row=0, int column=0);

        OleHelper &write(AnsiString value, int row=0, int column=0);

        OleHelper &write_under_line(int row=0, int column=0);
        OleHelper &write_box();
private:
        Variant &m_vSheet;
        Variant m_vRange;
};

OleHelper::OleHelper(Variant &vSheet, AnsiString begin_column_type, int begin_column_num, AnsiString end_column_type, int end_column_num):m_vSheet(vSheet){
        if (begin_column_type==""){
                return;
        }
        AnsiString strRange = begin_column_type + AnsiString(begin_column_num)+":" + end_column_type + AnsiString(end_column_num);
        m_vRange = m_vSheet.OlePropertyGet("Range", strRange.c_str());
}
OleHelper &OleHelper::merge(bool column){
        m_vRange.OleProcedure("Select");
        m_vRange.OleFunction( "Merge", column);   //true:row-merge false:column-merge
        return *this;
}
OleHelper &OleHelper::set_row_height(int height, int row){
        if (row==0){
                m_vRange.OlePropertyGet("Rows").OlePropertySet("RowHeight", height);        
        } else {
                m_vSheet.OlePropertyGet("Rows",row).OlePropertySet("RowHeight", height);
        }

        return *this;
}
OleHelper &OleHelper::set_font_type(AnsiString type, int row, int column){
        if (row==0){
                m_vRange.OlePropertyGet("Cells").OlePropertyGet("Font").OlePropertySet("Name", type.c_str());
        } else {
                m_vSheet.OlePropertyGet("Cells", row, column).OlePropertyGet("Font").OlePropertySet("Name", type.c_str());
        }
        return *this;
}
OleHelper &OleHelper::set_font_style(AnsiString style, int row, int column){
        if (row==0){
                m_vRange.OlePropertyGet("Cells").OlePropertyGet("Font").OlePropertySet("FontStyle", style.c_str());
        } else {
                m_vSheet.OlePropertyGet("Cells", row, column).OlePropertyGet("Font").OlePropertySet("FontStyle", style.c_str());
        }
        return *this;
}
OleHelper &OleHelper::set_font_size(int size, int row, int column){
        if (row==0){
                m_vRange.OlePropertyGet("Cells").OlePropertyGet("Font").OlePropertySet("Size", size);
        } else {
                m_vSheet.OlePropertyGet("Cells", row, column).OlePropertyGet("Font").OlePropertySet("Size", size);
        }
        return *this;
}

OleHelper &OleHelper::set_alignment(int alignment_type, int row, int column){
        if (row==0){
                m_vRange.OlePropertyGet("Cells").OlePropertySet("HorizontalAlignment", alignment_type); //����
        } else {
                m_vSheet.OlePropertyGet("Cells",row,column).OlePropertySet("HorizontalAlignment", alignment_type); //����
        }

        return *this;        
}

OleHelper &OleHelper::write(AnsiString value, int row, int column){
        if (row==0){
                m_vRange.OlePropertyGet("Cells").OlePropertySet("Value", value.c_str());
        } else {
                m_vSheet.OlePropertyGet("Cells",row,column).OlePropertySet("Value", value.c_str());        
        }
        return *this;
}
OleHelper &OleHelper::write_under_line(int row, int column){
        if (row==0){
                m_vRange.OlePropertyGet("Cells").OlePropertyGet("Borders").OlePropertyGet("Item",xlEdgeBottom).OlePropertySet("Weight",xlThin);//����Ԫ���ϱ߿�
        } else {
                m_vSheet.OlePropertyGet("Cells", row, column).OlePropertyGet("Borders").OlePropertyGet("Item",xlEdgeBottom).OlePropertySet("Weight",xlThin);//����Ԫ���ϱ߿�
        }
        return *this;
}
OleHelper &OleHelper::write_box(){
        Variant vBorders = m_vRange.OlePropertyGet("Borders");
        vBorders.OlePropertySet("linestyle",xlContinuous);
        vBorders.OlePropertySet("weight",xlThin);
        vBorders.OlePropertySet("colorindex",xlAutomatic);
        return *this;
}
//data type

static String cstr[]={"��","Ҽ","��","��","��", "��", "½","��","��","��"};
static String wstr[] ={ "", "", "ʰ", "��", "Ǫ", "��", "ʰ", "��", "Ǫ", "��", "ʰ", "��", "Ǫ" };
int SplitString(TStringList *resultStringList,String splittStr,String sourceStr)
{
    resultStringList->Text = "";
    int splitterLength = splittStr.Length();
    int pos = sourceStr.Pos(splittStr);
    String tmp;
    while (pos > 0) {
        tmp = sourceStr.SubString(1, pos - 1);
        resultStringList->Add(tmp);
        sourceStr = sourceStr.SubString(pos + splitterLength, sourceStr.Length() - pos - splitterLength + 1);
        pos = sourceStr.Pos(splittStr);
    }
    resultStringList->Add(sourceStr);
    return resultStringList->Count;
}
String ConvertBigInt(String oldString)
{
    oldString = oldString.Trim();
    TStringList * strList = new TStringList();
    int count = SplitString(strList, ".", oldString);
    if (count == 0 || count > 2) {
        ShowMessage("���ָ�ʽ����ȷ��");
    }
    //��������
    String str = strList->Strings[0];
    if (str.Length() > 12) {
        ShowMessage("����̫�󣬲��ܴ���");
    }

    //������������
    int len = str.Length();
    String tmpstr, rstr;
    rstr="";

    TReplaceFlags   rf;
    rf   <<   rfReplaceAll;
    for (int i = 1; i <= len; i++) {
        tmpstr = str.SubString(len - i + 1, 1);
        rstr = cstr[StrToInt(tmpstr)] + wstr[i] + rstr;
    }

    rstr=StringReplace(rstr, "ʰ��", "ʰ", rf);
    rstr=StringReplace(rstr, "��ʰ", "��", rf);
    rstr=StringReplace(rstr, "���", "��", rf);
    rstr=StringReplace(rstr, "��Ǫ", "��", rf);
    rstr=StringReplace(rstr, "����", "��", rf);
    for (int i = 1; i <= 6; i++) {
        rstr = StringReplace(rstr, "����", "��", rf);
    }
    rstr=StringReplace(rstr, "����", "��", rf);
    rstr = StringReplace(rstr, "����", "��", rf);
    rstr=StringReplace(rstr, "����", "��", rf);
    rstr+="Բ";

        //С������
    if (count == 2) {
        String strXiaoshu = strList->Strings[1];
        int xiaoshuLength = strXiaoshu.Length();
        if (xiaoshuLength == 1) {
            rstr += cstr[StrToInt(strXiaoshu)] + "��";
        } else if (xiaoshuLength >= 2 ) {
            rstr += cstr[StrToInt(strXiaoshu[1])] + "��" + cstr[StrToInt(strXiaoshu[2])] + "��";;
        } else if  (xiaoshuLength == 0) {
            rstr += "��";
        }
    }
    return rstr;
}
//flush
/*
void TFinanceOpForm::Editor2RowUpdateOut(TListItem *pItem){
        if(pItem==NULL)
                return;
	pItem->SubItems->Strings[4] = AnsiString(GetDate(dtpOutDate));
	pItem->SubItems->Strings[5] = edtOutBank->Text;
	pItem->SubItems->Strings[6] = edtOutAccount->Text;
	pItem->SubItems->Strings[7] = edtOutMan->Text;
}

void TOperationForm::Editor2RowAdd(TListItem *pItem){
        if(pItem==NULL)
                return;
        pItem->Caption = edtOId->Text;
	pItem->SubItems->Add(cbbSlName->Text);
	pItem->SubItems->Add(cbbClShortName->Text);
	pItem->SubItems->Add(cbbGoodsName->Text);
	pItem->SubItems->Add(edtLoadingId->Text);
	pItem->SubItems->Add(cbbWay->Text);
	pItem->SubItems->Add(cbbGoodsSrc->Text);
	pItem->SubItems->Add(cbbGoodsDst->Text);
	pItem->SubItems->Add(edtNum->Text);
	pItem->SubItems->Add(edtNetWeight->Text);
	pItem->SubItems->Add(edtVolume->Text);
}
void TOperationForm::Row2Editor(TListItem *pItem){

        if(pItem==NULL)
                return;

        int i = 0;
        edtOId->Text = pItem->Caption;
	locateCbbByValue(cbbSlName, pItem->SubItems->Strings[i++]);
	locateCbbByValue(cbbClShortName, pItem->SubItems->Strings[i++]);
	locateCbbByValue(cbbGoodsName, pItem->SubItems->Strings[i++]);
	edtLoadingId->Text    = pItem->SubItems->Strings[i++];
	locateCbbByValue(cbbWay, pItem->SubItems->Strings[i++]);
	locateCbbByValue(cbbGoodsSrc, pItem->SubItems->Strings[i++]);
	locateCbbByValue(cbbGoodsDst, pItem->SubItems->Strings[i++]);
	edtNum->Text          = pItem->SubItems->Strings[i++];
	edtNetWeight->Text    = pItem->SubItems->Strings[i++];
	edtVolume->Text       = pItem->SubItems->Strings[i++];

        //to indetail\outdetail TODO: load every time??? (imp by cache)
        flushMoneyDetail(edtOId->Text);        

}

*/
#endif
