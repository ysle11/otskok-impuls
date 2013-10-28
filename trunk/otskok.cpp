#include <windows.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>

#include "main.h"
#include "otskok.h"
//#include "Server.h"
//extern Server* server;

extern void *Mmalloc(size_t size);
extern char *Mstrdup(const char *s);
extern void *Mrealloc(void* ptr, size_t size);
extern void Mfree(void *ptr);
extern void wlog(const char* buffer);
extern void title(int h,const char* buffer);
extern void wcmd(int reqestor);
extern HWND hwnd,hlog,hpro,hper,hcmd;

Otskok::Otskok(){}
Otskok::~Otskok(){}

void Otskok::action(int t,int tperiod,int historymode,bool tcurbar)
{
    testerperiod=tperiod;
    actmode=historymode;
    tradecurbar=tcurbar;
	if(t==testing){mode=testing;test();}
	if(t==optimizing){mode=optimizing;optimize();}
	if(t==debuging){mode=debuging;debug();}
}
inline char* Otskok::intToStr(int i){
	static char strt[18]="";memset(strt,0,18);
	snprintf(strt, 17, "%lld", i);

	return (char *)strt;
}
inline bool Otskok::strToInt(const char *s, int *i){
	char *endChar;
	setlocale(LC_NUMERIC, "C");
	int t = strtol(s, &endChar, 10);
	*i=t;
	if(*endChar!='\0')return false;

	return true;
}
inline int Otskok::strToInt(const char *s){
	char *endChar;
	setlocale(LC_NUMERIC, "C");
	int t = strtol(s, &endChar, 10);
	if(*endChar!='\0')return -1;

	return t;
}
inline char* Otskok::doubleToStr(double d,int precsion) {
	static char strotskok[18]="";
	memset(strotskok,0,18);
	snprintf(strotskok, 12, "%.*f", precsion,d);

	return (char *)strotskok;
}
inline char* Otskok::timeToStr(const time_t st) {
	static char strtime1[28]="";
    tm stm1;
    time_t st1;
    st1=time(0);
	memset(strtime1,0,28);
	memset(&stm1,0,sizeof(struct tm));
	if(st==0)
	stm1=*localtime((const time_t *)&st1);
	else
	stm1=*localtime((const time_t *)&st);
	strftime(strtime1,27,"%d.%m.%Y %H:%M:%S ",&stm1);
	return (char *)strtime1;
}
inline char* Otskok::gmtimeToStr(const time_t st) {
	static char gmstrtime[28]="";
     tm stm1;
     time_t st1;
    st1=time(0);
	memset(&gmstrtime,0,28);
	memset(&stm1,0,sizeof(struct tm));
	if(st==0)
	stm1=*gmtime((const time_t *)&st1);
	else
	stm1=*gmtime((const time_t *)&st);
	strftime(gmstrtime,27,"%d.%m.%Y %H:%M ",&stm1);
	return (char *)gmstrtime;
}
inline int Otskok::rdtsc(){__asm__ __volatile__("rdtsc");}
inline char* Otskok::substr(const char* S, const int pos, const int len){
	int i;
	char *SubS;
	if ( pos+len <= strlen(S)){
		SubS = (char *)Mmalloc(len+1);
		for (i=0;i<len;i++) SubS[i] = S[pos+i];
		SubS[len] = (char) NULL;
	} else SubS = NULL;

	return (char *)SubS;
}
int Otskok::find(const char *s, const char *key)
{
    whitespaceskip;
	int len = strlen(key);
	for(int i = 0; *s; i++)
	{
		const char *a = s, *e = s;
		elementskip;
		if(*e=='"')
		{
			e++;
			if(s[-1]=='"') --s;
		}
		if(s-e==len && !strncmp(e, key, s-e)) return i;
		else s = a;
		elementskip, whitespaceskip;
	}
    return -1;
}
/* Journals */
void Otskok::journalsinit(bool init){
	if(!init){
		history = (struct deal *)Mmalloc(sizeof(struct deal)*2);
		trades = (struct deal *)Mmalloc(sizeof(struct deal)*2);
	}else{
		history = (struct deal *)Mrealloc(history,sizeof(struct deal)*2);
		trades = (struct deal *)Mrealloc(trades,sizeof(struct deal)*2);
	}
	iTradesTotal=0;
	iTradesTotalOP_BUY=0;
	iTradesTotalOP_SELL=0;
	iTradesTotalOP_BUYLIMIT=0;
	iTradesTotalOP_SELLLIMIT=0;
	iTradesTotalOP_BUYSTOP=0;
	iTradesTotalOP_SELLSTOP=0;
	iHistoryTotal=0;
	iTradesCurrent=0;
	iHistoryCurrent=0;
	minprofit=0;
	maxprofit=0;
	totalprofit=0;
	curorderticket=0;
	totalorders=0;
	drawdowncnt=0;
	profitcnt=0;
	minprofitcntmax=0;
	maxprofitcntmax=0;
	drawdowncntmax=1;
	profitcntmax=0;
	prevorderprofitorno=false;
	lorderscnt=0;
	sorderscnt=0;
	testertargetprofit=testerpoint*testerspread;//(testerpoint*(testerspread+testerstop))*testertargetprofitmul;
}
void Otskok::add_row_history(double tOrderPrice, int opclose){
	history[iHistoryTotal].iOrderTicket=trades[iTradesCurrent].iOrderTicket;
	history[iHistoryTotal].iOrderType=trades[iTradesCurrent].iOrderType;
	history[iHistoryTotal].iOpenPrice=trades[iTradesCurrent].iOpenPrice;
	history[iHistoryTotal].iOrderOpenPrice=trades[iTradesCurrent].iOrderOpenPrice;
	history[iHistoryTotal].iOrderClosePrice=tOrderPrice;
	history[iHistoryTotal].iOrderStopLoss=trades[iTradesCurrent].iOrderStopLoss;
	history[iHistoryTotal].iOrderTakeProfit=trades[iTradesCurrent].iOrderTakeProfit;
	history[iHistoryTotal].iOrderOpenTime=trades[iTradesCurrent].iOrderOpenTime;
	history[iHistoryTotal].iOrderCloseTime=testercurdatetime;
	if(trades[iTradesCurrent].iOrderType<=OP_SELL)
	history[iHistoryTotal].iOrderOpclose=opclose;
	if(history[iHistoryTotal].iOrderType==OP_BUY)
	history[iHistoryTotal].iOrderProfit=(int)((history[iHistoryTotal].iOrderClosePrice
					-history[iHistoryTotal].iOrderOpenPrice)/testerpoint-testerspread);else
	if(history[iHistoryTotal].iOrderType==OP_SELL)
	history[iHistoryTotal].iOrderProfit=(int)((history[iHistoryTotal].iOrderOpenPrice
					-history[iHistoryTotal].iOrderClosePrice)/testerpoint-testerspread);
	if((history[iHistoryTotal].iOrderType==OP_SELL)||(history[iHistoryTotal].iOrderType==OP_BUY)){
		if(history[iHistoryTotal].iOrderProfit<=0){
			minprofit+=history[iHistoryTotal].iOrderProfit;
			drawdowncnt++;
			maxprofitcntmax=0;
			if(!prevorderprofitorno)minprofitcntmax++;
			prevorderprofitorno=false;
			if(drawdowncntmax<minprofitcntmax)drawdowncntmax=minprofitcntmax;
		}else
		if(history[iHistoryTotal].iOrderProfit>0){
			maxprofit+=history[iHistoryTotal].iOrderProfit;
			profitcnt++;
			minprofitcntmax=0;
			if(prevorderprofitorno)maxprofitcntmax++;
			prevorderprofitorno=true;
			if(profitcntmax<maxprofitcntmax)profitcntmax=maxprofitcntmax;
			if(history[iHistoryTotal].iOrderType==OP_BUY)lorderscnt++;else
			if(history[iHistoryTotal].iOrderType==OP_SELL)sorderscnt++;
		}
	}else
	if(opclose==OP_DELETE)history[iHistoryTotal].iOrderProfit=0;
	iHistoryTotal++;
	history = (struct deal *)Mrealloc(history,sizeof(struct deal)*(iHistoryTotal+2));

}
void Otskok::add_row_trades(short tCmd,double tPrice,double tStop,double tTake){
	curorderticket++;
	totalorders++;
	
	if(tCmd==OP_BUY)      iTradesTotalOP_BUY++;else
	if(tCmd==OP_SELL)     iTradesTotalOP_SELL++;else
	if(tCmd==OP_BUYLIMIT) iTradesTotalOP_BUYLIMIT++;else
	if(tCmd==OP_SELLLIMIT)iTradesTotalOP_SELLLIMIT++;else
	if(tCmd==OP_BUYSTOP)  iTradesTotalOP_BUYSTOP++;else
	if(tCmd==OP_SELLSTOP) iTradesTotalOP_SELLSTOP++;
	
	trades[iTradesTotal].iOrderTicket=curorderticket;
	trades[iTradesTotal].iOrderType=tCmd;
	trades[iTradesTotal].iOrderOpenPrice=tPrice;
	//if(tCmd==OP_SELL)trades[iTradesTotal].iOrderClosePrice=testercurl;else
	//if(tCmd==OP_BUY)trades[iTradesTotal].iOrderClosePrice=testercurh;
	trades[iTradesTotal].iOpenPrice=testercuro;
	trades[iTradesTotal].iOrderStopLoss=tStop;
	trades[iTradesTotal].iOrderTakeProfit=tTake;
	trades[iTradesTotal].iOrderOpclose=OP_DELETE;
	trades[iTradesTotal].iOrderOpenTime=testercurdatetime;
	trades[iTradesTotal].iOrderCloseTime=testercurdatetime;
	iTradesTotal++;
	trades = (struct deal *)Mrealloc(trades,sizeof(struct deal)*(iTradesTotal+2));
}
void Otskok::del_row_trades(double tOrderPrice, short opclose){
	if(iTradesTotal>0)if(iTradesCurrent>=0)
	{
        short tCmd=trades[iTradesCurrent].iOrderType;
		if(tCmd==OP_BUY)      iTradesTotalOP_BUY--;else
		if(tCmd==OP_SELL)     iTradesTotalOP_SELL--;else
		if(tCmd==OP_BUYLIMIT) iTradesTotalOP_BUYLIMIT--;else
		if(tCmd==OP_SELLLIMIT)iTradesTotalOP_SELLLIMIT--;else
		if(tCmd==OP_BUYSTOP)  iTradesTotalOP_BUYSTOP--;else
		if(tCmd==OP_SELLSTOP) iTradesTotalOP_SELLSTOP--;

	//if(tCmd==OP_SELL)tOrderPrice=testercurl;else
	//if(tCmd==OP_BUY)tOrderPrice=testercurh;

		add_row_history(tOrderPrice,opclose);
		for(int i=iTradesCurrent;i<iTradesTotal;i++){
			trades[i].iOrderTicket=trades[i+1].iOrderTicket;
			trades[i].iOrderType=trades[i+1].iOrderType;
			trades[i].iOpenPrice=trades[i+1].iOpenPrice;
			trades[i].iOrderProfit=trades[i+1].iOrderProfit;
			trades[i].iOrderOpenPrice=trades[i+1].iOrderOpenPrice;
			trades[i].iOrderClosePrice=trades[i+1].iOrderClosePrice;
			trades[i].iOrderStopLoss=trades[i+1].iOrderStopLoss;
			trades[i].iOrderTakeProfit=trades[i+1].iOrderTakeProfit;
			trades[i].iOrderOpenTime=trades[i+1].iOrderOpenTime;
			trades[i].iOrderCloseTime=trades[i+1].iOrderCloseTime;
			trades[i].iOrderOpclose=trades[i+1].iOrderOpclose;
		}
		iTradesTotal--;if(iTradesTotal<0)iTradesTotal=0;
		iTradesCurrent--;if(iTradesCurrent<0)iTradesCurrent=0;
		trades = (struct deal *)Mrealloc(trades,sizeof(struct deal)*(iTradesTotal+2));
	}
}
void Otskok::OrderSelect(int tIndex, int selmode){
	if(selmode==MODE_TRADES)
	{
		iTradesCurrent=tIndex;iHistoryCurrent=-1;
		curOrderTicket=trades[tIndex].iOrderTicket;
		curOrderType=trades[tIndex].iOrderType;
		curOrderProfit=trades[tIndex].iOrderProfit;
		curOrderOpenTime=trades[tIndex].iOrderOpenTime;
		curOrderCloseTime=trades[tIndex].iOrderCloseTime;
		curOpenPrice=trades[tIndex].iOpenPrice;
		curOrderOpenPrice=trades[tIndex].iOrderOpenPrice;
		curOrderClosePrice=trades[tIndex].iOrderClosePrice;
		curOrderStopLoss=trades[tIndex].iOrderStopLoss;
		curOrderTakeProfit=trades[tIndex].iOrderTakeProfit;
		curOrderOpclose=trades[tIndex].iOrderOpclose;
	} else
	if(selmode==MODE_HISTORY)
	{
		iHistoryCurrent=tIndex;iTradesCurrent=-1;
		curOrderTicket=history[tIndex].iOrderTicket;
		curOrderType=history[tIndex].iOrderType;
		curOrderProfit=history[tIndex].iOrderProfit;
		curOrderOpenTime=history[tIndex].iOrderOpenTime;
		curOrderCloseTime=history[tIndex].iOrderCloseTime;
		curOpenPrice=history[tIndex].iOpenPrice;
		curOrderOpenPrice=history[tIndex].iOrderOpenPrice;
		curOrderClosePrice=history[tIndex].iOrderClosePrice;
		curOrderStopLoss=history[tIndex].iOrderStopLoss;
		curOrderTakeProfit=history[tIndex].iOrderTakeProfit;
		curOrderOpclose=history[tIndex].iOrderOpclose;
	}
}
void Otskok::OrderModify(double tOrderPrice,double tStop,double tTake){
	if(trades[iTradesCurrent].iOrderType<=OP_SELL){
		trades[iTradesCurrent].iOrderStopLoss=tStop;
		trades[iTradesCurrent].iOrderTakeProfit=tTake;
	}
}
void Otskok::OrderSend(int cmd, double price, double stop, double take){
	//if(!tradecurbar){if(cmd==OP_BUYSTOP)stop=price-testerpoint*250;if(cmd==OP_SELLSTOP)stop=price+testerpoint*250;}
	if(cmd==OP_BUY)      add_row_trades(OP_BUY      ,price,stop,take);else
	if(cmd==OP_SELL)     add_row_trades(OP_SELL     ,price,stop,take);else
	if(cmd==OP_BUYLIMIT) add_row_trades(OP_BUYLIMIT ,price,stop,take);else
	if(cmd==OP_SELLLIMIT)add_row_trades(OP_SELLLIMIT,price,stop,take);else
	if(cmd==OP_BUYSTOP)  add_row_trades(OP_BUYSTOP  ,price,stop,take);else
	if(cmd==OP_SELLSTOP) add_row_trades(OP_SELLSTOP ,price,stop,take);
	iOrdersControl();
}
void Otskok::OrderClose(int OrderTicket, double tOrderPrice, int opclose){
	if(trades[iTradesCurrent].iOrderTicket==OrderTicket)
	del_row_trades(tOrderPrice,opclose);
}
void Otskok::OrderDelete(int OrderTicket){
    short tCmd=trades[iTradesCurrent].iOrderType;
	if((tCmd==OP_BUYLIMIT)||(tCmd==OP_BUYSTOP)||(tCmd==OP_SELLLIMIT)||(tCmd==OP_SELLSTOP))
    if(trades[iTradesCurrent].iOrderTicket==OrderTicket)
    if(((mode!=optimizing)&&(testermetadata->ctm[testercntper-1]!=testercurdatetime))||(mode!=testing))
	del_row_trades(0.0,OP_DELETE);
}
void Otskok::iOrdersControl(){
	double price=testercurc;
	double priceh=testercurh;
	double pricel=testercurl;
	int datetime=testercurdatetime;
	for(int i=0;i<iTradesTotal;i++){
		OrderSelect(i,MODE_TRADES);
		if(((mode!=optimizing)&&(testermetadata->ctm[testercntper-1]!=testercurdatetime))||(mode!=testing))
		{
			if(trades[i].iOrderType==OP_BUYSTOP){
				if(trades[i].iOrderOpenPrice<=priceh){
	        		iTradesTotalOP_BUYSTOP--;iTradesTotalOP_BUY++;
					history[iHistoryTotal].iOrderTicket=trades[i].iOrderTicket;
					history[iHistoryTotal].iOrderType=trades[i].iOrderType;
					history[iHistoryTotal].iOpenPrice=trades[i].iOpenPrice;
					history[iHistoryTotal].iOrderOpenPrice=trades[i].iOrderOpenPrice;
					history[iHistoryTotal].iOrderClosePrice=0.0;
					history[iHistoryTotal].iOrderStopLoss=trades[i].iOrderStopLoss;
					history[iHistoryTotal].iOrderTakeProfit=trades[i].iOrderTakeProfit;
					history[iHistoryTotal].iOrderOpenTime=trades[i].iOrderOpenTime;
					history[iHistoryTotal].iOrderCloseTime=datetime;
					history[iHistoryTotal].iOrderOpclose=OP_BUY;
					history[iHistoryTotal].iOrderProfit=0;
					iHistoryTotal++;
					history = (struct deal *)realloc(history,sizeof(struct deal)*(iHistoryTotal+2));

					trades[i].iOrderType=OP_BUY;
					trades[i].iOrderOpenTime=testercurdatetime;
					trades[i].iOrderCloseTime=testercurdatetime;
					trades[i].iOrderProfit=(int)((price-trades[i].iOrderOpenPrice)/testerpoint-testerspread);
				} else if(datetime-trades[i].iOrderOpenTime>testerltime)OrderDelete(trades[i].iOrderTicket);
			}else
			if(trades[i].iOrderType==OP_SELLSTOP){
				if(trades[i].iOrderOpenPrice>=pricel){
	        		iTradesTotalOP_SELLSTOP--;iTradesTotalOP_SELL++;
					history[iHistoryTotal].iOrderTicket=trades[i].iOrderTicket;
					history[iHistoryTotal].iOrderType=trades[i].iOrderType;
					history[iHistoryTotal].iOpenPrice=trades[i].iOpenPrice;
					history[iHistoryTotal].iOrderOpenPrice=trades[i].iOrderOpenPrice;
					history[iHistoryTotal].iOrderClosePrice=0.0;
					history[iHistoryTotal].iOrderStopLoss=trades[i].iOrderStopLoss;
					history[iHistoryTotal].iOrderTakeProfit=trades[i].iOrderTakeProfit;
					history[iHistoryTotal].iOrderOpenTime=trades[i].iOrderOpenTime;
					history[iHistoryTotal].iOrderCloseTime=datetime;
					history[iHistoryTotal].iOrderOpclose=OP_SELL;
					history[iHistoryTotal].iOrderProfit=0;
					iHistoryTotal++;
					history = (struct deal *)realloc(history,sizeof(struct deal)*(iHistoryTotal+2));

					trades[i].iOrderType=OP_SELL;
					trades[i].iOrderOpenTime=testercurdatetime;
					trades[i].iOrderCloseTime=testercurdatetime;
	    			trades[i].iOrderProfit=(int)((trades[i].iOrderOpenPrice-price)/testerpoint-testerspread);
				} else if(datetime-trades[i].iOrderOpenTime>testerltime)OrderDelete(trades[i].iOrderTicket);
			}else
			if(trades[i].iOrderType==OP_BUYLIMIT){
				if(trades[i].iOrderOpenPrice>=pricel&&trades[i].iOrderOpenPrice<=priceh){
	                iTradesTotalOP_BUYLIMIT--;iTradesTotalOP_BUY++;
					history[iHistoryTotal].iOrderTicket=trades[i].iOrderTicket;
					history[iHistoryTotal].iOrderType=trades[i].iOrderType;
					history[iHistoryTotal].iOpenPrice=trades[i].iOpenPrice;
					history[iHistoryTotal].iOrderOpenPrice=trades[i].iOrderOpenPrice;
					history[iHistoryTotal].iOrderClosePrice=0.0;
					history[iHistoryTotal].iOrderStopLoss=trades[i].iOrderStopLoss;
					history[iHistoryTotal].iOrderTakeProfit=trades[i].iOrderTakeProfit;
					history[iHistoryTotal].iOrderOpenTime=trades[i].iOrderOpenTime;
					history[iHistoryTotal].iOrderCloseTime=datetime;
					history[iHistoryTotal].iOrderOpclose=OP_BUY;
					history[iHistoryTotal].iOrderProfit=0;
					iHistoryTotal++;
					history = (struct deal *)realloc(history,sizeof(struct deal)*(iHistoryTotal+2));

					trades[i].iOrderType=OP_BUY;
					trades[i].iOrderOpenTime=testercurdatetime;
					trades[i].iOrderCloseTime=testercurdatetime;
					trades[i].iOrderProfit=(int)((price-trades[i].iOrderOpenPrice)/testerpoint-testerspread);
				} else if(datetime-trades[i].iOrderOpenTime>testerltime)OrderDelete(trades[i].iOrderTicket);
			}else
			if(trades[i].iOrderType==OP_SELLLIMIT){
				if(trades[i].iOrderOpenPrice<=priceh&&trades[i].iOrderOpenPrice>=pricel){
	                iTradesTotalOP_SELLLIMIT--;iTradesTotalOP_SELL++;
					history[iHistoryTotal].iOrderTicket=trades[i].iOrderTicket;
					history[iHistoryTotal].iOrderType=trades[i].iOrderType;
					history[iHistoryTotal].iOpenPrice=trades[i].iOpenPrice;
					history[iHistoryTotal].iOrderOpenPrice=trades[i].iOrderOpenPrice;
					history[iHistoryTotal].iOrderClosePrice=0.0;
					history[iHistoryTotal].iOrderStopLoss=trades[i].iOrderStopLoss;
					history[iHistoryTotal].iOrderTakeProfit=trades[i].iOrderTakeProfit;
					history[iHistoryTotal].iOrderOpenTime=trades[i].iOrderOpenTime;
					history[iHistoryTotal].iOrderCloseTime=datetime;
					history[iHistoryTotal].iOrderOpclose=OP_SELL;
					history[iHistoryTotal].iOrderProfit=0;
					iHistoryTotal++;
					history = (struct deal *)realloc(history,sizeof(struct deal)*(iHistoryTotal+2));

					trades[i].iOrderType=OP_SELL;
					trades[i].iOrderOpenTime=testercurdatetime;
					trades[i].iOrderCloseTime=testercurdatetime;
					trades[i].iOrderProfit=(int)((trades[i].iOrderOpenPrice-price)/testerpoint-testerspread);
				} else if(datetime-trades[i].iOrderOpenTime>testerltime)OrderDelete(trades[i].iOrderTicket);
			}
		}
		if(trades[i].iOrderType==OP_BUY){
            //trades[i].iOrderProfit=(int)((price-trades[i].iOrderOpenPrice)/testerpoint-testerspread);
			if(trades[i].iOrderStopLoss>=pricel&&trades[i].iOrderStopLoss!=0.0)
				OrderClose(trades[i].iOrderTicket,trades[i].iOrderStopLoss,OP_SL);else
			if(trades[i].iOrderTakeProfit<=priceh&&trades[i].iOrderTakeProfit!=0.0)
				OrderClose(trades[i].iOrderTicket,trades[i].iOrderTakeProfit,OP_TP);else
			OrderClose(trades[i].iOrderTicket,testercurc,OP_CLOSE);
		}else
		if(trades[i].iOrderType==OP_SELL){
            //trades[i].iOrderProfit=(int)((trades[i].iOrderOpenPrice-price)/testerpoint-testerspread);
			if(trades[i].iOrderStopLoss<=priceh&&trades[i].iOrderStopLoss!=0.0)
				OrderClose(trades[i].iOrderTicket,trades[i].iOrderStopLoss,OP_SL);else
			if(trades[i].iOrderTakeProfit>=pricel&&trades[i].iOrderTakeProfit!=0.0)
				OrderClose(trades[i].iOrderTicket,trades[i].iOrderTakeProfit,OP_TP);else
			OrderClose(trades[i].iOrderTicket,testercurc,OP_CLOSE);
		}
	}
	testercontrol();
}
/* Tester */
void Otskok::testerinit()
{
    memset(testervals,0,sizeof(testervals));
/*	strcpy(&testervals[0][0],"#HPQ");
	testervalspreads[0]=3;
	testervalstops[0]=10;
	strcpy(&testervals[1][0],"#AA");
	testervalspreads[1]=3;
	testervalstops[1]=10;
	strcpy(&testervals[2][0],"#MSFT");
	testervalspreads[2]=3;
	testervalstops[2]=10;
	strcpy(&testervals[3][0],"#YM");
	testervalspreads[3]=3;
	testervalstops[3]=5;
	strcpy(&testervals[4][0],"#EP");
	testervalspreads[4]=25;
	testervalstops[4]=5;
	strcpy(&testervals[5][0],"#ENQ");
	testervalspreads[5]=50;
	testervalstops[5]=5;
    testervalcnt=6;*/
	strcpy(&testervals[0][0],"EURUSD");
	testervalspreads[0]=1;
	testervalstops[0]=10;
	strcpy(&testervals[1][0],"USDCHF");
	testervalspreads[1]=2;
	testervalstops[1]=10;
	strcpy(&testervals[2][0],"GBPCHF");
	testervalspreads[2]=5;
	testervalstops[2]=10;
	strcpy(&testervals[3][0],"GBPUSD");
	testervalspreads[3]=2;
	testervalstops[3]=10;
	strcpy(&testervals[4][0],"USDJPY");
	testervalspreads[4]=2;
	testervalstops[4]=10;
	strcpy(&testervals[5][0],"USDCAD");
	testervalspreads[5]=2;
	testervalstops[5]=10;
	strcpy(&testervals[6][0],"AUDUSD");
	testervalspreads[6]=2;
	testervalstops[6]=10;
	strcpy(&testervals[7][0],"NZDUSD");
	testervalspreads[7]=3;
	testervalstops[7]=10;
	strcpy(&testervals[8][0],"GBPJPY");
	testervalspreads[8]=5;
	testervalstops[8]=10;
	strcpy(&testervals[9][0],"CHFJPY");
	testervalspreads[9]=3;
	testervalstops[9]=10;
	strcpy(&testervals[10][0],"EURJPY");
	testervalspreads[10]=3;
	testervalstops[10]=10;
    testervalcnt=11;

    memset(testersmas,0,sizeof(testersmas));

    strcpy(&testersmas[0][0],"adxmain");
    strcpy(&testersmas[1][0],"adxup");
    strcpy(&testersmas[2][0],"adxdn");
    strcpy(&testersmas[3][0],"sma");
    strcpy(&testersmas[4][0],"cci");
    strcpy(&testersmas[5][0],"rsi");
    strcpy(&testersmas[6][0],"ichimoku");
    strcpy(&testersmas[7][0],"momentum");
    strcpy(&testersmas[8][0],"ema");
    strcpy(&testersmas[9][0],"bandsup");
    strcpy(&testersmas[10][0],"bandsdn");
    strcpy(&testersmas[11][0],"bears");
    strcpy(&testersmas[12][0],"bulls");
    strcpy(&testersmas[13][0],"osma");
    strcpy(&testersmas[14][0],"stddev");
    strcpy(&testersmas[15][0],"force");
    strcpy(&testersmas[16][0],"ac");
    strcpy(&testersmas[17][0],"tma");
    strcpy(&testersmas[18][0],"zerolagema");
    strcpy(&testersmas[19][0],"volumesma");

	strcpy(&testersmas[20][0],"randma");
    strcpy(&testersmas[21][0],"demarker");
    strcpy(&testersmas[22][0],"atr");
    strcpy(&testersmas[23][0],"stochastic");
    strcpy(&testersmas[24][0],"adx");
    strcpy(&testersmas[25][0],"gma");
    strcpy(&testersmas[26][0],"stochastic");
    strcpy(&testersmas[27][0],"demarker2");
    strcpy(&testersmas[28][0],"lssma");
    strcpy(&testersmas[29][0],"lwma");
    strcpy(&testersmas[30][0],"sinema");
    strcpy(&testersmas[31][0],"rvi");
    strcpy(&testersmas[32][0],"adx2");
    strcpy(&testersmas[33][0],"rvi2");

    testersmacnt=34;//14
    testerfxok=false;
	testerdataok=false;
    memset(testerpath,0,sizeof(testerpath));
//    lstrcat(testerpath,"history\\MMCIS-Demo\\");
    lstrcat(testerpath,"f:\\Program Files\\MMCIS MetaTrader 4 Client Terminal\\history\\MMCIS-Demo\\");
    testerbacktest=0;
    testercuritem=0;
    testertargetprofitmul=2;optstop=1;
    testercntper=2000;testercntpervoid=testercntper;
    testerconsolidationbars=3;
    testerdtime=86399;
	testerltime=431999;
	testerquant=3;
	testerisoptimize=false;

	testermdrawdownclimit=2;
	memset(strategyset,0,sizeof(strategyset));
	lstrcat(strategyset,"patterns.");
	if(actmode==light)lstrcat(strategyset,"l");else
	if(actmode==medium)lstrcat(strategyset,"m");else
	if(actmode==hard)lstrcat(strategyset,"h");
	lstrcat(strategyset,intToStr(testerperiod));
	switch(testerperiod){
		case 10080:{
            testerbacktest=0;
            testertargetprofitmul=1;
            testerconsolidationbars=1;
			optstop=45;
			if(mode==optimizing){optstop=45;/*testerdtime*=120;testerltime*=120;*/}
			break;}
		case 1440:{
            testerbacktest=0;
            testertargetprofitmul=1;
            testerconsolidationbars=1;
			optstop=12;
			if(mode==optimizing){optstop=12;/*testerdtime*=24;testerltime*=24;*/}
			break;}
		case 240:{
            testerbacktest=0;
            testertargetprofitmul=1;
            testerconsolidationbars=1;
			optstop=11;
			if(mode==optimizing){optstop=11;/*testerdtime*=8;testerltime*=8;*/}
			break;}
		case 60:{
            testerbacktest=0;
            testertargetprofitmul=1;
            testerconsolidationbars=1;
			optstop=11;
			if(mode==optimizing){optstop=11;/*testerdtime*=8;testerltime*=8;*/}
			break;}
		case 15:{
            testerbacktest=0;
            testertargetprofitmul=1;
            testerconsolidationbars=1;
			optstop=11;
			if(mode==optimizing){optstop=11;/*testerdtime*=8;testerltime*=8;*/}
			break;}
	}
//  testerbacktest=0;
	if(mode==optimizing){
        testerbacktest=0;
        //testertargetprofitmul*=20;
		testeroptcnf = new optimizationconfig[1];
		memset(testeroptcnf,0,sizeof(struct optimizationconfig));
		testeroptcnf[0].period=testerperiod;
		testeroptcnf[0].cntpervoid=testercntpervoid;
		testeroptcnf[0].dtime=testerdtime;
		testeroptcnf[0].ltime=testerltime;
		testeroptcnf[0].quant=testerquant;
		memcpy(testeroptcnf[0].smas,testersmas,sizeof(testersmas));
		memcpy(testeroptcnf[0].vals,testervals,sizeof(testervals));
		memcpy(testeroptcnf[0].valstops,testervalstops,sizeof(testervalstops));
		memcpy(testeroptcnf[0].valspreads,testervalspreads,sizeof(testervalspreads));
		testeroptcnf[0].valcnt=testervalcnt;
		testeroptcnf[0].smacnt=testersmacnt;
		testeroptval = new optimizationvals[testervalcnt*testersmacnt*2];
		memset(testeroptval,0,sizeof(struct optimizationvals)*testervalcnt*testersmacnt*2);}
	else{
        //testertargetprofitmul*=80;
		testeroptcnf=0;
		testeroptval=0;
	}
	journalsinit(false);
    testermetadata=0;
}
void Otskok::testerloaddata()
{
	if(testermetadata!=0){delete[] testermetadata;testermetadata=0;}
	testermetadata = new mdata[1];
	HANDLE hFile;DWORD dwRead;
	memset(testermetadata,0,sizeof(struct mdata));
	char fullpath[256],tmp[20];memset(fullpath,0,256);memset(tmp,0,20);
	lstrcat(fullpath,testerpath);
	memset(datfile,0,255);
	lstrcat(datfile,fullpath);
	lstrcat(fullpath,testervals[testercuritem]);
	snprintf(tmp, 20, "%lld", testerperiod);
	lstrcat(fullpath,tmp);
	lstrcat(fullpath,".hst");
	testerdataok=false;

	hFile = CreateFile(fullpath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
	int i1=0;
	if(!(!hFile)){
		int dwFileSize = GetFileSize(hFile, NULL);
		if(dwFileSize>=148+44){
			testerdataok=true;
			i1=0;
			char* membuf = new char[dwFileSize];
			if(actmode==light){testercntper=2100+((rand()%5)+1)*15;if(testerperiod==15)testercntper*=2;}else //*(dwFileSize-148)/44/3;515;if(testerperiod==60)testercntper=715*/;if(testerperiod==240)testercntper*=2;}else	//(dwFileSize-148)/44/3
            if(actmode==medium)testercntper=(dwFileSize-148)/44-(dwFileSize-148)/44/3;else
            if(actmode==hard)testercntper=(dwFileSize-148)/44-10;
            //if(testercntper<1300)testercntper=1300;
			int i=dwFileSize-44*testercntper-44*testerbacktest;
			if((int)((dwFileSize-148)/44)<testercntper-testerbacktest){i=(dwFileSize-148)/44;testercntper=i-testerbacktest;i=148;}
			SetFilePointer(hFile,i,NULL,FILE_BEGIN);
			ReadFile(hFile, membuf,44*testercntper, &dwRead, NULL);
			int t1=testertestfrom,t2=testertestto;
			i=0;while(i1<testercntper){
				memcpy(&testermetadata->ctm[i1],&membuf[i],4);i+=4;
				memcpy(&testermetadata->open[i1],&membuf[i],8);i+=8;
				memcpy(&testermetadata->low[i1],&membuf[i],8);i+=8;
				memcpy(&testermetadata->high[i1],&membuf[i],8);i+=8;
				memcpy(&testermetadata->close[i1],&membuf[i],8);i+=8;
				memcpy(&testermetadata->volume[i1],&membuf[i],8);i+=8;
                if(mode==optimizing)
				if(testertestfrom!=0&&testertestto!=0&&i1>0){
					if(testermetadata->ctm[i1]>t1&&testermetadata->ctm[i1-1]<=t1)testertestfrom=i1-1;
					if(testermetadata->ctm[i1]>t2&&testermetadata->ctm[i1-1]<=t2)testertestto=i1;
				}
				i1++;
			}
			SetFilePointer(hFile,84,NULL,FILE_BEGIN);
			ReadFile(hFile, &testerdigits,4, &dwRead, NULL);
			CloseHandle(hFile);
			testerpoint=1/pow((double)10,(int)testerdigits);
			testerspread=testervalspreads[testercuritem];
			testerstop=testervalstops[testercuritem];
			testertargetprofit=testerpoint*testerspread;//testerpoint*(testerspread+testerstop)*testertargetprofitmul;
			delete []membuf;
		}
		testermincnttrades=292;
		if(testerperiod==10080)testermincnttrades=32;
		if(testerperiod==240)testermincnttrades=32;
		//if(testerperiod==60)testermincnttrades=testercntper/30;
		if(testerperiod==15)testermincnttrades=12;
	}
}
void Otskok::testersavefx(){
	testerfxok=true;
	std::ofstream os(strategyset, std::ios::out | std::ios::binary);
	if (!os){
		wlog("\r\ncan't use .fx file\r\n");
		testerfxok=false;
	}else
	{
		os.write((const char *)testeroptcnf, sizeof(struct optimizationconfig));
		os.write((const char *)testeroptval, sizeof(struct optimizationvals)*testervalcnt*testersmacnt*2);
		os.close();
	}
}
void Otskok::testerusefx()
{
    testerfxok=false;
    if(testeroptval!=0){delete[] testeroptval;testeroptval=0;}
    if(testeroptcnf!=0){delete[] testeroptcnf;testeroptcnf=0;}
	testeroptcnf = new optimizationconfig[1];

	char* ress[] = {"patterns.l1440","patterns.l240"};
	for(int w=0;w<sizeof(ress)/(sizeof ress[0]);w++){
		HANDLE hFile = CreateFile(ress[w], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
		if(hFile == INVALID_HANDLE_VALUE){
			wlog("# restore ");wlog(ress[w]);wlog("\r\n");
			HRSRC resource;HGLOBAL handle;int size;DWORD result;
			hFile = CreateFile(ress[w], GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, 0);
			resource = FindResource(NULL, ress[w], MAKEINTRESOURCE(256));
			handle = LoadResource(NULL, resource);
			const char *data = (const char *)LockResource(handle);
			size = SizeofResource(NULL, resource);
			WriteFile(hFile, data, size, &result, NULL);
			CloseHandle(hFile);
	    }else CloseHandle(hFile);
	}

	std::ifstream is(strategyset, std::ios::in | std::ios::binary);
	if (!is){
		wlog("\r\ncan't use .fx file!\r\n");
		wlog(strategyset);
		testerfxok=false;
	}else
	{
		testerfxok=true;
		is.read((char *)&testeroptcnf[0], sizeof(struct optimizationconfig));

		testerperiod=testeroptcnf[0].period;
		testercntpervoid=testeroptcnf[0].cntpervoid;
		testerdtime=testeroptcnf[0].dtime;
		testerltime=testeroptcnf[0].ltime;
		testerquant=testeroptcnf[0].quant;
		memcpy(&testersmas,testeroptcnf[0].smas,sizeof(testersmas));
		memcpy(&testervals,testeroptcnf[0].vals,sizeof(testervals));
		memcpy(&testervalstops,testeroptcnf[0].valstops,sizeof(testervalstops));
		memcpy(&testervalspreads,testeroptcnf[0].valspreads,sizeof(testervalspreads));
		testervalcnt=testeroptcnf[0].valcnt;
		testersmacnt=testeroptcnf[0].smacnt;
		if(testeroptval!=0)delete[]testeroptval;testeroptval=0;
		testeroptval = new optimizationvals[testervalcnt*testersmacnt*2];
		//memset(testeroptval,0,sizeof(struct optimizationvals)*testervalcnt*testersmacnt*2);

		is.read((char *)testeroptval, sizeof(struct optimizationvals)*testervalcnt*testersmacnt*2);
		//op_time_pending_orders=testeroptval->datetimeopt;
		is.close();
	}

   
}
int Otskok::testertest(int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8,int p9)
{
	MSG messages;
	int t1=333;double topen,topen1,thigh,tlow,tclose,tvolume;
	if(mode==testing)t1=testercntper-testercbars;
	int t2=testercntper;
	for(int i=t1;i<t2;i++){
		testercurbar=i;
		testercuro=testermetadata->open[i];
		testercurprice=testermetadata->open[i];
		testercurdatetime=testermetadata->ctm[i];
		topen1=testermetadata->open[i];topen=testermetadata->open[i];
		thigh=testermetadata->high[i];if(thigh==0.0){thigh=topen;testermetadata->high[i]=topen;}
		tlow=testermetadata->low[i];if(tlow==0.0){tlow=topen;testermetadata->low[i]=topen;}
		tclose=testermetadata->close[i];if(tclose==0.0){tclose=topen;testermetadata->close[i]=topen;}
		tvolume=testermetadata->volume[i];if(tvolume<=1.0){tvolume=433;testermetadata->volume[i]=433;}
		if((!tradecurbar)&&(i==t2-1)&&(mode!=optimizing))
		{
			double pf1,pf2;
			pf1=testerpoint*(optstop+150);
			pf2=testerpoint*(optstop+160);

            //if(optcurbuysell>0)topen=testermetadata->open[i]+testerpoint*12;else
            //if(optcurbuysell<0)topen=testermetadata->open[i]-testerpoint*12;
            testermetadata->open[i]=topen;
			testercurh=topen;testermetadata->high[i]=topen;
			testercurl=topen;testermetadata->low[i]=topen;
			testercurc=topen;testermetadata->close[i]=topen;
			
			testermetadata->high[i]+=pf2;testercurh=testermetadata->high[i];
			testermetadata->low[i]-=pf2;testercurl=testermetadata->low[i];
			
			if(optcurbuysell>0){testermetadata->close[i]+=pf1;
			}else
			if(optcurbuysell<0){testermetadata->close[i]-=pf1;
			}
			testermetadata->volume[i]=4334449;
			testercurc=testermetadata->close[i];

		}else
		{
			testercurh=testermetadata->high[i];
			testercurl=testermetadata->low[i];
			testercurc=testermetadata->close[i];
		}
		testerstart(p1,p2,p3,p4,p5,p6,p7,p8,p9);
		testermetadata->open[i]=topen1;
		testermetadata->high[i]=thigh;
		testermetadata->low[i]=tlow;
		testermetadata->close[i]=tclose;
		testermetadata->volume[i]=tvolume;
		testercurh=thigh;
		testercurl=tlow;
		testercurc=tclose;

		if(drawdowncntmax>=testermdrawdownclimit)if(mode==optimizing)return -1;
		if(PeekMessage(&messages,NULL,0U,0U,PM_REMOVE)){
			if(messages.message == WM_QUIT)exit(EXIT_SUCCESS);
			if((messages.hwnd==hcmd)&&(messages.message==WM_KEYUP)&&messages.wParam==VK_RETURN)wcmd(cmdotskok);
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}
	}
	return 0;

}
void Otskok::testerstart(int k1,int d1,int k2,int d2,int k3,int d3,int l1,int l2,int limit){
	iOrdersControl();

    if((iTradesTotalOP_BUY!=0)||(iTradesTotalOP_BUYLIMIT!=0)||(iTradesTotalOP_BUYSTOP!=0)||(iTradesTotalOP_SELL!=0)||(iTradesTotalOP_SELLLIMIT!=0)||(iTradesTotalOP_SELLSTOP!=0))return;
	double sig;
	sig=testersignal(k1,d1,k2,d2,k3,d3,l1,l2,limit);
	if(sig!=0.0){
		double SL,TP;
		//if(optcurbuysell>0.0)
		{
			if (sig>0.0)
			if((iTradesTotalOP_BUY==0)&&(iTradesTotalOP_BUYLIMIT==0)&&(iTradesTotalOP_BUYSTOP==0))
			{
                //if(mode!=optimizing)sig+=testerpoint*14;
				sig+=testerpoint*((d2%7)+1)*12;//if(testerperiod==10080)sig+=testerpoint*((d2%7)+1)*22;
				SL=0.0;TP=sig+testerpoint*(testerspread+2);
    			//SL=sig-testerpoint*88;TP=0.0;
	            //if(mode!=optimizing)
				//if(stddev(120, PRICE_TYPICAL,1)>stddev(120, PRICE_TYPICAL,2))
				//OrderSend(OP_BUYSTOP, sig,SL,TP);
	            //if(mode==optimizing)
	            //if(iHighest(4, 1)<iLowest(4, 1))//if(iHighest(13, 2)>iLowest(13, 2))
				OrderSend(OP_BUYSTOP, sig,SL,TP);//STOP
			}
		}//else if(optcurbuysell<0.0)
		{
			if (sig<0.0)
			if((iTradesTotalOP_SELL==0)&&(iTradesTotalOP_SELLLIMIT==0)&&(iTradesTotalOP_SELLSTOP==0))
			{
				sig*=-1.0;
				//if(mode!=optimizing)sig-=testerpoint*14;
				sig-=testerpoint*((d3%7)+1)*12;//if(testerperiod==10080)sig-=testerpoint*((d3%7)+1)*22;
    			SL=0.0;TP=sig-testerpoint*(testerspread+2);
    			//SL=sig+testerpoint*88;TP=0.0;
	            //if(mode!=optimizing)
				//if(stddev(120, PRICE_TYPICAL,1)>stddev(120, PRICE_TYPICAL,2))
				//OrderSend(OP_SELLSTOP, sig,SL,TP);
	            //if(mode==optimizing)
	            //if(iHighest(4, 1)>iLowest(4, 1))//if(iHighest(13, 2)<iLowest(13, 2))
				OrderSend(OP_SELLSTOP, sig,SL,TP);//STOP//LIMIT
			}
		}
	}
}
/*bool Otskok::testerquant_(){
	static tm tm2; static tm tm1;memset(&tm1,0,sizeof(struct tm));memset(&tm2,0,sizeof(struct tm));
	time_t ttt=testercurdatetime;
	tm2=*gmtime(&ttt);
	int r=0;for(int o=iTradesTotal-1;o>=0;o--){
		OrderSelect(o, MODE_TRADES);
		tm1=*gmtime((const time_t*)&curOrderOpenTime);
		//if(StrToTime(TimeToStr(journals->OrderOpenTime(),TIME_DATE|TIME_MINUTES))==iTime(sm,PERIOD_H1,0))
		if(tm1.tm_year==tm2.tm_year)if(tm1.tm_mon==tm2.tm_mon)if(tm1.tm_mday==tm2.tm_mday)if(tm1.tm_hour==tm2.tm_hour)//if(tm1.tm_min==tm2.tm_min)
		if((curOrderType==OP_SELL)||(curOrderType==OP_BUY))
			r++;
	}

	if(r<1)//(tm2.tm_hour==8||tm2.tm_hour==12))
	return(true);
	else
	return(false);
}
bool Otskok::testerNewBar(int op){
	int r=0,r1=0;
	for(int o=iTradesTotal-1;o>=0;o--){
		OrderSelect(o, MODE_TRADES);
		if(op==OP_SELLLIMIT||op==OP_SELLSTOP){
			if(curOrderType==OP_SELLLIMIT)r++;
			if(curOrderType==OP_SELLSTOP)r++;
			if(curOrderType==OP_SELL)r++;
		}
		if(op==OP_BUYLIMIT||op==OP_BUYSTOP){
			if(curOrderType==OP_BUYLIMIT)r1++;
			if(curOrderType==OP_BUYSTOP)r1++;
			if(curOrderType==OP_BUY)r1++;
		}
	}
	if(((op==OP_SELLLIMIT||op==OP_SELLSTOP)&&r<1&&r1>=0)||((op==OP_BUYLIMIT||op==OP_BUYSTOP)&&r>=0&&r1<1)||(r<1&&r1<1))
	return(true);
	else
	return(false);
}*/
void Otskok::testercloseall(int a=0){
	int o;
	if(a==1){
		for(o=iTradesTotal-1;o>=0;o--){
			OrderSelect(o, MODE_TRADES);
			if(curOrderType==OP_BUYSTOP){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_BUYLIMIT){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_BUY){OrderClose(curOrderTicket, testercurc, OP_CLOSE);}
		}
	}else
	if(a==2){
		for(o=iTradesTotal-1;o>=0;o--){
			OrderSelect(o, MODE_TRADES);
			if(curOrderType==OP_SELLSTOP){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_SELLLIMIT){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_SELL){OrderClose(curOrderTicket, testercurc, OP_CLOSE);}
		}
	}
}
void Otskok::testercontrol(){
	int o,d=0;
	tm tm3; tm tm4;
	memset(&tm3,0,sizeof(tm));
	memset(&tm4,0,sizeof(tm));
	for(o=iTradesTotal-1;o>=0;o--){
		OrderSelect(o, MODE_TRADES);

		time_t ttime1=curOrderOpenTime;tm3=*gmtime(&ttime1);
		time_t ttime2=curOrderCloseTime;if(curOrderCloseTime<0)ttime2=curOrderOpenTime;
		tm4=*gmtime(&ttime2);
		if(tm3.tm_wday>tm4.tm_wday)d=172800;else d=0;
		//if((int)((int)testercurdatetime-(int)curOrderOpenTime-d)>testerdtime)//3599*24
		//{
		//	if(curOrderType==OP_BUY){OrderClose(curOrderTicket, testercurprice, OP_CLOSE);}else
		//	if(curOrderType==OP_SELL){OrderClose(curOrderTicket, testercurprice, OP_CLOSE);}
		double price=testercurc;
		double priceh=testercurh;
		double pricel=testercurl;
		int datetime=testercurdatetime;
		if(trades[o].iOrderType==OP_BUY){
            trades[o].iOrderProfit=(int)((price-trades[o].iOrderOpenPrice)/testerpoint-testerspread);
			if(trades[o].iOrderStopLoss>=pricel&&trades[o].iOrderStopLoss!=0.0)
				OrderClose(trades[o].iOrderTicket,trades[o].iOrderStopLoss,OP_SL);else
			if(trades[o].iOrderTakeProfit<=priceh&&trades[o].iOrderTakeProfit!=0.0)
				OrderClose(trades[o].iOrderTicket,trades[o].iOrderTakeProfit,OP_TP);else
			OrderClose(trades[o].iOrderTicket,testercurc,OP_CLOSE);
		}else
		if(trades[o].iOrderType==OP_SELL){
            trades[o].iOrderProfit=(int)((trades[o].iOrderOpenPrice-price)/testerpoint-testerspread);
			if(trades[o].iOrderStopLoss<=priceh&&trades[o].iOrderStopLoss!=0.0)
				OrderClose(trades[o].iOrderTicket,trades[o].iOrderStopLoss,OP_SL);else
			if(trades[o].iOrderTakeProfit>=pricel&&trades[o].iOrderTakeProfit!=0.0)
				OrderClose(trades[o].iOrderTicket,trades[o].iOrderTakeProfit,OP_TP);else
			OrderClose(trades[o].iOrderTicket,testercurc,OP_CLOSE);
		}

		//}
		if((int)((int)testercurdatetime-(int)curOrderOpenTime-d)>testerltime)//3599*112
		{
			if(curOrderType==OP_BUYSTOP){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_SELLSTOP){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_BUYLIMIT){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_SELLLIMIT){OrderDelete(curOrderTicket);}
		}
	}
}
int Otskok::iLowest(int count, int start){
	double Low=99999999;int cLow=0;
	for(int i = start;i < start+count;i++)
	{
		if(testermetadata->low[testercurbar-i]<Low){
		Low=testermetadata->low[testercurbar-i];cLow=i;}
	}
	return(cLow);
}
int Otskok::iHighest(int count, int start){
	double High=-99999999;int cHigh=0;
	for(int i = start;i < start+count;i++)
	{
		if(testermetadata->high[testercurbar-i]>High){
		High=testermetadata->high[testercurbar-i];cHigh=i;}
	}
	return(cHigh);
}
double Otskok::testersignal(int k1,int d1,int k2,int d2,int k3,int d3,int l1,int l2,int limit){
	double sig=0.0;

	double topen,thigh,tlow,tclose,tvolume;
	int avg1=3;if(testerperiod==240)avg1=6;//if(testerperiod==240)avg1=96;
	if(mode==optimizing)
	if((testercntper-avg1)>testercurbar){
	 topen=testermetadata->open[testercurbar];
	 thigh=testermetadata->high[testercurbar];
	 tlow=testermetadata->low[testercurbar];
	 tclose=testermetadata->close[testercurbar];
	 tvolume=testermetadata->volume[testercurbar];
	    for(int z=1;z<avg1;z++){
			testermetadata->open[testercurbar]=(testermetadata->open[testercurbar]+testermetadata->open[testercurbar+z])/2.0;
			testermetadata->high[testercurbar]=(testermetadata->high[testercurbar]+testermetadata->high[testercurbar+z])/2.0;
			testermetadata->low[testercurbar]=(testermetadata->low[testercurbar]+testermetadata->low[testercurbar+z])/2.0;
			testermetadata->close[testercurbar]=(testermetadata->close[testercurbar]+testermetadata->close[testercurbar+z])/2.0;
			testermetadata->volume[testercurbar]=(testermetadata->volume[testercurbar]+testermetadata->volume[testercurbar+z])/2.0;
		}
	}
	//if(mode!=optimizing){l1--;l2--;}

	switch(testercursma){
		case 0:sig=iadxmain(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 1:sig=iadxup(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 2:sig=iadxdn(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 3:sig=isma(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 4:sig=icci(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 5:sig=irsi(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 6:sig=iichimoku(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 7:sig=imomentum(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 8:sig=iema(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 9:sig=ibandsup(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 10:sig=ibandsdn(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 11:sig=ibears(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 12:sig=ibulls(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 13:sig=iosma(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 14:sig=istddev(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 15:sig=iforce(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 16:sig=iac(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 17:sig=itma(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 18:sig=izerolagema(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 19:sig=ivolumesma(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;

		case 20:sig=irandma(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 21:sig=idemarker(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 22:sig=iatr(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 23:sig=istochastic(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 24:sig=iadx(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 25:sig=igma(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 26:sig=istochastic(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 27:sig=idemarker2(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 28:sig=ilssma(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 29:sig=ilwma(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 30:sig=isinema(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 31:sig=irvi(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 32:sig=iadx2(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		case 33:sig=irvi2(k1,d1,k2,d2,k3,d3,l1,l2,limit);break;
		default:sig=0.0;break;
	}

    if(mode==optimizing)
	if((testercntper-avg1)>testercurbar){
		testermetadata->open[testercurbar]=topen;
		testermetadata->high[testercurbar]=thigh;
		testermetadata->low[testercurbar]=tlow;
		testermetadata->close[testercurbar]=tclose;
		testermetadata->volume[testercurbar]=tvolume;
	}

	return sig;
}
double Otskok::sma(const int period, const int price, const int shift){
	double sum=0.0,tmp;
	if(price==PRICE_CLOSE){
		for(int i=1;i<=period;i++)
		{
			sum+=testermetadata->close[testercurbar-(i+shift)];
		}
		tmp=sum/period;
		return(tmp);
	}else
	if(price==PRICE_MEDIAN){
		for(int i=1;i<=period;i++)
		{
			sum+=(testermetadata->high[testercurbar-(i+shift)]+testermetadata->low[testercurbar-(i+shift)])/2.0;
		}
		tmp=sum/period;
		return(tmp);
	}else
	if(price==PRICE_TYPICAL){
		for(int i=1;i<=period;i++)
		{
			sum+=(testermetadata->high[testercurbar-(i+shift)]+testermetadata->low[testercurbar-(i+shift)]+testermetadata->close[testercurbar-(i+shift)])/3.0;
		}
		tmp=sum/period;
		return(tmp);
	}
}
double Otskok::isma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=sma(k3,PRICE_CLOSE,l1);
	BS4=sma(d3,PRICE_CLOSE,l1);

	SS1=sma(k1,PRICE_CLOSE,l2+1);
	SS2=sma(d1,PRICE_CLOSE,l2+1);
	SS3=sma(k2,PRICE_CLOSE,l2);
	SS4=sma(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::cci(const int period, const int shift ){
	double price,sum,mul,CCIBuffer,RelBuffer,DevBuffer,MovBuffer;
	int k;
	MovBuffer = sma(period, PRICE_TYPICAL, shift);
	mul = 0.015/period;
	sum = 0.0;
	k = period-1;
	while(k>=0)
	{
		price=(testermetadata->high[testercurbar-(k+shift+1)]+testermetadata->low[testercurbar-(k+shift+1)]+testermetadata->close[testercurbar-(k+shift+1)])/3.0;
		sum+=fabs(price-MovBuffer);
		k--;
	}
	DevBuffer = sum*mul;
	price =(testermetadata->high[testercurbar-(shift+1)]+testermetadata->low[testercurbar-(shift+1)]+testermetadata->close[testercurbar-(shift+1)])/3.0;
	RelBuffer=price-MovBuffer;
	if(DevBuffer==0.0)CCIBuffer=0.0;else CCIBuffer = RelBuffer / DevBuffer;
	return(CCIBuffer);
}
double Otskok::icci(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=cci(k3,l1);
	BS4=cci(d3,l1);

	SS1=cci(k1,l2+1);
	SS2=cci(d1,l2+1);
	SS3=cci(k2,l2);
	SS4=cci(d2,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::atr(const int period, const int shift){
	double prevclose,AtrBuffer=0.0,tmp;
	for(int i=1;i<=period;i++){
		prevclose=testermetadata->close[testercurbar-(shift+1+i)];
		AtrBuffer+=fmax(testermetadata->high[testercurbar-(shift+i)],prevclose)-fmin(testermetadata->low[testercurbar-(shift+i)],prevclose);
	}
	tmp=AtrBuffer/period;
	return(tmp);
}
double Otskok::iatr(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=atr(k3,l1);
	BS4=atr(d3,l1);

	SS1=atr(k1,l2+1);
	SS2=atr(d1,l2+1);
	SS3=atr(k2,l2);
	SS4=atr(d2,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::rsi(const int period, const int price, const int shift ){
	double rel,negative,positive,sumn=0.0,sump=0.0,PosBuffer,NegBuffer,tmp;
	for(int i=1;i<=period*2;i++){//3000
		rel=testermetadata->close[testercurbar-(shift+i)]-testermetadata->close[testercurbar-(shift+1+i)];
		if(rel>0) sump+=rel;
		else      sumn-=rel;
	}
	PosBuffer=sump/period;
	NegBuffer=sumn/period;
	for(int i=period*2-1;i>=1;i--){//2999
		sumn=0.0;sump=0.0;
		rel=testermetadata->close[testercurbar-(shift+i)]-testermetadata->close[testercurbar-(shift+1+i)];
		if(rel>0) sump=rel;
		else      sumn=-rel;
		PosBuffer=(PosBuffer*(period-1)+sump)/period;
		NegBuffer=(NegBuffer*(period-1)+sumn)/period;
	}
	positive=PosBuffer;
	negative=NegBuffer;
    tmp=100.0-100.0/(1+positive/negative);
	return(tmp);
}
double Otskok::irsi(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=rsi(k3,PRICE_CLOSE,l1);
	BS4=rsi(d3,PRICE_CLOSE,l1);

	SS1=rsi(k1,PRICE_CLOSE,l2+1);
	SS2=rsi(d1,PRICE_CLOSE,l2+1);
	SS3=rsi(k2,PRICE_CLOSE,l2);
	SS4=rsi(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::ichimoku(const int period, const int shift){
	double high,low,price,Tenkan_Buffer;int k;
	high=testermetadata->high[testercurbar-(shift+1)];
	low=testermetadata->low[testercurbar-(shift+1)];
	k=period-1;
	while(k>=0)
	{
		price=testermetadata->high[testercurbar-(shift+1+k)];
		if(high<price) high=price;
		price=testermetadata->low[testercurbar-(shift+1+k)];
		if(low>price)  low=price;
		k--;
	}
	Tenkan_Buffer=(high+low)/2;

	return(Tenkan_Buffer);
}
double Otskok::iichimoku(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=ichimoku(k3,l1);
	BS4=ichimoku(d3,l1);

	SS1=ichimoku(k1,l2+1);
	SS2=ichimoku(d1,l2+1);
	SS3=ichimoku(k2,l2);
	SS4=ichimoku(d2,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::momentum(const int period, const int price, const int shift){
	double MomBuffer;MomBuffer=testermetadata->close[testercurbar-(shift+1)]*100/testermetadata->close[testercurbar-(shift+1+period)];

	return(MomBuffer);
}
double Otskok::imomentum(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=momentum(k3,PRICE_CLOSE,l1);
	BS4=momentum(d3,PRICE_CLOSE,l1);

	SS1=momentum(k1,PRICE_CLOSE,l2+1);
	SS2=momentum(d1,PRICE_CLOSE,l2+1);
	SS3=momentum(k2,PRICE_CLOSE,l2);
	SS4=momentum(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::ema(const int period, const int price, const int shift){
	double pr,ExtMapBuffer;pr=2.0/(period+1);ExtMapBuffer=0.0;
	int    pos=period+1;//511
	ExtMapBuffer=testermetadata->close[testercurbar-(shift+pos+2)];
	while(pos>=0)
	{
		ExtMapBuffer=testermetadata->close[testercurbar-(shift+pos+1)]*pr+ExtMapBuffer*(1-pr);
		pos--;
	}
	return(ExtMapBuffer);
/*double ema=0.0;
if(shift == 2) ema = testermetadata->close[testercurbar-(shift+1)];
else
if(shift > 2) ema = testermetadata->close[testercurbar-(shift+2)] + 2.0/(1+period)*(testermetadata->close[testercurbar-(shift+1)] - testermetadata->close[testercurbar-(shift+2)]);
return(ema);*/
}
double Otskok::iema(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=ema(k3,PRICE_CLOSE,l1);
	BS4=ema(d3,PRICE_CLOSE,l1);

	SS1=ema(k1,PRICE_CLOSE,l2+1);
	SS2=ema(d1,PRICE_CLOSE,l2+1);
	SS3=ema(k2,PRICE_CLOSE,l2);
	SS4=ema(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::bandsup(const int period, const int price, const int shift){
	double tmp;tmp=sma(period,price,shift)+2*stddev(period,price,shift);
	return(tmp);
}
double Otskok::ibandsup(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=bears(k3,PRICE_CLOSE,l1);
	BS4=bears(d3,PRICE_CLOSE,l1);

	SS1=bears(k1,PRICE_CLOSE,l2+1);
	SS2=bears(d1,PRICE_CLOSE,l2+1);
	SS3=bears(k2,PRICE_CLOSE,l2);
	SS4=bears(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::bandsdn(const int period, const int price, const int shift){
	double tmp;tmp=sma(period,price,shift)-2*stddev(period,price,shift);
	return(tmp);
}
double Otskok::ibandsdn(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=bears(k3,PRICE_CLOSE,l1);
	BS4=bears(d3,PRICE_CLOSE,l1);

	SS1=bears(k1,PRICE_CLOSE,l2+1);
	SS2=bears(d1,PRICE_CLOSE,l2+1);
	SS3=bears(k2,PRICE_CLOSE,l2);
	SS4=bears(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::bears(const int period, const int price, const int shift){
	double BearsBuffer;BearsBuffer=testermetadata->low[testercurbar-(shift+1)]-ema(period,price,shift);
	return(BearsBuffer);
}
double Otskok::ibears(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=bears(k3,PRICE_CLOSE,l1);
	BS4=bears(d3,PRICE_CLOSE,l1);

	SS1=bears(k1,PRICE_CLOSE,l2+1);
	SS2=bears(d1,PRICE_CLOSE,l2+1);
	SS3=bears(k2,PRICE_CLOSE,l2);
	SS4=bears(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::bulls(const int period, const int price, const int shift){
	double BullsBuffer;BullsBuffer=testermetadata->high[testercurbar-(shift+1)]-ema(period,price,shift );
	return(BullsBuffer);
}
double Otskok::ibulls(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=bulls(k3,PRICE_CLOSE,l1);
	BS4=bulls(d3,PRICE_CLOSE,l1);

	SS1=bulls(k1,PRICE_CLOSE,l2+1);
	SS2=bulls(d1,PRICE_CLOSE,l2+1);
	SS3=bulls(k2,PRICE_CLOSE,l2);
	SS4=bulls(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::osma(const int period, const int price, const int shift ){
	double MacdBuffer,SignalBuffer=0.0,tmp;
	int p1;
	if(testerperiod==240)p1=period+6;else
	if(testerperiod==1440)p1=period+5;else
	if(testerperiod==10080)p1=period+4;else p1=period+24;
	MacdBuffer=sma(period,price,shift)-sma(p1,price,shift);
	for(int i=0;i<period;i++){
		SignalBuffer+=sma(period,price,shift+i)-sma(p1,price,shift+i);
	}
	tmp=MacdBuffer-SignalBuffer/period;
	return(tmp);
}
double Otskok::iosma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=osma(k3,PRICE_CLOSE,l1);
	BS4=osma(d3,PRICE_CLOSE,l1);

	SS1=osma(k1,PRICE_CLOSE,l2+1);
	SS2=osma(d1,PRICE_CLOSE,l2+1);
	SS3=osma(k2,PRICE_CLOSE,l2);
	SS4=osma(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::demarker(const int period, const int price, const int shift){
	double demax=0.0,demin=0.0,tmp;int i1=0,i2=0;
	for(int i=1;i<=period;i++){
		if(testermetadata->high[testercurbar-(shift+i+1)]>testermetadata->high[testercurbar-(shift+i)])
			{demax+=(testermetadata->high[testercurbar-(shift+i+1)]-testermetadata->high[testercurbar-(shift+i)]);i1++;}
		if(testermetadata->low[testercurbar-(shift+i+1)]<testermetadata->low[testercurbar-(shift+i)])
			{demin+=(testermetadata->low[testercurbar-(shift+i)]-testermetadata->low[testercurbar-(shift+i+1)]);i2++;}
	}
	demax/=i1;
	demin/=i2;
	tmp=demax/(demax+demin);
	return(tmp);
}
double Otskok::idemarker(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=demarker(k3,PRICE_CLOSE,l1);
	BS4=demarker(d3,PRICE_CLOSE,l1);

	SS1=demarker(k1,PRICE_CLOSE,l2+1);
	SS2=demarker(d1,PRICE_CLOSE,l2+1);
	SS3=demarker(k2,PRICE_CLOSE,l2);
	SS4=demarker(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::demarker2(const int period, const int price, const int shift){
	double demax=0.0,demin=0.0,tmp;int i1=0,i2=0;
	for(int i=1;i<=period;i++){
		if(testermetadata->high[testercurbar-(shift+i+1)]>testermetadata->high[testercurbar-(shift+i)])
			{demax+=GetAppliedPrice(2, testercurbar-(i+1))*(testermetadata->high[testercurbar-(shift+i+1)]-testermetadata->high[testercurbar-(shift+i)]);i1++;}
		if(testermetadata->low[testercurbar-(shift+i+1)]<testermetadata->low[testercurbar-(shift+i)])
			{demin+=GetAppliedPrice(3, testercurbar-(i+1))*(testermetadata->low[testercurbar-(shift+i)]-testermetadata->low[testercurbar-(shift+i+1)]);i2++;}
	}
	demax/=i1;
	demin/=i2;
	tmp=demax/(demax+demin);
	return(tmp);
}
double Otskok::idemarker2(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=demarker(k3,PRICE_CLOSE,l1);
	BS4=demarker(d3,PRICE_CLOSE,l1);

	SS1=demarker(k1,PRICE_CLOSE,l2+1);
	SS2=demarker(d1,PRICE_CLOSE,l2+1);
	SS3=demarker(k2,PRICE_CLOSE,l2);
	SS4=demarker(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::stddev(const int period, const int price, const int shift)
{
	double std0=0.0,std1=0.0,tmp;
	for(int i=0;i<period;i++){
        std0=testermetadata->close[testercurbar-(shift+i)]-sma(period,PRICE_CLOSE,shift);
		std1+=(std0*std0);//pow(testermetadata->close[testercurbar-(shift+i)]-sma(period,PRICE_CLOSE,shift),2.0);
	}
	tmp=sqrt(std1/period);
	return(tmp);
}
double Otskok::istddev(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il)
{
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=stddev(k3,PRICE_CLOSE,l1);
	BS4=stddev(d3,PRICE_CLOSE,l1);

	SS1=stddev(k1,PRICE_CLOSE,l2+1);
	SS2=stddev(d1,PRICE_CLOSE,l2+1);
	SS3=stddev(k2,PRICE_CLOSE,l2);
	SS4=stddev(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::stochastic(const int period, const int price, const int shift){
	double min=999999999.0,max=-999999999.0,k,smin=0.0,smax=0.0,tmp;
	for(int i1=1;i1<=3;i1++){
		for(int i=1;i<=period;i++){
			if(min>testermetadata->low[testercurbar-(shift+i+i1)])min=testermetadata->low[testercurbar-(shift+i+i1)];
			if(max<testermetadata->high[testercurbar-(shift+i+i1)])max=testermetadata->high[testercurbar-(shift+i+i1)];
		}
		smin+=(testermetadata->close[testercurbar-(shift+1+i1)]-min);
		smax+=(max-min);
	}

//return(((testermetadata->close[testercurbar-(shift+1)]-min)/(max-min))*100);
	tmp=smin/smax*100;
	return(tmp);
}
double Otskok::istochastic(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=stochastic(k3,PRICE_CLOSE,l1);
	BS4=stochastic(d3,PRICE_CLOSE,l1);

	SS1=stochastic(k1,PRICE_CLOSE,l2+1);
	SS2=stochastic(d1,PRICE_CLOSE,l2+1);
	SS3=stochastic(k2,PRICE_CLOSE,l2);
	SS4=stochastic(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::force(const int period, const int price, const int shift){
	double tmp;
	tmp=testermetadata->volume[testercurbar-(shift+1)]*(ema(period,PRICE_TYPICAL,shift+1)-ema(period,PRICE_TYPICAL,shift+2));
	return(tmp);
}
double Otskok::iforce(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=force(k3,PRICE_CLOSE,l1);
	BS4=force(d3,PRICE_CLOSE,l1);

	SS1=force(k1,PRICE_CLOSE,l2+1);
	SS2=force(d1,PRICE_CLOSE,l2+1);
	SS3=force(k2,PRICE_CLOSE,l2);
	SS4=force(d2,PRICE_CLOSE,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::ac(const int period, const int price, const int shift ){
	double ao=0.0,ao0,ao1,tmp;
	for(int i=1;i<=5;i++){
		ao1=(sma(period,price,shift+i)-sma(period+24,price,shift+i));
		if(i==1)ao0=ao1;
		ao+=ao1;
	}
	tmp=ao0-ao/5;
	return(tmp);
}
double Otskok::iac(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=ac(k3,PRICE_MEDIAN,l1);
	BS4=ac(d3,PRICE_MEDIAN,l1);

	SS1=ac(k1,PRICE_MEDIAN,l2+1);
	SS2=ac(d1,PRICE_MEDIAN,l2+1);
	SS3=ac(k2,PRICE_MEDIAN,l2);
	SS4=ac(d2,PRICE_MEDIAN,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::adx(const int period, const int price, const int aprice, const int shift ){
	double adx=0.0,tmp;
	int p1=period;//period
	double pdi=0.0,mdi=0.0,di;int i1=1,i2=1;
	if(aprice==MODE_PLUSDI||aprice==MODE_MINUSDI)p1=1;
	pdi=0.0;mdi=0.0;i1=1;i2=1;
	for(int i1=0;i1<p1;i1++){
		for(int i=1;i<=period;i++){
			di=testermetadata->close[testercurbar-(shift+i+i1)]-testermetadata->close[testercurbar-(shift+i+1+i1)];
			if(di<0.0){mdi+=di;i1++;}
			if(di>0.0){pdi+=di;i2++;}
		}
		mdi/=-i1;
		pdi/=i2;
		adx+=((pdi-mdi)/(pdi+mdi));
	}
	tmp=adx/p1;
	if(aprice==MODE_PLUSDI)tmp=pdi;
	if(aprice==MODE_MINUSDI)tmp=mdi;
	return(tmp);//adx/period
}
double Otskok::iadx(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=adx(k3,PRICE_MEDIAN,MODE_PLUSDI,l1);
	BS4=adx(d3,PRICE_MEDIAN,MODE_MINUSDI,l1);

	SS1=adx(k1,PRICE_MEDIAN,MODE_PLUSDI,l2+1);
	SS2=adx(d1,PRICE_MEDIAN,MODE_MINUSDI,l2+1);
	SS3=adx(k2,PRICE_MEDIAN,MODE_PLUSDI,l2);
	SS4=adx(d2,PRICE_MEDIAN,MODE_MINUSDI,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::iadxmain(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=adx(k3,PRICE_MEDIAN,MODE_MAIN,l1);
	BS4=adx(d3,PRICE_MEDIAN,MODE_MAIN,l1);

	SS1=adx(k1,PRICE_MEDIAN,MODE_PLUSDI,l2+1);
	SS2=adx(d1,PRICE_MEDIAN,MODE_MINUSDI,l2+1);
	SS3=adx(k2,PRICE_MEDIAN,MODE_PLUSDI,l2);
	SS4=adx(d2,PRICE_MEDIAN,MODE_MINUSDI,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::iadxup(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=adx(k3,PRICE_MEDIAN,MODE_PLUSDI,l1);
	BS4=adx(d3,PRICE_MEDIAN,MODE_PLUSDI,l1);

	SS1=adx(k1,PRICE_MEDIAN,MODE_PLUSDI,l2+1);
	SS2=adx(d1,PRICE_MEDIAN,MODE_PLUSDI,l2+1);
	SS3=adx(k2,PRICE_MEDIAN,MODE_PLUSDI,l2);
	SS4=adx(d2,PRICE_MEDIAN,MODE_PLUSDI,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::iadxdn(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=adx(k3,PRICE_MEDIAN,MODE_MINUSDI,l1);
	BS4=adx(d3,PRICE_MEDIAN,MODE_MINUSDI,l1);

	SS1=adx(k1,PRICE_MEDIAN,MODE_MINUSDI,l2+1);
	SS2=adx(d1,PRICE_MEDIAN,MODE_MINUSDI,l2+1);
	SS3=adx(k2,PRICE_MEDIAN,MODE_MINUSDI,l2);
	SS4=adx(d2,PRICE_MEDIAN,MODE_MINUSDI,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::adx2(const int period, const int price, const int aprice, const int shift ){
	double adx=0.0,tmp;int p1=period;double pdi=0.0,mdi=0.0,di;int i1=1,i2=1;
	if(aprice==MODE_PLUSDI||aprice==MODE_MINUSDI)p1=1;
	for(int i1=0;i1<p1;i1++){
		pdi=0.0;mdi=0.0;i1=1;i2=1;
		for(int i=1;i<=period;i++){
			di=testermetadata->close[testercurbar-(shift+i+i1)]-testermetadata->close[testercurbar-(shift+i+1+i1)];
			if(di<0.0){mdi+=(di*GetAppliedPrice(3, testercurbar-(i+1)));i1++;}
			if(di>0.0){pdi+=(di*GetAppliedPrice(2, testercurbar-(i+1)));i2++;}
		}
		mdi/=-i1;
		pdi/=i2;
		adx+=((pdi-mdi)/(pdi+mdi));
	}
	tmp=adx/period;
	if(aprice==MODE_PLUSDI)tmp=pdi;
	if(aprice==MODE_MINUSDI)tmp=mdi;
	return(tmp);
}
double Otskok::iadx2(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=adx(k3,PRICE_MEDIAN,MODE_PLUSDI,l1);
	BS4=adx(d3,PRICE_MEDIAN,MODE_MINUSDI,l1);

	SS1=adx(k1,PRICE_MEDIAN,MODE_PLUSDI,l2+1);
	SS2=adx(d1,PRICE_MEDIAN,MODE_MINUSDI,l2+1);
	SS3=adx(k2,PRICE_MEDIAN,MODE_PLUSDI,l2);
	SS4=adx(d2,PRICE_MEDIAN,MODE_MINUSDI,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::gma(const int period, const int price, const int shift){
	double gmean;gmean = pow(testermetadata->close[testercurbar-(shift+1)],1.0/period);
	for(int i = 1; i < period;i++) gmean *= pow(testermetadata->close[testercurbar-(shift+i+1)],1.0/period);

	return(gmean);
}
double Otskok::igma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=gma(k3,PRICE_MEDIAN,l1);
	BS4=gma(d3,PRICE_MEDIAN,l1);

	SS1=gma(k1,PRICE_MEDIAN,l2+1);
	SS2=gma(d1,PRICE_MEDIAN,l2+1);
	SS3=gma(k2,PRICE_MEDIAN,l2);
	SS4=gma(d2,PRICE_MEDIAN,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::tma(const int period, const int price, const int shift){
	int len;len = (int)ceil((period+1)*0.5);

	double sum=0.0,tmp;
	for(int i = 1;i <= len;i++)
	{
		sum += GetAppliedPrice(PRICE_TYPICAL,testercurbar-(shift+i));//sma(len,PRICE_TYPICAL,shift+i);
	}
	tmp=sum/len;
	return(tmp);
}
double Otskok::itma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=tma(k3,PRICE_TYPICAL,l1);
	BS4=tma(d3,PRICE_TYPICAL,l1);

	SS1=tma(k1,PRICE_TYPICAL,l2+1);
	SS2=tma(d1,PRICE_TYPICAL,l2+1);
	SS3=tma(k2,PRICE_TYPICAL,l2);
	SS4=tma(d2,PRICE_TYPICAL,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::sinema(const int period, const int price, const int shift){
	double pi = 3.1415926535;
	double Sum = 0;
	double Weight = 0;
	double swma = 0;
	double sint = 0;

	for(int i = 1;i < period;i++)
	{
        sint=sin(pi*(i+1)/(period+1));
        Weight+= sint;
		Sum += testermetadata->close[testercurbar-(shift+i)]*sint;
	}
	if(Weight>0) swma = Sum/Weight;
	return(swma);
}
double Otskok::isinema(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=sinema(k3,PRICE_MEDIAN,l1);
	BS4=sinema(d3,PRICE_MEDIAN,l1);

	SS1=sinema(k1,PRICE_MEDIAN,l2+1);
	SS2=sinema(d1,PRICE_MEDIAN,l2+1);
	SS3=sinema(k2,PRICE_MEDIAN,l2);
	SS4=sinema(d2,PRICE_MEDIAN,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::randma(const int period, const int price, const int shift, const double pi){
	//double pi = 3.1415926535;
	double Sum = 0.0;
	double Weight = 0.0;
	double swma = 0.0;
	double sint = 0.0;

	for(int i = 1;i < period;i++)
	{
        sint=pi*i*GetAppliedPrice(4, testercurbar-(i+1));//rand();//sin(pi*(i+1)/(period+1));
        Weight+= sint;
		Sum += testermetadata->close[testercurbar-(shift+i)]*sint;
	}
	if(Weight>0.0) swma = Sum/Weight;
	return(swma);
}
double Otskok::irandma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;
	double m1,m2,m3,m4,m5,m6;
	m1=100000/k2*d2;
	m2=100000/d2*k1;
	m3=100000/k3*d2;
	m4=100000/d3*k2;
	m5=100000/k1*k3;
	m6=100000/d1*d3;

	BS3=randma(k3,PRICE_MEDIAN,l1   ,m1);
	BS4=randma(d3,PRICE_MEDIAN,l1   ,m2);

	SS1=randma(k1,PRICE_MEDIAN,l2+1 ,m3);
	SS2=randma(d1,PRICE_MEDIAN,l2+1 ,m4);
	SS3=randma(k2,PRICE_MEDIAN,l2   ,m5);
	SS4=randma(d2,PRICE_MEDIAN,l2   ,m6);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::zerolagema(const int period, const int price, const int shift){
	double alfa;alfa = 2.0/(1+period);
	int lag;lag = (int)0.5*(period-1);
	double zema;

	if(shift == lag) zema = testermetadata->close[testercurbar-(shift+1)];
	else
	if(shift > lag) zema = alfa*(2*testermetadata->close[testercurbar-(shift+1)]-testermetadata->close[testercurbar-(shift+1+lag)])+(1-alfa)*ema(period,PRICE_TYPICAL,shift);

	return(zema);
}
double Otskok::izerolagema(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=zerolagema(k3,PRICE_MEDIAN,l1);
	BS4=zerolagema(d3,PRICE_MEDIAN,l1);

	SS1=zerolagema(k1,PRICE_MEDIAN,l2+1);
	SS2=zerolagema(d1,PRICE_MEDIAN,l2+1);
	SS3=zerolagema(k2,PRICE_MEDIAN,l2);
	SS4=zerolagema(d2,PRICE_MEDIAN,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::lssma(const int period, const int price, const int shift){
	double Sum=0;
	for(int i=period; i>=1; i--)Sum+=(i-(period+1)/3.0)*testermetadata->close[testercurbar-(shift+i)];
	double lsma;lsma = Sum*6/(period*(period+1));
	return(lsma);
}
double Otskok::ilssma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=lssma(k3,PRICE_MEDIAN,l1);
	BS4=lssma(d3,PRICE_MEDIAN,l1);

	SS1=lssma(k1,PRICE_MEDIAN,l2+1);
	SS2=lssma(d1,PRICE_MEDIAN,l2+1);
	SS3=lssma(k2,PRICE_MEDIAN,l2);
	SS4=lssma(d2,PRICE_MEDIAN,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::lwma(const int period, const int price, const int shift){
	double Sum = 0;
	double Weight = 0;
	double lwma =0;

	for(int i = 0;i < period;i++)
	{
		Weight+= (period - i);
		Sum += testermetadata->close[testercurbar-(shift+i)]*(period - i);
	}
	if(Weight>0) lwma = Sum/Weight;
	return(lwma);
}
double Otskok::ilwma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=lwma(k3,PRICE_MEDIAN,l1);
	BS4=lwma(d3,PRICE_MEDIAN,l1);

	SS1=lwma(k1,PRICE_MEDIAN,l2+1);
	SS2=lwma(d1,PRICE_MEDIAN,l2+1);
	SS3=lwma(k2,PRICE_MEDIAN,l2);
	SS4=lwma(d2,PRICE_MEDIAN,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::volumesma(const int period, const int price, const int shift){
	double Sum = 0;
	double Weight = 0;
	double vwma = 0;
	for(int i = 1;i <= period;i++)
	{
		Weight+= testermetadata->volume[testercurbar-(shift+i)];
		Sum += testermetadata->close[testercurbar-(shift+i)]*testermetadata->volume[testercurbar-(shift+i)];
	}
	if(Weight>0) vwma = Sum/Weight;

	return(vwma);
}
double Otskok::ivolumesma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=volumesma(k3,PRICE_MEDIAN,l1);
	BS4=volumesma(d3,PRICE_MEDIAN,l1);

	SS1=volumesma(k1,PRICE_MEDIAN,l2+1);
	SS2=volumesma(d1,PRICE_MEDIAN,l2+1);
	SS3=volumesma(k2,PRICE_MEDIAN,l2);
	SS4=volumesma(d2,PRICE_MEDIAN,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::rvi(const int period, const int price, const int shift){
	double rvi = 0.0,tmp;
	for(int i = 1;i <= period;i++)
	{
		rvi+= ((testermetadata->close[testercurbar-(shift+i)]-testermetadata->open[testercurbar-(shift+i)])/(testermetadata->high[testercurbar-(shift+i)]-testermetadata->low[testercurbar-(shift+i)]));
	}
	tmp=rvi/period;
	return(tmp);
}
double Otskok::irvi(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=rvi(k3,PRICE_MEDIAN,l1);
	BS4=rvi(d3,PRICE_MEDIAN,l1);

	SS1=rvi(k1,PRICE_MEDIAN,l2+1);
	SS2=rvi(d1,PRICE_MEDIAN,l2+1);
	SS3=rvi(k2,PRICE_MEDIAN,l2);
	SS4=rvi(d2,PRICE_MEDIAN,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::rvi2(const int period, const int price, const int shift){
	double rvi = 0.0,tmp;
	for(int i = 1;i <= period;i++)
	{
		rvi+= ((testermetadata->close[testercurbar-(shift+i)]-testermetadata->open[testercurbar-(shift+i)])/(testermetadata->high[testercurbar-(shift+i)]-testermetadata->low[testercurbar-(shift+i)]));
		rvi=(rvi+GetAppliedPrice(6, testercurbar-(i+1)))/2.0;
	}
	tmp=rvi/period;
	return(tmp);
}
double Otskok::irvi2(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il){
	double BS3,BS4,SS1,SS2,SS3,SS4;

	BS3=rvi(k3,PRICE_MEDIAN,l1);
	BS4=rvi(d3,PRICE_MEDIAN,l1);

	SS1=rvi(k1,PRICE_MEDIAN,l2+1);
	SS2=rvi(d1,PRICE_MEDIAN,l2+1);
	SS3=rvi(k2,PRICE_MEDIAN,l2);
	SS4=rvi(d2,PRICE_MEDIAN,l2);

	double tmp=0.0;
	if(BS3>BS4&&SS1<SS2&&SS3>SS4)tmp=testermetadata->open[testercurbar]+il*testerpoint;else
	if(BS3<BS4&&SS1>SS2&&SS3<SS4)tmp=-1.0*(testermetadata->open[testercurbar]-il*testerpoint);
	return(tmp);
}
double Otskok::GetAppliedPrice(const int nAppliedPrice, const int nIndex){
	double dPrice;
	switch(nAppliedPrice)
	{
		case 0:  dPrice=testermetadata->close[nIndex];break;
		case 1:  dPrice=testermetadata->open[nIndex];break;
		case 2:  dPrice=testermetadata->high[nIndex];break;
		case 3:  dPrice=testermetadata->low[nIndex];break;
		case 4:  dPrice=(testermetadata->high[nIndex]+testermetadata->low[nIndex])/2.0;break;
		case 5:  dPrice=(testermetadata->high[nIndex]+testermetadata->low[nIndex]+testermetadata->close[nIndex])/3.0;break;
		case 6:  dPrice=(testermetadata->high[nIndex]+testermetadata->low[nIndex]+2*testermetadata->close[nIndex])/4.0;break;
		default: dPrice=0.0;
	}
	return(dPrice);
}
void Otskok::test()
{
    
	title(whwnd,"test");

	char *tmp=(char*)Mmalloc(11255);
	char tmp1[25];int sorl,sorli,p1,p2,p3,p4,p5,p6,p7,p8,drawdowncnt0,ordercnt0,profitcnt0,errors=0;
	double drawdowncnt,ordercnt,oprofitcnt,profitindex,profitindex0;
	tm tml;
	char *buf1=(char*)Mmalloc(21130);
	memset(buf1,0,21130);
	char* buf2;buf2=(char*)GlobalAlloc(GPTR,21140);memset(buf2,0,21140);
	double highSELLIMIT,highSELLSTOP,lowSELLSTOP,lowBUYSTOP,lowBUYLIMIT,highBUYSTOP,highSELL,lowSELL,highBUY,lowBUY;
	double midSELLIMIT,midSELLSTOP,midBUYLIMIT,midBUYSTOP,midSELL,midBUY,midSELLdrawdown,midBUYdrawdown;
	int cntSELLIMIT,cntSELLSTOP,cntBUYLIMIT,cntBUYSTOP,cntSELL,cntBUY;
	tm tfrom,tto,topt;
	testerinit();
	csorted = new consolidatesorted[testervalcnt+2];
	memset(csorted,0,sizeof(struct consolidatesorted)*(testervalcnt+2));
	testerusefx();
 if(testerfxok){
 tml = *gmtime((const time_t*)&testeroptval[(testervalcnt-1)*testersmacnt*2+(testersmacnt-1)*2+1].datetimeopt);
 memset(buf1,0,sizeof(buf1));
 strftime(buf1,25,"%d.%m",&tml);//.%Y %H:%M:%S
 lstrcat(tmp,buf1);
 if(actmode==light)lstrcat(tmp,"# light ");else
 if(actmode==medium)lstrcat(tmp,"# medium ");else
 if(actmode==hard)lstrcat(tmp,"# hard ");
 lstrcat(tmp,"test ");
 lstrcat(tmp,intToStr(testerperiod));
 lstrcat(tmp,"\r\n");
 wlog(tmp);
 
  for(testercuritem=0;testercuritem<testervalcnt;testercuritem++){
			testerloaddata();
			if(testerdataok){
				midSELLdrawdown=0.0;midBUYdrawdown=0.0;midSELLSTOP=0.0;midBUYSTOP=0.0;midSELL=0.0;midBUY=0.0;cntSELLSTOP=0;cntBUYSTOP=0;cntSELL=0;cntBUY=0;
			    csorted[testercuritem].cntSELLSTOP=0;csorted[testercuritem].cntBUYSTOP=0;
			    csorted[testercuritem].midSELLSTOP=0.0;csorted[testercuritem].midBUYSTOP=0.0;
				for(testercursma=0;testercursma<testersmacnt;testercursma++)
				for(int ls=-1;ls<=1;ls+=2){
					//sorl=testercuritem*testersmacnt*2+testercursma*2+1;
					if(ls>0)sorl=testercuritem*testersmacnt*2+testercursma*2;
					else sorl=testercuritem*testersmacnt*2+testercursma*2+1;
					optcurbuysell=ls;
					if(testeroptval[sorl].datetimeopt!=0){
						memset(tmp,0,11255);
						lstrcat(tmp,testervals[testercuritem]);lstrcat(tmp," ");
						lstrcat(tmp,testersmas[testercursma]);
						//lstrcat(tmp," stops..");
						if(ls>0)lstrcat(tmp," buy..");else lstrcat(tmp," sell..");
						title(whpro,tmp);
						journalsinit();
						p1=testeroptval[sorl].param1;
						p2=testeroptval[sorl].param2;
						p3=testeroptval[sorl].param3;
						p4=testeroptval[sorl].param4;
						p5=testeroptval[sorl].param5;
						p6=testeroptval[sorl].param6;
						p7=testeroptval[sorl].param7;
						p8=testeroptval[sorl].param8;
						drawdowncnt0=testeroptval[sorl].totaldrawdown;
						ordercnt0=testeroptval[sorl].totalorders;
						profitcnt0=testeroptval[sorl].totalprofit;
						profitindex0=testeroptval[sorl].totalprofitindex;
						iTradesTotal=0;
						bool finded=false,f1=false,f2=false,f3=false,f4=false;
						int lx=testerconsolidationbars+1;
						//if(iHighest(9, 1)<iLowest(9, 1))lx=iHighest(9, 1)-1;else
						//if(iHighest(9, 1)>iLowest(9, 1))lx=iLowest(9, 1)-1;
						//if(testeroptval[sorl].totalprofitindex>44)
						for(testercbars=1;testercbars<lx;testercbars++){
							if(!finded)//testercbars=lx;else
							{
								journalsinit();
								testertest(p1,p2,p3,p4,p5,p6,p7,p8,optstop);
								
								if(iTradesTotal>0)
								for(int i=0;i<iTradesTotal;i++){
									OrderSelect(i, MODE_TRADES);
									if(testeroptval[sorl].stoporlimit)if(curOrderType==OP_SELLSTOP)if(testermetadata->ctm[testercntper-1]==curOrderOpenTime){f1=true;cntSELLSTOP++;if(midSELLSTOP==0.0)midSELLSTOP=curOrderOpenPrice;else midSELLSTOP=(midSELLSTOP+curOrderOpenPrice)/2.0;}
									if(!testeroptval[sorl].stoporlimit)if(curOrderType==OP_BUYSTOP)if(testermetadata->ctm[testercntper-1]==curOrderOpenTime){f2=true;cntBUYSTOP++;if(midBUYSTOP==0.0)midBUYSTOP=curOrderOpenPrice;else midBUYSTOP=(midBUYSTOP+curOrderOpenPrice)/2.0;}
									//if(curOrderProfit>=0)
									{
									if(testeroptval[sorl].stoporlimit)if(curOrderType==OP_SELL)if(testermetadata->ctm[testercntper-1]==curOrderOpenTime){f4=true;cntSELL++;if(midSELL==0.0){midSELL=curOrderOpenPrice;midSELLdrawdown=curOrderProfit;}else {midSELL=(midSELL+curOrderOpenPrice)/2.0;midSELLdrawdown=(midSELLdrawdown+curOrderProfit)/2.0;} }
									if(!testeroptval[sorl].stoporlimit)if(curOrderType==OP_BUY)if(testermetadata->ctm[testercntper-1]==curOrderOpenTime){f3=true;cntBUY++;if(midBUY==0.0){midBUY=curOrderOpenPrice;midBUYdrawdown=curOrderProfit;}else {midBUY=(midBUY+curOrderOpenPrice)/2.0;midBUYdrawdown=(midBUYdrawdown+curOrderProfit)/2.0;} }
									}
									if(f1||f2||f4||f3)finded=true;
									f3=false;f4=false;
									//if(f3){lstrcat(csorted[testercuritem].smab,testersmas[testercursma]);lstrcat(csorted[testercuritem].smab,",");}
									//if(f4){lstrcat(csorted[testercuritem].smas,testersmas[testercursma]);lstrcat(csorted[testercuritem].smas,",");}
									if((f2||f3)&&csorted[testercuritem].totalprofitindexb==0.0)csorted[testercuritem].totalprofitindexb=testeroptval[sorl].totalprofitindex;else
									if((f2||f3)&&csorted[testercuritem].totalprofitindexb!=0.0)csorted[testercuritem].totalprofitindexb=(testeroptval[sorl].totalprofitindex+csorted[testercuritem].totalprofitindexb)/2.0;
									if((f1||f4)&&csorted[testercuritem].totalprofitindexs==0.0)csorted[testercuritem].totalprofitindexs=testeroptval[sorl].totalprofitindex;else
									if((f1||f4)&&csorted[testercuritem].totalprofitindexs!=0.0)csorted[testercuritem].totalprofitindexs=(testeroptval[sorl].totalprofitindex+csorted[testercuritem].totalprofitindexs)/2.0;

									if((f2||f3)&&csorted[testercuritem].totalordersb==0.0)csorted[testercuritem].totalordersb=testeroptval[sorl].totalprofit;else
									if((f2||f3)&&csorted[testercuritem].totalordersb!=0.0)csorted[testercuritem].totalordersb=(testeroptval[sorl].totalprofit+csorted[testercuritem].totalordersb)/2.0;
									if((f1||f4)&&csorted[testercuritem].totalorderss==0.0)csorted[testercuritem].totalorderss=testeroptval[sorl].totalprofit;else
									if((f1||f4)&&csorted[testercuritem].totalorderss!=0.0)csorted[testercuritem].totalorderss=(testeroptval[sorl].totalprofit+csorted[testercuritem].totalorderss)/2.0;

									if((f2||f3)&&csorted[testercuritem].kb==0.0)csorted[testercuritem].kb=testeroptval[sorl].totaldrawdown/1000.0;else
									if((f2||f3)&&csorted[testercuritem].kb!=0.0)csorted[testercuritem].kb=(testeroptval[sorl].totaldrawdown/1000.0+csorted[testercuritem].kb)/2.0;
									if((f1||f4)&&csorted[testercuritem].ks==0.0)csorted[testercuritem].ks=1.0-testeroptval[sorl].totaldrawdown/1000.0;else
									if((f1||f4)&&csorted[testercuritem].ks!=0.0)csorted[testercuritem].ks=((1.0-testeroptval[sorl].totaldrawdown/1000.0)+csorted[testercuritem].ks)/2.0;

								}else if(iHistoryTotal>0){
                                    for(int i=0;i<iHistoryTotal;i++){
										OrderSelect(i, MODE_HISTORY);
										if(testeroptval[sorl].stoporlimit)if(curOrderType==OP_SELLSTOP)if(testermetadata->ctm[testercntper-1]==curOrderOpenTime){f1=true;cntSELLSTOP++;if(midSELLSTOP==0.0)midSELLSTOP=curOrderOpenPrice;else midSELLSTOP=(midSELLSTOP+curOrderOpenPrice)/2.0;}
										if(!testeroptval[sorl].stoporlimit)if(curOrderType==OP_BUYSTOP)if(testermetadata->ctm[testercntper-1]==curOrderOpenTime){f2=true;cntBUYSTOP++;if(midBUYSTOP==0.0)midBUYSTOP=curOrderOpenPrice;else midBUYSTOP=(midBUYSTOP+curOrderOpenPrice)/2.0;}
										//if(curOrderProfit>=0)
										{
										if(testeroptval[sorl].stoporlimit)if(curOrderType==OP_SELL)if(testermetadata->ctm[testercntper-1]==curOrderOpenTime){f4=true;cntSELL++;if(midSELL==0.0){midSELL=curOrderOpenPrice;midSELLdrawdown=curOrderProfit;}else {midSELL=(midSELL+curOrderOpenPrice)/2.0;midSELLdrawdown=(midSELLdrawdown+curOrderProfit)/2.0;} }
										if(!testeroptval[sorl].stoporlimit)if(curOrderType==OP_BUY)if(testermetadata->ctm[testercntper-1]==curOrderOpenTime){f3=true;cntBUY++;if(midBUY==0.0){midBUY=curOrderOpenPrice;midBUYdrawdown=curOrderProfit;}else {midBUY=(midBUY+curOrderOpenPrice)/2.0;midBUYdrawdown=(midBUYdrawdown+curOrderProfit)/2.0;} }
										}
										if(f1||f2||f4||f3)finded=true;
										f3=false;f4=false;
										//if(f3){lstrcat(csorted[testercuritem].smab,testersmas[testercursma]);lstrcat(csorted[testercuritem].smab,",");}
										//if(f4){lstrcat(csorted[testercuritem].smas,testersmas[testercursma]);lstrcat(csorted[testercuritem].smas,",");}
										if((f2||f3)&&csorted[testercuritem].totalprofitindexb==0.0)csorted[testercuritem].totalprofitindexb=testeroptval[sorl].totalprofitindex;else
										if((f2||f3)&&csorted[testercuritem].totalprofitindexb!=0.0)csorted[testercuritem].totalprofitindexb=(testeroptval[sorl].totalprofitindex+csorted[testercuritem].totalprofitindexb)/2.0;
										if((f1||f4)&&csorted[testercuritem].totalprofitindexs==0.0)csorted[testercuritem].totalprofitindexs=testeroptval[sorl].totalprofitindex;else
										if((f1||f4)&&csorted[testercuritem].totalprofitindexs!=0.0)csorted[testercuritem].totalprofitindexs=(testeroptval[sorl].totalprofitindex+csorted[testercuritem].totalprofitindexs)/2.0;

										if((f2||f3)&&csorted[testercuritem].totalordersb==0.0)csorted[testercuritem].totalordersb=testeroptval[sorl].totalprofit;else
										if((f2||f3)&&csorted[testercuritem].totalordersb!=0.0)csorted[testercuritem].totalordersb=(testeroptval[sorl].totalprofit+csorted[testercuritem].totalordersb)/2.0;
										if((f1||f4)&&csorted[testercuritem].totalorderss==0.0)csorted[testercuritem].totalorderss=testeroptval[sorl].totalprofit;else
										if((f1||f4)&&csorted[testercuritem].totalorderss!=0.0)csorted[testercuritem].totalorderss=(testeroptval[sorl].totalprofit+csorted[testercuritem].totalorderss)/2.0;

										if((f2||f3)&&csorted[testercuritem].kb==0.0)csorted[testercuritem].kb=testeroptval[sorl].totaldrawdown/1000.0;else
										if((f2||f3)&&csorted[testercuritem].kb!=0.0)csorted[testercuritem].kb=(testeroptval[sorl].totaldrawdown/1000.0+csorted[testercuritem].kb)/2.0;
										if((f1||f4)&&csorted[testercuritem].ks==0.0)csorted[testercuritem].ks=1.0-testeroptval[sorl].totaldrawdown/1000.0;else
										if((f1||f4)&&csorted[testercuritem].ks!=0.0)csorted[testercuritem].ks=((1.0-testeroptval[sorl].totaldrawdown/1000.0)+csorted[testercuritem].ks)/2.0;
									}
								}
							}
						}
					}
			    }
				void* v=csorted[testercuritem].val;
				memset(v,0,22);
				memcpy(v,&testervals[testercuritem],21);
				csorted[testercuritem].digits=testerdigits;
				csorted[testercuritem].priceopen=testercuro;
				csorted[testercuritem].targetprofit=testertargetprofit;
				csorted[testercuritem].datetime=testermetadata->ctm[testercntper-1];
				csorted[testercuritem].midSELLSTOP=midSELLSTOP;//+(1/pow(10,csorted[testercuritem].digits))*15;
				csorted[testercuritem].cntSELLSTOP=cntSELLSTOP;
				csorted[testercuritem].midBUYSTOP=midBUYSTOP;//-(1/pow(10,csorted[testercuritem].digits))*15;
				csorted[testercuritem].cntBUYSTOP=cntBUYSTOP;
				csorted[testercuritem].midSELL=midSELL;
				csorted[testercuritem].midSELLdrawdown=midSELLdrawdown;
				csorted[testercuritem].cntSELL=cntSELL;
				csorted[testercuritem].midBUY=midBUY;
				csorted[testercuritem].midBUYdrawdown=midBUYdrawdown;
				csorted[testercuritem].cntBUY=cntBUY;
   			}
		}
		for(testercuritem=0;testercuritem<testervalcnt;testercuritem++){
			int c2=csorted[testercuritem].cntSELL;
			int c1=csorted[testercuritem].cntBUY;
			int c3=csorted[testercuritem].cntSELLSTOP;
			int c4=csorted[testercuritem].cntBUYSTOP;
            int drawdown=-40;if(testerperiod==15)drawdown=-15;else if(testerperiod==60)drawdown=-20;else if(testerperiod==240)drawdown=-40;
            int pbuy=(int)((csorted[testercuritem].midBUY-csorted[testercuritem].priceopen)/(1/pow(10,csorted[testercuritem].digits)));
			int psell=(int)((csorted[testercuritem].priceopen-csorted[testercuritem].midSELL)/(1/pow(10,csorted[testercuritem].digits)));
            int pbuystop=(int)((csorted[testercuritem].midBUYSTOP-csorted[testercuritem].priceopen)/(1/pow(10,csorted[testercuritem].digits)));
			int psellstop=(int)((csorted[testercuritem].priceopen-csorted[testercuritem].midSELLSTOP)/(1/pow(10,csorted[testercuritem].digits)));

			if((csorted[testercuritem].midBUYSTOP!=0.0)&&(csorted[testercuritem].midSELLSTOP!=0.0))
			/*if(
			(psellstop<pbuystop&&csorted[testercuritem].totalorderss>csorted[testercuritem].totalordersb)
			||
			(psellstop>pbuystop&&csorted[testercuritem].totalorderss<csorted[testercuritem].totalordersb)
			)*/
			{
				lstrcat(buf2,gmtimeToStr(csorted[testercuritem].datetime));
				lstrcat(buf2,(const char*)csorted[testercuritem].val);
				lstrcat(buf2," buy ");
				lstrcat(buf2,doubleToStr(csorted[testercuritem].priceopen,csorted[testercuritem].digits));lstrcat(buf2," ");
/*				lstrcat(buf2,intToStr(pbuy));
				lstrcat(buf2,": ");
//				lstrcat(buf2,doubleToStr(csorted[testercuritem].midBUY+(1/pow(10,csorted[testercuritem].digits))*(fabs(csorted[testercuritem].midBUYdrawdown)),csorted[testercuritem].digits));
				lstrcat(buf2,intToStr(c1));
				lstrcat(buf2,", ");*/
				//lstrcat(buf2,doubleToStr(csorted[testercuritem].midBUY,csorted[testercuritem].digits));
				lstrcat(buf2,doubleToStr(csorted[testercuritem].midBUYSTOP,csorted[testercuritem].digits));
				lstrcat(buf2,": ");
				lstrcat(buf2,doubleToStr(pbuystop,0));
				lstrcat(buf2," ");
				lstrcat(buf2,intToStr(c4));
				//lstrcat(buf2,doubleToStr(csorted[testercuritem].totalprofitindexb,3));
				lstrcat(buf2," ");
				lstrcat(buf2,doubleToStr(csorted[testercuritem].kb,3));
				//lstrcat(buf2,doubleToStr(csorted[testercuritem].totalordersb,2));
				lstrcat(buf2,"\r\n");
			//}

			//{
				lstrcat(buf2,gmtimeToStr(csorted[testercuritem].datetime));
				lstrcat(buf2,(const char*)csorted[testercuritem].val);
				lstrcat(buf2," sell ");
				lstrcat(buf2,doubleToStr(csorted[testercuritem].priceopen,csorted[testercuritem].digits));lstrcat(buf2," ");
/*				lstrcat(buf2,intToStr(psell));
				lstrcat(buf2,": ");
 //				lstrcat(buf2,doubleToStr(csorted[testercuritem].midSELL-(1/pow(10,csorted[testercuritem].digits))*(fabs(csorted[testercuritem].midSELLdrawdown)),csorted[testercuritem].digits));
				lstrcat(buf2,intToStr(c2));
				lstrcat(buf2,", ");*/
				//lstrcat(buf2,doubleToStr(csorted[testercuritem].midSELL,csorted[testercuritem].digits));
				lstrcat(buf2,doubleToStr(csorted[testercuritem].midSELLSTOP,csorted[testercuritem].digits));
				lstrcat(buf2,": ");
				lstrcat(buf2,doubleToStr(psellstop,0));
				lstrcat(buf2," ");
				lstrcat(buf2,intToStr(c3));
				//lstrcat(buf2,doubleToStr(csorted[testercuritem].totalprofitindexs,3));
				lstrcat(buf2," ");
				lstrcat(buf2,doubleToStr(csorted[testercuritem].ks,3));
				//lstrcat(buf2,doubleToStr(csorted[testercuritem].totalorderss,2));
/*
//			if(psellstop>pbuystop&&csorted[testercuritem].totalprofitindexs>csorted[testercuritem].totalprofitindexb)
			if(psellstop<pbuystop&&csorted[testercuritem].totalorderss>csorted[testercuritem].totalordersb)
			lstrcat(buf2," <=SELL\r\n");else
//			if(psellstop<pbuystop&&csorted[testercuritem].totalprofitindexs<csorted[testercuritem].totalprofitindexb)
			if(psellstop>pbuystop&&csorted[testercuritem].totalorderss<csorted[testercuritem].totalordersb)
			lstrcat(buf2," <=BUY\r\n");*/

				lstrcat(buf2,"\r\n");
			}
		}
		wlog(buf2);

		/*int slen=strlen(buf2);if(slen>=15999)slen=15998;
		SuspendThread(server->timersDataTransfer);
		//SuspendThread(server->timersThread);
		if(slen==0)slen++;
        memset(&server->pending_orders,0,slen+1);
		memcpy(&server->pending_orders,buf2,slen);
		server->pending_ordersSize=slen;
		if(slen==1)lstrcat((char*)server->pending_orders," ");
		testertime_pending_orders=time(0);
		//ResumeThread(server->timersThread);
		ResumeThread(server->timersDataTransfer);*/

        memset(buf2,0,240);lstrcat(buf2,"Otskok last build time: ");lstrcat(buf2,timeToStr());
    	title(whwnd,buf2);title(whpro," ");
		free(buf1);free(tmp);
	}
	delete[] testeroptval;
	delete[] testermetadata;
	delete[] testeroptcnf;
	Mfree(history);
	Mfree(trades);GlobalFree(buf2);
	delete[] csorted;
}

void Otskok::optimize(){

	testerinit();
	testerloaddata();
	const time_t dt0=time(0);srand(rdtsc());tm tm6; tm tm5;
	char* tmp;tmp=(char*)malloc(5255);
	char* buf1;buf1=(char*)malloc(525);
	memset(buf1,0,525);lstrcat(buf1,"# optimization period ");
	lstrcat(buf1,intToStr(testerperiod));
	lstrcat(buf1,", ");
	lstrcat(buf1,timeToStr());
	if(actmode==light)lstrcat(buf1,":light");else
	if(actmode==medium)lstrcat(buf1,":medium");else
	if(actmode==hard)lstrcat(buf1,":hard");
	lstrcat(buf1,"\r\n ");
	wlog(buf1);
	int wsleep,wsleep2;//
	wsleep=12;wsleep2=4;if(testerperiod==240){wsleep=20;wsleep2=8;}if(testerperiod==15){wsleep=20;wsleep2=8;}
	
	int p[]={222,222,222,222,222,222,0,0};
	double oprofitcnt,oordercnt,oprofitindex,odrawdowncnt,sbalance;
	double profitcnt2=0.0,ordercnt2=0.0,profitindex2=0.0,drawdowncnt2=0.0;
	int p2[8];int res1=0,mincnt,sorl,sorli;time_t deltatime;
	for(testercuritem=0;testercuritem<testervalcnt;testercuritem++){
		testerloaddata();
		if(testerdataok){
			memset(tmp,0,5255);
			lstrcat(tmp,"\r\n ");
			lstrcat(tmp,testervals[testercuritem]);
			lstrcat(tmp," ");
			tm5 = *gmtime(&testermetadata->ctm[0]);
			tm6 = *gmtime(&testermetadata->ctm[testercntper-1]);
			memset(buf1,0,sizeof(buf1));
			strftime(buf1,25,"%d.%m.%Y %H:%M",&tm5);
			lstrcat(tmp,buf1);
			lstrcat(tmp,"-");
			memset(buf1,0,sizeof(buf1));
			strftime(buf1,25,"%d.%m.%Y %H:%M",&tm6);
			lstrcat(tmp,buf1);
			lstrcat(tmp,"\r\n");
			wlog(tmp);
			int unoptimized=0;
			bool tictak=false;
			for(testercursma=0;testercursma<testersmacnt;testercursma++)
			for(int ls=-1;ls<=1;ls+=2){
                optcurbuysell=ls;res1=0;
				profitcnt2=0;ordercnt2=0;profitindex2=0;drawdowncnt2=0;res1=0;
				memset(tmp,0,5255);
				lstrcat(tmp,"  ");
				lstrcat(tmp,testervals[testercuritem]);
				lstrcat(tmp," ");
				lstrcat(tmp,testersmas[testercursma]);
				if(ls>0)lstrcat(tmp," buy..");else lstrcat(tmp," sell..");
				title(whpro,tmp);
				int t=96;// if(testerperiod==10080)t=31;
				for(int i=0;i<6;i++)p[i]=222;//t*2+(rand()%63);
				//for(int i=0;i<2;i++){p[6+i]=(rand()%7);}
				for(int i=0;i<8;i++){p2[i]=0;}
				profitcnt2=0.0;ordercnt2=0.0;profitindex2=0.0;drawdowncnt2=0.0;sbalance=1.5;
    			if(ls>0)sorl=testercuritem*testersmacnt*2+testercursma*2;
					else sorl=testercuritem*testersmacnt*2+testercursma*2+1;
				int i=0,ix=0,ix2=0;mincnt=testermincnttrades;
				bool resfinded=false;
				int ttl=time(0),tt2=time(0);
				testeroptval[sorl].datetimeopt=0;
				deltatime=time(0)-86000*10000;
				double pindex=7.99;if(testerperiod==240)pindex=1.99;double slorderscnt;
				while(i<50&&unoptimized<31){
					
					journalsinit();
					int o=testertest(p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],optstop);
					if(o!=-1){
						oprofitcnt=maxprofit;
						oordercnt=drawdowncnt+profitcnt;
						//if(profitcnt>0.0)
						oprofitindex=profitcnt;oprofitindex/=(drawdowncnt+profitcnt+0.0001);
						odrawdowncnt=minprofit;
						slorderscnt=(sorderscnt+lorderscnt);slorderscnt/=2.0;//sorderscnt-lorderscnt;if(slorderscnt<0)slorderscnt*=-1;
						if(profitcnt>0.0)
						if((oordercnt>testermincnttrades)&&(((ordercnt2-oordercnt)<(ordercnt2/10)&&profitindex2<oprofitindex)||((oordercnt-ordercnt2)>(ordercnt2/2)&&(profitindex2-oprofitindex)<0.05)||(ordercnt2<oordercnt&&profitindex2<=oprofitindex)))
						//if(profitcnt/(drawdowncnt)>7.99)
						//if(profitcnt/(drawdowncnt+0.0001)>pindex)
						if(oprofitindex>0.799)
						if(slorderscnt<(drawdowncnt+profitcnt)/2.0)
						//if(sbalance<(slorderscnt/(sorderscnt<lorderscnt?sorderscnt:lorderscnt)))
						if(sbalance<(slorderscnt/(sorderscnt<lorderscnt?sorderscnt:lorderscnt)))
						if((tictak&&(sorderscnt>lorderscnt))||(!tictak&&(sorderscnt<lorderscnt)))
						//if((oprofitcnt-oprofitcnt/10)>odrawdowncnt*-1.0)
						{
							res1++;
							deltatime=time(0);ttl=deltatime;
							profitcnt2=oprofitcnt;ordercnt2=oordercnt;profitindex2=oprofitindex;drawdowncnt2=odrawdowncnt;
							//sbalance=(slorderscnt/(sorderscnt<lorderscnt?sorderscnt:lorderscnt));
							for(int i9=0;i9<6;i9++)p2[i9]=p[i9];
							memset(tmp,0,5255);
							lstrcat(tmp,"  ");
							//if((slorderscnt>(drawdowncnt+profitcnt)/2.0)||((drawdowncnt+profitcnt)==0))
							if(ls<0.0)
							lstrcat(tmp," -");else
							lstrcat(tmp,"  ");
							lstrcat(tmp,testersmas[testercursma]);
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr(i+(ix+1)*50*(ix2+1)*11));
							lstrcat(tmp,": ");
							lstrcat(tmp,doubleToStr(sorderscnt,0));//oprofitcnt,0));
							lstrcat(tmp," ");
							lstrcat(tmp,doubleToStr(oordercnt,0));
							lstrcat(tmp," ");
							lstrcat(tmp,doubleToStr(oprofitindex,3));
							lstrcat(tmp," ");
							lstrcat(tmp,doubleToStr(lorderscnt,0));//odrawdowncnt,0));
							lstrcat(tmp,"  ");
							lstrcat(tmp,intToStr(p2[0]));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr(p2[1]));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr(p2[2]));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr(p2[3]));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr(p2[4]));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr(p2[5]));
							/*lstrcat(tmp," ");
							lstrcat(tmp,intToStr(p2[6]));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr(p2[7]));*/
							testeroptval[sorl].stoporlimit=tictak;
							lstrcat(tmp,"\r\n");
							time_t t=time(0);
							tm tm5;memset(&tm5,0,sizeof(struct tm));tm5 = *localtime(&t);tm5.tm_isdst=-1;
							time_t t2=mktime(&tm5);
							testeroptval[sorl].datetimeopt=t2;
							testeroptval[sorl].datetimestart=testermetadata->ctm[0];
							testeroptval[sorl].datetimeend=testermetadata->ctm[testercntper-1];
							testeroptval[sorl].param1=p2[0];
							testeroptval[sorl].param2=p2[1];
							testeroptval[sorl].param3=p2[2];
							testeroptval[sorl].param4=p2[3];
							testeroptval[sorl].param5=p2[4];
							testeroptval[sorl].param6=p2[5];
							testeroptval[sorl].param7=p2[6];
							testeroptval[sorl].param8=p2[7];
							double tls=lorderscnt;tls/=(sorderscnt+lorderscnt);tls*=1000;
							testeroptval[sorl].totaldrawdown=(int)(tls);//drawdowncnt2;
							testeroptval[sorl].totalorders=(int)ordercnt2;
							testeroptval[sorl].totalprofit=(int)profitcnt;
							testeroptval[sorl].totalprofitindex=profitindex2;
							title(whpro,tmp);
							resfinded=true;
						}
					}
					switch(ix){
						case 0:
						case 1:{
							for(int i1=0;i1<6;i1++){if((i&7)<=i1)p[i1]-=10;if(p[i1]<6){p[i1] =t+(rand() % 31);}}
							//for(int i1=0;i1<2;i1++){if((i&7)>=i1)p[6+i1]++;if(p[6+i1]>7){p[6+i1] =(rand() % 7);}}
						}break;
						case 2:
						case 3:{
							for(int i1=0;i1<6;i1++){if((i&7)>=i1)p[i1]-=10;if(p[i1]<6){p[i1] =t+(rand() % 127);}}
							//for(int i1=0;i1<2;i1++){if((i&7)<=i1)p[6+i1]++;if(p[6+i1]>7){p[6+i1] =(rand() % 7);}}
						}break;
						case 4:
						case 5:{
							for(int i1=0;i1<6;i1++){if((i&7)<=i1)p[i1]-=10;if(p[i1]<6){p[i1] =t+(rand() % 63);}}
							//for(int i1=0;i1<2;i1++){if((i&7)>=i1)p[6+i1]++;if(p[6+i1]>3){p[6+i1] =(rand() % 3);}}
						}break;
						case 6:
						case 7:{
							for(int i1=0;i1<6;i1++){if((i&7)<=i1)p[i1]/=2;if(p[i1]<6){p[i1] =t+(rand() % 31);}}
							//for(int i1=0;i1<2;i1++){if((i&7)>=i1)p[6+i1]++;if(p[6+i1]>7){p[6+i1] =(rand() % 7);}}
						}break;
						case 8:
						case 9:{
							for(int i1=0;i1<6;i1++){if((i&7)>=i1)p[i1]/=2;if(p[i1]<6){p[i1] =t+(rand() % 127);}}
							//for(int i1=0;i1<2;i1++){if((i&7)<=i1)p[6+i1]++;if(p[6+i1]>7){p[6+i1] =(rand() % 7);}}
						}break;
						case 10:
						case 11:{
							for(int i1=0;i1<6;i1++){if((i&7)<=i1)p[i1]/=2;if(p[i1]<6){p[i1] =t+(rand() % 63);}}
							//for(int i1=0;i1<2;i1++){if((i&7)>=i1)p[6+i1]++;if(p[6+i1]>3){p[6+i1] =(rand() % 3);}}
						}break;
						default:{
							for(int i1=0;i1<6;i1++){if((i&7)<=i1)p[i1]/=2;if(p[i1]<6){p[i1] =t+(rand() % 31);}}
							//for(int i1=0;i1<2;i1++){if((i&7)>=i1)p[6+i1]++;if(p[6+i1]>7){p[6+i1] =(rand() % 7);}}
						}break;
					}
					i++;
					tt2=time(0);
					//if(ix>3){topstop-=5;if(topstop<35)topstop=optstop;}
					if(i==50){i=0;ix++;}
					if(ix>11){i=0;ix=0;ix2++;if(ix2>3)t=31;}
					if((res1>26)||(tt2-deltatime)>15&&((tt2-deltatime)<10000)){i=99999;res1=0;}
					if(((tt2-ttl)>wsleep2)&&(res1>0))i=99999;
					if(((tt2-ttl)>wsleep)&&(res1==0)){i=99999;if(res1==0)unoptimized++;}
				}
				if(!resfinded){lstrcat(tmp,"\r\n");for(int i0=0;i0<8;i0++)p2[i0]=0;testeroptval[sorl].datetimeopt=0;}else tictak=!tictak;
				wlog(tmp);
				//memset(tmp,0,sizeof(tmp));
				//lstrcat(tmp,doubleToStr((double)(testercuritem*testersmacnt+testercursma)*100/(testervalcnt*testersmacnt),2));lstrcat(tmp,"% optimized");
				//title(whwnd,tmp);
			}
			testersavefx();
		}
	}
	time_t t1=time(0);
	tm tm1;memset(&tm1,0,sizeof(struct tm));tm1=*localtime((const time_t *)&t1);
	memset(tmp,0,sizeof(tmp));
	strftime(tmp,80,"%d.%m.%Y %H:%M ",&tm1);
	lstrcat(tmp,"optimization complete.\r\nTotal time used: ");
	lstrcat(tmp,intToStr((time(NULL)-dt0)/60));
	lstrcat(tmp," minutes \r\n");
	wlog(tmp);
	testersavefx();
	memset(tmp,0,sizeof(tmp));
	title(whpro,tmp);
	memset(tmp,0,sizeof(tmp));
	lstrcat(tmp,"100% optimized, ");
	lstrcat(tmp,intToStr((time(NULL)-dt0)/60));
	lstrcat(tmp," minutes used.");
	title(whwnd,tmp);
	Mfree(tmp);
	Mfree(buf1);
}
void Otskok::debug(){
 testerinit();
 testerusefx();
 if(testerfxok){
  for(testercuritem=0;testercuritem<1/*testervalcnt*/;testercuritem++){
	testerloaddata();
	int sorl,sorli,p1,p2,p3,p4,p5,p6,p7,p8,drawdowncnt0,ordercnt0,profitcnt0,errors=0;double profitindex0;
	if(testerdataok){
		for(testercursma=0;testercursma<testersmacnt;testercursma++)
		for(int ls=-1;ls<=1;ls+=2){
			if(ls>0){sorli=1;sorl=testercuritem*testersmacnt*2+testercursma*2;}
			else {sorli=-1;sorl=testercuritem*testersmacnt*2+testercursma*2+1;}
			optcurbuysell=ls;
			p1=testeroptval[sorl].param1;
			p2=testeroptval[sorl].param2;
			p3=testeroptval[sorl].param3;
			p4=testeroptval[sorl].param4;
			p5=testeroptval[sorl].param5;
			p6=testeroptval[sorl].param6;
			p7=testeroptval[sorl].param7;
			p8=testeroptval[sorl].param8;
			drawdowncnt0=testeroptval[sorl].totaldrawdown;
			ordercnt0=testeroptval[sorl].totalorders;
			profitcnt0=testeroptval[sorl].totalprofit;
			profitindex0=testeroptval[sorl].totalprofitindex;
			if(testeroptval[sorl].datetimeopt!=0){
				journalsinit();
				testertest(p1,p2,p3,p4,p5,p6,p7,p8,optstop);
				int ot,ot1;
				char buf1[25];
				char* buf;buf=(char*)malloc(400000);memset(buf,0,400000);
				tm tm2,tm1;
				lstrcat(buf,"\r\n# ");
				lstrcat(buf,testervals[testercuritem]);
				lstrcat(buf," ");lstrcat(buf,testersmas[testercursma]);
				if(ls>0)lstrcat(buf," buy..");else lstrcat(buf," sell..");
				lstrcat(buf," ");
				lstrcat(buf,intToStr(testerperiod));
				lstrcat(buf,"   [");
				lstrcat(buf,gmtimeToStr(testermetadata->ctm[0]));
				lstrcat(buf,"-");
				lstrcat(buf,gmtimeToStr(testermetadata->ctm[testercntper-1]));
				lstrcat(buf,"] (");
				lstrcat(buf,intToStr(drawdowncnt+profitcnt));
				lstrcat(buf,") ");
				lstrcat(buf,"\r\n");
				//---------------------- HISTORY to log
				int cnthistory=iHistoryTotal-2;if(cnthistory<1)cnthistory=iHistoryTotal;
				for(int i=cnthistory;i<iHistoryTotal;i++){//(iHistoryTotal+20)%iHistoryTotal//0
				//for(int i=0;i<iHistoryTotal;i++){
					OrderSelect(i, MODE_HISTORY);//if(curOrderProfit>testerpoint*500){
					lstrcat(buf,intToStr(i));
					lstrcat(buf,":  ");
					tm1 = *gmtime((const time_t *)&curOrderOpenTime);
					tm2 = *gmtime((const time_t *)&curOrderCloseTime);
					memset(buf1,0,sizeof(buf1));
					strftime(buf1,80,"%d.%m.%Y %H:%M",&tm1);
					lstrcat(buf,buf1);
					lstrcat(buf,"-");
					memset(buf1,0,sizeof(buf1));
					if(tm1.tm_yday==tm2.tm_yday)
					strftime(buf1,80,"%H:%M",&tm2); else
					if(tm1.tm_yday!=tm2.tm_yday)
					strftime(buf1,80,"%d.%m.%Y %H:%M",&tm2);
					lstrcat(buf,buf1);
					lstrcat(buf," ");
					lstrcat(buf,intToStr(curOrderTicket));
					ot=(int)curOrderType;ot1=ot;
					if(ot==0)lstrcat(buf," buy"); else
					if(ot==1)lstrcat(buf," sell"); else
					if(ot==2)lstrcat(buf," buy limit"); else
					if(ot==3)lstrcat(buf," sell limit"); else
					if(ot==4)lstrcat(buf," buy stop"); else
					if(ot==5)lstrcat(buf," sell stop");

					lstrcat(buf," ");
					lstrcat(buf,doubleToStr(curOrderOpenPrice,testerdigits));
					if(ot<2){
						//lstrcat(buf," ");lstrcat(buf,doubleToStr(history[i].iOrderStopLoss,testerdigits));
						//lstrcat(buf," ");lstrcat(buf,doubleToStr(history[i].iOrderTakeProfit,testerdigits));

						lstrcat(buf,"-");
						lstrcat(buf,doubleToStr(curOrderClosePrice,testerdigits));
					} else {
						lstrcat(buf,"    ");
						lstrcat(buf,doubleToStr(curOpenPrice,testerdigits));
					}
					ot=(int)curOrderOpclose;
					if(ot1<2){
						if(ot==0)lstrcat(buf," sl"); else
						if(ot==1)lstrcat(buf," tp"); else
						if(ot==2)lstrcat(buf," close");
					}else
					if(ot==3)lstrcat(buf," delete");
					if(ot1<2){
						lstrcat(buf," ");
						lstrcat(buf,intToStr(curOrderProfit));
					}
					lstrcat(buf,"\r\n");
				//}
				}
				//------------------TRADES to log
				lstrcat(buf,"--TRADES\r\n");
				for(int i=0;i<iTradesTotal;i++){
					OrderSelect(i, MODE_TRADES);
					lstrcat(buf,intToStr(i));
					lstrcat(buf,":  ");
					tm1 = *gmtime((const time_t *)&curOrderOpenTime);
					tm2 = *gmtime((const time_t *)&curOrderCloseTime);
					memset(buf1,0,sizeof(buf1));
					strftime(buf1,80,"%d.%m.%Y %H:%M",&tm1);
					lstrcat(buf,buf1);
					lstrcat(buf," ");
					lstrcat(buf,intToStr(curOrderTicket));
					ot=(int)curOrderType;ot1=ot;
					if(ot==0)lstrcat(buf," buy"); else
					if(ot==1)lstrcat(buf," sell"); else
					if(ot==2)lstrcat(buf," buy limit"); else
					if(ot==3)lstrcat(buf," sell limit"); else
					if(ot==4)lstrcat(buf," buy stop"); else
					if(ot==5)lstrcat(buf," sell stop");
					lstrcat(buf," ");
					lstrcat(buf,doubleToStr(curOrderOpenPrice,testerdigits));
					if(ot<2){
						/*lstrcat(buf," ");
						lstrcat(buf,doubleToStr(trades[i].iOrderStopLoss,testerdigits));
						lstrcat(buf," ");
						lstrcat(buf,doubleToStr(trades[i].iOrderTakeProfit,testerdigits));
						*/
						lstrcat(buf,"-");
						lstrcat(buf,doubleToStr(curOrderClosePrice,testerdigits));
					} else {
						lstrcat(buf,"    ");
						lstrcat(buf,doubleToStr(curOpenPrice,testerdigits));
					}
					if(ot1<2){
						lstrcat(buf," ");
						lstrcat(buf,intToStr(curOrderProfit));
					}
					lstrcat(buf,"\r\n");
				}
				lstrcat(buf,"Total orders: ");
				lstrcat(buf,intToStr(drawdowncnt+profitcnt));
				lstrcat(buf,"\r\n");
				lstrcat(buf,"Profitable: ");
				double tmp;tmp=profitcnt/(drawdowncnt+0.001);
				lstrcat(buf,doubleToStr(tmp,2));
				lstrcat(buf,"\r\n");
				lstrcat(buf,"drawdown total: ");
				lstrcat(buf,intToStr(minprofit));
				lstrcat(buf," (");
				lstrcat(buf,intToStr(drawdowncnt));
				lstrcat(buf,")\r\n");
				lstrcat(buf,"profit total: ");
				lstrcat(buf,intToStr(maxprofit));
				lstrcat(buf," (");
				lstrcat(buf,intToStr(profitcnt));
				lstrcat(buf,")\r\n");
				wlog(buf);
				free(buf);
			}
		}
	}
  }
 }
}

