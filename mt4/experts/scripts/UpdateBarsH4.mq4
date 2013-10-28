string mode;
string sms[]={"EURUSD","USDCHF","GBPUSD","USDJPY","USDCAD","AUDUSD","NZDUSD","GBPJPY","CHFJPY","EURJPY","GBPCHF"};//,,"EURGBP","USDSGD"};
//string sms[]={"#HPQ","#AA","#MSFT","#YM","#EP","#ENQ"};
int i,i1,i2,i3,secs=60;
int tfs[]={PERIOD_H4};//,PERIOD_H1,PERIOD_H4
color clr;
bool fresh=true,allfresh=true;
datetime servertime,tc;
//+------------------------------------------------------------------+
//| script program start function                                    |
//+------------------------------------------------------------------+
int start()
  {
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
      y+=10;x=55;//265;
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
                           if((TimeDayOfWeek(servertime-tfs[i1]*i2*secs)>0)&&(TimeDayOfWeek(servertime-tfs[i1]*i2*secs)<6))
                            if(iBarShift(sms[i],tfs[i1],servertime-tfs[i1]*i2*secs,true)<0&&fresh){fresh=false;allfresh=false;break;}
               clr=Red;if(iBars(sms[i],tfs[i1])>200 && fresh)clr=Green;
               string t1=TimeToStr(iTime(sms[i],tfs[i1],0));
               ObjectSetText(mode,""+StringSubstr(t1,5) +"   "+iBars(sms[i],tfs[i1]), 8, "Arial Narrow", clr);
            }
         }   
      for(i=0;i<ArraySize(sms);i++)if(maxticktime<MarketInfo( ""+sms[i], MODE_TIME))maxticktime=MarketInfo( ""+sms[i], MODE_TIME);
      ObjectSetText("MODE_TIME",TimeToStr(maxticktime,TIME_DATE|TIME_SECONDS)+"  H4", 8, "Arial Narrow", White);
         WindowRedraw();if(IsStopped())return(0);Sleep(100);i3++;if(i3>30){i3=0;RefreshRates();}
   }
   return(0);
  }
//+------------------------------------------------------------------+