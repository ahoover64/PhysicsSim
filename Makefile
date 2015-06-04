#----------------------------------------------
#    Example Makefile for compiling a program that uses
#    the Matrix and Vector classes
#----------------------------------------------
#
#  Programmer: Donald House
#  Date: March 8, 1999
#
#  Copyright (C) - Donald H. House. 2005
#

CC      = clang++
C	= cpp
H	= h
CFLAGS 	= -g -std=c++11 -Wall

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm ${MAGICKLIB}
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lGLU -lm
  endif
endif

HFILES 	= Matrix.${H} Vector.${H} Utility.${H} Camera.${H} ParticleControl.${H} Utils.${H} Emitter.${H} Gauss.${H} SpringMesh.${H}
OFILES 	= Matrix.o Vector.o Utility.o Camera.o ParticleControl.o Utils.o Emitter.o Gauss.o SpringMesh.o
PROJECT = trampolineTest

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

Camera.o: Camera.${C} Camera.${H} Matrix.${H} Vector.${H} Utility.${H}
	${CC} $(CFLAGS) -c Camera.${C}

Matrix.o: Matrix.${C} Matrix.${H} Vector.${H} Utility.${H}
	${CC} $(CFLAGS) -c Matrix.${C}

Vector.o: Vector.${C} Vector.${H} Utility.${H}
	${CC} $(CFLAGS) -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} $(CFLAGS) -c Utility.${C}

ParticleControl.o: ParticleControl.${C} ParticleControl.${H}
	${CC} $(CFLAGS) -c ParticleControl.${C}

Utils.o: Utils.${C} Utils.${H}
	${CC} $(CFLAGS) -c Utils.${C}

Gauss.o: Gauss.${C} Gauss.${H}
	${CC} $(CFLAGS) -c Gauss.${C}

Emitter.o: Emitter.${C} Emitter.${H}
	${CC} $(CFLAGS) -c Emitter.${C}

SpringMesh.o: SpringMesh.${C} SpringMesh.${H}
	${CC} $(CFLAGS) -c SpringMesh.${C}




debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm -fr *.o *~ ${PROJECT}
