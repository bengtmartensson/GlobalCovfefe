# Makefile for creating stuff on host.
# Use Arduino IDE for compiling for Arduino.

# The functions for generating keywords.txt requires KeywordsTxtGenerator,
# https://github.com/bengtmartensson/KeywordsTxtGenerator, to be installed in
# KEYWORD_TXT_GENERATOR_DIR.

ifneq ($(ARDUINO),)
$(error This Makefile is not for compiling target code, for that, use the Arduino IDE.)
endif

KEYWORD_TXT_GENERATOR_DIR = ../KeywordsTxtGenerator
DOXYGEN := doxygen
DOXYFILE :=  Doxyfile
XSLTPROC := xsltproc
TRANSFORMATION := $(KEYWORD_TXT_GENERATOR_DIR)/doxygen2keywords.xsl

BROWSER=firefox

# Get VERSION from the version in library.properties
VERSION=$(subst version=,,$(shell grep version= library.properties))
ORIGINURL=$(shell git remote get-url origin)

default: all

src/version/version.h: library.properties
	echo "// This file was automatically generated from $<; do not edit." >  $@
	echo "#define VERSION \"$(VERSION)\""  >> $@

doc: api-doc/index.html
	$(BROWSER) $<

api-doc/index.html xml/index.xml: $(wildcard src/*)
	$(DOXYGEN) $(DOXYFILE)

gh-pages: api-doc/index.html
	rm -rf gh-pages
	git clone --depth 1 -b gh-pages $(ORIGINURL) gh-pages
	rm -rf gh-pages/*
	cp -rf api-doc/* gh-pages
	(cd gh-pages; git add . )
	(cd gh-pages; git commit -a -m "Update of API documentation for version $(VERSION)")
	@echo Now perform \"git push\" from gh-pages

clean:
	rm -rf *.a *.o api-doc xml gh-pages

spotless: clean
	rm -rf keywords.txt src/version/version.h

keywords.txt: xml/index.xml
	$(XSLTPROC) $(TRANSFORMATION) $< > $@

all: api-doc/index.html keywords.txt src/version/version.h

.PHONY: clean spotless all default
