APP = main

OBJECTS = main.o renderer.o sppm.o stb_image.o
SOURCES = sources/*
OUTPUTS = outputs/*
SCENES = scenes/debug.h scenes/bretesche.h scenes/pt_sky.h scenes/pig.h scenes/dinosaurs.h

CXXC = g++ # further icc for high performance
CXXFLAGS = -std=c++11 -O3 -fopenmp
LINKFLAGS = -O3 -fopenmp

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

stb_image.o: stb_image.cpp Makefile
	echo 'Compiling: stb_image.o' && \
	$(CXXC) $(CXXFLAGS) -c $*.cpp -o $*.o

%.o: %.cpp %.h Makefile $(SCENES)
	echo 'Compiling: $*.o' && \
	$(CXXC) $(CXXFLAGS) -c $*.cpp -o $*.o

run:
	make
	./$(APP)

clean:
	make clean_objs

sync:
	rsync --exclude ".vscode" --exclude "outputs" --exclude ".git" --exclude ".DS_Store" -azv ../BoArtist gorgon:~/

sync_outputs:
	rsync -azv gorgon:~/BoArtist/outputs ./

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