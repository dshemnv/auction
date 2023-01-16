TARGETAPP := auction
CC := gcc

IDIR := include
LDIR := lib
SRCDIR := src
BUILDDIR := bin
 
IFLAGS := $(addprefix -I,$(IDIR))
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(subst $(SRCDIR),$(BUILDDIR),$(SRCS:.c=.o))
DEPFILES := $(OBJS:%.o=%.d)

CFLAGS := $(IFLAGS) -Wall -MMD -MP

all: $(SRCS) $(BUILDDIR)/$(TARGETAPP)
run: all
	./$(BUILDDIR)/$(TARGETAPP)

$(BUILDDIR)/$(TARGETAPP): $(OBJS)
	$(CC) $(OBJS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILDDIR)

-include $(wildcard $(DEPFILES))