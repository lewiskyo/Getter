#OPT ?= -O2 -DNDEBUG
# (B) Debug mode, w/ full line-level debugging symbols
OPT ?= -g2
# (C) Profiling mode: opt, but w/debugging symbols
# OPT ?= -O2 -g2 -DNDEBUG
$(shell CC="$(CC)" CXX="$(CXX)" TARGET_OS="$(TARGET_OS)" ./build_config 1>&2)
include config.mk

CFLAGS += -I. $(PLATFORM_CCFLAGS) $(OPT)
CXXFLAGS += -I. $(PLATFORM_CXXFLAGS) $(OPT)

LDFLAGS += $(PLATFORM_LDFLAGS)
LIBS += $(PLATFORM_LIBS)

GETTER_SOURCES += $(shell find getter -name '*.cpp')
GETTER_OBJECTS = $(GETTER_SOURCES:.cpp=.o)

LIBRARY = libgetter.a

TARGETS = $(LIBRARY)

default: $(TARGETS)


install: libgetter.a
	mkdir -p $(PREFIX)/usr/local/include/getter
	cp -f getter/*.h $(PREFIX)/usr/local/include/getter
	cp -f libgetter.a $(PREFIX)/usr/local/lib

uninstall:
	rm -rf $(PREFIX)/usr/local/include/getter $(PREFIX)/usr/local/lib/libgetter.a

clean:
			-rm -f $(TARGETS)
			-rm -f */*.o

$(LIBRARY): $(GETTER_OBJECTS)
		rm -f $@
			$(AR) -rs $@ $(GETTER_OBJECTS)

.cpp.o:
		$(CXX) $(CXXFLAGS) -c $< -o $@

.c.o:
		$(CC) $(CFLAGS) -c $< -o $@

.cpp:
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS) $(LIBRARY) $(LIBS)