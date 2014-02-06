#property indicator_chart_window
string mode;
string sms[]={"EURUSD","USDCHF","GBPUSD","USDJPY","USDCAD","AUDUSD","NZDUSD","GBPJPY","CHFJPY","EURJPY","GBPCHF","EURGBP"};
int i,i1,i2;
int tfs[]={PERIOD_H1,PERIOD_H4,PERIOD_D1,PERIOD_W1,PERIOD_MN1};
color clr;
bool fresh=true,param=false;
int freshbar=0;
int init(){if(UninitializeReason()==REASON_PARAMETERS){param=true;return(0);}
   int y=10,x=10;
   for(i=0;i<ArraySize(sms);i++)
   {
      mode=""+sms[i]; 
      ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,mode+":", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);y+=10;
   }
   y=0;
   for(i=0;i<ArraySize(sms);i++)
   {
      y+=10;x=20;
      for(i1=0;i1<ArraySize(tfs);i1++)
      { x+=40;
      mode=""+sms[i]+DoubleToStr(tfs[i1],0); 
      ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSetText(mode,"0", 8, "Arial Narrow", DimGray);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, x);ObjectSet(mode, OBJPROP_YDISTANCE, y);
      }
   } 
   return(0);
}

int deinit(){for(i=0;i<ArraySize(sms);i++){for(i1=0;i1<ArraySize(tfs);i1++){mode=""+sms[i]+DoubleToStr(tfs[i1],0);ObjectDelete(mode);}ObjectDelete(sms[i]);}return(0);}
int start(){    
   if(iTime(NULL,PERIOD_H1,0)<freshbar)return(0);freshbar=iTime(NULL,PERIOD_H1,0);
   if(param){param=false;return(0);}
   for(i=0;i<ArraySize(sms);i++)
   {  
      for(i1=0;i1<ArraySize(tfs);i1++)
      {
      mode=""+sms[i]+DoubleToStr(tfs[i1],0);
      fresh=true;for(i2=0;i2<200;i2++)if(iBarShift(sms[i],tfs[i1],iTime(sms[i],tfs[i1],i2),true)<0&&fresh)fresh=false;
      clr=DimGray;if(iBars(sms[i],tfs[i1])>200 && fresh)clr=Green;
      ObjectSetText(mode,""+iBars(sms[i],tfs[i1]), 8, "Arial Narrow", clr);
      }
   }
  
   return(0);
}

