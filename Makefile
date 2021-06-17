
.PHONY:clean

all: demo hello
	@echo done.

hello:hello.cpp glQuickText.h
	c++ -o hello -I. hello.cpp -lglfw -lGLU -lGL

demo:demo.cpp glQuickText.h
	c++ -o demo -I. demo.cpp -lglfw -lGLU -lGL -lm

clean:
	rm -f demo hello

