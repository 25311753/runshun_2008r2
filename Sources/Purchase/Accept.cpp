//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include "Accept.h"
#include "DataModule.h"
#include "BaseCode.h"
#include "LdyInterface.h"
#include <assert.h>
#include "common.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "trayicon"
#pragma resource "*.dfm"
#include <cctype>

TAcceptForm *AcceptForm;
int ColumnToSort;

//g_theOperator.op_class
bool is_op_class_yupei;
#define D_OPCLASS_YUPEI 6
//---------------------------------------------------------------------------
void Accept(int nAuth)
{
        CALL_FORM(TAcceptForm);
}

__fastcall TAcceptForm::TAcceptForm(TComponent* Owner)
        : TForm(Owner)
{
        m_enWorkState=EN_IDLE;
        m_sCidCopy = "";
        m_sContainerCopy = "";

        is_op_class_yupei = g_theOperator.op_class == D_OPCLASS_YUPEI;
}
//---------------------------------------------------------------------------
static bool chkFormatContainerNo(AnsiString ss){
        if (ss.Length()!=11){
                ShowMessage("��Ÿ�ʽ����ӦΪ11");
                return false;
        }
        char cinfo[12];
        memset(cinfo, 0x00, sizeof(cinfo));
        snprintf(cinfo, 11, ss.c_str());


        for (int i=0;i<4; ++i){
                if (!(cinfo[i]>='a' && cinfo[i]<='z') && !(cinfo[i]>='A' && cinfo[i]<='Z')){
                        ShowMessage("��Ÿ�ʽ�Ƿ���ǰ1-4λӦΪӢ����ĸ");
                        ShowMessage(cinfo[i]);
                        return false;
                }
        }
        for (int i=4;i<11; ++i){
                if (!(cinfo[i]>='0' && cinfo[i]<='9')){
                        ShowMessage("��Ÿ�ʽ�Ƿ���5-12λӦΪ����");
                        return false;
                }
        }

        return true;
}

void __fastcall TAcceptForm::FormShow(TObject *Sender)
{
        lb_sum_cnt_con->Caption = "";
        edtContainerNo->Text = "";
        edtQryLicenseNo->Text = "";
        edtCid->Text = "";
        cbbGoodsPerf->Text = "";
        cbbGoodsPerfQry->Text = "";

        edtLading->Text = "";
        edtContainerInfo->Text = "";
        edtSealId->Text = "";

        edtCustfree->Text = "";
        edtCliWorkid->Text = "";
        edtBeiZhu->Text = "";
              edtQryCid->Text = "";
        edtQryLading->Text = "";
        edtQryJZS->Text = "";
         CString szSQL;

        edtBoatNo->Text = "";
/*
        cbbBoatno->ItemIndex = -1;
        cbbBoatno->Text = "";
        szSQL.Format("select * from dicboatno");
        RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{
                cbbBoatno->Items->Add(dm1->Query1->FieldByName("bname")->AsString);
                m_lstBoatno.Add(dm1->Query1->FieldByName("bname")->AsString.c_str());
                dm1->Query1->Next();
        }
*/
        edtBoatOrder->Text = "";
        edtBoatName->Text = "";
/*
        cbbBoatorder->ItemIndex = -1;
        cbbBoatorder->Text = "";
	szSQL.Format("select * from dicboatorder");
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{

                cbbBoatorder->Items->Add(dm1->Query1->FieldByName("boname")->AsString);
                m_lstBoatorder.Add(dm1->Query1->FieldByName("boname")->AsString.c_str());
		dm1->Query1->Next();
        }
*/
        cbbQryStatus->ItemIndex = 0;
        cbbQryStatus->Text = "";
	szSQL.Format("select * from dicstatus");
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{

                cbbQryStatus->Items->Add(dm1->Query1->FieldByName("sname")->AsString);
                m_lstQryStatus.Add(dm1->Query1->FieldByName("sname")->AsString.c_str());
		dm1->Query1->Next();
        }

        cbbStatus->ItemIndex = -1;
        cbbStatus->Text = "";
	szSQL.Format("select * from dicstatus");
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{

                cbbStatus->Items->Add(dm1->Query1->FieldByName("sname")->AsString);
                m_lstStatus.Add(dm1->Query1->FieldByName("sname")->AsString.c_str());
		dm1->Query1->Next();
        }
	szSQL.Format("select * from dicshipagent");
	RunSQL(dm1->Query1,szSQL,true);
	while(!dm1->Query1->Eof)
	{

                cbbShipAgent->Items->Add(dm1->Query1->FieldByName("saname")->AsString);
		dm1->Query1->Next();
        }

        m_lstGoodsperf.Add("����");
        m_lstGoodsperf.Add("��");
        m_lstGoodsperf.Add("������");
//        TDateTime tDate=GetSysTime(false);

//        dtpEndDateYYYYMMDD->DateTime=tDate;
//        TDateTime tDate_default;
//         tDate_default=StrToDateTime("2012-01-01 17:00:00");
//        dtpEndDateHHMM->Time=tDate_default;
        TDateTime tNow=TDateTime::CurrentDateTime();
        dtpEndDateYYYYMMDD->DateTime=tNow;
        dtpEndDateHHMM->Time = EncodeTime(17,00,00,0);
        dtpQryAcceptDate->DateTime=tNow;
        dtpQryAcceptDateEnd->DateTime=tNow;

        m_enWorkState=EN_IDLE;
        ResetCtrl();
        AuthFilter(g_theOperator.op_class,m_enWorkState);
}
//---------------------------------------------------------------------------
#include "ContainerInfo.h"
void __fastcall TAcceptForm::btnInputContanerinfoClick(TObject *Sender)
{
	TContainerInfoForm *pCI;
	pCI=new TContainerInfoForm(this);
	assert(pCI!=NULL);
	pCI->ShowModal();
        //if(pMT->ShowModal()==mrOk)
	{
		edtContainerInfo->Text = pCI->m_szContainerInfo;
	}
	delete pCI;
        
}
//---------------------------------------------------------------------------
//��װ��װ����Ϣ:������|��� ���� ������#��� ���� ������#@��
CString TAcceptForm::GetContainerInfo(){
        CString szContInfo;
        szContInfo += (IntToStr(lstViewContainer->Items->Count)).c_str();
        szContInfo += '|';
        for (int i=0; i<lstViewContainer->Items->Count; ++i) {
                szContInfo += lstViewContainer->Items->Item[i]->Caption.c_str();
                szContInfo += ' ';
                szContInfo += lstViewContainer->Items->Item[i]->SubItems->Strings[0].c_str();
                szContInfo += ' ';
                szContInfo += lstViewContainer->Items->Item[i]->SubItems->Strings[1].c_str();
                szContInfo += '#';
        }
        szContInfo += '@';
        return szContInfo;
}
void __fastcall TAcceptForm::btnAddClick(TObject *Sender)
{
	ResetCtrl();
        lstViewContainer->Clear();
//        edtShipAgent->Text = "";
        edtContainerNo->Text = "";
        edtCid->Text = "";
        cbbGoodsPerf->Text = "";
        cbbGoodsPerfQry->Text = "";

        edtLading->Text = "";
        edtContainerInfo->Text = "";
        edtSealId->Text = "";
        edtCustfree->Text = "";
//        edtShipAgent->Text = "";
        edtCliWorkid->Text = "";
        edtBeiZhu->Text = "";
        edtQryCid->Text = "";
        edtQryLading->Text = "";
        edtQryJZS->Text = "";
        cbbClient->Text = "";
        cbbCustomsCharge->Text = "";

        cbbQryClient->Text = "";
//        cbbBoatno->ItemIndex = -1;
        edtBoatNo->Text = "";
//        cbbBoatorder->ItemIndex = -1;
        edtBoatOrder->Text = "";
        edtBoatName->Text = "";
        cbbQryStatus->ItemIndex = 0;
        cbbStatus->ItemIndex = -1;
        cbbShipAgent->ItemIndex = 0;
        if(edtCid->CanFocus())	edtCid->SetFocus();
        
        m_enWorkState=EN_ADDNEW;
        ResetCtrl();
        edtCid->SetFocus();


}
//---------------------------------------------------------------------------

int TAcceptForm::add(TObject *Sender)
{
        int rt = -1;

        if(edtCid->Text.IsEmpty() || cbbClient->Text.IsEmpty() || edtLading->Text.IsEmpty() || \
                /*edtBoatNo->Text.IsEmpty() || edtBoatOrder->Text.IsEmpty() || edtBoatName->Text.IsEmpty() || */ cbbGoodsPerf->Text.IsEmpty() ||\
              lstViewContainer->Items->Count == 0)
        {
                ShowMessage("�������������Ϣ");
                return rt;
        }
        if(!isNum(edtCid->Text)){
                ShowMessage("������������");
                return rt;
        }

        char strAddSQL[256];
        sprintf(strAddSQL,"select cid as cnt from customs where cid='%s'",edtCid->Text.c_str());
//        RunSQL(strAddSQL,true);
       	RunSQL(dm1->Query1,strAddSQL,true);
	if(!dm1->Query1->Eof)
	{
                ShowMessage("���ݿ������иñ�ŵļ�¼!");
                return rt;
        }

        //gen date
        unsigned short year0,month0,day0,hour0,minute0,sec0,msec0;
	dtpEndDateYYYYMMDD->DateTime.DecodeDate(&year0,&month0,&day0);
     	dtpEndDateHHMM->DateTime.DecodeTime(&hour0,&minute0,&sec0,&msec0);
        char strDate0[80];
   	sprintf(strDate0,"%04d-%02d-%02d %02d:%02d",year0,month0,day0,hour0,minute0,sec0,msec0);

        CString szSQL;
        szSQL="insert into customs(cid,client,ladingid,containerinfo,sealid,bnname,boname,endcustdate,\
                        custfree,cliworkid,beizhu,goodsperf,status,acceptdate,boatno,boatorder,acceptor,shipagent,boatname)\
                        values(";
        szSQL += Str2DBString(edtCid->Text.c_str());
        szSQL +=","; szSQL += Str2DBString(cbbClient->Text.c_str());
        szSQL +=","; szSQL += Str2DBString(edtLading->Text.c_str());
        szSQL +=","; szSQL += Str2DBString(GetContainerInfo());//edtContainerInfo->Text.c_str());
//        szSQL +=","; szSQL += Str2DBString(edtSealId->Text.c_str());
        szSQL +=","; szSQL += Str2DBString(lstViewContainer->Items->Item[0]->SubItems->Strings[1].c_str());
        szSQL +=","; szSQL += Str2DBString(edtBoatNo->Text.c_str());
        szSQL +=","; szSQL += Str2DBString(edtBoatOrder->Text.c_str());
        //szSQL +=","; szSQL += Str2DBString(edtEndDate->Text.c_str());
        szSQL +=","; szSQL += Str2DBString(strDate0);
        szSQL +=","; szSQL += Text2DBFloat(edtCustfree->Text.IsEmpty()?AnsiString("0"):edtCustfree->Text,4).c_str();//Str2DBString(edtCustfree->Text.c_str());
        edtCustfree->Text = Text2DBFloat(edtCustfree->Text.IsEmpty()?AnsiString("0"):edtCustfree->Text,4).c_str();
        szSQL +=","; szSQL += Str2DBString(edtCliWorkid->Text.c_str());
        szSQL +=","; szSQL += Str2DBString(edtBeiZhu->Text.c_str());
        szSQL +=","; szSQL += Str2DBString(cbbGoodsPerf->Text.c_str());
        szSQL +=","; szSQL += Str2DBString("�ѽӵ�");
//        szSQL +=","; szSQL += Str2DBString(GetSysDate());
        szSQL +=",getdate()";
        szSQL +=","; szSQL += Str2DBString(edtBoatNo->Text.c_str());
        szSQL +=","; szSQL += Str2DBString(edtBoatOrder->Text.c_str());
        szSQL +=","; szSQL += Str2DBString(g_theOperator.op_name);
        szSQL +=","; szSQL += Str2DBString(cbbShipAgent->Text.c_str());
        szSQL +=","; szSQL += Str2DBString(edtBoatName->Text.c_str());        
        szSQL +=")";

//        lbdebug->Caption = AnsiString(szSQL);
        if(!RunSQL(dm1->Query1,szSQL))
        {
                ShowMessage("insert fail!") ;

                return rt;
        }

        int column_no = lstViewDown->Items->Count;
        int cnt_con = lstViewContainer->Items->Count;
        TListItem *pItem =lstViewDown->Items->Add();
        assert(pItem!=NULL);
        pItem->Caption=(column_no+1);
        pItem->SubItems->Add(edtCid->Text);
        pItem->SubItems->Add(cbbGoodsPerf->Text);
        pItem->SubItems->Add(AnsiString(GetSysDate()));
        pItem->SubItems->Add(cbbClient->Text);
        pItem->SubItems->Add(edtLading->Text);
        //pItem->SubItems->Add(AnsiString(GetContainerInfo()));
        pItem->SubItems->Add(lstViewContainer->Items->Item[0]->Caption);
        pItem->SubItems->Add(IntToStr(cnt_con));
        pItem->SubItems->Add("");
        pItem->SubItems->Add(edtBoatNo->Text);
        pItem->SubItems->Add(edtBoatOrder->Text);        
        pItem->SubItems->Add(strDate0);
        pItem->SubItems->Add("�ѽӵ�");
        pItem->SubItems->Add("");       //��֤������ʱ����������������
        pItem->SubItems->Add(edtCliWorkid->Text);
        pItem->SubItems->Add(edtCustfree->Text);
        pItem->SubItems->Add("");
        pItem->SubItems->Add(g_theOperator.op_name);
        pItem->SubItems->Add("");       //doer
        pItem->SubItems->Add(edtBeiZhu->Text);
        pItem->SubItems->Add(cbbShipAgent->Text.c_str());
        pItem->SubItems->Add(edtBoatName->Text.c_str());
        pItem->SubItems->Add(AnsiString(GetContainerInfo()));
        flushSum();
        rt = 0;
        return rt;
}
void TAcceptForm::refreshAdd()
{
}
void TAcceptForm::refreshMod()
{
        unsigned short year0,month0,day0;
	dtpQryAcceptDate->DateTime.DecodeDate(&year0,&month0,&day0);
        char strDate0[80];

   	sprintf(strDate0,"%04d%02d%02d",year0,month0,day0);

        unsigned short year1,month1,day1;
	dtpQryAcceptDateEnd->DateTime.DecodeDate(&year1,&month1,&day1);
        char strDate1[80];

   	sprintf(strDate1,"%04d%02d%02d",year1,month1,day1);

        CString szSQL;
        szSQL="select *,CONVERT(varchar(10), acceptdate, 23) as ad, substring(containerinfo,0,charindex('|',containerinfo)) as cnt_con \
                from customs where 1=1 ";
        if (!edtQryCid->Text.IsEmpty()){
                szSQL +=" and cid="; szSQL += Str2DBString(edtQryCid->Text.c_str());
        }
        TListItem *pItem  = lstViewDown->Selected;
	RunSQL(dm1->Query1,szSQL,true);
//return;
	while(!dm1->Query1->Eof)
	{
//                pItem->Caption=dm1->Query1->FieldByName("cid")->AsString;
                pItem->SubItems->Strings[0] = dm1->Query1->FieldByName("cid")->AsString;
	        pItem->SubItems->Strings[1] = dm1->Query1->FieldByName("goodsperf")->AsString;
		pItem->SubItems->Strings[2] = dm1->Query1->FieldByName("ad")->AsString;
		pItem->SubItems->Strings[3] = dm1->Query1->FieldByName("client")->AsString;
		pItem->SubItems->Strings[4] = dm1->Query1->FieldByName("ladingid")->AsString;
		pItem->SubItems->Strings[5] = AnsiString(getFirstContainerNo(dm1->Query1->FieldByName("containerinfo")->AsString));
		pItem->SubItems->Strings[6] = dm1->Query1->FieldByName("cnt_con")->AsString;
		pItem->SubItems->Strings[7] = dm1->Query1->FieldByName("licenseno")->AsString;
		pItem->SubItems->Strings[8] = dm1->Query1->FieldByName("boatno")->AsString;
		pItem->SubItems->Strings[9] = dm1->Query1->FieldByName("boatorder")->AsString;
		pItem->SubItems->Strings[10] = dm1->Query1->FieldByName("endcustdate")->AsString;
		pItem->SubItems->Strings[11] = dm1->Query1->FieldByName("status")->AsString;
		pItem->SubItems->Strings[12] = dm1->Query1->FieldByName("tag_dzclz_date")->AsString;
		pItem->SubItems->Strings[13] = dm1->Query1->FieldByName("cliworkid")->AsString;
		pItem->SubItems->Strings[14] = dm1->Query1->FieldByName("custfree")->AsString;
		pItem->SubItems->Strings[15] = dm1->Query1->FieldByName("sealid")->AsString;
		pItem->SubItems->Strings[16] = dm1->Query1->FieldByName("acceptor")->AsString;
		pItem->SubItems->Strings[17] = dm1->Query1->FieldByName("doer")->AsString;
		pItem->SubItems->Strings[18] = dm1->Query1->FieldByName("beizhu")->AsString;
                pItem->SubItems->Strings[19] = dm1->Query1->FieldByName("shipagent")->AsString;
                pItem->SubItems->Strings[20] = dm1->Query1->FieldByName("containerinfo")->AsString;
                lstViewContainer->Items->Clear();
                flushContainer(AnsiString(pItem->SubItems->Strings[20].c_str()));
		dm1->Query1->Next();
	}
        flushSum();

}
void __fastcall TAcceptForm::btnQueryClick(TObject *Sender)
{

        unsigned short year0,month0,day0;
	dtpQryAcceptDate->DateTime.DecodeDate(&year0,&month0,&day0);
        char strDate0[80];

   	sprintf(strDate0,"%04d%02d%02d",year0,month0,day0);

        unsigned short year1,month1,day1;
	dtpQryAcceptDateEnd->DateTime.DecodeDate(&year1,&month1,&day1);
        char strDate1[80];

   	sprintf(strDate1,"%04d-%02d-%02d 23:59:59",year1,month1,day1);

        CString szSQL;
        szSQL="select *,CONVERT(varchar(10), acceptdate, 23) as ad, substring(containerinfo,0,charindex('|',containerinfo)) as cnt_con \
                from customs where 1=1 ";
        if (!edtQryCid->Text.IsEmpty()){
                szSQL +=" and cid="; szSQL += Str2DBString(edtQryCid->Text.c_str());
        }
        if (!cbbQryClient->Text.IsEmpty()) {
                szSQL += " and client="; szSQL += Str2DBString(cbbQryClient->Text.c_str());
        }
        if (!edtQryLading->Text.IsEmpty())   {
                szSQL += " and ladingid="; szSQL += Str2DBString(edtQryLading->Text.c_str());
        }
        if (!cbbQryStatus->Text.IsEmpty()){
                szSQL += " and status="; szSQL += Str2DBString(cbbQryStatus->Text.c_str());
        }
        if (!edtQryJZS->Text.IsEmpty()){
                szSQL += " and containerinfo like'%"; szSQL += edtQryJZS->Text.c_str(); szSQL+="%'";
        }
        if (!edtQryLicenseNo->Text.IsEmpty()){
                szSQL += " and licenseno="; szSQL += Str2DBString(edtQryLicenseNo->Text.c_str());
        }
        if (!cbbGoodsPerfQry->Text.IsEmpty()){
                szSQL += " and goodsperf="; szSQL += Str2DBString(cbbGoodsPerfQry->Text.c_str());
        }

        if (cbIsQryByDate->Checked) {
                szSQL += " and acceptdate between "; szSQL += Str2DBString(strDate0);
                szSQL += " and ";
                szSQL += Str2DBString(strDate1);
        }
//        szSQL += " order by CONVERT(varchar(100), acceptdate, 23), endcustdate";   //accpetdate��ʽ��yyyymmdd��������endcustdate�����ܸ���
        szSQL += " order by acceptdate";
        
        TListItem *pItem;
        lstViewDown->Items->Clear();
	RunSQL(dm1->Query1,szSQL,true);

        if (dm1->Query1->Eof){
                ShowMessage("��¼������");
                return;
        }

        int column_no = 0;
	while(!dm1->Query1->Eof)
	{
                column_no++;
                pItem=lstViewDown->Items->Add();
                pItem->Caption= column_no;
                pItem->SubItems->Add(dm1->Query1->FieldByName("cid")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("goodsperf")->AsString);
//		pItem->SubItems->Add(dm1->Query1->FieldByName("ad")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("acceptdate")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("client")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("ladingid")->AsString);
		//pItem->SubItems->Add(dm1->Query1->FieldByName("containerinfo")->AsString);
                pItem->SubItems->Add(AnsiString(getFirstContainerNo(dm1->Query1->FieldByName("containerinfo")->AsString)));
                pItem->SubItems->Add(dm1->Query1->FieldByName("cnt_con")->AsString);
                pItem->SubItems->Add(dm1->Query1->FieldByName("licenseno")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("boatno")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("boatorder")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("endcustdate")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("status")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("tag_dzclz_date")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("cliworkid")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("custfree")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("sealid")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("acceptor")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("doer")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("beizhu")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("shipagent")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("boatname")->AsString);

                pItem->SubItems->Add(dm1->Query1->FieldByName("containerinfo")->AsString);
                lstViewContainer->Items->Clear();

		dm1->Query1->Next();
	}

       	ResetCtrl();
        //clear up area
        edtQryCid->Text 				= "";
        edtCid->Text 						= "";
        cbbGoodsPerf->ItemIndex	= -1;
        cbbClient->Text					= "";
        cbbCustomsCharge->Text					= "";

//        cbbBoatno->ItemIndex		= -1;
        edtBoatNo->Text = "";
//        cbbBoatorder->ItemIndex	= -1;
        edtBoatOrder->Text = "";
        edtBoatName->Text = "";
        cbbStatus->ItemIndex		= -1;
        edtLading->Text 				= "";
        edtContainerInfo->Text 	= "";
        edtCliWorkid->Text 			= "";
        edtCustfree->Text 			= "";
        edtSealId->Text 				= "";
        edtBeiZhu->Text					= "";
        cbbShipAgent->ItemIndex	= -1;
        edtContainerNo->Text		= "";
        edtSealId->Text					= "";
        lstViewContainer->Clear();
        flushSum();
        //~
}
//---------------------------------------------------------------------------
void TAcceptForm::flushContainer(AnsiString c){
        lstViewContainer->Items->Clear();

		char cnt[10];memset(cnt,sizeof(cnt),0x00);
		char body[2048];memset(body,sizeof(body),0x00);
		sscanf(c.c_str(),"%[^|]|",cnt);
		sscanf(c.c_str(),"%*[^|]|%[^@]",body);

		char strTmp1[2048];
		memset(strTmp1,sizeof(strTmp1),0x00);
		strcpy(strTmp1,body);
                int num = StrToInt(cnt);
		for (int i =0; i<num; ++i){
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

                        TListItem *pItem =lstViewContainer->Items->Add();
                        assert(pItem!=NULL);
                        pItem->Caption=AnsiString(str3);
                        pItem->SubItems->Add(AnsiString(str4));
                        pItem->SubItems->Add(AnsiString(strSealId));

		}


        if (lstViewContainer->Items->Count>0){
                edtContainerNo->Text = lstViewContainer->Items->Item[0]->Caption;
                cbbContainerType->ItemIndex=cbbContainerType->Items->IndexOf(lstViewContainer->Items->Item[0]->SubItems->Strings[0]);
                edtSealId->Text = lstViewContainer->Items->Item[0]->SubItems->Strings[1];
        }

}
//should be common
CString TAcceptForm::getFirstContainerNo(AnsiString c)
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
void __fastcall TAcceptForm::lstViewDownSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
  if(Selected!=NULL){
//        ShowMessage("lstViewDownSelectItem");
        edtQryCid->Text = Item->SubItems->Strings[0];//Item->Caption;
        edtCid->Text = Item->SubItems->Strings[0];//Item->Caption;

        cbbGoodsPerf->ItemIndex=cbbGoodsPerf->Items->IndexOf(Item->SubItems->Strings[1]);
        cbbGoodsPerfQry->ItemIndex=cbbGoodsPerf->Items->IndexOf(Item->SubItems->Strings[1]) +1;
        cbbClient->Text=Item->SubItems->Strings[3];
        cbbCustomsCharge->Text = AnsiString(getCustomsCharge(cbbClient->Text.c_str()));
        edtBoatNo->Text = Item->SubItems->Strings[8];
        edtBoatOrder->Text = Item->SubItems->Strings[9];
        cbbStatus->ItemIndex=cbbStatus->Items->IndexOf(Item->SubItems->Strings[11]);
        AnsiString status = Item->SubItems->Strings[11];
        edtLading->Text =  Item->SubItems->Strings[4].c_str();
        edtContainerInfo->Text = Item->SubItems->Strings[18].c_str(); //�������ã���ɾ����
        edtCliWorkid->Text =   Item->SubItems->Strings[13].c_str();
        edtCustfree->Text = Item->SubItems->Strings[14].c_str();
        edtSealId->Text = Item->SubItems->Strings[15].c_str();
        edtBeiZhu->Text=Item->SubItems->Strings[18].c_str();
        cbbShipAgent->ItemIndex=cbbShipAgent->Items->IndexOf(Item->SubItems->Strings[19]);
        edtBoatName->Text = Item->SubItems->Strings[20].c_str();

        flushContainer(AnsiString(Item->SubItems->Strings[21].c_str()));
        TDateTime tDate;
        tDate=StrToDateTime(Item->SubItems->Strings[10].c_str());
        dtpEndDateYYYYMMDD->DateTime=tDate;
        dtpEndDateHHMM->Time=tDate;
  }
        ResetCtrl();
}
//---------------------------------------------------------------------------

void __fastcall TAcceptForm::btnDelClick(TObject *Sender)
{
        char strMsg[256],strSQL[512];
        sprintf(strMsg,"\n  ��Ҫɾ�����Ϊ��%s���ļ�¼��  \n",edtCid->Text.c_str());
        if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
                return;

        CString szSQL;
        szSQL.Format("delete customs where cid=%s", edtCid->Text);
        if(!RunSQL(dm1->Query1,szSQL))
                ShowMessage("delete fail!");
        //btnQueryClick(Sender);
        TListItem *pItem;
        pItem=lstViewDown->Selected;
        if(pItem!=NULL)
        {
                int nSel=lstViewDown->Items->IndexOf(pItem);
                lstViewDown->Items->Delete(nSel);
                lstViewDown->Selected=NULL;
                flushSum();
        }
        ShowMessage("ɾ���ɹ�");
}
//---------------------------------------------------------------------------

void __fastcall TAcceptForm::btnModClick(TObject *Sender)
{
  m_enWorkState=EN_EDIT;
  ResetCtrl();
//  edtCid->SetFocus();
}
//---------------------------------------------------------------------------

void TAcceptForm::mod(TObject *Sender)
{
        if (lstViewContainer->Items->Count == 0){
                ShowMessage("�����Ϣ��ȫ");
                return;
        }
 char strMsg[256],strSQL[512];
  sprintf(strMsg,"\n  ��Ҫ�޸ı��Ϊ��%s���ļ�¼��  \n",edtCid->Text.c_str());
  if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
   return;

unsigned short year0,month0,day0,hour0,minute0,sec0,msec0;
	dtpEndDateYYYYMMDD->DateTime.DecodeDate(&year0,&month0,&day0);
     	dtpEndDateHHMM->DateTime.DecodeTime(&hour0,&minute0,&sec0,&msec0);
   char strDate0[80];
   	//sprintf(strDate0,"%04d%02d%02d",year0,month0,day0);
        sprintf(strDate0,"%04d-%02d-%02d %02d:%02d",year0,month0,day0,hour0,minute0,sec0,msec0);
        CString szSQL;
        szSQL="update customs set ";

szSQL +="client="; szSQL += Str2DBString(cbbClient->Text.c_str());
szSQL +=",ladingid="; szSQL += Str2DBString(edtLading->Text.c_str());
szSQL +=",containerinfo="; szSQL += Str2DBString(GetContainerInfo());
szSQL +=",sealid="; szSQL += Str2DBString(lstViewContainer->Items->Item[0]->SubItems->Strings[1].c_str());
szSQL +=" , boatno="; szSQL += Str2DBString(edtBoatNo->Text.c_str());
szSQL +=" , boatorder="; szSQL += Str2DBString(edtBoatOrder->Text.c_str());
//szSQL +=","; szSQL += Str2DBString(edtEndDate->Text.c_str());
szSQL +=", endcustdate="; szSQL += Str2DBString(strDate0);

szSQL +=" , custfree="; szSQL += Text2DBFloat(edtCustfree->Text.IsEmpty()?AnsiString("0"):edtCustfree->Text,4).c_str();
edtCustfree->Text = Text2DBFloat(edtCustfree->Text.IsEmpty()?AnsiString("0"):edtCustfree->Text,4).c_str();
szSQL +=" , cliworkid="; szSQL += Str2DBString(edtCliWorkid->Text.c_str());
szSQL +=" , beizhu="; szSQL += Str2DBString(edtBeiZhu->Text.c_str());
szSQL +=",goodsperf="; szSQL += Str2DBString(cbbGoodsPerf->Text.c_str());
szSQL +=",status="; szSQL += Str2DBString(cbbStatus->Text.c_str());
//szSQL +=",acceptdate="; szSQL += Str2DBString(GetSysDate());
//szSQL +=",acceptor="; szSQL += Str2DBString(g_theOperator.op_name);           //del 0511 ȡ��ÿ���޸ĸ��½ӵ���
szSQL +=",shipagent="; szSQL += Str2DBString(cbbShipAgent->Text.c_str());
szSQL +=",boatname="; szSQL += Str2DBString(edtBoatName->Text.c_str());

szSQL += " where cid="; szSQL+=Str2DBString(edtCid->Text.c_str());


//        Edit1->Text = AnsiString(szSQL);
        if(!RunSQL(dm1->Query1,szSQL))
        {
                ShowMessage("update fail!") ;

                return;
        }

//        lstViewDown->Items->Clear();
//        btnQueryClick(Sender);

        ShowMessage("�޸ĳɹ�");
}
bool TAcceptForm::isDupContainer(AnsiString c, AnsiString &dupCid){
        CString szSQL;
        szSQL.Format("select cid from customs where containerinfo like '%%%s %%'", c);
//        ShowMessage(AnsiString(szSQL));
        RunSQL(dm1->Query1,szSQL,true);
        if (!dm1->Query1->Eof) {
                dupCid = AnsiString(dm1->Query1->FieldByName("cid")->AsString);
                return true;
        }
        return false;
}

void __fastcall TAcceptForm::btnAddContainerClick(TObject *Sender)
{
  edtContainerNo->Text = Trim(edtContainerNo->Text);
  if(edtContainerNo->Text.IsEmpty() || edtSealId->Text.IsEmpty())
  {
    ShowMessage("��������źͷ�����");
    if(edtContainerNo->CanFocus())	edtContainerNo->SetFocus();
    return;
  }
  if (!chkFormatContainerNo(edtContainerNo->Text)){
        return;
  }
       //�絥�ظ���ż��
       AnsiString dupCid="";
      if (isDupContainer(AnsiString(edtContainerNo->Text.c_str()), dupCid)) {
                char strMsg[256],strSQL[512];
                sprintf(strMsg,"\n  [ %s ]������ͬ��š�%s��, ȷ���������?\n", dupCid, edtContainerNo->Text.c_str());
                if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
                return;
      }
  //�����ظ���ż��
  for(int i=0;i<lstViewContainer->Items->Count;i++) {
      TListItem *pItem;
      pItem = lstViewContainer->Items->Item[i];
      if (pItem->Caption == edtContainerNo->Text) {
                char strMsg[256],strSQL[512];
                sprintf(strMsg,"\n  ��Ҫ����ظ���š�%s����  \n",edtContainerNo->Text.c_str());
                if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
                return;
      }
  }

	TListItem *pItem =lstViewContainer->Items->Add();
        assert(pItem!=NULL);
        pItem->Caption=edtContainerNo->Text;
        pItem->SubItems->Add(cbbContainerType->Text);
        pItem->SubItems->Add(edtSealId->Text);

  edtContainerNo->Text = "";
  edtSealId->Text = "";

}
//---------------------------------------------------------------------------

void __fastcall TAcceptForm::lstViewContainerSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
        edtContainerNo->Text = Item->Caption;
        cbbContainerType->ItemIndex=cbbContainerType->Items->IndexOf(Item->SubItems->Strings[0]);
        edtSealId->Text = Item->SubItems->Strings[1];
}
//---------------------------------------------------------------------------



void __fastcall TAcceptForm::btnModContainerClick(TObject *Sender)
{

  if(lstViewContainer->Selected == NULL)
  {
    ShowMessage("��ѡ���¼");
    if(edtContainerNo->CanFocus())	edtContainerNo->SetFocus();
    return;
  }
  for(int i=0;i<lstViewContainer->Items->Count;i++) {
      TListItem *pItem;
      pItem = lstViewContainer->Items->Item[i];
      if (pItem!=lstViewContainer->Selected && pItem->Caption == edtContainerNo->Text) {
                char strMsg[256],strSQL[512];
                sprintf(strMsg,"\n ��š�%s�����ظ�,�����޸���  \n",edtContainerNo->Text.c_str());
                if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
                return;
      }
  }
  if (!chkFormatContainerNo(edtContainerNo->Text)){
        return;
  }
        TListItem *pItem;
        pItem = lstViewContainer->Selected;
        if (pItem!=NULL){
                pItem->Caption = edtContainerNo->Text;
                pItem->SubItems->Strings[0] = cbbContainerType->Text;
                pItem->SubItems->Strings[1] = edtSealId->Text;

        }
        
}
//---------------------------------------------------------------------------

void __fastcall TAcceptForm::btnDelContainerClick(TObject *Sender)
{
  if(lstViewContainer->Selected == NULL)
  {
    ShowMessage("��ѡ���¼");
    if(edtContainerNo->CanFocus())	edtContainerNo->SetFocus();
    return;
  }
          lstViewContainer->Selected->Delete();        
}
//---------------------------------------------------------------------------



void __fastcall TAcceptForm::edtQryCidKeyPress(TObject *Sender, char &Key)
{
    if (Key==13) //enter key been pressed
    {
        Key=0;
        btnQueryClick(Sender);
    }        
}
//---------------------------------------------------------------------------




void __fastcall TAcceptForm::btnCancelClick(TObject *Sender)
{
  int nState;

  nState=m_enWorkState;
  m_enWorkState=EN_IDLE;
  ResetCtrl();
 lstViewDown->Selected=NULL;
//  Row2Editor();

  switch(nState)
  { //����ȷ����Ĺ��λ��
    case EN_ADDNEW:
        if (btnAdd->CanFocus())
                btnAdd->SetFocus();
        break;
    case EN_EDIT:
        if (btnMod->CanFocus())
                btnMod->SetFocus();
        break;
    default:  break;
  }
      btnMod->Enabled=false;
    btnDel->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TAcceptForm::btnOKClick(TObject *Sender)
{
  char strName[80],*ptr,strTemp[80];
  int nState;
  int oldState=m_enWorkState;
  switch(m_enWorkState)
  {
    case EN_ADDNEW:
      if (-1 == add(Sender))
        return;
      break;
    case EN_EDIT:
       mod(Sender);
      break;
    default:
      ShowMessage("Work State not AddNew or Edit");
      return;
  }
  TListItem *pItem;
  if(m_enWorkState==EN_ADDNEW)
  {
//     pItem=lstViewdown->Items->Add();
//    pItem->Caption=edtName->Text;
//    lstViewdown->Selected=pItem;
  }
  else if(m_enWorkState==EN_EDIT)
  {
        refreshMod();
  }
  m_enWorkState=EN_IDLE;
  ResetCtrl();
      btnMod->Enabled=false;
    btnDel->Enabled=false;
    if(oldState==EN_EDIT){
       lstViewDown->Selected=NULL;
    }

}
//---------------------------------------------------------------------------
void TAcceptForm::AuthFilter(int op_class, int work_state){
        switch (op_class){
                case D_OPCLASS_YUPEI:
                        //skip client custfree
                        lstViewDown->Columns->Items[4]->MaxWidth=1;
                        lstViewDown->Columns->Items[4]->Width=0;

                        lstViewDown->Columns->Items[15]->MaxWidth=1;
                        lstViewDown->Columns->Items[15]->Width=0;

                        btnCopy->Enabled    = !is_op_class_yupei;
                        //always not visiable
                        cbbClient->Visible = !is_op_class_yupei;
                        edtCustfree->Visible = !is_op_class_yupei;
                        cbbCustomsCharge->Visible = !is_op_class_yupei;
                        if (work_state != EN_IDLE){
                                        //TRUE
                                    EnableEdit(edtBoatNo,true);
                                    EnableEdit(edtBoatOrder,true);
                                    EnableEdit(edtBoatName,true);
                                        //FASLSE
                                    EnableCombo(cbbClient,false);
                                    EnableEdit(edtLading,false);
                                    EnableEdit(edtCustfree,false);
                                    EnableCombo(cbbShipAgent,false);
                                    EnableEdit(edtCliWorkid,false);
                                    EnableCombo(cbbGoodsPerf,false);
                                    dtpEndDateYYYYMMDD->Enabled = false;
                                    dtpEndDateHHMM->Enabled = false;
                                    EnableEdit(edtBeiZhu,false);
                                    //container all disable
                                    EnableEdit(edtContainerNo,false);
                                    EnableCombo(cbbContainerType,false);
                                    EnableEdit(edtSealId,false);
                                    btnAddContainer->Enabled=false;
                                    btnModContainer->Enabled=false;
                                    btnDelContainer->Enabled=false;
                        }
                        break;
                default:
                        break;
        }
}
void TAcceptForm::ResetCtrl(){

  
  if(m_enWorkState==EN_IDLE)
  {
    bool isSelected = (lstViewDown->Selected!=NULL);

    lstViewDown->Enabled = true;

    //btn
    //Ԥ����Աֻ���޸�Ȩ��
    btnAdd->Enabled     =!is_op_class_yupei && true;
    btnMod->Enabled     =isSelected;
    btnDel->Enabled     =!is_op_class_yupei && isSelected;
    btnOK->Enabled      =false;
    btnCancel->Enabled  =false;
    btnPaste->Enabled   =!is_op_class_yupei && false;

    //edt all disable
    EnableEdit(edtCid,false);
    EnableCombo(cbbClient,false);
    EnableEdit(edtLading,false);
    EnableEdit(edtBoatNo,false);
    EnableEdit(edtBoatOrder,false);
    EnableEdit(edtBoatName,false);
    EnableEdit(edtCustfree,false);
    EnableCombo(cbbShipAgent,false);
    EnableEdit(edtCliWorkid,false);
    EnableCombo(cbbGoodsPerf,false);
    dtpEndDateYYYYMMDD->Enabled = false;
    dtpEndDateHHMM->Enabled = false;
    EnableEdit(edtBeiZhu,false);
    //container all disable
    EnableEdit(edtContainerNo,false);
    EnableCombo(cbbContainerType,false);
    EnableEdit(edtSealId,false);
    btnAddContainer->Enabled=false;
    btnModContainer->Enabled=false;
    btnDelContainer->Enabled=false;
    //qry area
    btnQuery->Enabled = true;
    EnableEdit(edtQryCid,true);
    EnableCombo(cbbQryClient,true);
    EnableEdit(edtQryLading,true);
    EnableCombo(cbbQryStatus,true);

    EnableEdit(edtQryJZS,true);
    EnableEdit(edtQryLicenseNo,true);
    cbIsQryByDate->Enabled = true;
    dtpQryAcceptDate->Enabled = true;
    dtpQryAcceptDateEnd->Enabled = true;
    EnableCombo(cbbGoodsPerfQry,true);


    

  }
  else
  {
    //btn
    btnAdd->Enabled     =!is_op_class_yupei && false;
    btnMod->Enabled     =false;
    btnDel->Enabled     =!is_op_class_yupei && false;
    btnOK->Enabled      =true;
    btnCancel->Enabled  =true;
    if (m_enWorkState==EN_ADDNEW){
            btnPaste->Enabled=!is_op_class_yupei && true;
    }

    //lstview
        lstViewDown->Enabled=false;

    //edt all enable
    EnableEdit(edtCid,m_enWorkState==EN_ADDNEW);
    EnableCombo(cbbClient,true);
    EnableEdit(edtLading,true);
    EnableEdit(edtBoatNo,true);
    EnableEdit(edtBoatOrder,true);
    EnableEdit(edtBoatName,true);    
    EnableEdit(edtCustfree,true);
    EnableCombo(cbbShipAgent,true);
    EnableEdit(edtCliWorkid,true);
    EnableCombo(cbbGoodsPerf,true);
    dtpEndDateYYYYMMDD->Enabled = true;
    dtpEndDateHHMM->Enabled = true;
    EnableEdit(edtBeiZhu,true);
    //container all disable
    EnableEdit(edtContainerNo,true);
    EnableCombo(cbbContainerType,true);
    EnableEdit(edtSealId,true);
    btnAddContainer->Enabled=true;
    btnModContainer->Enabled=true;
    btnDelContainer->Enabled=true;
    //qry area
    btnQuery->Enabled = false;
    EnableEdit(edtQryCid,false);
    EnableCombo(cbbQryClient,false);
    EnableEdit(edtQryLading,false);
    EnableCombo(cbbQryStatus,false);
    EnableEdit(edtQryJZS,false);
    EnableEdit(edtQryLicenseNo,false);
    cbIsQryByDate->Enabled = false;
    dtpQryAcceptDate->Enabled = false;
    dtpQryAcceptDateEnd->Enabled = false;
    EnableCombo(cbbGoodsPerfQry,false);
  }
  AuthFilter(g_theOperator.op_class,m_enWorkState);
}

void __fastcall TAcceptForm::cbbQryClientChange(TObject *Sender)
{
        cbbQryClient->Items->Clear();
        if (cbbQryClient->Text.IsEmpty()) {
                return;
        }
        cbbQryClient->Items->Clear();
        CString szSQL;
	szSQL.Format("select shortname from client where shortname like '%%%s%%'", cbbQryClient->Text.c_str());
//        edtDebug->Text = AnsiString(szSQL);
	RunSQL(dm1->Query1,szSQL,true);

	while(!dm1->Query1->Eof)
	{

                cbbQryClient->Items->Add(dm1->Query1->FieldByName("shortname")->AsString);
		dm1->Query1->Next();
        }
}
//---------------------------------------------------------------------------



void __fastcall TAcceptForm::cbbClientSelect(TObject *Sender)
{
        int nSel = cbbClient->ItemIndex;
        if (nSel == -1)
                return;
//        cbbCustomsCharge->ItemIndex = nSel; //move to onExit
}
//---------------------------------------------------------------------------

void __fastcall TAcceptForm::cbbQryClientSelect(TObject *Sender)
{
        int nSel = cbbQryClient->ItemIndex;
        if (nSel == -1)
                return;        
}
//---------------------------------------------------------------------------





void __fastcall TAcceptForm::cbbQryClient1Change(TObject *Sender)
{
        cbbQryClient->Items->Clear();
        if (cbbQryClient->Text.IsEmpty()) {
                return;
        }
//        cbbQryClient->Items->Clear();

        cbbQryClient->SelStart = AnsiString(cbbQryClient->Text).Length();
        CString szSQL;
	szSQL.Format("select shortname from client where shortname like '%%%s%%'", cbbQryClient->Text.c_str());
//        edtDebug->Text = AnsiString(szSQL);
	RunSQL(dm1->Query1,szSQL,true);

	while(!dm1->Query1->Eof)
	{

                cbbQryClient->Items->Add(dm1->Query1->FieldByName("shortname")->AsString);
		dm1->Query1->Next();
        }        
}
//---------------------------------------------------------------------------

void __fastcall TAcceptForm::cbbQryClient1Select(TObject *Sender)
{
        int nSel = cbbQryClient->ItemIndex;
        if (nSel == -1)
                return;
}
//---------------------------------------------------------------------------














void __fastcall TAcceptForm::Button1Click(TObject *Sender)
{
         this->TrayIcon1->Minimize();
        
}
//---------------------------------------------------------------------------

void __fastcall TAcceptForm::cbbClientChange(TObject *Sender)
{
        cbbClient->Items->Clear();
        cbbCustomsCharge->Items->Clear();

        AnsiString val = cbbClient->Text;
        if (cbbClient->Text.IsEmpty()) {
                return;
        }
        cbbClient->SelStart = AnsiString(cbbClient->Text).Length();
        CString szSQL;
	szSQL.Format("select shortname,customs_charge from client where shortname like '%%%s%%'", cbbClient->Text.c_str());
//        Edit1->Text = AnsiString(szSQL);
	RunSQL(dm1->Query1,szSQL,true);

	while(!dm1->Query1->Eof)
	{

//                cbbClient->Items->Add(dm1->Query1->FieldByName("fullname")->AsString);
                cbbClient->Items->Add(dm1->Query1->FieldByName("shortname")->AsString); //0107
                cbbCustomsCharge->Items->Add(dm1->Query1->FieldByName("customs_charge")->AsString); //0107

		dm1->Query1->Next();
        }
        
        cbbClient->Text = val;
//        cmbclass->ItemIndex=sclass->IndexOf(dm1->OperTab->FieldByName("op_class")->AsInteger);
//        cbbCustomsCharge->ItemIndex=cbbClient->Items->IndexOf(cbbClient->Text.c_str());
//        cbbCustomsCharge->ItemIndex = cbbClient->ItemIndex;
}

void  TAcceptForm::flushSum()
{
        TListItem *pItem;
        int sum_cnt_con=0;
        int i=0;
        for(i=0;i<lstViewDown->Items->Count;i++)
        {
                pItem=lstViewDown->Items->Item[i];
                sum_cnt_con += StrToInt(pItem->SubItems->Strings[6]);
        }
        lb_sum_cnt_con->Caption = IntToStr(sum_cnt_con);
}
//---------------------------------------------------------------------------






















void __fastcall TAcceptForm::cbbClientExit(TObject *Sender)
{
//cbbCustomsCharge->ItemIndex=cbbClient->Items->IndexOf(cbbClient->Text.c_str());
        cbbCustomsCharge->Text = AnsiString(getCustomsCharge(cbbClient->Text.c_str()));
}
//---------------------------------------------------------------------------
CString TAcceptForm::getCustomsCharge(CString client){
        CString rt="";
        CString szSQL;
	szSQL.Format("select customs_charge from client where shortname = '%s'", client);
	RunSQL(dm1->Query3,szSQL,true);

	if(!dm1->Query3->Eof)
	{
                rt = dm1->Query3->FieldByName("customs_charge")->AsString.c_str();
        }
        return rt;
}
void __fastcall TAcceptForm::btnClearQryCondClick(TObject *Sender)
{
        edtQryCid->Text = "";
        cbbQryClient->Text = "";
        edtQryLading->Text = "";
        cbbQryStatus->ItemIndex = -1;
        edtQryJZS->Text = "";
        cbbGoodsPerfQry->ItemIndex = -1;
        edtQryLicenseNo->Text= "";
        cbIsQryByDate->Checked = false;
        
        TDateTime tNow=TDateTime::CurrentDateTime();
        dtpQryAcceptDate->DateTime=tNow;
        dtpQryAcceptDateEnd->DateTime=tNow;
}
//---------------------------------------------------------------------------





void __fastcall TAcceptForm::lstViewDownColumnClick(TObject *Sender,
      TListColumn *Column)
{
    if(m_nColumnToSort==Column->Index)  
    {
      //�����ε��ͬһ����ı������Ż��ǽ����ŵ�Tag��־
      Column->Tag = 1-Column->Tag;
    }

    m_nColumnToSort = Column->Index;//���浱ǰ������к�
    //���� lvInvoiceΪ�����TListView�ؼ�ָ��
    lstViewDown->AlphaSort();
}
//---------------------------------------------------------------------------

void __fastcall TAcceptForm::lstViewDownCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{

    if (m_nColumnToSort == 0)
    {
      //����к�Ϊ0���б���Caption���Խ��бȽ�
      Compare = CompareText(Item1->Caption,Item2->Caption);
    }
    else 
    {
      //����кŲ�Ϊ0��������е���Ų�����������бȽ�
      int ix = m_nColumnToSort-1;
      Compare = CompareText(Item1->SubItems->Strings[ix], Item2->SubItems->Strings[ix]);
    }
		
    //TagΪ1ʱ�������У�Ϊ0ʱ��������
    if(lstViewDown->Columns->Items[m_nColumnToSort]->Tag)
    {
      Compare = -Compare;
    }

}
//---------------------------------------------------------------------------










void __fastcall TAcceptForm::dtpEndDateYYYYMMDDEnter(TObject *Sender)
{
/*
        dtpEndDateYYYYMMDD->SetFocus();
        SendMessage(dtpEndDateYYYYMMDD->Handle, WM_KEYDOWN, VK_RIGHT, 0);
*/
}
//---------------------------------------------------------------------------



void __fastcall TAcceptForm::btnCopyClick(TObject *Sender)
{
        if (edtCid->Text.IsEmpty() || lstViewContainer->Items->Count==0){
                ShowMessage("�޿ɸ�������");
                return;
        }

        CString szSQL;
        szSQL="select cid from customs where 1=1 ";
        szSQL +=" and cid="; szSQL += Str2DBString(edtCid->Text.c_str());
	RunSQL(dm1->Query1,szSQL,true);

        if (dm1->Query1->Eof){
                ShowMessage("�����Ƶļ�¼������,��������������Ϣ�������ٸ��ƣ������Ȳ�ѯ���ٸ��ƣ�");
                return;
        }
        //20140701 �ǰ���żƣ�һ��ų��ķݣ����Ƶ����ݾ���ʾ
        szSQL.Format("select cid from customs where containerinfo like '%%%s%%' order by cid", \
                lstViewContainer->Items->Item[0]->Caption.c_str());
//        ShowMessage(AnsiString(szSQL));
        RunSQL(dm1->Query1,szSQL,true);
        int pos = 1;
	while(!dm1->Query1->Eof)
	{
                if (edtCid->Text == dm1->Query1->FieldByName("cid")->AsString){
                        break;
                }
                dm1->Query1->Next();
                pos++;
        }
        if (pos>=3){
                char strMsg[256],strSQL[512];
                sprintf(strMsg,"\n  �Ѵ���ͬ���3�ݵ�, ȷ�ϸ��ƣ�  \n");
                if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
                        return;
        }
        m_sCidCopy = edtCid->Text.c_str();
        m_sContainerCopy = lstViewContainer->Items->Item[0]->Caption.c_str();
        ShowMessage("���Ƴɹ�������:"+m_sCidCopy);
 /*
        CString szSQL;
        szSQL="select cid from customs where 1=1 ";
        szSQL +=" and cid="; szSQL += Str2DBString(edtCid->Text.c_str());
	RunSQL(dm1->Query1,szSQL,true);

        if (dm1->Query1->Eof){
                ShowMessage("�����Ƶļ�¼������,��������������Ϣ�������ٸ��ƣ������Ȳ�ѯ���ٸ��ƣ�");
                return;
        }

        m_sCidCopy = edtCid->Text.c_str();
        ShowMessage("���Ƴɹ�������:"+m_sCidCopy);
*/
}
//---------------------------------------------------------------------------

void __fastcall TAcceptForm::btnPasteClick(TObject *Sender)
{
        CString szSQL;
        //TODO �ظ��߼�
        //20140701 �ǰ���żƣ�һ��ų��ķݣ����Ƶ����ݾ���ʾ
        szSQL.Format("select count(*) as cnt from customs where containerinfo like '%%%s%%' ", \
                m_sContainerCopy.c_str());
        RunSQL(dm1->Query1,szSQL,true);
        int pos = 1;
	if(!dm1->Query1->Eof)
	{
                if (dm1->Query1->FieldByName("cnt")->AsInteger >=3){
                        char strMsg[256],strSQL[512];
                        sprintf(strMsg,"\n  �Ѵ���ͬ���3�ݵ�, ȷ�ϸ��ƣ�  \n");
                        if(Application->MessageBox(strMsg,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
                                return;
                }
        }

        szSQL="select *,CONVERT(varchar(10), acceptdate, 23) as ad, substring(containerinfo,0,charindex('|',containerinfo)) as cnt_con \
                from customs where 1=1 ";
        if (!m_sCidCopy.IsEmpty()){
                szSQL +=" and cid="; szSQL += Str2DBString(m_sCidCopy.c_str());
        }else{
                ShowMessage("�޿�ճ�����ݣ����ȸ���");
                return;
        }

        TListItem *pItem;
        lstViewDown->Items->Clear();
	RunSQL(dm1->Query1,szSQL,true);

        if (dm1->Query1->Eof){
                ShowMessage("�����Ƶļ�¼������");
                return;
        }

	if(!dm1->Query1->Eof)
	{
                lstViewContainer->Items->Clear();

                edtCid->Text = "";
//                cbbGoodsPerf->ItemIndex=cbbGoodsPerf->Items->IndexOf(dm1->Query1->FieldByName("goodsperf")->AsString);
                cbbGoodsPerf->ItemIndex = cbbGoodsPerf->Items->IndexOf("��");
                cbbClient->Text=dm1->Query1->FieldByName("client")->AsString;
                cbbCustomsCharge->Text = AnsiString(getCustomsCharge(cbbClient->Text.c_str()));
                edtBoatNo->Text = dm1->Query1->FieldByName("boatno")->AsString;
                edtBoatOrder->Text = dm1->Query1->FieldByName("boatorder")->AsString;
                edtBoatName->Text = dm1->Query1->FieldByName("boatname")->AsString;
                cbbStatus->ItemIndex=cbbStatus->Items->IndexOf(dm1->Query1->FieldByName("status")->AsString);
                AnsiString status = dm1->Query1->FieldByName("status")->AsString;
                edtLading->Text =  dm1->Query1->FieldByName("ladingid")->AsString.c_str();
                edtContainerInfo->Text = dm1->Query1->FieldByName("containerinfo")->AsString.c_str(); //�������ã���ɾ����
                edtCliWorkid->Text =   dm1->Query1->FieldByName("cliworkid")->AsString.c_str();
//                edtCustfree->Text = dm1->Query1->FieldByName("custfree")->AsString.c_str();
                edtCustfree->Text = "0";
                edtSealId->Text = dm1->Query1->FieldByName("sealid")->AsString.c_str();
                edtBeiZhu->Text = dm1->Query1->FieldByName("beizhu")->AsString.c_str();
                cbbShipAgent->ItemIndex=cbbShipAgent->Items->IndexOf(dm1->Query1->FieldByName("shipagent")->AsString);
                flushContainer(AnsiString(dm1->Query1->FieldByName("containerinfo")->AsString.c_str()));
/*
                TDateTime tNow=TDateTime::CurrentDateTime();
                dtpEndDateYYYYMMDD->DateTime=tNow;
                dtpEndDateHHMM->Time = EncodeTime(17,00,00,0);
                dtpQryAcceptDate->DateTime=tNow;
                dtpQryAcceptDateEnd->DateTime=tNow;
*/
                TDateTime tDate;
                tDate=StrToDateTime(dm1->Query1->FieldByName("endcustdate")->AsString.c_str());
                dtpEndDateYYYYMMDD->DateTime=tDate;
                dtpEndDateHHMM->Time=tDate;
//		dm1->Query1->Next();
	}
}







