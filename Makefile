# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: etran <etran@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/06 03:40:09 by eli               #+#    #+#              #
#    Updated: 2023/12/11 22:53:47 by etran            ###   ########.fr        #
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
SHD_BIN_DIR	:=	$(OBJ_DIR)/bin

# main
APP_DIR		:=	app

# engine
ENG_DIR		:=	$(APP_DIR)/engine
CORE_DIR	:=	$(ENG_DIR)/core
GFX_DIR		:=	$(ENG_DIR)/gfx

# renderers
REN_DIR		:=	$(GFX_DIR)/renderers
CULL_DIR	:=	$(REN_DIR)/culling
SCENE_DIR	:=	$(REN_DIR)/scene
SHADOW_DIR	:=	$(REN_DIR)/shadows


# gameplay
GAME_DIR	:=	$(APP_DIR)/gameplay
GEN_DIR		:=	$(APP_DIR)/generation
CHAR_DIR	:=	$(GAME_DIR)/character

# other
UTILS_DIR	:=	$(APP_DIR)/utils
IMG_DIR		:=	$(UTILS_DIR)/img
MODEL_DIR	:=	$(UTILS_DIR)/model
TOOLS_DIR	:=	tools

# ---------------- SUBDIRECTORIES -------------- #
SUBDIRS		:=	$(APP_DIR)		\
				$(ENG_DIR)		\
				$(CORE_DIR)		\
				$(GFX_DIR)		\
				$(REN_DIR)		\
				$(SCENE_DIR)	\
				$(SHADOW_DIR)	\
				$(CULL_DIR)		\
				$(GAME_DIR)		\
				$(GEN_DIR)		\
				$(CHAR_DIR)		\
				$(UTILS_DIR)	\
				$(IMG_DIR)		\
				$(MODEL_DIR)	\
				$(TOOLS_DIR)

OBJ_SUBDIRS	:=	$(addprefix $(OBJ_DIR)/,$(SUBDIRS))
INC_SUBDIRS	:=	$(addprefix $(SRC_DIR)/,$(SUBDIRS))

# ---------------- SOURCE FILES ---------------- #
SRC_FILES	:=	$(TOOLS_DIR)/matrix.cpp \
				$(MODEL_DIR)/model.cpp \
				$(MODEL_DIR)/parser.cpp \
				$(MODEL_DIR)/obj_parser.cpp \
				$(MODEL_DIR)/mtl_parser.cpp \
				$(GEN_DIR)/perlin_noise.cpp \
				$(GEN_DIR)/cube.cpp \
				$(GEN_DIR)/packed_cube.cpp \
				$(GEN_DIR)/block.cpp \
				$(GEN_DIR)/chunk.cpp \
				$(GEN_DIR)/world.cpp \
				$(GEN_DIR)/world_generator.cpp \
				$(IMG_DIR)/ppm_loader.cpp \
				$(IMG_DIR)/image_handler.cpp \
				$(GAME_DIR)/game_state.cpp \
				$(GAME_DIR)/clock.cpp \
				$(CHAR_DIR)/character.cpp \
				$(CHAR_DIR)/player.cpp \
				$(ENG_DIR)/window.cpp \
				$(ENG_DIR)/swap_chain.cpp \
				$(ENG_DIR)/input_handler.cpp \
				$(ENG_DIR)/engine.cpp \
				$(CORE_DIR)/debug_module.cpp \
				$(CORE_DIR)/device.cpp \
				$(CORE_DIR)/core.cpp \
				$(GFX_DIR)/pipeline_manager.cpp \
				$(GFX_DIR)/graphics_pipeline.cpp \
				$(GFX_DIR)/compute_pipeline.cpp \
				$(GFX_DIR)/pipeline_helpers.cpp \
				$(GFX_DIR)/descriptor_pool.cpp \
				$(GFX_DIR)/descriptor_set.cpp \
				$(GFX_DIR)/command_pool.cpp \
				$(GFX_DIR)/render_pass.cpp \
				$(GFX_DIR)/target.cpp \
				$(GFX_DIR)/buffer.cpp \
				$(GFX_DIR)/command_buffer.cpp \
				$(GFX_DIR)/image_buffer.cpp \
				$(GFX_DIR)/texture_handler.cpp \
				$(GFX_DIR)/culling_texture_handler.cpp \
				$(GFX_DIR)/chunk_texture_handler.cpp \
				$(CULL_DIR)/bounding_box.cpp \
				$(CULL_DIR)/bounding_frustum.cpp \
				$(CULL_DIR)/culling_pipeline.cpp \
				$(SCENE_DIR)/scene_render_pass.cpp \
				$(SCENE_DIR)/scene_descriptor_set.cpp \
				$(SCENE_DIR)/scene_pipeline.cpp \
				$(SCENE_DIR)/scene_texture_handler.cpp \
				$(SCENE_DIR)/scene_target.cpp \
				$(SHADOW_DIR)/shadows_render_pass.cpp \
				$(SHADOW_DIR)/shadows_descriptor_set.cpp \
				$(SHADOW_DIR)/shadows_pipeline.cpp \
				$(SHADOW_DIR)/shadows_texture_handler.cpp \
				$(SHADOW_DIR)/shadows_target.cpp \
				$(APP_DIR)/app.cpp \
				main.cpp

SRC			:=	$(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJ			:=	$(addprefix $(OBJ_DIR)/,$(SRC_FILES:.cpp=.o))
DEP			:=	$(addprefix $(OBJ_DIR)/,$(SRC_FILES:.cpp=.d))

CXX			:=	clang++
EXTRA		:=	-Wall -Werror -Wextra
INCLUDES	:=	$(addprefix -I./,$(INC_SUBDIRS))

MACROS		:=	__DEBUG \
				__LINUX \
				NDEBUG
DEFINES		:=	$(addprefix -D,$(MACROS))

CFLAGS		:=	$(EXTRA) \
				-std=c++17 \
				-MMD \
				-MP \
				$(INCLUDES) \
				-g \
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
# 				shadow.vertex

SHD_FILES:= culling.compute

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
$(NAME): $(SHD_BIN) $(OBJ)
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
fclean: clean clean_shaders
	@${RM} $(NAME)
	@echo "Removed $(NAME)."

.PHONY: re
re: fclean all