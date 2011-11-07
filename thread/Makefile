TARGETS  = UserThread UserThread2
SRCS     = UserThread.cpp UserThread2.cpp
OBJS     = $(SRCS:.cpp=.o)

CXXFLAGS = -g
LDFLAGS  = -g

.PHONY: all clean
all: $(TARGETS)

$(TARGETS):
	$(CXX) -o $@ $^ $(LDFLAGS)

.cpp.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f $(OBJS) $(TARGETS)

# dependencies
UserThread.o: UserThread.cpp
UserThread: UserThread.o

UserThread2.o: UserThread2.cpp
UserThread2: UserThread2.o
