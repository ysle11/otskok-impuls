/*#import "wininet.dll"
  int InternetAttemptConnect (int x);
  int InternetOpenA(string sAgent, int lAccessType, string sProxyName = "", string sProxyBypass = "", int lFlags = 0);
  int InternetOpenUrlA(int hInternetSession, string sUrl, string sHeaders = "", int lHeadersLength = 0, int lFlags = 0, int lContext = 0);
  int InternetReadFile(int hFile, int& sBuffer[], int lNumBytesToRead, int& lNumberOfBytesRead[]);
  int InternetCloseHandle(int hInet);
#import*/

string mode;
string sms[]={"EURUSD","USDCHF","GBPUSD","USDJPY","USDCAD","AUDUSD","NZDUSD","GBPJPY","CHFJPY","EURJPY","GBPCHF","EURGBP","USDSGD"};
int i,i1,i2,i3;
int tfs[]={PERIOD_M15};//,PERIOD_H1,PERIOD_H4
color clr;
bool fresh=true,allfresh=true;
datetime servertime,tc,prevbuildtime;
//+------------------------------------------------------------------+
//| script program start function                                    |
//+------------------------------------------------------------------+
int start()
  {
/*while(true){
datetime ctime=StrToTime(TimeToStr(TimeCurrent(),TIME_DATE|TIME_MINUTES));
if((prevbuildtime!=ctime)&&((TimeMinute(ctime)==0)||(TimeMinute(ctime)==15)||(TimeMinute(ctime)==30)||(TimeMinute(ctime)==45))){
   prevbuildtime=ctime;*/
   ObjectsDeleteAll();WindowRedraw();
//----
      mode="MODE_TIME"; 
      datetime maxticktime=0;
      for(i=0;i<ArraySize(sms);i++)if(maxticktime<MarketInfo( ""+sms[i], MODE_TIME))maxticktime=MarketInfo( ""+sms[i], MODE_TIME);
      if(ObjectFind(mode)<0)ObjectCreate(mode,OBJ_LABEL,0,0,0);
         ObjectSetText(mode,TimeToStr(maxticktime,TIME_DATE|TIME_SECONDS), 8, "Arial Narrow", White);
         ObjectSet(mode, OBJPROP_CORNER, 0);
         ObjectSet(mode, OBJPROP_XDISTANCE, 11);ObjectSet(mode, OBJPROP_YDISTANCE, 1);


   int y=10,x=10;
   for(i=0;i<ArraySize(sms);i++)
   {
      mode=""+sms[i]; 
      ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,mode+":", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   }
   y=0;
   for(i=0;i<ArraySize(sms);i++)
   {
      y+=10;x=100;
      for(i1=0;i1<ArraySize(tfs);i1++)
      { 
      mode=""+sms[i]+DoubleToStr(tfs[i1],0); 
      if(ObjectFind(mode)<0)ObjectCreate(mode,OBJ_LABEL,0,0,0);
         ObjectSetText(mode,"0", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
      x+=110;}
   }
   allfresh=false;RefreshRates();i3=0;
   while(!allfresh){allfresh=true;          
         for(i=0;i<ArraySize(sms);i++)
         {  
            for(i1=0;i1<ArraySize(tfs);i1++)
            {
               tc=TimeCurrent();
               switch(tfs[i1]){
                  case PERIOD_MN1:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+".01 00:00");break;
                  case PERIOD_W1:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" 00:00");break;
                  case PERIOD_D1:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" 00:00");break;
                  case PERIOD_H4:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" "+TimeHour(tc)+":00");break;
                  case PERIOD_H1:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" "+TimeHour(tc)+":00");break;
                  case PERIOD_M15:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" "+TimeHour(tc)+":"+TimeMinute(tc));break;
               }
               mode=""+sms[i]+DoubleToStr(tfs[i1],0);
               fresh=true;for(i2=0;i2<200;i2++)
                           if((TimeDayOfWeek(servertime-tfs[i1]*i2*15)>0)&&(TimeDayOfWeek(servertime-tfs[i1]*i2*15)<6)&&tfs[i1]<PERIOD_W1&&tfs[i1]>PERIOD_M5)
                            if(iBarShift(sms[i],tfs[i1],servertime-tfs[i1]*i2*15,true)<0&&fresh){fresh=false;allfresh=false;break;}
               clr=DimGray;if(iBars(sms[i],tfs[i1])>200 && fresh)clr=Green;
               string t1=TimeToStr(iTime(sms[i],tfs[i1],0));
               ObjectSetText(mode,""+StringSubstr(t1,5) +"   "+iBars(sms[i],tfs[i1]), 8, "Arial Narrow", clr);
            }
         }   
      for(i=0;i<ArraySize(sms);i++)if(maxticktime<MarketInfo( ""+sms[i], MODE_TIME))maxticktime=MarketInfo( ""+sms[i], MODE_TIME);
      ObjectSetText("MODE_TIME",TimeToStr(maxticktime,TIME_DATE|TIME_SECONDS)+"  M15", 8, "Arial Narrow", White);
         
         WindowRedraw();if(IsStopped())return(0);Sleep(100);i3++;if(i3>30){i3=0;RefreshRates();}
   }
//---   
/*Sleep(52000);
while(!IsConnected())Sleep(4000);
signals();

   }Sleep(400);}*/
   
   return(0);
  }
//+------------------------------------------------------------------+

/*
void signals()
  {
  
//----

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
   if(TXT == "") Alert("Нет считанных данных");
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


      int index,index0=0,ix=0,y=150,x=8;i = 0;string s1,s2,s3[9],prevvaldate="";
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
 
      string mode=val[ix];int oplimit=OP_SELLLIMIT;color clr=Red;if(op[ix]==" buylimit"){clr=Blue;oplimit=OP_BUYLIMIT;}
      OrderSend(StringTrimLeft(val[ix]),oplimit,0.1,StrToDouble(priceopen[ix]),3,0,StrToDouble(tp[ix]),"otskok",0,StrToTime(TimeToStr(TimeCurrent(),TIME_DATE|TIME_MINUTES))+900,CLR_NONE);
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

}*/