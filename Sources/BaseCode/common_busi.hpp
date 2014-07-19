#ifndef  _buis_common_util_
#define  _buis_common_util_

#include <vcl.h>
#include "common.hpp"

bool chkFormatContainerNo(AnsiString ss){
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

//��װ��װ����Ϣ:������|��� ���� ������#��� ���� ������#@��
CString GetContainerInfo(TListView *lv){
        CString szContInfo;
        szContInfo += (IntToStr(lv->Items->Count)).c_str();
        szContInfo += '|';
        for (int i=0; i<lv->Items->Count; ++i) {  
                szContInfo += lv->Items->Item[i]->Caption.c_str();
                szContInfo += ' ';
                szContInfo += lv->Items->Item[i]->SubItems->Strings[0].c_str();
                szContInfo += ' ';
                szContInfo += lv->Items->Item[i]->SubItems->Strings[1].c_str();
                szContInfo += '#';
        }
        szContInfo += '@';
        return szContInfo;
}

//��editת��money
double edt2money(TEdit *edt, int decimal_place=0){
        double rt = 0;
        AnsiString s = edt->Text;
        try{
                if (s.IsEmpty()){
                        return 0;
                }
                if (isMoney(s, decimal_place)){
                        TryStrToFloat(s, rt);
                }
        }catch(...){
        }
        return rt;
}
//��AnsiStringת��money
double str2money(AnsiString s, int decimal_place=0){
        double rt = 0;
        try{
                if (s.IsEmpty()){
                        return 0;
                }
                if (isMoney(s, decimal_place)){
                        TryStrToFloat(s, rt);
                }
        }catch(...){
        }
        return rt;
}
#endif