/** In questo file sono implementate le funzioni
 *  relative al "cuore" del programma
 */

#include "core.h"
#include "gui.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/** Creazione e inizializzazione di una nuova partita */
void core_nuova_partita(partita* match)
{
    int i;
    match->colore = -1;

    // Riempimento iniziale del mazzo
    for(i = 0; i < CARTE_TOTALI; i++) match->mazzo[i] = i;
    match->mazzo_n = CARTE_TOTALI;

    // Mescolamento del mazzo di carte attarverso l'algoritmo fisher-yates
    core_mischia_mazzo(match);

    #if DEBUG
        for(i = 0; i < CARTE_TOTALI; i++) printf(" %d ", match->mazzo[i]);
        printf("\n\n");
    #endif

    // Distribuzione delle carte del pc
    match->pc_n = 0;
    for(i = CARTE_TOTALI-CARTE_INIZIALI; i < (CARTE_TOTALI); i++)
    {
        match->pc[match->pc_n] = match->mazzo[i];
        match->pc_n = match->pc_n + 1;
    }
    match->mazzo_n -= CARTE_INIZIALI;

    #if DEBUG
        printf("PC_N:  %d \n", match->pc_n);
        for(i = 0; i < match->pc_n; i++) printf(" %d ", match->pc[i]);
        printf("\n");
    #endif

    // Distribuzione delle carte del giocatore
    match->giocatore_n = 0;
    for(i = CARTE_TOTALI - 2 * CARTE_INIZIALI; i < (CARTE_TOTALI-CARTE_INIZIALI); i++)
    {
        match->giocatore[match->giocatore_n] = match->mazzo[i];
        match->giocatore_n = match->giocatore_n + 1;
    }
    match->mazzo_n -= CARTE_INIZIALI;

    #if DEBUG
        printf("GIOC_N:  %d \n", match->giocatore_n);
        for(i = 0; i < match->giocatore_n; i++) printf(" %d ", match->giocatore[i]);
        printf("\n");
    #endif

    // Carta del tavolo
    match->tavolo[0] = match->mazzo[--match->mazzo_n];
    match->tavolo_n = 1;

    int carta = match->tavolo[0] % CARTE_PER_COLORE;
    if((carta == 24) || (carta == 25))
    {
        #if DEBUG
            printf("Rimischio!\n");
        #endif
		core_nuova_partita(match);
    }
}


/** Trasformazione del codice della carta in path */
void core_code_to_path(int codice, char *path)
{
    /*  n % CARTE_PER_COLORE -> numero carta
     *  n / CARTE_PER_COLORE -> colore
     */

	int col = codice / CARTE_PER_COLORE;
	int carta = codice % CARTE_PER_COLORE;

	if(carta < 10)
		sprintf(path, "data/%s/%d.png", cartelle_colori[col], carta); // Numeri da 0 a 9
	else if(carta < 19)
		sprintf(path, "data/%s/%d.png", cartelle_colori[col], carta - 9); // Numeri da 0 a 9 (secondo mazzo)
	else if(carta < 21)
		sprintf(path, "data/%s/12.png", cartelle_colori[col]); // +2
	else if(carta < 23)
		sprintf(path, "data/%s/11.png", cartelle_colori[col]); // Stop
	else if(carta < 24)
		sprintf(path, "data/%s/13.png", cartelle_colori[col]); // +4
	else if(carta < 25)
		sprintf(path, "data/%s/14.png", cartelle_colori[col]); // Cambio colore
	else if(carta < 27)
		sprintf(path, "data/%s/10.png", cartelle_colori[col]); // Cambio giro

	#if DEBUG
		printf("\n%d -> %s\n", codice, path);
	#endif
}

/** Controllo sulla compatibilità della carta giocata */
int core_compatibilita(partita *match, int carta_tavolo, int carta_giocata)
{
    int colore_tavolo, numero_tavolo;
    int colore_carta, numero_carta;

    // Gestione del colore scelto tramite il cambio colore o il +4
    if(match->colore != -1)
    {
        colore_tavolo = match->colore;
        colore_carta = (int) (((float) carta_giocata) / ((float) CARTE_PER_COLORE));
        numero_carta = carta_giocata % CARTE_PER_COLORE;

        #if DEBUG
            printf("Colore scelto: %d - Colore test: %d\n", colore_tavolo, colore_carta);
        #endif

        if(numero_carta == 24) // Cambio colore
        {
            match->colore = -1;
            return 1;
        }
        else if(numero_carta == 23) // +4
        {
            match->colore = -1;
            return 1;
        }
        else if(colore_carta == colore_tavolo)
        {
            match->colore = -1;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        colore_tavolo = (int) (((float) carta_tavolo) / ((float) CARTE_PER_COLORE));
        numero_tavolo = carta_tavolo % CARTE_PER_COLORE;
        if((numero_tavolo < 19) && (numero_tavolo > 9))
            numero_tavolo = numero_tavolo - 9;

        colore_carta = (int) (((float) carta_giocata) / ((float) CARTE_PER_COLORE));
        numero_carta = carta_giocata % CARTE_PER_COLORE;

        if((numero_carta > 9) && (numero_carta < 19))
            numero_carta = numero_carta - 9;
        if(carta_tavolo == 24) // Cambio colore
            return 1;
        if((numero_carta == 23) || (numero_carta == 24)) // +4 o cambio colore
            return 1;
        if(((numero_carta == 20) && (numero_tavolo == 19)) || ((numero_carta == 19) && (numero_tavolo == 20))) // +2
            return 1;
        if(((numero_carta == 21) && (numero_tavolo == 22)) || ((numero_carta == 22) && (numero_tavolo == 21))) // Stop
            return 1;
        if(((numero_carta == 25) && (numero_tavolo == 26)) || ((numero_carta == 26) && (numero_tavolo == 25))) // Cambio giro
            return 1;
        if((colore_carta == colore_tavolo) || (numero_carta == numero_tavolo))
            return 1;
    }

    #if DEBUG
        printf("carta_tavolo %d ", carta_tavolo);
        printf("colore_tavolo %d ", colore_tavolo);
        printf("numero_tavolo %d \n", numero_tavolo);
        printf("carta_giocata %d ", carta_giocata);
        printf("colore_carta %d ", colore_carta);
        printf("numero_carta %d \n\n", numero_carta);
    #endif

    // Carta giocata non valida
    return 0;
}

/** Riadattamento degli array ad ogni mossa del gioco */
void core_gioca_e_adatta(partita* match, int carta_giocata, int pos)
{
    int i;

    if(match->turno == PC)
    {
        match->tavolo[match->tavolo_n] = match->pc[pos];
        match->tavolo_n = match->tavolo_n + 1;

        // Compattazione dell'array del pc
        for(i = pos; i < (match->pc_n)-1; i++)
        {
            match->pc[i] = match->pc[i+1];
        }
        match->pc_n = match->pc_n - 1;
    }
    else
    {
        match->tavolo[match->tavolo_n] = match->giocatore[pos];
        match->tavolo_n = match->tavolo_n + 1;

        // Compattazione dell'array del giocatore
        for(i = pos; i < (match->giocatore_n)-1; i++)
        {
            match->giocatore[i] = match->giocatore[i+1];
        }
        match->giocatore_n = match->giocatore_n - 1;
    }


    // Controllo per il +2
    int numero_carta = match->tavolo[(match->tavolo_n) -1] % CARTE_PER_COLORE;
    if((numero_carta == 25) || (numero_carta == 26)|| (numero_carta == 21) ||(numero_carta == 22))
    {
        match->turno = !match->turno;
    }
    if((numero_carta == 19) || (numero_carta == 20))
    {
        if(match->turno == PC)
            gui_plus_carte(match, 2); //2 per indicare il numero di carte pescate

        for(i = 0; i < 2; i++)
            core_pesca_carta(match, !(match->turno));
    }

    // Controllo per il +4
    else if((numero_carta == 23))
    {
        if(match->turno == PC)
            gui_plus_carte(match,4); //4 per indicare il numero di carte pescate

        for(i = 0; i < 4; i++)
            core_pesca_carta(match, !(match->turno));

        if(match->turno == GIOCATORE)
            gui_chiedi_colore(match);
        else
            match->colore = rand() % 4;
    }

    // Controllo per il cambio colore
    else if(numero_carta == 24)
    {
        if(match->turno == GIOCATORE)
            gui_chiedi_colore(match);
        else
            match->colore = rand() % 4;
    }
    match->turno = !match->turno;
}

/** Gestione della partita del computer*/
void core_giocata_pc(partita* match)
{
    int i;
    int pos_carta = -1;

    // Scorrimento dell'array e controllo della compatibilità di ogni carta
    for(i = 0; i < match->pc_n; i++)
    {
        if(core_compatibilita(match, match->tavolo[(match->tavolo_n)-1], match->pc[i]))
        {
            pos_carta = i;
            break;
        }
    }

    // Viene giocata la carta nella posizione restituita (in base alla compatibilità)
    if(pos_carta != -1)
        core_gioca_e_adatta(match, match->pc[pos_carta], pos_carta);
    else
    {
        if(core_pesca_carta(match, PC))
            core_giocata_pc(match);
    }
}

/** Gestione del pescaggio di una carta in base al turno */
int core_pesca_carta(partita* match, player turno)
{
    if(turno == GIOCATORE)
    {
        match->giocatore[match->giocatore_n] = match->mazzo[match->mazzo_n];
        match->mazzo_n = match->mazzo_n - 1;
        match->giocatore_n = match->giocatore_n + 1;
    }
    else
    {
        match->pc[match->pc_n] = match->mazzo[match->mazzo_n];
        match->mazzo_n = match->mazzo_n - 1;
        match->pc_n = match->pc_n + 1;
    }

    if(match->mazzo_n <= 0)
        core_rimescola_mazzo(match);

    return 1;
}

/** Gestione del "rimescolamento" del mazzo sul tavolo nel
 *  caso in cui siano finite le carte da pescare
 */
void core_rimescola_mazzo(partita* match)
{
    int i;
    for(i = 0; i < (match->tavolo_n)-1; i++)
    {
        match->mazzo[i] = match->tavolo[i];
        match->mazzo_n ++;
    }
    match->tavolo[0] = match->tavolo[(match->tavolo_n) - 1];
    match->tavolo_n = 1;

    core_mischia_mazzo(match);
}

/** Algoritmo fisher yates per il mescolamento delle carte */
void core_mischia_mazzo(partita* match)
{
    int i,j, buff;
    srand(time(NULL));

    for(i = 1; i < match->mazzo_n; i++)
    {
        j = -1 + rand() % i;
        buff = match->mazzo[j];
        match->mazzo[j] = match->mazzo[i];
        match->mazzo[i] = buff;
    }
}

/** Stampa degli array per il Debug */
void debug_stampa_array(partita *match)
{
    int i;

    printf("Tavolo: ");
    for(i = 0; i < match->tavolo_n; i++)
        printf("%d ", match->tavolo[i]);
    printf("\n");

    printf("PC: ");
    for(i = 0; i < match->pc_n; i++)
        printf("%d ", match->pc[i]);
    printf("\n");

    printf("Giocatore: ");
    for(i = 0; i < match->giocatore_n; i++)
        printf("%d ", match->giocatore[i]);
    printf("\n");

    printf("Mazzo: ");
    for(i = 0; i < match->mazzo_n; i++)
        printf("%d ", match->mazzo[i]);
    printf("\n\n");
}
