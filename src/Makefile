CC := g++
CFLAGS := -Wall -pedantic -Wno-long-long -std=c++11 -DLINUX_ENV=1
LIBS := -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
OBJDIR := objects
SRCS := $(shell ls *.cpp)
OBJS := $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

all: directories dzejkobdraw

directories: 
	@mkdir -p objects

$(OBJDIR)/%.o: %.cpp
	$(CC) $(LIBS) $(CFLAGS) -c $< -o $@
	
dzejkobdraw: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBS) -o dzejkobdraw
	
clean:
	rm -rf $(OBJDIR)
