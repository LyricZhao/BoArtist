APP = main

OBJECTS = main.o renderer.o
SOURCES = sources/*
OUTPUTS = outputs/*

CXXC = g++-8 # further icc for high performance
CXXFLAGS = -std=c++11 -O3 -fopenmp
LINKFLAGS = -O3 -fopenmp

COMMENTS = ''
DEL = rm -rf

default:
	make clean_objs
	make $(APP)

$(APP): $(OBJECTS) Makefile
	echo 'Linking: $(APP)' && \
	$(CXXC) $(LINKFLAGS) $(OBJECTS) -o $(APP)

$(APP).o: $(APP).cpp Makefile
	echo 'Compiling: $(APP).o:'	&& \
	$(CXXC) $(CXXFLAGS) -c $*.cpp -o $*.o

%.o: %.cpp %.h Makefile
	echo 'Compiling: $*.o' && \
	$(CXXC) $(CXXFLAGS) -c $*.cpp -o $*.o

run:
	make
	./$(APP)

clean:
	make clean_objs
	make clean_outputs

clean_outputs:
	echo 'Cleaning outputs ...'
	-$(DEL) $(OUTPUTS)

clean_objs:
	echo 'Cleaning objects ...'
	-$(DEL) *.o
	-$(DEL) $(APP)

push:
	make clean_objs -s
	echo 'Comments: $(COMMENTS)'
	git add .gitignore
	git add *
	git commit -m "$(COMMENTS)"
	git push origin master