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
 
      string mode=val[ix];int oplimit=999;color clr=Red;
      double takeprofit=StrToDouble(tp[ix]);
      if(op[ix]==" buystop"){clr=Blue;oplimit=OP_BUYSTOP;}
      if(op[ix]==" sellstop"){clr=Red;oplimit=OP_SELLSTOP;}
      
      double akb=AccountFreeMargin()/1.5;if(!IsConnected())akb=10000.0;
      double lots;
      if(IsDemo())lots=NormalizeDouble(MathAbs((akb-300)/MarketInfo(StringTrimLeft(val[ix]),MODE_MARGINREQUIRED)),1);
      if(lots<0.1)lots=0.1;
      if(!IsDemo())lots=0.01;
      
      //closeall(StringTrimLeft(val[ix]),oplimit);
      //OrderSend(StringTrimLeft(val[ix]),oplimit,lots,StrToDouble(priceopen[ix]),3,0,takeprofit,"",0,StrToTime(TimeToStr(TimeCurrent(),TIME_DATE|TIME_MINUTES))+7200,CLR_NONE);
      opens(StringTrimLeft(val[ix]),oplimit,takeprofit);
           WindowRedraw();Sleep(250);
      
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
void opens(string sm,int OP,double tp){
   int Op_step=MarketInfo(sm,MODE_STOPLEVEL);
   int count=MathFloor(MathAbs(MarketInfo(sm,MODE_ASK)-tp)/MarketInfo(sm,MODE_POINT)/Op_step)-2;if(count>6)count=6;else if(count<7)count/=1.33;
   double initlot=(AccountFreeMargin( )/1.33)/ MarketInfo(sm,MODE_MARGINREQUIRED);initlot=initlot-MarketInfo(sm,MODE_LOTSTEP)*2;
//   if(OP==OP_BUYSTOP)OrderSend(sm,OP_BUY,NormalizeDouble(initlot,1),NormalizeDouble(MarketInfo(sm,MODE_ASK),MarketInfo(sm,MODE_DIGITS)),3,0,tp);
//   if(OP==OP_SELLSTOP)OrderSend(sm,OP_SELL,NormalizeDouble(initlot,1),NormalizeDouble(MarketInfo(sm,MODE_BID),MarketInfo(sm,MODE_DIGITS)),3,0,tp);
   if(OP==OP_BUYSTOP)Print(sm," ",OP_BUY," ",NormalizeDouble(initlot,1)," ",NormalizeDouble(MarketInfo(sm,MODE_ASK),MarketInfo(sm,MODE_DIGITS))," ",3,0," ",tp);
   if(OP==OP_SELLSTOP)Print(sm," ",OP_SELL," ",NormalizeDouble(initlot,1)," ",NormalizeDouble(MarketInfo(sm,MODE_BID),MarketInfo(sm,MODE_DIGITS))," ",3,0," ",tp);
   double boomlot=0,ilot,t=0;
   ilot=initlot;
   int i;
   boomlot=ilot*MarketInfo(sm,MODE_TICKVALUE)*MarketInfo(sm,MODE_STOPLEVEL)/ MarketInfo(sm,MODE_MARGINREQUIRED);
   ilot=ilot+boomlot;
   for (i=0;i<count;i++){
      boomlot=ilot*(MarketInfo(sm,MODE_TICKVALUE)*Op_step/ MarketInfo(sm,MODE_MARGINREQUIRED)-MarketInfo(sm,MODE_TICKVALUE)*MarketInfo(sm,MODE_SPREAD)/ MarketInfo(sm,MODE_MARGINREQUIRED));
      ilot=ilot+boomlot;
//      if(OP==OP_BUYSTOP)OrderSend(sm,OP_BUYSTOP,NormalizeDouble(boomlot,1),NormalizeDouble(MarketInfo(sm,MODE_ASK)+MarketInfo(sm,MODE_POINT)*Op_step*(i+1),MarketInfo(sm,MODE_DIGITS)),3,0,tp,NULL,0,StrToTime(TimeToStr(TimeCurrent(),TIME_DATE|TIME_MINUTES))+86000);
//      if(OP==OP_SELLSTOP)OrderSend(sm,OP_SELLSTOP,NormalizeDouble(boomlot,1),NormalizeDouble(MarketInfo(sm,MODE_BID)-MarketInfo(sm,MODE_POINT)*Op_step*(i+1),MarketInfo(sm,MODE_DIGITS)),3,0,tp,NULL,0,StrToTime(TimeToStr(TimeCurrent(),TIME_DATE|TIME_MINUTES))+86000);
      if(OP==OP_BUYSTOP)Print(sm," ",OP_BUYSTOP," ",NormalizeDouble(boomlot,1)," ",NormalizeDouble(MarketInfo(sm,MODE_ASK)+MarketInfo(sm,MODE_POINT)*Op_step*(i+1),MarketInfo(sm,MODE_DIGITS))," ",3,0," ",tp);
      if(OP==OP_SELLSTOP)Print(sm," ",OP_SELLSTOP," ",NormalizeDouble(boomlot,1)," ",NormalizeDouble(MarketInfo(sm,MODE_BID)-MarketInfo(sm,MODE_POINT)*Op_step*(i+1),MarketInfo(sm,MODE_DIGITS))," ",3,0," ",tp);

      t=t+boomlot*MarketInfo(sm,MODE_MARGINREQUIRED);Sleep(1000);RefreshRates();
   }
}
void closeall(string sm,int a=999){
   int o;
   if(a==OP_BUYSTOP||a==OP_BUYLIMIT){
    for(o=OrdersTotal()-1;o>=0;o--){
      OrderSelect(o, SELECT_BY_POS, MODE_TRADES);
      if(OrderSymbol()==sm&&OrderComment()=="impuls"){
       if(OrderType()==OP_BUYSTOP){OrderDelete(OrderTicket());continue;}
       if(OrderType()==OP_BUYLIMIT){OrderDelete(OrderTicket());continue;}
      }
    }   
   } else
   if(a==OP_SELLSTOP||a==OP_SELLLIMIT){
    for(o=OrdersTotal()-1;o>=0;o--){
      OrderSelect(o, SELECT_BY_POS, MODE_TRADES);
      if(OrderSymbol()==sm&&OrderComment()=="impuls"){
       if(OrderType()==OP_SELLSTOP){OrderDelete(OrderTicket());continue;}
       if(OrderType()==OP_SELLLIMIT){OrderDelete(OrderTicket());continue;}
      }
    }   
   }
}