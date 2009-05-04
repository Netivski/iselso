// LancaProcessosConsola.cpp : Defines the entry point for the console application.
//

#include <locale.h>
#include <tchar.h>
#include <windows.h>


void showFileTime( _TCHAR* token, FILETIME *ft ){
  SYSTEMTIME st;
  FileTimeToSystemTime(ft, &st);
  _tprintf(TEXT("%s: %d:%d:%d:%d\n"), token, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}

int _tmain(int argc, _TCHAR* argv[])
{
    TCHAR command[80];
    STARTUPINFO si;         //permite definir atributos da janela do novo processo
    PROCESS_INFORMATION piA;//devolve os Handles e Ids globais do novo proc. e thrd.
    HANDLE newProcessHandle;
    
    _tsetlocale(LC_ALL, TEXT("portuguese_portugal"));

    _tprintf(TEXT("Escreva o nome de um programa a executar> "));
    _tscanf_s(TEXT("%s"), &command, 80);
    _tprintf(TEXT("\nLi o comando: %s.\n"), command);

    // usar a funcao da Win32 API para fazer fill de zero à estrutura  
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);        // STARTUPINFO por omissão
    // Criar o processo
    if (!CreateProcess(NULL, command,NULL,NULL,FALSE,0,NULL,NULL, &si, &piA) ) {
        _tprintf(TEXT("Erro ao criar Processo (%d)"), GetLastError());
        return -1;
    }
	
    // guardar o handle do processo
    newProcessHandle = piA.hProcess;

    _tprintf(TEXT("Processo criado. Esperando a sua conclusão\n"));
    


	//Esperar que o processo termine
	DWORD result = WaitForSingleObject(newProcessHandle, INFINITE);
	if (result == WAIT_FAILED)
	    _tprintf(TEXT("Erro no WaitForSingleObject (%d)"), GetLastError());


     //A espera da terminação do processo podia ser feita através da função
     //GetExitCodeProcess mas neste caso teriamos uma espera activa
	DWORD exitCode;
	//do {
		GetExitCodeProcess(newProcessHandle, &exitCode);
	//	Sleep(1);
	//} while (exitCode == STILL_ACTIVE);

    _tprintf(TEXT("O processo terminou com o código %d\n"), exitCode);


    _tprintf(TEXT("Terminou...\n"));

	FILETIME ft, ft2, ft3, ft4;
	GetProcessTimes(newProcessHandle, &ft, &ft2, &ft3, &ft4);
	FileTimeToLocalFileTime( &ft2, &ft2 ); 
	FileTimeToLocalFileTime( &ft , &ft  );
	showFileTime( TEXT("lpUserTime::")    , &ft4);
    showFileTime( TEXT("lpKernelTime::")  , &ft3);
	showFileTime( TEXT("lpExitTime::")    , &ft2);
	showFileTime( TEXT("lpCreationTime::"), &ft);

		
    BOOL resultClose;
    resultClose= CloseHandle(newProcessHandle);
    if ( !resultClose )
        _tprintf(TEXT("Erro no CloseHandle (%d)"), GetLastError());

	resultClose = CloseHandle(piA.hThread);
    if ( !resultClose )
        _tprintf(TEXT("Erro no CloseHandle (%d)"), GetLastError());


	return 0;
}