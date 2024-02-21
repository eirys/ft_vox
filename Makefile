# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: etran <etran@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/06 03:40:09 by eli               #+#    #+#              #
#    Updated: 2024/02/21 18:16:56 by etran            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ============================================================================ #
#                                    TARGETS                                   #
# ============================================================================ #

# ---------------- FINAL BINARY ---------------- #
NAME		:=	ft_vox

# --------------- DIRECTORY NAMES -------------- #
SRC_DIR		:=	src
OBJ_DIR		:=	obj
SHD_DIR		:=	shaders

# shader binaries
SHD_BIN_DIR	:=	$(OBJ_DIR)/shaders

# app
APP_DIR		:=	app
UI_DIR		:=	$(APP_DIR)/ui
ENGINE_DIR	:=	$(APP_DIR)/engine
CORE_DIR	:=	$(ENGINE_DIR)/core

# libraries
LIBS_DIR	:=	libs
MATH_DIR	:=	$(LIBS_DIR)/math
DECL_DIR	:=	$(LIBS_DIR)/decl
IO_DIR		:=	$(LIBS_DIR)/io

# ---------------- SUBDIRECTORIES -------------- #
SUBDIRS		:=	$(APP_DIR) \
				$(UI_DIR) \
				$(ENGINE_DIR) \
				$(CORE_DIR) \
				$(LIBS_DIR) \
				$(MATH_DIR) \
				$(DECL_DIR) \
				$(IO_DIR)

OBJ_SUBDIRS	:=	$(addprefix $(OBJ_DIR)/,$(SUBDIRS))
INC_SUBDIRS	:=	$(addprefix $(SRC_DIR)/,$(SUBDIRS)) \
				$(SHD_DIR)

# ---------------- SOURCE FILES ---------------- #
SRC_FILES	:=	entrypoint.cpp \
				$(APP_DIR)/app.cpp \
				$(CORE_DIR)/core.cpp \
				$(CORE_DIR)/debug_module.cpp \
				$(MATH_DIR)/maths.cpp \
				$(MATH_DIR)/matrix.cpp \
				$(UI_DIR)/window.cpp

SRC			:=	$(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJ			:=	$(addprefix $(OBJ_DIR)/,$(SRC_FILES:.cpp=.o))
DEP			:=	$(addprefix $(OBJ_DIR)/,$(SRC_FILES:.cpp=.d))

CXX			:=	clang++
EXTRA		:=	-Wall \
				-Werror \
				-Wextra
INCLUDES	:=	$(addprefix -I./,$(INC_SUBDIRS))

MACROS		:=	NDEBUG \
				SHD_BIN_DIR=\"$(SHD_BIN_DIR)/\" \
				__DEBUG \
				__LOG \
				__INFO \
				__LINUX \
				VOX_CPP

## All macros:

## SHD_BIN_DIR
## Must be defined to the directory where the shader binaries will be stored.

## __RELEASE
## Enables release build, disables debug logs.

## __VERBOSE
## Enables verbose Vulkan validation layers.

## __DEBUG
## For debug logs.

## __LOG
## Enables logging, messages meant for the user.

## __INFO
## Enables info messages, for both the user and the developer.

## __LINUX
## Enables Linux-specific code.

## VOX_CPP
## Enables C++ code.

## NDEBUG
## Disables assertions (if using <cassert>).

DEFINES		:=	$(addprefix -D,$(MACROS))

CFLAGS		:=	$(EXTRA) \
				-std=c++17 \
				-MMD \
				-MP \
				$(INCLUDES) \
				-O3 \
				$(DEFINES)

LDFLAGS		:=	-lglfw \
				-lvulkan \
				-ldl \
				-lpthread \
				-lX11 \
				-lXxf86vm \
				-lXrandr \
				-lXi

# ------------------- SHADERS ------------------ #
# SHD_FILES	:=	scene.fragment \
# 				scene.vertex \
# 				shadow.vertex \
# 				culling.compute

SHD			:=	$(addprefix $(SHD_BIN_DIR)/,$(SHD_FILES))
SHD_BIN		:=	$(addsuffix .spv,$(SHD))

# -------------------- MISC -------------------- #
GLSLC		:=	glslc
RM			:=	rm -rf

# ============================================================================ #
#                                     RULES                                    #
# ============================================================================ #

.PHONY: all
all: $(NAME)

-include $(DEP)

# Compile binary
$(NAME):  shaders $(OBJ)
	@$(CXX) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "\`$(NAME)\` successfully created."

# Compile obj files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR) $(OBJ_SUBDIRS)
	@echo "Compiling file $<..."
	@$(CXX) $(CFLAGS) -c $< -o $@

# Compile shader binaries
$(SHD_BIN_DIR)/%.spv: $(SHD_DIR)/glsl/%.glsl
	@mkdir -p $(OBJ_DIR) $(SHD_BIN_DIR)
	@echo "Compiling shader $<..."
	@$(GLSLC) -fshader-stage=$(subst .,,$(suffix $(basename $<))) $< -o $@

.PHONY: shaders
shaders: $(SHD_BIN)

.PHONY: clean_shaders
clean_shaders:
	@${RM} $(SHD_BIN_DIR)
	@echo "Removed shader binaries."

.PHONY: shaders_re
shaders_re: clean_shaders $(SHD_BIN)

.PHONY: clean
clean:
	@${RM} $(OBJ_DIR)
	@echo "Cleaning object files and dependencies."

.PHONY: fclean
fclean: clean
	@${RM} $(NAME)
	@echo "Removed $(NAME)."

.PHONY: re
re: fclean all