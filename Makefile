FILES = main lexer 
OBJECTS = $(addsuffix .o,$(FILES))
DEPENDANCE = $(addsuffix .d,$(FILES))

SOURCES_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

DEPENDANCE_PATH = $(addprefix $(BUILD_DIR)/,$(DEPENDANCE))
OBJECTS_PATH = $(addprefix $(BUILD_DIR)/,$(OBJECTS))

CXX = clang++
CXXFLAGS := -I$(INCLUDE_DIR) -std=c++11

.PHONY: clean debug

main: $(OBJECTS_PATH)
	$(CXX) $(CXXFLAGS) $^ -o $@

debug: CXXFLAGS := $(CXXFLAGS) -g
debug: main

$(BUILD_DIR)/%.o : $(SOURCES_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.d : $(SOURCES_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MM -MT $(BUILD_DIR)/$*.o $< > $@

clean:
	$(RM) $(DEPENDANCE_PATH) $(OBJECTS_PATH)

include $(DEPENDANCE_PATH)
