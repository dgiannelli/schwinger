# Guest Student Program 2016 Presentation Makefile
# Jülich Supercomputing Centre
# Research Centre Jülich
# Use latexmk to generate pdf automatically
# Use lualatex to support fancy tikz features
# Do not change this file unless you know what you are doing ...

all: build

TEXHOME		 = $(HOME)/texmf

LATEXMK			 = $(or $(wildcard $(TEXHOME)/bin/latexmk), latexmk)
LATEXMK_FLAGS		 = --lualatex --latexoption="$(LATEXMK_LATEXOPTION)" --bibtex --use-make
LATEXMK_FLAGS		+= --recorder-
LATEXMK_FLAGS		+= $(if $(LATEXMK_FORCE_REBUILD),-g)
LATEXMK_FORCE_REBUILD	?=
LATEXMK_LATEXOPTION	 = --interaction=nonstopmode
LATEXMK_LATEXOPTION	+= --synctex=1
LATEXMK_LATEXOPTION	+= --shell-escape
LATEXMK_CLEAN		 = $(LATEXMK) -c
LATEXMK_DISTCLEAN	 = $(LATEXMK) -C

GIT		?= git

# Find the .git directory
GITDIR		:= $(shell $(GIT) rev-parse --git-dir)

# defines SOURCE_BASENAME

TARGET		 = slides
SOURCES		 = $(TARGET:=.tex)


# include tikz externalize Makefile, ignore if not available
-include $(TARGET).makefile


# cannot build tikz externalized figures concurrently due to shared usage of *.vrb
.NOTPARALLEL:

.PHONY: build clean distclean predepend

### Build target ###
build:
	$(MAKE) build-stage1

check-texmf:
	@if [ ! -d "$(TEXHOME)" ]; then echo "FATAL ERROR: $(patsubst $(HOME)/%,~/%,$(TEXHOME)) has not been initialized. Run 'make get-texmf' first."; false; fi

build-target: $(TARGET).pdf

deps-stage1: $(SOURCES)

build-stage1: build-target
	test ! -s $(TARGET).makefile || $(MAKE) -q deps-stage2 || $(MAKE) build-stage2

deps-stage2: $(ALL_FIGURES)

build-stage2: LATEXMK_FORCE_REBUILD = yes
build-stage2: build-target

.PHONY: $(TARGET).pdf
$(TARGET).pdf: deps-stage1
$(TARGET).pdf: deps-stage2
$(TARGET).pdf:
	$(LATEXMK) $(LATEXMK_FLAGS) $(SOURCES)


### Clean target ###
# remove generated files
clean:
	$(LATEXMK_CLEAN)
	$(RM) *.aux *.bbl *.blg
	$(RM) *.vrb *.snm *.nav
	$(RM) *.synctex.gz
	$(RM) *.pgf-plot.gnuplot *.pgf-plot.table
	rm -f *.x.table *.x.gnuplot

# Clean up a little bit more, esp. tikz externalize generated images
distclean: clean
	$(LATEXMK_DISTCLEAN)
	$(RM) $(TARGET).pdf
	$(RM) $(TARGET).figlist
	$(RM) $(TARGET).makefile
	$(RM) *.auxlock
	$(MAKE) -C cache clean

get-texmf:
	test -d $(TEXHOME) || git clone git@gsp.fz-juelich.de:2016/texmf $(TEXHOME)

update-texmf:
	cd $(TEXHOME) && git pull
