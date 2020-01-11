
PROJECT_MODULES = src/lld_control.c
				  
PROJECT_TESTS   = 
				  		
PROJECT_CSRC    = src/main.c src/common.c src/debug.c \
					$(PROJECT_MODULES) $(PROJECT_TESTS)

PROJECT_INCDIR	= include tests $(ROSINC)

PROJECT_LIBS	= -lm

PROJECT_OPTS	= -specs=nosys.specs

