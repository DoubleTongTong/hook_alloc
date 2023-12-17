CC=gcc
CFLAGS=-Wall -fPIC -g
LDFLAGS=-ldl
SOURCE=hook_alloc.c
OBJECT=$(SOURCE:.c=.o)
SHARED_LIB=libhookalloc.so

all: $(SHARED_LIB)

$(SHARED_LIB): $(OBJECT)
	$(CC) -shared -o $(SHARED_LIB) $(OBJECT) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECT) $(SHARED_LIB)

