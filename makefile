TARGET = app
CXX = g++
PKG_CONFIG = pkg-config

SDL3_CFLAGS = $(shell $(PKG_CONFIG) --cflags sdl3)
SDL3_LIBS   = $(shell $(PKG_CONFIG) --libs sdl3)

# diretórios de include
INC_DIRS = include
SYS_INC_DIRS = external

CXXFLAGS = -std=c++17 -Wall -Wextra \
           $(addprefix -I,$(INC_DIRS)) \
           $(addprefix -isystem ,$(SYS_INC_DIRS)) \
           $(SDL3_CFLAGS)

SRC_DIR = src
BUILD_DIR = build

# procura recursivamente todos os .cpp
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
# gera os .o correspondentes em build/
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# regra padrão
all: $(BUILD_DIR)/$(TARGET)

# linkagem final
$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@ $(SDL3_LIBS)

# compila cada .cpp -> .o (cria pastas em build conforme necessário)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# limpeza
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
