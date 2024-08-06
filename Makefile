BUILD_DIR=build
CMAKE_SRC=.

MAKEFLAGS += --no-print-directory

.PHONY:
all: CMakeLists.txt
	@cmake --build $(BUILD_DIR)

.PHONY:
configure: CMakeLists.txt
	@cmake -S $(CMAKE_SRC) -B $(BUILD_DIR) 

.PHONY:
clean: CMakeLists.txt
	@cmake --build $(BUILD_DIR) --target clean
