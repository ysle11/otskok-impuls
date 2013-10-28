#property indicator_separate_window
#property indicator_minimum 0
#property indicator_maximum 1
#property indicator_buffers 1
#property indicator_color1 Black
double MainBufferFast[];
extern bool EURUSD=false;
extern bool USDCHF=false;
extern bool GBPUSD=false;
extern bool USDJPY=false;
extern bool USDCAD=false;
extern bool AUDUSD=false;
extern bool NZDUSD=false;
extern bool GBPJPY=false;
extern bool CHFJPY=false;
extern bool EURJPY=false;
extern bool GBPCHF=false;
extern bool EURGBP=false;
string mode;
string sms[]={"EURUSD","USDCHF","GBPUSD","USDJPY","USDCAD","AUDUSD","NZDUSD","GBPJPY","CHFJPY","EURJPY","GBPCHF","EURGBP"};
double vals[12];
int i,i1,i2;
int init(){
   SetIndexStyle(0,DRAW_HISTOGRAM);SetIndexBuffer(0, MainBufferFast);IndicatorDigits(0);
   
   ArrayResize(vals,ArraySize(sms));ArrayInitialize(vals,0);
   int y=10,x=270;
   for(i=0;i<ArraySize(sms);i++)
   {
      mode=sms[i]+"lot"; 
      ObjectCreate(mode, OBJ_LABEL, 0 ,0,0);ObjectSetText(mode," ", 8, "Arial Narrow", Gold);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   }
   int cnt=0;
   if(EURUSD){cnt++;vals[0]=MarketInfo("EURUSD",MODE_MARGINREQUIRED);}
   if(USDCHF){cnt++;vals[1]=MarketInfo("USDCHF",MODE_MARGINREQUIRED);}
   if(GBPUSD){cnt++;vals[2]=MarketInfo("GBPUSD",MODE_MARGINREQUIRED);}
   if(USDJPY){cnt++;vals[3]=MarketInfo("USDJPY",MODE_MARGINREQUIRED);}
   if(USDCAD){cnt++;vals[4]=MarketInfo("USDCAD",MODE_MARGINREQUIRED);}
   if(AUDUSD){cnt++;vals[5]=MarketInfo("AUDUSD",MODE_MARGINREQUIRED);}
   if(NZDUSD){cnt++;vals[6]=MarketInfo("NZDUSD",MODE_MARGINREQUIRED);}
   if(GBPJPY){cnt++;vals[7]=MarketInfo("GBPJPY",MODE_MARGINREQUIRED);}
   if(CHFJPY){cnt++;vals[8]=MarketInfo("CHFJPY",MODE_MARGINREQUIRED);}
   if(EURJPY){cnt++;vals[9]=MarketInfo("EURJPY",MODE_MARGINREQUIRED);}
   if(GBPCHF){cnt++;vals[10]=MarketInfo("GBPCHF",MODE_MARGINREQUIRED);}
   if(EURGBP){cnt++;vals[11]=MarketInfo("EURGBP",MODE_MARGINREQUIRED);}
   for(i=0;i<ArraySize(sms);i++)
   {  
      mode=sms[i]+"lot";
      if(vals[i]==0)ObjectSetText(mode," ", 8, "Arial Narrow", Gold);
      else if(cnt>0)
      ObjectSetText(mode,DoubleToStr(AccountFreeMargin()/cnt/vals[i],2), 8, "Arial Narrow", Gold);
      //Print(""+sms[i]+": "+DoubleToStr(AccountFreeMargin()/cnt/vals[i],2));
   }   
   return(0);
}

int deinit(){for(i=0;i<ArraySize(sms);i++){mode=sms[i]+"lot";ObjectDelete(mode);}return(0);}
int start(){   
   for(i=0;i<Bars-IndicatorCounted()+1;i++)MainBufferFast[i]=1; 
   return(0);
}

