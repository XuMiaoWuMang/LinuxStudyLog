CC = g++
Source = Server.cc Client.cc
Object = $(Source:.cc=.o)
Executable = Server Client
Commend = -Wall -Wextra -Werror -std=c++11 -pthread
fifoname = fifo
all: Server Client

Server: Server.cc
	@$(CC) $(Commend) Server.cc -o Server 

Client: Client.cc
	@$(CC) $(Commend) Client.cc -o Client

.PHONY: clean
clean:
	@rm -f $(Executable)
	


