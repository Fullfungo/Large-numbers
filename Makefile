all:: build

Compiler := g++-10

Program_name := executable.exe

#Header_files := $(wildcard *.hpp)
Source_files := $(wildcard *.cpp)
Object_files := $(patsubst %.cpp,%.o,$(Source_files))

Dependency := $(patsubst %.cpp,%.d,$(Source_files))


Supressing_flags := -Wno-unused-value -Wno-error=unused-value -Wno-unused-parameter
Flags := -std=c++2a -pedantic -Wall -Wextra -Werror -g $(Supressing_flags)
Dependency_flags := -MMD -MP

$(Program_name): $(Object_files)
	$(Compiler) $(Flags) $^ -o $@

run:: $(Program_name)
	./$(Program_name)

build:: $(Program_name)

-include $(Dependency)

%.o: %.cpp Makefile
	$(Compiler) $(Flags) $(Dependency_flags) -c $<

clean::
	rm -f *.s
	rm -f *.o
	rm -f *.d
	rm -f *.gch
	rm -f $(Program_name)

rerun:: clean run

rebuild:: clean build
