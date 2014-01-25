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

/** In questo file si trovano le strutture,
*   le librerie e i prototipi delle funzioni
*   implementate nel file gui.c relative
*   all'interfaccia grafica del gioco
*/

#ifndef GUI_H
#define GUI_H
#include <gtk/gtk.h>
#include "gui.h"
#include "core.h"
#include <stdlib.h>

/** Dimensione massima del nome del giocatore */
#define DIM_NOME	128

/** Struttura che racchiude tutti i dati della gui e del gioco nella
 *   sessione di gioco attuale */
typedef struct
{
    /* Struttura contenente i dati della partita */
	partita 	match;

	/* Nome del giocatore */
	char		nome_giocatore[DIM_NOME];

	/* Tempo di gioco */
	unsigned	tempo;

	/* Flag di gioco in corso*/
	int			start;

    /* Finestre utilizzate e modificate nell'interfaccia */
	GtkWidget*	finestra;
    GtkWidget*  finestra_nome;
	GtkWidget*  finestra_pausa;
    GtkWidget*  finestra_mazzi;

    /* hbox delle carte del pc e del giocatore */
	GtkWidget*  hbox_pc;
	GtkWidget*  hbox_giocatore;

    /* ScrolledWidow per le carte del pc e del giocatore */
	GtkWidget*	scroll_pc;
	GtkWidget*	scroll_giocatore;

    /* Labels utilizzate e modificate nell'interfaccia */
	GtkWidget*	label_timer;
	GtkWidget*	label_nome;
	GtkWidget*  label_turno;
	GtkWidget*	label_carte_mazzo;

    /* Immagini utilizzate e modificate durante il gioco */
	GtkWidget*	image_carta_tavolo;
	GtkWidget*	image_mazzo;
	GtkWidget*  image_colore;

    /* Event box utilizzate per la gestione delle "carte cliccabili" */
	GtkWidget*  event_box_giocatore[60];
	GtkWidget*  event_box_mazzo;
	GtkWidget*	entry_box_nome;

    /* Widget per la creazione della lista per la scelta dei mazzi */
	GtkWidget*  tree_view_mazzi;
	GtkListStore* lista_mazzi;

} gui;

/** prototipi delle funzioni */
void gui_crea_interfaccia_gioco(gui *);
void gui_crea_finestra_nome(GtkWidget *, gui *);
void gui_finestra_mazzo(GtkWidget* item ,gui* sessione);
void gui_finestra_credits();
void gui_finestra_credits();
void gui_finestra_istruzioni();

void gui_nuova_partita(GtkWidget *, gui *);
void gui_aggiorna(gui *);
int	 gui_aggiorna_timer(gui *);
void gui_pausa(GtkWidget* , gui* );
void gui_riavvia_timer(GtkWidget* , gui* );
void gui_gioca_carta(GtkWidget *, GdkEventButton *, gui *);
void gui_pesca_carta(GtkWidget *, GdkEventButton *, gui *);
int  gui_giocata_pc(gui *sessione);
void gui_vincita(gui* sessione);
void gui_chiedi_colore(partita *match);
void gui_plus_carte(partita* match,int );
void gui_cambia_mazzo(GtkWidget *row, gui *sessione);

void gui_chiudi_finestra(GtkWidget* button, GtkWidget* finestra);
void gui_esci(GtkWidget *);

#endif
