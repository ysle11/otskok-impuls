string sms[]={"EURUSD","USDCHF","GBPCHF","GBPUSD","USDJPY","USDCAD","AUDUSD","NZDUSD","GBPJPY","CHFJPY","EURJPY","NZDJPY","AUDNZD","AUDCAD","AUDCHF","AUDJPY","AUDSGD","EURAUD","EURCAD","EURNZD","GBPAUD","GBPCAD","NZDCAD","EURNOK","EURSEK","USDDKK","USDNOK","USDSEK","USDZAR"};
extern int    back=0;   
int start()
  {
   int    i,period=412;
   string mode,sig,sig1;
   string minstoch="EURUSD",maxstoch="EURUSD";
   string mincci="EURUSD",maxcci="EURUSD";
   string mindmr="EURUSD",maxdmr="EURUSD";
   string minrsi="EURUSD",maxrsi="EURUSD";
   string minadx="EURUSD",maxadx="EURUSD";
   string minmfi="EURUSD",maxmfi="EURUSD";
   string minwpr="EURUSD",maxwpr="EURUSD";
   ObjectsDeleteAll();
     ObjectCreate("time1", OBJ_LABEL, 0 ,0,0);
     ObjectSet("time1", OBJPROP_CORNER, 0);
     ObjectSet("time1", OBJPROP_XDISTANCE, 25);ObjectSet("time1", OBJPROP_YDISTANCE, 25);
     ObjectSetText("time1",TimeToStr(TimeCurrent())+" Period: "+period+" back: "+back, 16, "Arial Narrow", Gold);

   ObjectCreate("back", OBJ_VLINE, 0,Time[back] ,0);ObjectSet("back", OBJPROP_COLOR, Gray);ObjectSet("back", OBJPROP_BACK, true);
//----
   for(i=0;i<ArraySize(sms);i++)
   {
      if(iStochastic(sms[i],0,period,3,9,MODE_SMA,0,MODE_MAIN,back)>iStochastic(maxstoch,0,period,3,9,MODE_SMA,0,MODE_MAIN,back))
      maxstoch=sms[i];
      if(iStochastic(sms[i],0,period,3,9,MODE_SMA,0,MODE_MAIN,back)<iStochastic(minstoch,0,period,3,9,MODE_SMA,0,MODE_MAIN,back))
      minstoch=sms[i];
      if(iCCI(sms[i],0,period,PRICE_TYPICAL,back)>iCCI(maxcci,0,period,PRICE_TYPICAL,back))
      maxcci=sms[i];
      if(iCCI(sms[i],0,period,PRICE_TYPICAL,back)<iCCI(mincci,0,period,PRICE_TYPICAL,back))
      mincci=sms[i];
      if(iDeMarker(sms[i],0,period,back)>iDeMarker(maxdmr,0,period,back))
      maxdmr=sms[i];
      if(iDeMarker(sms[i],0,period,back)<iDeMarker(mindmr,0,period,back))
      mindmr=sms[i];
      if(iRSI(sms[i],0,period,PRICE_TYPICAL,back)>iRSI(maxrsi,0,period,PRICE_TYPICAL,back))
      maxrsi=sms[i];
      if(iRSI(sms[i],0,period,PRICE_TYPICAL,back)<iRSI(minrsi,0,period,PRICE_TYPICAL,back))
      minrsi=sms[i];
      if(iADX(sms[i],0,period,PRICE_HIGH,MODE_MAIN,back)>iADX(maxadx,0,period,PRICE_HIGH,MODE_MAIN,back))
      maxadx=sms[i];
      if(iADX(sms[i],0,period,PRICE_HIGH,MODE_MAIN,back)<iADX(minadx,0,period,PRICE_HIGH,MODE_MAIN,back))
      minadx=sms[i];
      if(iMFI(sms[i],0,period,back)>iMFI(maxmfi,0,period,back))
      maxmfi=sms[i];
      if(iMFI(sms[i],0,period,back)<iMFI(minmfi,0,period,back))
      minmfi=sms[i];
      if(iWPR(sms[i],0,period,back)>iWPR(maxwpr,0,period,back))
      maxwpr=sms[i];
      if(iWPR(sms[i],0,period,back)<iWPR(minwpr,0,period,back))
      minwpr=sms[i];
   }

   int y=60,x;
   for(i=0;i<ArraySize(sms);i++)
   {
      x=10;
      mode=sms[i]+"name"; 
      ObjectCreate(mode, OBJ_LABEL, 0 ,0,0);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
      ObjectSetText(mode,sms[i], 8, "Arial Narrow", Gold);

      x=70;
      mode=sms[i]+"stoch"; 
      sig=DoubleToStr(iStochastic(sms[i],0,period,3,9,MODE_SMA,0,MODE_MAIN,back),4)+" ";
      if(sms[i]==maxstoch)sig=sig+" maxstoch ";if(sms[i]==minstoch)sig=sig+" minstoch ";
      sig1=DoubleToStr(iCCI(sms[i],0,period,PRICE_TYPICAL,back),4);
      if(sms[i]==maxcci)sig=sig+sig1+" maxcci ";if(sms[i]==mincci)sig=sig+sig1+" mincci ";
      sig1=DoubleToStr(iDeMarker(sms[i],0,period,back),4);
      if(sms[i]==maxdmr)sig=sig+sig1+" maxdmr ";if(sms[i]==mindmr)sig=sig+sig1+" mindmr ";
      sig1=DoubleToStr(iRSI(sms[i],0,period,PRICE_TYPICAL,back),4);
      if(sms[i]==maxrsi)sig=sig+sig1+" maxrsi ";if(sms[i]==minrsi)sig=sig+sig1+" minrsi ";
      sig1=DoubleToStr(iADX(sms[i],0,period,PRICE_HIGH,MODE_MAIN,back),4);
      if(sms[i]==maxadx)sig=sig+sig1+" maxadx ";if(sms[i]==minadx)sig=sig+sig1+" minadx ";
      sig1=DoubleToStr(iMFI(sms[i],0,period,back),4);
      if(sms[i]==maxmfi)sig=sig+sig1+" maxmfi ";if(sms[i]==minmfi)sig=sig+sig1+" minmfi ";
      sig1=DoubleToStr(iWPR(sms[i],0,period,back),4);
      if(sms[i]==maxwpr)sig=sig+sig1+" maxwpr ";if(sms[i]==minwpr)sig=sig+sig1+" minwpr ";
      ObjectCreate(mode, OBJ_LABEL, 0 ,0,0);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
      ObjectSetText(mode,sig, 8, "Arial Narrow", Gold);
      y+=11;
   }
   
//----
   return(0);
  }
//+------------------------------------------------------------------+