TARGET=executavel
CXX=gcc
LD=gcc
OBJS=main.c 
executavel:: clean $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) -lcurses  -g 
clean:
	rm -rf *.o executavel