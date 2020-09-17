
PROJECT_MODULES = 	src/lld_control.c			\
					src/lld_encoder.c			\
					src/odometry.c				\
					src/wheel_control.c			\
          			src/robot_odometry.c 		\
					src/i2c.c					\
					src/lcd.c					\
					src/gyroscope.c

				  
PROJECT_TESTS   =	tests/test_lld_control.c	\
					tests/test_lld_encoder.c	\
					tests/test_odometry.c		\
					tests/test_frequency.c		\
					tests/test_wheel_control.c	\
					tests/test_joystick.c		\
          			tests/test_robot_odometry.c \
          			tests/test_gyroscope.c
				  		
PROJECT_CSRC    = src/main.c src/common.c src/debug.c \
					$(PROJECT_MODULES) $(PROJECT_TESTS)

PROJECT_INCDIR	= include

PROJECT_LIBS	= -lm

PROJECT_OPTS	= -specs=nosys.specs

