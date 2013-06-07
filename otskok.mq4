//+------------------------------------------------------------------+
//|                                                       otskok.mq4 |
//|                                                                  |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

#import "wininet.dll"
  int InternetAttemptConnect (int x);
  int InternetOpenA(string sAgent, int lAccessType, string sProxyName = "", string sProxyBypass = "", int lFlags = 0);
  int InternetOpenUrlA(int hInternetSession, string sUrl, string sHeaders = "", int lHeadersLength = 0, int lFlags = 0, int lContext = 0);
  int InternetReadFile(int hFile, int& sBuffer[], int lNumBytesToRead, int& lNumberOfBytesRead[]);
  int InternetCloseHandle(int hInet);
#import

//+------------------------------------------------------------------+
//| expert initialization function                                   |
//+------------------------------------------------------------------+
int start()
  {
  ObjectsDeleteAll();
//----
/*  string date[111];
  string val[111];
  string op[111];
  string priceopencurbar[111];
  string priceopen[111];
  string tp[111];
  string pips[111];
  string tg[111];


  int handle;
  int cBuffer[1110];
  int dwBytesRead[1]; 
  handle=FileOpen("response.txt", FILE_BIN|FILE_READ);dwBytesRead[0]=FileSize(handle) ;
  if(handle>0)
    {
     FileReadArray(handle, cBuffer, 0, 1110);
     FileClose(handle);
    }
      string text = ""; int i;  
      for( i = 0; i < 1110; i++)
      {
         text = text + CharToStr(cBuffer[i] & 0x000000FF);
         if(StringLen(text) == dwBytesRead[0]) break;
         text = text + CharToStr(cBuffer[i] >> 8 & 0x000000FF);
         if(StringLen(text) == dwBytesRead[0]) break;
         text = text + CharToStr(cBuffer[i] >> 16 & 0x000000FF);
         if(StringLen(text) == dwBytesRead[0]) break;
         text = text + CharToStr(cBuffer[i] >> 24 & 0x000000FF);
      }
      int index,index0=0,ix=0;i = 0;string s1,s2,s3[9];
      while( index0 < StringLen(text))
      {
         index=StringFind(text, "\r\n", index0);
         s1=StringSubstr(text,index0,(index-index0));index0=index+2;

           for(i=0;i<9;i++){
             s3[i]=" "+StringSubstr(s1,0,StringFind(s1, " ", 0));s2=StringSubstr(s1,StringFind(s1, " ",0)+1);s1=s2;}
           
           date[ix]=s3[0]+""+s3[1];
           val[ix]=s3[2];
           op[ix]=s3[3];
           priceopencurbar[ix]=s3[4];
           priceopen[ix]=s3[5];
           tp[ix]=s3[6];
           pips[ix]=s3[7];
           tg[ix]=s3[8];
         Print(StringConcatenate(date[ix],val[ix],op[ix],priceopencurbar[ix],priceopen[ix],tp[ix],pips[ix],tg[ix]));
         ix++;
      }
*/
   string url="http://127.0.0.1/otskok";
   int rv = InternetAttemptConnect(0);
   if(rv != 0)Alert("Ошибка при вызове InternetAttemptConnect()");
   int hInternetSession = InternetOpenA("ie", 0, "0", "0", 0);
   if(hInternetSession <= 0)Alert("Ошибка при вызове InternetOpenA()");
   int hURL = InternetOpenUrlA(hInternetSession, url, "", 0, 0, 0);
   if(hURL <= 0)Alert("Ошибка при вызове InternetOpenUrlA()");

   int cBuffer[1256];
   int dwBytesRead[1]; 
   string TXT = "";
  // while(!IsStopped())
  // {
      for(int i = 0; i<1256; i++) cBuffer[i] = 0;
      bool bResult = InternetReadFile(hURL, cBuffer, 1118, dwBytesRead);
      string text = "";   
      for(i = 0; i < 450; i++)
      {
         text = text + "" + CharToStr(cBuffer[i] & 0x000000FF);
         if(StringLen(text) == dwBytesRead[0]) break;
         text = text + ""+ CharToStr(cBuffer[i] >> 8 & 0x000000FF);
         if(StringLen(text) == dwBytesRead[0]) break;
         text = text + ""+ CharToStr(cBuffer[i] >> 16 & 0x000000FF);
         if(StringLen(text) == dwBytesRead[0]) break;
         text = text + ""+ CharToStr(cBuffer[i] >> 24 & 0x000000FF);
      }
      TXT = TXT+ " " + text+ " " + dwBytesRead[1];
  //    Sleep(1);
  // }
   if((TXT == "")||(TXT == " ")) Alert("Нет считанных данных");
   InternetCloseHandle(hInternetSession);
   
//   Print(TXT);
  string date[111];
  string val[111];
  string op[111];
  string priceopencurbar[111];
  string priceopen[111];
  string tp[111];
  string pips[111];
  string tg[111];


  /*int handle;
  int cBuffer[1110];
  int dwBytesRead[1]; 
  handle=FileOpen("response.txt", FILE_BIN|FILE_READ);dwBytesRead[0]=FileSize(handle) ;
  if(handle>0)
    {
     FileReadArray(handle, cBuffer, 0, 1110);
     FileClose(handle);
    }
      string text = ""; int i;  */
      /*for( i = 0; i < 1110; i++)
      {
         text = text + CharToStr(cBuffer[i] & 0x000000FF);
         if(StringLen(text) == dwBytesRead[0]) break;
         text = text + CharToStr(cBuffer[i] >> 8 & 0x000000FF);
         if(StringLen(text) == dwBytesRead[0]) break;
         text = text + CharToStr(cBuffer[i] >> 16 & 0x000000FF);
         if(StringLen(text) == dwBytesRead[0]) break;
         text = text + CharToStr(cBuffer[i] >> 24 & 0x000000FF);
      }*/
      int index,index0=0,ix=0,y=8,x=8;i = 0;string s1,s2,s3[9],prevvaldate="";
      if(StringLen(text)>1)
      while( index0 < StringLen(text))
      {
         index=StringFind(text, "\r\n", index0);
         s1=StringSubstr(text,index0,(index-index0));index0=index+2;

           for(i=0;i<9;i++){
             s3[i]=" "+StringSubstr(s1,0,StringFind(s1, " ", 0));s2=StringSubstr(s1,StringFind(s1, " ",0)+1);s1=s2;}
           
           date[ix]=s3[0]+""+s3[1];
           val[ix]=s3[2];
           op[ix]=s3[3];
           priceopencurbar[ix]=s3[4];
           priceopen[ix]=s3[5];
//           tp[ix]=s3[6];
           tp[ix]=StringSubstr(s3[6],4,StringLen(s3[6]));
           pips[ix]=s3[7];
           tg[ix]=s3[8];
 
      string mode=val[ix];int oplimit=OP_SELLLIMIT;color clr=Red;double stoploss=0.0,takeprofit=0.0;
      if(op[ix]==" buylimit"){clr=Blue;oplimit=OP_BUYLIMIT;takeprofit=StrToDouble(tp[ix]);}
      if(op[ix]==" selllimit"){clr=Red;oplimit=OP_SELLLIMIT;takeprofit=StrToDouble(tp[ix]);}
      takeprofit=StrToDouble(tp[ix]);
      if(op[ix]==" buystop"){clr=Blue;oplimit=OP_BUYSTOP;}//stoploss=StrToDouble(priceopen[ix])-MathAbs(StrToDouble(priceopen[ix])-StrToDouble(tp[ix]))*3;}
      if(op[ix]==" sellstop"){clr=Red;oplimit=OP_SELLSTOP;}//stoploss=StrToDouble(priceopen[ix])+MathAbs(StrToDouble(priceopen[ix])-StrToDouble(tp[ix]))*3;}
      
      double akb=AccountBalance()/5;if(!IsConnected())akb=10000.0;
      OrderSend(StringTrimLeft(val[ix]),oplimit,NormalizeDouble(MathAbs((akb-300)/MarketInfo(StringTrimLeft(val[ix]),MODE_MARGINREQUIRED)),1),StrToDouble(priceopen[ix]),3,stoploss,takeprofit,"otskok",0,StrToTime(TimeToStr(TimeCurrent(),TIME_DATE|TIME_MINUTES))+7200,CLR_NONE);
           WindowRedraw();Sleep(500);
      
      x=100;if(ObjectFind(mode)<0)
      ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
      ObjectSetText(mode,StringConcatenate(val[ix],priceopen[ix]," tp:",tp[ix],pips[ix],tg[ix]), 8, "Arial Narrow", clr);//,op[ix],priceopencurbar[ix]

      mode=""+mode+"datetime";clr=Black;if(prevvaldate!=date[ix])clr=White;prevvaldate=date[ix];
      x=10;if(ObjectFind(mode)<0)
      ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
      ObjectSetText(mode,date[ix], 8, "Arial Narrow", clr);

      y+=12;            
         Print(StringConcatenate(date[ix],val[ix],op[ix],priceopencurbar[ix],priceopen[ix],tp[ix],pips[ix],tg[ix]));
         ix++;
      }
      else {
         ObjectCreate("error", OBJ_LABEL, 0,0,0);ObjectSet("error", OBJPROP_CORNER, 0);ObjectSet("error", OBJPROP_XDISTANCE, 8);ObjectSet("error", OBJPROP_YDISTANCE, 8);
         ObjectSetText("error", "0 signals from otskok",8, "Arial Narrow", Red);
      }
//----
   return(0);
  }
//+------------------------------------------------------------------+