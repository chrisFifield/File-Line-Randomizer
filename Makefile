OBJS = random.o
CC = g++
DEBUG = -g3
SANITIZERS = -Og -fno-optimize-sibling-calls -fno-ipa-icf -fno-omit-frame-pointer -fno-common -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined -fsanitize=shift -fsanitize=shift-exponent -fsanitize=shift-base -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=alignment -fsanitize=object-size -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize=bool -fsanitize=enum -fsanitize=vptr -fsanitize=pointer-overflow -fsanitize=builtin -fno-sanitize-recover=all -fsanitize-address-use-after-scope -fstack-protector-all
CFLAGS = -Wall -Wextra -c $(DEBUG) $(SANITIZERS)
LFLAGS = -Wall -Wextra $(DEBUG) $(SANITIZERS)
EXE = exe
LIBS = random.hpp
OTHER = *.o

$(EXE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(EXE)

random.o: random.cpp $(LIBS)
	$(CC) $(CFLAGS) random.cpp -o random.o

clean:
	rm -f $(EXE) $(OBJS) $(OTHER) -v

clear:
	rm -f $(OTHER) -v

remake: clean $(EXE)

rmk: clean $(EXE)

clr: clear
