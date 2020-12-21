SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
DEPS = $(OBJS:%.o=%.d)
BINS = $(subst src/,,$(OBJS))
BINS ::= $(subst .o,,$(BINS))

INCLUDELIBS =  # -I  -I libs/utils
#LDLIBS = -lutils -lm
LDLIBS =  -lm

LDFLAGS = #-L libs/utils
CFLAGS = -g -Wall -std=c++2a

#all: info libutils $(BINS)
all: $(BINS)

#$(BINS): $(OBJS)
#	$(CXX) -Wall $(LDFLAGS) -o $@ src/$@.o $(LDLIBS)

# Include all .d Makefiles
-include $(DEPS)


%: src/%.o Makefile
	$(CXX) -Wall $(LDFLAGS) -o $@ $< $(LDLIBS)

src/%.o: src/%.cpp
	$(CXX) $(CFLAGS) $(INCLUDELIBS) -MMD -c $< -o $@

.PHONY : clean

info:
	$(info $$(SRCS) = $(SRCS))
	$(info $$(OBJS) = $(OBJS))
	$(info $$(DEPS) = $(DEPS))
	$(info $$(BINS) = $(BINS))


#clean: libutilsclean 
clean: 
	-rm $(BINS) $(OBJS) $(DEPS)


#libutils:
#	$(MAKE) -C libs/utils

#libutilsclean:
#	$(MAKE) -C libs/utils clean
