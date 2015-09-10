# Makefile for afkTaskKill.
# For windows XP and higher.
#
all: afkTaskKill.cpp
	g++ -Wall -o afkTaskKiller.exe afkTaskKill.cpp

clean:
	del *.o afkTaskKiller.exe