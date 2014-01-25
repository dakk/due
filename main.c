/*
    Due: a GTK+ card game
    Copyright (C) 2010-2014  Davide Gessa, Sara Casti, Gianmarco Cherchi

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include "gui.h"
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[])
{
  gui sessione;

  // Impostazione del mazzo di default
  strcpy(sessione.match.nome_mazzo, "troll");

  // Inizializzazione delle librerie GTK
  gtk_init(&argc, &argv);

  // Chiamata dell'interfaccia di gioco
  gui_crea_interfaccia_gioco(&sessione);

  // Loop  di rendering
  gtk_main ();
  return 0;
}
