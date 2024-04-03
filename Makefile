# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: etran <etran@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/06 03:40:09 by eli               #+#    #+#              #
#    Updated: 2024/04/03 22:15:48 by etran            ###   ########.fr        #
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
GFX_DIR		:=	$(ENGINE_DIR)/gfx
UI_DIR		:=	$(ENGINE_DIR)/ui
GAME_DIR	:=	$(ENGINE_DIR)/game
WORLD_DIR	:=	$(GAME_DIR)/world

# gfx
BUF_DIR		:=	$(GFX_DIR)/buffers
CMD_DIR		:=	$(GFX_DIR)/command
SYNC_DIR	:=	$(GFX_DIR)/sync

DESC_DIR	:=	$(GFX_DIR)/descriptor
SETS_DIR	:=	$(DESC_DIR)/sets
SAMPLER_DIR	:=	$(DESC_DIR)/sampler

RENDER_DIR	:=	$(GFX_DIR)/rendering
PIP_DIR		:=	$(RENDER_DIR)/pipelines
PASSES_DIR	:=	$(RENDER_DIR)/passes

DEBUG_PIP_DIR	:=	$(PIP_DIR)/debug

# libraries
LIBS_DIR	:=	libs
MATH_DIR	:=	$(LIBS_DIR)/math
DECL_DIR	:=	$(LIBS_DIR)/decl
IO_DIR		:=	$(LIBS_DIR)/io
PROC_DIR	:=	$(LIBS_DIR)/procedural
LOAD_DIR	:=	$(LIBS_DIR)/load

# ---------------- SUBDIRECTORIES -------------- #
SUBDIRS		:=	$(LIBS_DIR) \
				$(MATH_DIR) \
				$(DECL_DIR) \
				$(ENGINE_DIR) \
				$(GFX_DIR) \
				$(UI_DIR) \
				$(IO_DIR) \
				$(BUF_DIR) \
				$(SAMPLER_DIR) \
				$(CMD_DIR) \
				$(DESC_DIR) \
				$(SETS_DIR) \
				$(SYNC_DIR) \
				$(RENDER_DIR) \
				$(PIP_DIR) \
				$(PASSES_DIR) \
				$(PROC_DIR) \
				$(GAME_DIR) \
				$(LOAD_DIR) \
				$(WORLD_DIR) \
				$(DEBUG_PIP_DIR)

OBJ_SUBDIRS	:=	$(addprefix $(OBJ_DIR)/,$(SUBDIRS))
INC_SUBDIRS	:=	$(addprefix $(SRC_DIR)/,$(SUBDIRS)) \
				$(SHD_DIR)

# ---------------- SOURCE FILES ---------------- #
SRC_FILES	:=	entrypoint.cpp \
				$(LOAD_DIR)/ppm_loader.cpp \
				$(LOAD_DIR)/image_handler.cpp \
				$(IO_DIR)/io_helpers.cpp \
				$(ENGINE_DIR)/engine.cpp \
				$(GFX_DIR)/renderer.cpp \
				$(GFX_DIR)/core.cpp \
				$(GFX_DIR)/device.cpp \
				$(GFX_DIR)/swap_chain.cpp \
				$(CMD_DIR)/command_pool.cpp \
				$(CMD_DIR)/command_buffer.cpp \
				$(SAMPLER_DIR)/game_texture_sampler.cpp \
				$(SAMPLER_DIR)/skybox_sampler.cpp \
				$(SAMPLER_DIR)/chunk_data_sampler.cpp \
				$(SAMPLER_DIR)/perlin_noise_sampler.cpp \
				$(SETS_DIR)/descriptor_set.cpp \
				$(SETS_DIR)/mvp_set.cpp \
				$(SETS_DIR)/world_set.cpp \
				$(DESC_DIR)/descriptor_pool.cpp \
				$(DESC_DIR)/descriptor_table.cpp \
				$(RENDER_DIR)/pipeline.cpp \
				$(RENDER_DIR)/render_pass.cpp \
				$(PASSES_DIR)/main_render_pass.cpp \
				$(PIP_DIR)/scene_pipeline.cpp \
				$(PIP_DIR)/skybox_pipeline.cpp \
				$(PIP_DIR)/starfield_pipeline.cpp \
				$(DEBUG_PIP_DIR)/debug_tex_pipeline.cpp \
				$(BUF_DIR)/buffer.cpp \
				$(BUF_DIR)/image_buffer.cpp \
				$(SYNC_DIR)/fence.cpp \
				$(SYNC_DIR)/gfx_semaphore.cpp \
				$(PROC_DIR)/perlin_noise.cpp \
				$(MATH_DIR)/maths.cpp \
				$(MATH_DIR)/matrix.cpp \
				$(GAME_DIR)/game_state.cpp \
				$(WORLD_DIR)/world.cpp \
				$(WORLD_DIR)/chunk.cpp \
				$(UI_DIR)/controller.cpp \
				$(UI_DIR)/window.cpp

SRC			:=	$(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJ			:=	$(addprefix $(OBJ_DIR)/,$(SRC_FILES:.cpp=.o))
DEP			:=	$(addprefix $(OBJ_DIR)/,$(SRC_FILES:.cpp=.d))

CXX			:=	clang++
MACROS		:=	GLFW_INCLUDE_VULKAN \
				__LOG \
				__INFO \
				__LINUX \
				VOX_CPP
DEFINES		:=	$(addprefix -D,$(MACROS))

## All macros:
## SHD_BIN_DIR : Must be defined to the directory where the shader binaries will be stored.
## __RELEASE : Enables release build, disables debug logs.
## __VERBOSE : Enables verbose Vulkan validation layers.
## __DEBUG : For debug logs.
## __LOG : Enables logging, messages meant for the user.
## __INFO : Enables info messages, for both the user and the developer.
## __LINUX : Enables Linux-specific code.
## VOX_CPP : Enables C++ code.
## NDEBUG : Disables assertions (if using <cassert>).

# EXTRA		?=	-Wall \
# 				-Werror \
# 				-Wextra
INCLUDES	:=	$(addprefix -I./,$(INC_SUBDIRS))

CFLAGS		:=	$(EXTRA) \
				-std=c++20 \
				-MD \
				-MP \
				-O3 \
				$(INCLUDES) \
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
SHD_FILES	:=	scene.fragment \
				scene.vertex \
				skybox.vertex \
				skybox.fragment \
				starfield.vertex \
				starfield.fragment

SHD			:=	$(addprefix $(SHD_BIN_DIR)/,$(SHD_FILES))
SHD_BIN		:=	$(addsuffix .spv,$(SHD))
SHD_DEP		:=	$(addsuffix .d,$(SHD))

# -------------------- MISC -------------------- #
GLSLC		?=	glslc
GLSLC_FLAGS	:=	-MD

RM			:=	rm -rf

# ============================================================================ #
#                                     RULES                                    #
# ============================================================================ #

.PHONY: all
all: $(NAME)

-include $(DEP)
-include $(SHD_DEP)

# Compile binary
$(NAME): shaders $(OBJ)
	@$(CXX) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "\`$(NAME)\` successfully created."

# Compile obj files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR) $(OBJ_SUBDIRS)
	@echo "Compiling file $<..."
	@$(CXX) $(CFLAGS) -c $< -o $@

# Compile shader binaries
$(SHD_BIN_DIR)/%.spv: $(SHD_DIR)/%.glsl
	@mkdir -p $(OBJ_DIR) $(SHD_BIN_DIR)
	@echo "Compiling shader $<..."
	@$(GLSLC) $(GLSLC_FLAGS) -fshader-stage=$(subst .,,$(suffix $(basename $<))) $< -o $@

.PHONY: shaders
shaders: $(SHD_BIN)


.PHONY: clean
clean:
	@$(RM) $(OBJ_DIR)
	@echo "Cleaning object files and dependencies."

.PHONY: fclean
fclean: clean
	@$(RM) $(NAME)
	@echo "Removed $(NAME)."

.PHONY: re
re: fclean all

.PHONY: clean_shaders
clean_shaders:
	@$(RM) $(SHD_BIN_DIR)
	@echo "Removed shader binaries."

.PHONY: shaders_re
shaders_re: clean_shaders $(SHD_BIN)

.PHONY: force
force:
	@make -S shaders_re all