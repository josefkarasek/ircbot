CC =g++
CFLAGS =--std=c++11 -Wall -pedantic -Wextra
FILES =ircbot.cpp UnixTCPService.cpp UnixUDPService.cpp
RESULT =ircbot
LOGIN = xkaras27
ADDITIONALS = Makefile README manual.pdf NetworkException.h ircbot.cpp SocketNetworkService.h UnixTCPService.cpp UnixTCPService.h UnixUDPService.h UnixUDPService.cpp

$(RESULT): $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(RESULT)

clean:
	rm -f $(RESULT)

tar: clean
	tar -cf $(LOGIN).tar $(ADDITIONALS)

rmtar:
	rm -f $(LOGIN).tar