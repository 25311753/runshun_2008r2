//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DataModule.h"
#include "BaseCode.h"
#include "PrnStuffBookList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPrnStuffBookListForm *PrnStuffBookListForm;
//---------------------------------------------------------------------------
__fastcall TPrnStuffBookListForm::TPrnStuffBookListForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPrnStuffBookListForm::QRDBText9Print(TObject *sender,AnsiString &Value)
{
	CStringArray lstLine;
	//��һ�ַ����ֽ�ɶ���,width Ϊÿ������
	SplitStrToLines(Value.c_str(),30,lstLine);
	if(lstLine.GetSize()>=2)
	{
		Value=(LPCSTR)lstLine[0];
		txtName2->Caption=(LPCSTR)lstLine[1];
	}
	else
	{
		txtName2->Caption="";
	}
}
//---------------------------------------------------------------------------

