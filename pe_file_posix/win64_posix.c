// jimon, 2014
// small posix proxy to test cppgm course on windows

#include "win64_posix.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <inttypes.h>

#include "mman.h"

LIBDLL_API int64_t syscall_msg(int64_t arg)
{
	return MessageBoxA(NULL, (const char*)arg, "CPPGM Win Posix Emulator", 0);
}

LIBDLL_API int64_t syscall_prv(
		int64_t func,
		int64_t arg0,
		int64_t arg1,
		int64_t arg2,
		int64_t arg3,
		int64_t arg4,
		int64_t arg5)
{
	switch(func)
	{
	case SN_EXIT:
		ExitProcess(arg0);
		return 0;
	case SN_READ:
		return read(arg0, (void*)arg1, arg2);
	case SN_WRITE:
		return write(arg0, (const void*)arg1, arg2);
	case SN_MMAP:
		return mmap((void*)arg0, arg1, arg2, arg3, arg4, arg5);
	case SN_MUNMAP:
		return munmap((void*)arg0, arg1);
	default:
		return -1;
	}
}

LIBDLL_API int64_t syscall0(
		int64_t func)
{
	return syscall_prv(func, 0, 0, 0, 0, 0, 0);
}

LIBDLL_API int64_t syscall1(
		int64_t func,
		int64_t arg0)
{
	return syscall_prv(func, arg0, 0, 0, 0, 0, 0);
}

LIBDLL_API int64_t syscall2(
		int64_t func,
		int64_t arg0,
		int64_t arg1
		)
{
	return syscall_prv(func, arg0, arg1, 0, 0, 0, 0);
}

LIBDLL_API int64_t syscall3(
		int64_t func,
		int64_t arg0,
		int64_t arg1,
		int64_t arg2
		)
{
	return syscall_prv(func, arg0, arg1, arg2, 0, 0, 0);
}

LIBDLL_API int64_t syscall4(
		int64_t func,
		int64_t arg0,
		int64_t arg1,
		int64_t arg2,
		int64_t arg3
		)
{
	return syscall_prv(func, arg0, arg1, arg2, arg3, 0, 0);
}

LIBDLL_API int64_t syscall5(
		int64_t func,
		int64_t arg0,
		int64_t arg1,
		int64_t arg2,
		int64_t arg3,
		int64_t arg4
		)
{
	return syscall_prv(func, arg0, arg1, arg2, arg3, arg4, 0);
}

LIBDLL_API int64_t syscall6(
		int64_t func,
		int64_t arg0,
		int64_t arg1,
		int64_t arg2,
		int64_t arg3,
		int64_t arg4,
		int64_t arg5
		)
{
	return syscall_prv(func, arg0, arg1, arg2, arg3, arg4, arg5);
}

