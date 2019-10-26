CC := g++
CFLAGS := -Wall -pedantic -Wno-long-long -std=c++11 -DLINUX_ENV=1
LIBS := -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
SRCDIR := src
OBJDIR := objects
BINARYNAME := hrabajak
SRCS := $(shell ls $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

compile: directories $(BINARYNAME)

all: compile doc

directories: 
	@mkdir -p objects

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(LIBS) $(CFLAGS) -c $< -o $@
	
$(BINARYNAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBS) -o $(BINARYNAME)
	
doc:
	@doxygen Doxyfile
	@exit 0
	
run:
	@./$(BINARYNAME) examples/locomotive edit
	@exit $?
	
clean:
	@rm -rf $(OBJDIR)
	@rm -f doxygen_sqlite3.db
	@rm -f $(BINARYNAME)
	@exit 0

.PHONY: doc run clean directories
