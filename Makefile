# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: etran <etran@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/06 03:40:09 by eli               #+#    #+#              #
#    Updated: 2023/07/16 08:49:10 by etran            ###   ########.fr        #
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
GEN_DIR		:=	$(APP_DIR)/generation
CHAR_DIR	:=	$(GAME_DIR)/character
IMG_DIR		:=	$(UTILS_DIR)/img
MODEL_DIR	:=	$(UTILS_DIR)/model

SUBDIRS		:=	$(APP_DIR) \
				$(TOOLS_DIR) \
				$(GAME_DIR) \
				$(CHAR_DIR) \
				$(ENG_DIR) \
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
				$(IMG_DIR)/ppm_loader.cpp \
				$(IMG_DIR)/image_handler.cpp \
				$(GAME_DIR)/gameplay.cpp \
				$(CHAR_DIR)/character.cpp \
				$(CHAR_DIR)/player.cpp \
				$(ENG_DIR)/window.cpp \
				$(ENG_DIR)/debug_module.cpp \
				$(ENG_DIR)/device.cpp \
				$(ENG_DIR)/swap_chain.cpp \
				$(ENG_DIR)/render_pass.cpp \
				$(ENG_DIR)/descriptor_set.cpp \
				$(ENG_DIR)/command_pool.cpp \
				$(ENG_DIR)/texture_sampler.cpp \
				$(ENG_DIR)/input_buffer.cpp \
				$(ENG_DIR)/engine.cpp \
				$(ENG_DIR)/buffer.cpp \
				$(ENG_DIR)/command_buffer.cpp \
				$(ENG_DIR)/image_buffer.cpp \
				$(APP_DIR)/app.cpp \
				main.cpp

SRC			:=	$(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJ			:=	$(addprefix $(OBJ_DIR)/,$(SRC_FILES:.cpp=.o))
DEP			:=	$(addprefix $(OBJ_DIR)/,$(SRC_FILES:.cpp=.d))

# shaders
SHD_FILES	:=	vert \
				frag

SHD			:=	$(addprefix $(SHD_DIR)/,$(SHD_FILES))
SHD_BIN		:=	$(addsuffix .spv,$(SHD))

# compiler
CXX			:=	clang++
EXTRA		:=	-Wall -Werror -Wextra
INCLUDES	:=	$(addprefix -I./,$(INC_SUBDIRS))

CFLAGS		:=	$(EXTRA) \
				-std=c++17 \
				-MMD \
				-MP \
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

# misc
GLSLC		:=	glslc
RM			:=	rm -rf

# ============================================================================ #
#                                     RULES                                    #
# ============================================================================ #

.PHONY: all
all: $(NAME)

-include $(DEP)
$(NAME): $(SHD_BIN) $(OBJ)
	@$(CXX) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "\`$(NAME)\` successfully created."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR) $(OBJ_SUBDIRS)
	@echo "Compiling file $<..."
	@$(CXX) $(CFLAGS) -c $< -o $@

$(SHD_DIR)/%.spv: $(SHD_DIR)/shader.%
	@echo "Compiling shader $<..."
	@$(GLSLC) $< -o $@

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