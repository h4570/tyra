TARGET		:= libtyra.a

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR		:= src/ps2
INCDIR		:= inc/ps2
BUILDDIR    := obj
TARGETDIR   := bin
RESDIR      := res
SRCEXT      := cpp
VSMEXT		:= vsm
VCLEXT		:= vcl
VCLPPEXT	:= vclpp
DEPEXT      := d
OBJEXT      := o

SRCSHAREDDIR	:= src/shared
INCSHAREDDIR	:= inc/shared

#Flags, Libraries and Includes
CFLAGS      := 
LIB         := 
LIBDIRS     := 
INC         := -I$(INCDIR) -I$(INCSHAREDDIR)
INCDEP      := -I$(INCDIR) -I$(INCSHAREDDIR)

include ../Makefile.base

# --- Shared

SHARED_SOURCES	:= $(shell find $(SRCSHAREDDIR) -type f -name *.$(SRCEXT))
SHARED_OBJECTS	:= $(patsubst $(SRCSHAREDDIR)/%,$(BUILDDIR)/%,$(SHARED_SOURCES:.$(SRCEXT)=.$(OBJEXT)))

-include $(SHARED_OBJECTS:.$(OBJEXT)=.$(DEPEXT))

$(BUILDDIR)/%.$(OBJEXT): $(SRCSHAREDDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CXX) $(CFLAGS) $(INCDEP) -MM $(SRCSHAREDDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

# --- Compile library

$(TARGET): $(OBJECTS) $(SHARED_OBJECTS) $(VCL_OBJECTS) $(VU_OBJECTS_VCL) $(VU_OBJECTS_VSM) $(IRXEM_OBJECTS)
	$(AR) rcs $(TARGETDIR)/$(TARGET) $(OBJECTS) $(SHARED_OBJECTS) $(VCL_OBJECTS) $(VU_OBJECTS_VCL) $(VU_OBJECTS_VSM) $(IRXEM_OBJECTS)

# --- Test

test: $(OBJECTS) $(SHARED_OBJECTS) $(VCL_OBJECTS) $(VU_OBJECTS_VCL) $(VU_OBJECTS_VSM) $(IRXEM_OBJECTS)
	$(CXX) -o $(TARGETDIR)/tests.elf $^ -lstdc++