compiler = g++

flags = -c -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal 								\
		-Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs 									\
		-Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe 										\
		-fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body 										\
		-Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op 												\
		-Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith 					\
		-Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits 						\
		-Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

includes = C:\Users\eremc\.vscode\Tests\TX\TXLib.h 																	\
           ../Includes/Aggregate.h ./inc/Assembler.h ./inc/AssemblerFuncs.h 										\
		   ../Stack/inc/Utils.h ../Stack/inc/Stack.h ../Stack/inc/Defines.h ../Stack/inc/Errors.h					\

sources = ./src/main.cpp ../Stack/src/Utils.cpp ../Stack/src/Stack.cpp

objects = $(sources:.cpp=.o)

execute = Stack.exe

all: $(sources) $(execute) run

run:
	@$(execute)
	@echo -e -en '\E[;32m'"\n\t\t\t\033[1mRunning...\t\t\t\033[0m\n\n"
	
$(execute): $(objects) 
	$(compiler) $(objects) -o $@

.cpp.o: $(includes)
	@$(compiler) $(flags) $< -o $@

clean:
	rm -rf ./src/*.o
	rm -rf *.exe