/* --------------------------------------------------------------------------
 *
 * ETK++ --- The Easy Toolkit for C++ programing
 * Copyright (C) 2004-2006, Anthony Lee, All Rights Reserved
 *
 * ETK++ library is a freeware; it may be used and distributed according to
 * the terms of The MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * File: etk-os.cpp
 *
 * --------------------------------------------------------------------------*/

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include <signal.h>
#include <windows.h>

#include <etk/support/Locker.h>
#include <etk/kernel/Kernel.h>
#include <etk/app/Application.h>

HINSTANCE etk_dll_hinstance = NULL;

#if 0
void (__cdecl *signal_int_old_func)(int) = NULL;
void (__cdecl *signal_term_old_func)(int) = NULL;
extern ELocker* etk_get_handler_operator_locker();


void __cdecl etk_signal_func(int signumber)
{
	ELocker *hLocker = etk_get_handler_operator_locker();

	hLocker->Lock();
	if(etk_app != NULL) etk_app->PostMessage(signumber == SIGINT ? E_QUIT_REQUESTED : _QUIT_);
	hLocker->Unlock();

	void (__cdecl *old_func)(int) = NULL;
	switch(signumber)
	{
		case SIGINT: old_func = signal_int_old_func; break;
		case SIGTERM: old_func = signal_term_old_func; break;
		default: break;
	}

//	ETK_WARNING("[KERNEL]: Signal(%s) done.", (signumber == SIGINT ? "SIGINT" : (
//						   signumber == SIGTERM ? "SIGTERM" : "UNKNOWN")));

	while(signumber != SIGINT && etk_app != NULL) e_snooze(1000);

	if(old_func != NULL)
	{
//		ETK_WARNING("[KERNEL]: Calling old signal functions...");
		(*old_func)(signumber);
	}

	return;
}
#endif


extern "C" {
BOOL WINAPI
DllMain(HINSTANCE hinstDLL,  /* handle to DLL module */
	DWORD fdwReason,     /* reason for calling functionm */
	LPVOID lpvReserved   /* reserved */)
{
	etk_dll_hinstance = hinstDLL;

	switch(fdwReason) 
	{
		case DLL_PROCESS_ATTACH:
		/* The DLL is being mapped into process's address space */
		/* Do any required initialization on a per application basis, return FALSE if failed */
		{
			WSADATA wsaData;
			WSAStartup(0x202, &wsaData);

#if 0
			etk_system_boot_time();
			signal_int_old_func = signal(SIGINT, etk_signal_func);
			signal_term_old_func = signal(SIGTERM, etk_signal_func);
#endif
			break;
		}

		case DLL_THREAD_ATTACH:
		/* A thread is created. Do any required initialization on a per thread basis*/
		{
			break;
		}

		case DLL_PROCESS_DETACH:
		/* The DLL unmapped from process's address space. Do necessary cleanup */
		{
#if 0
			signal(SIGINT, signal_int_old_func);
			signal(SIGTERM, signal_term_old_func);
#endif
			WSACleanup();
			break;
		}

		case DLL_THREAD_DETACH:
		/* Thread exits with cleanup */
		{
			break;
		}
	}

	return TRUE;
}


char* etk_win32_convert_active_to_utf8(const char *str, eint32 length)
{
	if(str == NULL || *str == 0 || length == 0) return NULL;

	eint32 nChars = (eint32)strlen(str);

	if(length < 0 || length > nChars) length = nChars;

	WCHAR *wStr = (WCHAR*)malloc(sizeof(WCHAR) * (size_t)(length + 1));
	if(wStr == NULL) return NULL;

	bzero(wStr, sizeof(WCHAR) * (size_t)(length + 1));
	MultiByteToWideChar(CP_ACP, 0, str, length, wStr, length);

	char *uStr = e_unicode_convert_to_utf8((const eunichar*)wStr, -1);
	free(wStr);

	return uStr;
}


char* etk_win32_convert_utf8_to_active(const char *str, eint32 length)
{
	eunichar *wStr = e_utf8_convert_to_unicode(str, length);
	eint32 len = e_unicode_strlen(wStr);

	if(wStr == NULL) return NULL;

	char *aStr = (char*)malloc((size_t)len * 2 + 1);
	if(aStr == NULL) {free(wStr); return NULL;}

	bzero(aStr, (size_t)len * 2 + 1);
	WideCharToMultiByte(CP_ACP, 0, (WCHAR*)wStr, -1, aStr, len * 2, NULL, NULL);

	free(wStr);

	return aStr;
}

} // extern "C"

