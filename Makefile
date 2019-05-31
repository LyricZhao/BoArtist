APP = main

OBJECTS = main.o renderer.o
SOURCES = sources/*
OUTPUTS = outputs/*

CXXC = g++-9 # further icc for high performance
CXXFLAGS = -std=c++11 -O3
LINKFLAGS = -O3

COMMENTS = ''
DEL = rm -rf

default:
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

sync:
	rsync --exclude ".vscode" --exclude ".git" --exclude ".DS_Store" -azv ../BoArtist im:~/

sync_output:
	rsync -azv im:~/BoArtist/outputs ./

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