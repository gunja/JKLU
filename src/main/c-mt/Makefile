# JAVA_HOME=
#JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64/

all:   libsuperLUMTWrapper.so

libsuperLUMTWrapper.so: edu_ufl_cise_klu_wrapper_SuperLUMTWrapper.o libsuperlu_mt_PTHREAD.a
	gcc -shared -o $@ edu_ufl_cise_klu_wrapper_SuperLUMTWrapper.o  libsuperlu_mt_PTHREAD.a \
-Wl,--no-whole-archive  -Wl,-rpath,/usr/lib/x86_64-linux-gnu/ -L/usr/lib/x86_64-linux-gnu/  -lpthread -lm -lblas

edu_ufl_cise_klu_wrapper_SuperLUMTWrapper.o: edu_ufl_cise_klu_wrapper_SuperLUMTWrapper.c
	gcc -shared -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux" -D_LONGINT -I../../superlu-mt/SRC -fPIC -O3 -c $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o libsuperLUMTWrapper.so
