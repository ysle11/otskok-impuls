//+------------------------------------------------------------------+
//|                                                    totallots.mq4 |
//|                                                                  |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

//+------------------------------------------------------------------+
//| script program start function                                    |
//+------------------------------------------------------------------+
int start()
  {
//----
  int i,accTotal=OrdersHistoryTotal();
  double lt=0.0;
  for(i=0;i<accTotal;i++)
    {
     //---- check selection result
     if(OrderSelect(i,SELECT_BY_POS,MODE_HISTORY)==false)
       {
        Print("Ошибка при доступе к исторической базе (",GetLastError(),")");
        break;
       }
       if((OrderType()==OP_SELL)||(OrderType()==OP_BUY))lt+=OrderLots();
     // работа с ордером ...
    }
   Print("Account (",AccountNumber(),") LotsHistoryTotal: ",DoubleToStr(lt,1));
//----
   return(0);
  }
//+------------------------------------------------------------------+