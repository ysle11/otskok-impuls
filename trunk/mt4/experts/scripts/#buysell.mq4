//string smsbuy[]={"EURUSD","EURAUD","EURCAD","EURCHF","EURNZD","EURNOK","EURSEK","EURGBP","GBPUSD","GBPCHF","GBPAUD","GBPCAD","USDJPY","GBPJPY","CHFJPY","EURJPY","AUDUSD","AUDNZD","AUDCAD","AUDCHF","AUDJPY","AUDSGD","NZDUSD","NZDCAD","NZDJPY","GOLD","SILVER","USDCHF","USDHKD","USDDKK","USDSEK","USDRUB","USDCAD","USDNOK","USDZAR","USDSGD"};
//string smsbuy[]={"AUDNZD","EURCHF","AUDCAD","NZDUSD","USDJPY","NZDCAD","USDRUB"};
string smsbuy[]={"EURCHF","USDZAR","USDRUB","NZDUSD","AUDCAD","USDJPY","GBPCAD","NZDCAD"};
int start()
  {
     ObjectsDeleteAll();
     ObjectCreate("time1", OBJ_LABEL, 0 ,0,0);
     ObjectSet("time1", OBJPROP_CORNER, 0);
     ObjectSet("time1", OBJPROP_XDISTANCE, 50);ObjectSet("time1", OBJPROP_YDISTANCE, 50);
     ObjectSetText("time1",TimeToStr(TimeCurrent()), 26, "Arial Black", Gold);

   for(int i=0;i<ArraySize(smsbuy);i++)
   {
     color clr=Blue;
     double p1=iHigh(NULL,0,iHighest(NULL,0,MODE_HIGH,2111,0));
     double p2=iLow(NULL,0,iLowest(NULL,0,MODE_LOW,2111,0));
     double c1=(p1-p2)/Point;
     double open1=iOpen(smsbuy[i],0,0);
     double close1=iClose(smsbuy[i],0,0);if(open1>close1)clr=Red;close1=(close1-open1)*MarketInfo(smsbuy[i],MODE_POINT);
     double high1=iHigh(smsbuy[i],0,0);high1=(high1-open1)*MarketInfo(smsbuy[i],MODE_POINT);
     double low1=iLow(smsbuy[i],0,0);low1=(open1-low1)*MarketInfo(smsbuy[i],MODE_POINT);
     //double m1=MathMax(close1,high1);m1=MathMax(m1,low1);m1=c1/m1;
     double m1=(iHigh(smsbuy[i],0,iHighest(smsbuy[i],0,MODE_HIGH,11,0))-iLow(smsbuy[i],0,iLowest(smsbuy[i],0,MODE_LOW,11,0)))*MarketInfo(smsbuy[i],MODE_POINT);m1=c1/m1;
     close1=close1*m1;low1=low1*m1;high1=high1*m1;
   ObjectCreate(smsbuy[i]+"buy", OBJ_TEXT,0, Time[(ArraySize(smsbuy)-i)*3],p1+Point*1300 );
   
   
   ObjectSetText(smsbuy[i]+"buy",smsbuy[i], 9, "Arial Black", clr);
   //ObjectCreate("back", OBJ_VLINE, 0,Time[back] ,0);
   ObjectSet(smsbuy[i]+"buy", OBJPROP_COLOR, clr);
   ObjectSet(smsbuy[i]+"buy", OBJPROP_ANGLE, 90);

   //p1-=Point*1300;p2-=Point*1300;
   ObjectCreate(smsbuy[i]+"buybar", OBJ_RECTANGLE,0,0 ,p1,p2 );
   ObjectSet(smsbuy[i]+"buybar", OBJPROP_COLOR, clr);
   ObjectSet(smsbuy[i]+"buybar", OBJPROP_BACK, true);
   ObjectSet(smsbuy[i]+"buybar", OBJPROP_PRICE1, p2);
   ObjectSet(smsbuy[i]+"buybar", OBJPROP_PRICE2, p2+close1*Point);
   ObjectSet(smsbuy[i]+"buybar", OBJPROP_TIME1, Time[(ArraySize(smsbuy)-i-1)*3]);
   ObjectSet(smsbuy[i]+"buybar", OBJPROP_TIME2, Time[(ArraySize(smsbuy)-i)*3-1]);

   ObjectCreate(smsbuy[i]+"buybarhl", OBJ_RECTANGLE,0,0 ,p1,p2 );
   ObjectSet(smsbuy[i]+"buybarhl", OBJPROP_COLOR, clr);
   ObjectSet(smsbuy[i]+"buybarhl", OBJPROP_WIDTH, 2);
   ObjectSet(smsbuy[i]+"buybarhl", OBJPROP_BACK, false);
   ObjectSet(smsbuy[i]+"buybarhl", OBJPROP_PRICE1, p2+high1*Point);
   ObjectSet(smsbuy[i]+"buybarhl", OBJPROP_PRICE2, p2-low1*Point);
   ObjectSet(smsbuy[i]+"buybarhl", OBJPROP_TIME1, Time[(ArraySize(smsbuy)-i)*3-2]);
   ObjectSet(smsbuy[i]+"buybarhl", OBJPROP_TIME2, Time[(ArraySize(smsbuy)-i)*3-2]);

   }   

   return(0);
  }

