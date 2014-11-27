CC =g++
CFLAGS =-g --std=c++11 -Wall -pedantic -Wextra
FILES =main.cpp UnixTCPService.cpp UnixUDPService.cpp Utils.cpp Ircbot.cpp
RESULT =ircbot
LOGIN = xkaras27
ADDITIONALS = Makefile README manual.pdf NetworkException.h main.cpp SocketNetworkService.h UnixTCPService.cpp UnixTCPService.h UnixUDPService.h UnixUDPService.cpp Utils.h Utils.cpp Ircbot.cpp Ircbot.h

$(RESULT): $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(RESULT)

clean:
	rm -f $(RESULT)

tar: clean
	tar -cf $(LOGIN).tar $(ADDITIONALS)

rmtar:
	rm -f $(LOGIN).tar
