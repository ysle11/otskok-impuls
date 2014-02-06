
#property indicator_separate_window
#property indicator_minimum -10
#property indicator_maximum 100
#property indicator_buffers 6
#property indicator_color1 Blue
#property indicator_color2 Red
#property indicator_color3 Green
#property indicator_color4 Magenta
#property indicator_color5 BlueViolet
#property indicator_color6 DodgerBlue

//---- input parameters
extern int RSIOMA          = 14;
//---- buffers
double RSIBuffer[];
double PosBuffer[];
double NegBuffer[];

double bdn[],bup[];
double sdn[],sup[];

double marsioma[];
string short_name;
string sigs,sigb;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
   short_name = StringConcatenate("r(",RSIOMA,")");   
   IndicatorBuffers(8);
   Comment(Symbol());
   SetIndexBuffer(0,RSIBuffer);
   SetIndexBuffer(2,bup);
   SetIndexBuffer(1,bdn);
   SetIndexBuffer(3,sdn);
   SetIndexBuffer(4,sup);
   SetIndexBuffer(5,marsioma);
   
   SetIndexStyle(0,DRAW_NONE,EMPTY,3);
   SetIndexStyle(2,DRAW_HISTOGRAM,STYLE_SOLID,3,DodgerBlue);
   SetIndexStyle(1,DRAW_HISTOGRAM,STYLE_SOLID,3,Pink);
   SetIndexStyle(3,DRAW_NONE,EMPTY,1);
   SetIndexStyle(4,DRAW_NONE,EMPTY,1);
   SetIndexStyle(5,DRAW_LINE,STYLE_SOLID,1);
   
   SetIndexBuffer(6,PosBuffer);
   SetIndexBuffer(7,NegBuffer);
      
   IndicatorShortName(short_name);

   SetIndexDrawBegin(0,30);
   SetIndexDrawBegin(1,30);
   SetIndexDrawBegin(2,30);
   SetIndexDrawBegin(3,30);
   SetIndexDrawBegin(4,30);
   SetIndexDrawBegin(5,30);
   SetIndexDrawBegin(6,30);
   SetIndexDrawBegin(7,30);
//----
 sigs="sell"+Symbol()+""+Period()+"p"+RSIOMA;sigb="buy"+Symbol()+""+Period()+"p"+RSIOMA;
if(!GlobalVariableCheck(sigs))GlobalVariableSet(sigs,1);
if(!GlobalVariableCheck(sigb))GlobalVariableSet(sigb,1);


   return(0);
  }
//+------------------------------------------------------------------+
//| Relative Strength Index                                          |
//+------------------------------------------------------------------+
int start()
  {
   
  
   
   int    i,counted_bars=IndicatorCounted();
   double rel,negative,positive;
//----
   //if(Bars<=RSIOMA) return(0);
//---- initial zero
   if(counted_bars<1)
      for(i=1;i<=RSIOMA;i++) RSIBuffer[Bars-i]=0.0;
//----
   i=Bars-RSIOMA-1;
   int ma = i;
   if(counted_bars>=RSIOMA) i=Bars-counted_bars-1;
   while(i>=0)
     {
      double sumn=0.0,sump=0.0;
        {
         //---- smoothed moving average
         double ccma = iOsMA(Symbol(), 0, RSIOMA,2*RSIOMA,RSIOMA-RSIOMA/3,PRICE_CLOSE,i);//iMA(Symbol(),0,RSIOMA,0,RSIOMA_MODE,RSIOMA_PRICE,i);
         double ppma = iOsMA(Symbol(), 0, RSIOMA,2*RSIOMA,RSIOMA-RSIOMA/3,PRICE_CLOSE,i+1);//iMA(Symbol(),0,RSIOMA,0,RSIOMA_MODE,RSIOMA_PRICE,i+1);
            
         rel=ccma-ppma;
         
         if(rel>0) sump=rel;
         else      sumn=-rel;
         positive=(PosBuffer[i+1]*(RSIOMA-1)+sump)/RSIOMA;
         negative=(NegBuffer[i+1]*(RSIOMA-1)+sumn)/RSIOMA;
        }
      PosBuffer[i]=positive;
      NegBuffer[i]=negative;
      if(negative==0.0||positive==0.0) RSIBuffer[i]=0.0;
      else RSIBuffer[i]=100.0-100.0/(1+positive/negative);
  
      i--;
     }
     
     while(ma>=0)
     {
         marsioma[ma] = iRSIOnArray(RSIBuffer,0,3,ma); 
         ma--;
     }    
     if(marsioma[0]>marsioma[1])
      {
         SetIndexStyle(5, DRAW_LINE, STYLE_SOLID, 1, Blue);
         if(marsioma[1]<50){
           bup[0]=100;
           if(GlobalVariableGet(sigb)!=Time[0]){GlobalVariableSet(sigb,Time[0]);Alert(TimeToStr(Time[0])," ",sigb);wlog(TimeToStr(Time[0])+" "+sigb);}
         }
         else bup[0]=0;
      }
      else 
      if(marsioma[0]<marsioma[1])
      {
         SetIndexStyle(5, DRAW_LINE, STYLE_SOLID, 1, Red);
         if(marsioma[1]>50){
           bdn[0]=100;
           if(GlobalVariableGet(sigs)!=Time[0]){GlobalVariableSet(sigs,Time[0]);Alert(TimeToStr(Time[0])," ",sigs);wlog(TimeToStr(Time[0])+" "+sigs);}
         }
         else bdn[0]=0;
      }
      else SetIndexStyle(5, DRAW_LINE, STYLE_SOLID, 1, Green);

     
//----
   return(0);
  }
void wlog(string rsig){
  /*int handle;
  rsig=TimeToStr(TimeCurrent(),TIME_DATE|TIME_SECONDS)+":  "+rsig+"\r\n";
  while(handle<1){handle=FileOpen("tsig.txt", FILE_CSV|FILE_WRITE, ';');Sleep(100);}
    if(handle<1)
    {
     Print("can't open file error-",GetLastError());
     return(0);
    }
  FileSeek(handle, 0, SEEK_END);
  FileWrite(handle, rsig );
  FileClose(handle);
  handle=0;*/

}