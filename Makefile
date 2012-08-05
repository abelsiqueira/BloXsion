CXXFLAGS = -Wall -Wextra -ggdb
LIB = -lallegro_ttf -lallegro_font -lallegro_primitives -lallegro \
			-lallegro_audio -lallegro_acodec -lallegro_image

EXEC = main
TRUECOLORS = -DTRUE_COLORS

OBJS = gameclass_update.o gameclass_draw.o gameclass.o main.o

all: $(OBJS)
	g++ -o $(EXEC) $(OBJS) $(LIB) $(CXXFLAGS) $(TRUECOLORS)

%.o: %.cpp
	g++ -c -o $@ $< $(CXXFLAGS) $(TRUECOLORS)

clean:
	rm -f *.o

purge: clean
	rm -f $(EXEC)
