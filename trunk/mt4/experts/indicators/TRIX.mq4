//+------------------------------------------------------------------+
//|                                                         TRIX.mq4 |
//|                                                          by Raff | 
//|                                                                  | 
//+------------------------------------------------------------------+ 
#property copyright "Copyright � 2006, raff1410@o2.pl"

//---- indicator settings
#property  indicator_separate_window
#property  indicator_buffers 6
#property  indicator_color1 Red
#property  indicator_color2 DeepSkyBlue
#property  indicator_color3 Yellow
#property  indicator_color4 Aqua
#property  indicator_color5 DarkGray
#property  indicator_color6 DarkGray
#property  indicator_width1 1
#property  indicator_width2 1
#property  indicator_width3 0
#property  indicator_width4 0
#property  indicator_width5 1
#property  indicator_width6 1
#property  indicator_style1 STYLE_SOLID
#property  indicator_style2 STYLE_DOT
#property  indicator_style3 STYLE_SOLID
#property  indicator_style4 STYLE_SOLID
#property  indicator_style5 STYLE_SOLID
#property  indicator_style6 STYLE_SOLID


#property  indicator_level1 0

//---- indicator parameters
extern int  TRIX_Period     = 13;
extern int  Signal_Period = 8;
extern bool Signals      = false;
extern int  CountBars    = 1500;

//---- indicator buffers
double      ind_buffer1[];
double      ind_buffer2[];
double      ind_buffer3[];
double      ind_buffer4[];
double      ind_buffer5[];
double      ind_buffer6[];
double      ind_buffer7[];

//---- variables
static bool TurnedUp = false;
static bool TurnedDown = false;  
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- drawing settings
   IndicatorBuffers(7);
   SetIndexStyle(0,DRAW_LINE);
   SetIndexStyle(1,DRAW_LINE);
   SetIndexStyle(2,DRAW_ARROW);
   SetIndexArrow(2,233);
   SetIndexStyle(3,DRAW_ARROW);
   SetIndexArrow(3,234);
   SetIndexStyle(4,DRAW_LINE);
   SetIndexStyle(5,DRAW_HISTOGRAM);

//---- indicator buffers mapping
   SetIndexBuffer(0,ind_buffer1);
   SetIndexBuffer(1,ind_buffer2);
   SetIndexBuffer(2,ind_buffer3);
   SetIndexBuffer(3,ind_buffer4);
   SetIndexBuffer(4,ind_buffer5);
   SetIndexBuffer(5,ind_buffer6);
   SetIndexBuffer(6,ind_buffer7);
   
//---- name for DataWindow and indicator subwindow label
   IndicatorShortName("TRIX index | Period: "+TRIX_Period+", Signal Period: "+Signal_Period+" | ");
   SetIndexLabel(0,"TRIX");
   SetIndexLabel(1,"Signal");

//---- initialization done
   return(0);
  }
//+------------------------------------------------------------------+
//| TRIX                                                             |
//+------------------------------------------------------------------+
int start()
  {
   if (TRIX_Period==Signal_Period) return(0);
   
   int i, limit=CountBars;
   if (limit>Bars) limit=Bars-1;

//---- trix 
   for(i=0; i<limit; i++) ind_buffer1[i]=iMA(NULL,0,TRIX_Period,0,MODE_SMMA,PRICE_CLOSE,i);
   for(i=0; i<limit; i++) ind_buffer2[i]=iMAOnArray(ind_buffer1,0,TRIX_Period,0,MODE_SMMA,i);
   for(i=0; i<limit; i++) ind_buffer7[i]=iMAOnArray(ind_buffer2,0,TRIX_Period,0,MODE_EMA,i);
   
   for(i=0; i<limit-1; i++) ind_buffer1[i]=(ind_buffer7[i]-ind_buffer7[i+1])/ind_buffer7[i+1];
   for(i=0; i<limit-1; i++) ind_buffer2[i]=iMAOnArray(ind_buffer1,0,Signal_Period,0,MODE_EMA,i);
   for(i=0; i<limit-1; i++) {ind_buffer5[i]=ind_buffer1[i]-ind_buffer2[i]; ind_buffer6[i]=ind_buffer5[i];}

//---- signals
   i=limit-1;
   while(i>=0)
   {
     if (Signals==true)
     {
     if (ind_buffer2[i]<ind_buffer1[i] && ind_buffer2[i+1]>=ind_buffer1[i+1]) ind_buffer3[i]=ind_buffer2[i]-0.0001;
     if (ind_buffer2[i]>ind_buffer1[i] && ind_buffer2[i+1]<=ind_buffer1[i+1]) ind_buffer4[i]=ind_buffer2[i]+0.0001;
     
     if (ind_buffer3[0]==ind_buffer2[0]-0.0001 && TurnedUp==false)
        {  
              TurnedDown = false;
              TurnedUp = true;
        } 
     if (ind_buffer4[0]==ind_buffer2[0]+0.0001 && TurnedDown==false)
        {  
              TurnedUp = false;
              TurnedDown = true;
        } 
     }
   i--;
   }
//---- done
   return(0);
  }