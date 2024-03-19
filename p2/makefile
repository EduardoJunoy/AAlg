CC = gcc -g 
CFLAGS = -ansi -Wall -pedantic -O2
EXE = exercise1 exercise2y3 calculate_eficiency

all : $(EXE) clear

.PHONY : clean clear

$(EXE) : % : %.o sorting.o permutations.o times.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -o $@ $@.o sorting.o permutations.o times.o

permutations.o : permutations.c permutations.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $<

sorting.o : sorting.c sorting.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $<

times.o : times.c times.h
	$(CC) $(CFLAGS) -c $<

exercise1_test:
	@echo Running exercise1
	@./exercise1 -size 10


clean :
	rm -f *.o $(EXE)

clear:
	rm -f *.o