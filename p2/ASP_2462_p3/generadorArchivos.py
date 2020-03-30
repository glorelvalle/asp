# Práctica 2 de Arquitectura de Sistemas Paralelos
#
# Generador de archivos
# Michael Alexander Fajardo y Gloria del Valle
import random
import threading

class hilo(threading.Thread):
    def __init__(self, group=None, target=None, name=None,
        args=(), kwargs=None, *, daemon=None):
        super().__init__(group=group, target=target, name=name,
                     daemon=daemon)

        self.Filname = args[0]
        self.FileType = args[1]
    def run(self):
        file = open(self.Filname,"w")
        numbers = 0
        numberList = []
        number = 0
        if self.FileType == 1:
            while numbers < 300:
                number = random.randrange(1000,4000)
                if number not in numberList:
                    numberList.append(number)
                    numbers = numbers + 1
        elif self.FileType == 2:
            while numbers < 900:
                number = random.randrange(300,700)
                numberList.append(number)
                numbers = numbers + 1
        elif self.FileType == 3:
            while numbers < 450:
                i = 0
                while i < 5:
                    number = random.randrange(0,200)
                    numberList.append(number)
                    i +=1
                number = random.randrange(1000,4000)
                numberList.append(number)
                numbers = numbers + 5
        else:
            while numbers < 400:
                number = random.randrange(1000,3000)
                numberList.append(number)
                number = random.randrange(1,40)
                numberList.append(number)
                numbers = numbers + 2

        for num in numberList:
            file.write(str(num) + "\n")
        file.close()


ficheroA = hilo(args = ("ficheroA.txt",1))
ficheroB = hilo(args = ("ficheroB.txt",2))
ficheroC = hilo(args = ("ficheroC.txt",3))
ficheroD = hilo(args = ("ficheroD.txt",4))

ficheroA.start()
ficheroB.start()
ficheroC.start()
ficheroD.start()
