PY_FILES := $(wildcard *.py)
PY_FILES := $(filter-out deviceGeneration.py, $(PY_FILES))
DIRS := $(patsubst %.py, %, $(PY_FILES))

all: $(PY_FILES)
	for i in $^ ; do \
		python deviceGeneration.py -d $$i ; \
	done  ; \

% : %.py
	python deviceGeneration.py -d $^

realclean:
clean:
	rm -f *.pyc
	for i in $(DIRS) ; do \
		if [  -d $$i  ]; then \
			rm -rf $$i ; \
		fi ; \
	done  ; \

install:
all_install: $(DIRS)
	for i in $^ ; do \
		${ii}_install; \
	done  ; \


%_install: %
#	cp -rf $^/$^Src ../../../cpswAsynPortDriverApp/deviceDriversSrc/.
	cp -f $^/ui/$^.ui ../../ui/.
#	cp -f $^/Db/$^.template ../../../cpswAsynPortDriverApp/deviceDriversSrc/Db/.
#	cp -f $^/Db/$^.template ../../../cpswAsynPortDriverApp/Db/.
#	cp -f $^/deviceLibrary/$^.* ../../../cpswAsynPortDriverApp/deviceLibrarySrc/.
