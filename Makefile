CC = gcc
CFLAGS = -Iinclude -Ithird_party/stb -I$(SRCDIR) -Wall -Wextra
LDFLAGS =
OBJDIR = obj
SRCDIR = src
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
EXECUTABLE = $(BINDIR)/ImageProcessor

all: $(EXECUTABLE)

$(OBJDIR):
	-mkdir $(OBJDIR) 2>NUL || echo "obj directory exists"
	
$(BINDIR):
	-mkdir $(BINDIR) 2>NUL || echo "bin directory exists"

$(EXECUTABLE): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rmdir /S /Q $(OBJDIR) $(BINDIR) 2>NUL || echo "Cleaned"

.PHONY: all clean $(OBJDIR) $(BINDIR)
