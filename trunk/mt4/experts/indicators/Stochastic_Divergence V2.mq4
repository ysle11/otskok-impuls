//+------------------------------------------------------------------+
//|                     edited from     FX5_MACD_Divergence_V1.1.mq4 |
//|                                                              FX5 |
//|Editor: byens (byens@web.de)                        hazem@uk2.net |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2007, FX5"
#property link      "hazem@uk2.net"

#property indicator_separate_window
#property indicator_buffers 4
#property indicator_color1 Green
#property indicator_color2 Red
#property indicator_color3 Blue
#property indicator_color4 Red
#property indicator_level1 80
#property indicator_level2 20
#property indicator_maximum	100
#property indicator_minimum	0

#define arrowsDisplacement 0.0001

extern int KPeriod=8;
extern int DPeriod=3;
extern int Slowing=3;

double bullishDivergence[];
double bearishDivergence[];
double macd[];
double signal[];

static string   indicatorName;

//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
   SetIndexStyle(0, DRAW_ARROW);
   SetIndexStyle(1, DRAW_ARROW);
   SetIndexStyle(2, DRAW_LINE);
   SetIndexStyle(3, DRAW_LINE);
//----   
   SetIndexBuffer(0, bullishDivergence);
   SetIndexBuffer(1, bearishDivergence);
   SetIndexBuffer(2, macd);
   SetIndexBuffer(3, signal);   
//----   
   SetIndexArrow(0, 233);
   SetIndexArrow(1, 234);
//----
   indicatorName = "Stochastic_Divergence(" + KPeriod + ", " + 
                                 DPeriod + ", " + Slowing + ")";
   SetIndexDrawBegin(3, Slowing);//signalSMA);
   IndicatorDigits(Digits + 2);
   IndicatorShortName(indicatorName);

   return(0);
}
//+------------------------------------------------------------------+
//| Custom indicator deinitialization function                       |
//+------------------------------------------------------------------+
int deinit(){
    for(int i = ObjectsTotal() - 1; i >= 0; i--){
       string label = ObjectName(i);
       if(StringSubstr(label, 0, 25) != "Stochastic_DivergenceLine")continue;ObjectDelete(label);   
    }
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int start(){
    int countedBars=IndicatorCounted();
    if(countedBars<0)countedBars=0;CalculateIndicator(countedBars);
    return(0);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CalculateIndicator(int countedBars){
     for(int i=Bars-countedBars;i>=0;i--)
     {
       macd[i]=iStochastic(NULL,0,KPeriod,DPeriod,Slowing,MODE_SMA,0,MODE_MAIN,i);
       signal[i]=iStochastic(NULL,0,KPeriod,DPeriod,Slowing,MODE_SMA,0,MODE_SIGNAL,i);

       CatchBullishDivergence(i+2);
       CatchBearishDivergence(i+2);
     }              
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CatchBullishDivergence(int shift){
   if(!(macd[shift] <= macd[shift+1] && macd[shift] < macd[shift+2] && macd[shift] < macd[shift-1]))return;  
   int lastTrough=GetIndicatorLastTrough(shift);
//----   
   if(macd[shift]>=macd[lastTrough]&&Low[shift]<=Low[lastTrough]){
      bullishDivergence[shift]=macd[shift]-arrowsDisplacement;                        
   }
//----   
   if(macd[shift]<=macd[lastTrough]&&Low[shift]>=Low[lastTrough]){
      bullishDivergence[shift]=macd[shift]-arrowsDisplacement;
    }      
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CatchBearishDivergence(int shift){
     if(!(macd[shift] >= macd[shift+1] && macd[shift] > macd[shift+2] && macd[shift] > macd[shift-1]))return;
     int lastPeak = GetIndicatorLastPeak(shift);  
     if(macd[shift] <= macd[lastPeak] && High[shift] >= High[lastPeak]){
       bearishDivergence[shift] = macd[shift] + arrowsDisplacement;       
     }
     if(macd[shift] >= macd[lastPeak] && High[shift] <= High[lastPeak]){
       bearishDivergence[shift] = macd[shift] + arrowsDisplacement;
     }   
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int GetIndicatorLastPeak(int shift){
    for(int i = shift + 5; i < Bars; i++){
       if(signal[i] >= signal[i+1] && signal[i] >= signal[i+2] && signal[i] >= signal[i-1] && signal[i] >= signal[i-2]){
          for(int j = i; j < Bars; j++){
             if(macd[j] >= macd[j+1] && macd[j] > macd[j+2] && macd[j] >= macd[j-1] && macd[j] > macd[j-2])
             return(j);
          }
       }
    }
    return(-1);
}

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int GetIndicatorLastTrough(int shift){
    for(int i = shift + 5; i < Bars; i++){
        if(signal[i] <= signal[i+1] && signal[i] <= signal[i+2] && signal[i] <= signal[i-1] && signal[i] <= signal[i-2]){
           for(int j = i; j < Bars; j++){
                if(macd[j] <= macd[j+1] && macd[j] < macd[j+2] && macd[j] <= macd[j-1] && macd[j] < macd[j-2])
                   return(j);
           }
        }
    }
    return(-1);
}


