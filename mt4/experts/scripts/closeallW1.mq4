//+------------------------------------------------------------------+
//|                                                     closeall.mq4 |
//|                                                                  |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""
int magic=10080;
//+------------------------------------------------------------------+
//| script program start function                                    |
//+------------------------------------------------------------------+
int start()
  {
//----
    int o;

    for(o=OrdersTotal()-1;o>=0;o--){
      OrderSelect(o, SELECT_BY_POS, MODE_TRADES);
       if(OrderType()==OP_BUYSTOP)if(OrderMagicNumber()==magic){OrderDelete(OrderTicket());continue;}
       if(OrderType()==OP_BUYLIMIT)if(OrderMagicNumber()==magic){OrderDelete(OrderTicket());continue;}
    }

    for(o=OrdersTotal()-1;o>=0;o--){
      OrderSelect(o, SELECT_BY_POS, MODE_TRADES);
       if(OrderType()==OP_SELLSTOP)if(OrderMagicNumber()==magic){OrderDelete(OrderTicket());continue;}
       if(OrderType()==OP_SELLLIMIT)if(OrderMagicNumber()==magic){OrderDelete(OrderTicket());continue;}
    }   

  
//----
   return(0);
  }
//+------------------------------------------------------------------+