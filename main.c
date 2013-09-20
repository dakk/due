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
