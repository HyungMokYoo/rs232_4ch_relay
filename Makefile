CC		= gcc -g

all: rs232_relay

rs232_relay: rs232_relay.o
	$(CC) -o $@ $<
rs232_relay.o: rs232_relay.c
	$(CC) -c $<

clean:
	rm -rf rs232_relay *.o
