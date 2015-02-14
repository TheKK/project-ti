CXX = g++
CXX_FLAGS = -std=c++11 -g -Wall

SRC_DIR = src
INC_DIR = inc
OUT_DIR = out

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
INCS = $(addprefix -I, $(INC_DIR))
OBJS = $(addprefix $(OUT_DIR)/, $(SRCS:.cpp=.o))
DEPS = $(SRCS:.cpp=.d)

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -ljsoncpp

OUT_EXE = map

$(OUT_EXE): $(OBJS)
	@echo "[LD]	$(notdir $@)"
	@$(CXX) $^ $(CXX_FLAGS) $(LIBS) -o $@

$(OUT_DIR)/%.o: %.cpp
	@echo "[CXX]	$(notdir $@)"
	@mkdir -p $(dir $@)
	@$(CXX) $^ $(CXX_FLAGS) $(INCS) -c -o $@

.PHONY: clean
clean:
	@rm -frv $(OBJ) $(OUT_EXE)

todo:
	@echo "[TODO]"
	@grep -r TODO .
	@echo
	@echo "[FIXME]"
	@grep -r FIXME .
	@echo
	@echo "[XXX]"
	@grep -r XXX .
