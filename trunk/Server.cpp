#ifndef SERVER_CPP
#define SERVER_CPP
#include <windows.h>
#include <winsock2.h>
#include <process.h>
#include <fcntl.h> 
#include <time.h>
#include <fstream>
#include <string>
#include <new>
#include "main.h"
#include "Server.h"
#include "otskok.h"

using namespace std;

extern HWND hwnd,hlog,hpro,hper,hcmd;
extern void *Mmalloc(size_t size);
extern char *Mstrdup(const char *s);
extern void *Mrealloc(void* ptr, size_t size);
extern void Mfree(void *ptr);
extern void wlog(const char* buffer);
extern void title(int h,const char* buffer);
extern void wcmd(int reqestor);
extern Server* server;


static DWORD WINAPI TimersThreadProc(void *inf){
	while(WaitForSingleObject(server->timersThread,200) == WAIT_TIMEOUT)
	{
		server->s();ReleaseMutex(server->timersThread);
	}
	return 0;
}
static DWORD WINAPI DataTransfer(void *inf){
	while(WaitForSingleObject(server->timersDataTransfer,4) == WAIT_TIMEOUT)
	{
        int len,maxlen;
        time_t t0=time(0);
        if(server->mutex2)
		{
				for(int i=0;i<server->counterRequests;i++){
					closesocket(server->requests[i].sock);
					shutdown(server->requests[i].sock,1);
					server->requests[i].bufio=server->DONE;
				//				        wlog(server->timeToStr(server->requests[i].datetime));
				//				        wlog(inet_ntoa(server->requests[i].sin.sin_addr));
				//						wlog(" ..\r\n");
					memcpy(server->requests+server->reqsize*i,server->requests+server->reqsize*(i+1),server->reqsize*server->counterRequests-server->reqsize*(i+1));
					server->counterRequests--;
				}
		}else
        if(!server->mutex2)
		for(int i=0;i<server->counterRequests;i++){
			if((t0-server->requests[i].datetime)>2){
				//closesocket(server->requests[i].sock);
				shutdown(server->requests[i].sock,1);
				server->requests[i].bufio=server->DONE;
			    wlog(server->timeToStr(server->requests[i].datetime));
			    wlog(inet_ntoa(server->requests[i].sin.sin_addr));
				wlog(" timeout 2 secs\r\n");
				memcpy(server->requests+server->reqsize*i,server->requests+server->reqsize*(i+1),server->reqsize*server->counterRequests-server->reqsize*(i+1));
				server->counterRequests--;
			}
		 if(server->requests[i].bufio==server->INPUT){
		     maxlen=2000-server->requests[i].currPtr;
             len = recv(server->requests[i].sock, (char *)(server->requests[i].inbuf+server->requests[i].currPtr), maxlen, 0);
			 if(len==0)server->requests[i].currPtr+=len;
			   else {
                   server->requests[i].bufio=server->OUTPUT;
                   server->requests[i].currPtr=0;
                   if(!strcmp(server->substr((const char *)server->requests[i].inbuf,5,1)," ")){server->requests[i].bufid=server->MAINPAGE;server->requests[i].bufio=server->OUTPUT;} else
                   if(!strcmp(server->substr((const char *)server->requests[i].inbuf,5,6),"otskok")){server->requests[i].bufid=server->PENDING_ORDERS;server->requests[i].bufio=server->OUTPUT;} else
                   if(!strcmp(server->substr((const char *)server->requests[i].inbuf,5,6),"impuls")){server->requests[i].bufid=server->PENDING_ORDERS_IM;server->requests[i].bufio=server->OUTPUT;} else
                   {
						//closesocket(server->requests[i].sock);
                        shutdown(server->requests[i].sock,1);
						server->requests[i].bufio=server->DONE;
				        wlog(server->timeToStr(server->requests[i].datetime));
				        wlog(inet_ntoa(server->requests[i].sin.sin_addr));
						wlog(" ..\r\n");
						memcpy(server->requests+server->reqsize*i,server->requests+server->reqsize*(i+1),server->reqsize*server->counterRequests-server->reqsize*(i+1));
						server->counterRequests--;
			   }   }
         }//else
		 if(server->requests[i].bufio==server->OUTPUT){
//         wlog("server->OUTPUT\r\n");
             if(server->requests[i].bufid==server->MAINPAGE){
			     maxlen=server->mainpageSize-server->requests[i].currPtr;
	             len = send(server->requests[i].sock, (const char *)(server->mainpage+server->requests[i].currPtr), maxlen, 0);
				 if(len==0)server->requests[i].currPtr+=len;
				   else {
						//closesocket(server->requests[i].sock);
						shutdown(server->requests[i].sock,1);
				   		server->requests[i].bufio=server->DONE;
				        wlog(server->timeToStr(server->requests[i].datetime));
				        wlog(inet_ntoa(server->requests[i].sin.sin_addr));
						wlog(" mainpage\r\n");
						memcpy(server->requests+server->reqsize*i,server->requests+server->reqsize*(i+1),server->reqsize*server->counterRequests-server->reqsize*(i+1));
				        server->counterRequests--;
				   }
			 } else
             if(server->requests[i].bufid==server->PENDING_ORDERS){
			     maxlen=server->pending_ordersSize-server->requests[i].currPtr;
	             len = send(server->requests[i].sock, (const char *)(server->pending_orders+server->requests[i].currPtr), maxlen, 0);
				 if(len==0)server->requests[i].currPtr+=len;
				   else {
						//closesocket(server->requests[i].sock);
                        shutdown(server->requests[i].sock,1);
						server->requests[i].bufio=server->DONE;
				        wlog(server->timeToStr(server->requests[i].datetime));
						wlog(inet_ntoa(server->requests[i].sin.sin_addr));
						wlog(" otskok\r\n");
						memcpy(server->requests+server->reqsize*i,server->requests+server->reqsize*(i+1),server->reqsize*server->counterRequests-server->reqsize*(i+1));
				        server->counterRequests--;
				   }
			 } else
             if(server->requests[i].bufid==server->PENDING_ORDERS_IM){
			     maxlen=server->pending_orders_imSize-server->requests[i].currPtr;
	             len = send(server->requests[i].sock, (const char *)(server->pending_orders_im+server->requests[i].currPtr), maxlen, 0);
				 if(len==0)server->requests[i].currPtr+=len;
				   else {
						//closesocket(server->requests[i].sock);
						shutdown(server->requests[i].sock,1);
						server->requests[i].bufio=server->DONE;
				        wlog(server->timeToStr(server->requests[i].datetime));
				        wlog(inet_ntoa(server->requests[i].sin.sin_addr));
						wlog(" impuls\r\n");
						memcpy(server->requests+server->reqsize*i,server->requests+server->reqsize*(i+1),server->reqsize*server->counterRequests-server->reqsize*(i+1));
				        server->counterRequests--;
				   }
			 }
         }
         //ReleaseMutex(server->timersThreadChild);
		 //TerminateThread(server->timersThreadChild,1);
		 //TerminateThread(GetCurrentThread(),0);
		 //ExitThread(0);
		 //CloseHandle(server->timersThreadChild);
		}
	}
	return 0;
}
void Server::s(){
	if(!mutex)
	{
		mutex=true;
		tm tm1,tm2;
		t1=time(0);
		char buf[1024];
		memset(&tm1,0,sizeof(struct tm));tm1=*localtime((const time_t *)&t1);
		memset(&tm2,0,sizeof(struct tm));tm2=*localtime((const time_t *)&prevbuildtime);

		if(counterRequests_prev!=counterRequests||curtime_prev!=curtime){
			memset(buf,0,1024);
			strftime(buf,80,"%H:%M:%S ",&tm1);lstrcat(buf,"   ");lstrcat(buf,intToStr(counterRequests));
			SetWindowText(hper,buf);
		}counterRequests_prev=counterRequests;curtime_prev=curtime;
	
	    curtime=t1;
	    //if(((curtime-prevbuildtime)>3600)||(tm1.tm_hour!=tm2.tm_hour&&tm1.tm_min==1))
		//if(((curtime-prevbuildtime)>900)||(((curtime-prevbuildtime)>900)&&(tm1.tm_min==1||tm1.tm_min==16||tm1.tm_min==31||tm1.tm_min==46))){server->otskok(testing);prevbuildtime=curtime;}
		//if(((curtime-prevbuildtime_im)>900)||(((curtime-prevbuildtime_im)>900)&&(tm1.tm_min==1||tm1.tm_min==16||tm1.tm_min==31||tm1.tm_min==46))){server->impuls(testing);prevbuildtime_im=curtime;}

		int addr_length=sizeof(sin);
		bool needsend=true;
		sock2 = accept(sock,(sockaddr*)&sintmp,&addr_length);
		if (sock2!=INVALID_SOCKET)
		{
SuspendThread(server->timersDataTransfer);
			memset(buf,0,1024);
			lstrcat(buf,"<br><center>server:");
			lstrcat(buf,timeToStr());
			lstrcat(buf,"<table cellpadding=0 cellspacing=0 border=0><tr><td><a href=/otskok>otskok ");
/*			lstrcat(buf,intToStr(config->period));
			lstrcat(buf,"</a> </td><td><b>");
			lstrcat(buf,timeToStr(config->op_time_pending_orders));
			lstrcat(buf,"</b><br>");
			lstrcat(buf,timeToStr(config->time_pending_orders));
			lstrcat(buf,"</td></tr><tr><td><a href=/impuls>impuls ");
			lstrcat(buf,intToStr(config->period_im));
			lstrcat(buf,"</a> </td><td><b>");
			lstrcat(buf,timeToStr(config->op_time_pending_orders_im));
			lstrcat(buf,"</b><br>");
			lstrcat(buf,timeToStr(config->time_pending_orders_im));*/
			lstrcat(buf,"</td></tr></table></center>");
			memcpy(mainpage,buf,strlen(buf));mainpageSize=strlen(buf);

	        counterRequests++;
	        requests = (struct Requests*)Mrealloc(requests,sizeof(struct Requests)*counterRequests);
	        requests[counterRequests-1].sock=sock2;
	        requests[counterRequests-1].currPtr=0;
	        requests[counterRequests-1].bufio=INPUT;
	        requests[counterRequests-1].datetime=t1;
	        memset(&requests[counterRequests-1].inbuf[0],0,2000);
			memcpy(&requests[counterRequests-1].sin,&sintmp,sizeof(SOCKADDR_IN));
ResumeThread(server->timersDataTransfer);
//          wlog("accept\r\n");
		}
		mutex=false;
	}
}

Server::Server(){
    //wsd = new WSADATA;
	WSAStartup(0x0101, &wsd);

}
Server::~Server(){
	TerminateThread(timersThread,0);
	TerminateThread(timersDataTransfer,0);
	closesocket(sock);
	delete[] this->requests;
	WSACleanup();
}
void  Server::otskok(int t){
//		bool f=false;
//		if(!f)throw SocketException ( "Could not create Otskok." );

Otskok* otskokobj;
	for(int i=1;i<2;i++){
//		mutex=true;this->mutex2=true;
		otskokobj=new Otskok;
		//Otskok* otskokobj = (Otskok*)Mmalloc(sizeof(class Otskok));memset(otskokobj,0,sizeof(class Otskok));
		//memset(otskokobj,0,sizeof(class Otskok));
		//try{
			otskokobj->Otskok2(t);//}catch(SocketException &e){throw e;}
		//
		delete otskokobj;
//		mutex=false;this->mutex2=false;
		}
}
void  Server::impuls(int t){
		Impuls* impulsobj;
		impulsobj=new Impuls;
		impulsobj->Impuls2(t);
		delete impulsobj;
}

void  Server::on(){

	//this->mainpage=new uint8_t[16000];//(uint8_t)malloc(16000);
	memcpy(this->mainpage,"mainpage",8);mainpageSize=8;
    memcpy(this->pending_orders,"pending_orders",14);pending_ordersSize=14;
    memcpy(this->pending_orders_im,"pending_orders_im",17);pending_orders_imSize=17;

	reqsize = sizeof(struct Requests);
		////
	//requests = (struct Requests*)malloc(sizeof(struct Requests)*3);
		this->requests = new Requests[2];
    //memset(&this->requests,0,sizeof(struct Requests)*2);
    counterRequests=0;


	mutex=false;this->mutex2=false;
	prevbuildtime_im=time(0)-8901;
	prevbuildtime=time(0)-3601;
	prevopttime=0;
	prevopttime_im=0;
	while(WSAIsBlocking());
	memset(&sin,0,sizeof(sin));

	sock=socket(PF_INET,SOCK_STREAM,0);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("127.0.0.1");//INADDR_ANY;
	sin.sin_family = PF_INET;
	sin.sin_port = htons(80);
	int on1=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char*)&on1,sizeof(int));
	//memset(&(sin.sin_zero),'\0',8);
	int bind_return = ::bind (sock,(struct sockaddr*)&sin,sizeof(struct sockaddr));
	u_long u=1;
	ioctlsocket(sock,FIONBIO,&u);

	int listen_return =::listen(sock, 650);

	timersThread = CreateThread(NULL,0,TimersThreadProc,0,0,0);
	if(timersThread==NULL)wlog("# timersThread error\r\n");
	  else wlog("# timersThread created\r\n");
    timersDataTransfer = CreateThread(NULL,0,DataTransfer,0,0,0);
	if(timersDataTransfer==NULL)wlog("# timersDataTransfer error\r\n");
	  else wlog("# timersDataTransfer created\r\n");
	  ResumeThread(timersThread);
	  ResumeThread(timersDataTransfer);

}

inline char* Server::intToStr(int i){
	static char str1[18]="";memset(str1,0,18);
	snprintf(str1, 17, "%lld", i);

	return (char *)str1;
}
inline bool Server::strToInt(const string &s, int *i){
	char *endChar;
	setlocale(LC_NUMERIC, "C");
	*i= strtol(s.c_str(), &endChar, 10);
	if(*endChar!='\0')return false;

	return true;
}
inline char* Server::doubleToStr(double d,int precsion) {
	static char str[18]="";
	memset(str,0,18);
	snprintf(str, 12, "%.*f", precsion,d);

	return (char *)str;
}
inline char* Server::timeToStr(const time_t st) {
	static char strtime[28]="";
    static tm stm1;
    static time_t st1;
    st1=time(0);
	memset(strtime,0,28);
	memset(&stm1,0,sizeof(struct tm));
	if(st==0)
	stm1=*localtime((const time_t *)&st1);
	else
	stm1=*localtime((const time_t *)&st);
	strftime(strtime,27,"%d.%m.%Y %H:%M:%S ",&stm1);
	return (char *)strtime;
}
inline int Server::rdtsc(){__asm__ __volatile__("rdtsc");}
inline char* Server::substr(const char* S, const int pos, const int len){
	int i;
	char *SubS;
	if ( pos+len <= strlen(S)){
		SubS = (char *)Mmalloc(len+1);
		for (i=0;i<len;i++) SubS[i] = S[pos+i];
		SubS[len] = (char) NULL;
	} else SubS = NULL;

	return (char *)SubS;
}
inline int Server::find(const char *s, const char *key)
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
#endif
