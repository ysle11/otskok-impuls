#ifndef SERVER_H
#define SERVER_H
using std::string;
using std::ofstream;
using std::ios;
#include "otskok.h"
#include "impuls.h"

extern void *Mmalloc(size_t size);
extern char *Mstrdup(const char *s);
extern void *Mrealloc(void* ptr, size_t size);
extern void Mfree(void *ptr);

class Server{
protected:
		 char* intToStr(int i);
		 bool strToInt(const string &s, int *i);
		 char* doubleToStr(double d,int precsion);
		 int rdtsc();
		 int find(const char *s, const char *key);
private:
	struct Requests{
		SOCKET sock;
		SOCKADDR_IN sin;
		time_t datetime;
		int currPtr;
		short bufid;
		short bufio;
	    uint8_t inbuf[2000];
	};

	SOCKET sock,sock2;
	WSADATA wsd;
	SOCKADDR_IN sin,sintmp;
	bool mutex;
	time_t t1;
	int prevbuildtime,prevbuildtime_im,curtime;
	int curtime_prev,counterRequests_prev;
	void rebuild_pending_orders();
	void rebuild_pending_orders_im();
public:
	HANDLE hMutex;
	Requests* requests;
		 char* timeToStr(const time_t st=0);
  		 char* substr(const char* S, const int pos, const int len);
	int reqsize;
	int counterRequests;
	int counterRequestsMax;
	HANDLE timersThread,timersDataTransfer;
	bool mutex2;

	uint8_t mainpage[16000];
	uint8_t pending_orders[16000];
	uint8_t pending_orders_im[16000];
	int mainpageSize;
	int pending_ordersSize;
	int pending_orders_imSize;
	int prevopttime,prevopttime_im;

public:
	void otskok(int t,int tperiod);
	void impuls(int t,int tperiod);
    enum { MAINPAGE = 1, PENDING_ORDERS, PENDING_ORDERS_IM, INPUT, OUTPUT, DONE };
	void s();
	Server();
	~Server();
	void on(bool up=true);
};
#endif
