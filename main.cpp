#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <new>
#include "main.h"
#include "Server.h"

Server* server;
void wcmd(int reqestor);
void wlog(const char* buffer);
void title(int h,const char* buffer);
void* Mmalloc(size_t size);
char* Mstrdup(const char *s);
void* Mrealloc(void* ptr, size_t size);
void Mfree(void *ptr);
WNDCLASSEX wincl;
NOTIFYICONDATA note;
char szClassName[]="patterns";
HWND hwnd,hlog,hpro,hper,hcmd;
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszArgument,int nFunsterStil)
{
	MSG messages;
	
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
	hwnd =CreateWindowEx (0,szClassName,"patterns",WS_CAPTION|WS_MINIMIZEBOX|WS_VISIBLE|WS_SYSMENU,CW_USEDEFAULT,CW_USEDEFAULT,544,375,HWND_DESKTOP,NULL,hThisInstance,NULL);
	hlog =CreateWindowEx (0,"Edit","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_NOHIDESEL|WS_VSCROLL,0,0,538,310,hwnd,NULL,hThisInstance,NULL);
	hcmd =CreateWindowEx (0,"Edit","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_NOHIDESEL|WS_VSCROLL,0,310,538,20,hwnd,NULL,hThisInstance,NULL);
	hpro =CreateWindowEx (0,"Static"," ",WS_CHILD | WS_VISIBLE,0,330,440,355,hwnd,NULL,hThisInstance,NULL);
	hper =CreateWindowEx (0,"Static"," ",WS_CHILD | WS_VISIBLE,450,330,120,35,hwnd,NULL,hThisInstance,NULL);
	ShowWindow (hwnd, nFunsterStil);
	UpdateWindow(hwnd);

	server = new Server;
    if((!strcmp(lpszArgument,"/t15 /opt"))||(!strcmp(lpszArgument,"/t60 /opt")))
	server->on(false);else server->on();
	if(!strcmp(lpszArgument,"/t15 /debug"))server->otskok(debuging,15);else
	if(!strcmp(lpszArgument,"/t15 /test")){
		server->otskok(testing,15);
		server->impuls(testing,15);
	}else
	if(!strcmp(lpszArgument,"/t15 /opt")){
		server->impuls(optimizing,15);
		server->otskok(optimizing,15);
	}else
	if(!strcmp(lpszArgument,"/t60 /debug"))server->otskok(debuging,60);else
	if(!strcmp(lpszArgument,"/t60 /test")){
		server->otskok(testing,60);
		server->impuls(testing,60);
	}else
	if(!strcmp(lpszArgument,"/t60 /opt")){
		server->impuls(optimizing,60);
		server->otskok(optimizing,60);
	}

	while (GetMessage (&messages, NULL, 0, 0))
	{if((messages.hwnd==hcmd)&&(messages.message==WM_KEYUP)&&messages.wParam==VK_RETURN)wcmd(cmdmain);
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}
	return messages.wParam;
}
LRESULT CALLBACK WindowProcedure (HWND hwnd1, UINT message, WPARAM wParam, LPARAM lParam)
{
switch (message)
{
	case WM_DESTROY:
		{
			delete server;
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
void wcmd(int reqestor){
		if(reqestor==cmdmain)wlog("\r\n main enter\r\n");else
		if(reqestor==cmdotskok)wlog("\r\n otskok enter\r\n");else
		if(reqestor==cmdimpuls)wlog("\r\n impuls enter\r\n");
}
void wlog(const char* buffer){
		char* buf;
	    int hloglen=GetWindowTextLength(hlog)+1;
	    int slen=strlen(buffer)+1;
        buf= new char[hloglen+slen+1];
		memset(buf,0,hloglen+slen+1);
		if((hloglen+slen)<30000)GetWindowText(hlog,buf,hloglen);
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
	if((aux=(char *)malloc(size))==NULL)return NULL;
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
	if((aux=(char *)realloc(ptr, size))==NULL)return NULL;
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
