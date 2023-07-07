RACK_DIR ?= ../..
FLAGS +=
SOURCES += $(wildcard src/*.cpp)
SOURCES += $(wildcard src/util/*.cpp)
SOURCES += $(wildcard src/test/*.cpp)
SOURCES += $(wildcard src/modules/*.cpp)
SOURCES += $(wildcard src/modules/*/*.cpp)
SOURCES += $(wildcard src/modules/*/*/*.cpp)
DISTRIBUTABLES += $(wildcard LICENSE*) res
DISTRIBUTABLES += presets
DISTRIBUTABLES += selections
# Include the VCV Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk
