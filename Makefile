TARGETAPP := auction
CC := gcc

IDIR := include
LDIR := lib
SRCDIR := src
BUILDDIR := bin
 
IFLAGS := $(addprefix -I,$(IDIR))
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(subst $(SRCDIR),$(BUILDDIR),$(SRCS:.cpp=.o))
DEPFILES := $(OBJS:%.o=%.d)
LIBS := -lm

CFLAGS := $(IFLAGS) -Wall -MMD -MP -g
VALFLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes 

all: $(SRCS) $(BUILDDIR)/$(TARGETAPP)
run: all
	./$(BUILDDIR)/$(TARGETAPP)
check: all
	colour-valgrind $(VALFLAGS) $(BUILDDIR)/$(TARGETAPP) 

$(BUILDDIR)/$(TARGETAPP): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILDDIR)

-include $(wildcard $(DEPFILES))