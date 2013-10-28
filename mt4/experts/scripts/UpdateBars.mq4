/*#import "wininet.dll"
  int InternetAttemptConnect (int x);
  int InternetOpenA(string sAgent, int lAccessType, string sProxyName = "", string sProxyBypass = "", int lFlags = 0);
  int InternetOpenUrlA(int hInternetSession, string sUrl, string sHeaders = "", int lHeadersLength = 0, int lFlags = 0, int lContext = 0);
  int InternetReadFile(int hFile, int& sBuffer[], int lNumBytesToRead, int& lNumberOfBytesRead[]);
  int InternetCloseHandle(int hInet);
#import*/
#import "shell32.dll"
 int ShellExecuteA(int hWnd, string Operation, string FileName, string Parameters, string Directory, int ShowCmd);
#import
#import "libmysql.dll"
int mysql_init(int db);
int mysql_errno(int TMYSQL);
int mysql_real_connect(int TMYSQL, string host, string user, string password, 
                       string DB,int port,int socket,int clientflag);
int mysql_real_query(int TMSQL, string query, int length);
int mysql_store_result (int TMSQL);
string mysql_fetch_row (int TMSQL);
void mysql_free_result(int TMSQL);
void mysql_close(int TMSQL);
#import
string mode;
string sms[]={"EURUSD","USDCHF","GBPUSD","USDJPY","USDCAD","AUDUSD","NZDUSD","GBPJPY","CHFJPY","EURJPY","GBPCHF"};
int i,i1,i2,i3,secs=60;
int tfs[]={PERIOD_D1,PERIOD_H4,PERIOD_H1,PERIOD_M15};
color clr;
bool fresh=true,allfresh=true,tlimit[9],needupdatetable[9][50];
datetime servertime,tc,prevbuildtime,maxticktime,prevmaxticktime,inited[9][50];
datetime prevtime15,prevtime60,prevtime240,prevtime1440,time15,time60,time240,time1440;
int mysql,myerr;
int init(){
   return(0);
}
int start(){
   for(i=0;i<ArraySize(sms);i++)
   for(i1=0;i1<ArraySize(tfs);i1++){
       //inited[i1][i]=false;
       needupdatetable[i1][i]=false;
       initing(sms[i],tfs[i1],2);
   }
   tlimiting(PERIOD_D1,1,100);
   tlimiting(PERIOD_H4,1,200);
   tlimiting(PERIOD_H1,1,2000);
   tlimiting(PERIOD_M15,1,500);
   prevbuildtime=0;
while(true)if((TimeDayOfWeek(TimeLocal())>0)&&(TimeDayOfWeek(TimeLocal())<6))
{
tc=TimeLocal();
datetime ctime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" "+TimeHour(tc)+":"+DoubleToStr(MathFloor(TimeMinute(tc)/15.0)*15.0,0));
if(prevbuildtime!=ctime){
   if(prevbuildtime!=0){
      ShellExecuteA(0, "", "route.bat", "on", "Z:\home\localhost\www\\", 0);
      Sleep(10000);
      if(!IsConnected())ShellExecuteA(0, "", "rasdial.exe", "IT", "f:\WINDOWS\system32\\", 0);
   }
   prevbuildtime=ctime;
   ObjectsDeleteAll();WindowRedraw();
//----
   mode="MODE_TIME"; 
   maxticktime=0;
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
      y+=10;
      for(i1=0;i1<ArraySize(tfs);i1++)
      { 
         if(tfs[i1]==PERIOD_M15)x=55;else
         if(tfs[i1]==PERIOD_H1)x=160;else
         if(tfs[i1]==PERIOD_H4)x=265;else
         if(tfs[i1]==PERIOD_D1)x=365;
         mode=""+sms[i]+DoubleToStr(tfs[i1],0); 
         if(ObjectFind(mode)<0)ObjectCreate(mode,OBJ_LABEL,0,0,0);
         ObjectSetText(mode,"0", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
      }
   }
   allfresh=false;RefreshRates();i3=0;
   while(!allfresh){allfresh=true;          
         for(i1=0;i1<ArraySize(tfs);i1++)
         {  bool tfresh=true;
            for(i=0;i<ArraySize(sms);i++)
            {
               tc=TimeLocal();
               switch(tfs[i1]){
                  case PERIOD_MN1:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+".01 00:00");break;
                  case PERIOD_W1:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" 00:00");break;
                  case PERIOD_D1:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" 00:00");break;
                  case PERIOD_H4:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" "+DoubleToStr(MathFloor(TimeHour(tc)/4.0)*4.0,0)+":00");break;
                  case PERIOD_H1:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" "+TimeHour(tc)+":00");break;
                  case PERIOD_M30:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" "+TimeHour(tc)+":"+DoubleToStr(MathFloor(TimeMinute(tc)/30.0)*30.0,0));break;
                  case PERIOD_M15:servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" "+TimeHour(tc)+":"+DoubleToStr(MathFloor(TimeMinute(tc)/15.0)*15.0,0));break;
               }
               string t1;
               mode=""+sms[i]+DoubleToStr(tfs[i1],0);
               if(servertime==iTime(sms[i],tfs[i1],0)&&initing(sms[i],tfs[i1],0,servertime)){
                  clr=MediumSeaGreen;
                  t1=TimeToStr(iTime(sms[i],tfs[i1],0));
                  ObjectSetText(mode,""+StringSubstr(t1,5) +"   "+iBars(sms[i],tfs[i1]), 8, "Arial Narrow", clr);WindowRedraw();
               }else 
               {  
                  fresh=true;for(i2=0;i2<tlimiting(tfs[i1]);i2++)
                              if((TimeDayOfWeek(servertime-i2*tfs[i1]*secs)>0)&&(TimeDayOfWeek(servertime-i2*tfs[i1]*secs)<6))
                               if(iBarShift(sms[i],tfs[i1],servertime-i2*tfs[i1]*secs,true)<0&&fresh){fresh=false;allfresh=false;break;}
                  clr=Red;if(iBars(sms[i],tfs[i1])>tlimiting(tfs[i1]) && fresh)clr=Green;
                  initing(sms[i],tfs[i1],1,servertime);
                  needupdatetable[i1][i]=true;
                  t1=TimeToStr(iTime(sms[i],tfs[i1],0));
                  ObjectSetText(mode,""+StringSubstr(t1,5) +"   "+iBars(sms[i],tfs[i1]), 8, "Arial Narrow", clr);WindowRedraw();
                  Sleep(20);
                  tfresh=false;
               }
            }
            for(i=0;i<ArraySize(sms);i++)if(maxticktime<MarketInfo(""+sms[i],MODE_TIME))maxticktime=MarketInfo(""+sms[i], MODE_TIME);
            ObjectSetText("MODE_TIME",TimeToStr(servertime,TIME_DATE|TIME_SECONDS)+"  "+tfs[i1], 8, "Arial Narrow", White);WindowRedraw();
            if(tfresh){Sleep(100);RefreshRates();}
         }   
         
         WindowRedraw();if(IsStopped())return(0);Sleep(100);i3++;if(i3>30){i3=0;RefreshRates();}
   }
      if(IsConnected())ShellExecuteA(0, "", "route.bat", "off", "Z:\home\localhost\www\\", 0);
      for(i=0;i<ArraySize(sms);i++)
      for(i1=0;i1<ArraySize(tfs);i1++){
          if(needupdatetable[i1][i])
          {
            needupdatetable[i1][i]=false;
            updatetable(sms[i],tfs[i1]);
          }
      }
   }
   ObjectSetText("MODE_TIME",TimeToStr(TimeLocal(),TIME_DATE|TIME_SECONDS), 8, "Arial Narrow", White);WindowRedraw();
   if(IsConnected())ShellExecuteA(0, "", "route.bat", "off", "Z:\home\localhost\www\\", 0);
   if(IsStopped())return(0);
   Sleep(1000);
   }
   return(0);
}
bool initing(string symbol,int period,int set=0,datetime curtime=0){
   int ci,ci1;
   if(set==0){
      for(ci=0;ci<ArraySize(sms);ci++)
      for(ci1=0;ci1<ArraySize(tfs);ci1++)
       if(tfs[ci1]==period&&sms[ci]==symbol)
         if(inited[ci1][ci]==curtime)return(true);
      inited[ci1][ci]=curtime;
      return(false);
   }else
   if(set==1){
      for(ci=0;ci<ArraySize(sms);ci++)
      for(ci1=0;ci1<ArraySize(tfs);ci1++)
       if(tfs[ci1]==period&&sms[ci]==symbol)
         {inited[ci1][ci]=curtime;return(true);}
      return(false);
   }else
   if(set==2){
      for(ci=0;ci<ArraySize(sms);ci++)
      for(ci1=0;ci1<ArraySize(tfs);ci1++)
       if(tfs[ci1]==period&&sms[ci]==symbol)
         {inited[ci1][ci]=0;return(true);}
      return(false);
   }
}
int tlimiting(int period,int set=0,int value=200){
   int ci1;
   if(set==0){
      for(ci1=0;ci1<ArraySize(tfs);ci1++)
       if(tfs[ci1]==period)
         return(tlimit[ci1]);
      return(0);
   }else
   if(set==1){
      for(ci1=0;ci1<ArraySize(tfs);ci1++)
       if(tfs[ci1]==period)
         {tlimit[ci1]=value;return(value);}
      return(0);
   }
}
void updatetable(string symbol,int period){
   mysql = mysql_init(mysql);
   if(mysql == 0)Print("mysql allocating error");
   string host = "127.0.0.1";
   string user = "root";
   string password = "";
   string DB = "admin";
   int clientflag = 0;
   int port = 3306;
   string socket = "";
   int res = mysql_real_connect(mysql,host,user,password,DB,port,socket,clientflag);
   int err = GetLastError();
   if(res != mysql)Print("error=", mysql, " ", mysql_errno(mysql), " ");

   string query = "",fetch = "";
   int length = 0,y,myerr;datetime time;

   query = "SELECT ctm FROM tvals WHERE `valname`='"+symbol+"' AND `timeframe`="+period+";";
   length = StringLen(query);
   y=mysql_real_query(mysql, query, length);y=mysql_store_result(mysql);fetch=mysql_fetch_row(y);mysql_free_result(y);
   time=StrToTime(StringSubstr(fetch,8,StringLen(fetch)));
   int needbars=iBarShift(symbol,period,time);
   if(StringSubstr(fetch,8,StringLen(fetch))==""){
      query = "SELECT MAX(`ctm`) FROM mt4 WHERE `valname`='"+symbol+"' AND `timeframe`="+period+";";
      length = StringLen(query);
      y=mysql_real_query(mysql, query, length);y=mysql_store_result(mysql);fetch=mysql_fetch_row(y);mysql_free_result(y);
      time=StrToTime(StringSubstr(fetch,8,StringLen(fetch)));
      needbars=iBarShift(symbol,period,time);
      if(StringSubstr(fetch,8,StringLen(fetch))=="")needbars=iBars(symbol,period);else{
         query = "INSERT INTO tvals(id,valname,timeframe,ctm) VALUES ('','"+symbol+"',"+period+",'"+StringSubstr(fetch,8,StringLen(fetch))+"');";
         length = StringLen(query); 
         y=mysql_real_query(mysql, query, length);     
      }
   }
   Print(symbol + " " + period + " need bars=",needbars);
   Print(symbol + " " + period + " time=",StringSubstr(fetch,8,StringLen(fetch)));
   myerr = mysql_errno(mysql);if(myerr > 0)Print("error=",myerr);

   for(int i=needbars-1;i>=0;i--){
   query = StringConcatenate("insert into mt4(id,valname,timeframe,ctm,open,low,high,close,volume) values(",
                             "'','", symbol, "','", period, "','", 
                             TimeToStr(iTime(symbol,period,i)), "',", 
                             iOpen(symbol,period,i), ",", iLow(symbol,period,i),",",
                             iHigh(symbol,period,i), ",", iClose(symbol,period,i),",",
                             iVolume(symbol,period,i), ");");
   length = StringLen(query);
//   Print("query=",query);
   mysql_real_query(mysql, query, length);myerr = mysql_errno(mysql);if(myerr > 0)Print("error=",myerr);
   query = StringConcatenate("update tvals set `ctm`='", 
                             TimeToStr(iTime(symbol,period,i)), 
                             "' WHERE `valname`='"+symbol+"' AND `timeframe`="+period+";");
   length = StringLen(query);
//   Print("query=",query);
   mysql_real_query(mysql, query, length);myerr = mysql_errno(mysql);if(myerr > 0)Print("error=",myerr);
   }
   query = "";fetch = "";
   length = 0;


   mysql_close(mysql);
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