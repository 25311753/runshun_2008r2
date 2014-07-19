//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <assert.h>
#include "DataModule.h"
#include "BaseCode.h"
#include "LdyInterface.h"
#include "TConst.h"
#include "MaterialEdit.h"
#include "AskPurchaseMT.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMaterialEditForm *MaterialEditForm;

void MaterialEdit(int nAuth)
{
	if (nAuth==AUTH_NONE)
        {
                return;
        }
	TMaterialEditForm *pForm = new TMaterialEditForm(Application);
	assert(pForm!=NULL);
	pForm->m_nAuthor=nAuth;
	pForm->ShowModal();
	delete pForm;
}
void MaterialEdit(LPCSTR lpszCorp, LPCSTR lpszID0, LPCSTR lpszID1, int nAuth)
{
	if (nAuth==AUTH_NONE)
        {
        	return;
        }

        TMaterialEditForm *pForm = new TMaterialEditForm(Application);
	assert(pForm!=NULL);
	if (lpszCorp!=NULL && lpszID0!=NULL && lpszID1!=NULL)
        {
                pForm->m_szCorp=lpszCorp;
                pForm->m_szID0=lpszID0;
                pForm->m_szID1=lpszID1;

        }
	pForm->m_nAuthor=nAuth;
	pForm->ShowModal();
	delete pForm;
}
/*
void MaterialEdit(LPCSTR lpszID,int nAuth)
{
	if(nAuth==AUTH_NONE)	return;

	TMaterialEditForm *pForm;
  pForm=new TMaterialEditForm(Application);
  assert(pForm!=NULL);
	if(lpszID!=NULL)	pForm->m_szIDFirst=lpszID;
  pForm->m_nAuthor=nAuth;
  pForm->ShowModal();
  delete pForm;
}

*/

//---------------------------------------------------------------------------
__fastcall TMaterialEditForm::TMaterialEditForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


void TMaterialEditForm::ResetCorp()
{
	int i;
	for (i = 0; i<m_lstCorpID.GetSize(); i++)
	{
		if (m_cCurCorp==m_lstCorpID[i][0])
		{
			lstCorp->ItemIndex=i;
			break;
		}
	}
	if (i==m_lstCorpID.GetSize())
        {
        	lstCorp->ItemIndex=-1;
        }
}

void TMaterialEditForm::ResetMiddle()
{
	CString szSQL;

	m_lstMidID.RemoveAll();

	lstMid->Items->Clear();
	if (m_cCurLarge=='\0')
        {
        	return;
        }
	szSQL.Format("select * from dicmiddletype where mt_code like '%c_' order by mt_code", m_cCurLarge);
	RunSQL(dm1->Query1, szSQL, true);
	while (!dm1->Query1->Eof)
	{
		m_lstMidID.Add(dm1->Query1->FieldByName("mt_code")->AsString.c_str());
		lstMid->Items->Add(dm1->Query1->FieldByName("mt_name")->AsString);
		dm1->Query1->Next();
	}
}

void TMaterialEditForm::ResetMin()
{
	CString szSQL;

	m_lstMinID.RemoveAll();

	lstMin->Items->Clear();

	if (m_szCurMid.IsEmpty())
        {
        	return;
        }

	szSQL.Format("select * from dicsmalltype where st_code like '%s%%' order by st_code", m_szCurMid);
	RunSQL(dm1->Query1, szSQL, true);
	while (!dm1->Query1->Eof)
	{
		m_lstMinID.Add(dm1->Query1->FieldByName("st_code")->AsString.c_str());
		lstMin->Items->Add(dm1->Query1->FieldByName("st_name")->AsString);
		dm1->Query1->Next();
	}
}
void __fastcall TMaterialEditForm::FormShow(TObject *Sender)
{
	CString szSQL;

        edtSpec->Text = "";
	edtCorp->Text = "";
	edtID0->Text = "";
	edtID1->Text = "";

	szSQL = "select * from diccorp order by cp_code";
	RunSQL(dm1->Query1, szSQL, true);
	while (!dm1->Query1->Eof)
	{
		m_lstCorpID.Add(dm1->Query1->FieldByName("cp_code")->AsString.c_str());
		lstCorp->Items->Add(dm1->Query1->FieldByName("cp_name")->AsString);
		dm1->Query1->Next();
	}

	szSQL = "select * from diclargetype order by lt_code";
	RunSQL(dm1->Query1, szSQL, true);
	while (!dm1->Query1->Eof)
	{
		m_lstLargID.Add(dm1->Query1->FieldByName("lt_code")->AsString.c_str());
		lstLarge->Items->Add(dm1->Query1->FieldByName("lt_name")->AsString);
		dm1->Query1->Next();
	}

	szSQL = "select * from DicUnits order by du_code";
	RunSQL(dm1->Query1, szSQL, true);
	while (!dm1->Query1->Eof)
	{
		m_lstUnitsID.Add(dm1->Query1->FieldByName("du_code")->AsString.c_str());
		lstUnits->Items->Add(dm1->Query1->FieldByName("du_name")->AsString);
		dm1->Query1->Next();
	}


        if (m_szCorp!=NULL && m_szID0!=NULL && m_szID1!=NULL)
        {
                edtCorp->Text = m_szCorp;
                edtID0->Text = m_szID0;
                edtID1->Text = m_szID1;
        }
        else
        	edtCorp->Text = g_theOperator.op_cpcode;
	m_enState = EN_STATE_IDLE;
	ResetCtrl();



}
//---------------------------------------------------------------------------

void __fastcall TMaterialEditForm::lstCorpClick(TObject *Sender)
{
	int nSel;
	if ((nSel=lstCorp->ItemIndex)<0 || nSel>=m_lstCorpID.GetSize())
        {
        	return;
        }
	m_enClick = EN_CLICK_CORP;
	edtCorp->Text = (LPCSTR)m_lstCorpID[nSel];
	ResetAllUI();
}
//---------------------------------------------------------------------------

//------------------------------------------------------------------------------

void TMaterialEditForm::ResetAllUI()
{
	char strMid[80];
	CString szSQL;
	int i, nSel;
	bool bCorp, bLarge, bMid;
	char strTemp[80], strID[80]={0};
	char cCorp;

        if (m_enState==EN_STATE_EDIT)
        {//������޸ģ������±��Ϊ׼

	        strcpy(strTemp, edtCorpNew->Text.c_str());
	        cCorp = strTemp[0];
	        strcpy(strID, edtID0New->Text.c_str());
        }
        else
        {//����Ļ�����ԭ���Ϊ׼
                strcpy(strTemp, edtCorp->Text.c_str());
	        cCorp = strTemp[0];
	        strcpy(strID, edtID0->Text.c_str());
        }

	bCorp = bLarge = bMid = false;
	if (cCorp!=m_cCurCorp)
	{
		bCorp=true;
		m_cCurCorp=cCorp;
		ResetCorp();
	}
	if (bCorp || strID[0]!=m_cCurLarge)
	{
		bLarge = true;
		m_cCurLarge = strID[0];
		for (i=0; i<m_lstLargID.GetSize(); i++)
		{
			if (strID[0]==m_lstLargID[i][0])
			{
				lstLarge->ItemIndex = i;
				break;
			}
		}
		if (i==m_lstLargID.GetSize())
                {
                	lstLarge->ItemIndex=-1;
                }
		ResetMiddle();
	}

	strcpy(strMid, strID);
	strMid[2] = '\0';
	if (bLarge || m_szCurMid!=strMid)
	{
		m_szCurMid = strMid;
		bMid = true;
		for (i=0; i<m_lstMidID.GetSize(); i++)
		{
			if (m_lstMidID[i]==strMid)
			{
				lstMid->ItemIndex=i;
				break;
			}
		}
		if (i==m_lstMidID.GetSize())
                {
                	lstMid->ItemIndex=-1;
                }
		ResetMin();
	}

        if (bMid || m_szCurMin!=strID || m_enState==EN_STATE_EDIT)
	{
		m_szCurMin = strID;
		for (i=0; i<m_lstMinID.GetSize(); i++)
		{
			if (m_lstMinID[i]==strID)
			{
				lstMin->ItemIndex=i;
				break;
			}
		}
		if (i==m_lstMinID.GetSize())
                {
                	lstMin->ItemIndex=-1;
                }
		ResetView();
	}
        
	m_enClick=EN_CLICK_NONE;
      
}

/*
//------------------------------------------------------------------------------

void TMaterialEditForm::ResetAllUI()
{
	char strMid[80];
	CString szSQL;
	int i,nSel;
	bool bCorp,bLarge,bMid;
	char strTemp[80],strID[80]={0};
	char cCorp;

	strcpy(strTemp,edtCorp->Text.c_str());
	cCorp=strTemp[0];
	strcpy(strID,edtID0->Text.c_str());

	bCorp=bLarge=bMid=false;
	if(cCorp!=m_cCurCorp)
	{
		bCorp=true;
		m_cCurCorp=cCorp;
		ResetCorp();
	}
	if(bCorp || strID[0]!=m_cCurLarge)
	{
		bLarge=true;
		m_cCurLarge=strID[0];
		for(i=0;i<m_lstLargID.GetSize();i++)
		{
			if(strID[0]==m_lstLargID[i][0])
			{
				lstLarge->ItemIndex=i;
				break;
			}
		}
		if(i==m_lstLargID.GetSize())	lstLarge->ItemIndex=-1;
		ResetMiddle();
	}
	strcpy(strMid,strID);
	strMid[2]='\0';
	if(bLarge || m_szCurMid!=strMid)
	{
		m_szCurMid=strMid;
		bMid=true;
		for(i=0;i<m_lstMidID.GetSize();i++)
		{
			if(m_lstMidID[i]==strMid)
			{
				lstMid->ItemIndex=i;
				break;
			}
		}
		if(i==m_lstMidID.GetSize())	lstMid->ItemIndex=-1;
		ResetMin();
	}

//----------------NEW 0521------------------------------------------------------
        if(bMid || m_szCurMin!=strID || m_enState==EN_STATE_EDIT)
	{
		m_szCurMin=strID;
		for(i=0;i<m_lstMinID.GetSize();i++)
		{
			if(m_lstMinID[i]==strID)
			{
				lstMin->ItemIndex=i;
				break;
			}
		}
		if(i==m_lstMinID.GetSize())	lstMin->ItemIndex=-1;
		ResetView();
	}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	if(bMid || m_szCurMin!=strID)
//	{
//		m_szCurMin=strID;
//		for(i=0;i<m_lstMinID.GetSize();i++)
//		{
//			if(m_lstMinID[i]==strID)
//			{
//				lstMin->ItemIndex=i;
//				break;
//			}
//		}
//		if(i==m_lstMinID.GetSize())	lstMin->ItemIndex=-1;
//		ResetView();
//	}

//------------------------------------------------------------------------------

	m_enClick=EN_CLICK_NONE;
}
*/

//------------------------------------------------------------------------------

//---------------------------------------------------------------------------

void TMaterialEditForm::ResetView()
{
	CString szSQL, szSQL1;
	TListItem *pItem;
	char strID[80];
	lstView->Items->Clear();

        if (m_cCurCorp=='\0')
        {
        	return;
        }
	if (m_szCurMin.IsEmpty())
        {
        	return;
        }
	if (m_szCurMin.GetLength()!=4)
        {
                return;
        }

	szSQL.Format("select * from materialno where mn_code like '%c%s%%'", m_cCurCorp, m_szCurMin);
        szSQL += " order by mn_code";
	RunSQL(dm1->Query1, szSQL, true);
	while (!dm1->Query1->Eof)
	{
		memset(strID, 0, sizeof(strID));
		strcpy(strID, dm1->Query1->FieldByName("mn_code")->AsString.c_str());
		pItem=lstView->Items->Add();
		assert(pItem!=NULL);
                
		pItem->Caption = strID;
		pItem->SubItems->Add(dm1->Query1->FieldByName("mn_name")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("mn_usage")->AsString);
		pItem->SubItems->Add(dm1->Query1->FieldByName("mn_rem")->AsString);
                if (!dm1->Query1->FieldByName("mn_ducode")->IsNull)
                {
                	szSQL1.Format("select * from DicUnits where du_code='%s'", dm1->Query1->FieldByName("mn_ducode")->AsString);
                	RunSQL(dm1->Query2, szSQL1, true);
                        pItem->SubItems->Add(dm1->Query2->FieldByName("du_name")->AsString);
                }
                else
		pItem->SubItems->Add("");
                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_date")	->AsString);

		dm1->Query1->Next();
	}
}

void __fastcall TMaterialEditForm::lstLargeClick(TObject *Sender)
{
	int nSel;
	char strTemp[80], strID[80];

	if ((nSel=lstLarge->ItemIndex)<0 || nSel>=m_lstLargID.GetSize())
        {
        	return;
        }
	m_enClick = EN_CLICK_LARGE;
	strcpy(strTemp, m_lstLargID[nSel]);
	memset(strID, 0, sizeof(strID));
	strcpy(strID, edtID0->Text.c_str());
	strID[0] = strTemp[0];
	edtID0->Text = strID;
	ResetAllUI();
}

void __fastcall TMaterialEditForm::edtCorpChange(TObject *Sender)
{
	if (m_enClick!=EN_CLICK_NONE)
        {
        	return;
        }
        ResetAllUI();
}

void __fastcall TMaterialEditForm::lstMidClick(TObject *Sender)
{
	int nSel;
	char strTemp[80], strID[80];

	if ((nSel=lstMid->ItemIndex)<0 || nSel>=m_lstMidID.GetSize())
        {
        	return;
        }
	m_enClick = EN_CLICK_MID;
	strcpy(strTemp, m_lstMidID[nSel]);
	memset(strID, 0, sizeof(strID));
	strcpy(strID, edtID0->Text.c_str());
	strID[1] = strTemp[1];
	edtID0->Text = strID;
	ResetAllUI();
}

void __fastcall TMaterialEditForm::lstMinClick(TObject *Sender)
{
	int nSel;
	char strTemp[80], strID[80];

	if((nSel=lstMin->ItemIndex)<0 || nSel>=m_lstMinID.GetSize())
        {
        	return;
        }
	m_enClick = EN_CLICK_MIN;
	edtID0->Text = (LPCSTR)m_lstMinID[nSel];
	ResetAllUI();
}
//---------------------------------------------------------------------------

void __fastcall TMaterialEditForm::edtID0Change(TObject *Sender)
{
	if (m_enClick!=EN_CLICK_NONE)
        {
        	return;
        }

	ResetAllUI();
}
//---------------------------------------------------------------------------

void __fastcall TMaterialEditForm::edtID1Change(TObject *Sender)
{
	int i;
	TListItem *pItem;

	if (m_enClick!=EN_CLICK_NONE)
        {
        	return;
        }

	for (i=0; i<lstView->Items->Count; i++)
	{
		pItem = lstView->Items->Item[i];
		if (pItem->Caption==edtCorp->Text+edtID0->Text+"-"+edtID1->Text)
		{
			lstView->Selected = pItem;
			break;
		}
	}
	if (i==lstView->Items->Count)
        {
  	        edtSpec->Text = "";
                lstView->Selected=NULL;
        }

        ResetCtrl();
}
//------------------------------------------------------------------------------

void __fastcall TMaterialEditForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if(Key==VK_RETURN)
	{
        	KeyReturnHandle(ActiveControl);
        }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialEditForm::lstViewSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
	TListItem *pItem;
	CString szSQL1;
	char strID[80];
        int i;

	if (!Selected || lstView->Selected==NULL)
        {
        	return;
        }
	pItem = lstView->Selected;
	m_enClick = EN_CLICK_VIEW;
	strcpy(strID, pItem->Caption.c_str());

	edtID1->Text = strID+6;
	strID[5] = '\0';
        edtID0->Text = strID+1;
	strID[1] = '\0';
        edtCorp->Text = strID;
	edtSpec->Text = pItem->SubItems->Strings[0];
	edtUsage->Text = pItem->SubItems->Strings[1];
	Memo1->Text = pItem->SubItems->Strings[2];
//��ʾ��λ
	for (i = 0; i<m_lstUnitsID.GetSize(); i++)
	{
		if (i==lstUnits->Items->IndexOf(pItem->SubItems->Strings[3]))
		{
			lstUnits->ItemIndex = i;
			break;
		}
	}
	if (i==m_lstUnitsID.GetSize())
        {
                lstUnits->ItemIndex=-1;
        }

	m_enClick=EN_CLICK_NONE;
        ResetCtrl();
}
//---------------------------------------------------------------------------

void TMaterialEditForm::ResetCtrl()
{
        //if(strcmp(g_theOperator.op_code,"A")==0 || strcmp(g_theOperator.op_code,"admin")==0)
        if(g_theOperator.op_cpcode[0]=='A' || strcmp(g_theOperator.op_code, "admin")==0)
        {
                bool bAuth, bEdit, bIdle, bEditD, bQuery;

                bAuth = m_nAuthor==AUTH_FULL;
                bIdle = m_enState==EN_STATE_IDLE;
                bEdit = m_enState==EN_STATE_EDIT;
                bEditD = m_enState==EN_STATE_DATE;
                bEditD = m_enState==EN_STATE_DATE;
                bQuery = m_enState==EN_STATE_QUERY;

                EnableEdit(edtCorp, !bEdit);
                EnableEdit(edtID0, !bEdit);
                EnableEdit(edtID1, !bEdit);
                //�޸���ı���֮��ʾ��
                EnableEdit(edtCorpNew, bEdit);
                EnableEdit(edtID0New, bEdit);   // ֻ�����ʾ����춹����ϵ�Ҫ�󣬲������޸ġ�
                EnableEdit(edtID1New, bEdit);
                EnableEdit(edtDATE, bEditD && RB1->Checked);

                EnableCombo(lstCorp, !bEdit);
                EnableCombo(lstLarge, !bEdit);
                EnableCombo(lstMid, !bEdit);
                EnableCombo(lstMin, !bEdit);

                EnableEdit(edtSpec, !bIdle && !bQuery);
                EnableEdit(edtUsage, !bIdle && !bQuery);
	        Memo1->Enabled = !bIdle && !bQuery;
                lstUnits->Enabled = !bIdle && !bQuery;
                RB1->Enabled = bIdle;
                RB2->Enabled = bIdle;
                DateTimePicker1->Enabled = bEditD && RB2->Checked;
                DateTimePicker2->Enabled = bEditD && RB2->Checked;

                lstView->Enabled=bIdle;

                //����Timer������ͱ����
                if (m_enState==EN_STATE_DATE)
                {
                        Timer1->Enabled = false;
                        edtID1->Text = "";
                        edtID1New->Text = "";

                }
                if (m_enState==EN_STATE_IDLE)
                {
                        Timer1->Enabled = true;
                }
                if(m_enState==EN_STATE_EDIT)
                {//��ʾ�ɱ��
                  //                edtCorpNew->Text=edtCorp->Text;         //0523
                  //                edtID0New->Text=edtID0->Text;
                  //                edtID1New->Text=edtID1->Text;
                }

        	btnNew->Enabled = bIdle && bAuth;
        	btnEdit->Enabled = bIdle && bAuth && lstView->Selected!=NULL;
                btnMov->Enabled = bIdle && bAuth  && lstView->Selected!=NULL;
                btnOK->Enabled = !bIdle;
                btndq->Enabled = bIdle && bAuth;                  //    new        0519
                btnNO->Enabled = bIdle && bAuth && lstView->Selected!=NULL;             //    new        0524
                btnExit->Caption = bIdle ? "�˳�" : "����";
        }
        else
        {
        	bool bAuth, bEdit, bIdle;

                bAuth = m_nAuthor==AUTH_FULL;
                bIdle = m_enState==EN_STATE_IDLE;
                bEdit = m_enState==EN_STATE_EDIT;

                EnableEdit(edtCorp, !bEdit);
                EnableEdit(edtID0, !bEdit);
                EnableEdit(edtID1, !bEdit);
                //��A֮ͬ��j�Ͳ���
                EnableEdit(edtCorpNew, false);
                EnableEdit(edtID0New, false);
                EnableEdit(edtID1New, false);

                EnableEdit(edtDATE, false);
                Timer1->Enabled = false;
                btndq->Enabled = false;
                btnNO->Enabled = false;

                EnableCombo(lstCorp, !bEdit);
                EnableCombo(lstLarge, !bEdit);
                EnableCombo(lstMid, !bEdit);
                EnableCombo(lstMin, !bEdit);
                EnableEdit(edtSpec, !bIdle);
                EnableEdit(edtUsage, !bIdle);
                Memo1->Enabled = !bIdle;
                lstUnits->Enabled = !bIdle;
                lstView->Enabled = bIdle;
                btnNew->Enabled = bIdle && bAuth;
                btnEdit->Enabled = bIdle && bAuth && lstView->Selected!=NULL;
                btnMov->Enabled = bIdle && bAuth  && lstView->Selected!=NULL;
                btnOK->Enabled = !bIdle;
                btnExit->Caption = bIdle ? "�˳�" : "����";
        }
}
//------------------------------------------------------------------------------

void __fastcall TMaterialEditForm::btnExitClick(TObject *Sender)
{
	if (m_enState==EN_STATE_IDLE)
  	{
                Close();
        }
        else
        {// ûѡ��ȷ������ѡ���ˡ�����������һ���ٰ��ͻ��˳�
          	m_enState=EN_STATE_IDLE;
        }
        ResetCtrl();
}
//---------------------------------------------------------------------------

void __fastcall TMaterialEditForm::btnNewClick(TObject *Sender)
{
	m_enState = EN_STATE_NEW;
	ResetCtrl();
	edtCorp->Text = g_theOperator.op_cpcode;
	if(edtID0->Text.IsEmpty())
        {
        	edtID0->SetFocus();
        }
	else
        {
        	edtID1->SetFocus();
        }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialEditForm::btnEditClick(TObject *Sender)
{
	m_enState = EN_STATE_EDIT;
	ResetCtrl();
	edtSpec->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TMaterialEditForm::btnOKClick(TObject *Sender)
{
        if (g_theOperator.op_cpcode[0]=='A' || strcmp(g_theOperator.op_code, "admin")==0)
        {
        	CString szSQL;
	        char strID[80], strIDNew[80], strTemp[2048], strDATE[32], strMnName[80];                  // new add strDATE[32]  20050518
                                                                                // new strIDNew[80] 20050520
                TListItem *pItem;
	        int nSel, nSelNew;                                                       //new nSelNew

                sprintf(strID, "%s%s-%s", edtCorp->Text.c_str(), edtID0->Text.c_str(), edtID1->Text.c_str());
                sprintf(strIDNew, "%s%s-%s", edtCorpNew->Text.c_str(), edtID0New->Text.c_str(), edtID1New->Text.c_str());  // new 0520  new code
                sprintf(strDATE, "%s", edtDATE->Text.c_str());
                sprintf(strMnName, "%s", edtSpec->Text.c_str());

                if (((nSel=lstCorp->ItemIndex)<0 || nSel>=m_lstCorpID.GetSize()) && !(m_enState==EN_STATE_DATE))
                {
                        ShowMessage("��������Ч�Ĺ�˾��");
                        if (edtCorp->CanFocus())
                        {
                        	edtCorp->SetFocus();
                        }
                        return;
                }

                if (edtCorp->Text=='A' && edtCorp->Text!=g_theOperator.op_cpcode && strcmp(g_theOperator.op_code, "admin")!=0)
                {//�Ӵ��������Ĳɹ������޸���������Ȩ��
                        ShowMessage("��ֻ���������޸��Լ���˾�Ĳ��ϱ��");
                        if(edtCorp->CanFocus())
                        {
                                edtCorp->SetFocus();
                        }
                        return;
                }

                if (((nSel=lstLarge->ItemIndex)<0 || nSel>=m_lstLargID.GetSize()) && !(m_enState==EN_STATE_DATE))
                {
                        ShowMessage("��������Ч�Ĵ����");
                        if (edtID0->CanFocus())
                        {
                        	edtID0->SetFocus();
                        }
                        return;
                }
                if (((nSel=lstMid->ItemIndex)<0 || nSel>=m_lstMidID.GetSize()) && !(m_enState==EN_STATE_DATE))
                {
                        ShowMessage("��������Ч�������");
                        if (edtID0->CanFocus())
                        {
                        	edtID0->SetFocus();
                        }
                        return;
                }
                if (((nSel=lstMin->ItemIndex)<0 || nSel>=m_lstMinID.GetSize()) && !(m_enState==EN_STATE_DATE))
                {
                        ShowMessage("��������Ч��С���");
                        if (edtID0->CanFocus())
                        {
                        	edtID0->SetFocus();
                        }
                        return;
                }
                if (((nSel=lstUnits->ItemIndex)<0 || nSel>=m_lstUnitsID.GetSize()) && !(m_enState==EN_STATE_DATE) && !(m_enState==EN_STATE_QUERY))
                {
                        ShowMessage("��ѡ����Ч���ϵ�λ");
                        if (lstUnits->CanFocus())
                        {
                                lstUnits->SetFocus();
                        }
                        return;
                }
/*
	if((nSel=lstCorp->ItemIndex)<0 || nSel>=m_lstCorpID.GetSize())
	{
		ShowMessage("��������Ч�Ĺ�˾��");
		if(edtCorp->CanFocus())	edtCorp->SetFocus();
		return;
	}
	if(edtCorp->Text!=g_theOperator.op_cpcode && strcmp(g_theOperator.op_code,"admin")!=0)
	{
		ShowMessage("��ֻ���������޸��Լ���˾�Ĳ��ϱ��");
		if(edtCorp->CanFocus())	edtCorp->SetFocus();
		return;
	}
	if((nSel=lstLarge->ItemIndex)<0 || nSel>=m_lstLargID.GetSize())
	{
		ShowMessage("��������Ч�Ĵ����");
		if(edtID0->CanFocus())	edtID0->SetFocus();
		return;
	}
	if((nSel=lstMid->ItemIndex)<0 || nSel>=m_lstMidID.GetSize())
	{
		ShowMessage("��������Ч�������");
		if(edtID0->CanFocus())	edtID0->SetFocus();
		return;
	}
	if((nSel=lstMin->ItemIndex)<0 || nSel>=m_lstMinID.GetSize())
	{
		ShowMessage("��������Ч��С���");
		if(edtID0->CanFocus())	edtID0->SetFocus();
		return;
	}
*/

                if (edtDATE->Text.IsEmpty() && m_enState==EN_STATE_DATE && RB1->Checked==true)
                {//���ղ�ѯ
                        ShowMessage("�����뽨������(����)");
                        if (edtDATE->CanFocus())
                        {
                                edtDATE->SetFocus();
                        }
                        return;
                }

                if (edtID1->Text.IsEmpty() && !(m_enState==EN_STATE_DATE))
                {
                        ShowMessage("�������Ʒ���");
                        if (edtID1->CanFocus())
                        {
                                edtID1->SetFocus();
                        }
                        return;
                }

                if (edtID1New->Text.IsEmpty() && !(m_enState==EN_STATE_DATE) && m_enState==EN_STATE_EDIT)
                {
                        ShowMessage("�������µĲ�Ʒ���");
                        if (edtID1New->CanFocus())
                        {
                                edtID1New->SetFocus();
                        }
                        return;
                }
                if (edtSpec->Text.IsEmpty() && !(m_enState==EN_STATE_DATE))
                {
                        ShowMessage("��������;˵��");
                        if (edtSpec->CanFocus())
                        {
                                edtSpec->SetFocus();
                        }
                        return;
                }

                szSQL.Format("select mn_code from materialno where mn_code=%s", Str2DBString(strID));
                RunSQL(dm1->Query1, szSQL, true);

                if (m_enState==EN_STATE_NEW && !dm1->Query1->Eof)
                {
                        ShowMessage("���ݿ������иñ�ŵĲ��ϱ��");
                        if (edtID1->CanFocus())
                        {
                                edtID1->SetFocus();
                        }
                        return;
                }
                else if (((m_enState==EN_STATE_EDIT) || (m_enState==EN_STATE_QUERY)) && dm1->Query1->Eof)
                {
                        ShowMessage("���ݿ���û�иò��ϱ��");
                        if (edtID1->CanFocus())
                        {
                                edtID1->SetFocus();
                        }
                        return;
                }
        
                szSQL.Format("select * from materialno where mn_name=%s", Str2DBString(strMnName));
                RunSQL(dm1->Query1, szSQL, true);
                if (m_enState==EN_STATE_NEW && !dm1->Query1->Eof)
                {
                        ShowMessage("�ò������н������ϣ������ظ��ٽ�");
                        return;
                }

                szSQL.Format("select * from materialno where mn_name=%s", Str2DBString(strMnName));
                RunSQL(dm1->Query1, szSQL, true);
                if (m_enState==EN_STATE_EDIT && strcmp(strID, strIDNew)==0 && !dm1->Query1->Eof)
                {
                        ShowMessage("�ò������н������ϣ����ٴ�����Ʒ�����");
                        if (edtSpec->CanFocus())
                        {
                                edtSpec->SetFocus();
                        }
                        return;
                }
                szSQL.Format("select * from codecheck where cc_ocode=%s", Str2DBString(strID));
                RunSQL(dm1->Query1, szSQL, true);
                if (m_enState==EN_STATE_EDIT && strcmp(strID, strIDNew)!=0 && !dm1->Query1->Eof)
                {
                        ShowMessage("�ñ���Ѿ����޸Ĺ����������޸�,�밴OK����ѡ�����");
                        if (edtID1New->CanFocus())
                        {
                                edtID1New->SetFocus();
                        }
                        return;
                }
                szSQL.Format("select * from codecheck where cc_ncode=%s", Str2DBString(strID));
                RunSQL(dm1->Query1, szSQL, true);
                if (m_enState==EN_STATE_EDIT && strcmp(strID, strIDNew)!=0 && !dm1->Query1->Eof)
                {
                        ShowMessage("�ñ��������һ������޸Ķ��ɵģ������ٶԸñ�Ž��ж����޸�!");
                        ShowMessage("��ѡ��OK����ѡ�����");
                        if (edtID1->CanFocus())
                        {
                                edtID1->SetFocus();
                        }
                        return;
                }
                szSQL.Format("select * from codecheck where cc_ncode=%s", Str2DBString(strIDNew));
                RunSQL(dm1->Query1,szSQL, true);
                if (m_enState==EN_STATE_EDIT && strcmp(strID, strIDNew)!=0 && !dm1->Query1->Eof)
                {
                        ShowMessage("�޸���ı����Ϊ��һ��ŵ��޸Ľ������������һ�����!");
                        if (edtID1New->CanFocus())
                        {
                                edtID1New->SetFocus();
                        }
                        return;
                }
                szSQL.Format("select * from materialno where mn_code=%s", Str2DBString(strIDNew));
                RunSQL(dm1->Query1, szSQL, true);
                if (m_enState==EN_STATE_EDIT && strcmp(strID, strIDNew)!=0 && !dm1->Query1->Eof)
                {
                        ShowMessage("�޸���ı���Ѿ����ڣ���������һ���޸ĺ�!");
                        if (edtID1New->CanFocus())
                        {
                                edtID1New->SetFocus();
                        }
                        return;
                }

                szSQL.Format("select mn_date from materialno where mn_date=%s", Str2DBString(strDATE));
                RunSQL(dm1->Query1, szSQL, true);
                if (m_enState==EN_STATE_DATE && RB1->Checked==true && dm1->Query1->Eof)
                {
                        ShowMessage("���ݿ���û�и����Ӧ�ı�Ž���");
                        return;
                }
                if(m_enState==EN_STATE_DATE)
                {
                        szSQL.Format("select * from materialno where mn_name=%s and mn_code<>%s",
                                        Str2DBString(edtSpec->Text.c_str()),
                                        Str2DBString(strIDNew));
                }
                else if (m_enState==EN_STATE_EDIT)
                {
                        szSQL.Format("select * from materialno where mn_name=%s",
                                        Str2DBString(edtSpec->Text.c_str()));

                }
                else
                {
                        szSQL.Format("select * from materialno where mn_name=%s"
                        " and substring(mn_code, 1, 1)=%s"
                        " and mn_code<>%s", Str2DBString(edtSpec->Text.c_str()),
                        Str2DBString(g_theOperator.op_cpcode), Str2DBString(strID));
                }
                RunSQL(dm1->Query1,szSQL,true);
                if (!dm1->Query1->Eof)
                {
                        sprintf(strTemp, "���ݿ���������ͬ���ƵĲ��ϱ�ţ�����Ϊ��%s��", dm1->Query1->FieldByName("mn_code")->AsString.c_str());
                        ShowMessage(strTemp);
                        return;
                }



                if (m_enState==EN_STATE_NEW)
                {
                        szSQL = "insert into materialno (mn_code, mn_name, mn_usage, mn_rem, mn_date, mn_ducode) values(";
                        szSQL += Str2DBString(strID);
                        szSQL += ",";	szSQL+=Str2DBString(edtSpec->Text.c_str());
                        szSQL += ",";	szSQL+=Str2DBString(edtUsage->Text.c_str());
                        szSQL += ",";	szSQL+=Str2DBString(Memo1->Text.c_str());
                        szSQL += ",";	szSQL+=Str2DBString(strDATE);
                        szSQL += ",";   szSQL+=Str2DBString(m_lstUnitsID[lstUnits->ItemIndex]);
                        szSQL += ")";
                }

        //---------new 0524-------------------------------------------------------------// "�޸�"���
                else if ((m_enState==EN_STATE_EDIT) && strcmp(strID, strIDNew)!=0)        //�����±�ţ��ɱ����Ȼ���� �������ݵ�һ����
                {
                        szSQL = "insert into materialno (mn_code,mn_name,mn_usage,mn_rem,mn_date,mn_ducode) values(";
                        szSQL += Str2DBString(strIDNew);
                        szSQL += ",";	szSQL += Str2DBString(edtSpec->Text.c_str());
                        szSQL += ",";	szSQL += Str2DBString(edtUsage->Text.c_str());
                        szSQL += ",";	szSQL += Str2DBString(Memo1->Text.c_str());
                        szSQL += ",";	szSQL += Str2DBString(strDATE);
                        szSQL += ",";     szSQL += Str2DBString(m_lstUnitsID[lstUnits->ItemIndex]);
                        szSQL += ")";
                }

                else if (m_enState==EN_STATE_EDIT && strcmp(strID,strIDNew)==0)          //һ��"�޸�"
                {
                        szSQL = "update materialno set";                                  //�ɱ����Ȼ����
                        szSQL += " mn_name=";			szSQL += Str2DBString(edtSpec->Text.c_str());
                        szSQL += ",mn_usage=";			szSQL += Str2DBString(edtUsage->Text.c_str());
                        szSQL += ",mn_rem=";			szSQL += Str2DBString(Memo1->Text.c_str());
                        szSQL += ",mn_ducode=";                 szSQL += Str2DBString(m_lstUnitsID[lstUnits->ItemIndex]);
                        szSQL += ",mn_date=";			szSQL += Str2DBString(strDATE); // new 0519
                        szSQL += " where mn_code=";	        szSQL += Str2DBString(strID);     //�൱춡�szSQL+=Str2DBString(strIDNew);
                                                                                        //����strID��strIDNewһ��
                }
                try
                {
                        RunSQL(dm1->Query1, szSQL);
                }
                catch(...)
                {
                        sprintf(strTemp, "���ݿ����ʧ��!SQL=\n%s", szSQL);
                        ShowMessage(strTemp);
                        return;
                }
                if ((m_enState==EN_STATE_EDIT) && strcmp(strID, strIDNew)!=0)
                {// codecheck ͬ������
                        szSQL = "insert into codecheck (cc_ncode,cc_ocode) values(";
                        szSQL += Str2DBString(strIDNew);
                        szSQL += ",";     szSQL += Str2DBString(strID);
                        szSQL += ")";
                        try
                        {
                                RunSQL(dm1->Query1, szSQL);
                        }
                        catch(...)
                        {
                                sprintf(strTemp, "���ݿ����ʧ��!SQL=\n%s", szSQL);
                                ShowMessage(strTemp);
                                return;
                        }
                }
                if (m_enState==EN_STATE_EDIT)
                {
                 //materiallook ͬ������
                        szSQL.Format("update materiallook set ml_mncode='%s',ml_mnname='%s' where ml_mncode='%s'", strIDNew, edtSpec->Text.c_str(), strID);
                        try
                        {
                                RunSQL(dm1->Query1, szSQL);
                        }
                        catch(...)
                        {
                                sprintf(strTemp, "���ݿ����ʧ��!SQL=\n%s", szSQL);
                                ShowMessage(strTemp);
                                return;
                        }
                }
                if(m_enState==EN_STATE_NEW)
                {
                        pItem = lstView->Items->Insert(0);
                        assert(pItem!=NULL);
                        pItem->Caption = strID;
                        pItem->SubItems->Add(edtSpec->Text);
                        pItem->SubItems->Add(edtUsage->Text);
                        pItem->SubItems->Add(Memo1->Text);
                        pItem->SubItems->Add(lstUnits->Text);
                        pItem->SubItems->Add(edtDATE->Text);
                }
                else if (m_enState==EN_STATE_DATE)
                {
                        CString szSQL1;
                        lstView->Items->Clear();
                        if (RB1->Checked==true)
                        {//���ղ�ѯ
                                szSQL.Format("select * from materialno where mn_date=%s", Str2DBString(strDATE));
                        }
                        else
                        {//���ղ�ѯ
                                char strDate0[80], strDate1[80];
                                unsigned short year0, month0, day0, year1, month1, day1;

                                DateTimePicker1->DateTime.DecodeDate(&year0, &month0, &day0);
                                sprintf(strDate0, "%04d-%02d-%02d", year0, month0, day0);
                                DateTimePicker2->DateTime.DecodeDate(&year1, &month1, &day1);
                                sprintf(strDate1, "%04d-%02d-%02d", year1, month1, day1);
                                szSQL.Format("select * from materialno where mn_date between '%s' and '%s'", strDate0, strDate1);
                        }
                        szSQL += " order by mn_code";
                        RunSQL(dm1->Query1, szSQL, true);

                        while(!dm1->Query1->Eof)
                        {
        //                        lstView->Items->Clear();
                                memset(strID, 0, sizeof(strID));
                                strcpy(strID, dm1->Query1->FieldByName("mn_code")->AsString.c_str());
                                pItem = lstView->Items->Add();
                                assert(pItem!=NULL);
                                pItem->Caption = strID;
                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_name")->AsString);
                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_usage")->AsString);
                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_rem")->AsString);

                                if (!dm1->Query1->FieldByName("mn_ducode")->IsNull)
                                {
                                        szSQL1.Format("select * from DicUnits where du_code='%s'", dm1->Query1->FieldByName("mn_ducode")->AsString);
                                        RunSQL(dm1->Query2, szSQL1, true);
                                        pItem->SubItems->Add(dm1->Query2->FieldByName("du_name")->AsString);
                                }
                                else
                                {
                                        pItem->SubItems->Add("");
                                }

                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_date")->AsString);
                                dm1->Query1->Next();
                         }
                }
                else if (m_enState==EN_STATE_QUERY)
                {
                        CString szSQL1;
                        lstView->Items->Clear();
                        szSQL.Format("select * from materialno where mn_code in(select cc_ncode as mn_code from codecheck where cc_ocode =%s", Str2DBString(strID));
                        szSQL += ")";
                        szSQL += " order by mn_code";
                        RunSQL(dm1->Query1, szSQL, true);

                        while (!dm1->Query1->Eof)
                        {
        //                        lstView->Items->Clear();
                                memset(strID, 0, sizeof(strID));
                                strcpy(strID, dm1->Query1->FieldByName("mn_code")->AsString.c_str());
                                pItem=lstView->Items->Add();
                                assert(pItem!=NULL);
                                pItem->Caption = strID;
                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_name")->AsString);
                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_usage")->AsString);
                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_rem")->AsString);
                                if (!dm1->Query1->FieldByName("mn_ducode")->IsNull)
                                {
                                        szSQL1.Format("select * from DicUnits where du_code='%s'", dm1->Query1->FieldByName("mn_ducode")->AsString);
                                        RunSQL(dm1->Query2, szSQL1, true);
                                        pItem->SubItems->Add(dm1->Query2->FieldByName("du_name")->AsString);
                                }
                                else
                                {
                                        pItem->SubItems->Add("");
                                }
                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_date")->AsString);
                                dm1->Query1->Next();
                        }
                }
                else if (m_enState==EN_STATE_EDIT)
                {
                        CString szSQL1;
                        lstView->Items->Clear();
                        szSQL.Format("select * from materialno where mn_code like '%s%s%%'", edtCorpNew->Text,edtID0New->Text);
                        szSQL += " order by mn_code";
                        RunSQL(dm1->Query1, szSQL, true);

                        while (!dm1->Query1->Eof)
                        {

                                memset(strID, 0, sizeof(strID));
                                strcpy(strID, dm1->Query1->FieldByName("mn_code")->AsString.c_str());
                                pItem = lstView->Items->Add();
                                assert(pItem!=NULL);
                                pItem->Caption = strID;
                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_name")->AsString);
                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_usage")->AsString);
                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_rem")->AsString);
                                if(!dm1->Query1->FieldByName("mn_ducode")->IsNull)
                                {
                                        szSQL1.Format("select * from DicUnits where du_code='%s'", dm1->Query1->FieldByName("mn_ducode")->AsString);
                                        RunSQL(dm1->Query2, szSQL1, true);
                                        pItem->SubItems->Add(dm1->Query2->FieldByName("du_name")->AsString);
                                }
                                else
                                pItem->SubItems->Add("");
                                pItem->SubItems->Add(dm1->Query1->FieldByName("mn_date")->AsString);
                                dm1->Query1->Next();
                        }
                }
                else
                {
                        pItem = lstView->Selected;
                        if (pItem!=NULL)
                        {
                                pItem->SubItems->Strings[0] = edtSpec->Text;
                                pItem->SubItems->Strings[1] = edtUsage->Text;
                                pItem->SubItems->Strings[2] = Memo1->Text;
                        }
                }
                m_enState = EN_STATE_IDLE;
                ResetCtrl();
                if (btnNew->CanFocus())
                {
                        btnNew->SetFocus();
                }
        }
//-----------new 0618 ��A֮ͬ�ʾ�ֻ������ͨ�޸�---------------------------------
        else
        {
                CString szSQL;
        	char strID[80], strTemp[2048];
                TListItem *pItem;
        	int nSel;

        	sprintf(strID, "%s%s-%s",edtCorp->Text.c_str(), edtID0->Text.c_str(), edtID1->Text.c_str());
        	if ((nSel=lstCorp->ItemIndex)<0 || nSel>=m_lstCorpID.GetSize())
        	{
        		ShowMessage("��������Ч�Ĺ�˾��");
        		if (edtCorp->CanFocus())
                        {
                        	edtCorp->SetFocus();
                        }
        		return;
        	}
        	if (edtCorp->Text!=g_theOperator.op_cpcode && strcmp(g_theOperator.op_code, "admin")!=0)
        	{
        		ShowMessage("��ֻ���������޸��Լ���˾�Ĳ��ϱ��");
        		if (edtCorp->CanFocus())
                        {
                        	edtCorp->SetFocus();
                        }
        		return;
        	}
        	if ((nSel=lstLarge->ItemIndex)<0 || nSel>=m_lstLargID.GetSize())
        	{
        		ShowMessage("��������Ч�Ĵ����");
        		if (edtID0->CanFocus())
                        {
                        	edtID0->SetFocus();
                        }
        		return;
        	}
        	if ((nSel=lstMid->ItemIndex)<0 || nSel>=m_lstMidID.GetSize())
        	{
        		ShowMessage("��������Ч�������");
        		if (edtID0->CanFocus())
                        {
                        	edtID0->SetFocus();
                        }
        		return;
        	}
        	if ((nSel=lstMin->ItemIndex)<0 || nSel>=m_lstMinID.GetSize())
        	{
        		ShowMessage("��������Ч��С���");
        		if (edtID0->CanFocus())
                        {
                        	edtID0->SetFocus();
                        }
        		return;
        	}
        	if (edtID1->Text.IsEmpty())
        	{
        		ShowMessage("�������Ʒ���");
        		if (edtID1->CanFocus())
                        {
                        	edtID1->SetFocus();
                        }
        		return;
           	}
        	if (((nSel=lstUnits->ItemIndex)<0 || nSel>=m_lstUnitsID.GetSize()) && !(m_enState==EN_STATE_DATE) && !(m_enState==EN_STATE_QUERY))
	        {
        		ShowMessage("��ѡ����Ч���ϵ�λ");
		        if (lstUnits->CanFocus())
                        {
                        	lstUnits->SetFocus();
                        }
	        	return;
        	}
        	if (edtSpec->Text.IsEmpty())
        	{
        		ShowMessage("��������;˵��");
        		if (edtSpec->CanFocus())
                        {
                        	edtSpec->SetFocus();
                        }
        		return;
        	}
        	szSQL.Format("select mn_code from materialno where mn_code=%s", Str2DBString(strID));
        	RunSQL(dm1->Query1,szSQL, true);
        	if (m_enState==EN_STATE_NEW && !dm1->Query1->Eof)
        	{
        		ShowMessage("���ݿ������иñ�ŵĲ��ϱ��");
        		if (edtID1->CanFocus())
                        {
                        	edtID1->SetFocus();
                        }
        		return;
        	}
        	else if (m_enState==EN_STATE_EDIT && dm1->Query1->Eof)
        	{
        		ShowMessage("���ݿ���û�иò��ϱ��");
        		if (edtID1->CanFocus())
                        {
                        	edtID1->SetFocus();
                        }
        		return;
        	}
        	szSQL.Format("select * from materialno where mn_name=%s"
        		" and substring(mn_code,1,1)=%s"
        		" and mn_code<>%s", Str2DBString(edtSpec->Text.c_str()),
        		Str2DBString(g_theOperator.op_cpcode), Str2DBString(strID));
        	RunSQL(dm1->Query1, szSQL, true);
        	if (!dm1->Query1->Eof)
        	{
        		sprintf(strTemp,"���ݿ���������ͬ���ƵĲ��ϱ�ţ�����Ϊ��%s��", dm1->Query1->FieldByName("mn_code")->AsString.c_str());
        		ShowMessage(strTemp);
        		return;
        	}
        	if (m_enState==EN_STATE_NEW)
        	{
		        szSQL = "insert into materialno (mn_code,mn_name,mn_usage,mn_rem,mn_ducode) values(";
	        	szSQL += Str2DBString(strID);
	        	szSQL += ",";	szSQL += Str2DBString(edtSpec->Text.c_str());
	        	szSQL += ",";	szSQL += Str2DBString(edtUsage->Text.c_str());
	        	szSQL += ",";	szSQL += Str2DBString(Memo1->Text.c_str());
                        szSQL += ",";   szSQL += Str2DBString(m_lstUnitsID[lstUnits->ItemIndex]);
	        	szSQL += ")";
        	}
        	else
        	{
        		szSQL = "update materialno set";
        		szSQL += " mn_name=";			szSQL += Str2DBString(edtSpec->Text.c_str());
        		szSQL += ",mn_usage=";			szSQL += Str2DBString(edtUsage->Text.c_str());
        		szSQL += ",mn_rem=";			szSQL += Str2DBString(Memo1->Text.c_str());
                        szSQL += ",mn_ducode=";                 szSQL += Str2DBString(m_lstUnitsID[lstUnits->ItemIndex]);
        		szSQL += " where mn_code=";     	szSQL += Str2DBString(strID);
        	}
        	try
        	{
        		RunSQL(dm1->Query1, szSQL);
        	}
        	catch(...)
             	{
        		sprintf(strTemp, "���ݿ����ʧ��!SQL=\n%s", szSQL);
              		ShowMessage(strTemp);
        		return;
        	}
        	if (m_enState==EN_STATE_NEW)
        	{
        		pItem = lstView->Items->Insert(0);
        		assert(pItem!=NULL);
        		pItem->Caption = strID;
        		pItem->SubItems->Add(edtSpec->Text);
        		pItem->SubItems->Add(edtUsage->Text);
        		pItem->SubItems->Add(Memo1->Text);
        		pItem->SubItems->Add(lstUnits->Text);
        	}
        	else
        	{
	        	pItem = lstView->Selected;
	        	if (pItem!=NULL)
        		{
        			pItem->SubItems->Strings[0] = edtSpec->Text;
        			pItem->SubItems->Strings[1] = edtUsage->Text;
        			pItem->SubItems->Strings[2] = Memo1->Text;
                		pItem->SubItems->Strings[3] = lstUnits->Text;
        		}
	        }
	        m_enState = EN_STATE_IDLE;
	        ResetCtrl();
	        if (btnNew->CanFocus())
                {
                	btnNew->SetFocus();
                }
        }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialEditForm::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if (Key==VK_LEFT || Key==VK_RIGHT)
        {
                EditorArrowHandle(ActiveControl,Key);
        }
	else if (Key!=VK_RETURN)
        {
                EditorKeyMaxLenHandle(ActiveControl,Key);
        }
}

//-------------------new 0525 ɾ����¼ʱ��ͬ��ɾ����codecheck����Ӧ��¼---------
void __fastcall TMaterialEditForm::btnMovClick(TObject *Sender)
{
	CString szSQL, szSQL1;
	char strID[80], strTemp[2048];
        TListItem *pItem;
	int nSel;

        if ((pItem = lstView->Selected)==NULL)
        {
        	return;
        }
	sprintf(strID, "%s%s-%s", edtCorp->Text.c_str(), edtID0->Text.c_str(), edtID1->Text.c_str());
	if (edtCorp->Text!=g_theOperator.op_cpcode && strcmp(g_theOperator.op_code, "admin")!=0)
	{
		ShowMessage("��ֻ���������޸��Լ���˾�Ĳ��ϱ��");
		if (edtCorp->CanFocus())
                {
                	edtCorp->SetFocus();
                }
		return;
	}
	szSQL.Format("select mn_code from materialno where mn_code=%s", Str2DBString(strID));
	RunSQL(dm1->Query1, szSQL, true);

	if (dm1->Query1->Eof)
	{
		ShowMessage("���ݿ���û�иñ�ŵĲ��ϱ��");
		if (edtID1->CanFocus())
                {
                	edtID1->SetFocus();
                }
		return;
	}

	sprintf(strTemp, "��Ҫɾ�����Ϊ��%s���Ĳ��ϱ����", strID);
	if (Application->MessageBox(strTemp,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
        {
                return;
        }

	try
	{
		szSQL.Format("delete materialno where mn_code=%s", Str2DBString(strID));
		RunSQL(dm1->Query1, szSQL);
	}
	catch(...)
	{
		sprintf(strTemp, "���ݿ����ʧ�ܣ�SQL=\n%s", szSQL);
		ShowMessage(strTemp);
		return;
	}
        try
	{
		szSQL.Format("delete codecheck where cc_ocode=%s", Str2DBString(strID));//ɾ��codecheck�ж�Ӧ��¼
		RunSQL(dm1->Query1, szSQL);
	}
	catch(...)
	{
		sprintf(strTemp, "���ݿ����ʧ�ܣ�SQL=\n%s", szSQL);
		ShowMessage(strTemp);
		return;
	}

        try
	{
		szSQL.Format("delete codecheck where cc_ncode=%s", Str2DBString(strID));//ɾ��codecheck�ж�Ӧ��¼
		RunSQL(dm1->Query1, szSQL);
	}
	catch(...)
	{
		sprintf(strTemp, "���ݿ����ʧ�ܣ�SQL=\n%s", szSQL);
		ShowMessage(strTemp);
		return;
	}
        try
	{
		szSQL.Format("delete materiallook where ml_mncode=%s", Str2DBString(strID));//ɾ��materiallook�ж�Ӧ��¼
		RunSQL(dm1->Query1, szSQL);
	}
	catch(...)
	{
		sprintf(strTemp, "���ݿ����ʧ�ܣ�SQL=\n%s", szSQL);
		ShowMessage(strTemp);
		return;
	}

	lstView->Items->Delete(lstView->Items->IndexOf(pItem));
	ResetCtrl();
}
//---------------------------------------------------------------------------


/*
//------------------------------------------------------------------------------
void __fastcall TMaterialEditForm::btnMovClick(TObject *Sender)
{
	CString szSQL;
	char strID[80],strTemp[2048];
  TListItem *pItem;
	int nSel;

  if((pItem=lstView->Selected)==NULL)	return;
	sprintf(strID,"%s%s-%s",edtCorp->Text.c_str(),edtID0->Text.c_str(),edtID1->Text.c_str());
	if(edtCorp->Text!=g_theOperator.op_cpcode && strcmp(g_theOperator.op_code,"admin")!=0)
	{
		ShowMessage("��ֻ���������޸��Լ���˾�Ĳ��ϱ��");
		if(edtCorp->CanFocus())	edtCorp->SetFocus();
		return;
	}
	szSQL.Format("select mn_code from materialno where mn_code=%s",Str2DBString(strID));
	RunSQL(dm1->Query1,szSQL,true);
	if(dm1->Query1->Eof)
	{
		ShowMessage("���ݿ���û�иñ�ŵĲ��ϱ��");
		if(edtID1->CanFocus())	edtID1->SetFocus();
		return;
	}

	sprintf(strTemp,"��Ҫɾ�����Ϊ��%s���Ĳ��ϱ����",strID);
	if(Application->MessageBox(strTemp,"����",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)!=IDYES)
		return;

	try
	{
		szSQL.Format("delete materialno where mn_code=%s",Str2DBString(strID));
		RunSQL(dm1->Query1,szSQL);
	}
	catch(...)
	{
		sprintf(strTemp,"���ݿ����ʧ�ܣ�SQL=\n%s",szSQL);
		ShowMessage(strTemp);
		return;
	}
	lstView->Items->Delete(lstView->Items->IndexOf(pItem));
	ResetCtrl();
}
//---------------------------------------------------------------------------
*/
//------------------------------------------------------------------------------
void __fastcall TMaterialEditForm::Timer1Timer(TObject *Sender)
{
        String str;
        TDateTime date1;
        date1=Date();
        str=date1;
        edtDATE->Text=str;
}

void __fastcall TMaterialEditForm::btndqClick(TObject *Sender)
{
        if (RB1->Checked==false && RB2->Checked==false)
        {
                ShowMessage("��ѡ�����ڲ�ѯ����!");
                return;
        }
        m_enState = EN_STATE_DATE;
        ResetCtrl();
}
//---------------------------------------------------------------------------

void __fastcall TMaterialEditForm::edtID1NewChange(TObject *Sender)
{
        int i;
	TListItem *pItem;

	if (m_enClick!=EN_CLICK_NONE)
        {
        	return;
        }
	for (i=0; i<lstView->Items->Count; i++)
	{
		pItem = lstView->Items->Item[i];
		if (pItem->Caption==edtCorp->Text+edtID0->Text+"-"+edtID1->Text)
		{
			lstView->Selected = pItem;
			break;
		}
	}
	if(i==lstView->Items->Count)
        {
                lstView->Selected = NULL;
        }
        ResetCtrl();
}

//----------------NEW 0525 �¾ɲ�ѯʱ���������ʱ�Զ�ת��Ϊ�Լ��Ĺ�˾-----------
void __fastcall TMaterialEditForm::btnNOClick(TObject *Sender)
{
        m_enState = EN_STATE_QUERY;
	ResetCtrl();
	if (edtID0->Text.IsEmpty())
        {
        	edtID0->SetFocus();
        }
	else
        {
        	edtID1->SetFocus();
        }
}
//---------------------------------------------------------------------------

void __fastcall TMaterialEditForm::btnQueryBatClick(TObject *Sender)
{
	Variant vExcel, vSheet;
        int nRow,i;
        TListItem *pItem;

        try
        {
                nRow = 1;
		if (!OpenExcel("���ϱ�Ų�ѯ.xls", vExcel, vSheet))
                {
                	return;
                }
		ListHead2Excel(lstView, vSheet, nRow++);
		for (i=0; i<lstView->Items->Count; i++)
                {
    	                pItem = lstView->Items->Item[i];
                        InsertExcelItem(vSheet, nRow++,
      	                pItem->Caption.c_str(),
                        pItem->SubItems->Strings[0].c_str(),
                        pItem->SubItems->Strings[1].c_str(),
                        pItem->SubItems->Strings[2].c_str(),
                        pItem->SubItems->Strings[3].c_str(),
                        pItem->SubItems->Strings[4].c_str(),
				NULL);
		}
	}
	catch(...)
	{
	}
}
//---------------------------------------------------------------------------

void __fastcall TMaterialEditForm::Button1Click(TObject *Sender)
{
        Variant vExcel,vSheet;
        int nRow, i;
        TListItem *pItem;
        try
        {
                nRow = 1;
                if (!OpenExcel("���ϱ�Ų�ѯ.xls", vExcel, vSheet))
                {
                	return;
                }
                ListHead2Excel(lstView, vSheet, nRow++);
                for(i=0; i<lstView->Items->Count; i++)
                {
                        pItem = lstView->Items->Item[i];
                        InsertExcelItem(vSheet, nRow++,
                        pItem->Caption.c_str(),
                        pItem->SubItems->Strings[0].c_str(),
                        pItem->SubItems->Strings[1].c_str(),
                        pItem->SubItems->Strings[2].c_str(),
                        pItem->SubItems->Strings[3].c_str(),
                        pItem->SubItems->Strings[4].c_str(),
                                                NULL);
                }
        }
        catch(...)
        {
        }
}
//---------------------------------------------------------------------------












