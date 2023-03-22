all: fe16insts fe16memory fe16regs fe16syscalls fe16vm

fe16insts:
	clang -Wall -c src/fe16insts.c
fe16memory:
	clang -Wall -c src/fe16memory.c
fe16regs:
	clang -Wall -c src/fe16regs.c
fe16syscalls:
	clang -Wall -c src/fe16syscalls.c
fe16vm:
	clang -Wall -c src/fe16vm.c
make:
	clang -Wall -g fe16vm.o fe16insts.o fe16memory.o fe16regs.o fe16syscalls.o -o fe16vm
clean:
	rm -rf fe16insts.o fe16memory.o fe16regs.o fe16syscalls.o fe16vm.o fe16vm

