OBJS 	= mvote.o LinkedList.o ZipList.o
SOURCE	= mvote.c LinkedList.c ZipList.c
HEADER  = LinkedList.h ZipList.h
OUT  	= mvote
CC	= gcc
FLAGS   = -g -c -pedantic -ansi  -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

# create/compile the individual files >>separately<< 
mvote.o: mvote.c
	$(CC) $(FLAGS) mvote.c

ZipList.o: ZipList.c
	$(CC) $(FLAGS) ZipList.c

LinkedList.o: LinkedList.c
	$(CC) $(FLAGS) LinkedList.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
