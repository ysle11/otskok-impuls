#include <windows.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>

#include "lzma.h"
#include "main.h"
#include "impuls.h"
#include "Server.h"

extern Server* server;

extern void *Mmalloc(size_t size);
extern char *Mstrdup(const char *s);
extern void *Mrealloc(void* ptr, size_t size);
extern void Mfree(void *ptr);
extern void wlog(const char* buffer);
extern void title(int h,const char* buffer);
extern void wcmd(int reqestor);
extern HWND hwnd,hlog,hpro,hper,hcmd;

Impuls::Impuls(){}
Impuls::~Impuls(){}

void Impuls::Impuls2(int t,int tperiod)
{
testerperiod=tperiod;
if(t==testing){mode=testing;test();}
if(t==optimizing){mode=optimizing;optimize();}

}
inline char* Impuls::intToStr(int i){
	static char strt[18]="";memset(strt,0,18);
	snprintf(strt, 17, "%lld", i);

	return (char *)strt;
}
inline bool Impuls::strToInt(const char *s, int *i){
	char *endChar;
	setlocale(LC_NUMERIC, "C");
	int t = strtol(s, &endChar, 10);
	*i=t;
	if(*endChar!='\0')return false;

	return true;
}
inline int Impuls::strToInt(const char *s){
	char *endChar;
	setlocale(LC_NUMERIC, "C");
	int t = strtol(s, &endChar, 10);
	if(*endChar!='\0')return -1;

	return t;
}
inline char* Impuls::doubleToStr(double d,int precsion) {
	static char strimpuls[18]="";
	memset(strimpuls,0,18);
	snprintf(strimpuls, 12, "%.*f", precsion,d);

	return (char *)strimpuls;
}
inline char* Impuls::timeToStr(const time_t st) {
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
inline char* Impuls::gmtimeToStr(const time_t st) {
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
inline int Impuls::rdtsc(){__asm__ __volatile__("rdtsc");}
inline char* Impuls::substr(const char* S, const int pos, const int len){
	int i;
	char *SubS;
	if ( pos+len <= strlen(S)){
		SubS = (char *)Mmalloc(len+1);
		for (i=0;i<len;i++) SubS[i] = S[pos+i];
		SubS[len] = (char) NULL;
	} else SubS = NULL;

	return (char *)SubS;
}
int Impuls::find(const char *s, const char *key)
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
void Impuls::journalsinit(bool init){
	if(!init){
		history = (struct deal *)Mmalloc(sizeof(struct deal)*2);
		trades = (struct deal *)Mmalloc(sizeof(struct deal)*2);
	}else{
		history = (struct deal *)Mrealloc(history,sizeof(struct deal)*2);
		trades = (struct deal *)Mrealloc(trades,sizeof(struct deal)*2);
	}
	iTradesTotal=0;
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
	testertargetprofit=(testerpoint*(testerspread+testerstop));
	LastBuyOpen=0;
	LastSellOpen=0;
}
void Impuls::add_row_history(double tOrderPrice, int opclose){
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
	iHistoryTotal++;
	history = (struct deal *)Mrealloc(history,sizeof(struct deal)*(iHistoryTotal+2));

}
void Impuls::add_row_trades(short tCmd,double tPrice,double tStop,double tTake){
	curorderticket++;
	totalorders++;
	trades[iTradesTotal].iOrderTicket=curorderticket;
	trades[iTradesTotal].iOrderType=tCmd;
	trades[iTradesTotal].iOrderOpenPrice=tPrice;
	trades[iTradesTotal].iOpenPrice=testercuro;
	trades[iTradesTotal].iOrderStopLoss=tStop;
	trades[iTradesTotal].iOrderTakeProfit=tTake;
	trades[iTradesTotal].iOrderOpclose=OP_DELETE;
	trades[iTradesTotal].iOrderOpenTime=testercurdatetime;
	iTradesTotal++;
	trades = (struct deal *)Mrealloc(trades,sizeof(struct deal)*(iTradesTotal+2));
}
void Impuls::del_row_trades(double tOrderPrice, short opclose){
	if(iTradesTotal>0)if(iTradesCurrent>=0)
	{
		add_row_history(tOrderPrice,opclose);
		for(int i=iTradesCurrent;i<iTradesTotal;i++){
			trades[i].iOrderTicket=trades[i+1].iOrderTicket;
			trades[i].iOrderType=trades[i+1].iOrderType;
			trades[i].iOpenPrice=trades[i+1].iOpenPrice;
			trades[i].iOrderOpenPrice=trades[i+1].iOrderOpenPrice;
			trades[i].iOrderStopLoss=trades[i+1].iOrderStopLoss;
			trades[i].iOrderTakeProfit=trades[i+1].iOrderTakeProfit;
			trades[i].iOrderOpenTime=trades[i+1].iOrderOpenTime;
			trades[i].iOrderOpclose=trades[i+1].iOrderOpclose;
		}
		iTradesTotal--;if(iTradesTotal<0)iTradesTotal=0;
		iTradesCurrent--;if(iTradesCurrent<0)iTradesCurrent=0;
		trades = (struct deal *)Mrealloc(trades,sizeof(struct deal)*(iTradesTotal+2));
	}
}
void Impuls::OrderSelect(int tIndex, int selmode){
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
void Impuls::OrderModify(double tOrderPrice,double tStop,double tTake){
	if(trades[iTradesCurrent].iOrderType<=OP_SELL){
		trades[iTradesCurrent].iOrderStopLoss=tStop;
		trades[iTradesCurrent].iOrderTakeProfit=tTake;
	}
}
void Impuls::OrderSend(int cmd, double price, double stop, double take){
	if(cmd==OP_BUY)      add_row_trades(OP_BUY      ,price,stop,take);else
	if(cmd==OP_SELL)     add_row_trades(OP_SELL     ,price,stop,take);else
	if(cmd==OP_BUYLIMIT) add_row_trades(OP_BUYLIMIT ,price,stop,take);else
	if(cmd==OP_SELLLIMIT)add_row_trades(OP_SELLLIMIT,price,stop,take);else
	if(cmd==OP_BUYSTOP)  add_row_trades(OP_BUYSTOP  ,price,stop,take);else
	if(cmd==OP_SELLSTOP) add_row_trades(OP_SELLSTOP ,price,stop,take);
	iOrdersControl();
}
void Impuls::OrderClose(int OrderTicket, double tOrderPrice, int opclose){
	if(trades[iTradesCurrent].iOrderTicket==OrderTicket)
	del_row_trades(tOrderPrice,opclose);
}
void Impuls::OrderDelete(int OrderTicket){
	if(iTradesTotal>0)if(iTradesCurrent>=0)if(trades[iTradesCurrent].iOrderTicket==OrderTicket){
		int i=iTradesTotal-1;
		while(trades[i].iOrderTicket>=OrderTicket){
		if(trades[i].iOrderTicket==OrderTicket){
			history[iHistoryTotal].iOrderTicket=trades[iTradesCurrent].iOrderTicket;
			history[iHistoryTotal].iOrderType=trades[iTradesCurrent].iOrderType;
			history[iHistoryTotal].iOpenPrice=trades[iTradesCurrent].iOpenPrice;
			history[iHistoryTotal].iOrderOpenPrice=trades[iTradesCurrent].iOrderOpenPrice;
			history[iHistoryTotal].iOrderStopLoss=trades[iTradesCurrent].iOrderStopLoss;
			history[iHistoryTotal].iOrderTakeProfit=trades[iTradesCurrent].iOrderTakeProfit;
			history[iHistoryTotal].iOrderOpenTime=trades[iTradesCurrent].iOrderOpenTime;
			history[iHistoryTotal].iOrderClosePrice=testercurprice;
			history[iHistoryTotal].iOrderCloseTime=testercurdatetime;
			history[iHistoryTotal].iOrderOpclose=OP_DELETE;
			history[iHistoryTotal].iOrderProfit=0;
			iHistoryTotal++;history = (struct deal *)realloc(history,sizeof(struct deal)*(iHistoryTotal+2));
			totalorders--;
			}
			i--;
		}
		for(int i=iTradesCurrent;i<iTradesTotal;i++){
			trades[i].iOrderTicket=trades[i+1].iOrderTicket;
			trades[i].iOrderType=trades[i+1].iOrderType;
			trades[i].iOrderOpenPrice=trades[i+1].iOrderOpenPrice;
			trades[i].iOrderStopLoss=trades[i+1].iOrderStopLoss;
			trades[i].iOrderTakeProfit=trades[i+1].iOrderTakeProfit;
			trades[i].iOrderOpenTime=trades[i+1].iOrderOpenTime;
		}
		iTradesTotal--;if(iTradesTotal<0)iTradesTotal=0;
		iTradesCurrent--;if(iTradesCurrent<0)iTradesCurrent=0;
		trades = (struct deal *)realloc(trades,sizeof(struct deal)*(iTradesTotal+2));
	}
}

time_t* Impuls::OrderOpenTime(){
	static time_t t;
	if(iTradesCurrent==-1)t=history[iHistoryCurrent].iOrderOpenTime;
	else
	t=trades[iTradesCurrent].iOrderOpenTime;
	return(&t);
}

void Impuls::iOrdersControl(){
	double price=testercurprice;
	double priceh=testercurh;
	double pricel=testercurl;
	int datetime=testercurdatetime;
	for(int i=0;i<iTradesTotal;i++){
		OrderSelect(i,MODE_TRADES);
		if(trades[i].iOrderType==OP_BUYSTOP){
			if(trades[i].iOrderOpenPrice<=priceh){
				history[iHistoryTotal].iOrderTicket=trades[i].iOrderTicket;
				history[iHistoryTotal].iOrderType=trades[i].iOrderType;
				history[iHistoryTotal].iOpenPrice=trades[i].iOpenPrice;
				history[iHistoryTotal].iOrderOpenPrice=trades[i].iOrderOpenPrice;
				history[iHistoryTotal].iOrderClosePrice=price;
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
			} else if(datetime-trades[i].iOrderOpenTime>testerltime)OrderDelete(trades[i].iOrderTicket);
		}else
		if(trades[i].iOrderType==OP_SELLSTOP){
			if(trades[i].iOrderOpenPrice>=pricel){
				history[iHistoryTotal].iOrderTicket=trades[i].iOrderTicket;
				history[iHistoryTotal].iOrderType=trades[i].iOrderType;
				history[iHistoryTotal].iOpenPrice=trades[i].iOpenPrice;
				history[iHistoryTotal].iOrderOpenPrice=trades[i].iOrderOpenPrice;
				history[iHistoryTotal].iOrderClosePrice=price;
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
			} else if(datetime-trades[i].iOrderOpenTime>testerltime)OrderDelete(trades[i].iOrderTicket);
		}else
		if(trades[i].iOrderType==OP_BUYLIMIT){
			if(trades[i].iOrderOpenPrice>=pricel){
				history[iHistoryTotal].iOrderTicket=trades[i].iOrderTicket;
				history[iHistoryTotal].iOrderType=trades[i].iOrderType;
				history[iHistoryTotal].iOpenPrice=trades[i].iOpenPrice;
				history[iHistoryTotal].iOrderOpenPrice=trades[i].iOrderOpenPrice;
				history[iHistoryTotal].iOrderClosePrice=price;
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
			} else if(datetime-trades[i].iOrderOpenTime>testerltime)OrderDelete(trades[i].iOrderTicket);
		}else
		if(trades[i].iOrderType==OP_SELLLIMIT){
			if(trades[i].iOrderOpenPrice<=priceh){
				history[iHistoryTotal].iOrderTicket=trades[i].iOrderTicket;
				history[iHistoryTotal].iOrderType=trades[i].iOrderType;
				history[iHistoryTotal].iOpenPrice=trades[i].iOpenPrice;
				history[iHistoryTotal].iOrderOpenPrice=trades[i].iOrderOpenPrice;
				history[iHistoryTotal].iOrderClosePrice=price;
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
			} else if(datetime-trades[i].iOrderOpenTime>testerltime)OrderDelete(trades[i].iOrderTicket);
		}
		if(trades[i].iOrderType==OP_BUY){
			if(trades[i].iOrderStopLoss>=price&&trades[i].iOrderStopLoss!=0.0)
				OrderClose(trades[i].iOrderTicket,trades[i].iOrderStopLoss,OP_SL);else
			if(trades[i].iOrderTakeProfit<=priceh&&trades[i].iOrderTakeProfit!=0.0&&((testercurc>trades[i].iOrderOpenPrice&&testercurc>testercuro&&testercurdatetime==trades[i].iOrderOpenTime)||testercurdatetime>trades[i].iOrderOpenTime))
				OrderClose(trades[i].iOrderTicket,trades[i].iOrderTakeProfit,OP_TP);
		}else
		if(trades[i].iOrderType==OP_SELL){
			if(trades[i].iOrderStopLoss<=price&&trades[i].iOrderStopLoss!=0.0)
				OrderClose(trades[i].iOrderTicket,trades[i].iOrderStopLoss,OP_SL);else
			if(trades[i].iOrderTakeProfit>=pricel&&trades[i].iOrderTakeProfit!=0.0&&((testercurc<trades[i].iOrderOpenPrice&&testercurc<testercuro&&testercurdatetime==trades[i].iOrderOpenTime)||testercurdatetime>trades[i].iOrderOpenTime))
				OrderClose(trades[i].iOrderTicket,trades[i].iOrderTakeProfit,OP_TP);
		}
	}
}
/* Tester */
void Impuls::testerinit()
{
    memset(testervals,0,sizeof(testervals));
//
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
    testervalcnt=6;
*/
//
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
/*	strcpy(&testervals[11][0],"EURGBP");
	testervalspreads[11]=2;
	testervalstops[11]=10;
	strcpy(&testervals[12][0],"USDSGD");
	testervalspreads[12]=5;
	testervalstops[12]=10;*/
    testervalcnt=11;

	testerdataok=false;
    memset(testerpath,0,sizeof(testerpath));
//    lstrcat(testerpath,"history\\MMCIS-Demo\\");
    lstrcat(testerpath,"f:\\Program Files\\MMCIS MetaTrader 4 Client Terminal\\history\\MMCIS-Demo\\");
//    testerperiod=15;
    testercuritem=0;
    testercntper=8000;testercntpervoid=testercntper;
    testerconsolidationbars=17;
    testerdtime=86376;
	testerltime=403088;
	testerquant=3;
	testerisoptimize=false;
//	testerqfcount=5;
	testermincnttrades=49;
	testermdrawdownclimit=2;
	memset(strategyset,0,sizeof(strategyset));
	lstrcat(strategyset,"patterns.fx");lstrcat(strategyset,intToStr(testerperiod));

	if(mode==optimizing){
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
		memset(testeroptval,0,sizeof(struct optimizationvals)*testervalcnt*testersmacnt*2);
	}else{
		testeroptcnf=0;
		testeroptval=0;
	}
	journalsinit(false);
    testermetadata=0;
}
void Impuls::testerloaddata()
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
			testercntper=testercntpervoid;
			int i=dwFileSize-44*testercntper;
			if((int)((dwFileSize-148)/44)<testercntper){i=(dwFileSize-148)/44;testercntper=i;i=148;}
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
			testertargetprofit=testerpoint*(testerspread+testerstop);
			delete []membuf;
		}
	}
}
int Impuls::testertest(double p1,int p2,int p3,int p4,int p5,int p6,int p7,double p8,int p9)
{
	MSG messages;
	int t1=testercntper-testercbars;
	if(mode==optimizing)t1=333;
	int t2=testercntper;
	for(int i=t1;i<t2;i++){
		testercurbar=i;
		testercuro=testermetadata->open[i];
		testercurprice=testermetadata->open[i];
		testercurdatetime=testermetadata->ctm[i];
		if(i==t2-1){
			testercurh=testermetadata->open[i];
			testercurl=testermetadata->open[i];
			testercurc=testermetadata->open[i];
		}else
		{
			testercurh=testermetadata->high[i];
			testercurl=testermetadata->low[i];
			testercurc=testermetadata->close[i];
		}
		double iclose=testermetadata->close[i];testermetadata->close[i]=testermetadata->open[i];
		double ihigh=testermetadata->high[i];testermetadata->high[i]=testermetadata->open[i];
		double ilow=testermetadata->low[i];testermetadata->low[i]=testermetadata->open[i];
		testerstart(p1,p2,p3,p4,p5,p6,p7,p8,p9);
		testermetadata->close[i]=iclose;
		testermetadata->high[i]=ihigh;
		testermetadata->low[i]=ilow;
		if(drawdowncntmax>=testermdrawdownclimit)if(mode==optimizing)return -1;
		if(PeekMessage(&messages,NULL,0U,0U,PM_REMOVE)){
			if(messages.message == WM_QUIT)exit(EXIT_SUCCESS);
			if((messages.hwnd==hcmd)&&(messages.message==WM_KEYUP)&&messages.wParam==VK_RETURN)wcmd(cmdimpuls);
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}
	}
	return 0;

}
void Impuls::testerstart(double k1,int d1,int k2,int d2,int k3,int d3,int l1,double l2,int limit){
	iOrdersControl();
	//testercontrol();
	double sig=testersignal(k1,d1,k2,d2,k3,d3,l1,l2,limit);
	if(sig!=0.0){
		double SL,TP;
		if (sig>0.0)
		if (testerNewBar(OP_BUYLIMIT))
		//if (testerquant())
		{
			testercloseall(2);
			SL=0;TP=sig+testertargetprofit;
			if(limit<0)OrderSend(OP_BUYSTOP, sig,SL,TP);else
			if(limit>0)OrderSend(OP_BUYLIMIT,sig,SL,TP);
		}
		if (sig<0.0)
		if (testerNewBar(OP_SELLLIMIT))
		//if (testerquant())
		{
			sig*=-1.0;
			testercloseall(1);SL=0;TP=sig-testertargetprofit;
			if(limit<0)OrderSend(OP_SELLSTOP, sig,SL,TP);else
			if(limit>0)OrderSend(OP_SELLLIMIT,sig,SL,TP);
		}
	}
}
bool Impuls::testerquant_(){
	tm tm2; tm tm1;
	time_t ttt=testercurdatetime;
	tm2=*gmtime(&ttt);
	int r=0;for(int o=iTradesTotal-1;o>=0;o--){
		OrderSelect(o, MODE_TRADES);
		tm1=*gmtime((const time_t*)&curOrderOpenTime);
		//if(StrToTime(TimeToStr(OrderOpenTime(),TIME_DATE|TIME_MINUTES))==iTime(sm,PERIOD_H1,0))
		if(tm1.tm_year==tm2.tm_year)if(tm1.tm_mon==tm2.tm_mon)if(tm1.tm_mday==tm2.tm_mday)if(tm1.tm_hour==tm2.tm_hour)//if(tm1.tm_min==tm2.tm_min)
		if((curOrderType==OP_SELL)||(curOrderType==OP_BUY))
			r++;
	}
	int h=0,r2=0;
	if(testerisoptimize){
		int o=iHistoryTotal-1;bool f=true;
		while(o>=0&&f){
			OrderSelect(o, MODE_HISTORY);
			tm1=*gmtime((const time_t*)curOrderOpenTime);
			//if(StrToTime(TimeToStr(OrderOpenTime(),TIME_DATE))==iTime(sm,PERIOD_D1,0))
			if(tm1.tm_year==tm2.tm_year)if(tm1.tm_mon==tm2.tm_mon)if(tm1.tm_mday==tm2.tm_mday){
				f=true;
				if((curOrderType==OP_SELL)||(curOrderType==OP_BUY)){
					h++;
					if(tm1.tm_hour==tm2.tm_hour)r2++;
				}
			}else f=false;
			o--;
		}
	}
	if(h<testerquant&&r<1&&r2<1)
	return(true);
	else
	return(false);
}
bool Impuls::testerNewBar(int op){
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
}
void Impuls::testercloseall(int a=0){
	int o;
	if(a==1){
		for(o=iTradesTotal-1;o>=0;o--){
			OrderSelect(o, MODE_TRADES);
			if(curOrderType==OP_BUYSTOP){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_BUYLIMIT){OrderDelete(curOrderTicket);}
		}
	}else
	if(a==2){
		for(o=iTradesTotal-1;o>=0;o--){
			OrderSelect(o, MODE_TRADES);
			if(curOrderType==OP_SELLSTOP){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_SELLLIMIT){OrderDelete(curOrderTicket);}
		}
	}
}
void Impuls::testercontrol(){
	int o,d;
	tm tm3; tm tm4;
	memset(&tm3,0,sizeof(tm));
	memset(&tm4,0,sizeof(tm));
	for(o=iTradesTotal-1;o>=0;o--){
		OrderSelect(o, MODE_TRADES);

		time_t ttime1=curOrderOpenTime;tm3=*gmtime(&ttime1);
		time_t ttime2=curOrderCloseTime;if(curOrderCloseTime<0)ttime2=curOrderOpenTime;
		tm4=*gmtime(&ttime2);
		d=0;
		if(tm3.tm_wday>tm4.tm_wday)d=172800;
		if((int)((int)testercurdatetime-(int)curOrderOpenTime-d)>testerdtime)//3599*24
		{
			if(curOrderType==OP_BUY){OrderClose(curOrderTicket, testercurprice, OP_CLOSE);}else
			if(curOrderType==OP_SELL){OrderClose(curOrderTicket, testercurprice, OP_CLOSE);}
		}
		if((int)((int)testercurdatetime-(int)curOrderOpenTime-d)>testerltime)//3599*112
		{
			if(curOrderType==OP_BUYSTOP){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_SELLSTOP){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_BUYLIMIT){OrderDelete(curOrderTicket);}else
			if(curOrderType==OP_SELLLIMIT){OrderDelete(curOrderTicket);}
		}
	}
}
double Impuls::testersignal(double Probab,int dstop,int Nstop,int delta,int Nidelt,int Nideltstop,int NN,double forg,int limit){

	   double sig=0.0;
  	   double Price=testercuro,Point=testerpoint;int CurTime=testercurdatetime,Ncomb,mn;
  	   int buy_open=0, sell_open=0, buy_close=0, sell_close=0;
	   for(int idelt = 1; idelt <= Nidelt; idelt++)
	     {
	       engine[idelt].delt = delta*idelt;
	       if(fabs(Price - engine[idelt].P[1]) > (engine[idelt].delt - 0.5)*Point)
	         {
	           for(int i = 1; i <= NN - 1; i++)
	             {
	               engine[idelt].P[NN+1-i] = engine[idelt].P[NN-i];
	             }
	           engine[idelt].P[1] = Price;
	         }
	       if(fabs(engine[idelt].P[1] - engine[idelt].pt) > 0.5*Point)
	         {
	           for(int i = 1; i <= NN - 1; i++)
	             {
	               if(engine[idelt].P[i] > engine[idelt].P[i+1])
	                 {
	                   engine[idelt].sr[i] = 1;
	                 }
	               else
	                 {
	                   engine[idelt].sr[i] = 0;
	                 }
	             }
	           Ncomb=0;mn=1;for(int i=1;i<=NN-1;i++){Ncomb = Ncomb + mn*engine[idelt].sr[i];mn = 2*mn;}

	           for(int istop = 1; istop <= Nstop; istop++)
	             {
	               if(engine[idelt].sd[Ncomb][istop] == 0 && CurTime - engine[idelt].LastSd[Ncomb][istop] >2*testerperiod*60)
	                 {
	                   engine[idelt].sd[Ncomb][istop] = 1;
	                   engine[idelt].cen[Ncomb][istop] = Price;
	                   engine[idelt].LastSd[Ncomb][istop] = CurTime;
	                 }
	             }
	           for(int istop = 1; istop <= Nstop; istop++)
	             {
	               for(int i = 0; i <= mn - 1; i++)
	                 {
	                   if(engine[idelt].sd[i][istop] == 1)
	                     {
						   //double forgw = forg;
	                       if(Price - engine[idelt].cen[i][istop] > dstop*istop*Point)
	                         {
	                           engine[idelt].rost[i][istop] = engine[idelt].rost[i][istop] / forg + 1.0;
	                           engine[idelt].spad[i][istop] = engine[idelt].spad[i][istop] / forg;
	                           engine[idelt].sd[i][istop] = 0;
	                           engine[idelt].nsd[i][istop] = engine[idelt].nsd[i][istop] + 1;
	                         }
	                       if(engine[idelt].cen[i][istop] - Price > dstop*istop*Point)
	                         {
	                           engine[idelt].spad[i][istop] = engine[idelt].spad[i][istop] / forg + 1.0;
	                           engine[idelt].rost[i][istop] = engine[idelt].rost[i][istop] / forg;
	                           engine[idelt].sd[i][istop] = 0;
	                           engine[idelt].nsd[i][istop] = engine[idelt].nsd[i][istop] + 1;
	                         }
	                     }
	                 }
	             }
	           for(int istop = 1; istop <= Nstop; istop++)
	             {
	               prob = engine[idelt].rost[Ncomb][istop] / (engine[idelt].rost[Ncomb][istop] +
	                      engine[idelt].spad[Ncomb][istop] + 0.0001);
	               if(prob > Probab && engine[idelt].nsd[Ncomb][istop] > Nideltstop && CurTime - LastBuyOpen > 2*testerperiod*60)
	                 {
					   buy_open++;
	                   for(int i=0;i<iTradesTotal;i++)
	                         { OrderSelect(i,MODE_TRADES);
	                           if(trades[iTradesCurrent].iOrderType == OP_SELL)
	                             {
	                               if(prob > (FDlast_sell + 0.05))
								     sell_close++;
	                             }
	                         }
	                   FDlast_buy=prob;
	                 }
	               for(int i=0;i<iTradesTotal;i++)
	                     { OrderSelect(i,MODE_TRADES);
	                       if(trades[iTradesCurrent].iOrderType == OP_SELL)
	                         {
	                           if(prob > 0.6 && engine[idelt].nsd[Ncomb][istop] > Nideltstop &&
	                             (trades[iTradesCurrent].iOrderOpenPrice - Price) > (dstop / 2)*Point)
	                               sell_close++;
	                         }
	                     }
	               prob = engine[idelt].spad[Ncomb][istop] / (engine[idelt].rost[Ncomb][istop]  +
	                      engine[idelt].spad[Ncomb][istop] + 0.0001);
	               if(prob > Probab && engine[idelt].nsd[Ncomb][istop] > Nideltstop && CurTime - LastSellOpen > 2*testerperiod*60)
	                 {
	                   sell_open++;
	                   for(int i=0; i<iTradesTotal; i++)
	                         { OrderSelect(i,MODE_TRADES);
	                           if(trades[iTradesCurrent].iOrderType == OP_BUY)
	                             {
	                               if(prob > (FDlast_buy + 0.05))
	                                   buy_close++;
	                             }
	                         }
	                   FDlast_sell=prob;
	                 }
	               for(int i=0; i<iTradesTotal; i++)
	                     { OrderSelect(i,MODE_TRADES);
	                       if(trades[iTradesCurrent].iOrderType == OP_BUY)
	                         {
	                           if(prob > 0.6 && engine[idelt].nsd[Ncomb][istop] > Nideltstop &&
	                             (Price - trades[iTradesCurrent].iOrderOpenPrice) > (dstop / 2)*Point)
	                               buy_close++;
	                         }
	                     }
	             }
	         }
	       engine[idelt].pt=engine[idelt].P[1];
	     }
	     if(sell_open>buy_open&&buy_close>=sell_close){sell_open=1;buy_open=0;}else if(buy_open>sell_open&&sell_close>=buy_close){sell_open=0;buy_open=1;}else {sell_open=0;buy_open=0;}
	     //if(sell_open>buy_open){sell_open=1;buy_open=0;}else if(buy_open>sell_open){sell_open=0;buy_open=1;}else{sell_open=0;buy_open=0;}
	       if(buy_open == 1)
	         {
			   sig=(testermetadata->open[testercurbar]-limit*testerpoint);
			   LastBuyOpen=CurTime;
			   //FDlast_buy=sig;
	         }else
	       if(sell_open == 1)
	         {
			   sig=(-(testermetadata->open[testercurbar]+limit*testerpoint));
			   LastSellOpen=CurTime;
			   //FDlast_sell=sig*(-1.0);
	         }
	return sig;
}
void Impuls::test()
{
	wlog("# impuls test\r\n");title(whwnd,"impuls test");
	char *tmp=(char*)Mmalloc(11255);
	char tmp1[25];int drawdowncnt0,ordercnt0,profitcnt0,profitindex0,errors=0;
	double drawdowncnt,ordercnt,oprofitcnt,profitindex,p1,p2,p3,p4,p5,p6,p7,p8;
	char *buf1=(char*)Mmalloc(21130);
	memset(buf1,0,21130);
	char* buf2;buf2=(char*)GlobalAlloc(GPTR,21140);memset(buf2,0,21140);
	double highSELLIMIT,lowSELLSTOP,lowBUYLIMIT,highBUYSTOP,highSELL,lowSELL,highBUY,lowBUY;
	double midSELLIMIT,midSELLSTOP,midBUYLIMIT,midBUYSTOP,midSELL,midBUY;
	int cntSELLIMIT,cntSELLSTOP,cntBUYLIMIT,cntBUYSTOP,cntSELL,cntBUY;
	tm tfrom,tto,topt;
	testerinit();
	csorted = new consolidatesorted[testervalcnt+2];
	memset(csorted,0,sizeof(struct consolidatesorted)*(testervalcnt+2));
	testerreset_im(true);
	testerop_info = new optimized_info[1];

	for(testercuritem=0;testercuritem<testervalcnt;testercuritem++){
		testerloaddata();
		if(testerdataok){
			errors=0;highSELLIMIT=0.0;lowSELLSTOP=99999999.0;lowBUYLIMIT=99999999.0;highBUYSTOP=0.0;highSELL=0.0;lowSELL=99999999.0;highBUY=0.0;lowBUY=99999999.0;
			cntSELLIMIT=0;cntSELLSTOP=0;cntBUYLIMIT=0;cntBUYSTOP=0;cntSELL=0;cntBUY=0;
			midSELLIMIT=0.0,midSELLSTOP=0.0,midBUYLIMIT=0.0,midBUYSTOP=0.0,midSELL=0.0,midBUY=0.0;
			for(int ls=-1;ls<=1;ls+=2){
				testerreset_im();
				if(ls<0)testerloadimpuls(true);else testerloadimpuls();
				if(testerimpulsok){
					memset(tmp,0,11255);
					lstrcat(tmp,testervals[testercuritem]);lstrcat(tmp," ");
					if(ls>0)lstrcat(tmp," limits..");else lstrcat(tmp," stops..");
					title(whpro,tmp);
					journalsinit();
					p1=testerop_info->p1;
					p2=testerop_info->p2;
					p3=testerop_info->p3;
					p4=testerop_info->p4;
					p5=testerop_info->p5;
					p6=testerop_info->p6;
					p7=testerop_info->p7;
					p8=testerop_info->p8;
					for(testercbars=1;testercbars<(testerconsolidationbars+2);testercbars++){
						if(iTradesTotal>0)testercbars=testerconsolidationbars+3;else
						{
							journalsinit();
							if(mode!=optimizing)testertest(0.82,(int)p2,(int)p3,(int)p4,(int)p5,(int)p6,(int)p7,p8,ls*18);else
							{
								testertest(p1,(int)p2,(int)p3,(int)p4,(int)p5,(int)p6,(int)p7,p8,ls*18);
							    testerrestoreimpuls();
							}
						}
					}
					oprofitcnt=maxprofit;
					ordercnt=drawdowncnt+profitcnt;
					profitindex=profitcntmax/drawdowncntmax;
					drawdowncnt=minprofit;
					int ot,ot1;
					for(int i=0;i<iTradesTotal;i++){
						OrderSelect(i, MODE_TRADES);
						ot=(int)curOrderType;ot1=ot;
						if(curOrderOpenPrice>highSELLIMIT&&(ot==OP_SELLLIMIT))highSELLIMIT=curOrderOpenPrice;
						if(curOrderOpenPrice<lowSELLSTOP&&(ot==OP_SELLSTOP))lowSELLSTOP=curOrderOpenPrice;
						if(curOrderOpenPrice>highBUYSTOP&&(ot==OP_BUYSTOP))highBUYSTOP=curOrderOpenPrice;
						if(curOrderOpenPrice<lowBUYLIMIT&&(ot==OP_BUYLIMIT))lowBUYLIMIT=curOrderOpenPrice;

						if(curOrderOpenPrice>highSELL&&(ot==OP_SELL))highSELL=curOrderOpenPrice;
						if(curOrderOpenPrice<lowSELL&&(ot==OP_SELL))lowSELL=curOrderOpenPrice;
						if(curOrderOpenPrice>highBUY&&(ot==OP_BUY))highBUY=curOrderOpenPrice;
						if(curOrderOpenPrice<lowBUY&&(ot==OP_BUY))lowBUY=curOrderOpenPrice;

						if(ot==OP_SELLLIMIT){if(midSELLIMIT==0.0)midSELLIMIT=curOrderOpenPrice;else midSELLIMIT=(midSELLIMIT+curOrderOpenPrice)/2;}
						if(ot==OP_SELLSTOP){if(midSELLSTOP==0.0)midSELLSTOP=curOrderOpenPrice;else midSELLSTOP=(midSELLSTOP+curOrderOpenPrice)/2;}
						if(ot==OP_BUYLIMIT){if(midBUYLIMIT==0.0)midBUYLIMIT=curOrderOpenPrice;else midBUYLIMIT=(midBUYLIMIT+curOrderOpenPrice)/2;}
						if(ot==OP_BUYSTOP){if(midBUYSTOP==0.0)midBUYSTOP=curOrderOpenPrice;else midBUYSTOP=(midBUYSTOP+curOrderOpenPrice)/2;}

						if(ot==OP_SELL){if(midSELL==0.0)midSELL=curOrderOpenPrice;else midSELL=(midSELL+curOrderOpenPrice)/2;}
						if(ot==OP_BUY){if(midBUY==0.0)midBUY=curOrderOpenPrice;else midBUY=(midBUY+curOrderOpenPrice)/2;}

						if(ot==OP_SELLLIMIT)cntSELLIMIT++;
						if(ot==OP_SELLSTOP)cntSELLSTOP++;
						if(ot==OP_BUYLIMIT)cntBUYLIMIT++;
						if(ot==OP_BUYSTOP)cntBUYSTOP++;
						if(ot==OP_SELL)cntSELL++;
						if(ot==OP_BUY)cntBUY++;
					}
				}
			}
			if(midSELLIMIT!=0.0&&midSELLSTOP==0.0)midSELLSTOP=midSELLIMIT-2*testertargetprofit;else
			if(midSELLIMIT==0.0&&midSELLSTOP!=0.0)midSELLIMIT=midSELLSTOP+2*testertargetprofit;
			if(midBUYLIMIT!=0.0&&midBUYSTOP==0.0)midBUYSTOP=midBUYLIMIT+2*testertargetprofit;else
			if(midBUYLIMIT==0.0&&midBUYSTOP!=0.0)midBUYLIMIT=midBUYSTOP-2*testertargetprofit;

			if(highSELLIMIT!=0.0&&lowSELLSTOP==99999999.0)lowSELLSTOP=highSELLIMIT-2*testertargetprofit;else
			if(highSELLIMIT==0.0&&lowSELLSTOP!=99999999.0)highSELLIMIT=lowSELLSTOP+2*testertargetprofit;
			if(highBUYSTOP!=0.0&&lowBUYLIMIT==99999999.0)lowBUYLIMIT=highBUYSTOP-2*testertargetprofit;else
			if(highBUYSTOP==0.0&&lowBUYLIMIT!=99999999.0)highBUYSTOP=lowBUYLIMIT+2*testertargetprofit;

			void* v=csorted[testercuritem].val;
			memset(v,0,22);
			memcpy(v,&testervals[testercuritem],21);
			csorted[testercuritem].digits=testerdigits;
			csorted[testercuritem].priceopen=testercuro;
			csorted[testercuritem].targetprofit=testertargetprofit;
			csorted[testercuritem].datetime=testermetadata->ctm[testercntper-1];
			csorted[testercuritem].highSELLIMIT=highSELLIMIT;
			csorted[testercuritem].midSELLIMIT=midSELLIMIT;
			csorted[testercuritem].cntSELLIMIT=cntSELLIMIT;
			csorted[testercuritem].lowSELLSTOP=lowSELLSTOP;
			csorted[testercuritem].midSELLSTOP=midSELLSTOP;
			csorted[testercuritem].cntSELLSTOP=cntSELLSTOP;
			csorted[testercuritem].highBUYSTOP=highBUYSTOP;
			csorted[testercuritem].midBUYSTOP=midBUYSTOP;
			csorted[testercuritem].cntBUYSTOP=cntBUYSTOP;
			csorted[testercuritem].lowBUYLIMIT=lowBUYLIMIT;
			csorted[testercuritem].midBUYLIMIT=midBUYLIMIT;
			csorted[testercuritem].cntBUYLIMIT=cntBUYLIMIT;
			csorted[testercuritem].lowSELL=lowSELL;
			csorted[testercuritem].midSELL=midSELL;
			csorted[testercuritem].cntSELL=cntSELL;
			csorted[testercuritem].lowBUY=lowBUY;
			csorted[testercuritem].midBUY=midBUY;
			csorted[testercuritem].cntBUY=cntBUY;
		}
	}
	for(testercuritem=0;testercuritem<testervalcnt;testercuritem++){
		int c1=csorted[testercuritem].cntSELLIMIT+csorted[testercuritem].cntSELLSTOP;
		int c2=csorted[testercuritem].cntBUYLIMIT+csorted[testercuritem].cntBUYSTOP;
		if(c1>c2&&c1>0){
			lstrcat(buf2,gmtimeToStr(csorted[testercuritem].datetime));
			lstrcat(buf2,(const char*)csorted[testercuritem].val);
//			lstrcat(buf2," selllimit ");
			lstrcat(buf2," buystop ");
			lstrcat(buf2,doubleToStr(csorted[testercuritem].priceopen,csorted[testercuritem].digits));
			lstrcat(buf2," ");lstrcat(buf2,doubleToStr(csorted[testercuritem].highSELLIMIT,csorted[testercuritem].digits));lstrcat(buf2," tp:");
//			lstrcat(buf2,doubleToStr((csorted[testercuritem].highSELLIMIT-csorted[testercuritem].targetprofit),csorted[testercuritem].digits));
			lstrcat(buf2,doubleToStr((csorted[testercuritem].highSELLIMIT+csorted[testercuritem].targetprofit),csorted[testercuritem].digits));
			lstrcat(buf2," pips:");
			lstrcat(buf2,intToStr((int)((csorted[testercuritem].highSELLIMIT-csorted[testercuritem].lowSELLSTOP)/(1/pow(10,csorted[testercuritem].digits)))));
			lstrcat(buf2," tg:");
			lstrcat(buf2,intToStr((int)((csorted[testercuritem].targetprofit)/(1/pow(10,csorted[testercuritem].digits)))));lstrcat(buf2,"\r\n");
		}
		if(c1<c2&&c2>0){
			lstrcat(buf2,gmtimeToStr(csorted[testercuritem].datetime));
			lstrcat(buf2,(const char*)csorted[testercuritem].val);
//			lstrcat(buf2," buylimit ");
			lstrcat(buf2," sellstop ");
			lstrcat(buf2,doubleToStr(csorted[testercuritem].priceopen,csorted[testercuritem].digits));lstrcat(buf2," ");
			lstrcat(buf2,doubleToStr(csorted[testercuritem].lowBUYLIMIT,csorted[testercuritem].digits));lstrcat(buf2," tp:");
//			lstrcat(buf2,doubleToStr((csorted[testercuritem].lowBUYLIMIT+csorted[testercuritem].targetprofit),csorted[testercuritem].digits));
			lstrcat(buf2,doubleToStr((csorted[testercuritem].lowBUYLIMIT-csorted[testercuritem].targetprofit),csorted[testercuritem].digits));
			lstrcat(buf2," pips:");lstrcat(buf2,intToStr((int)((csorted[testercuritem].highBUYSTOP-csorted[testercuritem].lowBUYLIMIT)/(1/pow(10,csorted[testercuritem].digits)))));lstrcat(buf2," tg:");
			lstrcat(buf2,intToStr((int)((csorted[testercuritem].targetprofit)/(1/pow(10,csorted[testercuritem].digits)))));lstrcat(buf2,"\r\n");
		}
	}
	wlog(buf2);
	int slen=strlen(buf2);if(slen>=15999)slen=15998;
	SuspendThread(server->timersDataTransfer);
	if(slen==0)slen++;
	memset(&server->pending_orders_im,0,slen+1);
	memcpy(&server->pending_orders_im,buf2,slen);
	server->pending_orders_imSize=slen;
	if(slen==1)lstrcat((char*)server->pending_orders_im," ");
	ResumeThread(server->timersDataTransfer);
    memset(buf2,0,240);lstrcat(buf2,"Impuls last build time: ");lstrcat(buf2,timeToStr());
	title(whwnd,buf2);title(whpro," ");

	delete[] testerop_info;
	delete[] testeroptval;
	delete[] testermetadata;
	delete[] testeroptcnf;
	delete[] csorted;
	Mfree(history);
	Mfree(trades);
	GlobalFree(buf2);
	free(buf1);
	free(tmp);
}

void Impuls::optimize(){

	testerinit();
	testerloaddata();
	const time_t dt0=time(0);srand(rdtsc());tm tm6; tm tm5;
	char* tmp;tmp=(char*)malloc(5255);
	char* buf1;buf1=(char*)malloc(525);
	memset(buf1,0,525);lstrcat(buf1,"# optimization ");lstrcat(buf1,gmtimeToStr(testermetadata->ctm[0]));lstrcat(buf1,"- ");
	lstrcat(buf1,gmtimeToStr(testermetadata->ctm[testercntper-1]));
	lstrcat(buf1,"\r\n ");
	wlog(buf1);

	double p[]={0.8,35.0,3.0,2.0,21.0,7.0,11.0,1.05};
	double oprofitcnt,oordercnt,oprofitindex,odrawdowncnt;
	double profitcnt2=0.0,ordercnt2=0.0,profitindex2=0.0,drawdowncnt2=0.0;
	int p2[8];int res1=0,mincnt,sorl,sorli;
	testerreset_im(true);
	testerop_info = new optimized_info[1];
	for(testercuritem=0;testercuritem<testervalcnt;testercuritem++){
		testerloaddata();
		if(testerdataok){
			memset(tmp,0,5255);
			lstrcat(tmp,"\r\n ");
			lstrcat(tmp,testervals[testercuritem]);
			lstrcat(tmp," ");
			lstrcat(tmp,gmtimeToStr(testermetadata->ctm[0]));
			lstrcat(tmp,"-");
			lstrcat(tmp,gmtimeToStr(testermetadata->ctm[testercntper-1]));
			lstrcat(tmp,"\r\n");
			wlog(tmp);

			testerop_info->datefrom=testermetadata->ctm[1];
			testerop_info->dateto=testermetadata->ctm[testercntper-1];
			testerop_info->dateopt=time(0);
			testerop_info->p1=p[0];
			testerop_info->p2=(int)p[1];
			testerop_info->p3=(int)p[2];
			testerop_info->p4=(int)p[3];
			testerop_info->p5=(int)p[4];
			testerop_info->p6=(int)p[5];
			testerop_info->p7=(int)p[6];
			testerop_info->p8=p[7];
			for(int ls=-1;ls<=1;ls+=2){
                testerreset_im();
				profitcnt2=0;ordercnt2=0;profitindex2=0;drawdowncnt2=0;res1=0;
				profitcnt2=0.0;ordercnt2=0.0;profitindex2=0.0;drawdowncnt2=0.0;
				int i=0,ix=0;mincnt=testermincnttrades;//p[0]=0.92;
				while(i<9){
					journalsinit(true);
					int o=testertest(p[0],(int)p[1],(int)p[2],(int)p[3],(int)p[4],(int)p[5],(int)p[6],p[7],ls*18);
					if(o!=-1){
						oprofitcnt=maxprofit;
						oordercnt=drawdowncnt+profitcnt;
						oprofitindex=(oordercnt/testercntper)*24*4;
						odrawdowncnt=minprofit;
						{
							res1++;
							profitcnt2=oprofitcnt;ordercnt2=oordercnt;profitindex2=oprofitindex;drawdowncnt2=odrawdowncnt;
							memset(tmp,0,sizeof(tmp));
							lstrcat(tmp,"  ");
							if((fabs(sorderscnt-lorderscnt)>(drawdowncnt+profitcnt)/2)||((drawdowncnt+profitcnt)==0))
							lstrcat(tmp,"-");
							lstrcat(tmp,"  ");
							lstrcat(tmp,intToStr(i+ix*50));
							lstrcat(tmp,": ");
							lstrcat(tmp,doubleToStr(oprofitcnt,0));
							lstrcat(tmp," ");
							lstrcat(tmp,doubleToStr(oordercnt,0));
							lstrcat(tmp," ");
							lstrcat(tmp,doubleToStr(oprofitindex,2));
							lstrcat(tmp," ");
							lstrcat(tmp,doubleToStr(odrawdowncnt,0));
							lstrcat(tmp,"     ");
							lstrcat(tmp,doubleToStr(p[0],2));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr((int)p[1]));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr((int)p[2]));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr((int)p[3]));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr((int)p[4]));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr((int)p[5]));
							lstrcat(tmp," ");
							lstrcat(tmp,intToStr((int)p[6]));
							lstrcat(tmp," ");
							lstrcat(tmp,doubleToStr(p[7],2));
							if(ls<0)lstrcat(tmp," limit");else lstrcat(tmp," stop");
							lstrcat(tmp,"\r\n");
							wlog(tmp);
						}
					}
					i++;
					//p[0]-=0.01;
				}
			if(ls<0)testersaveimpuls(true);else testersaveimpuls();
			}
		}
	}
	delete[] testerop_info;
	memset(tmp,0,sizeof(tmp));
	lstrcat(tmp,timeToStr(0));
	lstrcat(tmp,"optimization complete.\r\nTotal time used: ");
	lstrcat(tmp,intToStr((time(NULL)-dt0)/60));
	lstrcat(tmp," minutes \r\n");
	wlog(tmp);
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
void Impuls::testerrestoreimpuls(){
	memcpy(engine,engine2, sizeof(struct Engine)*22);
}
void Impuls::testersaveimpuls(bool limitstop){
	 testerop_info->period_im=testerperiod;
	 char tmp[255];
	 memset(tmp,0,sizeof(tmp));
	 lstrcat(tmp,testervals[testercuritem]);
	 if(limitstop)lstrcat(tmp," limit");else lstrcat(tmp," stop");
	 lstrcat(tmp," compress..\r\n");
	 wlog(tmp);
	 string full_path;

 		 int MAX_SIZE=4334188;//3252749;
	 uint8_t* in_buf; in_buf=(uint8_t*)malloc(MAX_SIZE);
	 uint8_t* out_buf; out_buf=(uint8_t*)malloc(MAX_SIZE);
	 size_t out_len=0,in_len1;

	 lzma_ret ret_xz;
	 uint32_t preset = 1;//6|LZMA_PRESET_EXTREME;
	 lzma_check check = LZMA_CHECK_CRC64;
	 lzma_stream strm = LZMA_STREAM_INIT;
	 lzma_action action;
	 ret_xz = lzma_easy_encoder (&strm, preset, check);

	 strm.next_in = (const uint8_t *)engine ;
	 strm.avail_in = sizeof(struct Engine)*22;
     action = LZMA_RUN;
	do {
	 strm.next_out = out_buf;
	 strm.avail_out = MAX_SIZE;
     ret_xz = lzma_code (&strm, action);
	 out_len=MAX_SIZE-strm.avail_out;
	} while (strm.avail_out == 0);

     action = LZMA_FINISH;
	do {
	 strm.next_out = out_buf+out_len;
	 strm.avail_out = MAX_SIZE;
     ret_xz = lzma_code (&strm, action);
     out_len+=(MAX_SIZE-strm.avail_out);
	} while (strm.avail_out == 0);

	full_path = strategyset;
	full_path+=testervals[testercuritem];
	if(limitstop)full_path+="l";else full_path+="s";
	std::ofstream os(full_path.c_str(), std::ios::out | std::ios::binary);
	if (!os){
		wlog("\r\ncan't use .fx file\r\n");
	}else
	{
		os.write((const char *)testerop_info, sizeof(struct optimized_info));
		os.write((const char *)out_buf, out_len);
		os.close();
	}

	in_len1=out_len;lzma_end (&strm);
	free(out_buf);free(in_buf);
}
void Impuls::testerloadimpuls(bool limitstop){
	string full_path;
	testerimpulsok=false;
	int MAX_SIZE=4334188;//3252749;
	uint8_t* in_buf; in_buf=new uint8_t[MAX_SIZE];
	size_t out_len=0,in_len;

	lzma_ret ret_xz;
	uint32_t preset = 1;//|LZMA_PRESET_EXTREME;
	lzma_check check = LZMA_CHECK_CRC64;
	lzma_action action;

	full_path = strategyset;
	full_path += testervals[testercuritem];
	if(limitstop)full_path+="l";else full_path+="s";
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if (!is){
		wlog(" err: can't use .fx impuls file (");
		wlog(datfile);
		wlog(") for ");
		wlog(testervals[testercuritem]);
	    if(limitstop)wlog(" limit");else wlog(" stop");
		wlog(" !\r\n");
	}else
	{
		is.seekg(0, ios::end);
		in_len=is.tellg();
		is.seekg(0, ios::beg);
		in_len-=sizeof(struct optimized_info);
		is.read((char *)&testerop_info[0], sizeof(struct optimized_info));
		testertime_pending_orders=testerop_info[0].dateopt;
		testerperiod=testerop_info[0].period_im;
		is.read((char *)in_buf, in_len);
		is.close();
		testerimpulsok=true;
	}

    memset(engine,0, sizeof(struct Engine)*22);
    MAX_SIZE=sizeof(struct Engine)*22;

	const uint32_t flags = LZMA_TELL_UNSUPPORTED_CHECK | LZMA_CONCATENATED;
	uint64_t memory_limit = UINT64_MAX;
	lzma_stream strm1 = LZMA_STREAM_INIT;
	ret_xz = lzma_stream_decoder (&strm1, memory_limit, flags);

	strm1.next_in = in_buf;
	strm1.avail_in = in_len;
    action = LZMA_RUN;
	do {
	 strm1.next_out = (uint8_t *)engine;
	 strm1.avail_out = MAX_SIZE;
	 ret_xz = lzma_code (&strm1, action);
	 out_len=MAX_SIZE-strm1.avail_out;
	} while (strm1.avail_out == 0);

    action = LZMA_FINISH;
	do {
	 strm1.next_out = (uint8_t *)engine+out_len;
	 strm1.avail_out = MAX_SIZE;
	 ret_xz = lzma_code (&strm1, action);
	 out_len+=(MAX_SIZE-strm1.avail_out);
	} while (strm1.avail_out == 0);
	lzma_end (&strm1);

    for(int i = 0; i <= 21; i++)
	{memset(&engine[i].cen[0][0],0, sizeof(engine[i].cen));
     memset(&engine[i].LastSd[0][0],0, sizeof(engine[i].LastSd));
     memset(&engine[i].sd[0][0],0, sizeof(engine[i].sd));
	}
	memcpy(engine2,engine, sizeof(struct Engine)*22);
	delete[] in_buf;
}
void Impuls::testerreset_im(bool a){
	 if(a){
		engine=new Engine[22];
	    engine2=new Engine[22];
	 }
	 else
	 {
		delete[] engine;engine=new Engine[22];
	    delete[] engine2;engine2=new Engine[22];
	 }
	 memset(engine,0,sizeof(struct Engine)*22);
	 memset(engine2,0,sizeof(struct Engine)*22);
	 LastBuyOpen=0;FDlast_buy=0.0;LastSellOpen=0;FDlast_sell=0.0;
};

