#ifndef IMPULS_H
#define IMPULS_H

class Impuls
{
	char* intToStr(int i);
	bool strToInt(const char *s, int *i);
	int strToInt(const char *s);
	char* doubleToStr(double d,int precsion);
	int rdtsc();
	int find(const char *s, const char *key);
	char* substr(const char* S, const int pos, const int len);
	char* timeToStr(const time_t st=0);
	char* gmtimeToStr(const time_t st=0);
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

	time_t* OrderOpenTime();
	void iOrdersControl();
/* TODO (root#1#): Tester */
	struct Engine{
		    int sr[32];
		    int sd[4096][3];
		    int nsd[4096][3];
		    int LastSd[4096][3];
		 double cen[4096][3];
		 double rost[4096][3];
		 double spad[4096][3];
		 double P[32];
		 double pt;
		    int delt;
	};
	Engine *engine;
	Engine *engine2;
	struct optimizationconfig{
		char path[255];
		int period;
		int cntpervoid;
		int dtime;
		int ltime;
		int quant;
		char smas[255][22];
		char vals[255][22];
		int valstops[255];
		int valspreads[255];
		int valcnt;
		int smacnt;
	};
	struct optimizationvals{
		short int param1;
		short int param2;
		short int param3;
		short int param4;
		short int param5;
		short int param6;
		short int param7;
		short int param8;
		char stoporlimit;
		int datetimeopt;
		int datetimestart;
		int datetimeend;

		int totalprofit;
		int totalorders;
		int totalprofitindex;
		int totaldrawdown;
	};

	struct mdata{
		time_t ctm[12000];
		double open[12000];
		double low[12000];
		double high[12000];
		double close[12000];
		double volume[12000];
	};
	optimizationconfig* testeroptcnf;
	optimizationvals* testeroptval;
	mdata* testermetadata;
	char testersmas[255][22];
	char testervals[255][22];
	int testervalstops[255];
	int testervalspreads[255];
	char testercurval[255];
	int testercurperiod;
    double FDlast_sell,FDlast_buy,prob;int LastSellOpen,LastBuyOpen;
	struct optimized_info{
		time_t datefrom;
		time_t dateto;
		time_t dateopt;
		   int period_im;
		double p1;
		   int p2;
		   int p3;
		   int p4;
		   int p5;
		   int p6;
		   int p7;
		double p8;
	};
    optimized_info* testerop_info;
    bool testerimpulsok;

	void testerinit();
	void testerloaddata();
	void testerstart(double k1,int d1,int k2,int d2,int k3,int d3,int l1,double l2,int limit);
	double testersignal(double k1,int d1,int k2,int d2,int k3,int d3,int l1,double l2,int limit);
	bool testerquant_();
	bool testerNewBar(int op);
	void testercloseall(int a);
	void testercontrol();
//	void logresults();
	int testertest(double p1,int p2,int p3,int p4,int p5,int p6,int p7,double p8,int p9);
	void testersaveimpuls(bool limitstop=false);
	void testerloadimpuls(bool limitstop=false);
	void testerrestoreimpuls();
    void testerreset_im(bool a=false);

	double testertargetprofit,testerpoint;
	int testerspread,testerstop;
	time_t testercurdatetime;
	double testercurh,testercuro,testercurl,testercurc,testercurprice;
	int testerperiod,testercntpervoid,testerdtime,testerltime,testerquant;
	int testervalcnt,testercuritem,testersmacnt,testercursma;
	char testerpath[255];
	char strategyset[255];
	char datfile[255];
	int testertestfrom,testertestto;
	bool testerfxok,testerdataok;
	int testercurbar;
	time_t testertime_pending_orders;

	int testerconsolidationbars,testercbars,testerdigits,testercntper;
	bool testerisoptimize;
	int testermincnttrades,testermdrawdownclimit;
	void testerusefx();

/* TODO (root#1#): Consolidating */

		struct consolidatesorted{
			char val[22];
			int digits;
			int datetime;
			double priceopen;
			double targetprofit;
			double highSELLIMIT;
			double midSELLIMIT;
			int cntSELLIMIT;
			double lowSELLSTOP;
			double midSELLSTOP;
			int cntSELLSTOP;
			double highBUYSTOP;
			double midBUYSTOP;
			int cntBUYSTOP;
			double lowBUYLIMIT;
			double midBUYLIMIT;
			int cntBUYLIMIT;
			double lowSELL;
			double midSELL;
			int cntSELL;
			double lowBUY;
			double midBUY;
			int cntBUY;
		};
    	consolidatesorted* csorted;
		void test();
		void optimize();
    	void debug();
	public:
		Impuls();
		~Impuls();
		void Impuls2(int t);
};

#endif // IMPULS_H
