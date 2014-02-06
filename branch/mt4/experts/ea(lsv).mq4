extern double Probab = 0.8;
extern int    dstop = 25;
extern int    Nstop = 1;
extern int    delta = 1;
extern int    Nidelt = 20;
extern int    NN = 10;
extern double forg = 1.05;

datetime i_time;
double isopt=1.0,psell,pbuy;
bool tictak=false;

int mn, Ncomb,Ncomb1,Ncomb2, ii=0, metka, file1, idelt, i, istop, Take0; 
int buy_open, sell_open, buy_open1, sell_open1, buy_open2, sell_open2 ; 
double time0, prob, lt, LastBuyOpen, LastSellOpen; 
string FileName;
int delt[30], sr[50,30], sd[5000,30,4], stop[4], nsd[5000,30,4]; 
double P[12,30], pt[30], cen[5000,30,4], rost[5000,30,4], spad[5000,30,4], LastSd[5000,30,4]; 
double iask,Stop0,mr,minprofit;int y;

 double Magicbuy                 =555;
 double Magicsell                =556;
  int ticketbuy,ticketsell,profitbuy,profitsell;
 int       shag                  = 20;
 int       x1                    = 75;
 int       x2                    = 96;
 int       x3                    = 36;
 int       x4                    = 102;
 double openpricebuy,openpricesell;
 int orderclosebuy(int ticketbuy)
{
    OrderSelect(ticketbuy, SELECT_BY_TICKET, MODE_TRADES);                        
    OrderClose(ticketbuy,OrderLots(),Bid,3,CLR_NONE); 
    return(0);
} 
     
int orderclosesell(int ticketsell)
{
    OrderSelect(ticketsell, SELECT_BY_TICKET, MODE_TRADES);
    OrderClose(ticketsell,OrderLots(),Ask,3, CLR_NONE); 
    return(0); 

}
int OrdersTotalMagicbuy(int Magicbuy)
 {
   int j=0;
   int r;
   for (r=0;r<OrdersTotal();r++)
   {
     if(OrderSelect(r,SELECT_BY_POS,MODE_TRADES))
     {
        if(OrderSymbol() == Symbol())if (OrderMagicNumber()==Magicbuy) j++;
     }
   }   
 return(j); 
 }
int OrdersTotalMagicsell(int Magicsell)
{
   int d=0;
   int n;
   for (n=0;n<OrdersTotal();n++)
   {
     if(OrderSelect(n,SELECT_BY_POS,MODE_TRADES))
     {
        if(OrderSymbol() == Symbol())if (OrderMagicNumber()==Magicsell) d++;
     }
   }    
 return(d);
  
}


int init() 
  { 
//if(!IsOptimization())isopt=0.0;
//не очень хорошая инициализация. Не рекомендуется часто выключать систему 
minprofit=(15+MarketInfo(Symbol(),MODE_SPREAD))*Point;
mr=MarketInfo(Symbol(),MODE_MARGINREQUIRED);
if(IsOptimization())ii=1;
//Считывание информации из файла 
   if(ii == 0)
     { 
       FileName = "FD_" + Symbol();
       ii = 1; 
       file1 = FileOpen(FileName, FILE_BIN ); 
       if(file1>0){
       time0 = FileReadDouble(file1); 
       //----
       Probab = FileReadDouble(file1);
       dstop = FileReadInteger(file1); 
       Nstop = FileReadInteger(file1); 
       delta = FileReadInteger(file1); 
       Nidelt = FileReadInteger(file1); 
       NN = FileReadInteger(file1); 
       forg = FileReadDouble(file1);          
           /* for(idelt = 1; idelt <= Nidelt; idelt++)
                {
                  for(i = 1; i <= NN; i++)
                    { 
                      P[i,idelt] = FileReadNumber(file1);
                    }
                } 
           */     
           for(istop = 1; istop <= Nstop; istop++)
             { 
               for(idelt = 1; idelt <= Nidelt; idelt++)
                 { 
                   for(i= 0; i <= mn - 1; i++)
                     { 
                       rost[i,idelt,istop] = FileReadDouble(file1); 
                       spad[i,idelt,istop] = FileReadDouble(file1); 
                       nsd[i,idelt,istop] = FileReadInteger(file1); 
                     }
                 }
             }  
           
       FileClose(file1); 
       }
         string mode="loaded";
         ObjectCreate(mode, OBJ_LABEL, 0,0,0);ObjectSet(mode, OBJPROP_CORNER, 0);ObjectSet(mode, OBJPROP_XDISTANCE, 1);ObjectSet(mode, OBJPROP_YDISTANCE, 1);
         ObjectSetText(mode,"+", 12, "Arial Narrow", Green);

     }
   for(idelt = 1; idelt <= Nidelt; idelt++)
     { 
       delt[idelt] = delta*idelt; 
       for(i = 1; i <= NN - 1; i++)
           P[i,idelt] = iClose(Symbol(),PERIOD_W1,i-1); 
     } 
   return(0);
  } 
int deinit() 
  {
   //if(true==false)
   if(IsTesting()&&!IsOptimization())
   {
       FileName = "FD_" + Symbol();
       file1=FileOpen(FileName, FILE_BIN | FILE_WRITE); 
       FileWriteDouble(file1, CurTime()); 
       /* for(idelt = 1; idelt <= Nidelt; idelt++)
            {
              for(i=1;i<=NN;i++)
                { 
                  FileWrite(file1, P[i,idelt]);
                }    
            }
       */       

       FileWriteDouble(file1, Probab);
       FileWriteInteger(file1, dstop); 
       FileWriteInteger(file1, Nstop); 
       FileWriteInteger(file1, delta); 
       FileWriteInteger(file1, Nidelt); 
       FileWriteInteger(file1, NN); 
       FileWriteDouble(file1, forg);
       for(istop = 1; istop <= Nstop; istop++)
         { 
           for(idelt = 1; idelt <= Nidelt; idelt++)
             { 
               for(i = 0; i <= mn - 1; i++)
                 {  
                   FileWriteDouble(file1, rost[i,idelt,istop]); 
                   FileWriteDouble(file1, spad[i,idelt,istop]); 
                   FileWriteInteger(file1, nsd[i,idelt,istop]); 
                 }
             }
         } 
       FileClose(file1);  
   }
   return(0);
  } 
void closeall(int a=0){
   int o;
   if(a==1){
    for(o=OrdersTotal()-1;o>=0;o--){
      OrderSelect(o, SELECT_BY_POS, MODE_TRADES);
       //if(OrderType()==OP_BUYSTOP){OrderDelete(OrderTicket());continue;}
       //if(OrderType()==OP_BUYLIMIT){OrderDelete(OrderTicket());continue;}
       if(OrderSymbol() == Symbol())if(OrderMagicNumber()==Magicbuy)if(OrderType()==OP_BUY){OrderClose(OrderTicket(), OrderLots(), Bid, 3, Magenta);continue;}
    }   
   }else 
   if(a==2){
    for(o=OrdersTotal()-1;o>=0;o--){
      OrderSelect(o, SELECT_BY_POS, MODE_TRADES);
       //if(OrderType()==OP_SELLSTOP){OrderDelete(OrderTicket());continue;}
       //if(OrderType()==OP_SELLLIMIT){OrderDelete(OrderTicket());continue;}
       if(OrderSymbol() == Symbol())if(OrderMagicNumber()==Magicsell)if(OrderType()==OP_SELL){OrderClose(OrderTicket(), OrderLots(), Ask, 3, Magenta);continue;}
    }   
   }
}

int start() 
  {    
   buy_open = 0; sell_open = 0;iask=Ask;    
  
//----

   for(idelt = 1; idelt <= Nidelt; idelt++)
     { 
       delt[idelt] = delta*idelt; 
       if(MathAbs(iask - P[1, idelt]) > (delt[idelt] - 0.5)*Point)
         { 
           for(i = 1; i <= NN - 1; i++)P[NN+1-i, idelt] = P[NN-i, idelt];  
           P[1,idelt] = iask;
         } 
       if(MathAbs(P[1, idelt] - pt[idelt]) > 0.5*Point)
         { 
           for(i = 1; i <= NN - 1; i++)
             if(P[i,idelt] > P[i+1, idelt])sr[i, idelt] = 1;else sr[i,idelt] = 0;
           Ncomb=0;mn=1;for(i=1;i<=NN-1;i++){Ncomb = Ncomb + mn*sr[i, idelt];mn = 2*mn;} 
           
           for(istop = 1; istop <= Nstop; istop++)
             if(sd[Ncomb, idelt, istop]==0&& CurTime() - LastSd[Ncomb, idelt, istop] > 
                  20*60){sd[Ncomb, idelt, istop]=1;cen[Ncomb, idelt, istop]=iask;LastSd[Ncomb, idelt, istop] = CurTime();} 

           for(int y = 2; y <= 5; y++)
           for(istop = 1; istop <= Nstop; istop++)
             { 
               stop[istop] = dstop*istop; 
               //----
               for(i = 0; i <= mn - 1; i++)
                 { 
                   if(sd[i, idelt, istop] == 1)
                     { 
                       
                       if(iask - cen[i, idelt, istop] > stop[istop]*Point*y*0.1)
                         {
                           rost[i, idelt, istop] = rost[i, idelt, istop] / forg + 1; 
                           spad[i, idelt, istop] = spad[i, idelt, istop] / forg; 
                           sd[i, idelt, istop] = 0;
                           nsd[i, idelt, istop] = nsd[i, idelt, istop] + 1; 

                         }
                       //----
                       if(cen[i, idelt, istop] - iask > stop[istop]*Point*y*0.1)
                         {
                           spad[i, idelt, istop] = spad[i, idelt, istop] / forg + 1; 
                           rost[i, idelt, istop] = rost[i, idelt, istop] / forg; 
                           sd[i, idelt, istop] = 0;
                           nsd[i, idelt, istop] = nsd[i, idelt, istop] + 1; 

                         }  
                     } 
                 } 
             } 
           for(istop = 1; istop <= Nstop; istop++)
             {
               prob = rost[Ncomb, idelt, istop] / (rost[Ncomb, idelt, istop] + 
                      spad[Ncomb, idelt, istop] + 0.0001); 
               if(prob > Probab && nsd[Ncomb, idelt, istop] > 3 && CurTime() - 
                  LastBuyOpen > 2*60){buy_open ++;lt=dstop*istop;} 

               // ---
               prob = spad[Ncomb, idelt, istop] / (rost[Ncomb, idelt, istop] + 
                      spad[Ncomb, idelt, istop] + 0.0001); 
               if(prob > Probab && nsd[Ncomb, idelt, istop] > 3 && CurTime() - 
                  LastSellOpen > 2*60){sell_open ++;lt=dstop*istop;}

             }  
 
         }  
       pt[idelt] = P[1, idelt];

     } 

if(sell_open>buy_open)OrderNew(OP_SELL);else 
if(buy_open>sell_open)OrderNew(OP_BUY);
   profitbuy=0;profitsell=0;   
  for (int cnt=0;cnt<OrdersTotal();cnt++)
  {
    OrderSelect(cnt,SELECT_BY_POS,MODE_TRADES);
    if(OrderSymbol() == Symbol())if(OrderMagicNumber () == Magicbuy){openpricebuy = OrderOpenPrice();profitbuy = profitbuy+OrderProfit() ;ticketbuy = OrderTicket();}
    if(OrderSymbol() == Symbol())if(OrderMagicNumber () == Magicsell){openpricesell = OrderOpenPrice();profitsell = profitsell+OrderProfit();ticketsell = OrderTicket();}
  }
  if (profitbuy>0)
  {
  if (Bid>=(OrdersTotalMagicbuy(Magicbuy)*minprofit)+openpricebuy) orderclosebuy(ticketbuy);
  } 
  if (profitsell>0)
  {
  if (Ask<=(openpricesell-OrdersTotalMagicsell(Magicsell)*minprofit))orderclosesell(ticketsell);    
  } 


 
return(0);
}


void OrderNew(int cmd)
  {//if(OrdersTotal()>0)return;
  int o;psell=0;pbuy=0;
   /*   for(o=OrdersTotal()-1;o>=0;o--){
      OrderSelect(o, SELECT_BY_POS, MODE_TRADES);
         if(OrderMagicNumber()==999){
            if(OrderType()==OP_SELL&&(cmd == OP_SELL))ot++;
            if(OrderType()==OP_BUY&&(cmd == OP_BUY))ot++;
            if(OrderType()==OP_BUYSTOP&&(cmd == OP_BUY))ot++;
            if(OrderType()==OP_SELLSTOP&&(cmd == OP_SELL))ot++;
            if(OrderType()==OP_BUYLIMIT&&(cmd == OP_BUY))ot++;
            if(OrderType()==OP_SELLLIMIT&&(cmd == OP_SELL))ot++;
         }
      }*/
      

  



    if(cmd == OP_SELL)//OP_BUY
      { //closeall(1);//if(ot>0)return;
        LastSellOpen = CurTime();
        //OrderSend(Symbol(), OP_SELLLIMIT, lot/8, Bid+lt, 3,isopt*(Ask +lt+ m1), Ask +lt-p , "", Magicsell, TimeCurrent()+(Period()*60*10-1), Red);    
        //else 
        if((!IsOptimization())&&(!IsTesting()))psell=Ask - 30*Point;
        if(OrdersTotalMagicsell(Magicsell)==0){OrderSend(Symbol(), OP_SELL, lotmax(), Bid, 3,Ask + lt*Point, psell,  "", Magicsell, 0, Red);PlaySound("alert.wav");}    
        else if(OrdersTotalMagicsell(Magicsell)==1)
        for( o=OrdersTotal()-1;o>=0;o--){
           OrderSelect(o, SELECT_BY_POS, MODE_TRADES);
           if(OrderSymbol() == Symbol())if(OrderType()==OP_SELL)if(OrderMagicNumber()==Magicsell)
              if(OrderOpenPrice()>Bid){OrderModify(OrderTicket(),OrderOpenPrice(),Ask + lt*Point,psell,0);PlaySound("alert.wav");}
        }
        //lt+=m;
        //OrderSend(Symbol(), OP_BUYSTOP, lot, Ask+lt*Point, 3, Bid+lt*Point-Take0*Point,0, "", 0, TimeCurrent()+(Period()*60*4+1), Blue);
        //OrderSend(Symbol(), OP_SELLLIMIT, lot/4, Bid+lt, 3,isopt*(Ask +lt+ m1), Ask +lt-p , "", Magicsell, TimeCurrent()+(Period()*60*10-1), Red);    
        //lt+=m;
        //OrderSend(Symbol(), OP_SELLLIMIT, lot/2, Bid+lt, 3,isopt*(Ask +lt+ m1), Ask +lt-p , "", Magicsell, TimeCurrent()+(Period()*60*10-1), Red);    
        //lt+=m;
        //OrderSend(Symbol(), OP_SELLLIMIT, lot, Bid+lt, 3,isopt*(Ask +lt+ m1), Ask +lt-p , "", Magicsell, TimeCurrent()+(Period()*60*10-1), Red);    
        



      } 
    else
      { //closeall(2);//if(ot>0)return;
        LastBuyOpen = CurTime(); 
        //OrderSend(Symbol(), OP_BUYLIMIT, lot/8, Ask-lt, 3,isopt*(Bid -lt- m1), Bid -lt+p , "", Magicbuy, TimeCurrent()+(Period()*60*10-1), Blue);
        //else 
        if((!IsOptimization())&&(!IsTesting()))pbuy=Bid + 30*Point;
        if(OrdersTotalMagicbuy(Magicbuy)==0){OrderSend(Symbol(), OP_BUY, lotmax(), Ask, 3,Bid - lt*Point, pbuy, "", Magicbuy, 0, Blue);PlaySound("alert.wav");}
        else if(OrdersTotalMagicbuy(Magicbuy)==1)
        for( o=OrdersTotal()-1;o>=0;o--){
           OrderSelect(o, SELECT_BY_POS, MODE_TRADES);
           if(OrderSymbol() == Symbol())if(OrderType()==OP_BUY)if(OrderMagicNumber()==Magicbuy)
              if(OrderOpenPrice()<Ask){OrderModify(OrderTicket(),OrderOpenPrice(),Bid - lt*Point, pbuy,0);PlaySound("alert.wav");}
        }
        //lt+=m;
        //OrderSend(Symbol(), OP_SELLSTOP, lot, Bid-lt*Point, 3, Ask-lt*Point+Take0*Point, 0,"", 0, TimeCurrent()+(Period()*60*4+1), Red);    
        //OrderSend(Symbol(), OP_BUYLIMIT, lot/4, Ask-lt, 3,isopt*(Bid -lt- m1) , Bid -lt+p , "", Magicbuy, TimeCurrent()+(Period()*60*10-1), Blue);
        //lt+=m;
        //OrderSend(Symbol(), OP_BUYLIMIT, lot/2, Ask-lt, 3,isopt*(Bid -lt- m1) , Bid -lt+p , "", Magicbuy, TimeCurrent()+(Period()*60*10-1), Blue);
        //lt+=m;
        //OrderSend(Symbol(), OP_BUYLIMIT, lot, Ask-lt, 3,isopt*(Bid -lt- m1), Bid -lt+p , "", Magicbuy, TimeCurrent()+(Period()*60*10-1), Blue);
        
      } 
  }
double lotmax(){
  double lot;
  lot=NormalizeDouble(MathAbs(AccountBalance()/1.51/mr),1);
  //
  if(IsTesting())lot=0.1;
  if(!IsTesting())
  if(!IsOptimization()){if(AccountFreeMargin()>=1000)lot=NormalizeDouble(MathAbs(AccountFreeMargin()/1.04/mr),1);else lot=0.1;}
  if(lot<0.1)lot=0.1;if(lot>9999)lot=9999;//
return (lot);
}