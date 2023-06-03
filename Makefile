CHARMC=../../charmc $(OPTS)

OBJS = \
  msg.o \

all: ping

ping: $(OBJS)
	$(CHARMC) -o ping $(OBJS) -language converse++

msg.o: msg.c
	$(CHARMC) -gen-cpm msg.c
	$(CHARMC) msg.c
