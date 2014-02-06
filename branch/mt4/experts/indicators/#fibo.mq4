//+------------------------------------------------------------------+
//|                                                    Fibo_auto.mq4 |
//|                                         Copyright © 2007, Xupypr |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2007, Xupypr"

#property indicator_chart_window
#property indicator_buffers 7
#property indicator_color1 Aqua
#property indicator_color2 Aqua
#property indicator_color3 Aqua
#property indicator_color4 Aqua
#property indicator_color5 Magenta
#property indicator_color6 Magenta
#property indicator_color7 Magenta
//---- input parameters
extern int count=0;
string sm;
extern int Op_step=0;
extern double initlot=0;
extern int period=PERIOD_W1;
double delta=0;
int AllBars,DW;

//---- indicator buffers


//+------------------------------------------------------------------+
//| Custom indicator initialization function |
//+------------------------------------------------------------------+
int init()
{
sm=Symbol();
int ab=10000,af=10000;
if(IsConnected()){ab=AccountBalance();af=AccountFreeMargin( );}
if(Op_step==0)Op_step=MarketInfo(sm,MODE_STOPLEVEL);
if(count==0)count=MathFloor((iHigh(sm , period, 1)-iLow(sm , period, 1))/Point/Op_step/3);
if(initlot==0){initlot=af / MarketInfo(sm,MODE_MARGINREQUIRED);initlot=initlot-MarketInfo(sm,MODE_LOTSTEP)*4;delta=ab-initlot*MarketInfo(sm,MODE_MARGINREQUIRED); }
for (int i=0;i<=count;i++) {
  ObjectCreate("B"+i,OBJ_LABEL,0,0,0);
  ObjectSet("B"+i, OBJPROP_XDISTANCE, 0);
  ObjectSet("B"+i, OBJPROP_YDISTANCE, i*9);}
for (i=0;i<=count;i++) {
  ObjectCreate("S"+i,OBJ_LABEL,0,0,0);
  ObjectSet("S"+i, OBJPROP_XDISTANCE, 60);
  ObjectSet("S"+i, OBJPROP_YDISTANCE, i*9);}
for (i=0;i<=count;i++) {
  ObjectCreate("L"+i,OBJ_LABEL,0,0,0);
  ObjectSet("L"+i, OBJPROP_XDISTANCE, 37);
  ObjectSet("L"+i, OBJPROP_YDISTANCE, i*9);}

for (i=0;i<=count;i++) {
  ObjectCreate("P"+i,OBJ_LABEL,0,0,0);
  ObjectSet("P"+i, OBJPROP_XDISTANCE, 120);
  ObjectSet("P"+i, OBJPROP_YDISTANCE, i*9);}
    
ObjectCreate("C0",OBJ_LABEL,0,0,0);
ObjectSet("C0", OBJPROP_XDISTANCE, 0);
ObjectSet("C0", OBJPROP_YDISTANCE, (count+1)*9);  

ObjectCreate("T0",OBJ_LABEL,0,0,0);
ObjectSet("T0", OBJPROP_XDISTANCE, 105);
ObjectSet("T0", OBJPROP_YDISTANCE, (count+1)*9);  

 return(0);
}

//+------------------------------------------------------------------+
//| Custom indicator deinitialization function |
//+------------------------------------------------------------------+
int deinit()
{
 Comment("");
 for (int i=0;i<=count;i++){
  if (ObjectFind("B"+i)!=-1) ObjectDelete("B"+i);
  if (ObjectFind("S"+i)!=-1) ObjectDelete("S"+i);
  if (ObjectFind("L"+i)!=-1) ObjectDelete("L"+i);
  if (ObjectFind("P"+i)!=-1) ObjectDelete("P"+i);
 }
 if (ObjectFind("C0")!=-1) ObjectDelete("C0");
 if (ObjectFind("T0")!=-1) ObjectDelete("T0");
 return(0);
}

int start()
{
 double boomlot=0,ilot,t=0;
 ilot=initlot;
/*  lot[1]=7;//AccountFreeMargin( ) / MarketInfo(sm,MODE_MARGINREQUIRED);
 lot[2]=lot[1]*MarketInfo(sm,MODE_TICKVALUE)*Op_step/ MarketInfo(sm,MODE_MARGINREQUIRED);
 lot[3]=(lot[1]+lot[2])*MarketInfo(sm,MODE_TICKVALUE)*Op_step/ MarketInfo(sm,MODE_MARGINREQUIRED);  
 lot[4]=(lot[1]+lot[2]+lot[3])*MarketInfo(sm,MODE_TICKVALUE)*Op_step/ MarketInfo(sm,MODE_MARGINREQUIRED);
*/ int i;
ObjectSetText("C0",DoubleToStr((iHigh(sm , period, 1)-iLow(sm , period, 1))/Point,0)+"-"+DoubleToStr((iHigh(sm , period, 0)-iLow(sm , period, 0))/Point,0)+"   "+DoubleToStr(Ask,Digits)+" "+DoubleToStr(initlot,1),7,"Arial",Yellow);
boomlot=ilot*MarketInfo(sm,MODE_TICKVALUE)*MarketInfo(sm,MODE_STOPLEVEL)/ MarketInfo(sm,MODE_MARGINREQUIRED);
ilot=ilot+boomlot;
for (i=0;i<=count;i++){
boomlot=ilot*(MarketInfo(sm,MODE_TICKVALUE)*Op_step/ MarketInfo(sm,MODE_MARGINREQUIRED)-MarketInfo(sm,MODE_TICKVALUE)*MarketInfo(sm,MODE_SPREAD)/ MarketInfo(sm,MODE_MARGINREQUIRED));
ilot=ilot+boomlot;
ObjectSetText("B"+i,DoubleToStr(Ask+Point*Op_step*(i+1),Digits),7,"Arial",Blue);
ObjectSetText("L"+i,DoubleToStr(boomlot,1),7,"Arial",Silver);
ObjectSetText("S"+i,DoubleToStr(Bid-Point*Op_step*(i+1),Digits),7,"Arial",Red);
ObjectSetText("P"+i,"$"+DoubleToStr(boomlot*MarketInfo(sm,MODE_TICKVALUE),2),7,"Arial",Gold);
t=t+boomlot*MarketInfo(sm,MODE_MARGINREQUIRED);
}
ObjectSetText("T0","$"+DoubleToStr(delta+t+initlot*MarketInfo(sm,MODE_MARGINREQUIRED),2),7,"Arial",Gold);

}
//+------------------------------------------------------------------+