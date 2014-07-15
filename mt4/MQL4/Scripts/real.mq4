string mode;
string sms[]={"EURUSD","USDCHF","GBPUSD","USDJPY","USDCAD","AUDUSD","NZDUSD","GBPJPY","CHFJPY","EURJPY","GBPCHF"};
//string sms[]={"#HPQ","#AA","#MSFT","#YM","#EP","#ENQ"};
int i,i1,i2,i3,secs=60;
int tfs[]={PERIOD_D1};//,PERIOD_H1,PERIOD_H4
color clr;
bool fresh=true,allfresh=true;
datetime servertime,tc;
//+------------------------------------------------------------------+
//| script program start function                                    |
//+------------------------------------------------------------------+
int start()
  {
      double lot;
      for(i=0;i<ArraySize(sms);i++){
      
         lot=NormalizeDouble((AccountFreeMargin()/MarketInfo(sms[i],MODE_MARGINREQUIRED)/1.5),2)-MarketInfo(sms[i],MODE_MINLOT);

      OrderSend(sms[i],OP_SELLSTOP,lot,MarketInfo(sms[i],MODE_BID)-MarketInfo(sms[i],MODE_POINT)*100-MarketInfo(sms[i],MODE_POINT)*MarketInfo(sms[i],MODE_SPREAD),3,0,MarketInfo(sms[i],MODE_BID)-MarketInfo(sms[i],MODE_POINT)*100-MarketInfo(sms[i],MODE_POINT)*18-MarketInfo(sms[i],MODE_POINT)*MarketInfo(sms[i],MODE_SPREAD),NULL,0,TimeCurrent()+(22-TimeHour(TimeCurrent()))*3600+3600 );
      OrderSend(sms[i],OP_BUYSTOP,lot,MarketInfo(sms[i],MODE_ASK)+MarketInfo(sms[i],MODE_POINT)*100+MarketInfo(sms[i],MODE_POINT)*MarketInfo(sms[i],MODE_SPREAD),3,0,MarketInfo(sms[i],MODE_ASK)+MarketInfo(sms[i],MODE_POINT)*100+MarketInfo(sms[i],MODE_POINT)*18+MarketInfo(sms[i],MODE_POINT)*MarketInfo(sms[i],MODE_SPREAD),NULL,0,TimeCurrent()+(22-TimeHour(TimeCurrent()))*3600+3600 );
      }

   return(0);
  }
//+------------------------------------------------------------------+