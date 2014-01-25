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
*   implementate nel file core.c relative
*   al "cuore" del gioco
*/

#ifndef CORE_H
#define CORE_H

/** Attiva/Disattiva i messaggi di debug */
#define DEBUG				0

/** Numero di carte per ogni colore */
#define CARTE_PER_COLORE	27

/** Carte totali nel gioco */
#define CARTE_TOTALI		(CARTE_PER_COLORE * 4)

/** Carte inizialmente distribuite */
#define CARTE_INIZIALI		6

/** Dimensione massima del nome del mazzo */
#define NOME_MAZZO_DIM      56

/** Colori delle carte */
typedef enum { ROSSO = 0, BLU, VERDE, GIALLO } colore;

/** Giocatore corrente */
typedef enum {PC = 0, GIOCATORE }player;

/** Struttura contenente i dati della partita */
typedef struct
{
	/* Mazzo dal quale pescare */
	int mazzo[CARTE_TOTALI];
	int mazzo_n;

	/* Carte in mano del giocatore */
	int giocatore[CARTE_TOTALI];
	int giocatore_n;

	/* Carte in mano del pc */
	int pc[CARTE_TOTALI];
	int pc_n;

	/* Carte in tavola */
	int tavolo[CARTE_TOTALI];
	int tavolo_n;

    /**
	 * Colore attuale (per il +4/cambiogiro)
	 *  Se -1, viene utilizzato il colore della carta sul tavolo
	 *  Se >-1, viene utilizzato come colore per la prossima carta compatibile
	 */
	int colore;

	/* Turno corrente  */
	player      turno;

    /* Mazzo di carte corrente */
	char        nome_mazzo[NOME_MAZZO_DIM];
} partita;

/** Cartelle dei colori */
static char cartelle_colori[][12] = { "red", "blue", "green", "yellow" };

/** Cartelle dei colori */
static char colori[][12] = { "Rosso", "Blu", "Verde", "Giallo" };

/** Cartelle dei mazzi */
#define MAZZI_NUM   7
static char mazzi[][12] = { "troll", "batman", "sarde", "minnie", "nerd", "bazinga", "barney" };

/** prototipi delle funzioni */
void core_nuova_partita(partita *match);
void core_code_to_path(int codice, char *path);
int  core_compatibilita(partita *match, int carta_tavolo, int carta_giocata);
void core_gioca_e_adatta(partita* match, int carta_giocata, int pos);
int  core_pesca_carta(partita* match, player turno);
void core_giocata_pc(partita* match);
void core_mischia_mazzo(partita* match);
void core_rimescola_mazzo(partita* match);
void debug_stampa_array(partita *match);

#endif
