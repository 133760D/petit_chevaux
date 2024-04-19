#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <wchar.h>
#include <fcntl.h>
#include <io.h>
#include "jeu.h"

void fullscreen() //force le plein ecran en simulant alt+entree
{
	keybd_event(VK_MENU, 0x38, 0, 0);
	keybd_event(VK_RETURN, 0x1c, 0, 0);
	keybd_event(VK_RETURN, 0X1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
}
void enlevescroll(HANDLE console) {
	CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
	GetConsoleScreenBufferInfo(console, &consoleinfo);
	COORD fenetre =
	{
		consoleinfo.dwMaximumWindowSize.X,consoleinfo.dwMaximumWindowSize.Y
		//consoleinfo.srWindow.Right - consoleinfo.srWindow.Left + 1,consoleinfo.srWindow.Bottom - consoleinfo.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(console, fenetre);
}
void xypos(short x, short y, HANDLE console)
{
	COORD pos = {x,y};
	SetConsoleCursorPosition(console, pos);
}
int termlarge(HANDLE console) { //taille console en largeur
	CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
	int colonnes;
	GetConsoleScreenBufferInfo(console, &consoleinfo);
	colonnes = consoleinfo.dwMaximumWindowSize.X - 1;
	return colonnes;
}
int termhaut(HANDLE console) { //taille console en hauteur (visible sur l'ecran, pas de scroll)
	CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
	int lignes;
	GetConsoleScreenBufferInfo(console, &consoleinfo);
	lignes = consoleinfo.dwMaximumWindowSize.Y - 1;
	return lignes;
}
void animstart(HANDLE console) {
	int haut = termhaut(console);
	int large = termlarge(console)-1;
	for (int i = 0; i <= large; i++) {
		xypos(i, 0, console);
		wprintf(L"#");
		xypos(large-i, haut, console);
		wprintf(L"#");
		Sleep(10);
	}
	xypos(0, 1, console); //l'art ascii qui suit a été pris sur http://www.ascii-fr.com/-Chevaux-.html
	wprintf(L"\n                                                 █    █\n                                            ███ ██   ██\n                                         █████ ████████\n                                        █████ ███ ███ █\n                                      ██████ ███ ███ ███\n                                       ████ ██ ██ ███████\n                                      █████ █ ██ ███████\n                                    ██████ █ █ █████████\n                                   █████ █████ █████████\n                         ███        ██ ███████ █████████\n                ███ ████████████    ███████████ ████████\n             ████ ███████████████████████████████ ███████\n           █████ ██████████████████████████████████ ██████\n         ██████ ████████████████████████████████████ █████\n        ██████ █████████████████████████████████████  ███\n        █████ ███████████████████████████████████████\n       ██████ ████████████████████████████████████████\n    █ ███████ ████████████████████████████████████████\n     █████████ ███████████████████████████████████████\n    ██████████ ████████████████████████████████████████\n ███ ████████   ██████ ████████████████████████████████\n   ████████      ██████ ████████████████████ ██████████\n█ ████████        ███████ ███████████ ██████ ██████████\n █████████         ███████  ███████████ ██████ ████████\n██████████          █████ ███  █████████ ███████ ██████\n ██████████          ████ ██               ███████ ████\n ███████████           ██ █                  █████ ███\n  ██  ██ █ ██         █ ██                      ██ ███\n    █   █    █        █ ███                      █ ███\n                       █ ███                     ██ ███\n                       █ ███                     ██ ███\n                       █ ████                   ████ ██\n                      ███ ███                  █████ ███\n                     ████  ███                 ████   ██\n                    █████   ███                 ██    ██\n                   █████    ████                      ███\n                    ██        ███                     ███\n                               ████                     ██\n                                ████                    ███\n                                                        ████\n                                                         ██\n");
	int ml = large / 2;
	xypos(ml-19, 5, console);
	SetConsoleTextAttribute(console, 417);
	wprintf(L"                                     ");
	xypos(ml-19, 6, console);
	wprintf(L"       JEU DES PETITS CHEVAUX        ");
	xypos(ml-19, 7, console);
	wprintf(L"      - par Alexandre Maurin         ");
	xypos(ml-19, 8, console);
	wprintf(L"                                     ");
	SetConsoleTextAttribute(console, 7);
	xypos(ml - 19, 10, console);
	wprintf(L"               -REGLES-              ");
	xypos(ml - 19, 11, console);
	wprintf(L"                                     ");
	xypos(ml - 19, 12, console);
	wprintf(L"2-4 joueurs disposent de 4 chevaux.  ");
	xypos(ml - 19, 13, console);
	wprintf(L"Pour sortir de l'écurie, il faut     "); 
	xypos(ml - 19, 14, console);
	wprintf(L"faire un 6 avec le dé. Le parcours se"); 
	xypos(ml - 19, 15, console);
	wprintf(L"fait d'abord sur la périphérie,      "); 
	xypos(ml - 19, 16, console);
	wprintf(L"Lorsqu'un cheval écrase un autre,    "); 
	xypos(ml - 19, 17, console);
	wprintf(L"celui-ci est renvoyé dans son écurie.");
	xypos(ml - 19, 18, console);
	wprintf(L"Un 6 permet de rejouer. Après avoir  "); 
	xypos(ml - 19, 19, console);
	wprintf(L"fait un tour, le joueur doit faire le");
	xypos(ml - 19, 20, console);
	wprintf(L"chiffre exact pour s’arrêter devant  ");
	xypos(ml - 19, 21, console);
	wprintf(L"son escalier. Si le chiffre est trop ");
	xypos(ml - 19, 22, console);
	wprintf(L"grand, il avance jusqu’à la case puis");
	xypos(ml - 19, 23, console);
	wprintf(L"repart à reculons. Pour monter les   ");
	xypos(ml - 19, 24, console);
	wprintf(L"marches, le joueur doit obtenir la   ");
	xypos(ml - 19, 25, console);
	wprintf(L"valeur exacte inscrite dans la case. ");
	xypos(ml - 19, 26, console);
	wprintf(L"Puis à nouveau un 6 pour atteindre le");
	xypos(ml - 19, 27, console);
	wprintf(L"sommet.                              ");
	xypos(ml - 19, 28, console);
	wprintf(L"                                     ");
	
	xypos(ml - 25, 35, console);
	SetConsoleTextAttribute(console, 3);
	wprintf(L"APPUYEZ SUR N'IMPORTE QUELLE TOUCHE POUR COMMENCER");
	SetConsoleTextAttribute(console, 7);
	_getch();
	for (short i = 0; i < 24; i++) {
		xypos(ml - 19, 5+i, console);
		wprintf(L"                                     ");
	}
	xypos(ml - 25, 35, console);
	wprintf(L"                                                  ");
}
void menu(short pos, HANDLE console) {
	int ml = termlarge(console) / 2;
	int mh = termhaut(console) / 2;
	switch (pos)
	{
	case 1:
		xypos(ml - 9, mh - 2, console);
		wprintf(L"     ʀᴇᴘʀᴇɴᴅʀᴇ   ");//j'utilise des lettres "majuscules minuscules" pour un effet zoom
		xypos(ml - 9, mh - 1, console);
		wprintf(L"  ɴᴏᴜᴠᴇʟʟᴇ ᴘᴀʀᴛɪᴇ");
		xypos(ml - 11, mh, console);
		wprintf(L"  ᴄʜᴀʀɢᴇʀ sᴀᴜvᴇɢᴀʀᴅᴇ");
		xypos(ml - 9, mh + 1, console);
		wprintf(L"    > QUITTER    ");
		xypos(ml - 23, mh + 5, console);
		wprintf(L"utiliser les fleches haut et bas pour naviguer");
		break;
	case 2:
		xypos(ml - 9, mh - 2, console);
		wprintf(L"     ʀᴇᴘʀᴇɴᴅʀᴇ   ");
		xypos(ml - 9, mh - 1, console);
		wprintf(L"  ɴᴏᴜᴠᴇʟʟᴇ ᴘᴀʀᴛɪᴇ");
		xypos(ml - 11, mh, console);
		wprintf(L"> CHARGER SAUVEGARDE");
		xypos(ml - 9, mh + 1, console);
		wprintf(L"      qᴜɪᴛᴛᴇʀ    ");
		xypos(ml - 23, mh + 5, console);
		wprintf(L"utiliser les fleches haut et bas pour naviguer");
		break;
	case 3:
		xypos(ml - 9, mh - 2, console);
		wprintf(L"     ʀᴇᴘʀᴇɴᴅʀᴇ   ");
		xypos(ml - 9, mh - 1, console);
		wprintf(L"> NOUVELLE PARTIE");
		xypos(ml - 11, mh, console);
		wprintf(L"  ᴄʜᴀʀɢᴇʀ sᴀᴜᴠᴇɢᴀʀᴅᴇ");
		xypos(ml - 9, mh + 1, console);
		wprintf(L"      qᴜɪᴛᴛᴇʀ    ");
		xypos(ml - 23, mh + 5, console);
		wprintf(L"utiliser les fleches haut et bas pour naviguer");
		break;
	case 4:
		xypos(ml - 9, mh - 2, console);
		wprintf(L"   > REPRENDRE   ");
		xypos(ml - 9, mh - 1, console);
		wprintf(L"  ɴᴏᴜᴠᴇʟʟᴇ ᴘᴀʀᴛɪᴇ");
		xypos(ml - 11, mh, console);
		wprintf(L"  ᴄʜᴀʀɢᴇʀ sᴀᴜvᴇɢᴀʀᴅᴇ");
		xypos(ml - 9, mh + 1, console);
		wprintf(L"      qᴜɪᴛᴛᴇʀ    ");
		xypos(ml - 23, mh + 5, console);
		wprintf(L"utiliser les fleches haut et bas pour naviguer");
		break;
	}
}
void optionjeu(short menupos, short nbjoueur, short nbjauto, char** nomsjoueurs, HANDLE console) {
	int ml = termlarge(console) / 2;
	int mh = termhaut(console) / 2;
	if (menupos == 101) {
		xypos(ml - 15, mh - 6, console);
		wprintf(L"> Nombre de Joueurs = < %d >", nbjoueur);
	}
	else {
		xypos(ml - 15, mh - 6, console);
		wprintf(L"    Nombre de Joueurs = %d  ", nbjoueur);
	}
	if (menupos == 102) {
		xypos(ml - 15, mh - 4, console);
		wprintf(L"> Joueurs autonomes = < %d >", nbjauto);
	}
	else {
		xypos(ml - 15, mh - 4, console);
		wprintf(L"    Joueurs autonomes = %d  ", nbjauto);
	}
	
	if (menupos == 103) {
		xypos(ml - 13, mh - 2, console);
		wprintf(L" Nom Joueur 1 = >%S< ", nomsjoueurs[0]);
	}
	else {
		xypos(ml - 13, mh - 2, console);
		wprintf(L" Nom Joueur 1 = %S  ", nomsjoueurs[0]);
	}
	if (menupos == 104) {
		xypos(ml - 13, mh, console);
		wprintf(L" Nom Joueur 2 = >%S< ", nomsjoueurs[1]);
	}
	else {
		xypos(ml - 13, mh, console);
		wprintf(L" Nom Joueur 2 = %S  ", nomsjoueurs[1]);
	}
	if (menupos == 105) {
		xypos(ml - 13, mh + 2, console);
		wprintf(L" Nom Joueur 3 = >%S< ", nomsjoueurs[2]);
	}
	else {
		xypos(ml - 13, mh + 2, console);
		wprintf(L" Nom Joueur 3 = %S  ", nomsjoueurs[2]);
	}
	if (menupos == 106) {
		xypos(ml - 13, mh + 4, console);
		wprintf(L" Nom Joueur 4 = >%S< ", nomsjoueurs[3]);
	}
	else {
		xypos(ml - 13, mh + 4, console);
		wprintf(L" Nom Joueur 4 = %S  ", nomsjoueurs[3]);
	}
	if (menupos == 107) {
		xypos(ml - 13, mh + 6, console);
		wprintf(L"    > LANCER PARTIE <    ");
	}
	else {
		xypos(ml - 13, mh + 6, console);
		wprintf(L"      LANCER PARTIE      ");
	}
	
}
void change_nbjoueur(char op, short *nbjoueur, HANDLE console) {
	if (op=='+') *nbjoueur = *nbjoueur + 1;
	else *nbjoueur = *nbjoueur - 1;
	int ml = termlarge(console) / 2;
	int mh = termhaut(console) / 2;
	xypos(ml + 7, mh - 6, console);
	wprintf(L"< %d >", *nbjoueur);
}
void change_nbjauto(char op, short *nbjauto, HANDLE console) {
	if (op == '+') *nbjauto = *nbjauto + 1;
	else *nbjauto = *nbjauto - 1;
	int ml = termlarge(console) / 2;
	int mh = termhaut(console) / 2;
	xypos(ml + 7, mh - 4, console);
	wprintf(L"< %d >", *nbjauto);
}

void plateau(HANDLE console) {
	//dimension: 35x35
	//♞ <- dommage, la plupart des terminaux windows n'ont pas de fonts par default qui puisse l'afficher
	SetConsoleTextAttribute(console, 79);
	int ml = termlarge(console) / 2;
	int mh = termhaut(console) / 2;
	xypos(ml - 16, mh - 19, console);
	wprintf(L"# # # # # # # # #");
	xypos(ml - 16, mh - 18, console);
	wprintf(L"#              _ ");
	xypos(ml - 16, mh - 17, console);
	wprintf(L"#             |□ ");
	for (short i = 16; i > 12; i--) {
		xypos(ml - 16, mh - i, console);
		wprintf(L"#             |□|");
	}
	SetConsoleTextAttribute(console, 31);
	xypos(ml + 1, mh - 19, console);
	wprintf(L" # # # # # # # # #\n");
	xypos(ml + 1, mh - 18, console);
	wprintf(L"_ _              #\n");
	xypos(ml + 1, mh - 17, console);
	wprintf(L"□ D|             #\n");
	for (short i = 1; i < 5; i++) {
		xypos(ml+1, mh - 17 + i, console);
		wprintf(L"%d|□|             #", i);
	}
	SetConsoleTextAttribute(console, 79);
	xypos(ml - 16, mh - 12, console);
	wprintf(L"#  _ _ _ _ _ _|□|");
	SetConsoleTextAttribute(console, 31);
	wprintf(L"5|□|_ _ _ _ _ _  #\n");
	SetConsoleTextAttribute(console, 79);
	xypos(ml - 16, mh - 11, console);
	wprintf(L"# |D □ □ □ □ □ □|");
	SetConsoleTextAttribute(console, 31);
	wprintf(L"6|□ □ □ □ □ □ □| #\n");
	SetConsoleTextAttribute(console, 79);
	xypos(ml - 16, mh - 10, console);
	wprintf(L"# |□|1|2|3|4|5|6|");
	SetConsoleTextAttribute(console, 255);
	wprintf(L" ");
	SetConsoleTextAttribute(console, 111);
	wprintf(L"|6|5|4|3|2|1|□| #\n");
	SetConsoleTextAttribute(console, 47);
	xypos(ml - 16, mh - 9, console);
	wprintf(L"# |□ □ □ □ □ □ □|6");
	SetConsoleTextAttribute(console, 111);
	wprintf(L"|□ □ □ □ □ □ D| #\n");
	xypos(ml - 16, mh - 8, console);
	SetConsoleTextAttribute(console, 47);
	wprintf(L"#  ‾ ‾ ‾ ‾ ‾ ‾|□|5");
	SetConsoleTextAttribute(console, 111);
	wprintf(L"|□|‾ ‾ ‾ ‾ ‾ ‾  #\n");
	SetConsoleTextAttribute(console, 47);
	for (short i = 7; i > 3; i--) {
		xypos(ml - 16, mh - i, console);
		wprintf(L"#             |□|%d", i-3);
	}
	xypos(ml - 16, mh - 3, console);
	wprintf(L"#             |D □");
	SetConsoleTextAttribute(console, 111);
	wprintf(L" □|             #\n");
	for (short i = 1; i < 5; i++) {
		xypos(ml + 2, mh - 3 - i, console);
		wprintf(L"|□|             #");
	}
	xypos(ml - 16, mh - 2, console);
	SetConsoleTextAttribute(console, 47);
	wprintf(L"#              ‾ ‾");
	SetConsoleTextAttribute(console, 111);
	wprintf(L" ‾              #");
	xypos(ml - 16, mh - 1, console);
	SetConsoleTextAttribute(console, 47);
	wprintf(L"# # # # # # # # # ");
	SetConsoleTextAttribute(console, 111);
	wprintf(L"# # # # # # # # #");
}
void initialisation(JOUEUR* joueurs, short nbjoueur, short nbjauto, POS* cases, HANDLE console) {
	short x = (termlarge(console) / 2) - 16; //correspond au coin en bas a gauche du plateau
	short y = (termhaut(console) / 2) - 1;
	for (short i = 0; i < (nbjoueur+nbjauto); i++) {
		joueurs[i].chevaux = malloc(4 * sizeof(CHEVAL)); //(2*2short + 1short)*4chevaux
	}
	for (short i = 0; i < 4; i++) {
		short hauteur = 0;
		if (i % 2 == 0) hauteur=1;
		joueurs[0].chevaux[i].positiondepart.x = x + 6 + i;
		joueurs[0].chevaux[i].positiondepart.y = y - 4 + hauteur;
		joueurs[0].chevaux[i].sorti = 0;
		joueurs[0].couleur = 0;
		joueurs[0].depart = 0;
		joueurs[0].fin = 55;
		joueurs[0].escalier = malloc(6 * sizeof(POS));
		for (short k = 0; k < 6; k++) {
			joueurs[0].escalier[k].x = x + 17;
			joueurs[0].escalier[k].y = y - 3 - k;
		}
		if ((nbjoueur + nbjauto) == 2) {
			joueurs[1].chevaux[i].positiondepart.x = x + 24 + i;
			joueurs[1].chevaux[i].positiondepart.y = y - 14 + hauteur;
			joueurs[1].chevaux[i].sorti = 0;
			joueurs[1].couleur = 2;
			joueurs[1].depart = 28;
			joueurs[1].fin = 27;
			joueurs[1].escalier = malloc(6 * sizeof(POS));
			for (short k = 0; k < 6; k++) {
				joueurs[1].escalier[k].x = x + 15 - k*2;
				joueurs[1].escalier[k].y = y - 16;
			}
		}
		else {// 3 joueurs ou plus
			joueurs[1].chevaux[i].positiondepart.x = x + 6 + i;
			joueurs[1].chevaux[i].positiondepart.y = y - 14 + hauteur;
			joueurs[1].chevaux[i].sorti = 0;
			joueurs[1].couleur = 1;
			joueurs[1].depart = 14;
			joueurs[1].fin = 13;
			joueurs[1].escalier = malloc(6 * sizeof(POS));
			for (short k = 0; k < 6; k++) {
				joueurs[1].escalier[k].x = x + 5 + k * 2;
				joueurs[1].escalier[k].y = y - 9;
			}
			joueurs[2].chevaux[i].positiondepart.x = x + 24 + i;
			joueurs[2].chevaux[i].positiondepart.y = y - 14 + hauteur;
			joueurs[2].chevaux[i].sorti = 0;
			joueurs[2].couleur = 2;
			joueurs[2].depart = 28;
			joueurs[2].fin = 27;
			joueurs[2].escalier = malloc(6 * sizeof(POS));
			for (short k = 0; k < 6; k++) {
				joueurs[2].escalier[k].x = x + 15 - k * 2;
				joueurs[2].escalier[k].y = y - 16;
			}
			if ((nbjoueur + nbjauto) == 4) { // 4ieme joueurs
				joueurs[3].chevaux[i].positiondepart.x = x + 24 + i;
				joueurs[3].chevaux[i].positiondepart.y = y - 4 + hauteur;
				joueurs[3].chevaux[i].sorti = 0;
				joueurs[3].couleur = 3;
				joueurs[3].depart = 42;
				joueurs[3].fin = 41;
				joueurs[3].escalier = malloc(6 * sizeof(POS));
				for (short k = 0; k < 6; k++) {
					joueurs[3].escalier[k].x = x + 29 - k * 2;
					joueurs[3].escalier[k].y = y - 9;
				}
			}
		}

	}
	for (short j = 0; j < (nbjoueur + nbjauto); j++) {
		couleur(joueurs[j].couleur, console);
		for (short i = 0; i < 4; i++) {
			xypos(joueurs[j].chevaux[i].positiondepart.x, joueurs[j].chevaux[i].positiondepart.y, console);
			wprintf(L"$");
		}
	}
	//on s'occupe des cases maintenant
	for (short i = 0; i < 7; i++) {
		cases[i].x = x+15;
		cases[i].y = y-2-i;
		cases[i+6].x = x + 15 - i*2;
		cases[i+6].y = y - 8;
		cases[i + 14].x = x + 3 + i * 2;
		cases[i + 14].y = y - 10;
		cases[i + 20].x = x + 15;
		cases[i + 20].y = y - 10-i;
		cases[i + 28].x = x + 19;
		cases[i + 28].y = y - 16 + i;
		xypos(cases[i + 28].x, cases[i + 28].y, console);
		cases[i + 34].x = x + 19 + i * 2;
		cases[i + 34].y = y - 10;
		cases[i + 42].x = x + 31 - i * 2;
		cases[i + 42].y = y - 8;
		cases[i + 48].x = x + 19;
		cases[i + 48].y = y - 8 + i;

	}
	cases[13].x = x + 3;
	cases[13].y = y - 9;
	cases[27].x = x + 17;
	cases[27].y = y - 16;
	cases[41].x = x + 31;
	cases[41].y = y - 9;
	cases[55].x = x + 17;
	cases[55].y = y - 2;
	
	//73,26
}
void couleurbg(POS* cases, short couleur, CHEVAL cheval, HANDLE console) {
	short ml = (termlarge(console) / 2);
	short mh = (termhaut(console) / 2);
	short x = 0;
	short y = 0;
		if (cheval.sorti == 0) {
			x = cheval.positiondepart.x;
			y = cheval.positiondepart.y;
		}
		else {
			x = cases[cheval.pos].x;
			y = cases[cheval.pos].y;
		}
		if (x < ml + 2) {//vert ou rouge (ou bleu)
			if (y < mh - 9) {//rouge (ou bleu)
				if (x == ml + 1) {//bleu
					switch (couleur) {
					case -1://blanc sur bleu
						SetConsoleTextAttribute(console, 31);
						break;
					case 0://vert sur bleu
						SetConsoleTextAttribute(console, 26);
						break;
					case 1://rouge sur bleu
						SetConsoleTextAttribute(console, 28);
						break;
					case 2://bleu sur bleu
						SetConsoleTextAttribute(console, 27);
						break;
					case 3://jaune sur bleu
						SetConsoleTextAttribute(console, 30);
						break;
					}
				}
				else {//rouge
					switch (couleur) {
					case -1://blanc sur rouge
						SetConsoleTextAttribute(console, 79);
						break;
					case 0://vert sur rouge
						SetConsoleTextAttribute(console, 74);
						break;
					case 1://rouge sur rouge
						SetConsoleTextAttribute(console, 76);
						break;
					case 2://bleu sur rouge
						SetConsoleTextAttribute(console, 75);
						break;
					case 3://jaune sur rouge
						SetConsoleTextAttribute(console, 78);
						break;
					}
				}
			}
			else {//vert
				switch (couleur) {
				case -1://blanc sur vert
					SetConsoleTextAttribute(console, 47);
					break;
				case 0://vert sur vert
					SetConsoleTextAttribute(console, 42);
					break;
				case 1://rouge sur vert
					SetConsoleTextAttribute(console, 44);
					break;
				case 2://bleu sur vert
					SetConsoleTextAttribute(console, 43);
					break;
				case 3://jaune sur vert
					SetConsoleTextAttribute(console, 46);
					break;
				}
			}
		}
	else {//bleu ou jaune
		if (y < mh-10) {//bleu
			switch (couleur) {
			case -1://blanc sur bleu
				SetConsoleTextAttribute(console, 31);
				break;
			case 0://vert sur bleu
				SetConsoleTextAttribute(console, 26);
				break;
			case 1://rouge sur bleu
				SetConsoleTextAttribute(console, 28);
				break;
			case 2://bleu sur bleu
				SetConsoleTextAttribute(console, 27);
				break;
			case 3://jaune sur bleu
				SetConsoleTextAttribute(console, 30);
				break;
			}
		}
		else {//jaune
			switch (couleur) {
			case -1://blanc sur jaune
				SetConsoleTextAttribute(console, 111);
				break;
			case 0://vert sur jaune
				SetConsoleTextAttribute(console, 106);
				break;
			case 1://rouge sur jaune
				SetConsoleTextAttribute(console, 108);
				break;
			case 2://bleu sur jaune
				SetConsoleTextAttribute(console, 107);
				break;
			case 3://jaune sur jaune
				SetConsoleTextAttribute(console, 110);
				break;
			}
		}
	}
}
void couleur(short c, HANDLE console) {
	switch (c){
	case 0://vert
		SetConsoleTextAttribute(console, 42);
		break;
	case 1://rouge
		SetConsoleTextAttribute(console, 76);
		break;
	case 2://bleu
		SetConsoleTextAttribute(console, 27);
		break;
	case 3://jaune
		SetConsoleTextAttribute(console, 110);
		break;
	}
}

short lancede() {
	return (rand() % 6 + 1);//nombre "aleatoire" entre 1-6
}

void refreshcheval(POS* cases, CHEVAL cheval, JOUEUR joueur, short couleur, HANDLE console) {
	if (cheval.sorti == 0) xypos(cheval.positiondepart.x, cheval.positiondepart.y, console);//cheval pas sorti
	else if (cheval.sorti == 1) xypos(cases[cheval.pos].x, cases[cheval.pos].y, console);//cheval sorti
	else xypos(joueur.escalier[cheval.escalier].x, joueur.escalier[cheval.escalier].y, console);//cheval à l'escalier
	couleurbg(cases, couleur, cheval, console);
	
}
void suivant(short *menupos, short *tourde, JOUEUR* joueurs, short nbj, HANDLE console) {
	Sleep(1500);
	xypos(0, 15, console);
	wprintf(L"                                  ");
	(*tourde) += 1;
	if ((*tourde) == nbj) (*tourde) = 0;//on a fait un tour
	(*menupos) = 20;
	xypos(18, 10, console);
	wprintf(L"                ");
	xypos(0, 14, console);
	wprintf(L"              ");
	xypos(0, 0, console);
	Sleep(500);
	xypos(0, 10, console);
	wprintf(L"c'est au tour de: %S", joueurs[(*tourde)].nom);
	xypos(0, 0, console);
	Sleep(500);
	xypos(18, 10, console);
	wprintf(L"                ");
	xypos(0, 0, console);
	Sleep(500);
	xypos(0, 10, console);
	wprintf(L"c'est au tour de: %S", joueurs[(*tourde)].nom);
	xypos(0, 0, console);
	Sleep(500);
	xypos(0, 14, console);
	wprintf(L"APPUYEZ SUR ESPACE POUR LANCER LE DÉ");
}
void rejouer(short *menupos, short tourde, JOUEUR* joueurs, HANDLE console) {
	xypos(0, 15, console);
	wprintf(L"                                  ");
	(*menupos) = 20;
	xypos(0, 14, console);
	wprintf(L"              ");
	xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
	wprintf(L"<                       Vous pouvez rejouer.                       >");
	Sleep(500);
	xypos(0, 14, console);
	wprintf(L"APPUYEZ SUR ESPACE POUR LANCER LE DÉ");
}

void ecrasecheval(short pos, short nbj, short tourde, JOUEUR* joueurs, POS* cases, HANDLE console) {
	for (short i = 0; i < (nbj); i++) {
		for (short j = 0; j < 4; j++) {//il ecrase un autre cheval
			if ((joueurs[i].chevaux[j].sorti == 1) && (joueurs[i].chevaux[j].pos == pos)) {
				joueurs[i].chevaux[j].sorti = 0;
				couleurbg(cases, joueurs[i].couleur, joueurs[i].chevaux[j], console);
				xypos(joueurs[i].chevaux[j].positiondepart.x, joueurs[i].chevaux[j].positiondepart.y, console);
				wprintf("$");
				break;//il ne peut y avoir qu'un seul cheval par case, donc continuer la boucle est inutile
			}
		}
	}
}

void deplacecheval(CHEVAL *cheval, short couleur, short pos, POS* cases, HANDLE console) {
	couleurbg(cases, -1, (*cheval), console);
	xypos(cases[(*cheval).pos].x, cases[(*cheval).pos].y, console);
	if (((*cheval).pos == 0) || ((*cheval).pos == 14) || ((*cheval).pos == 28) || ((*cheval).pos == 42)) wprintf(L"D");
	else wprintf(L"□");
	(*cheval).pos = pos;
	couleurbg(cases, couleur, (*cheval), console);
	xypos(cases[pos].x, cases[pos].y, console);
	wprintf(L"$");
	SetConsoleTextAttribute(console, 7);
}
void menufin(short pos, HANDLE console) {
	int ml = termlarge(console) / 2 + 39;
	int mh = termhaut(console) / 2 - 16;
	switch (pos)
	{
	case 1001:
		xypos(ml - 9, mh+1, console);
		wprintf(L"  ɴᴏᴜᴠᴇʟʟᴇ ᴘᴀʀᴛɪᴇ");
		xypos(ml - 11, mh+3, console);
		wprintf(L"  ᴄʜᴀʀɢᴇʀ sᴀᴜvᴇɢᴀʀᴅᴇ");
		xypos(ml - 9, mh + 5, console);
		wprintf(L"    > QUITTER    ");
		break;
	case 1002:
		xypos(ml - 9, mh + 1, console);
		wprintf(L"  ɴᴏᴜᴠᴇʟʟᴇ ᴘᴀʀᴛɪᴇ");
		xypos(ml - 11, mh+3, console);
		wprintf(L"> CHARGER SAUVEGARDE");
		xypos(ml - 9, mh + 5, console);
		wprintf(L"      qᴜɪᴛᴛᴇʀ    ");
		break;
	case 1003:
		xypos(ml - 9, mh + 1, console);
		wprintf(L"> NOUVELLE PARTIE");
		xypos(ml - 11, mh+3, console);
		wprintf(L"  ᴄʜᴀʀɢᴇʀ sᴀᴜᴠᴇɢᴀʀᴅᴇ");
		xypos(ml - 9, mh + 5, console);
		wprintf(L"      qᴜɪᴛᴛᴇʀ    ");
		break;
	}
}