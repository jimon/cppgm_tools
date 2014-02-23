; win64 posix dll test

format PE64 console
entry start

section '.text' code readable executable

start:
	sub rsp,8*5 ; reserve stack for API use and make stack dqword aligned

