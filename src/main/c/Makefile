# JAVA_HOME=
#JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64/

all:   libsuperLUWrapper.so

libsuperLUWrapper.so: edu_ufl_cise_klu_wrapper_SuperLUWrapper.o edu_ufl_cise_klu_wrapper_SuperMatrix.o
	gcc -shared -o $@ $^ -Wl,-rpath,/usr/lib/x86_64-linux-gnu/ -L/usr/lib/x86_64-linux-gnu/ -lsuperlu

edu_ufl_cise_klu_wrapper_SuperLUWrapper.o: edu_ufl_cise_klu_wrapper_SuperLUWrapper.c
	gcc -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux" -I/usr/include/superlu/ -fPIC -O2 -c $^ -o $@

edu_ufl_cise_klu_wrapper_SuperMatrix.o: edu_ufl_cise_klu_wrapper_SuperMatrix.c
	gcc -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux" -I/usr/include/superlu/ -fPIC -O2 -c $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o libsuperLUWrapper.so
