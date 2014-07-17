# Makefile created by N.Richard
# Cours Multi-threading EPSI
# Version 2.4

CC := g++	
PREFIX := .
SRCDIR := $(PREFIX)/src
INCDIR := $(PREFIX)/lib
#un répertoire pour le binaire n'est pas intéressant car les chemins (relatif) posent souvent problèmes
EXEC := blur
SRC := $(wildcard $(SRCDIR)/*.cpp)
HEADERS := $(wildcard $(SRCDIR)/*.h)
OBJ := $(SRC:.cpp=.o)
#NDEBUG = RELEASE
#-Wfatal-errors : le compilateur s'arrêtera à la 1ère erreur rencontrée (ici un simple warning)
#Volontairement omis pour des raisons de performances : -Wconversion -Winline -Wsign-compare (-Wall)
CXXFLAGS += `sdl-config --cflags` -O3 -fopenmp #-pipe -std=c++11
CXX_RELEASE_FLAGS := -DNDEBUG  -march=native -fstack-protector --param=ssp-buffer-size=4
CXX_DEBUG_FLAGS := -pedantic -Wall -Wno-narrowing -Wextra -Woverloaded-virtual \
	-Wwrite-strings -Wno-variadic-macros -Wno-unused-parameter -Wno-unused-variable -Wvolatile-register-var -Wunsafe-loop-optimizations -Wcast-qual \
	-Wunknown-pragmas -Wmissing-include-dirs -Wstack-protector -Wfloat-equal -Wstrict-null-sentinel \
	-Wpointer-arith -Wredundant-decls -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlong-long -Werror  \
	-Weffc++ -Wold-style-cast -Wcast-align -Wdouble-promotion -Wlogical-op -Wfatal-errors -Wno-sign-compare

GCC_VER_MAJOR := $(shell $(CC) -dumpversion | cut -f1 -d.)
GCC_VER_MINOR := $(shell $(CC) -dumpversion | cut -f2 -d.)
GCC_VER_GT_4_9 := $(shell test $(GCC_VER_MAJOR) -gt 4 -o \( $(GCC_VER_MAJOR) -eq 4 -a $(GCC_VER_MINOR) -ge 9 \) && echo true)

ifeq ($(GCC_VER_GT_4_9),true)
	CXXFLAGS += -fdiagnostics-color=auto -fopenmp-simd
endif

#CFLAGS += -O3 -pedantic -Wall -Wextra -fopenmp `pkg-config --cflags sdl SDL_image SDL_ttf SDL_gfx`
LDFLAGS += `sdl-config --libs` -lSDL_Image -lSDL_TTF -lSDL_gfx -fopenmp -lm -lpthread -D_REENTRANT

.PHONY : all debug release run clean mrproper

all: release

debug: CXXFLAGS += $(CXX_DEBUG_FLAGS) 
debug: $(EXEC)

release: CXXFLAGS += $(CXX_RELEASE_FLAGS) 
release: $(EXEC)

$(EXEC): $(OBJ) $(HEADERS)
	$(CC) $^ -o $@ $(LDFLAGS) 
	@printf " \033[1;32mCompilation success !\033[0m\n"

%.o: %.c %.h
	$(CC) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

run: 
	@$(MAKE) && ./$(EXEC) ./img/testing.jpg

clean:
	@printf "  \033[1;31m\033[4mRemoved files\033[0m\033[31m :\n"
	@printf "\t*~ a.out core *.gch\n\t$(OBJ)\033[0m\n"
	@$(RM) -rf *~ a.out core $(SRCDIR)/*.gch $(OBJ)

mrproper: clean
	@printf "\033[31m\t$(EXEC)\033[0m\n"
	@$(RM) -rf $(EXEC)
