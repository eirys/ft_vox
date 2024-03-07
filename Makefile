# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: etran <etran@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/06 03:40:09 by eli               #+#    #+#              #
#    Updated: 2024/03/07 13:48:34 by etran            ###   ########.fr        #
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

# engine
ENGINE_DIR	:=	engine

# ui
UI_DIR		:=	$(ENGINE_DIR)/ui

# gfx
GFX_DIR		:=	$(ENGINE_DIR)/gfx
BUF_DIR		:=	$(GFX_DIR)/buffers
CMD_DIR		:=	$(GFX_DIR)/command
DESC_DIR	:=	$(GFX_DIR)/descriptor
SYNC_DIR	:=	$(GFX_DIR)/sync
RENDER_DIR	:=	$(GFX_DIR)/rendering
SETS_DIR	:=	$(DESC_DIR)/sets
UBO_DIR		:=	$(DESC_DIR)/ubo
PIP_DIR		:=	$(RENDER_DIR)/pipelines
PASS_DIR	:=	$(RENDER_DIR)/passes

# libraries
LIBS_DIR	:=	libs
MATH_DIR	:=	$(LIBS_DIR)/math
DECL_DIR	:=	$(LIBS_DIR)/decl
IO_DIR		:=	$(LIBS_DIR)/io

# ---------------- SUBDIRECTORIES -------------- #
SUBDIRS		:=	$(LIBS_DIR) \
				$(MATH_DIR) \
				$(DECL_DIR) \
				$(ENGINE_DIR) \
				$(UI_DIR) \
				$(IO_DIR) \
				$(GFX_DIR) \
				$(BUF_DIR) \
				$(CMD_DIR) \
				$(DESC_DIR) \
				$(SETS_DIR) \
				$(UBO_DIR) \
				$(SYNC_DIR) \
				$(RENDER_DIR) \
				$(PASS_DIR) \
				$(PIP_DIR)

OBJ_SUBDIRS	:=	$(addprefix $(OBJ_DIR)/,$(SUBDIRS))
INC_SUBDIRS	:=	$(addprefix $(SRC_DIR)/,$(SUBDIRS)) \
				$(SHD_DIR)

# ---------------- SOURCE FILES ---------------- #
SRC_FILES	:=	entrypoint.cpp \
				$(IO_DIR)/io_helpers.cpp \
				$(ENGINE_DIR)/engine.cpp \
				$(GFX_DIR)/renderer.cpp \
				$(GFX_DIR)/core.cpp \
				$(GFX_DIR)/device.cpp \
				$(GFX_DIR)/swap_chain.cpp \
				$(CMD_DIR)/command_pool.cpp \
				$(CMD_DIR)/command_buffer.cpp \
				$(DESC_DIR)/descriptor_pool.cpp \
				$(DESC_DIR)/descriptor_table.cpp \
				$(DESC_DIR)/descriptor_set.cpp \
				$(RENDER_DIR)/pipeline.cpp \
				$(RENDER_DIR)/render_pass.cpp \
				$(PIP_DIR)/scene_pipeline.cpp \
				$(PASS_DIR)/scene_render_pass.cpp \
				$(BUF_DIR)/buffer.cpp \
				$(BUF_DIR)/image_buffer.cpp \
				$(SYNC_DIR)/fence.cpp \
				$(SYNC_DIR)/gfx_semaphore.cpp \
				$(MATH_DIR)/maths.cpp \
				$(MATH_DIR)/matrix.cpp \
				$(UI_DIR)/window.cpp

SRC			:=	$(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJ			:=	$(addprefix $(OBJ_DIR)/,$(SRC_FILES:.cpp=.o))
DEP			:=	$(addprefix $(OBJ_DIR)/,$(SRC_FILES:.cpp=.d))

CXX			:=	clang++
EXTRA		?=	-Wall \
				-Werror \
				-Wextra
INCLUDES	:=	$(addprefix -I./,$(INC_SUBDIRS))

MACROS		:=	GLFW_INCLUDE_VULKAN \
				NDEBUG \
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
				-std=c++20 \
				-MMD \
				-MP \
				-g \
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