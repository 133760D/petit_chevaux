typedef struct pos {
	short x;
	short y;
} POS;
typedef struct cheval {
	short pos;//la case sur laquel le cheval est actuellement
	POS positiondepart;
	short sorti;
	short escalier;
} CHEVAL;

typedef struct joueur {
	char* nom;
	CHEVAL* chevaux;
	short couleur; // 0=vert , 1=rouge , 2=bleu , 3=jaune
	short depart;
	short fin; //1ere case juste avant l'escalier; égale à depart-1
	short lance; //le dernier lancer du joueur
	short selection; //quel cheval le joueur va choisir de déplacer
	POS* escalier;
	short IA;
} JOUEUR;

void fullscreen(); //met la console en plein ecran
void xypos(short x, short y, HANDLE console); //positionne sur la case au coord x,y de la console
int termlarge(HANDLE console); //permet d'obtenir la largeur (nombres de colonnes) de la console par rapport à l'écran
int termhaut(HANDLE console); //permet d'obtenir la hauteur (nombres lignes) de la console par rapport à l'écran
void animstart(HANDLE console); // animation de debut
void menu(short pos, HANDLE console); //gère l'affichage du menu
void enlevescroll(HANDLE console); //enlève la scroll bar de la console
void optionjeu(short menupos, short nbjoueur, short nbjauto, char** nomsjoueurs, HANDLE console); //menu d'option lors d'une nouvelle partie
void change_nbjoueur(char op, short *nbjoueur, HANDLE console); //change le nombre de joueur op = operateur (+ ou -)
void change_nbjauto(char op, short *nbjauto, HANDLE console); // change le nombre de joueur autonome
void plateau(HANDLE console); //Le plateau
void initialisation(JOUEUR* joueurs, short nbjoueur, short nbjauto, POS* cases, HANDLE console); //initialise les valeurs pour la partie
void couleur(short couleur, HANDLE console); //pour gerer la couleur des chevaux
short lancede(); //nombre aleatoire entre 1-6
void rejouer(short *menupos, short tourde, JOUEUR* joueurs, HANDLE console); //le joueur rejoue
void suivant(short *menupos, short *tourde, JOUEUR* joueurs, short nbj, HANDLE console); //joueur suivant
void couleurbg(POS* cases, short couleur, CHEVAL cheval, HANDLE console); //gère la couleur avec le background
void ecrasecheval(short pos, short nbj, short tourde, JOUEUR* joueurs, POS* cases, HANDLE console); //écraser un cheval et le renvoyer dans son écurie
void deplacecheval(CHEVAL *cheval, short couleur, short pos, POS* cases, HANDLE console); //permet de deplacer le cheval
void menufin(short pos, HANDLE console); //menu de fin de jeu
void refreshcheval(POS* cases, CHEVAL cheval, JOUEUR joueur, short couleur, HANDLE console); //pour rafraichir le cheval