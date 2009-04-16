// MyHelloWorldThreads.cpp : Defines the entry point for the console application.
//


#define NTHREADS 5
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <locale.h>

#include "..\..\include\BeginThreadex.h"


DWORD WINAPI ImprimeThread(LPVOID args)
{
    int id = *(int*) args;

	while (true) {
	    _tprintf(TEXT("Thread[%d]: Olá Mundo!!!\n"), id);
	}
    return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
    HANDLE ht[NTHREADS];
    DWORD	threadId[NTHREADS];

    _tsetlocale(LC_ALL, _T("portuguese_portugal"));    // Mudar o codepage

   _tprintf(TEXT("Main: Vou criar as tarefas\n"));

   // Criar as tarefas
   for(int i=0; i<NTHREADS; i++) {

	   int *thNumber = new int(i);

	   //ImprimeThread((LPVOID)thNumber);
       ht[i] = chBEGINTHREADEX( NULL, 0, ImprimeThread, (LPVOID)thNumber, NULL, &threadId[i]);
       if (ht[i] == NULL)
           _tprintf(TEXT("Erro ao criar a thread nº: %d (%d)"), i, GetLastError());
   }
    
    //Esperar a terminação de todas as tarefas
    _tprintf(TEXT("Main: Estou à espera que as tarefas terminem.\n"));
    DWORD result = WaitForMultipleObjects(NTHREADS, ht, TRUE, INFINITE);
    if (result == WAIT_FAILED)
        _tprintf(TEXT("Erro no WaitForSingleObject (%d)"), GetLastError());

    _tprintf(TEXT("Main: Vou sair.\n"));
	return 0;
}