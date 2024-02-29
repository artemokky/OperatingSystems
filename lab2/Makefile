CC = g++

CFLAGS = -Wall -Werror -Iinclude

OBJDIR = obj

OBJS = $(OBJDIR)/host.o $(OBJDIR)/client.o $(OBJDIR)/conn_mq.o $(OBJDIR)/conn_fifo.o $(OBJDIR)/conn_sock.o $(OBJDIR)/logger.o

EXECUTABLES = host client

all: $(EXECUTABLES)

host: $(OBJDIR)/host.o $(OBJDIR)/conn_mq.o $(OBJDIR)/conn_fifo.o $(OBJDIR)/conn_sock.o $(OBJDIR)/logger.o
	$(CC) $(CFLAGS) -o $@ $^

client: $(OBJDIR)/client.o $(OBJDIR)/conn_mq.o $(OBJDIR)/conn_fifo.o $(OBJDIR)/conn_sock.o $(OBJDIR)/logger.o
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: src/conn/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: src/utils/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(EXECUTABLES)

.PHONY: all clean
