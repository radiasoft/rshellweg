TGT_DIR := lib
HPP_DIR := src/libHellweg2D
H_DIR := src/physics
SRC_PATH := $(H_DIR):$(HPP_DIR)
vpath %.cpp $(SRC_PATH)
INCLUDES := $(wildcard $(HPP_DIR)/*.hpp) $(wildcard $(H_DIR)/*.h)
SRC := $(foreach d, $(subst :, ,$(SRC_PATH)), $(wildcard $(d)/*.cpp)) $(TGT_DIR)/pyhellweg.cpp
OBJ :=$(addprefix $(TGT_DIR)/,$(notdir $(SRC:%.cpp=%.o)))
TGT := $(TGT_DIR)/pyhellweg.cpython-37m-x86_64-linux-gnu.so
INSTALL_DIR :=  $(shell python -c 'import sysconfig; print(sysconfig.get_path("purelib"))')/rslinac
PY_PLATINCLUDE := $(shell python -c 'import sysconfig; print(sysconfig.get_path("platinclude"))')
PY_INCLUDE := $(shell python -c 'import sysconfig; print(sysconfig.get_path("data"))')/include
LOCAL_INCLUDE := $(HOME)/.local/include
CPPFLAGS := -pthread -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall -fPIC -DRSLINAC=1 -I$(HPP_DIR) -I$(H_DIR) -I$(PY_PLATINCLUDE) -I$(PY_INCLUDE) -I$(LOCAL_INCLUDE) -std=c++11

LDFLAGS := -shared -L/home/vagrant/.pyenv/versions/3.7.2/lib #TODO(robnagler) needs to be pulled from setup.py; do not include $CPPFLAGS

all: $(TGT)

install: all
	install -m 555 $(TGT) $(INSTALL_DIR)/$(notdir $(TGT))

clean:
	rm -rf $(TGT_DIR)

$(TGT_DIR)/pyhellweg.cpp: pyhellweg.pyx
	cython --cplus $^ -o $@

$(OBJ): $(INCLUDES) $(TGT_DIR)

$(TGT_DIR):
	mkdir $@

$(TGT_DIR)/%.o: %.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(TGT): $(OBJ)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@