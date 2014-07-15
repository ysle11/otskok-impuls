#ifndef OTSKOK_H
#define OTSKOK_H

class Otskok
{
	char* intToStr(int i);
	bool strToInt(const char *s, int *i);
	int strToInt(const char *s);
	char* doubleToStr(double d,int precsion);
	int rdtsc();
	int find(const char *s, const char *key);
	int find2(const char *s, const char *key);
	char* substr(const char* S, const int pos, const int len);
	char* timeToStr(const time_t st=0);
	char* gmtimeToStr(const time_t st=0);
	void sleep(int delay);
	int mode;
/* TODO (root#1#): Journals */
	struct deal{
		int iOrderTicket;
		int iOrderType;
		int iOrderProfit;
		int iOrderOpenTime;
		int iOrderCloseTime;
		double iOpenPrice;
		double iOrderOpenPrice;
		double iOrderClosePrice;
		double iOrderStopLoss;
		double iOrderTakeProfit;
		int iOrderOpclose;
	};
	int curorderticket;
	int totalorders;
	int iTradesTotal;
	int iTradesCurrent;
	int iTradesTotalOP_BUY;
	int iTradesTotalOP_SELL;
	int iTradesTotalOP_BUYLIMIT;
	int iTradesTotalOP_SELLLIMIT;
	int iTradesTotalOP_BUYSTOP;
	int iTradesTotalOP_SELLSTOP;
	int iHistoryTotal;
	int iHistoryCurrent;
	int minprofit;
	int maxprofit;
	int minprofitcntmax;
	int maxprofitcntmax;
	bool prevorderprofitorno;
	int drawdowncntmax;
	int profitcntmax;
	int drawdowncnt;
	int profitcnt;
	int sorderscnt;
	int lorderscnt;
	int totalprofit;

	int curOrderTicket;
	int curOrderType;
	int curOrderProfit;
	int curOrderOpenTime;
	int curOrderCloseTime;
	double curOpenPrice;
	double curOrderOpenPrice;
	double curOrderClosePrice;
	double curOrderStopLoss;
	double curOrderTakeProfit;
	int curOrderOpclose;

	deal *trades;
	deal *history;

	void journalsinit(bool init=true);
	void add_row_history(double tOrderPrice, int opclose);
	void add_row_trades(short tCmd,double tPrice,double tStop,double tTake);
	void del_row_trades(double tOrderPrice, short opclose);
	void OrderSelect(int tIndex, int selmode);
	void OrderModify(double tOrderPrice,double tStop,double tTake);
	void OrderSend(int cmd, double price, double stop, double take);
	void OrderClose(int OrderTicket, double tOrderPrice, int opclose);
	void OrderDelete(int OrderTicket);

	void iOrdersControl();
/* TODO (root#1#): Tester */
	struct optimizationvalparams{
		short int param1;
		short int param2;
		short int param3;
		short int param4;
		short int param5;
		short int param6;
		short int param7;
		int datetimeopt;
	};
	struct optimizationvals{
        optimizationvalparams params[4];
/*		int datetimestart;
		int datetimeend;

		int totalprofit;
		int totalorders;
		double totalprofitindex;
		int totaldrawdown;*/
	};
	struct mdata{
		time_t  ctm[10000];//time_t
		double open[10000];
		double low[10000];
		double high[10000];
		double close[10000];
		double  volume[10000];
	};
	char testerpath[255];
	char strategyset[255];
	char datfile[255];
	mdata* testermetadata;
	char testersmas[36][22];
	char testervals[255][22];
	optimizationvals* testeroptval;

	int testerperiod,testerspread;//,stopavg1b,stopavg1s,stopavg2b,stopavg2s;
	int stopavg1b[1201],stopavg1s[1201],stopavg2b[1201],stopavg2s[1201],spreadtp[1201];
	int testervalcnt,testersmacnt,testercuritem,testercursma;
	bool testerfxok,testerdataok;

	double topen,topen1,thigh,tlow,tclose;double tvolume;

	int testerdigits,testercntper,tester2point;
	double testerpoint;

	double testercurh,testercuro,testercurl,testercurc;
	int testercurbar;
	time_t testercurdatetime;

	int testermincnttrades,testermdrawdownclimit;
	int randptr,randcnt,randcnt2,randbytes[65536];
	void initrandbytes();
	int getrand();

	double mpage[33][1201][257][8];
	bool mpageis[33][1201][257][8];
	void mpage_update();

	int testertest(int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8,int p9);
	void testerstart(int k1,int d1,int k2,int d2,int k3,int d3,int l1,int l2,int limit);
	double testersignal(int k1,int d1,int k2,int d2,int k3,int d3,int l1,int l2,int limit);
	double sma(const int period, const int price, const int shift );
	double isma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double cci(const int period, const int shift );
	double icci(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double atr(const int period, const int shift );
	double iatr(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double rsi(const int period, const int price, const int shift );
	double irsi(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double ichimoku(const int period, const int shift );
	double iichimoku(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double momentum(const int period, const int price, const int shift );
	double imomentum(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double ema(const int period, const int price, const int shift );
	double iema(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double bandsup(const int period, const int price, const int shift );
	double ibandsup(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double bandsdn(const int period, const int price, const int shift );
	double ibandsdn(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double bears(const int period, const int price, const int shift );
	double ibears(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double bulls(const int period, const int price, const int shift );
	double ibulls(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double osma(const int period, const int price, const int shift );
	double iosma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double demarker(const int period, const int price, const int shift );
	double idemarker(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double demarker2(const int period, const int price, const int shift );
	double idemarker2(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double stddev(const int period, const int price, const int shift );
	double istddev(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double stochastic(const int period, const int price, const int shift );
	double istochastic(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double stochastic2(const int period, const int price, const int shift );
	double istochastic2(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double force(const int period, const int price, const int shift );
	double iforce(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double ac(const int period, const int price, const int shift );
	double iac(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double adx(const int period, const int price, const int aprice, const int shift );
	double iadx(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double iadxmain(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double iadxup(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double iadxdn(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double adx2(const int period, const int price, const int aprice, const int shift );
	double iadx2(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double gma(const int period, const int price, const int shift );
	double igma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double tma(const int period, const int price, const int shift );
	double itma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double sinema(const int period, const int price, const int shift );
	double isinema(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double randma(const int period, const int price, const int shift, const double rma );
	double irandma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double zerolagema(const int period, const int price, const int shift );
	double izerolagema(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double lssma(const int period, const int price, const int shift );
	double ilssma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double lwma(const int period, const int price, const int shift );
	double ilwma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double volumesma(const int period, const int price, const int shift );
	double ivolumesma(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double rvi(const int period, const int price, const int shift );
	double irvi(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double rvi2(const int period, const int price, const int shift );
	double irvi2(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);
	double wpr(const int period, const int shift );
	double iwpr(const int k1, const int d1, const int k2, const int d2, const int k3, const int d3, int l1, int l2, const int il);

	double GetAppliedPrice(const int nAppliedPrice, const int nIndex);
    int iLowest(int count, int start);
    int iHighest(int count, int start);


	void testerinit();
	void testerloaddata();
	void testerusefx();
	void testersavefx();
	int testerbacktest;
	int actmode;
/* TODO (root#1#): Optimizer */
	struct consolidatesorted{
		char val[22];
		int digits;
		int datetime;
		double priceopen;
		double priceclose;
		double pricehigh;
		double pricelow;
		double midBUYSTOP;
		int cntBUYSTOP;
		int cntBUYSTOPtotal;
		int cntBUYSTOP2total;
		double midSELLSTOP;
		int cntSELLSTOP;
		int cntSELLSTOPtotal;
		int cntSELLSTOP2total;
		double powerBUYSTOP;
		double powerSELLSTOP;
		double profitBUYSTOP;
		double profitSELLSTOP;
		double profitBUYSTOP2;
		double profitSELLSTOP2;
		int cntBUYSTOP2;
		int cntSELLSTOP2;
	};

	consolidatesorted* unsorted;

	int datetimemin,datetimemax;
	bool tradecurbar;
	int optcurbuysell,kperiod,kperiod1,cpuse,wsleep;

	void test();
	void test2();
	void optimize();
/* TODO (root#1#): Debug */
    void showlog();
	void debug();

	public:
		Otskok();
		void action(int t,int tperiod,int historymode,bool tcurbar,int tbackbar);
		virtual ~Otskok();

};

#endif // OTSKOK_H
