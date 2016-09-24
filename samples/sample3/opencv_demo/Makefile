APP_SRCS	= ${wildcard *.cpp}

APP_OBJS = $(APP_SRCS:.cpp=.o)

OBJS = $(APP_OBJS)
SRCS = $(APP_SRCS)
Include += `pkg-config --cflags opencv`
LIBS += `pkg-config --libs opencv`
LIBS += -L ./lib
MAKEFILE = Makefile

CC = g++

all: demo_opencv

demo_opencv: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)
$(APP_OBJS): %.o: %.cpp
	$(CC) -c -fPIC  $*.cpp $(Include)

clean:
	rm -f $(OBJS) demo_opencv
rebuild:
	make clean
	make
