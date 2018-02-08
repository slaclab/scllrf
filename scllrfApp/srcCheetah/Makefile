PY_FILES := $(wildcard *.py)
PY_FILES := $(filter-out deviceGeneration.py, $(PY_FILES))
PY_FILES := $(filter-out process_regmap.py, $(PY_FILES))
DIRS := $(patsubst %.py, %, $(PY_FILES))

all: $(PY_FILES)
	for i in $^ ; do \
		python deviceGeneration.py -d $$i ; \
	done  ; \
	python deviceGeneration.py -c scllrfPRC -i live_prc_regmap.json
	python deviceGeneration.py -c Gun -i regmapold.json

% : %.py
	python deviceGeneration.py -d $^

realclean: clean
clean:
	rm -f *.pyc
	for i in $(DIRS) ; do \
		if [  -d $$i  ]; then \
			rm -rf $$i ; \
		fi ; \
	done  ; \

copy: all
	cp */src/*.h ../src/;
	cp */src/*.cpp ../src/;
	cp */ui/* ../../display/.;
	cp */Db/*.template ../Db/.;

all_install: $(DIRS)
	for i in $^ ; do \
		${ii}_install; \
	done  ; \

#We don't want this to run every time we build the generated code etc., so make a dummy install target
install:
# Or use this one if you want it to generate and copy code every time
#install: copy


%_install: %
	for i in $(wildcard */) ; do \
		if [  -d $$i  ]; then \
	cp -rf $^/src/*.h ../src/.
	cp -rf $^/src/*.cpp ../src/.
	cp -f $^/ui/* ../../display/.
#	cp -f $^/Db/$^.template ../../../cpswAsynPortDriverApp/deviceDriversSrc/Db/.
	cp -f $^/Db/$^.template ../Db/.
#	cp -f $^/deviceLibrary/$^.* ../../../cpswAsynPortDriverApp/deviceLibrarySrc/.
		fi ; \
	done  ; \
