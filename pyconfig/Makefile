#!/usr/bin/make -f

CXX=g++

CFLAGS=-g
FMT_CFLAGS=-DFMT_SHARED

LDFLAGS=-g -Wl,--no-as-needed
FMT_LDFLAGS=/usr/lib/libfmt.so

BOOST_CFLAGS=-DBOOST_PYTHON_DYN_LINK -DBOOST_PYTHON_NO_LIB
BOOST_LDFLAGS=/usr/lib/libboost_python312.so

PYTHON_CFLAGS_312=-isystem /usr/include/python3.12
PYTHON_LDFLAGS_312=/usr/lib/libpython3.12.so

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(patsubst %/,%,$(dir $(mkfile_path)))
PYEMBED_PLAYGROUND_PATH := $(patsubst %/,%,$(dir $(mkfile_dir)))

execs = out/pyconfig_test1_3.8 \
        out/pyconfig_test1_3.9 \
        out/pyconfig_test1_3.10 \
        out/pyconfig_test1_3.11 \
        out/pyconfig_test1_3.12 \
        out/pyconfig_test2 \
        out/pyconfig_test3 \
        out/pyconfig_test4 \
        out/pyconfig_test5_3.5m \
        out/pyconfig_test5_3.6m \
        out/pyconfig_test5_3.7m \
        out/pyconfig_test5_3.8 \
        out/pyconfig_test5_3.9 \
        out/pyconfig_test5_3.10 \
        out/pyconfig_test5_3.11 \
        out/pyconfig_test5_3.12 \
        out/pyconfig_test6 \
        out/pyconfig_test7_3.8 \
        out/pyconfig_test7_3.9 \
        out/pyconfig_test7_3.10 \
        out/pyconfig_test7_3.11 \
        out/pyconfig_test7_3.12 \
        out/pyconfig_test8_3.5m \
        out/pyconfig_test8_3.6m \
        out/pyconfig_test8_3.7m \
        out/pyconfig_test8_3.8 \
        out/pyconfig_test8_3.9 \
        out/pyconfig_test8_3.10 \
        out/pyconfig_test8_3.11 \
        out/pyconfig_test8_3.12 \
        out/pyconfig_test9_3.5m \
        out/pyconfig_test9_3.6m \
        out/pyconfig_test9_3.7m \
        out/pyconfig_test9_3.8 \
        out/pyconfig_test9_3.9 \
        out/pyconfig_test9_3.10 \
        out/pyconfig_test9_3.11 \
        out/pyconfig_test9_3.12

all: $(execs)

clean:
	rm -rf out

out:
	mkdir -p out

out/pyconfig_test1_%: main.cpp out
	${CXX} \
	-std=c++20 \
	-Wno-write-strings \
	${CFLAGS} \
	-DPYEMBED_PLAYGROUND_PATH="\"$(PYEMBED_PLAYGROUND_PATH)\"" \
	-DPYEMBED_PLAYGROUND_PATH_L="L\"$(PYEMBED_PLAYGROUND_PATH)\"" \
	${FMT_CFLAGS} \
	-isystem /usr/include/python$(*F) \
	${LDFLAGS} \
	${FMT_LDFLAGS} \
	/usr/lib/libpython$(*F).so \
	-o $@ $<

out/pyconfig_test2: main2.cpp out
	${CXX} \
	-std=c++20 \
	${CFLAGS} \
	${FMT_CFLAGS} \
	${BOOST_CFLAGS} \
	${PYTHON_CFLAGS_312} \
	${LDFLAGS} \
	${FMT_LDFLAGS} \
	${BOOST_LDFLAGS} \
	${PYTHON_LDFLAGS_312} \
	-o $@ $<

out/pyconfig_test3: main3.cpp out
	${CXX} \
	-std=c++20 \
	${CFLAGS} \
	${BOOST_CFLAGS} \
	${PYTHON_CFLAGS_312} \
	${LDFLAGS} \
	${BOOST_LDFLAGS} \
	${PYTHON_LDFLAGS_312} \
	-o $@ $<

out/pyconfig_test4: main4.cpp out
	${CXX} \
	-std=c++20 \
	${CFLAGS} \
	${BOOST_CFLAGS} \
	${PYTHON_CFLAGS_312} \
	${LDFLAGS} \
	${BOOST_LDFLAGS} \
	${PYTHON_LDFLAGS_312} \
	-o $@ $<

out/pyconfig_test5_%: main5.cpp out
	${CXX} \
	-std=c++20 \
	-Wno-write-strings \
	${CFLAGS} \
	${FMT_CFLAGS} \
	-isystem /usr/include/python$(*F) \
	${LDFLAGS} \
	${FMT_LDFLAGS} \
	/usr/lib/libpython$(*F).so \
	-o $@ $<

out/pyconfig_test6: main6.cpp out
	${CXX} \
	-std=c++20 \
	${CFLAGS} \
	-DPYEMBED_PLAYGROUND_PATH="\"$(PYEMBED_PLAYGROUND_PATH)\"" \
	-DPYEMBED_PLAYGROUND_PATH_L="L\"$(PYEMBED_PLAYGROUND_PATH)\"" \
	${FMT_CFLAGS} \
	${BOOST_CFLAGS} \
	${PYTHON_CFLAGS_312} \
	${LDFLAGS} \
	${FMT_LDFLAGS} \
	${BOOST_LDFLAGS} \
	${PYTHON_LDFLAGS_312} \
	-o $@ $<

out/pyconfig_test7_%: main7.cpp out
	${CXX} \
	-std=c++20 \
	-Wno-write-strings \
	${CFLAGS} \
	-DPYEMBED_PLAYGROUND_PATH="\"$(PYEMBED_PLAYGROUND_PATH)\"" \
	-DPYEMBED_PLAYGROUND_PATH_L="L\"$(PYEMBED_PLAYGROUND_PATH)\"" \
	-DPY_VER_WITH_ABI=\"$(*F)\" \
	${FMT_CFLAGS} \
	-isystem /usr/include/python$(*F) \
	${LDFLAGS} \
	${FMT_LDFLAGS} \
	/usr/lib/libpython$(*F).so \
	-o $@ $<

out/pyconfig_test8_%: main8.cpp out
	${CXX} \
	-std=c++20 \
	-Wno-write-strings \
	${CFLAGS} \
	-DPYEMBED_PLAYGROUND_PATH="\"$(PYEMBED_PLAYGROUND_PATH)\"" \
	${FMT_CFLAGS} \
	-isystem /usr/include/python$(*F) \
	${LDFLAGS} \
	${FMT_LDFLAGS} \
	/usr/lib/libpython$(*F).so \
	-o $@ $<

out/pyconfig_test9_%: main9.cpp out
	${CXX} \
	-std=c++20 \
	-Wno-write-strings \
	${CFLAGS} \
	-DPYEMBED_PLAYGROUND_PATH="\"$(PYEMBED_PLAYGROUND_PATH)\"" \
	-DPY_VER_WITH_ABI=\"$(*F)\" \
	${FMT_CFLAGS} \
	-isystem /usr/include/python$(*F) \
	${LDFLAGS} \
	${FMT_LDFLAGS} \
	/usr/lib/libpython$(*F).so \
	-o $@ $<
