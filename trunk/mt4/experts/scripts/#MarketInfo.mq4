string sm;
int init(){
   string mode;int y=10;ObjectsDeleteAll(0,OBJ_LABEL);
   sm=Symbol();
   mode="BARS"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_SPREAD"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_STOPLEVEL"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_LOTSIZE"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_TICKVALUE"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_TICKSIZE"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_SWAPLONG"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_SWAPSHORT"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_TRADEALLOWED"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   mode="MODE_MARGINREQUIRED"; ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"disable", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 10);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   return(0);
}

int deinit(){return(0);}
int start(){    
   double errors;string sm1=sm;
   for(int i=0;i<iBars(sm1,0);i++)if(iBarShift(sm1,0,iTime(sm1,0,i),true)<0)errors++;
   ObjectSetText("BARS","BARS: "+DoubleToStr(iBars(sm1,0),0)+" errors: "+DoubleToStr(errors,0)+" ("+TimeToStr(iTime(sm1,0,iBars(sm1,0)-1))+" - "+TimeToStr(iTime(sm1,0,0))+")",8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_SPREAD","MODE_SPREAD: "+DoubleToStr(MarketInfo(sm,MODE_SPREAD),0),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_STOPLEVEL","MODE_STOPLEVEL: "+DoubleToStr(MarketInfo(sm,MODE_STOPLEVEL),0),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_LOTSIZE","MODE_LOTSIZE: "+DoubleToStr(MarketInfo(sm,MODE_LOTSIZE),5),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_TICKVALUE","MODE_TICKVALUE: "+DoubleToStr(MarketInfo(sm,MODE_TICKVALUE),5),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_TICKSIZE","MODE_TICKSIZE: "+DoubleToStr(MarketInfo(sm,MODE_TICKSIZE),Digits),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_SWAPLONG","MODE_SWAPLONG: "+DoubleToStr(MarketInfo(sm,MODE_SWAPLONG),Digits),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_SWAPSHORT","MODE_SWAPSHORT: "+DoubleToStr(MarketInfo(sm,MODE_SWAPSHORT),Digits),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_TRADEALLOWED","MODE_TRADEALLOWED: "+DoubleToStr(MarketInfo(sm,MODE_TRADEALLOWED),0),8,"Arial Narrow", DarkGray);
   ObjectSetText("MODE_MARGINREQUIRED","MODE_MARGINREQUIRED: "+DoubleToStr(MarketInfo(sm,MODE_MARGINREQUIRED),5),8,"Arial Narrow", DarkGray);
   
   return(0);
}