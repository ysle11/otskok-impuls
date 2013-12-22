#include <windows.h>
//#include <commctrl.h>
#include <windowsx.h>
#include <iostream>
//#include <new>
#include "main.h"
//#include "Server.h"
//#include "mysql/mysql.h"
#include "otskok.h"

//Server* server;
//int mysqltest();
char* globallog;int globallogsize;
inline int rdtsc(){__asm__ __volatile__("rdtsc");}
char* intToStr(int i);
int find(const char *s, const char *key);
int find2(const char *s, const char *key);
void decode(int act,int tperiod,int actmode,bool tradecurbar);
void wcmd(int reqestor);
void wlog(const char* buffer);
void wlogsave();
void title(int h,const char* buffer);
void* Mmalloc(size_t size);
char* Mstrdup(const char *s);
void* Mrealloc(void* ptr, size_t size);
void Mfree(void *ptr);
WNDCLASSEX wincl;
NOTIFYICONDATA note;
char szClassName[]="patterns";
HWND hwnd,hlog,hpro,hper,hcmd;
int action,period,mode;
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszArgument,int nFunsterStil)
{
	MSG messages;
	globallog = new char[1];globallog[0]=32;
	globallogsize=1;
//	globallog = (char*)Mrealloc(2);
	
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof (WNDCLASSEX);
	
	wincl.hIcon = LoadIcon( hThisInstance, "A" );
	wincl.hIconSm = wincl.hIcon;
	wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
	
	
	
	if (!RegisterClassEx (&wincl)) return 0;
	RECT lp;
	GetWindowRect(GetDesktopWindow(),&lp);
	int w=GetSystemMetrics(SM_CXSCREEN);
	int h=GetSystemMetrics(SM_CYSCREEN);
	hwnd =CreateWindowEx (0,szClassName,"patterns",WS_CAPTION|WS_MINIMIZEBOX|WS_VISIBLE|WS_SYSMENU,lp.right-414,0,414,675,HWND_DESKTOP,NULL,hThisInstance,NULL);//CW_USEDEFAULT
	hlog =CreateWindowEx (0,"Edit","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_NOHIDESEL|WS_VSCROLL,0,0,438,630,hwnd,NULL,hThisInstance,NULL);
//	hcmd =CreateWindowEx (0,"Edit","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_NOHIDESEL|WS_VSCROLL,0,310,538,20,hwnd,NULL,hThisInstance,NULL);
//	hcmd =CreateWindowEx (WS_EX_CLIENTEDGE,"ListBox","",WS_CHILD|WS_VISIBLE|WS_VSCROLL|LVS_REPORT | LVS_SHAREIMAGELISTS,0,260,538,70,hwnd,NULL,hThisInstance,NULL);
	hpro =CreateWindowEx (0,"Static"," ",WS_CHILD | WS_VISIBLE,0,630,410,35,hwnd,NULL,hThisInstance,NULL);
	hper =CreateWindowEx (0,"Static"," ",WS_CHILD | WS_VISIBLE,410,630,30,35,hwnd,NULL,hThisInstance,NULL);
/*    SendMessage( hcmd, LVM_FIRST+54, 0, 32 | 16 | 2 | 1);

	ListView_SetTextColor(hcmd,0x00000000);
	SendMessage( hcmd, LVM_SETTEXTBKCOLOR, 0, 0x00ffffef);
	ListView_SetBkColor(hcmd,0xffffef);
	ListBox_InsertString(hcmd,0,"MMCIS-Real");
    ListBox_InsertString(hcmd,0,"MMCIS-Demo");
	LV_ITEM lvi;//memset(&lvi,0,sizeof(LV_ITEM));
LV_COLUMN lvc;
lvc.pszText="dfg";
lvc.cx=44;
lvc.iSubItem=0;
ListView_InsertColumn(hcmd,1,&lvc);
	ListBox_SetColumnWidth(hcmd,41);
	lvi.cchTextMax=22;
	lvi.iItem=0;
	lvi.lParam=0;
	lvi.iSubItem=1;
	lvi.mask=LVIF_PARAM;
	lstrcat(lvi.pszText," subitem ");
	//ListBox_InsertItemData(hcmd,0,&lvi);
	SendMessage(hcmd, LVM_SETITEM, 0, (LPARAM)&lvi);
//	lvi.mask=LVIF_TEXT;
	lvi.cchTextMax=12;
	lvi.iItem=0;
	lvi.lParam=1;
	lvi.iSubItem=1;
//	lstrcat(lvi.pszText," subitem ");
	ListBox_InsertItemData(hcmd,0,&lvi);
	SendMessage(hcmd, LVM_SETTEXTCOLOR, 0, (LPARAM)(COLORREF)0xff0f00ff);
	ListView_Update(hcmd,0);
	ListView_RedrawItems(hcmd,0,3);
*/
//mysqltest();
	ShowWindow (hwnd, nFunsterStil);
	UpdateWindow(hwnd);
	srand(time(0));
    if(find(lpszArgument,"/opt"))action=optimizing;else
    if(find(lpszArgument,"/test"))action=testing;else
    if(find(lpszArgument,"/debug"))action=debuging;

    if(find(lpszArgument,"/1t"))period=1;else
    if(find(lpszArgument,"/5t"))period=5;else
    if(find(lpszArgument,"/15t"))period=15;else
    if(find(lpszArgument,"/60t"))period=60;else
    if(find(lpszArgument,"/240t"))period=240;else
    if(find(lpszArgument,"/1440t"))period=1440;else
    if(find(lpszArgument,"/10080t"))period=10080;else
    if(find(lpszArgument,"/43200t"))period=43200;

	mode=999;
	if(find2(lpszArgument,"/MMCIS-Demo"))mode=light;else
    if(find2(lpszArgument,"/MMCIS-Real"))mode=medium;else
    if(find2(lpszArgument,"/InstaForex-Demo.com"))mode=hard;

//	server = new Server;
//    if(action==optimizing)server->on(false);else server->on();

    if(mode==999)wlog("Optimization: patterns.exe /MMCIS-Demo /opt /1440t\r\nTesting: patterns.exe /MMCIS-Demo /test /1440t\r\nDebuging: patterns.exe /MMCIS-Demo /debug /1440t\r\n\r\naction: /opt,/test,/debug\r\nperiod: /1t,/5t,/15t,/60t,/240t,/1440t,/10080t,/43200t\r\nmode: /MMCIS-Demo,/MMCIS-Real,/InstaForex-Demo.com\r\n\r\nExample: f:\\Program Files\\MMCIS MetaTrader 4 Client Terminal\\patterns.exe /MMCIS-Demo /opt /1440t\r\n");
	else if(action!=optimizing)decode(action,period,mode,donottradecurrentbar);else decode(action,period,mode,tradecurrentbar);
    
    if(!find(lpszArgument,"/quit"))
	while (GetMessage (&messages, NULL, 0, 0))
	{if((messages.hwnd==hcmd)&&(messages.message==WM_KEYUP)&&messages.wParam==VK_RETURN)wcmd(cmdmain);
	if((messages.hwnd==hcmd)&&(messages.message==WM_LBUTTONDBLCLK))wlog(intToStr(ListBox_GetCurSel(hcmd) ));
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}wlogsave();

	delete[] globallog;
	return messages.wParam;
}
LRESULT CALLBACK WindowProcedure (HWND hwnd1, UINT message, WPARAM wParam, LPARAM lParam)
{
switch (message)
{
	case WM_DESTROY:
		{
//			delete server;
			wlogsave();
			PostQuitMessage (0);
		}
	break;
	case WM_SIZE:
		if(wParam==SIZE_MINIMIZED){
			memset(&note,0,sizeof(note));
			note.cbSize=sizeof(NOTIFYICONDATA);
			note.hWnd=hwnd1;
			note.uID=0;
			note.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
			note.hIcon=LoadIcon( wincl.hInstance, "A1" );
			note.uCallbackMessage=WM_USER+5;
			lstrcpy(note.szTip,szClassName);
			ShowWindow(hwnd1,SW_HIDE);
			Shell_NotifyIcon(NIM_ADD,&note);
		}
	break;
	case WM_USER+5:
		if(lParam==WM_LBUTTONDBLCLK){
			note.hIcon=wincl.hIcon;
			Shell_NotifyIcon(NIM_DELETE,&note);
			ShowWindow(hwnd1,SW_RESTORE);
			SetForegroundWindow(hwnd1);
		}
	break;
	default:
	return DefWindowProc (hwnd1, message, wParam, lParam);
}
return 0;
}
void decode(int act,int tperiod,int actmode,bool tradecurbar){
	Otskok* otskokobj;
	otskokobj=new Otskok;
	otskokobj->action(act,tperiod,actmode,tradecurbar);
	delete otskokobj;
}
void wcmd(int reqestor){
		if(reqestor==cmdmain)wlog("\r\n main enter\r\n");else
		if(reqestor==cmdotskok)wlog("\r\n otskok enter\r\n");else
		if(reqestor==cmdimpuls)wlog("\r\n impuls enter\r\n");
}
void wlog(const char* buffer){
		char* buf;
	    int hloglen=GetWindowTextLength(hlog)+1;
	    int slen=strlen(buffer)+1;
		if(slen<30000){
		        buf= new char[2];//hloglen+slen+1];
		        buf = (char*)Mrealloc(buf,hloglen+slen+1);
				memset(buf,0,hloglen+slen+1);
				if((hloglen+slen)<30000)GetWindowText(hlog,buf,hloglen);
				else{
					char* tmpbuf=new char[2];tmpbuf = (char*)Mrealloc(tmpbuf,hloglen+1);GetWindowText(hlog,&tmpbuf[0],hloglen);
					//int oldsize;oldsize=sizeof(globallog);
					globallog = (char*)Mrealloc(globallog,hloglen+globallogsize+1);
					memcpy(&globallog[globallogsize],tmpbuf,hloglen-1);globallogsize+=hloglen;
				}
				lstrcat(buf,buffer);
				SetWindowText(hlog, buf);
				Edit_Scroll(hlog,Edit_GetLineCount(hlog),0);
				int wlen=GetWindowTextLength(hlog);
				Edit_SetSel(hlog,wlen-1,wlen);
				Edit_SetSel(hlog,wlen,wlen);
				SetFocus(hlog);
				ShowCaret(hlog);
				delete[] buf;
		}
}
void wlogsave(){
    DWORD result;
    HANDLE hFile = CreateFile("patterns.log", GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    int i=GetFileSize(hFile, NULL);
    SetFilePointer(hFile,i,NULL,FILE_BEGIN);

	char* tmp; tmp=(char*)Mmalloc(3255);memset(tmp,0,3255);
	char* tmp2; tmp2=(char*)Mmalloc(125);memset(tmp2,0,125);
	time_t t1=time(0);
	tm tm1;memset(&tm1,0,sizeof(struct tm));tm1 = *localtime((const time_t *)&t1);
	strftime(tmp2,80,"%d.%m.%Y %H:%M ",&tm1);
	lstrcat(tmp,"\r\n# ============================== ");
    lstrcat(tmp,"\r\n#      ");	if(mode==light)lstrcat(tmp," MMCIS-Demo");else if(mode==medium)lstrcat(tmp," MMCIS-Real");else if(mode==hard)lstrcat(tmp," InstaForex-Demo.com");
	lstrcat(tmp,"\r\n#      action: ");if(action==optimizing)lstrcat(tmp,"optimizing");else if(action==debuging)lstrcat(tmp,"debuging"); else if(action==testing)lstrcat(tmp,"testing");
	lstrcat(tmp,"\r\n#      period: ");lstrcat(tmp,intToStr(period));
	lstrcat(tmp,"\r\n#      ");lstrcat(tmp,tmp2);
	lstrcat(tmp,"\r\n# ============================== ");lstrcat(tmp,"\r\n\r\n");
	int i1=strlen(tmp);WriteFile(hFile, &tmp[0], i1, &result, NULL);Mfree(tmp);Mfree(tmp2);
	SetFilePointer(hFile,i+i1,NULL,FILE_BEGIN);
    
    WriteFile(hFile, &globallog[0], globallogsize, &result, NULL);
    int hloglen;hloglen=GetWindowTextLength(hlog);char* buf2= new char[2];buf2 = (char*)Mrealloc(buf2,hloglen+1);GetWindowText(hlog,&buf2[0],hloglen);
    SetFilePointer(hFile,i1+i+globallogsize,NULL,FILE_BEGIN);
    WriteFile(hFile, &buf2[0], hloglen-1, &result, NULL);
    delete[] buf2;
	CloseHandle(hFile);
    
}
void title(int h,const char* buffer){
		if(h==whwnd)SetWindowText(hwnd,buffer);else
		if(h==whlog)SetWindowText(hlog,buffer);else
		if(h==whpro)SetWindowText(hpro,buffer);else
		if(h==whper)SetWindowText(hper,buffer);
}

void *Mmalloc(size_t size)
{
	char *aux=0;
	size++;
	if((aux=(char *)malloc(size))==NULL){wlog("\r\nMmalloc error\r\n");return NULL;}
	memset(aux, '\0', size);
	return (void *)aux;
}

char *Mstrdup(const char *s)
{
	char *aux=0;
	size_t size;
	if(!s)return NULL;
	size = strlen(s)+1;
	if((aux=(char *)malloc(size))==NULL)return NULL;
	memcpy(aux, s, size);
	return (char *)aux;
}

void *Mrealloc(void* ptr, size_t size)
{
	char *aux=0;
	if((aux=(char *)realloc(ptr, size))==NULL){wlog("\r\nMrealloc error\r\n");return NULL;}
	return (void *)aux;
}

void Mfree(void *ptr)
{
	if(ptr!=NULL){
		memset(ptr, '\0', sizeof(&ptr));
		free(ptr);
		ptr=NULL;
	}
}
inline char* intToStr(int i){
	static char str1[18]="";memset(str1,0,18);
	snprintf(str1, 17, "%lld", i);

	return (char *)str1;
}
inline int find(const char *s, const char *key)
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
		if(s-e==len && !strncmp(e, key, s-e)) return 1;//i;
		else s = a;
		elementskip, whitespaceskip;
	}
    return 0;
}
inline int find2(const char *s, const char *key)
{
	int len = strlen(key);
	int len2 = strlen(s);
	if(len<len2)
	for(int i = 0; i<(len2-len); i++)
	{
		bool crc=true;
		for(int i1 = 0; i1<len; i1++)
		if(s[i+i1]!=key[i1])crc=false;

		if(crc) return 1;

	}
    return 0;
}
/*
#define SELECT_QUERY "select * from mt4 where valname='EURUSD' and timeframe=15 order by `ctm` desc limit 100;"
static HMODULE mysql_dll = NULL;

typedef MYSQL* (WINAPI *pmysql_init)(MYSQL*);
typedef MYSQL* (WINAPI *pmysql_real_connect)(MYSQL *mysql, const char *host,
					   const char *user,
					   const char *passwd,
					   const char *db,
					   unsigned int port,
					   const char *unix_socket,
					   unsigned long clientflag);
typedef int (WINAPI *pmysql_query)(MYSQL *mysql, const char *q);

typedef const char * (WINAPI *pmysql_error)(MYSQL *mysql);
typedef MYSQL_RES * (WINAPI *pmysql_store_result)(MYSQL *mysql);
typedef unsigned int (WINAPI *pmysql_num_fields)(MYSQL_RES *res);
typedef void (WINAPI *pmysql_free_result)(MYSQL_RES *result);
typedef void (WINAPI *pmysql_close)(MYSQL *sock);
typedef MYSQL_ROW (WINAPI *pmysql_fetch_row)(MYSQL_RES *result);
typedef int (WINAPI *pmysql_options)(MYSQL *mysql,enum mysql_option option,const void *arg);

int mysqltest()
{
  int	count, num;
  MYSQL mysql,*sock;
  MYSQL_RES *res;
  MYSQL_ROW row;
  char	qbuf[160];

  mysql_dll = LoadLibraryA("libmysql.dll");
  static pmysql_init mysql_init=(pmysql_init)-1;
  static pmysql_real_connect mysql_real_connect=(pmysql_real_connect)-1;
  static pmysql_query mysql_query=(pmysql_query)-1;

  static pmysql_error mysql_error=(pmysql_error)-1;
  static pmysql_store_result mysql_store_result=(pmysql_store_result)-1;
  static pmysql_num_fields mysql_num_fields=(pmysql_num_fields)-1;
  static pmysql_free_result mysql_free_result=(pmysql_free_result)-1;
  static pmysql_close mysql_close=(pmysql_close)-1;
  static pmysql_fetch_row mysql_fetch_row=(pmysql_fetch_row)-1;
  static pmysql_options mysql_options=(pmysql_options)-1;

  if(mysql_init == (pmysql_init)-1)
  {
    mysql_init=(pmysql_init)GetProcAddress(mysql_dll,"mysql_init");
    mysql_real_connect=(pmysql_real_connect)GetProcAddress(mysql_dll,"mysql_real_connect");
    mysql_query=(pmysql_query)GetProcAddress(mysql_dll,"mysql_query");

    mysql_error=(pmysql_error)GetProcAddress(mysql_dll,"mysql_error");
    mysql_store_result=(pmysql_store_result)GetProcAddress(mysql_dll,"mysql_store_result");
    mysql_num_fields=(pmysql_num_fields)GetProcAddress(mysql_dll,"mysql_num_fields");
    mysql_free_result=(pmysql_free_result)GetProcAddress(mysql_dll,"mysql_free_result");
    mysql_close=(pmysql_close)GetProcAddress(mysql_dll,"mysql_close");
    mysql_fetch_row=(pmysql_fetch_row)GetProcAddress(mysql_dll,"mysql_fetch_row");
    mysql_options=(pmysql_options)GetProcAddress(mysql_dll,"mysql_options");
  }
    if (!mysql_init)wlog("Couldn't use libmysql.dll!\n%s\n\n");
    ULONG address_len;
    address_len= sizeof (MYSQL);

  mysql_init(&mysql);
//        mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");
//        mysql_options(&mysql, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
  if (!(sock = mysql_real_connect(&mysql,"127.0.0.1","root",0,"admin",3306,NULL,0)))
  {
    wlog("Couldn't connect to engine!\n%s\n\n");
    return(1);
  }
  mysql.reconnect= 1;
  count = 0;
    sprintf(qbuf,SELECT_QUERY,count);
    if(mysql_query(sock,qbuf))
    {
      wlog("Query failed (%s) ");
	  wlog(mysql_error(sock));wlog("\r\n");
      return(1);
    }
    if (!(res=mysql_store_result(sock)))
    {
      wlog("Couldn't get result from %s");
	  wlog(mysql_error(sock));wlog("\r\n");
      return(1);
    }
  while (count < 10)
  {
	row = mysql_fetch_row(res);
	for(int t=0;t<9;t++){
	wlog(row[t]);wlog(" ");}
	wlog("\r\n");
    count++;
  }
    mysql_free_result(res);
  mysql_close(sock);
  FreeLibrary(mysql_dll);
  mysql_dll = NULL;
  mysql_init = NULL;
  mysql_real_connect = NULL;
  mysql_query = NULL;
  return 0;		
}
*/
