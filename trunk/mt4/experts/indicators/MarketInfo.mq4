#property indicator_chart_window
string sm;
int init(){
   string mode;int y=10;
   sm=Symbol();
   mode="BARS"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_LOW"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_HIGH"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_TIME"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_BID"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_ASK"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_POINT"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_DIGITS"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_SPREAD"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_STOPLEVEL"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_LOTSIZE"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_TICKVALUE"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_TICKSIZE"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_SWAPLONG"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_SWAPSHORT"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_STARTING"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_EXPIRATION"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_TRADEALLOWED"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_MINLOT"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_LOTSTEP"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_MAXLOT"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_SWAPTYPE"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_PROFITCALCMODE"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_MARGINCALCMODE"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_MARGININIT"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_MARGINMAINTENANCE"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_MARGINHEDGED"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_MARGINREQUIRED"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_FREEZELEVEL"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   return(0);
}

int deinit(){ObjectsDeleteAll(0,OBJ_LABEL);return(0);}
int start(){    
   double errors;string sm1=sm;
   for(int i=0;i<iBars(sm1,0);i++)if(iBarShift(sm1,0,iTime(sm1,0,i),true)<0)errors++;
   ObjectSetText("BARS","BARS: "+DoubleToStr(iBars(sm1,0),0)+" errors: "+DoubleToStr(errors,0)+" ("+TimeToStr(iTime(sm1,0,iBars(sm1,0)-1))+" - "+TimeToStr(iTime(sm1,0,0))+")",8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_LOW","MODE_LOW: "+DoubleToStr(MarketInfo(sm,MODE_LOW),Digits),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_HIGH","MODE_HIGH: "+DoubleToStr(MarketInfo(sm,MODE_HIGH),Digits),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_TIME","MODE_TIME: "+TimeToStr(MarketInfo(sm,MODE_TIME)),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_BID","MODE_BID: "+DoubleToStr(MarketInfo(sm,MODE_BID),Digits),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_ASK","MODE_ASK: "+DoubleToStr(MarketInfo(sm,MODE_ASK),Digits),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_POINT","MODE_POINT: "+DoubleToStr(MarketInfo(sm,MODE_POINT),Digits),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_DIGITS","MODE_DIGITS: "+DoubleToStr(MarketInfo(sm,MODE_DIGITS),0),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_SPREAD","MODE_SPREAD: "+DoubleToStr(MarketInfo(sm,MODE_SPREAD),0),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_STOPLEVEL","MODE_STOPLEVEL: "+DoubleToStr(MarketInfo(sm,MODE_STOPLEVEL),0),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_LOTSIZE","MODE_LOTSIZE: "+DoubleToStr(MarketInfo(sm,MODE_LOTSIZE),5),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_TICKVALUE","MODE_TICKVALUE: "+DoubleToStr(MarketInfo(sm,MODE_TICKVALUE),5),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_TICKSIZE","MODE_TICKSIZE: "+DoubleToStr(MarketInfo(sm,MODE_TICKSIZE),Digits),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_SWAPLONG","MODE_SWAPLONG: "+DoubleToStr(MarketInfo(sm,MODE_SWAPLONG),Digits),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_SWAPSHORT","MODE_SWAPSHORT: "+DoubleToStr(MarketInfo(sm,MODE_SWAPSHORT),Digits),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_STARTING","MODE_STARTING: "+MarketInfo(sm,MODE_STARTING),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_EXPIRATION","MODE_EXPIRATION: "+MarketInfo(sm,MODE_EXPIRATION),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_TRADEALLOWED","MODE_TRADEALLOWED: "+MarketInfo(sm,MODE_TRADEALLOWED),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_MINLOT","MODE_MINLOT: "+DoubleToStr(MarketInfo(sm,MODE_MINLOT),5),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_LOTSTEP","MODE_LOTSTEP: "+DoubleToStr(MarketInfo(sm,MODE_LOTSTEP),5),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_MAXLOT","MODE_MAXLOT: "+DoubleToStr(MarketInfo(sm,MODE_MAXLOT),5),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_SWAPTYPE","MODE_SWAPTYPE: "+DoubleToStr(MarketInfo(sm,MODE_SWAPTYPE),5),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_PROFITCALCMODE","MODE_PROFITCALCMODE: "+DoubleToStr(MarketInfo(sm,MODE_PROFITCALCMODE),0),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_MARGINCALCMODE","MODE_MARGINCALCMODE: "+DoubleToStr(MarketInfo(sm,MODE_MARGINCALCMODE),0),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_MARGININIT","MODE_MARGININIT: "+MarketInfo(sm,MODE_MARGININIT),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_MARGINMAINTENANCE","MODE_MARGINMAINTENANCE: "+MarketInfo(sm,MODE_MARGINMAINTENANCE),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_MARGINHEDGED","MODE_MARGINHEDGED: "+DoubleToStr(MarketInfo(sm,MODE_MARGINHEDGED),0),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_MARGINREQUIRED","MODE_MARGINREQUIRED: "+DoubleToStr(MarketInfo(sm,MODE_MARGINREQUIRED),5),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_FREEZELEVEL","MODE_FREEZELEVEL: "+DoubleToStr(MarketInfo(sm,MODE_FREEZELEVEL),5),8,"Arial Narrow", DarkGray);
   
   return(0);
}