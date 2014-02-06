//+------------------------------------------------------------------+
//|                                                         inet.mq4 |
//|                                                                  |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""
#import "shell32.dll"
 int ShellExecuteA(int hWnd, string Operation, string FileName, string Parameters, string Directory, int ShowCmd);
#import
#import "user32.dll"
 int      SendMessageA(int hWnd,int Msg,int wParam,int lParam);
#import
#import "KERNEL32.dll"
 int WinExec(string FileName,int ShowCmd);
#import
//+------------------------------------------------------------------+
//| script program start function                                    |
//+------------------------------------------------------------------+
int start()
  {

//----
string s1="";
ShellExecuteA(0,"","route.bat","off","f:\WINDOWS\system32\\",1);
   Print(s1);
//----
   return(0);
  }
//+------------------------------------------------------------------+