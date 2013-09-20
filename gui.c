/** In questo file sono implementate le funzioni
 *  relative all'interfaccia grafica del gioco
 */

#include <string.h>
#include "gui.h"
#include <stdlib.h>

/** Creazione dell'interfaccia di gioco */
void gui_crea_interfaccia_gioco(gui *sessione)
{
	sessione->start = 0;
    sessione->tempo = 0;
    g_timeout_add(1000, (GSourceFunc) gui_aggiorna_timer, sessione);

	// Creazione della finestra
    sessione->finestra = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Impostazione del titolo della finestra
    gtk_window_set_title(GTK_WINDOW(sessione->finestra), "Due's Game");

    // Impostazione delle dimensioni della finestra
    gtk_window_set_default_size(GTK_WINDOW(sessione->finestra), 1000, 680);

    // Impostazione della posizione della finestra
    gtk_window_set_position(GTK_WINDOW(sessione->finestra), GTK_WIN_POS_CENTER);

    // Impostazione dei bordi
    gtk_container_set_border_width(GTK_CONTAINER(sessione->finestra), 0);

    // Inserimento dei widget nella finestra

    // Creazione vbox principale
    GtkWidget* vbox = gtk_vbox_new(FALSE, 0);

    // Inserimento della vbox nella finestra
    gtk_container_add(GTK_CONTAINER(sessione->finestra), vbox);

    // Creazione della menu bar
    GtkWidget *barra_menu = gtk_menu_bar_new();

    // Inserimento il menu bar nella Vbox_menu
    gtk_box_pack_start(GTK_BOX(vbox), barra_menu, FALSE, FALSE, 0);

    // Creazione dell'item da inserire nel menu
    GtkWidget *item_file = gtk_menu_item_new_with_label("File");

    // Inserimento dell'item nella menu bar
    gtk_menu_bar_append(GTK_MENU_BAR(barra_menu), item_file);

    // Creazione dei sottomenu
    GtkWidget *menu_sub = gtk_menu_new();

    // Collegamento del sottomenu all' item file "File"
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_file), menu_sub);

    // Creazione dell'item file nel sotto menu
    item_file = gtk_menu_item_new_with_label("Nuova partita");

    // Aggiunta dell'item file nel sotto menu
    gtk_menu_append(GTK_MENU(menu_sub), item_file);
    g_signal_connect(G_OBJECT(item_file), "activate", G_CALLBACK(gui_crea_finestra_nome), sessione);

    item_file = gtk_menu_item_new_with_label("Pausa");
    gtk_menu_append(GTK_MENU(menu_sub), item_file);
    g_signal_connect(G_OBJECT(item_file), "activate", G_CALLBACK(gui_pausa),sessione);

    item_file = gtk_menu_item_new_with_label("Esci");
    gtk_menu_append(GTK_MENU(menu_sub), item_file);
    g_signal_connect(G_OBJECT(item_file), "activate", G_CALLBACK(gui_esci),NULL);

    item_file = gtk_menu_item_new_with_label("Modifica");
    gtk_menu_bar_append(GTK_MENU_BAR(barra_menu), item_file);

    menu_sub = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_file), menu_sub);

    item_file = gtk_menu_item_new_with_label("Cambia mazzo");
    gtk_menu_append(GTK_MENU(menu_sub), item_file);
    g_signal_connect(G_OBJECT(item_file), "activate", G_CALLBACK(gui_finestra_mazzo), sessione);

    item_file = gtk_menu_item_new_with_label("Help");
    gtk_menu_bar_append(GTK_MENU_BAR(barra_menu), item_file);

    menu_sub = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_file), menu_sub);
    item_file = gtk_menu_item_new_with_label("Istruzioni");
    gtk_menu_append(GTK_MENU(menu_sub), item_file);
    g_signal_connect(G_OBJECT(item_file), "activate", G_CALLBACK(gui_finestra_istruzioni), NULL);

    item_file = gtk_menu_item_new_with_label("Credits");
    gtk_menu_append(GTK_MENU(menu_sub), item_file);
    g_signal_connect(G_OBJECT(item_file), "activate", G_CALLBACK(gui_finestra_credits), NULL);

    // Inserimento dell'immagine con il titolo
    GtkWidget* testata = gtk_image_new_from_file("data/varie/testata2.png");
    gtk_box_pack_start(GTK_BOX(vbox), testata, FALSE, FALSE, 0);

    // Inserimento label con il nome del pc
    GtkWidget* label_pc = gtk_label_new("Carte Computer");
    gtk_box_pack_start(GTK_BOX(vbox), label_pc, FALSE, FALSE, 0);

    // Inserimento scrolled Window per le carte del pc
    sessione->scroll_pc = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), sessione->scroll_pc, TRUE, TRUE, 0);
    sessione->hbox_pc = gtk_hbox_new(TRUE, 0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sessione->scroll_pc), sessione->hbox_pc);

    // Inserimento dell'hbox centrale
    GtkWidget* hbox_centrale = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_centrale, TRUE, TRUE, 0);


    GtkWidget* vbox1 = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_centrale), vbox1, TRUE, TRUE, 0);

    sessione->label_carte_mazzo =  gtk_label_new("Carte Mazzo: 108");
    gtk_box_pack_start(GTK_BOX(vbox1), sessione->label_carte_mazzo, TRUE, TRUE, 0);

    // Inserimento del mazzo di carte in una event box che le rende collegabili ad una funzione
    sessione->image_mazzo = gtk_image_new();
    sessione->event_box_mazzo = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(vbox1), sessione->event_box_mazzo, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(sessione->event_box_mazzo), sessione->image_mazzo);
    g_signal_connect(G_OBJECT(sessione->event_box_mazzo), "button_press_event",
                            G_CALLBACK(gui_pesca_carta), sessione);

    sessione->image_carta_tavolo = gtk_image_new();
    gtk_box_pack_start(GTK_BOX(hbox_centrale), sessione->image_carta_tavolo, FALSE, FALSE, 0);

    GtkWidget* vbox_dx = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_centrale), vbox_dx, TRUE, TRUE, 0);
    sessione->label_timer = gtk_label_new("Tempo: 00:00");
    gtk_box_pack_start(GTK_BOX(vbox_dx), sessione->label_timer, TRUE, TRUE, 0);

    sessione->image_colore = gtk_image_new();
    gtk_box_pack_start(GTK_BOX(vbox_dx), sessione->image_colore, TRUE, TRUE, 0);

    sessione->label_turno = gtk_label_new("Turno:");
    gtk_box_pack_start(GTK_BOX(vbox_dx), sessione->label_turno, TRUE, FALSE, 0);

    sessione->label_nome = gtk_label_new("Carte Giocatore");
    gtk_box_pack_start(GTK_BOX(vbox), sessione->label_nome, FALSE, FALSE, 0);

    sessione->scroll_giocatore = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), sessione->scroll_giocatore, TRUE, TRUE, 0);
    sessione->hbox_giocatore = gtk_hbox_new(TRUE, 0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sessione->scroll_giocatore),
                                            sessione->hbox_giocatore);

    // Funzione che aggiorna l'interfaccia di gioco
    gui_aggiorna(sessione);

    // Collegamento della X della finestra alla fine del programma
    g_signal_connect(sessione->finestra, "destroy", gtk_main_quit, NULL);

    // Visualizzazione di tutti gli oggetti della finestra
    gtk_widget_show_all(sessione->finestra);
}

/** Creazione della finestra che permette di acquisire
 *  il nome del giocatore
 */
void gui_crea_finestra_nome(GtkWidget *widget, gui *sessione)
{
    GtkWidget *vbox1 = NULL, *button = NULL;

    sessione->finestra_nome = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(sessione->finestra_nome), "Due's Game");
    gtk_window_set_default_size(GTK_WINDOW(sessione->finestra_nome), 400, 100);
    gtk_window_set_position(GTK_WINDOW(sessione->finestra_nome), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(sessione->finestra_nome), 20);

    vbox1 = gtk_vbox_new(TRUE, 10);
    gtk_container_add (GTK_CONTAINER (sessione->finestra_nome), vbox1);

    GtkWidget *label = gtk_label_new("Inserisci Nome");
    gtk_box_pack_start(GTK_BOX(vbox1), label, TRUE, TRUE, 0);

    sessione->entry_box_nome = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox1), sessione->entry_box_nome, TRUE, TRUE, 0);

    GtkWidget* hbox = gtk_hbox_new(TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox, TRUE, TRUE, 0);
    button = gtk_button_new_from_stock(GTK_STOCK_APPLY);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

    // Collegamento del click del bottone ad una funzione
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gui_nuova_partita), sessione);

    gtk_widget_show_all(sessione->finestra_nome);
}

/** Creazione di una nuova partita */
void gui_nuova_partita(GtkWidget *button, gui *sessione)
{
    char *nome;
    char new_label[128];
    char path[128];

    // Acquisizione del nome inserito
    nome = (char*) gtk_entry_get_text(GTK_ENTRY(sessione->entry_box_nome));

    strcpy(sessione->nome_giocatore, nome);

	#if DEBUG
		printf("Impostato il nome: %s\n", sessione->nome_giocatore);
	#endif

    strcpy(new_label, "Carte ");
    strcat(new_label, sessione->nome_giocatore);

    gtk_label_set_text(GTK_LABEL(sessione->label_nome), new_label);

    gtk_widget_destroy(sessione->finestra_nome);

    // Inizio di una nuova partita
    core_nuova_partita(&(sessione->match));

    // Inizializzazione del timer a 0 e impostazione a 1 del flag che indica l'inizio del gioco
	sessione->start = 1;
    sessione->tempo = 0;

    gui_aggiorna(sessione);

    // Creazione del path dell'immagine attraverso la funzione sprintf
    sprintf(path, "data/mazzi/%s/mazzo_big.png", (sessione->match).nome_mazzo);

    // Modifica di un' immagine precedentemente impostata
    gtk_image_set_from_file(GTK_IMAGE(sessione->image_mazzo), path);

    // Impostazione del turno di gioco
    sessione->match.turno = GIOCATORE;

    // Modifica di una label precedentemente impostata
    gtk_label_set_text(GTK_LABEL(sessione->label_turno), "Turno del GIOCATORE");
}

/** Gestione e aggiornamento del timer*/
int gui_aggiorna_timer(gui *sessione)
{
	char time[128];

	if(sessione->start == 1)
	{
		sessione->tempo = sessione->tempo + 1;
		sprintf(time, "Tempo: %d:%d", sessione->tempo / 60, sessione->tempo % 60);
		gtk_label_set_text(GTK_LABEL(sessione->label_timer), time);
	}
	return 1;
}

/** Aggiornamento dell'interfaccia ad ogni turno del gioco */
void gui_aggiorna(gui* sessione)
{
    int i;
    GtkWidget* image;
    GtkWidget* finestra_due;
	char path[128];
	char label[128];

	if((sessione->tempo == 0) && !(sessione->start))
	{
        sprintf(path, "data/mazzi/%s/mazzo_big_empty.png", (sessione->match).nome_mazzo);
        gtk_image_set_from_file(GTK_IMAGE(sessione->image_mazzo), path);

        sprintf(path, "data/mazzi/%s/empty.png", (sessione->match).nome_mazzo);

        // Distruzione di una hbox precedentemente creata
        gtk_widget_destroy(sessione->hbox_pc);

        // Creazione dell'hbox_pc con le carte aggiornate
        sessione->hbox_pc = gtk_hbox_new(TRUE, 0);
        gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sessione->scroll_pc), sessione->hbox_pc);

        for(i = 0; i < CARTE_INIZIALI; i++)
        {
            image = gtk_image_new_from_file(path);
            gtk_box_pack_start(GTK_BOX(sessione->hbox_pc), image, TRUE, TRUE, 0);
        }
        gtk_widget_show_all(sessione->hbox_pc);

        // Distruzione dell'hbox_giocatore esistente
        gtk_widget_destroy(sessione->hbox_giocatore);

        sessione->hbox_giocatore = gtk_hbox_new(TRUE, 0);
        gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sessione->scroll_giocatore),
                                                sessione->hbox_giocatore);

        for(i = 0; i < CARTE_INIZIALI; i++)
        {
            image = gtk_image_new_from_file(path);
            gtk_box_pack_start(GTK_BOX(sessione->hbox_giocatore), image, TRUE, TRUE, 0);
        }

        gtk_widget_show_all(sessione->hbox_giocatore);
        return;
	}

    sprintf(path, "data/mazzi/%s/mazzo_big.png", (sessione->match).nome_mazzo);
    gtk_image_set_from_file(GTK_IMAGE(sessione->image_mazzo), path);

    if(sessione->match.colore != -1)
    {
        sprintf(path, "data/colori/%s.png",colori[(sessione->match).colore]);
        gtk_image_set_from_file(GTK_IMAGE(sessione->image_colore), path);
    }
    else
        // Distruzione di un'immagine precedentemente creata
        gtk_image_clear(GTK_IMAGE(sessione->image_colore));

	gtk_widget_destroy(sessione->hbox_pc);

    // Creazione dell'hbox_pc con le carte aggiornate
    sessione->hbox_pc = gtk_hbox_new(TRUE, 0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sessione->scroll_pc), sessione->hbox_pc);

    for(i = 0; i < (sessione->match.pc_n); i++)
    {
        sprintf(path, "data/mazzi/%s/retro.png", (sessione->match).nome_mazzo);
		image = gtk_image_new_from_file(path);
        gtk_box_pack_start(GTK_BOX(sessione->hbox_pc), image, TRUE, TRUE, 0);
    }

    gtk_widget_show_all(sessione->hbox_pc);

    // Distruzione dell'hbox_giocatore esistente
	gtk_widget_destroy(sessione->hbox_giocatore);

    // Creazione dell'hbox con le carte aggiornate
    sessione->hbox_giocatore = gtk_hbox_new(TRUE, 0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sessione->scroll_giocatore),
                                            sessione->hbox_giocatore);

    for(i = 0; i < (sessione->match.giocatore_n); i++)
    {
		core_code_to_path(sessione->match.giocatore[i], path);

		sessione->event_box_giocatore[i] = gtk_event_box_new();
		image = gtk_image_new_from_file(path);

		gtk_box_pack_start(GTK_BOX(sessione->hbox_giocatore), sessione->event_box_giocatore[i], TRUE, TRUE, 0);

        gtk_container_add (GTK_CONTAINER (sessione->event_box_giocatore[i]), image);

        // Collegamento di una event box ad una funzione
        g_signal_connect(G_OBJECT(sessione->event_box_giocatore[i]), "button_press_event",
                            G_CALLBACK(gui_gioca_carta), sessione);
    }

	core_code_to_path(sessione->match.tavolo[sessione->match.tavolo_n - 1], path);
	gtk_image_set_from_file(GTK_IMAGE(sessione->image_carta_tavolo), path);

	sprintf(label,"Carte mazzo: %d", sessione->match.mazzo_n);
	gtk_label_set_text(GTK_LABEL(sessione->label_carte_mazzo), label);

    gtk_widget_show_all(sessione->hbox_giocatore);

    if((sessione->match.giocatore_n == 2) && (sessione->match.turno != PC))
    {
        // Visualizzazione della finestra che avvisa il giocatore che gli sono rimaste 2 carte
        finestra_due = gtk_message_dialog_new(GTK_WINDOW(sessione->finestra), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_OK, "Ti sono rimaste DUE carte!");
        gtk_window_set_title(GTK_WINDOW(finestra_due), "DUE!");
        gtk_dialog_run(GTK_DIALOG(finestra_due));
        gtk_widget_destroy(finestra_due);
    }
}

/** Giocata di una carta*/
void gui_gioca_carta(GtkWidget *event_box, GdkEventButton *evento, gui *sessione)
{
    int carta;
    int pos_carta = -1;
    int i;

    // Controllo del turno di gioco
    if((sessione->match.turno == PC) || (!sessione->start) )
        return;

    // Gestione del turno
    sessione->match.turno = GIOCATORE;
    gtk_label_set_text(GTK_LABEL(sessione->label_turno), "Turno del GIOCATORE");

    /* Scorre l'array delle event_box fino a che non trova l'event_box
    *  che ha generato l'evento, quindi la carta chiamata.
    */
    for(i = 0; i < (sessione->match.giocatore_n); i++)
    {
        if(event_box == sessione->event_box_giocatore[i])
        {
            pos_carta = i;
        }
    }

    // Carta selezionata
    carta = sessione->match.giocatore[pos_carta];

#if DEBUG
    printf("compatibilita: %d\n",core_compatibilita(&(sessione->match), sessione->match.tavolo[sessione->match.tavolo_n - 1], carta));
#endif

    // Controllo della compaibilità sulla carta scelta
    if(!core_compatibilita(&(sessione->match), sessione->match.tavolo[sessione->match.tavolo_n - 1], carta))
    {
        // Creazione di una message dialog
        GtkWidget *message = gtk_message_dialog_new(GTK_WINDOW(sessione->finestra), GTK_DIALOG_DESTROY_WITH_PARENT,
                                                    GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Mossa non valida");

        gtk_window_set_title(GTK_WINDOW(message), "Attenzione!");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);

        return;
    }

    // Chiamata alla funzione che gioca la carta scelta e riadatta i vari array
    core_gioca_e_adatta(&(sessione->match), carta, pos_carta);

    gui_aggiorna(sessione);

    // Controllo sull'eventuale fine della partita
    if(sessione->match.giocatore_n == 0)
    {
        gui_vincita(sessione);
        sessione->start = 0;
        return;
    }

    // Ritardo "forzato" della giocata del pc di 1500 msec
    g_timeout_add(1500, (GSourceFunc) gui_giocata_pc, sessione);

    // Modifica della label del turno
    gtk_label_set_text(GTK_LABEL(sessione->label_turno), "Turno del PC");

    gui_aggiorna(sessione);
}

/** Pausa del gioco */
void gui_pausa(GtkWidget* item, gui* sessione)
{

    if(!sessione->start) return;

    // Blocco del timer per lo "stop" del gioco
    sessione->start = 0;

    sessione->finestra_pausa = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(sessione->finestra_pausa), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(sessione->finestra_pausa), "Due's Game");
    gtk_window_set_default_size(GTK_WINDOW(sessione->finestra_pausa), 200, 100);
    gtk_container_border_width(GTK_CONTAINER(sessione->finestra_pausa), 20);

    GtkWidget* vbox = gtk_vbox_new(TRUE, 10);
    gtk_container_add(GTK_CONTAINER(sessione->finestra_pausa), vbox);
    GtkWidget* hbox = gtk_hbox_new(TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 10);

    GtkWidget* image = gtk_image_new_from_file("data/varie/pausa.png");
    gtk_box_pack_start(GTK_BOX(hbox), image, TRUE, TRUE, 10);

    GtkWidget* label = gtk_label_new("Gioco in Pausa!");
    gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 10);

    GtkWidget* hbox2 = gtk_hbox_new(TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 10);
    GtkWidget *button = gtk_button_new_with_label("Riprendi il Gioco");
    gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 10);
    g_signal_connect(button, "clicked", G_CALLBACK(gui_riavvia_timer), sessione);

    gtk_widget_show_all(sessione->finestra_pausa);
}

/** Riavvio del timer */
void gui_riavvia_timer(GtkWidget* button, gui* sessione)
{
    sessione->start = 1;
    gtk_widget_destroy(sessione->finestra_pausa);
}

/** Gestione del pescaggio di una carta dal mazzo */
void gui_pesca_carta(GtkWidget *event_box, GdkEventButton *evento, gui *sessione)
{
    // Gestione del cambio turno e del gioco in pausa
    if((sessione->match.turno == PC) || (!sessione->start) )
        return;

    // Gestione dell'acquisizione della carta pescata dal giocatore
    core_pesca_carta(&(sessione->match), GIOCATORE);
    gui_aggiorna(sessione);
}

/** Gestione della giocata del PC */
int gui_giocata_pc(gui *sessione)
{
    if(sessione->match.turno != GIOCATORE)
    {
        core_giocata_pc(&(sessione->match));
        gui_aggiorna(sessione);

        if(sessione->match.pc_n == 0)
        {
            gui_vincita(sessione);
            sessione->start = 0;
        }
    }
	gtk_label_set_text(GTK_LABEL(sessione->label_turno), "Turno del GIOCATORE");

    if(sessione->match.turno != GIOCATORE)
        gui_giocata_pc(sessione);

	return 0;
}

/** Gestione della finestra di fine partita */
void gui_vincita(gui* sessione)
{
    GtkWidget* image;

    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
    gtk_container_border_width(GTK_CONTAINER(win), 20);

    GtkWidget* vbox = gtk_vbox_new(FALSE, 10);
    gtk_container_add(GTK_CONTAINER(win), vbox);

    if(sessione->match.giocatore_n == 0)
    {
        image = gtk_image_new_from_file("data/varie/win.png");
        gtk_box_pack_start(GTK_BOX(vbox), image, FALSE, FALSE, 10);
    }
    else
    {
        image = gtk_image_new_from_file("data/varie/lose.png");
        gtk_box_pack_start(GTK_BOX(vbox), image, FALSE, FALSE, 10);
    }

    GtkWidget* hbox = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 10);

    GtkWidget* hbox2 = gtk_hbox_new(TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 10);

    GtkWidget *button = gtk_button_new_with_label("       Chiudi      ");
    gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 10);
    g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), sessione);

    if(sessione->match.pc_n == 0)
    {
        gtk_window_set_title(GTK_WINDOW(win), "GAME OVER!!!");

        image = gtk_image_new_from_file("data/varie/trollface_3.gif");
        gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 10);

        GtkWidget* label = gtk_label_new("HAI PERSO IL GIOCO!");
        gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 10);
    }
    else
    {
        gtk_window_set_title(GTK_WINDOW(win), "COMPLIMENTI!!!");

        image = gtk_image_new_from_file("data/varie/winner.png");
        gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 10);

        GtkWidget* label = gtk_label_new("HAI VINTO IL GIOCO!");
        gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 10);
    }
    gtk_widget_show_all(win);
}

/** Gestione del colore che si vuole utilizzare, in seguito ad un +4
 *  o cambio colore */
void gui_chiedi_colore(partita* match)
{
	GtkWidget *dialog;
	GtkWidget *button;
	GtkWidget *content;
	GtkWidget *hbox;

	// Struttura per la gestione dei colori
	GdkColor color;
	int scelta;

    // Modifica di una gtk_dialog per l'inserimento di un immagine e altri widget
	dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Due's Game - Cambio Colore");
	content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), gtk_image_new_from_file("data/varie/changecolor.png"), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new("Scegli il colore da utilizzare"), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(content), hbox, TRUE, TRUE, 0);

	button = gtk_button_new_with_label("Rosso");

	// Riempimento della struttura color
	gdk_color_parse("red", &color);

	// Modifica del colore di un widget
	gtk_widget_modify_bg(button, GTK_STATE_NORMAL, &color);
	gtk_dialog_add_action_widget(GTK_DIALOG(dialog), button, ROSSO);

	button = gtk_button_new_with_label("Giallo");
	gdk_color_parse("yellow", &color);
	gtk_widget_modify_bg(button, GTK_STATE_NORMAL, &color);
	gtk_dialog_add_action_widget(GTK_DIALOG(dialog), button, GIALLO);

	button = gtk_button_new_with_label("Verde");
	gdk_color_parse("green", &color);
	gtk_widget_modify_bg(button, GTK_STATE_NORMAL, &color);
	gtk_dialog_add_action_widget(GTK_DIALOG(dialog), button, VERDE);

	button = gtk_button_new_with_label("Blue");
	gdk_color_parse("blue", &color);
	gtk_widget_modify_bg(button, GTK_STATE_NORMAL, &color);
	gtk_dialog_add_action_widget(GTK_DIALOG(dialog), button, BLU);

	gtk_widget_show_all(dialog);

	scelta = gtk_dialog_run(GTK_DIALOG(dialog));

	gtk_widget_destroy(dialog);

	#if DEBUG
	     printf("Colore scelto: %d\n", scelta);
	#endif

    // Riapertura della finestra sinchè l'utente non effettua la scelta del colore
	if((scelta < 0) || (scelta > 3))
	     gui_chiedi_colore(match);

	match->colore = scelta;
}

/** Gestione della finestra +2 e +4 */
void gui_plus_carte(partita* match, int numero_carte)
{
    char path[128];
    GtkWidget* win;
    GtkWidget* image;
    GtkWidget* label;

    win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "Due's Game");
    gtk_window_set_default_size(GTK_WINDOW(win), 325, 150);
    gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);

    GtkWidget* vbox = gtk_vbox_new(TRUE, 10);
    gtk_container_add(GTK_CONTAINER(win), vbox);

    GtkWidget* hbox = gtk_hbox_new(TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 10);

    if(numero_carte == 2)
    {
        sprintf(path, "data/mazzi/%s/piu_due.png", match->nome_mazzo);
        image = gtk_image_new_from_file(path);
        label = gtk_label_new("Ricevi 2 carte!!");
    }
    else
    {
        sprintf(path, "data/mazzi/%s/piu_quattro.png", match->nome_mazzo);
        image = gtk_image_new_from_file(path);
        label = gtk_label_new("Ricevi 4 carte!!");
    }

        gtk_box_pack_start(GTK_BOX(hbox),image , TRUE, TRUE, 10);
        gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 10);

        GtkWidget* hbox2 = gtk_hbox_new(TRUE, 10);
        gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE ,FALSE , 10);

        GtkWidget* button = gtk_button_new_with_label("        OK        ");
        gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 10);
        g_signal_connect(button, "clicked", G_CALLBACK(gui_chiudi_finestra), win);
        gtk_widget_show_all(win);
}

/** Chiusura di una finestra al click del bottone*/
void gui_chiudi_finestra(GtkWidget* button, GtkWidget* finestra)
{
    gtk_widget_destroy(finestra);
}

/** Scelta del mazzo di carte con cui giocare */
void gui_finestra_mazzo(GtkWidget* item ,gui* sessione)
{
    int i;
    GtkTreeIter iter;
    char buf[128];

    sessione->finestra_mazzi = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(sessione->finestra_mazzi), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(sessione->finestra_mazzi), 350, 200);
    gtk_window_set_title(GTK_WINDOW(sessione->finestra_mazzi), "Due's Game - Sceglta mazzo");

    GtkWidget* vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(sessione->finestra_mazzi), vbox);

    GtkWidget* label = gtk_label_new("Scegli il mazzo con cui giocare");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

    // Creazione della list store
    sessione->lista_mazzi = gtk_list_store_new(2, GDK_TYPE_PIXBUF, GTK_TYPE_STRING);

    // Creazione della tree view
    sessione->tree_view_mazzi = gtk_tree_view_new_with_model(GTK_TREE_MODEL(sessione->lista_mazzi));
    gtk_box_pack_start(GTK_BOX(vbox), sessione->tree_view_mazzi, TRUE, TRUE, 0);

    // Creazione delle colonne della lista
    GtkTreeViewColumn* immagine;
    GtkTreeViewColumn* testo;

    immagine = gtk_tree_view_column_new_with_attributes("Anteprima", gtk_cell_renderer_pixbuf_new(), "pixbuf", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(sessione->tree_view_mazzi), immagine);

    testo = gtk_tree_view_column_new_with_attributes("Nome", gtk_cell_renderer_text_new(), "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(sessione->tree_view_mazzi), testo);

    // Riempimento delle righe della lista
    for(i = 0; i < MAZZI_NUM; i++)
    {
        sprintf(buf, "data/mazzi/%s/mini.png", mazzi[i]);
        gtk_list_store_append(sessione->lista_mazzi, &iter);
        gtk_list_store_set(sessione->lista_mazzi, &iter,
                            0, gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file(buf))),
                            1, mazzi[i], -1);
    }

    GtkWidget* hbox = gtk_hbox_new(TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    GtkWidget* button = gtk_button_new_with_label("       Ok       ");
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

    g_signal_connect(button, "clicked", G_CALLBACK(gui_cambia_mazzo), sessione);

    gtk_widget_show_all(sessione->finestra_mazzi);
}

/** Gestione del cambio di mazzo di carte scelto*/
void gui_cambia_mazzo(GtkWidget *row, gui *sessione)
{
    GtkTreeIter iter;
    GtkTreeSelection *selezione = gtk_tree_view_get_selection(GTK_TREE_VIEW(sessione->tree_view_mazzi));
    GtkTreeModel *model;

    if(gtk_tree_selection_get_selected (selezione, &model, &iter))
    {
        // Restituisce l'indice della riga scelta nella lista
        GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
        int *indices = gtk_tree_path_get_indices(path);

        if((indices[0] >= 0) && (indices[0] < MAZZI_NUM))
        {
            #if DEBUG
            printf("indice mazzo %d\n", indices[0]);
            #endif

            strcpy((sessione->match).nome_mazzo, mazzi[indices[0]]);
            gui_aggiorna(sessione);
        }
        gtk_widget_destroy(sessione->finestra_mazzi);
    }
}

/** Gestione della finestra credits */
void gui_finestra_credits()
{
    GtkWidget* finestra = NULL;
    finestra = gtk_message_dialog_new(GTK_WINDOW(finestra), GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                                "\nDavide Gessa  -  dagide@yahoo.it\n\nGianmarco Cherchi  -  gianmarcher@gmail.com\n\nSara Casti  -  sara_casti91@hotmail.it\n");

    gtk_window_set_title(GTK_WINDOW(finestra), "Due's Game - Credits");

    gtk_dialog_run(GTK_DIALOG(finestra));
    gtk_widget_destroy(finestra);
}

/** Gestione della finestra istruzioni */
void gui_finestra_istruzioni()
{
    GtkWidget* finestra;
    GtkWidget* label;
    GtkWidget* vbox;
    GtkWidget* content;

    finestra = gtk_dialog_new_with_buttons("Due's Game - Istruzioni", NULL, GTK_DIALOG_MODAL, GTK_STOCK_CLOSE, GTK_RESPONSE_NONE, NULL);

    content = gtk_dialog_get_content_area(GTK_DIALOG(finestra));
    vbox = gtk_vbox_new(FALSE, 10);
	gtk_box_pack_start(GTK_BOX(vbox), gtk_image_new_from_file("data/varie/testata2.png"), TRUE, TRUE, 0);
	label = gtk_label_new("Il GIOCO (avete perso) realizzato è una versione modificata del famosissimo gioco di carte UNO \n( http://it.wikipedia.org/wiki/UNO_(gioco_di_carte) ).\n\nLe uniche differenze sono:\n*\tViene dichiarato il possesso di 2 carte\n*\tIl +2 e il +4 non sono cumulabili\n*\tE' possibile cambiare il mazzo durante la partita");
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(content), vbox, TRUE, TRUE, 0);

    gtk_widget_show_all(finestra);
    gtk_dialog_run(GTK_DIALOG(finestra));
    gtk_widget_destroy(finestra);

}

/** Termine del programma */
void gui_esci(GtkWidget* item_esci)
{
    gtk_main_quit();
}
