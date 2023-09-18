.PHONY : all clean

CFG ?= debug

CFG_DIR := $(CFG)/
BIN_DIR := ../bin/$(CFG_DIR)
INT_DIR := int/$(CFG_DIR)
SRC_DIR := src/
OBJ_DIR := $(INT_DIR)obj/
DEP_DIR := $(INT_DIR)dep/

SRC_EXT := .cc
OBJ_EXT := .o
DEP_EXT := .d

SRC := $(shell find $(SRC_DIR) -name *$(SRC_EXT))
OBJ := $(SRC:$(SRC_DIR)%$(SRC_EXT)=$(OBJ_DIR)%$(OBJ_EXT))
DEP := $(SRC:$(SRC_DIR)%$(SRC_EXT)=$(DEP_DIR)%$(DEP_EXT))
OUT := $(BIN_DIR)hotload
OUT_DIR_LAZY = $(realpath $(dir $(OUT)))

CXX := g++
CXXFLAGS += \
	-std=c++17 \
	-fno-rtti \
	-fno-exceptions \
	-Werror \
	-Waddress \
	-Waddress-of-packed-member \
	-Wall \
	-Walloc-zero \
	-Walloca \
	-Wbool-compare \
	-Wbool-operation \
	-Wbuiltin-declaration-mismatch \
	-Wbuiltin-macro-redefined \
	-Wc++11-compat \
	-Wc++14-compat \
	-Wc++17-compat \
	-Wcast-function-type \
	-Wcast-qual \
	-Wchar-subscripts \
	-Wclass-conversion \
	-Wclass-memaccess \
	-Wclobbered \
	-Wcomment \
	-Wconditionally-supported \
	-Wconversion \
	-Wconversion-null \
	-Wcpp \
	-Wctor-dtor-privacy \
	-Wdangling-else \
	-Wdate-time \
	-Wdelete-incomplete \
	-Wdelete-non-virtual-dtor \
	-Wdeprecated \
	-Wdeprecated-copy \
	-Wdeprecated-copy-dtor \
	-Wdiv-by-zero \
	-Wdouble-promotion \
	-Wduplicated-branches \
	-Wduplicated-cond \
	-Weffc++ \
	-Wempty-body \
	-Wendif-labels \
	-Wenum-compare \
	-Wexpansion-to-defined \
	-Wextra \
	-Wextra-semi \
	-Wfloat-conversion \
	-Wfloat-equal \
	-Wformat-contains-nul \
	-Wformat-extra-args \
	-Wformat-nonliteral \
	-Wformat-security \
	-Wformat-signedness \
	-Wformat-y2k \
	-Wformat-zero-length \
	-Wframe-address \
	-Wif-not-aligned \
	-Wignored-attributes \
	-Wignored-qualifiers \
	-Winherited-variadic-ctor \
	-Winit-list-lifetime \
	-Winit-self \
	-Wint-in-bool-context \
	-Wint-to-pointer-cast \
	-Winvalid-offsetof \
	-Winvalid-pch \
	-Wliteral-suffix \
	-Wlogical-not-parentheses \
	-Wlogical-op \
	-Wlong-long \
	-Wmain \
	-Wmaybe-uninitialized \
	-Wmemset-elt-size \
	-Wmemset-transposed-args \
	-Wmisleading-indentation \
	-Wmissing-attributes \
	-Wmissing-braces \
	-Wmissing-declarations \
	-Wmissing-field-initializers \
	-Wmissing-include-dirs \
	-Wmultichar \
	-Wmultiple-inheritance \
	-Wmultistatement-macros \
	-Wnarrowing \
	-Wnoexcept \
	-Wnoexcept-type \
	-Wnon-template-friend \
	-Wnon-virtual-dtor \
	-Wnonnull \
	-Wnonnull-compare \
	-Wold-style-cast \
	-Wopenmp-simd \
	-Woverlength-strings \
	-Woverloaded-virtual \
	-Wpacked-bitfield-compat \
	-Wpacked-not-aligned \
	-Wparentheses \
	-Wpedantic \
	-Wpessimizing-move \
	-Wpmf-conversions \
	-Wpointer-arith \
	-Wpointer-compare \
	-Wpragmas \
	-Wprio-ctor-dtor \
	-Wpsabi \
	-Wredundant-decls \
	-Wredundant-move \
	-Wregister \
	-Wreorder \
	-Wrestrict \
	-Wreturn-type \
	-Wsequence-point \
	-Wshift-count-negative \
	-Wshift-count-overflow \
	-Wshift-negative-value \
	-Wsign-compare \
	-Wsign-conversion \
	-Wsign-promo \
	-Wsized-deallocation \
	-Wsizeof-array-argument \
	-Wsizeof-pointer-div \
	-Wsizeof-pointer-memaccess \
	-Wstrict-null-sentinel \
	-Wstringop-truncation \
	-Wsubobject-linkage \
	-Wsuggest-override \
	-Wswitch \
	-Wswitch-bool \
	-Wswitch-default \
	-Wswitch-enum \
	-Wsync-nand \
	-Wsynth \
	-Wtautological-compare \
	-Wterminate \
	-Wtrigraphs \
	-Wundef \
	-Wuninitialized \
	-Wunknown-pragmas \
	-Wunused \
	-Wunused-local-typedefs \
	-Wunused-macros \
	-Wunused-result \
	-Wunused-variable \
	-Wuseless-cast \
	-Wvarargs \
	-Wvariadic-macros \
	-Wvirtual-inheritance \
	-Wvirtual-move-assign \
	-Wvla \
	-Wvolatile-register-var \
	-Wwrite-strings \
	-Wzero-as-null-pointer-constant

ifeq ($(CFG),debug)
	CXXFLAGS += -g
else
	CXXFLAGS += -DNDEBUG -O3
endif

CXXFLAGS += -I../common/include/

LDFLAGS += -L$(BIN_DIR) -ldl -lX11 -lGL -lhotloadcommon

TARGET_OBJ_TO_DEP = $(@:$(OBJ_DIR)%$(OBJ_EXT)=$(DEP_DIR)%$(DEP_EXT))

all : $(OUT)

clean :
	$(RM) -r $(OUT) $(INT_DIR)

$(OUT) : $(OBJ)
	$(shell mkdir -p $(dir $@))
	$(CXX) $^ -o $@ $(LDFLAGS) -Wl,-rpath=$(OUT_DIR_LAZY)

$(OBJ_DIR)%$(OBJ_EXT) : $(SRC_DIR)%$(SRC_EXT)
	$(shell mkdir -p $(dir $@) $(dir $(TARGET_OBJ_TO_DEP)))
	$(CXX) -c $< -o $@ $(CXXFLAGS) -MMD -MP -MF $(TARGET_OBJ_TO_DEP)

-include $(DEP)
