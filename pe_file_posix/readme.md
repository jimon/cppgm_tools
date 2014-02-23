# CPPGM PA9 add-on to support PE and Win64

## ABI

Win64 ABI is a bit different from x86_64 SYSV ABI.
You will need to modify your code generator a bit to support it.

Check [MinGW x64 Software convention](http://sourceforge.net/apps/trac/mingw-w64/wiki/MinGW%20x64%20Software%20convention) for details.

## POSIX

PA9 require only 4 syscall functions from us : exit, read, write, mmap.
So I wrote a simple x64 dll to emulate this functions on Windows. You will need MinGW x64 and qmake (part of Qt SDK) to compile it. I've added precompiled dll and fasm sample to repo.

Probably it will be easy to expand it later. But I probably dont want to support whole POSIX, only a small subset for easy debugging on Windows.

## PE file generation

TODO  
I will add PE file creation sample (similar to PA9 ELF sample) later. 

## MinGW ABI (from link above)

###Floating point support (%st0-%st7, %mm0-%mm7)
For these fpu stack registers, there is no calling convention, and they are preserved across context switches.

###Parameter passing to prototyped functions
The first four arguments are passed in registers %rcx, %rdx, %r8, and %r9. For these parameters, stack space has to be reserved. If a function has more than four integer size arguments, they are passed on the stack instead.

Arguments that are floating point and double precision are passed in the %xmm0, %xmm1, %xmm2, and %xmm3 registers. Additionally, the standard integer registers %rcx, %rdx, %r8, and %r9 are set. For exampe, if the second argument passed is a floating point, %rdx and %xmm1 are used.

Arrays, m128, and strings are always passed by reference to memory reserved by the caller.

Structures and unions with byte sizes of 1, 2, 4, or 8, and _m64 are treated like integer arguments. Other sizes are passed as reference to memory reserved by the caller and have to be aligned to a 16-byte boundary.

###Parameter passing to unprototyped functions
For unprototyped functions, the first four arguments have to be passed in %xmm and integer registers.
See as reference in msdn: msdn library

###Return values of functions

Return values fitting in 64 bits are returned through %rax. Just m128 and doubles use the %xmm0 register. If the returned value does not fit into 64-bits, then the caller is responsible for passing the return value as a memory reference in the first argument. This pointer has to be returned by the callee as result in %rax. User defined types to be returned directly in %rax have to have a bit-size of 1, 2, 4, 8, 16, 32, or 64. Otherwise, they have to be passed by reference.
See as reference in msdn: msdn library

###Varargs

For vararg argument passing (e.g. ellipsis '...') the normal argument passing has to be done. The callee is responsible to dump the arguments that have their address taken.
See as reference in msdn: msdn library
See as reference in msdn: msdn library

###Register usage

There are 16 general-purpose registers, and 16 xmm floating point registers.

* %rax - Return and accumulator register - may scratched on call
* %rcx - First argument register - may scratched on call
* %rdx - Second argument register - may scratched on call
* %rbx - Used for profiling for gcc - Must be preserved by callee
* %rsi & %rdi - General purpose register - Must be preserved by callee
* %rbp - May used as frame pointer - Must be preserved by callee
* %rsp - The stack pointer - Not scratched
* %r8 - Third argument register - may scratched on call
* %r9 - Fourth argument register - may scratched on call
* %xmm0 up to %xmm3 - FP argument - may scratched on call
* %xmm4 & %xmm5 - fp register - Must be preserved as needed by caller
* %xmm6 up to %xmm15 - fp register - Must be preserved as needer by callee
* %r10 & %r11 - Used for this on gcc and for syscall/sysret instructions - May scratched, caller may saves
* %r12 up to %r15 - General purpose registers - Must be preserved by callee

###Parameter passing

For integers of 64bits or less, like Windows, parameters are passed right to left. **However, instead of starting with %rcx, %rdx, %r8, %9, then the stack, sysv uses %rdi, %rsi, %rdx, %rcx, %r8, %r9 and finally the stack. %rax is used for returning values for both sysv and Windows. Sysv allows a second value returned through %rdx.**

For floating points, Windows uses %xmm0 through %xmm3 for parameter passing. Sysv uses %xmm2 to %xmm7 instead. For return values, both Windows and sysv uses %xmm0. Sysv allows a second value returned through %xmm1.

Additional to be noted here is the varadic is quite different. For Windows, it is pure stack-based, but Sysv uses a sophisticated passing algorithm via internal structures and the rax register for flags.

Sysv supports a "red-zone", a 128-byte area after the address pointed by %rsp where data can be stored and used without readjusting the stack pointer. Red-zones are not available in Windows.

Register usage and saving schema is also different to x64 ABI (clobber list difference to be described later).
