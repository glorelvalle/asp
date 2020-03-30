CC = mpicc -pedantic -W
EXE = blocksends deadlock deadlock64k medida_latencia monte_carlo_paralell multiplicacion_nxn suma_mpi
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
