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
CFLAGS   = -O3 -Dgraphic -g -Wall
LFLAGS   = -Wall -O3 -g
#LIBS     =  -lm -lGL -lglut -lGLU -lpthread 
# -lglfw
#
PROG     = myDelaunay
LISTEOBJ = \
  watson.o main.o 
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
watson.o 	   : watson.c
#mainAnimate.o     : mainAnimate.c
#mainExample.o     : mainExample.c
main.o		   : main.c
#graphic.o 	   : graphic.c
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

