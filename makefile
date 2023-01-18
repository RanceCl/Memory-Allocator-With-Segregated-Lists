
comp = gcc
comp_flags = -g -Wall 
comp_libs = -lm  

Test0 : slab_test_0.o slab.o
	$(comp) $(comp_flags) slab_test_0.o slab.o -pthread -o Test0 $(comp_libs)

Test1 : slab_test_1.o slab.o
	$(comp) $(comp_flags) slab_test_1.o slab.o -pthread -o Test1 $(comp_libs)

Test2 : slab_test_2.o slab.o
	$(comp) $(comp_flags) slab_test_2.o slab.o -pthread -o Test2 $(comp_libs)

Test3 : slab_test_3.o slab.o
	$(comp) $(comp_flags) slab_test_3.o slab.o -pthread -o Test3 $(comp_libs)

Test4 : slab_test_4.o slab.o
	$(comp) $(comp_flags) slab_test_4.o slab.o -pthread -o Test4 $(comp_libs)


slab_test_0.o : slab_test_0.c slab.h
	$(comp) $(comp_flags) -c slab_test_0.c

slab_test_1.o : slab_test_1.c slab.h
	$(comp) $(comp_flags) -c slab_test_1.c

slab_test_2.o : slab_test_2.c slab.h
	$(comp) $(comp_flags) -c slab_test_2.c

slab_test_3.o : slab_test_3.c slab.h
	$(comp) $(comp_flags) -c slab_test_3.c

slab_test_4.o : slab_test_4.c slab.h
	$(comp) $(comp_flags) -c slab_test_4.c

slab.o : slab.c slab.h
	$(comp) $(comp_flags) -c slab.c

clean :
	rm -f *.o Test0 Test1 Test2 Test3 Test4 core
