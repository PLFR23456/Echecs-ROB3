CC = gcc
CFLAGS = -Wall -g -pthread  # Ajout de -pthread ici
OBJFILES = menu.o jeu.o temps.o plateau.o calculus.o misc.o
EXE = menu

# Règle pour compiler l'exécutable
$(EXE): $(OBJFILES)
	$(CC) $(OBJFILES) -o $(EXE) -pthread  # Ajout de -pthread lors de la liaison

# Règle pour compiler chaque fichier source en fichier objet
menu.o: menu.c menu.h
	$(CC) $(CFLAGS) -c menu.c

jeu.o: jeu.c jeu.h
	$(CC) $(CFLAGS) -c jeu.c

temps.o: temps.c temps.h
	$(CC) $(CFLAGS) -c temps.c

plateau.o: plateau.c plateau.h
	$(CC) $(CFLAGS) -c plateau.c

calculus.o: calculus.c calculus.h
	$(CC) $(CFLAGS) -c calculus.c

misc.o: misc.c misc.h
	$(CC) $(CFLAGS) -c misc.c

# Nettoyage des fichiers objets et exécutables
clean:
	rm -f $(OBJFILES) $(EXE)

# Règle pour exécuter le programme après compilation
run: $(EXE)
	./$(EXE)

# Règle par défaut : tout faire
all: run
