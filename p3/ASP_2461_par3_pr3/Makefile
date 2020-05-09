#CC = mpicc -pedantic -W
#CC = mpicc -fopenmp
CC = gcc -fopenmp
EXE = ejercicio1.1_parallel ejercicio1.1_priv_crit ejercicio1.1_worksharing ejercicio1.1_ws_red ejercicio1.4
all : $(EXE)

.PHONY : clean
clean:
		rm -f *.o core $(EXE)

$(EXE) : % : %.o
		@echo "#---------------"
		@echo "# Generando $@"
		$(CC) -o $@ $@.o

%.o : %.c
		@echo "#--------------"
		@echo "# Generando $@"
		$(CC) -c $<
