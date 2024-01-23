TARGET		:= libtyra.a

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := inc
BUILDDIR    := obj
TARGETDIR   := bin
RESDIR      := res
SRCEXT      := cpp
VSMEXT		:= vsm
VCLEXT		:= vcl
VCLPPEXT	:= vclpp
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS      := 
LIB         :=
LIBDIRS     := 
INC         := -I$(INCDIR)
INCDEP      := -I$(INCDIR) -I$(PS2SDK)/ports/include/freetype2

include ../Makefile.base

$(TARGET): $(OBJECTS) $(VCL_OBJECTS) $(VU_OBJECTS_VCL) $(VU_OBJECTS_VSM) $(IRXEM_OBJECTS)
	$(AR) rcs $(TARGETDIR)/$(TARGET) $(OBJECTS) $(VCL_OBJECTS) $(VU_OBJECTS_VCL) $(VU_OBJECTS_VSM) $(IRXEM_OBJECTS)
