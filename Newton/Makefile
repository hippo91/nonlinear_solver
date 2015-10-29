cc=gcc
DEBUG=false
LIBRARY=true
ifeq ($(DEBUG), true)
CFLAGS=-O0 -g3 -Wall -std=c99 -fopenmp
else
CFLAGS=-O3 -march=native -mtune=native -std=c99 -fopenmp
endif
ifeq ($(LIBRARY), true)
CFLAGS+=-fPIC -shared
endif


ifeq ($(LIBRARY), true)
vnr_internal_energy_evolution: newton.o utils.o vnr_internalenergy_evolution.o incrementations_methods.o miegruneisen.o stop_criterions.o quadratic_function.o launch_vnr_resolution.o
	$(cc) $(CFLAGS) -lm -shared -fPIC *.o -o vnr_internal_energy_evolution.so
else
newton.bin : newton.o utils.o vnr_internalenergy_evolution.o main.o incrementations_methods.o miegruneisen.o stop_criterions.o quadratic_function.o launch_vnr_resolution.o
	$(cc) $(CFLAGS) -lm *.o -o newton.bin
endif

miegruneisen.o : miegruneisen.c miegruneisen.h
	$(cc) $(CFLAGS) -c miegruneisen.c miegruneisen.h

utils.o : utils.c utils.h
	$(cc) $(CFLAGS) -c utils.c utils.h

vnr_internalenergy_evolution.o: vnr_internalenergy_evolution.c vnr_internalenergy_evolution.h
	$(cc) $(CFLAGS) -c vnr_internalenergy_evolution.c vnr_internalenergy_evolution.h

quadratic_function.o: quadratic_function.c quadratic_function.h
	$(cc) $(CFLAGS) -c quadratic_function.c quadratic_function.h

incrementations_methods.o: incrementations_methods.c incrementations_methods.h
	$(cc) $(CFLAGS) -c incrementations_methods.c incrementations_methods.h

stop_criterions.o : stop_criterions.c stop_criterions.h
	$(cc) $(CFLAGS) -c stop_criterions.c stop_criterions.h

newton.o : newton.c newton.h utils.h
	$(cc) $(CFLAGS) -c newton.c newton.h utils.h

launch_vnr_resolution.o: launch_vnr_resolution.c launch_vnr_resolution.h
	$(cc) $(CFLAGS) -c launch_vnr_resolution.c launch_vnr_resolution.h

main.o : main.c
	$(cc) $(CFLAGS) -c main.c

clean :
	rm *.o *.h.gch
