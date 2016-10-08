APP_SRCS	= ${wildcard *.cpp}

APP_OBJS = $(APP_SRCS:.cpp=.o)

OBJS = $(APP_OBJS)
SRCS = $(APP_SRCS)
#LIBS += -lpthread 
Include += -I /usr/lib/jvm/java-7-openjdk-amd64/include
Include += -I /usr/lib/jvm/java-7-openjdk-amd64/include/linux
Include += `pkg-config --cflags opencv`
LIBS += `pkg-config --libs opencv`
MAKEFILE = Makefile

CC = g++

all: libocr_java.so

libocr_java.so: $(OBJS)
	$(CC) -shared -fPIC -o $@ $(OBJS) $(LIBS)
$(APP_OBJS): %.o: %.cpp
	$(CC) -c -fPIC $(Include) $*.cpp

clean:
	rm -f $(OBJS) libocr_java.so
rebuild:
	make clean
	make
