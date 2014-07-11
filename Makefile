# Makefile created by N.Richard
# Cours Multi-threading EPSI
# Version 1.7 

CC=g++
PREFIX = .
SRCDIR = $(PREFIX)/src
INCDIR = $(PREFIX)/include
#un répertoire n'est pas intéressant car les chemins (relatif) posent souvent problèmes
EXEC = blur
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)
DEPENDS = $(SRC:.cpp=.d)
#CFLAGS += `pkg-config --cflags sdl SDL_image` -fopenmp -O3 -pedantic -Wall -Wextra
CXXFLAGS += `pkg-config --cflags sdl SDL_image SDL_ttf SDL_gfx` -fopenmp -O3 -Wall -Wno-narrowing -pedantic -Wextra -Woverloaded-virtual -Wfloat-equal -Wpointer-arith -Wshadow -Weffc++ -Wredundant-decls -Winit-self -Wswitch-default -Wundef -Wlong-long -std=c++11 -Werror#-Wconversion # #
LDFLAGS += `pkg-config --libs sdl SDL_image SDL_ttf SDL_gfx` -fopenmp -lm #-lpthread -D_REENTRANT
 
.PHONY : run clean mrproper

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS) 
	@printf " \033[1;32mCompilation success !\033[0m\n"

%.d: %.cpp
	@$(CC) -MM $< > $@ 

-include $(DEPENDS)

run: 
	@$(MAKE) && ./$(EXEC) ./img/testing.jpg

clean:
	@printf "  \033[1;31m\033[4mRemoved files\033[0m\033[31m :\n"
	@printf "\t*~ a.out core *.d \n\t$(OBJ)\033[0m\n"
	@$(RM) -rf *~ a.out core $(DEPENDS) $(OBJ)

mrproper: clean
	@printf "\033[31m\t$(EXEC)\033[0m\n"
	@$(RM) -rf $(EXEC)
