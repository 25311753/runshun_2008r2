#ifndef TConstH
#define TConstH
//---------------------------------------------------------------------------
extern AnsiString  corpcode; //��˾��
extern AnsiString  operatorcode,operatorname,operator_class,sysdate,systime;
extern void  WriteInfoFile(AnsiString  fname, AnsiString info);
extern AnsiString  ReadInfoFile(AnsiString  fname);
extern bool  isMasterSrv;
//---------------------------------------------------------------------------
#endif


