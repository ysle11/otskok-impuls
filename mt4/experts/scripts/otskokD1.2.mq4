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
int magic=1440;
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
/*   string url="http://127.0.0.1/otskok";
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
 */  
//   Print(TXT);
  string date[111];
  string val[111];
  string op[111];
  double priceopencurbar[111];
  double priceopen[111];
  int sigcnt[111];

  



  int handle;
  int cBuffer[1110];
  int dwBytesRead[1]; 
  handle=FileOpen("responseD1.txt", FILE_BIN|FILE_READ);dwBytesRead[0]=FileSize(handle) ;
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
      int index,index0=0,ix=0,y=8,x=8,tix= 0;i = 0;string s1,s2,s3[9],prevvaldate="";x=10;y=20;
      if(StringLen(text)>1){
      while( index0 < StringLen(text))
      {
         index=StringFind(text, "\r\n", index0);
         s1=StringSubstr(text,index0,(index-index0));index0=index+2;

           for(i=0;i<7;i++){
             s3[i]=" "+StringSubstr(s1,0,StringFind(s1, " ", 0));s2=StringSubstr(s1,StringFind(s1, " ",0)+1);s1=s2;}
           
           date[ix]=s3[0]+""+s3[1];
           val[ix]=StringTrimLeft(s3[2]);
           op[ix]=StringTrimLeft(s3[3]);
           priceopencurbar[ix]=StrToDouble(StringTrimLeft(s3[4]));
           priceopen[ix]=StrToDouble(StringTrimLeft(StringSubstr(s3[5],0,StringLen(s3[5])-1)));
           sigcnt[ix]=StrToInteger(StringTrimLeft(s3[6]));
           string mode=val[ix],added="";int op1;double lots,otp1,pnt=MarketInfo(val[ix],MODE_POINT);color clr=Blue;
           if(op[ix]=="buy"){op1=OP_BUYSTOP;otp1=priceopen[ix]+pnt*12+pnt*MarketInfo(val[ix],MODE_SPREAD);}else if(op[ix]=="sell"){op1=OP_SELLSTOP;otp1=priceopen[ix]-pnt*12-pnt*MarketInfo(val[ix],MODE_SPREAD);clr=Red;}
           lots=NormalizeDouble(MathAbs(AccountBalance()/1.04/MarketInfo(val[ix],MODE_MARGINREQUIRED)),1);
           if(lots<0.1)lots=0.1;if(!IsDemo())lots=0.01; 
           if(!isopened(val[ix],op1,priceopen[ix],otp1))
            {OrderSend(val[ix],op1,lots,priceopen[ix],3,0,otp1,NULL,magic,TimeCurrent()+(22-TimeHour(TimeCurrent()))*3600 );added="+";}
           Print(StringConcatenate(added," ",date[ix]," ",val[ix]," ",lots," ",op[ix]," ",priceopen[ix]," ",sigcnt[ix]));
            if(op[ix]=="buy")mode=mode+"_b";else if(op[ix]=="sell")mode=mode+"_s";
            if(ObjectFind(mode)<0)
            ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
            ObjectSetText(mode,StringConcatenate(added," ",val[ix]," ",lots," ",op[ix]," : ",priceopen[ix]), 8, "Arial Narrow", clr);//,op[ix],priceopencurbar[ix]
            y+=12;            

           ix++;
      }tix=ix;ix=0;}else {
         ObjectCreate("error", OBJ_LABEL, 0,0,0);ObjectSet("error", OBJPROP_CORNER, 0);ObjectSet("error", OBJPROP_XDISTANCE, 1);ObjectSet("error", OBJPROP_YDISTANCE, 1);
         ObjectSetText("error", "0 signals from otskok",8, "Arial Narrow", Red);
      }
            mode="datetime";clr=Black;if(prevvaldate!=date[ix])clr=White;prevvaldate=date[ix];
            if(ObjectFind(mode)<0)
            ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
            ObjectSetText(mode,date[ix], 8, "Arial Narrow", clr);

      /*double idx=0.0,cp;string cval="",tsig="";int coplimit=999;color cclr=Red;
      while( ix < tix)
      {
      double p,opp,curp=priceopen[ix],popen=priceopencurbar[ix],popen1;
      string mode=val[ix];int oplimit=999;color clr=Red;double stoploss=0.0,takeprofit=0.0;
      if(op[ix]=="buy"){
         clr=Blue;opp=priceopen[ix+1];
         p=MarketInfo(StringTrimLeft(val[ix]),MODE_BID);
         if(
         ((popen<curp&&popen<opp)&&(opp>0.0&&curp>0.0))
         ||((popen<curp)&&(opp==0.0&&curp>0.0))
         ||((popen<opp)&&(opp>0.0&&curp==0.0))
         )
         {oplimit=OP_BUY;tsig=sigcnt[ix]+"/"+sigcnt[ix+1];}else
         oplimit=999; 
         if(((curp-popen)>idx)&&(StrToInteger(sigcnt[ix])>0)&&(StrToInteger(sigcnt[ix])>StrToInteger(sigcnt[ix+1])))
            {
               coplimit=OP_BUY;
               idx=curp-popen;
               cval=val[ix];
               cp=p;
               cclr=Blue;
               popen1=popen;
            } 
      }else
      if(op[ix]=="sell"){
         clr=Red;opp=priceopen[ix-1];
         p=MarketInfo(StringTrimLeft(val[ix]),MODE_ASK);
         if(
         ((popen>curp&&popen>opp)&&(opp>0.0&&curp>0.0))
         ||((popen>curp)&&(opp==0.0&&curp>0.0))
         ||((popen>opp)&&(opp>0.0&&curp==0.0))
         )
         {oplimit=OP_SELL;tsig=sigcnt[ix]+"/"+sigcnt[ix-1];}else
         oplimit=999;
         if(((popen-curp)>idx)&&(StrToInteger(sigcnt[ix])>0)&&(StrToInteger(sigcnt[ix])>StrToInteger(sigcnt[ix-1])))
            {
               coplimit=OP_SELL;
               idx=popen-curp;
               cval=val[ix];
               cp=p;
               cclr=Red;
               popen1=popen;
            } 
      }
      if(curp==0.0)curp=opp;
      double lots;mode=StringConcatenate(mode,oplimit);
      while(IsTradeContextBusy())Sleep(100);
         if(oplimit<6){
            //OrderSend(StringTrimLeft(val[ix]),oplimit,lots,p,3,0,0,"",0,0,CLR_NONE);

            x=100;if(ObjectFind(mode)<0)
            ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
            ObjectSetText(mode,StringConcatenate(val[ix]," ",curp," : ",tsig," ",(MathAbs(iOpen(val[ix],0,0)-curp)/MarketInfo(val[ix],MODE_POINT)),"/",MathCeil((MathAbs(iOpen(val[ix],0,0)-curp)/MarketInfo(val[ix],MODE_POINT))-(MathAbs(((MarketInfo(val[ix],MODE_ASK)+MarketInfo(val[ix],MODE_BID))/2.0)-curp)/MarketInfo(val[ix],MODE_POINT))) ), 8, "Arial Narrow", clr);//,op[ix],priceopencurbar[ix]

            mode=""+mode+"datetime";clr=Black;if(prevvaldate!=date[ix])clr=White;prevvaldate=date[ix];
            x=10;if(ObjectFind(mode)<0)
            ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
            ObjectSetText(mode,date[ix], 8, "Arial Narrow", clr);

            y+=12;            
            Print(StringConcatenate(date[ix],val[ix],op[ix],priceopencurbar[ix]," ",priceopen[ix]," sigcnt:",sigcnt[ix]));
         }
         ix++;
      WindowRedraw();Sleep(250);
      }
      if(coplimit<6){
         lots=NormalizeDouble(MathAbs(AccountBalance()/1.04/MarketInfo(StringTrimLeft(cval),MODE_MARGINREQUIRED)),1);
         if(lots<0.1)lots=0.1;if(!IsDemo())lots=0.01;      
         y+=12;            
         OrderSend(cval,coplimit,lots,cp,3,0,0,"",0,0,cclr);
         mode="cval";
         x=100;if(ObjectFind(mode)<0)
         ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
         ObjectSetText(mode,StringConcatenate(cval," ",popen1," :",cp), 12, "Arial Narrow", cclr);

         Print(StringConcatenate(date[ix],val[ix],op[ix],priceopencurbar[ix]," ",priceopen[ix]," sigcnt:",sigcnt[ix]));
         
      }*/
      
//----
   return(0);
  }


bool isopened(string tval,int top,double tpriceopen,double totp1){
    bool opened=false;
    for(int o=OrdersTotal()-1;o>=0;o--){
      OrderSelect(o, SELECT_BY_POS, MODE_TRADES);
      if(OrderSymbol()==tval)if(OrderType()==top)if(OrderMagicNumber()==magic)
      {
         if(OrderOpenPrice()!=tpriceopen)OrderModify(OrderTicket(),tpriceopen,0,totp1,0);
         opened=true;
      }
    }   
   return (opened);

}