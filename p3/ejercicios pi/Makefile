CC = mpicc -pedantic -W
EXE = ejercicio1.1_mpi
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
