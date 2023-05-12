PY_EXT_SUFFIX := $(shell python -c 'import sysconfig; print(sysconfig.get_config_var("EXT_SUFFIX"))')
PY_INCLUDE := $(shell python -c 'import sysconfig; print(sysconfig.get_path("data"))')/include
PY_LIBDIR := $(shell python -c 'import sysconfig; print(sysconfig.get_config_var("LIBDIR"))')
PY_PLATINCLUDE := $(shell python -c 'import sysconfig; print(sysconfig.get_path("platinclude"))')
BUILD_DIR := build
HPP_DIR := src/libHellweg2D
H_DIR := src/physics
SRC_PATH := $(H_DIR):$(HPP_DIR)
vpath %.cpp $(SRC_PATH)
INCLUDES := $(wildcard $(HPP_DIR)/*.hpp) $(wildcard $(H_DIR)/*.h)
SRC := $(foreach d, $(subst :, ,$(SRC_PATH)), $(wildcard $(d)/*.cpp)) pyhellweg.cpp
OBJ :=$(addprefix $(BUILD_DIR)/,$(notdir $(SRC:%.cpp=%.o)))
TGT := $(BUILD_DIR)/pyhellweg$(PY_EXT_SUFFIX)
INSTALL_DIR :=  rshellweg
LOCAL_INCLUDE := $(HOME)/.local/include
CPPFLAGS := -fdiagnostics-color=never -pthread -Wno-unused-result -Wsign-compare -DNDEBUG -g -O0 -fwrapv -Wall -fPIC -DRSHELLWEG_LINUX=1 -I$(HPP_DIR) -I$(H_DIR) -I$(PY_PLATINCLUDE) -I$(PY_INCLUDE) -I$(LOCAL_INCLUDE) -std=c++11

LDFLAGS := -shared -L$(PY_LIBDIR)

all: $(TGT)

install: all
	install -m 555 $(TGT) $(INSTALL_DIR)/$(notdir $(TGT))

clean:
	rm -rf $(BUILD_DIR) rshellweg/pyhellweg*so

pyhellweg.cpp: pyhellweg.pyx
	cythonize $^

$(OBJ): $(INCLUDES) | $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: %.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(TGT): $(OBJ)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@
