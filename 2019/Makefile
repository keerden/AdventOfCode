OBJDIR = build
SRCDIR := src

#$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(CFILES:.c=.o))



SRCS = $(shell find $(SRCDIR) -type f -name "*.cpp")
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(OBJS:%.o=%.d)
BIN = solution

LDLIBS =  -lm

# enabled compiler warnings
WARNINGS := -Wall -Wextra

CFLAGS = -g $(WARNINGS) -std=c++2a

all: $(BIN)

$(BIN): $(OBJS) Makefile
	$(CXX) -Wall $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)





$(OBJDIR)/%.o: $(SRCDIR)/%.cpp Makefile
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) $(INCLUDELIBS) -MMD -c $< -o $@



info:
	$(info $$(SRCS) = $(SRCS))
	$(info $$(OBJS) = $(OBJS))
	$(info $$(DEPS) = $(DEPS))
	$(info $$(BIN) = $(BIN))


#clean: libutilsclean 
clean: 
	@rm -f $(OBJS) $(DEPS)

clean-all: clean
	@rm -f $(BIN)
	@rm -rf $(OBJDIR)

# Include all .d Makefiles
-include $(DEPS)

.PHONY : clean info
