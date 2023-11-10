# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: etran <etran@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/06 03:40:09 by eli               #+#    #+#              #
#    Updated: 2023/11/10 14:47:16 by etran            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ============================================================================ #
#                                    TARGETS                                   #
# ============================================================================ #

# final binary
NAME		:=	ft_vox

# directory names
SRC_DIR		:=	src
OBJ_DIR		:=	obj
SHD_DIR		:=	shaders

# subdirectories
TOOLS_DIR	:=	tools
APP_DIR		:=	app
ENG_DIR		:=	$(APP_DIR)/engine
OPTIM_DIR	:=	$(ENG_DIR)/optimization
SCENE_DIR	:=	$(ENG_DIR)/scene
SHADOW_DIR	:=	$(ENG_DIR)/shadows
CULL_DIR	:=	$(OPTIM_DIR)/culling
UTILS_DIR	:=	$(APP_DIR)/utils
GAME_DIR	:=	$(APP_DIR)/gameplay
GEN_DIR		:=	$(APP_DIR)/generation
CHAR_DIR	:=	$(GAME_DIR)/character
IMG_DIR		:=	$(UTILS_DIR)/img
MODEL_DIR	:=	$(UTILS_DIR)/model

SUBDIRS		:=	$(APP_DIR) \
				$(TOOLS_DIR) \
				$(GAME_DIR) \
				$(CHAR_DIR) \
				$(ENG_DIR) \
				$(OPTIM_DIR) \
				$(CULL_DIR) \
				$(SCENE_DIR) \
				$(SHADOW_DIR) \
				$(MODEL_DIR) \
				$(UTILS_DIR) \
				$(GEN_DIR) \
				$(IMG_DIR)

OBJ_SUBDIRS	:=	$(addprefix $(OBJ_DIR)/,$(SUBDIRS))
INC_SUBDIRS	:=	$(addprefix $(SRC_DIR)/,$(SUBDIRS))

# cpp files
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
				$(ENG_DIR)/debug_module.cpp \
				$(ENG_DIR)/device.cpp \
				$(ENG_DIR)/pipeline.cpp \
				$(ENG_DIR)/swap_chain.cpp \
				$(ENG_DIR)/render_pass.cpp \
				$(ENG_DIR)/descriptor_pool.cpp \
				$(ENG_DIR)/descriptor_set.cpp \
				$(ENG_DIR)/command_pool.cpp \
				$(ENG_DIR)/texture_handler.cpp \
				$(ENG_DIR)/map_texture_handler.cpp \
				$(ENG_DIR)/chunk_texture_handler.cpp \
				$(ENG_DIR)/target.cpp \
				$(ENG_DIR)/input_handler.cpp \
				$(ENG_DIR)/engine.cpp \
				$(ENG_DIR)/buffer.cpp \
				$(ENG_DIR)/command_buffer.cpp \
				$(ENG_DIR)/image_buffer.cpp \
				$(CULL_DIR)/bounding_box.cpp \
				$(CULL_DIR)/bounding_frustum.cpp \
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

# shaders
SHD_FILES	:=	scene.fragment \
				scene.vertex \
				shadow.vertex

SHD			:=	$(addprefix $(SHD_DIR)/,$(SHD_FILES))
SHD_BIN		:=	$(addsuffix .spv,$(SHD))

# compiler
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

# misc
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
$(SHD_DIR)/%.spv: $(SHD_DIR)/glsl/%.glsl
	@echo "Compiling shader $<..."
	@$(GLSLC) -fshader-stage=$(subst .,,$(suffix $(basename $<))) $< -o $@

.PHONY: clean_shaders
clean_shaders:
	@${RM} $(SHD_BIN)
	@echo "Removed shader binaries."

.PHONY: shaders_re
shaders_re: clean_shaders $(SHD_BIN)

.PHONY: clean
clean:
	@${RM} $(OBJ_DIR)
	@echo "Cleaning object files and dependencies."

.PHONY: fclean
fclean: clean
	@${RM} $(SHD_BIN)
	@echo "Removed shader binaries."
	@${RM} $(NAME)
	@echo "Removed $(NAME)."

.PHONY: re
re: fclean all