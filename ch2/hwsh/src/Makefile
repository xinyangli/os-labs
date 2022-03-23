OBJS := main.o exec.o signals.o hist.o analyzer.o getcmd.o stack.o
INCLUDE := ../include
CFLAGS += -I$(INCLUDE) -g

all: $(OBJS)
	$(CC) $(CFLAG) $(OBJS) -o ../bin/hwsh

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) *.o ../bin/hwsh