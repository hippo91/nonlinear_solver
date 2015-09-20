cc=gcc
DEBUG=true
ifeq ($(DEBUG), true)
CFLAGS=-O0 -g3 -Wall -std=c99
else
CFLAGS=-O3 -march=native -mtune=native -std=c99
endif


newton.bin : newton.o utils.o vnr_internalenergy_evolution.o main.o incrementations_methods.o miegruneisen.o stop_criterions.o
	$(cc) $(CFLAGS) -lm *.o -o newton.bin


miegruneisen.o : miegruneisen.c miegruneisen.h
	$(cc) $(CFLAGS) -c miegruneisen.c miegruneisen.h

utils.o : utils.c utils.h
	$(cc) $(CFLAGS) -c utils.c utils.h

vnr_internalenergy_evolution.o: vnr_internalenergy_evolution.c vnr_internalenergy_evolution.h
	$(cc) $(CFLAGS) -c vnr_internalenergy_evolution.c vnr_internalenergy_evolution.h

incrementations_methods.o: incrementations_methods.c incrementations_methods.h
	$(cc) $(CFLAGS) -c incrementations_methods.c incrementations_methods.h

stop_criterions.o : stop_criterions.c stop_criterions.h
	$(cc) $(CFLAGS) -c stop_criterions.c stop_criterions.h

newton.o : newton.c newton.h utils.h
	$(cc) $(CFLAGS) -c newton.c newton.h utils.h

main.o : main.c
	$(cc) $(CFLAGS) -c main.c

clean :
	rm miegruneisen.o utils.o vnr_internalenergy_evolution.o incrementations_methods.o newton.o main.o stop_criterions.o newton.bin miegruneisen.gch utils.gch vnr_internalenergy_evolution.gch incrementations_methods.gch newton.gch main.gch stop_criterions.gch
