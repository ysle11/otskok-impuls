#import "user32.dll"
  int GetParent(int hWnd);
  int PostMessageA(int hWnd,int Msg,int wParam,int lParam);
#import
#import "shell32.dll"
 int ShellExecuteA(int hWnd, string Operation, string FileName, string Parameters, string Directory, int ShowCmd);
#import

string mode;
//string sms[]={"EURUSD","USDCHF","GBPCHF","GBPUSD","USDJPY","USDCAD","AUDUSD","NZDUSD","GBPJPY","CHFJPY","EURJPY","NZDJPY","AUDNZD","AUDCAD","AUDCHF","AUDJPY","AUDSGD","EURAUD","EURCAD","EURNZD","GBPAUD","GBPCAD","NZDCAD","EURNOK","EURSEK","USDDKK","USDNOK","USDSEK","USDZAR"};
string sms[]={"EURUSD","USDCHF","GBPCHF","GBPUSD","USDJPY","USDCAD","AUDUSD","NZDUSD","GBPJPY","CHFJPY","EURJPY"};

int i,i1,i2,i3,secs=60;
int tfs[]={PERIOD_D1};//,PERIOD_H1,PERIOD_H4
color clr;
bool fresh=true,allfresh=true;
datetime servertime,tc;
//+------------------------------------------------------------------+
//| script program start function                                    |
//+------------------------------------------------------------------+
int period0=1440;
void SaveHistory(string sm){
  int digits=MarketInfo(sm,MODE_DIGITS);
  int ExtHandle=-1;
SymbolSelect(sm,true);
   
   ExtHandle=FileOpen(sm+period0+".hst",FILE_BIN|FILE_WRITE|FILE_SHARE_WRITE|FILE_SHARE_READ|FILE_ANSI);
   FileWriteInteger(ExtHandle,digits);
   for(int i=iBars( ""+sm, period0)-1;i>=0;i--){
      FileWriteInteger(ExtHandle,iTime(sm,period0,i));
      FileWriteDouble(ExtHandle,iOpen(sm,period0,i));
      FileWriteDouble(ExtHandle,iHigh(sm,period0,i));
      FileWriteDouble(ExtHandle,iLow(sm,period0,i));
      FileWriteDouble(ExtHandle,iClose(sm,period0,i));
      FileWriteDouble(ExtHandle,iVolume(sm,period0,i));
   }
   FileClose(ExtHandle);
}
int OnStart()
  {
  while(!IsConnected())Sleep(1000);
  Sleep(20000);
      mode="MODE_TIME"; 
      datetime maxticktime=0;
      for(i=0;i<ArraySize(sms);i++)if(maxticktime<MarketInfo( ""+sms[i], MODE_TIME))maxticktime=MarketInfo( ""+sms[i], MODE_TIME);
      if(ObjectFind(mode)<0)ObjectCreate(mode,OBJ_LABEL,0,0,0);
         ObjectSetText(mode,TimeToStr(maxticktime,TIME_DATE|TIME_SECONDS), 8, "Arial Narrow", White);
         ObjectSet(mode, OBJPROP_CORNER, 0);
         ObjectSet(mode, OBJPROP_XDISTANCE, 11);ObjectSet(mode, OBJPROP_YDISTANCE, 1);

//----

   int y=10,x=10;
   for(i=0;i<ArraySize(sms);i++)
   {
      mode=""+sms[i]; 
      ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,mode+":", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   }
   y=0;
   for(i=0;i<ArraySize(sms);i++)
   {
      y+=10;x=160;//365;
      for(i1=0;i1<ArraySize(tfs);i1++)
      { 
      mode=""+sms[i]+DoubleToStr(PERIOD_D1,0); 
      if(ObjectFind(mode)<0)ObjectCreate(mode,OBJ_LABEL,0,0,0);
         ObjectSetText(mode,"0", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
      x+=110;}
   }
   allfresh=false;RefreshRates();i3=0;
   while(!allfresh){allfresh=true;          
         for(i=0;i<ArraySize(sms);i++)
         {  

               tc=TimeCurrent();
               servertime=StrToTime(TimeYear(tc)+"."+TimeMonth(tc)+"."+TimeDay(tc)+" 00:00");
               mode=""+sms[i]+DoubleToStr(PERIOD_D1,0);
               fresh=true;for(i2=0;i2<5;i2++)
                           if((TimeDayOfWeek(servertime-PERIOD_D1*i2*secs)>0)&&(TimeDayOfWeek(servertime-PERIOD_D1*i2*secs)<6))
                            if(iBarShift(sms[i],PERIOD_D1,servertime-PERIOD_D1*i2*secs,true)<0&&fresh){fresh=false;allfresh=false;break;}
               clr=Red;if(iBars(sms[i],PERIOD_D1)>10 && fresh)clr=Green;
               string t1=TimeToStr(iTime(sms[i],PERIOD_D1,0));
               ObjectSetText(mode,""+StringSubstr(t1,5) +"   "+iBars(sms[i],PERIOD_D1), 8, "Arial Narrow", clr);

         }   
      for(i=0;i<ArraySize(sms);i++)if(maxticktime<MarketInfo( ""+sms[i], MODE_TIME))maxticktime=MarketInfo( ""+sms[i], MODE_TIME);
      ObjectSetText("MODE_TIME",TimeToStr(maxticktime,TIME_DATE|TIME_SECONDS)+"  D1", 8, "Arial Narrow", White);
         WindowRedraw();if(IsStopped())return(0);Sleep(100);i3++;if(i3>30){i3=0;RefreshRates();}
   }
         for(i=0;i<ArraySize(sms);i++)
         { SymbolSelect(sms[i],true);
SaveHistory(sms[i]);
}
   ShellExecuteA(0,"","route.bat","off","f:\WINDOWS\system32\\",1);
   Sleep(10000);
   PostMessageA(GetParent(GetParent(GetParent(WindowHandle(Symbol(),Period())))),18,0,0);
   return(0);
  }
//+------------------------------------------------------------------+