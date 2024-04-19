#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <wchar.h>
#include <fcntl.h>
#include <io.h>
#include "jeu.h"
#include <time.h>
int main(int argc, char *argv[]) {
	srand(time(NULL));//seed de la fonction random pour avoir de "l'aleatoire"
	short nbjoueur = 4; //nombre de joueurs par défaut
	short nbjauto = 0; //nombre de joueurs autonomes par défaut
	short tourde = 0;
	char keypressed;
	short blink = 0;//pour faire clignoter la selection
	JOUEUR* joueurs;
	POS* cases;
	short menupos = 3; //position sur le menu par défaut (3 = Nouvelle Partie)
	char** nomsjoueurs;
	nomsjoueurs = malloc(sizeof(char*)*4);
	for (short i = 0; i < 4; i++) {
		nomsjoueurs[i] = malloc(16 * sizeof(char)); //sizeof(char) = 1
		snprintf(nomsjoueurs[i], 16 * sizeof(char), "JOUEUR %d", i+1);
	}
	
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
	_setmode(_fileno(stdout), _O_U16TEXT);
	GetConsoleScreenBufferInfo(console, &consoleinfo);
	enlevescroll(console);
	fullscreen();
	Sleep(10);//pour pas que certains calculs se lance avant que la console est eu le temps de se mettre en plein ecran
	wprintf(L" ");//la toute première fois qu'on lance le programme la taille du fullscreen n'est pas 'validé' tant qu'on ne s'est encore jamais servi de la sortie standard depuis l'allumage de l'ordi (bug?) donc print juste avant de faire des calculs de taille de fenetre
	animstart(console);
recommencer:// <- goto pour la fin pour tout recommencer (je justifie l'usage de ce goto dans mon rapport)
	menu(menupos, console);
	while (1) {
		xypos(0, 0, console); //pour bloquer la position de la fenetre et ne pas avoir le petit truc blanc qui clignote en plein milieu
		keypressed = NULL; //important autrement la touche pressé va boucler en chaine
		while (_kbhit()) {
			Sleep(10);
			while (_kbhit()) keypressed = _getch();
		}

		Sleep(10);
		if (keypressed == 27 ) exit(0);//touche échap pour quitter
		//switch qui gère les touches du clavier
		if (menupos < 10) { //Menu principal
		switch (keypressed) {
		case 72: //fleche haut
			if (menupos < 4) {
				++menupos;
				menu(menupos, console);
			}
			break;

		case 80: //fleche bas
			if (menupos > 1) {
				--menupos;
				menu(menupos, console);
			}
			break;

		case '\r': //entree
		case ' ': //espace
			switch (menupos) {
			case 1: //quitter
				exit(0);
			case 2: //charger sauvegarde
				break;
			case 3: //nouvelle partie
				menupos = 101;//valeur pour determiner que l'on est dans les options de nouvelle partie
				system("cls"); //clear la console (sera system("clear") pour linux)
				optionjeu(menupos, nbjoueur, nbjauto, nomsjoueurs, console);
				break;
			case 4: //reprendre
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}//fin du switch
		}
		else if ((menupos > 100) && (menupos < 200)) {//Menu des options de jeu
			switch (keypressed) {
			case 72: //fleche haut
				if (menupos > 101) {
					--menupos;
					optionjeu(menupos, nbjoueur, nbjauto, nomsjoueurs, console);
				}
				break;

			case 80: //fleche bas
				if (menupos < 107) {
					++menupos;
					optionjeu(menupos, nbjoueur, nbjauto, nomsjoueurs, console);
				}
				break;

			case 75: //fleche gauche
				if (menupos == 101) {
					if (nbjoueur > 1) {
						if ((nbjoueur == 2) && (nbjauto == 0)) {
							nbjauto++;
							int ml = termlarge(console) / 2;
							int mh = termhaut(console) / 2;
							xypos(ml + 7, mh - 4, console);
							wprintf(L" %d  ", nbjauto);
						}
						change_nbjoueur('-', &nbjoueur, console);
					}
				}
				else if (menupos == 102) {
					if (nbjauto > 0) {
						if (nbjoueur == 1) {
							nbjoueur++;
							int ml = termlarge(console) / 2;
							int mh = termhaut(console) / 2;
							xypos(ml + 7, mh - 6, console);
							wprintf(L" %d  ", nbjoueur);
						}
						change_nbjauto('-', &nbjauto, console);
						}
				}
				break;
			case 77: //fleche droite
				if (menupos == 101) {
					if (nbjoueur < 4) {
						if ((nbjoueur + nbjauto) == 4) {
							nbjauto--;
							int ml = termlarge(console) / 2;
							int mh = termhaut(console) / 2;
							xypos(ml + 7, mh - 4, console);
							wprintf(L" %d  ", nbjauto);
						}
						change_nbjoueur('+', &nbjoueur, console);
					}
				}
				else if (menupos == 102) {
					if (nbjauto < 3) {
						if ((nbjoueur + nbjauto) == 4) {
							nbjoueur--;
							int ml = termlarge(console) / 2;
							int mh = termhaut(console) / 2;
							xypos(ml + 7, mh - 6, console);
							wprintf(L" %d  ", nbjoueur);
						}
						change_nbjauto('+', &nbjauto, console);
					}
				}
				break;

			case '\r': //entree
			case ' ': //espace
				if (menupos == 107) {// "lancer partie"
					menupos = 20; //la valeur pour dire qu'on est en jeu
					joueurs = malloc(sizeof(JOUEUR)*(nbjoueur + nbjauto));
					for (short i = 0; i < nbjauto; i++) {
						joueurs[(nbjoueur + nbjauto)-i-1].IA = 1;
					}
					for (short i = 0; i < (nbjoueur + nbjauto); i++) {//
						joueurs[i].nom = malloc(sizeof(char) * (strlen(nomsjoueurs[i]) + sizeof(char)));
						strcpy(joueurs[i].nom, nomsjoueurs[i]);
						free(nomsjoueurs[i]);
					}
					free(nomsjoueurs);
					system("cls");
					cases = malloc(sizeof(POS) * 56);//56cases en peripherie
					plateau(console);
					initialisation(joueurs, nbjoueur, nbjauto, cases, console);
					xypos(0, 0, console);
					SetConsoleTextAttribute(console, 7);
					wprintf(L"Appuyez sur ECHAP pour quitter");
					xypos(0, 10, console);
					wprintf(L"c'est au tour de: %S", joueurs[0].nom);
					xypos(0, 14, console);
					wprintf(L"APPUYEZ SUR ESPACE POUR LANCER LE DÉ");
				}
				break;

			default:
				break;
			}//fin du switch
		}
		else if (menupos == 20 || menupos == 21) {//on est en jeu
			if (joueurs[tourde].IA == 1) {
				if (menupos == 20) {
					keybd_event(VK_RETURN, 0x1c, 0, 0);
					keybd_event(VK_RETURN, 0X1c, KEYEVENTF_KEYUP, 0);
				}
				else {// pas fini implémentation
					short choixia = 0;
					for (short i = 0; i < 4; i++) {
						//du mouvement le plus judicieux au moins judicieux)
						if ((joueurs[tourde].chevaux[i].sorti == 2) && (joueurs[tourde].chevaux[i].escalier == 6) && (joueurs[tourde].lance == 6)) {
							joueurs[tourde].selection = i;
							choixia = 1;
							break;
						}
					}
					if (choixia = 0) {
						for (short i = 0; i < 4; i++) {
							if ((joueurs[tourde].chevaux[i].sorti == 2) && (joueurs[tourde].chevaux[i].escalier == (joueurs[tourde].lance - 1))) {
								joueurs[tourde].selection = i;
								choixia = 1;
								break;
							}
						}
					}
					if (choixia = 0) {
						for (short i = 0; i < 4; i++) {
							if ((joueurs[tourde].chevaux[i].sorti == 1) && (joueurs[tourde].chevaux[i].pos == joueurs[tourde].fin) && (joueurs[tourde].lance == 1)) {
								joueurs[tourde].selection = i;
								choixia = 1;
								break;
							}
						}
					}
					if (choixia = 0) {
						for (short i = 0; i < 4; i++) {
							if ((joueurs[tourde].chevaux[i].sorti == 1) && (joueurs[tourde].chevaux[i].pos != joueurs[tourde].fin)) {
								joueurs[tourde].selection = i;
								choixia = 1;
								break;
							}
						}
					}
					if (choixia = 0) {
						for (short i = 0; i < 4; i++) {
							if ((joueurs[tourde].chevaux[i].sorti == 0) && (joueurs[tourde].lance == 6)) {
								joueurs[tourde].selection = i;
								choixia = 1;
								break;
							}
						}
					}
				}
				keybd_event(VK_RETURN, 0x1c, 0, 0);
				keybd_event(VK_RETURN, 0X1c, KEYEVENTF_KEYUP, 0);
			}
			switch (keypressed) {
			case '\r': //entree
			case ' ': //espace
				if (menupos == 20) {
					short check = 0;
					joueurs[tourde].lance = lancede();
					menupos = 21;
					joueurs[tourde].selection = 0;
					xypos(0, 14, console);
					wprintf(L"votre lancé: %d                      ", joueurs[tourde].lance);
					if ((joueurs[tourde].lance != 6) && (joueurs[tourde].chevaux[0].sorti == 0) && (joueurs[tourde].chevaux[1].sorti == 0) && (joueurs[tourde].chevaux[2].sorti == 0) && (joueurs[tourde].chevaux[3].sorti == 0)) {
						xypos(0, 15, console);
						wprintf(L"VOUS NE POUVEZ FAIRE AUCUNE ACTION");
						xypos(0, 0, console);
						//suivant(&menupos, &tourde, joueurs, (nbjoueur + nbjauto), console);
						check = -1;
					}
					if (check != -1) {//-1 = le joueur ne peut pas jouer
						for (short i = 0; i < 4; i++) {
							if ((joueurs[tourde].chevaux[i].sorti == 0) && (joueurs[tourde].lance == 6)) {
								check++;
								break;//au moins 1 mouvement possible, pas besoin de verifier plus
							}
							else if ((joueurs[tourde].chevaux[i].sorti == 1) && (joueurs[tourde].chevaux[i].pos != joueurs[tourde].fin)) {
								check++;
								break;
							}
							else if ((joueurs[tourde].chevaux[i].sorti == 1) && (joueurs[tourde].chevaux[i].pos == joueurs[tourde].fin) && (joueurs[tourde].lance == 1)) {
								check++;
								break;
							}
							else if ((joueurs[tourde].chevaux[i].sorti == 2) && (joueurs[tourde].chevaux[i].escalier == (joueurs[tourde].lance - 1))) {
								check++;
								break;
							}
							else if ((joueurs[tourde].chevaux[i].sorti == 2) && (joueurs[tourde].chevaux[i].escalier == 6) && (joueurs[tourde].lance == 6)) {
								check++;
								break;
							}
						}
					}
					if (check < 1) {//si on n'a aucun coup possible
						SetConsoleTextAttribute(console, 7);
						if (joueurs[tourde].lance == 6) rejouer(&menupos, tourde, joueurs, console);
						else suivant(&menupos, &tourde, joueurs, (nbjoueur + nbjauto), console);
					}
					else {
						xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
						wprintf(L"< utilisez les fleches directionnelles pour selectionner un cheval >");
					}
				}
				else {//le cas où le joueur valide la selection
					if (joueurs[tourde].chevaux[joueurs[tourde].selection].sorti == 0) {//le cheval choisi est dans l'ecurie
						if (joueurs[tourde].lance != 6) {//il ne peut pas le sortir
							SetConsoleTextAttribute(console, 7);
							xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
							wprintf(L"<                VOUS NE POUVEZ PAS SORTIR CE CHEVAL               >");
							Sleep(500);
							xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
							wprintf(L"< utilisez les fleches directionnelles pour selectionner un cheval >");
						}
						else {// il le sort
							CHEVAL cheval = (joueurs[tourde].chevaux[joueurs[tourde].selection]);
							joueurs[tourde].chevaux[joueurs[tourde].selection].sorti = 1;
							for (short i = 0; i < (nbjoueur + nbjauto); i++) {
								for (short j = 0; j < 4; j++) {//il ecrase un autre cheval(peut etre un des siens)
									if ((joueurs[i].chevaux[j].sorti == 1) && (joueurs[i].chevaux[j].pos == joueurs[tourde].depart)) {
										couleurbg(cases, joueurs[i].couleur, joueurs[i].chevaux[j], console);
										xypos(joueurs[i].chevaux[j].positiondepart.x, joueurs[i].chevaux[j].positiondepart.y, console);
										wprintf("$");
										joueurs[i].chevaux[j].sorti = 0;
										break;//il ne peut y avoir qu'un seul cheval par case, donc continuer la boucle est inutile
									}
								}
							}
							xypos(cheval.positiondepart.x, cheval.positiondepart.y, console);
							couleurbg(cases, -1, cheval, console);
							wprintf(" ");
							joueurs[tourde].chevaux[joueurs[tourde].selection].pos = joueurs[tourde].depart;
							couleurbg(cases, joueurs[tourde].couleur, cheval, console);
							xypos(cases[joueurs[tourde].chevaux[joueurs[tourde].selection].pos].x, cases[joueurs[tourde].chevaux[joueurs[tourde].selection].pos].y, console);
							wprintf("$");
							SetConsoleTextAttribute(console, 7);
							xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
							wprintf(L"                                                                    ");
							rejouer(&menupos, tourde, joueurs, console);
						}

					}
					else {//le cheval est sorti
						CHEVAL cheval = (joueurs[tourde].chevaux[joueurs[tourde].selection]);
						if (cheval.sorti == 1) {//dans la peripherie
							if (joueurs[tourde].chevaux[joueurs[tourde].selection].pos == joueurs[tourde].fin) {
								//le cheval est desormais dans la 1ere case de l'escalier
								if (joueurs[tourde].lance == 1) {
									SetConsoleTextAttribute(console, 7);
									xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
									wprintf(L"                                                                    ");
									couleurbg(cases, -1, joueurs[tourde].chevaux[joueurs[tourde].selection], console);
									xypos(cases[joueurs[tourde].chevaux[joueurs[tourde].selection].pos].x, cases[joueurs[tourde].chevaux[joueurs[tourde].selection].pos].y, console);
									wprintf(L"□");
									joueurs[tourde].chevaux[joueurs[tourde].selection].escalier = 0;
									joueurs[tourde].chevaux[joueurs[tourde].selection].sorti = 2;
									couleurbg(cases, joueurs[tourde].couleur, joueurs[tourde].chevaux[joueurs[tourde].selection], console);
									xypos(joueurs[tourde].escalier[(joueurs[tourde].chevaux[joueurs[tourde].selection].escalier)].x , joueurs[tourde].escalier[(joueurs[tourde].chevaux[joueurs[tourde].selection].escalier)].y, console);
									wprintf(L"$");
									SetConsoleTextAttribute(console, 7);
									suivant(&menupos, &tourde, joueurs, (nbjoueur + nbjauto), console);
								}
								else {
									SetConsoleTextAttribute(console, 7);
									xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
									wprintf(L"<               VOUS NE POUVEZ PAS DEPLACER CE CHEVAL              >");
									Sleep(500);
									xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
									wprintf(L"< utilisez les fleches directionnelles pour selectionner un cheval >");
								}
							}
							else {//pas encore à l'escalier
								SetConsoleTextAttribute(console, 7);
								xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
								wprintf(L"                                                                    ");
								short fin = 0;//mettre a 1 pour dire qu'on va à reculons
								short tmp = joueurs[tourde].chevaux[joueurs[tourde].selection].pos;
								for (short k = 1; k <= (joueurs[tourde].lance); k++) {
									if (tmp == joueurs[tourde].fin) fin = 1;
									if (fin == 0) {
										if (joueurs[tourde].chevaux[joueurs[tourde].selection].pos == 55) tmp = 0;
										else tmp++;
									}
									else tmp--;
									Sleep(500);
									ecrasecheval(tmp, (nbjoueur + nbjauto), tourde, joueurs, cases, console);
									deplacecheval(&(joueurs[tourde].chevaux[joueurs[tourde].selection]), joueurs[tourde].couleur, tmp, cases, console);
								}
								SetConsoleTextAttribute(console, 7);
								if (joueurs[tourde].lance == 6) rejouer(&menupos, tourde, joueurs, console);
								else suivant(&menupos, &tourde, joueurs, (nbjoueur + nbjauto), console);
							}

						}
						else {//le cheval est à l'escalier
							if (joueurs[tourde].chevaux[joueurs[tourde].selection].escalier != 5) {//autre que derniere marche
								if (joueurs[tourde].lance == (joueurs[tourde].chevaux[joueurs[tourde].selection].escalier + 2)) {
									SetConsoleTextAttribute(console, 7);
									xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
									wprintf(L"                                                                    ");
									couleurbg(cases, -1, joueurs[tourde].chevaux[joueurs[tourde].selection], console);
									xypos(joueurs[tourde].escalier[(joueurs[tourde].chevaux[joueurs[tourde].selection].escalier)].x, joueurs[tourde].escalier[(joueurs[tourde].chevaux[joueurs[tourde].selection].escalier)].y, console);
									wprintf(L"%d", joueurs[tourde].chevaux[joueurs[tourde].selection].escalier);
									joueurs[tourde].chevaux[joueurs[tourde].selection].escalier += 1;
									couleurbg(cases, joueurs[tourde].couleur, joueurs[tourde].chevaux[joueurs[tourde].selection], console);
									xypos(joueurs[tourde].escalier[(joueurs[tourde].chevaux[joueurs[tourde].selection].escalier)].x, joueurs[tourde].escalier[(joueurs[tourde].chevaux[joueurs[tourde].selection].escalier)].y, console);
									wprintf(L"$");
									SetConsoleTextAttribute(console, 7);
									if (joueurs[tourde].lance == 6) rejouer(&menupos, tourde, joueurs, console);
									else suivant(&menupos, &tourde, joueurs, (nbjoueur + nbjauto), console);
								}
								else {//ne peut pas le deplacer
									SetConsoleTextAttribute(console, 7);
									xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
									wprintf(L"<               VOUS NE POUVEZ PAS DEPLACER CE CHEVAL              >");
									Sleep(500);
									xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
									wprintf(L"< utilisez les fleches directionnelles pour selectionner un cheval >");
								}
							}
							else {//derniere marche
								if (joueurs[tourde].lance == 6) {
									joueurs[tourde].chevaux[joueurs[tourde].selection].escalier += 1;
									//deplacer le pion au centre
									couleurbg(cases, -1, joueurs[tourde].chevaux[joueurs[tourde].selection], console);
									xypos(joueurs[tourde].escalier[joueurs[tourde].chevaux[joueurs[tourde].selection].escalier].x, joueurs[tourde].escalier[joueurs[tourde].chevaux[joueurs[tourde].selection].escalier].y, console);
									wprintf(L"6");
									couleurbg(cases, joueurs[tourde].couleur, joueurs[tourde].chevaux[joueurs[tourde].selection], console);
									xypos((termlarge(console) / 2) + 1, (termhaut(console) / 2) - 9, console);
									wprintf(L"$");
									SetConsoleTextAttribute(console, 7);
									xypos((termlarge(console) / 2) + 30, (termhaut(console) / 2) - 16, console);
									wprintf(L"GAGNANT: %S", joueurs[tourde].nom);
									for (short i = 0; i < (nbjoueur + nbjauto); i++) {
										free(joueurs[i].chevaux);
										free(joueurs[i].nom);
										free(joueurs[i].escalier);
									}
									free(joueurs);
									free(cases);
									menupos = 1003;
									menufin(menupos, console);
								}
								else {//ne peut pas le deplacer
									SetConsoleTextAttribute(console, 7);
									xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
									wprintf(L"<               VOUS NE POUVEZ PAS DEPLACER CE CHEVAL              >");
									Sleep(500);
									xypos((termlarge(console) / 2) - 32, (termhaut(console) / 2) + 1, console);
									wprintf(L"< utilisez les fleches directionnelles pour selectionner un cheval >");
								}
							}
						}

					}
				}
				break;

			case 75: //fleche gauche
				if (menupos==21) {
					if (joueurs[tourde].selection == 0) {
						refreshcheval(cases, joueurs[tourde].chevaux[joueurs[tourde].selection], joueurs[tourde], joueurs[tourde].couleur, console);
						wprintf(L"$");
						blink = 0;
						joueurs[tourde].selection = 3;
						Sleep(50);//pour eviter double click accidentel
					}
					else {
						refreshcheval(cases, joueurs[tourde].chevaux[joueurs[tourde].selection], joueurs[tourde], joueurs[tourde].couleur, console);
						wprintf(L"$");
						blink = 0;
						joueurs[tourde].selection -= 1;
						Sleep(50);
					}
				}
				break;
			case 77: //fleche droite
				if (menupos == 21) {
					if (joueurs[tourde].selection == 3) {
						refreshcheval(cases, joueurs[tourde].chevaux[joueurs[tourde].selection], joueurs[tourde], joueurs[tourde].couleur, console);
						wprintf(L"$");
						blink = 0;
						joueurs[tourde].selection = 0;
						Sleep(50);
					}
					else {
						refreshcheval(cases, joueurs[tourde].chevaux[joueurs[tourde].selection], joueurs[tourde], joueurs[tourde].couleur, console);
						wprintf(L"$");
						blink = 0;
						joueurs[tourde].selection += 1;
						Sleep(50);
					}
				}
				break;

			}// fin du switch
			if (menupos == 21) {
				blink++;
				if (blink==40) {
					refreshcheval(cases, joueurs[tourde].chevaux[joueurs[tourde].selection], joueurs[tourde], joueurs[tourde].couleur, console);
					wprintf(L" ");
				}
				else if (blink>79) {
					refreshcheval(cases, joueurs[tourde].chevaux[joueurs[tourde].selection], joueurs[tourde], joueurs[tourde].couleur, console);
					wprintf(L"$");
					blink = 0;
				}

			}	
	}//fin elseif
	else if (menupos > 1000) {
		switch (keypressed) {
		case 72: //fleche haut
			if (menupos < 1003) {
				++menupos;
				menufin(menupos, console);
			}
			break;

		case 80: //fleche bas
			if (menupos > 1001) {
				--menupos;
				menufin(menupos, console);
			}
			break;

		case '\r': //entree
		case ' ': //espace
			switch (menupos) {
			case 1001: //quitter
				exit(0);
			case 1002: //charger sauvegarde
				break;
			case 1003: //nouvelle partie
				system("cls");
				nbjoueur = 4; //nombre de joueurs par défaut
				nbjauto = 0; //nombre de joueurs autonomes par défaut
				tourde = 0;
				blink = 0;//pour faire clignoter la selection
				menupos = 3; //position sur le menu par défaut (3 = Nouvelle Partie)
				char** nomsjoueurs;
				nomsjoueurs = malloc(sizeof(char*) * 4);
				for (short i = 0; i < 4; i++) {
					nomsjoueurs[i] = malloc(16 * sizeof(char)); //sizeof(char) = 1 
					snprintf(nomsjoueurs[i], 16 * sizeof(char), "JOUEUR %d", i + 1);
				}
				goto recommencer;
			default:
				break;
			}
			break;

		default:
			break;
		}
	}
	}
}