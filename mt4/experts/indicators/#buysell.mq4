
#property indicator_chart_window
string smsbuy[]={"EURUSD","USDCHF","GBPCHF","GBPUSD","USDJPY","USDCAD","AUDUSD","NZDUSD","GBPJPY","CHFJPY","EURJPY","EURGBP","NZDJPY","AUDNZD","AUDCAD","AUDCHF","AUDJPY","AUDSGD","EURAUD","EURCAD","EURNZD","GBPAUD","GBPCAD","NZDCAD","EURNOK","EURSEK","USDDKK","USDNOK","USDSEK","USDZAR","USDSGD"};
//string smsbuy[]={"GBPCHF","GBPUSD","GBPJPY","GBPAUD","GBPCAD","","EURGBP"};
//string smsbuy[]={"USDCHF","USDJPY","USDCAD","USDDKK","USDNOK","USDSEK","USDZAR","USDSGD","","EURUSD","GBPUSD","AUDUSD","NZDUSD"};
extern int    back=0;   int issell=-1;
int init(){ObjectsDeleteAll();}
int start()
  {
     double totalvol=0,maxvol1=0,avgvol0=0,avgvol1=0;int i,i1;
     for(i=0;i<ArraySize(smsbuy);i++)if(iVolume(smsbuy[i],0,back)>maxvol1)maxvol1=iVolume(smsbuy[i],0,back);
     for(i1=1;i1<12;i1++)for(i=0;i<ArraySize(smsbuy);i++)avgvol1=avgvol1+iVolume(smsbuy[i],0,back+i1);
     for(i=0;i<ArraySize(smsbuy);i++)avgvol0=avgvol0+iVolume(smsbuy[i],0,back);
     avgvol0=avgvol0/ArraySize(smsbuy);
     avgvol1=avgvol1/11/ArraySize(smsbuy);avgvol0=avgvol0/avgvol1*100;
     for(i=0;i<ArraySize(smsbuy);i++)if(smsbuy[i]=="")issell=0;
   for(i=0;i<ArraySize(smsbuy);i++)
   if(smsbuy[i]!="")
   {
     color clr=Blue,volclr=DarkGray;
     double p1=iHigh(NULL,0,iHighest(NULL,0,MODE_HIGH,2111,0));
     double p2=iLow(NULL,0,iLowest(NULL,0,MODE_LOW,2111,0));
     double c1=(p1-p2)/Point;
     double open1=iOpen(smsbuy[i],0,back);
     double close1=iClose(smsbuy[i],0,back);
     if(((issell==1)&&(open1<close1))||((issell==0)&&(open1>close1)))volclr=Green;
     if(open1>close1)clr=Red;close1=(close1-open1)*MarketInfo(smsbuy[i],MODE_POINT);
     double high1=iHigh(smsbuy[i],0,back);high1=(high1-open1)*MarketInfo(smsbuy[i],MODE_POINT);
     double low1=iLow(smsbuy[i],0,back);low1=(open1-low1)*MarketInfo(smsbuy[i],MODE_POINT);
     double vol1=iVolume(smsbuy[i],0,back);totalvol=totalvol+vol1;
     double mvol1,lvol1=0.0,hvol1=0.0;
     for(int m=1;m<12;m++){
         if(iVolume(smsbuy[i],0,m)>hvol1)hvol1=iVolume(smsbuy[i],0,m);
         if((iVolume(smsbuy[i],0,m)<lvol1)||(lvol1==0.0))lvol1=iVolume(smsbuy[i],0,m);
     }mvol1=hvol1-lvol1;

   ObjectCreate(smsbuy[i]+"volbarhl", OBJ_RECTANGLE,0,0 ,0,0 );
   ObjectSet(smsbuy[i]+"volbarhl", OBJPROP_COLOR, volclr);
   ObjectSet(smsbuy[i]+"volbarhl", OBJPROP_WIDTH, 2);
   ObjectSet(smsbuy[i]+"volbarhl", OBJPROP_BACK, true);
   ObjectSet(smsbuy[i]+"volbarhl", OBJPROP_PRICE1, p2-c1*Point*1.5+(c1/mvol1*vol1*0.5)*Point);
   ObjectSet(smsbuy[i]+"volbarhl", OBJPROP_PRICE2, p2-c1*Point*1.5);
   ObjectSet(smsbuy[i]+"volbarhl", OBJPROP_TIME1, Time[(ArraySize(smsbuy)-i-1)*3]);
   ObjectSet(smsbuy[i]+"volbarhl", OBJPROP_TIME2, Time[(ArraySize(smsbuy)-i)*3-1]);
 
 
     //double m1=MathMax(close1,high1);m1=MathMax(m1,low1);m1=c1/m1;
     double m1=(iHigh(smsbuy[i],0,iHighest(smsbuy[i],0,MODE_HIGH,11,back))-iLow(smsbuy[i],0,iLowest(smsbuy[i],0,MODE_LOW,11,back)))*MarketInfo(smsbuy[i],MODE_POINT);m1=c1/m1;
     close1=close1*m1;low1=low1*m1;high1=high1*m1;
   ObjectCreate(smsbuy[i]+"buy", OBJ_TEXT,0, Time[(ArraySize(smsbuy)-i)*3],p1+c1*Point*0.8 );
   
   
   ObjectSetText(smsbuy[i]+"buy",smsbuy[i]+"    "+DoubleToStr(((iHigh(smsbuy[i],0,back)-iLow(smsbuy[i],0,back))/MarketInfo(smsbuy[i],MODE_POINT)),0)+"    "+DoubleToStr(vol1,0), 9, "Arial Black", clr);
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


   }else issell=1;   
     ObjectCreate("time1", OBJ_LABEL, 0 ,0,0);
     ObjectSet("time1", OBJPROP_CORNER, 0);
     ObjectSet("time1", OBJPROP_XDISTANCE, 22);ObjectSet("time1", OBJPROP_YDISTANCE, 22);
     ObjectSetText("time1",TimeToStr(TimeCurrent())+" "+DoubleToStr(avgvol0,0)+"%", 9, "Arial Black", Gold);

WindowRedraw();
   return(0);
  }

