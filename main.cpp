// @version 2013.09.08.1152
// @author Atlee Brink
/*
	implements main or WinMain

	usage:
		define MSWINDOWS if you want WinMain(...),
			else will default to main(...)
		implement mainFunc
		disable wchar_t built-in type
		compile and link this with your project

	notes:
		uses unsigned short for unicode and not inbuilt wchar_t
			this is for compatibility because wchar_t is not standard
*/
extern void mainFunc(int argc, unsigned short **argvw);

#ifdef MSWINDOWS
#include <Windows.h>
int CALLBACK WinMain(
	HINSTANCE hinstance
	,HINSTANCE hprevinstance
	,LPSTR lpcmdline
	,int ncmdshow
){
	int argc;
	unsigned short **argvw = CommandLineToArgvW(GetCommandLineW(), &argc);
	mainFunc(argc, argvw);
	return 0;
}
#else
int main(int argc, char *argv[]) {
	unsigned short **argvw = new unsigned short *[argc];
	for(int iarg = 0; iarg < argc; iarg++) {
		int len;
		for(len = 1; argv[iarg][len] != 0; len++);
		argvw[iarg] = new unsigned short [len];
		if(argvw[iarg]) {
			for(int ichar = 0; ichar <= len; ichar++) {
				argvw[iarg][ichar] = (unsigned short)argv[iarg][ichar];
			}
		}
	}
	mainFunc(argc, argvw);
	return 0;
}
#endif