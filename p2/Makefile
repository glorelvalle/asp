CC = mpicc -pedantic -W
EXE = ejercicio1.1 ejercicio1.2 ejercicio1.3 ejercicio2.1 ejercicio2.2 ejercicio2.3 ejercicio2.4 ejercicio2.5
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
