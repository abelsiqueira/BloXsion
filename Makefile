CXXFLAGS = -Wall -Wextra -ggdb
LIB = -lallegro_ttf -lallegro_font -lallegro_primitives -lallegro \
			-lallegro_audio -lallegro_acodec -lallegro_image

EXEC = main

OBJS = gameclass_draw.o gameclass.o main.o

all: $(OBJS)
	g++ -o $(EXEC) $(OBJS) $(LIB) $(CXXFLAGS)

%.o: %.cpp
	g++ -c -o $@ $< $(CXXFLAGS)

clean:
	rm -f *.o

purge: clean
	rm -f $(EXEC)
