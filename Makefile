CXX := g++

SRC_DIR := src
INC_DIR := inc
OBJ_DIR := out

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.cpp=.o))
DEP_FILES := $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.cpp=.d))

LDLIBS := -lSDL2 -lSDL2_image -lSDL2_ttf -ljsoncpp
LDFLAGS :=

CXXFLAGS := -std=c++11 -g -Wall -MMD -MP -I $(INC_DIR)

EXECUTABLE := map

# Commands
.PHONY: all clean todo
all: $(EXECUTABLE)

clean:
	@rm -frv $(OBJ_DIR) $(EXECUTABLE)

todo:
	@echo "[TODO]"
	@grep -r TODO .
	@echo
	@echo "[FIXME]"
	@grep -r FIXME .
	@echo
	@echo "[XXX]"
	@grep -r XXX .

ifneq ($(MAKECMDGOALS), clean)
-include $(DEP_FILES)
endif

# Recipes
$(EXECUTABLE): $(OBJ_FILES)
	@echo "[LD]	$(notdir $@)"
	@$(CXX) $^ $(CXXFLAGS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: %.cpp
	@echo "[CXX]	$(notdir $@)"
	@mkdir -p $(dir $@)
	@$(CXX) $< $(CXXFLAGS) -c -o $@
