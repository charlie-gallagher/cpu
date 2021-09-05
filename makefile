# CPU Makefile

TARGET = cpu

INCDIR = include
SRCDIR = src
OBJDIR = obj
CC = gcc -c
LD = gcc -o

CFLAGS = -I$(INCDIR) -Wall -g3
LDFLAGS = -g3
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

# Every file depends on all headers
# If I understand this correctly, not efficient, but fine
DEPS = $(wildcard $(INCIDR)/*.h)

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET) $^

$(OBJDIR)/$(SRCDIR)/%.o : $(SRCDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJS) : | $(OBJDIR)/$(SRCDIR)

$(OBJDIR)/$(SRCDIR) : | $(OBJDIR)
	mkdir $@

$(OBJDIR):
	mkdir $@

.PHONY : clean

clean :
	-rm -f $(OBJDIR)/$(SRCDIR)/*.o
