#
#  Makefile for Linux 
#
#     make all   (construction de l'executable)
#     make clean (effacement des fichiers objets et de l'executable)
#
#  A adapter en fonction des ordinateurs/environnements 
#  Compilateur, edition de liens, 
#
#
CC       = gcc  
LD       = gcc
CFLAGS   = -Dgraphic -g -Wall
LFLAGS   = -Wall -g
LIBS     =  -lglfw -lm -lGL -lglut -lGLU 
#-lglut -lGLU -lpthread -lglfw -lGL

PROG     = myDelaunay
LISTEOBJ = \
 glfem.o watson.o  main.o 
#graphic.o 
# ATTENTION... aucun caractere apres le caractere de continuation "\"
#
# compilation
#
.c.o :
	$(CC) -c  $(CFLAGS) -o $@ $<
# ATTENTION... la ligne precedente doit commencer par une tabulation
#
# dependances
#
all        : $(PROG)
watson.o 	   : watson.c watson.h
#mainAnimate.o     : mainAnimate.c
#mainExample.o     : mainExample.c
main.o		   : main.c glfem.h watson.h
glfem.o : glfem.c glfem.h watson.h
#graphic.o : graphic.c
#
# edition de lien
#
$(PROG) : $(LISTEOBJ)
	$(LD) -o $(PROG) $(LFLAGS) $(LISTEOBJ) $(LIBS)
# ATTENTION... la ligne precedente doit commencer par une tabulation
#
# effacement des fichiers intermediaires
#
clean :
	rm -vf $(PROG) $(LISTEOBJ) core a.out
# ATTENTION... la ligne precedente doit commencer par une tabulation
#
# ATTENTION... il faut une ligne vide a la fin du fichier.

