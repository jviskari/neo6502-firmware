SRCS = $(wildcard *.bsc)
PROGS = $(patsubst %.bsc,%.bas,$(SRCS))
CC=./bsc2bas

all: $(PROGS)

%.bas: %.bsc
	$(CC) $< $@

clean:
	rm -f $(PROGS)
