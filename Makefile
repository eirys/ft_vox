# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: etran <etran@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/06 03:40:09 by eli               #+#    #+#              #
#    Updated: 2023/06/05 21:14:50 by etran            ###   ########.fr        #
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
UTILS_DIR	:=	$(APP_DIR)/utils
GAME_DIR	:=	$(APP_DIR)/gameplay
IMG_DIR		:=	$(UTILS_DIR)/img
MODEL_DIR	:=	$(UTILS_DIR)/model
GEN_DIR		:=	$(UTILS_DIR)/generation

SUBDIRS		:=	$(APP_DIR) \
				$(TOOLS_DIR) \
				$(GAME_DIR) \
				$(ENG_DIR) \
				$(MODEL_DIR) \
				$(UTILS_DIR) \
				$(GEN_DIR) \
				$(IMG_DIR)

OBJ_SUBDIRS	:=	$(addprefix $(OBJ_DIR)/,$(SUBDIRS))
INC_SUBDIRS	:=	$(addprefix $(SRC_DIR)/,$(SUBDIRS))

# cpp files
INC_FILES	:=	$(TOOLS_DIR)/utils.h \
				$(TOOLS_DIR)/math.h \
				$(TOOLS_DIR)/matrix.h \
				$(TOOLS_DIR)/vector.h \
				$(UTILS_DIR)/vertex.h \
				$(UTILS_DIR)/uniform_buffer_object.h \
				$(MODEL_DIR)/model.h \
				$(MODEL_DIR)/material.h \
				$(MODEL_DIR)/parser.h \
				$(MODEL_DIR)/obj_parser.h \
				$(MODEL_DIR)/mtl_parser.h \
				$(GEN_DIR)/perlin_noise.h \
				$(GEN_DIR)/cube.h \
				$(IMG_DIR)/image_loader.h \
				$(IMG_DIR)/image_handler.h \
				$(IMG_DIR)/ppm_loader.h \
				$(GAME_DIR)/gameplay.h \
				$(GAME_DIR)/character.h \
				$(GAME_DIR)/player.h \
				$(ENG_DIR)/window.h \
				$(ENG_DIR)/debug_module.h \
				$(ENG_DIR)/device.h \
				$(ENG_DIR)/render_target.h \
				$(ENG_DIR)/render_target_resources.h \
				$(ENG_DIR)/descriptor_set.h \
				$(ENG_DIR)/command_buffer.h \
				$(ENG_DIR)/texture_sampler.h \
				$(ENG_DIR)/vertex_input.h \
				$(ENG_DIR)/engine.h \
				$(APP_DIR)/app.h

SRC_FILES	:=	$(TOOLS_DIR)/matrix.cpp \
				$(MODEL_DIR)/model.cpp \
				$(MODEL_DIR)/parser.cpp \
				$(MODEL_DIR)/obj_parser.cpp \
				$(MODEL_DIR)/mtl_parser.cpp \
				$(GEN_DIR)/perlin_noise.cpp \
				$(GEN_DIR)/cube.cpp \
				$(IMG_DIR)/ppm_loader.cpp \
				$(IMG_DIR)/image_handler.cpp \
				$(GAME_DIR)/gameplay.cpp \
				$(GAME_DIR)/character.cpp \
				$(GAME_DIR)/player.cpp \
				$(ENG_DIR)/window.cpp \
				$(ENG_DIR)/debug_module.cpp \
				$(ENG_DIR)/device.cpp \
				$(ENG_DIR)/render_target.cpp \
				$(ENG_DIR)/render_target_resources.cpp \
				$(ENG_DIR)/descriptor_set.cpp \
				$(ENG_DIR)/command_buffer.cpp \
				$(ENG_DIR)/texture_sampler.cpp \
				$(ENG_DIR)/vertex_input.cpp \
				$(ENG_DIR)/engine.cpp \
				$(APP_DIR)/app.cpp \
				main.cpp

INC			:=	$(addprefix	$(SRC_DIR)/,$(INC_FILES))
SRC			:=	$(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJ			:=	$(addprefix $(OBJ_DIR)/,$(SRC_FILES:.cpp=.o))

# shaders
SHD_FILES	:=	vert \
				frag

SHD			:=	$(addprefix $(SHD_DIR)/,$(SHD_FILES))
SHD_BIN		:=	$(addsuffix .spv,$(SHD))

# compiler
CXX			:=	c++
EXTRA		:=	-Wall -Werror -Wextra
INCLUDES	:=	$(addprefix -I./,\
				$(INC_SUBDIRS))

CFLAGS		:=	$(EXTRA)\
				-std=c++17 \
				$(INCLUDES) \
				-g \
				-D__DEBUG \
				-DNDEBUG

LDFLAGS		:=	-lglfw \
				-lvulkan \
				-ldl \
				-lpthread \
				-lX11 \
				-lXxf86vm \
				-lXrandr \
				-lXi

GLSLC		:=	glslc

# misc
RM			:=	rm -rf

# ============================================================================ #
#                                     RULES                                    #
# ============================================================================ #

.PHONY: all
all: $(NAME)

$(NAME): $(SHD_BIN) $(OBJ)
	@$(CXX) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "\`$(NAME)\` was successfully created."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC)
	@mkdir -p $(OBJ_DIR) $(OBJ_SUBDIRS)
	@echo "Compiling file $<..."
	@$(CXX) $(CFLAGS) -c $< -o $@

$(SHD_DIR)/%.spv: $(SHD_DIR)/shader.%
	@echo "Compiling shader $<..."
	@$(GLSLC) $< -o $@

.PHONY: clean
clean:
	@${RM} $(OBJ_DIR)
	@echo "Cleaning object files."

.PHONY: fclean
fclean: clean
	@${RM} $(SHD_BIN)
	@echo "Removed shader binaries."
	@${RM} $(NAME)
	@echo "Removed $(NAME)."

.PHONY: re
re: fclean all

# TODO: Remove
.PHONY: test
test:
	c++ $(INCLUDES) -std=c++17 -g -D__DEBUG src/app/utils/generation/perlin_noise.cpp