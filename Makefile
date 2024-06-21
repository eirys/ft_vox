# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: etran <etran@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/06 03:40:09 by eli               #+#    #+#              #
#    Updated: 2024/06/21 03:24:54 by etran            ###   ########.fr        #
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
MAP_DIR		:=	assets/maps

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
TEX_DIR		:=	$(DESC_DIR)/texture

RENDER_DIR	:=	$(GFX_DIR)/rendering
PIP_DIR		:=	$(RENDER_DIR)/pipelines
PASSES_DIR	:=	$(RENDER_DIR)/passes
GEO_DIR		:=	$(RENDER_DIR)/geometry

# libraries
LIBS_DIR	:=	libs
MATH_DIR	:=	$(LIBS_DIR)/math
DECL_DIR	:=	$(LIBS_DIR)/decl
IO_DIR		:=	$(LIBS_DIR)/io
PROC_DIR	:=	$(LIBS_DIR)/procedural
LOAD_DIR	:=	$(LIBS_DIR)/load

# other
SETUP_DIR	:=	setup

# ---------------- SUBDIRECTORIES -------------- #
SUBDIRS		:=	$(LIBS_DIR) \
				$(MATH_DIR) \
				$(DECL_DIR) \
				$(ENGINE_DIR) \
				$(GFX_DIR) \
				$(UI_DIR) \
				$(IO_DIR) \
				$(BUF_DIR) \
				$(TEX_DIR) \
				$(CMD_DIR) \
				$(DESC_DIR) \
				$(SETS_DIR) \
				$(SYNC_DIR) \
				$(RENDER_DIR) \
				$(GEO_DIR) \
				$(PIP_DIR) \
				$(PASSES_DIR) \
				$(PROC_DIR) \
				$(GAME_DIR) \
				$(LOAD_DIR) \
				$(WORLD_DIR)

OBJ_SUBDIRS	:=	$(addprefix $(OBJ_DIR)/,$(SUBDIRS))
INC_SUBDIRS	:=	$(addprefix $(SRC_DIR)/,$(SUBDIRS)) \
				$(SHD_DIR)

# ---------------- SOURCE FILES ---------------- #
SRC_FILES	:=	entrypoint.cpp \
				$(LOAD_DIR)/voxmap.cpp \
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
				$(TEX_DIR)/game_textures.cpp \
				$(TEX_DIR)/skybox_texture.cpp \
				$(TEX_DIR)/gbuffer_textures.cpp \
				$(TEX_DIR)/perlin_noise_texture.cpp \
				$(TEX_DIR)/shadowmap_texture.cpp \
				$(TEX_DIR)/sampler.cpp \
				$(SETS_DIR)/descriptor_set.cpp \
				$(SETS_DIR)/pfd_set.cpp \
				$(SETS_DIR)/world_set.cpp \
				$(SETS_DIR)/gbuffer_set.cpp \
				$(SETS_DIR)/ssao_sets.cpp \
				$(DESC_DIR)/descriptor_pool.cpp \
				$(DESC_DIR)/descriptor_table.cpp \
				$(DESC_DIR)/texture_table.cpp \
				$(RENDER_DIR)/pipeline_layout.cpp \
				$(RENDER_DIR)/push_constant.cpp \
				$(GEO_DIR)/vertex.cpp \
				$(GEO_DIR)/frustum_culling.cpp \
				$(GEO_DIR)/vertex_buffer.cpp \
				$(PASSES_DIR)/render_pass.cpp \
				$(PASSES_DIR)/main_render_pass.cpp \
				$(PASSES_DIR)/deferred_render_pass.cpp \
				$(PASSES_DIR)/ssao_render_pass.cpp \
				$(PASSES_DIR)/ssao_blur_render_pass.cpp \
				$(PASSES_DIR)/shadow_render_pass.cpp \
				$(PIP_DIR)/pipeline.cpp \
				$(PIP_DIR)/ssao_pipeline.cpp \
				$(PIP_DIR)/ssao_blur_pipeline.cpp \
				$(PIP_DIR)/deferred_pipeline.cpp \
				$(PIP_DIR)/scene_pipeline.cpp \
				$(PIP_DIR)/skybox_pipeline.cpp \
				$(PIP_DIR)/starfield_pipeline.cpp \
				$(PIP_DIR)/shadow_pipeline.cpp \
				$(PIP_DIR)/debug_tex_pipeline.cpp \
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
				$(WORLD_DIR)/block.cpp \
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
				VOX_SEED=42 \
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

EXTRA		:=	-Wall \
				-Werror \
				-Wextra

INCLUDES	:=	$(addprefix -I./,$(INC_SUBDIRS))

CFLAGS		:=	-std=c++20 \
				-MD \
				-MP \
				-g \
				-O3

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
				starfield.fragment \
				deferred.vertex \
				deferred.fragment \
				debug.vertex \
				debug.fragment \
				shadowmap.vertex \
				ssao.fragment \
				blur.fragment

SHD			:=	$(addprefix $(SHD_BIN_DIR)/,$(SHD_FILES))
SHD_BIN		:=	$(addsuffix .spv,$(SHD))
SHD_DEP		:=	$(addsuffix .d,$(SHD))

# -------------------- MISC -------------------- #
GLSLC		?=	glslc
GLSLC_FLAGS	:=	-MD \
				-O

RM			:=	rm -rf

MAPS		:=	$(MAP_DIR)/biomes.voxmap \
				# $(MAP_DIR)/heightmap.voxmap

# ============================================================================ #
#                                     RULES                                    #
# ============================================================================ #

# PROJECT ==================================================================== #

.PHONY: all
all: $(MAPS) $(NAME)

.PHONY: run
run: all
	@./$(NAME)

.PHONY: fclean
fclean: clean
	@$(RM) $(NAME)
	@echo "Removed $(NAME)."

.PHONY: re
re: fclean all

.PHONY: force
force: shaders_re run

# CPP ======================================================================== #
-include $(DEP)
-include $(SHD_DEP)

# Compile binary
$(NAME): shaders $(OBJ)
	@$(CXX) $(CFLAGS) $(INCLUDES) $(DEFINES) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "\`$(NAME)\` successfully created."

# Compile obj files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR) $(OBJ_SUBDIRS)
	@echo "Compiling file $<..."
	@$(CXX) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

.PHONY: clean
clean:
	@$(RM) $(OBJ_DIR)
	@echo "Cleaning object files and dependencies."

# SHADERS ==================================================================== #
# Compile shader binaries
$(SHD_BIN_DIR)/%.spv: $(SHD_DIR)/%.glsl
	@mkdir -p $(OBJ_DIR) $(SHD_BIN_DIR)
	@echo "Compiling shader $<..."
	@$(GLSLC) $(GLSLC_FLAGS) -fshader-stage=$(subst .,,$(suffix $(basename $<))) $< -o $@

.PHONY: shaders
shaders: $(SHD_BIN)

.PHONY: clean_shaders
clean_shaders:
	@$(RM) $(SHD_BIN_DIR)
	@echo "Removed shader binaries."

.PHONY: shaders_re
shaders_re: clean_shaders $(SHD_BIN)

# ASSETS ===================================================================== #
.PHONY: maps
maps: $(MAPS)

$(MAPS):
	@echo "Generating maps..."
	@mkdir -p $(MAP_DIR) $(OBJ_DIR)/$(SETUP_DIR)
	@$(CXX) $(CFLAGS) $(DEFINES) $(SRC_DIR)/$(SETUP_DIR)/map_generator.cpp -o $(OBJ_DIR)/$(SETUP_DIR)/map_generator
	@./obj/$(SETUP_DIR)/map_generator
	@echo "Maps generated."

.PHONY: remove_map
remove_map:
	@$(RM) $(MAP_DIR)
	@echo "Removed $(MAP_DIR)."