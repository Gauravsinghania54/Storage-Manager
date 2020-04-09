CC := gcc
SOURCE := dberror.c test_assign1_1.c storage_mgr.c
OBJECT := dberror.o test_assign1_1.o storage_mgr.o
assignment1: $(OBJECT)
	$(CC) -o test_assign1_1 $?
%.o: %.c
	$(CC) -g -c $<
run: assignment1
	./test_assign1_1
clean:
	rm -rf test_assign1_1 *.o