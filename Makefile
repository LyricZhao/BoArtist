APP = boartist

OBJECTS = main.o
SOURCES = sources/*
OUTPUTS = outputs/*

CXXC = g++ # further icc for high performance
CXXFLAGS = -std=c++11 -O2
LINKFLAGS = 

COMMENTS = ''
DEL = rm -rf

default:
	make clean_objs -s
	make $(APP) -s

$(APP): $(OBJECTS) Makefile
	echo 'Linking: $(APP)' && \
	$(CXXC) $(LINKFLAGS) $(OBJECTS) -o $(APP)

$(APP).o: $(APP).cpp Makefile
	echo 'Compiling: $(APP).o:'	&& \
	$(CXXC) $(CXXFLAGS) -c $*.cpp -o $*.o

%.o: %.cpp %.h Makefile
	echo 'Compiling: $*.o' && \
	$(CXXC) $(CXXFLAGS) -c $*.cpp -o $*.o

clean_sources:
	echo 'Cleaning sources ...'
	-$(DEL) $(SOURCES)

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