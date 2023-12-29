#include "lab.h"

//lebre-o-l  tartaruga-1-t  lobo-2-w/u  raposa-3-r   cordeiro-4-c
//fazer uma estrutura para a mesa onde guarda as cartas que estao na mesa e a pista 
typedef struct jogada {
	char anim_esco;  //animal escolhido para jogar
	int num_cart;    //numero de cartas que vai jogar do animal
	int num_cart_d;
	char car_jog[4]; //mini deck com as cartas que vai jogar -> (depois usar troca com o deck da mesa) 
}jogada;

typedef struct player {
	char nome[20];
	char mao[7];
	char mao_aposta[3];
	int pontos;
	int type;// 1=jigador   // 2=BOT
	jogada jogada;

}player;

typedef struct mesa {
	char cartas_mesa[8];
	int vez;              //jogador que joga primeiro 
	char podio[5];
	char descarte[81];
	int soma;
	int control[7];   //lebre-o-l  tartaruga-1-t  lobo-2-w/u  raposa-3-r   cordeiro-4-c    lobo a uivar-6-u
	char primeiro[5];
	int prime_n[5];
}mesa;

typedef struct bichos {
	char animal;
	int posicao_x;
	int posicao_y;
	int cor;
}bichos;

typedef struct pista {
	int tipo_pista;// 1-pista normal    2- pista com rio    3-partida     4-chegada
	int posicao;
}pista;

void tela_inicial() {
	setColor(MY_COLOR_PINK, MY_COLOR_LIGTH_YELLOW);
	system("cls");

	showRectCenteredAt(40, 9, 25, 3);
	gotoxy(30, 10);  printf("A lebre e a tartaruga\n");
	gotoxy(35, 12); printf("by Pedro\n");
	gotoxy(10, 50); system("pause");
	system("cls");
	gotoxy(10, 10);
}

char menu() {
	char opc;
	printf("manu:\n");
	gotoxy(10, 11); printf("a-comecar novo jogo");
	gotoxy(10, 12); printf("b-carregar ultimo jogo");
	gotoxy(10, 13); printf("c-ver regras");
	gotoxy(10, 14); printf("d-exit");
	do {
		printf("Escolha uma opcao: ");
		scanf_s("%c", &opc, 1);
		opc = tolower(opc);
		if (opc != 'a' && opc != 'b' && opc != 'c' && opc != 'd') {
			printf("opcao invalida");
			fflush(stdin);
		}
	} while (opc != 'a' && opc != 'b' && opc != 'c' && opc != 'd');
	system("cls");
	return opc;
}

//----------------------------------------------------------------------------------menu

void criar_bichos(bichos bichos[], int n) {
	int contador = 10;
	char animais[6] = { "ltwrc" };
	for (int i = 0; i < n; i++) {
		bichos[i].animal = animais[i];
		bichos[i].posicao_x = 10;
		bichos[i].posicao_y =contador;
		contador = contador +3;
	}
}

void mostrar_bichos(bichos bicho[], int n) {
	int i = 0;
	for (int i = 0; i < n; i++) {
		if (i == 0)
			setForeColor(14);
		else if (i == 1)
			setForeColor(10);
		else if (i == 2)
			setForeColor(8);
		else if (i == 4)
			setForeColor(1);
		else
			setForeColor(12);

		if(bicho[i].posicao_x>=130)
			showRectAt(131, bicho[i].posicao_y, 2, 2);
		else
			showRectAt(bicho[i].posicao_x+1, bicho[i].posicao_y, 2, 2);

	}
	setForeColor(6);


}

//------------------------------------------------------------------------------------bichos
void criar_mao_jog(player* player_1) {
	for (int i = 0; i < 4; i++) {
		player_1->jogada.car_jog[i] = 'x';
	}
}

void criar_podio(mesa* mesa) {
	for (int i = 0; i < 5; i++)
		mesa->podio[i] = 'x';
}

void criar_descarte(mesa* mesa) {
	for (int i = 0; i < 81; i++) {
		mesa->descarte[i] = 'x';
	}
}

void criar_cart_mesa(mesa* mesa) {
	for (int i = 0; i < 8; i++) {
		mesa->cartas_mesa[i] = 'x';
	}
}

void criar_baralho(char carta[], int size) {//cria baralho de 81 cartas atribuindo letras a cada carta diferente
	int cartas;
	for (cartas = 0; cartas < 81; cartas++) {
		if (cartas < 18)
			carta[cartas] = 'l';
		else if (cartas < 35)
			carta[cartas] = 't';
		else if (cartas < 48)
			carta[cartas] = 'w';
		else if (cartas < 51)
			carta[cartas] = 'u';
		else if (cartas < 66)
			carta[cartas] = 'r';
		else
			carta[cartas] = 'c';
	}
}

void criar_cartas_aposta(char aposta_1[], int size) {
	char an[] = { 'L','T','W','R','C' };
	for (int i = 0; i < size; i++)
		aposta_1[i] = an[i];
}

//-----------------------------------------------------------------------------criar baralho
void criar_mao(player* player_1) {
	int i = 0;
	for (i = 0; i < 7; i++)
		player_1->mao[i] = 'x';
}

void criar_mao_aposta(player* player_1) {
	int i = 0;
	for (i = 0; i < 3; i++)
		player_1->mao_aposta[i] = 'x';
}

//--------------------------------------------------------------------------------criar mao
void mostrar_baralho_disponivel(char carta[], int size_1, char deck_apostas_1[], int size_2, struct player player, struct player player_2, mesa mesa, bichos bicho[5]) {

	FILE* deck;
	deck = fopen("maindeck", "w");
	fprintf(deck, "baralho: \n");
	for (int cartas = 0; cartas < 81; cartas++)
		fprintf(deck, " %c", carta[cartas]);
	fprintf(deck, "\n\n");

	fprintf(deck, "deck de apostas: \n");
	for (int cartas = 0; cartas < 5; cartas++)
		fprintf(deck, " %c  ", deck_apostas_1[cartas]);
	fprintf(deck, "\n\n");

	fprintf(deck, "cartas do player: \n");
	for (int cartas = 0; cartas < 7; cartas++)
		fprintf(deck, " %c", player.mao[cartas]);
	fprintf(deck, "\n\n");

	fprintf(deck, "apostas do player: \n");
	for (int cartas = 0; cartas < 3; cartas++)
		fprintf(deck, " %c", player.mao_aposta[cartas]);
	fprintf(deck, "\n\n");

	fprintf(deck, "cartas do bot: \n");
	for (int cartas = 0; cartas < 7; cartas++)
		fprintf(deck, " %c", player_2.mao[cartas]);
	fprintf(deck, "\n\n");

	fprintf(deck, "apostas do bot: \n");
	for (int cartas = 0; cartas < 3; cartas++)
		fprintf(deck, " %c", player_2.mao_aposta[cartas]);
	fprintf(deck, "\n\n");

	fprintf(deck, "cartas que o player vai jogar: \n");
	for (int cartas = 0; cartas < 4; cartas++)
		fprintf(deck, " %c", player.jogada.car_jog[cartas]);
	fprintf(deck, "\n\n");

	fprintf(deck, "carta que o bot vai jogar:");
	for (int cartas = 0; cartas < 4; cartas++)
		fprintf(deck, " %c", player_2.jogada.car_jog[cartas]);
	fprintf(deck, "\n\n");

	fprintf(deck, "cartas na mesa: \n");
	for (int cartas = 0; cartas < 8; cartas++)
		fprintf(deck, " %c", mesa.cartas_mesa[cartas]);
	fprintf(deck, "\n\n");

	fprintf(deck, "numero de cartas na mesa: \n");
	fprintf(deck, " l_%d\n t_%d\n w_%d\n r_%d\n c_%d\n disponivel_%d \nu_%d ", mesa.control[0], mesa.control[1], mesa.control[2], mesa.control[3], mesa.control[4], mesa.control[5], mesa.control[6]);
	fprintf(deck, "\n\n");


	fprintf(deck, "cartas no descarte: \n");
	for (int cartas = 0; cartas < 81; cartas++)
		fprintf(deck, " %c", mesa.descarte[cartas]);

	fprintf(deck, "\n\n");

	fprintf(deck, "bichos: \n");
	for (int n = 0; n<5; n++)
		fprintf(deck, " %c-%d-%d\n", bicho[n].animal, bicho[n].posicao_x, bicho[n].posicao_y);

	fprintf(deck, "\n\n");

	fprintf(deck, "em primeiro: \n");
	for (int n = 0; n < 5; n++)
		fprintf(deck, " %c", mesa.primeiro[n]);

	fprintf(deck, "\n\n");

	fprintf(deck, "podio: \n");
	for (int n = 0; n < 5; n++)
		fprintf(deck, " %c", mesa.podio[n]);

	fclose(deck);
}
void mostrarRegras() {

	system("cls");

	setlocale(LC_ALL, "pt_BR.UTF-8");


	FILE* arquivo;
	char linha[100];

	arquivo = fopen("regras.txt", "r");


	if (arquivo == NULL) {
		printf("Erro ao abrir o arquivo de regras.\n");
		return;
	}

	while (fgets(linha, sizeof(linha), arquivo) != NULL)
	{
		printf("%s", linha);
	}

	fclose(arquivo);

	printf("\n");
	system("pause");
}

//----------------------------------------------------------------------------------chek
void baralhar(char baralho[], int size) {

	srand((unsigned int)time(NULL));

	for (int i = size - 1; i >= 0; i--)
		trocaChars(&baralho[i], &baralho[rand() % (i + 1)]);
}

void transferir_carta(int posicao_1, char baralho_1[], int size_1, int posicao_2, char baralho_2[], int size_2) {
	baralho_2[posicao_2] = baralho_1[posicao_1];
	baralho_1[posicao_1] = 'x';
}

void vaziar_desc(mesa* mesa, char baralho_1[],int size_1) {

	int pos_1 = 0, posi_2 = 0;

	for (int i = 0; i < size_1; i++) {

		if (baralho_1[i] == 'x') {
			baralho_1[i] = mesa->descarte[(size_1 - 1) - i];
			mesa->descarte[size_1 - i] = 'x';
		}
	}
}


void prencher_mao(char baralho_1[], int size_1, char baralho_2[], int size_2, mesa* mesa) { //para prencher a mao do jogador??
	int posicao_2 = 0;
	int posicao_1 = 0;
	do {
		if (baralho_1[posicao_1] == 'x')    //encontra a posicao que é x, para preencher com uma carta do baralho
			posicao_1++;
	} while (baralho_1[posicao_1] == 'x' && posicao_1 < size_1);
	if (baralho_1[size_1-1] == 'x') {

		vaziar_desc(mesa, baralho_1, size_1);
		do {
			if (baralho_1[posicao_1] == 'x')   
				posicao_1++;
		} while (baralho_1[posicao_1] == 'x' && posicao_1 < size_1);
	}
	while (posicao_2 < size_2) {

		if (baralho_2[posicao_2] == 'x') {
			transferir_carta(posicao_1, baralho_1, size_1, posicao_2, baralho_2, size_2);
			posicao_1++;
			if (posicao_1 == size_1) {

				vaziar_desc(mesa,baralho_1,size_1);
				posicao_1 = 0;
				do {
					if (baralho_1[posicao_1] == 'x')    
						posicao_1++;
				} while (baralho_1[posicao_1] == 'x' && posicao_1 < size_1);
			}
		}
		posicao_2++;
	}

}

void prencher_mesa_mao(char baralho_1[], int size_1, char baralho_2[], int size_2, mesa* mesa) { //para prencher a mao do jogador??
	int posicao_2 = 0;
	int posicao_1 = 0;
	do {
		if (baralho_1[posicao_1] == 'x')    //encontra a posicao que é x, para preencher com uma carta do baralho
			posicao_1++;
	} while (baralho_1[posicao_1] == 'x' && posicao_1 < size_1);

	while (posicao_2 < size_2) {

		if (baralho_2[posicao_2] == 'x') {
			transferir_carta(posicao_1, baralho_1, size_1, posicao_2, baralho_2, size_2);
			posicao_1++;
			if (posicao_1 == size_1)
				break;
		}
		posicao_2++;
	}


}

//----------------------------------------------------------------------------------operacoes com baralho
int menu_escolhe_aposta() {
	int nova_aposta = 0;
	gotoxy(55, 34); printf("escolha, usando os numeros correspondentes, uma das cartas disponiveis para a terceira aposta: ");
	scanf_s(" %d", &nova_aposta);
	do {
		if (nova_aposta < 1 || nova_aposta>7) {
			printf("opcao invalida......por favor insira um numero entre 1 e 7, incluindo estes");
			scanf_s(" %d", &nova_aposta);
		}
	} while (nova_aposta < 1 || nova_aposta>7);
	return nova_aposta;
}

void mostrar_cartas_d(struct player* player, mesa* mesa) {
	int i = 0;
	char animais[6][30] = { "lebre","tartaruga","lobo","lobo a uivar","raposa","cordeiro" };

	gotoxy(2, 34); printf("cartas disponiveis: ");
	gotoxy(30, 34); printf("cartas de aposta: ");
	for (i = 0; i < 7; i++) {
		gotoxy(2, 35 + i);
		switch (player->mao[i]) {
		case 'l':
			printf("%d- %s", i + 1, animais[0]);
			break;
		case 't':
			printf("%d- %s", i + 1, animais[1]);
			break;
		case 'w':
			printf("%d- %s", i + 1, animais[2]);
			break;
		case 'u':
			printf("%d- %s", i + 1, animais[3]);
			break;
		case 'r':
			printf("%d- %s", i + 1, animais[4]);
			break;
		case 'c':
			printf("%d- %s", i + 1, animais[5]);
			break;
		case 'x':
			break;
		}

	}
	for (i = 0; i < 3; i++) {
		gotoxy(30, 35 + i);
		switch (player->mao_aposta[i]) {
		case 'L':
			printf("%d- %s", i + 1, animais[0]);
			break;
		case 'T':
			printf("%d- %s", i + 1, animais[1]);
			break;
		case 'W':
			printf("%d- %s", i + 1, animais[2]);
			break;
		case 'U':
			printf("%d- %s", i + 1, animais[3]);
			break;
		case 'R':
			printf("%d- %s", i + 1, animais[4]);
			break;
		case 'C':
			printf("%d- %s", i + 1, animais[5]);
			break;
		case 'X':
			break;
		}
	}
	gotoxy(180, 19);
	printf("cartas na mesa: ");
	for (i = 0; i < 8; i++) {
		gotoxy(180, 20 + i);
		switch (mesa->cartas_mesa[i]) {
		case 'l':
			printf("%d- %s", i + 1, animais[0]);
			break;
		case 't':
			printf("%d- %s", i + 1, animais[1]);
			break;
		case 'w':
			printf("%d- %s", i + 1, animais[2]);
			break;
		case 'u':
			printf("%d- %s", i + 1, animais[3]);
			break;
		case 'r':
			printf("%d- %s", i + 1, animais[4]);
			break;
		case 'c':
			printf("%d- %s", i + 1, animais[5]);
			break;
		case 'x':
			break;
		}
	}
}

void escolher_aposta(player* player_1, int escolha) {
	escolha = escolha - 1;
	player_1->mao[escolha] = toupper(player_1->mao[escolha]);
	player_1->mao_aposta[2] = player_1->mao[escolha];
	player_1->mao[escolha] = 'o';
	for (int i = 0; i < 6; i++) {
		if (player_1->mao[i] == 'o')
			trocaChars(&player_1->mao[i], &player_1->mao[6]);
	}

}

//-------------------------------------------------------------------------------------interfasse cartas/jogabilidade
int aleatorio(int a, int b) { //o maior em primeiro
	int alea = rand() % a + b;
	return alea;
}

void criar_pista(struct pista pista[], int size) {
	int i = 0;
	int xx = 10;

	for (i = 0; i < 13; i++) {
		pista[i].posicao = xx;
		xx = xx + 10;
	}

	int alea_1 = 0, alea_2 = 0;
	alea_1 = aleatorio(11, 1);
	alea_2 = aleatorio(11, 1);
	do {
		if (alea_1 == alea_2)
			alea_2 = aleatorio(11, 0);
	} while (alea_1 == alea_2);

	//printf("%d %d", alea_1, alea_2);

	for (int j = 0; j < 13; j++) {
		if (j == alea_1 || j == alea_2)
			pista[j].tipo_pista = 2;
		else if (j == 0)
			pista[j].tipo_pista = 3;
		else if (j == 12)
			pista[j].tipo_pista = 4;
		else
			pista[j].tipo_pista = 1;
	}
}

void mostrar_pista(struct pista pista[], int size) {
	int i = 0;
	for (i = 0; i < size; i++) {
		if (pista[i].tipo_pista == 2) {
			setForeColor(11);
			showRectAt(pista[i].posicao, 10, 10, 15);
		}
		else if (pista[i].tipo_pista == 3 || pista[i].tipo_pista == 4) {
			setForeColor(6);
			showRectAt(pista[i].posicao, 10, 10, 15);
		}
		else {
			setForeColor(2);
			showRectAt(pista[i].posicao, 10, 10, 15);
		}
	}

}

//---------------------------------------------------------------------------------------pista

void ver_mesa(mesa* mesa) {
	for (int j = 0; j < 7; j++) 
		mesa->control[j] = 0;

	int i = 0;
	for (i = 0; i < 8; i++) {

		if (mesa->cartas_mesa[i] == 'l')
			mesa->control[0] ++ ;
		else if (mesa->cartas_mesa[i] == 't')
			mesa->control[1]++;
		else if (mesa->cartas_mesa[i] == 'w' || mesa->cartas_mesa[i] == 'u') {
			mesa->control[2]++;
			if (mesa->cartas_mesa[i] == 'u')
				mesa->control[6]++;
		}
		else if (mesa->cartas_mesa[i] == 'r')
			mesa->control[3]++;
		else if (mesa->cartas_mesa[i] == 'c')
			mesa->control[4]++;
		else if (mesa->cartas_mesa[i] != 'x')
			mesa->control[5]++;

	}
}

int contar_mesa(mesa* mesa) {
	int contador = 0;
	for (int i = 0; i < 5; i++) {
		if (mesa->control[i] == 4) {
			return 2;
			break;
		}
	}
	for (int i = 0; i < 8; i++) {
		if (mesa->cartas_mesa[i] == 'x')
			contador++;
	}
	if (contador == 0)
		return 2;


}


int analisar_cart_disp(player* player_1, mesa* mesa) {
	int i = 0;
	int contador = 0;
	int contador2 = 0;
	int cont3 = 0;
	for (i = 0; i < 8; i++) {

		if (mesa->cartas_mesa[i] == player_1->jogada.anim_esco)
			contador++;
		if (mesa->cartas_mesa[i] != 'x')
			contador2++;
	}
	int cond2 = contador2 + player_1->jogada.num_cart;
	int cond = player_1->jogada.num_cart + contador;
	if ((cond > 4)||(cond2>8)||(cont3 > 4))    //caso na mesa fiquem de 4 cartas do mesmo animal
		return -5;
	else
		return -1;
}

void turno(player* player_1, mesa* mesa) {

	mostrar_cartas_d(player_1,mesa);
	gotoxy(55, 34);
	do {
		printf("escolha o animal para jogar(l-lebre  t-tartaruga  w-lobo  r-raposa  c-cordeiro): ");
		scanf_s(" %c", &player_1->jogada.anim_esco, 1);
		player_1->jogada.anim_esco = tolower(player_1->jogada.anim_esco);

		while (player_1->jogada.anim_esco != 'l' && player_1->jogada.anim_esco != 't' && player_1->jogada.anim_esco != 'w' && player_1->jogada.anim_esco != 'r' && player_1->jogada.anim_esco != 'c' && player_1->jogada.anim_esco != 'u') {
			gotoxy(55, 34);                                                          //caso nao escolha uma das letras indicadas
			printf("\nEscolha invalida, por favor utilize as letras indicadas: ");
			scanf_s(" %c", &player_1->jogada.anim_esco, 1);
			player_1->jogada.anim_esco = tolower(player_1->jogada.anim_esco);
		}
		player_1->jogada.num_cart_d = 0;
		for (int i = 0; i < 7; i++) {
			if (player_1->mao[i] == player_1->jogada.anim_esco)
				player_1->jogada.num_cart_d++;      //se existir cartas do animal que pretende jogar adiciona 1 à variavel essa variavel vai ter que ter um numero superior ao numero de cartas que pretende jogar
		}
		if (player_1->jogada.num_cart_d == 0) {    //caso nao possua cartas deste animal
			printf("voce nao possui cartas desse animal, por favor escolha outro");
			gotoxy(55, 35);
		}
	} while (player_1->jogada.num_cart_d == 0);
	gotoxy(135, 34);
	printf("numero de cartas disponiveis: %d\n", player_1->jogada.num_cart_d);
	system("pause");

	gotoxy(55, 40);                           //quaantas cartas para jogar
	printf("quantas quartas deseja jogar?(escolha um numero entre 1 e 4, incluive): ");
	scanf_s("%d", &player_1->jogada.num_cart);
	do {
		
		if (player_1->jogada.num_cart > player_1->jogada.num_cart_d || player_1->jogada.num_cart < 1) {
			gotoxy(55, 41);
			printf("nao é possivel jogar esse numero de cartas, por favor tente novamente: ");
			scanf_s("%d", &player_1->jogada.num_cart);
		}

		int verifica = analisar_cart_disp(player_1, mesa);
		if (verifica == -5) {
			gotoxy(55, 41);
			printf("nao é possivel jogar esse numero de cartas, por favor tente novamente: ");
			scanf_s("%d", &player_1->jogada.num_cart);
		}

	} while (player_1->jogada.num_cart > player_1->jogada.num_cart_d || player_1->jogada.num_cart < 1);
	//printf("numero de cartas que vai jogar: %d\n", player_1->jogada.num_cart);
	int posicao = 0;


	//transferrir as cartas escolhidas para o mini dek da jogada e depois para a mesa 

	for (int i = 0; i < 8; i++) {
		if (posicao == player_1->jogada.num_cart)
			break;
		else if (player_1->mao[i] == player_1->jogada.anim_esco) {
			transferir_carta(i, player_1->mao, 6, posicao, player_1->jogada.car_jog, 4);
			posicao++;
		}
	}

	gotoxy(1, 1); printf("susseco");
	prencher_mesa_mao(player_1->jogada.car_jog, player_1->jogada.num_cart, mesa->cartas_mesa, 8, mesa);
}

void turno_bot(player* player_1, player* player_2, mesa* mesa, int nova_aposta) {
	player_1->jogada.anim_esco = 'x';
	int i = 0, j = 0;
	int posicao = -1;
	for (i = 0; i < 3; i++) {   //para as 3 apostas do bot ele vai ver se tem uma carta do mesmo animal
		j = 0;
		for (j = 0; j < 7; j++) {               //se tiver transfere a carta par aa mao da jogada
			if (player_1->mao[j] == tolower(player_1->mao_aposta[i])) {
				player_1->jogada.anim_esco = player_1->mao[j];
				posicao = j;
				player_1->jogada.num_cart = 1;
				transferir_carta(j, player_1->mao, 7, 1, player_1->jogada.car_jog, 4);
				break;
			}
		}
		if (posicao != -1)
			break;

	}
	if (player_1->jogada.anim_esco=='x') {
		do {                                    //caso nao tenha vai escolhar uma posicao aleatoria e vai escolher o animal dessa posicao
			posicao = rand() % 6;
		} while (posicao == nova_aposta || player_1->mao[posicao] == 'x' || player_1->mao[posicao] == player_2->jogada.anim_esco);
		player_1->jogada.anim_esco = player_1->mao[posicao];
		transferir_carta(posicao, player_1->mao, 7, 0, player_1->jogada.car_jog, 4);
		player_1->jogada.num_cart = 1;
	}
	gotoxy(10, 5);
	printf("animal escolhido pelo bot: %c       numero de cartas jogadas: 1", player_1->jogada.anim_esco);
	prencher_mesa_mao(player_1->jogada.car_jog, 3, mesa->cartas_mesa, 8, mesa);

	//manda as cartas da mao do jogador para a mesa
}

void mesa_descarte(mesa *mesa) {
	int posicao_1 = 0;
	int posicao_2 = 0;
	for (int i = 0; i < 81; i++) {
		if (mesa->descarte[i] == 'x') {//encontra a posicao que é x, para preencher com uma carta do baralho
			posicao_1 = i;
			break;
		}

	}

	for (posicao_2 = 0; posicao_2 < 8; posicao_2++) {
		if (mesa->cartas_mesa[posicao_2] != 'x') {
			mesa->descarte[posicao_1] = mesa->cartas_mesa[posicao_2];
			mesa->cartas_mesa[posicao_2] = 'x';
		}
		posicao_1++;
	}
	}



//-----------------------------------------------------------------------------------------1_jogada

void chegar_meta(bichos bichos[], int n, mesa* mesa) {

	int lugar = 0;
	for (int i = 0; i < 5; i++) {
		if (mesa->podio[i] == 'x') {
			lugar = i;
			break;
		}
	}
	if (bichos[n].posicao_x >= 130) {
		mesa->podio[lugar] = bichos[n].animal;
		for (int i = 0; i < 5; i++) {
			if ((mesa->podio[i] == bichos[n].animal) && (i != lugar)) {
				mesa->podio[lugar] = 'x';
			}
		}
	}

}


void bicho_primeiro(mesa* mesa, bichos bicho[], int n) {
	int max = 0;
	for (int i = 0; i < n; i++) {
		if (bicho[i].posicao_x > max) {
			max = bicho[i].posicao_x;
			mesa->primeiro[0] = bicho[i].animal;
		}
	}

}

void mover_bichos(mesa* mesa, bichos bichos[], int n, pista pista[],int nn) {  //lebre-o-l  tartaruga-1-t  lobo-2-w/u  raposa-3-r   cordeiro-4-c    lobo a uivar-6-u

	if (mesa->control[6] != 0) {
		if (mesa->control[2] == 1) {
			bichos[2].posicao_x = bichos[2].posicao_x + 10;
			chegar_meta(bichos, 2, mesa);
		}
		else if (mesa->control[2] > 1) {
			bichos[2].posicao_x = ((bichos[2].posicao_x / 10) + (mesa->control[2] - 1)) * 10;
			chegar_meta(bichos, 2, mesa);
		}
	}
	else {
		//lebre
		if (mesa->control[0] > 0) {
			if (mesa->primeiro[0] == 'l') {
				bichos[0].posicao_x = bichos[0].posicao_x + 40;
				chegar_meta(bichos, 0, mesa);
			}
			else {
				bichos[0].posicao_x = bichos[0].posicao_x + 20;
				chegar_meta(bichos, 0, mesa);
			}
		}
		//tartaruga
		if (mesa->control[1] == 4) {
			bichos[1].posicao_x = bichos[1].posicao_x + 20;
			chegar_meta(bichos, 1, mesa);
		}
		else {
			bichos[1].posicao_x = bichos[1].posicao_x + 10;
			chegar_meta(bichos, 1, mesa);
		}
		//lobo normal
		if (mesa->control[2] == 1)
			bichos[2].posicao_x = bichos[2].posicao_x + 10;
		else if (mesa->control[2] > 1)
			bichos[2].posicao_x = ((bichos[2].posicao_x/10) + (mesa->control[2] - 1))*10;
		//raposa
		if (mesa->control[3] > 0) {
			bichos[3].posicao_x = ((bichos[3].posicao_x / 10) + mesa->control[3]) * 10;
			chegar_meta(bichos, 3, mesa);
		}
		//cordeiro
		if (mesa->control[4] > 0) {
			int control = (bichos[4].posicao_x) / 10;
			for (int n = 1; n <= (mesa->control[4]+1); n++) {
				bichos[4].posicao_x = bichos[4].posicao_x + 10;
				if (pista[control].tipo_pista == 2)
					break;
                control++;
			}
			chegar_meta(bichos, 4, mesa);
		}


	}
}

//-------------------------------------------------------------------------------------fim de jogo

void contar_pontos(mesa* mesa, player* player) {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (mesa->podio[j] == player->mao_aposta[i]) {
				if (j == 0)
					player->pontos = player->pontos + 5;
				else if (j == 1)
					player->pontos = player->pontos + 3;
				else if (j == 2)
					player->pontos = player->pontos + 2;
			}
		}
	}

}


//-----------------------------------------------------------------------------------------movimento dos bichos
void ver_regras() {


	setlocale(LC_ALL, "pt_BR.UTF-8");

	FILE* regras;
	regras = fopen("regras.txt", "r");
	if (regras == NULL) {
		printf("Erro ao abrir o arquivo de regras.\n");
		return;
	}
	char caract = 0;

	while (!feof(regras)) {
		fscanf_s(regras, " %c", &caract, 1);
		printf("%c", caract);
	}
	fclose(regras);
}



int main() {

	srand((unsigned int)time(NULL));

	int nova_aposta = 0;
	int div = 0;
	int contar__mesa = 0;

	char carta[82]{};
	char aposta_1[6]{};
	bichos bicho[5];

	struct pista pista[13]{};
	struct player player_1 {};
	struct player bot {};
	player_1.type = 1;
	bot.type = 2;

	mesa mesa{};


	char opc = 0;

	tela_inicial();

	do {
		opc = menu();

		switch (opc) {

		case 'a':
			criar_podio(&mesa);
			criar_bichos(bicho, 5);
			criar_baralho(carta, 81);
			criar_descarte(&mesa);
			criar_cartas_aposta(aposta_1, 5);
			criar_cart_mesa(&mesa);
			criar_mao(&player_1);
			criar_mao(&bot);
			criar_mao_aposta(&player_1);
			criar_mao_aposta(&bot);
			baralhar(carta, 81);
			baralhar(aposta_1, 5);
			prencher_mao(carta, 81, player_1.mao, 7, &mesa);
			prencher_mao(aposta_1, 5, player_1.mao_aposta, 2, &mesa);
			prencher_mao(carta, 81, bot.mao, 7, &mesa);
			prencher_mao(aposta_1, 5, bot.mao_aposta, 2, &mesa);
			prencher_mao(aposta_1, 5, bot.mao_aposta, 2, &mesa);
			criar_pista(pista, 13);
			criar_mao_jog(&player_1);
			criar_mao_jog(&bot);
			//-----------------------------------------------------------------------------imiciar componentes do jogo^^^^

			setColor(3, 0);
			system("cls");
			gotoxy(10, 11); printf("bem vindo:)\n");
			gotoxy(10, 12); printf("Itroduza o nome do novo jogaror: ");
			(void)scanf_s(" %[^'\n']s", player_1.nome, 20);
			gotoxy(10, 13); printf("beem vindo %s\n", player_1.nome);

			//-----------------------------------------------------------------------------1-inserir nome de jogador
						//printf(" %c\n", carta[3]);
			system("pause");
			system("cls");

			mostrar_cartas_d(&player_1,&mesa);
			nova_aposta = menu_escolhe_aposta();
			escolher_aposta(&player_1, nova_aposta);
			escolher_aposta(&bot, rand() % 1 + 7);
			//printf(" %c", player_1.mao_aposta[2]);
			system("cls");

			mostrar_cartas_d(&player_1,&mesa);


			system("pause");

			//---------------------------------------------------------------------------------jogada
			mesa.vez = rand() % 2 + 1;
			do {
				system("cls");
				mostrar_cartas_d(&player_1, &mesa);
				mostrar_pista(pista, 13);
				mostrar_bichos(bicho, 5);
				div = mesa.vez % 2;
				if (div == 0) {
					do {
						turno(&player_1, &mesa);

						ver_mesa(&mesa);
						contar__mesa = contar_mesa(&mesa);
						system("cls");
						mostrar_cartas_d(&player_1, &mesa);
						mostrar_pista(pista, 13);
						mostrar_bichos(bicho, 5);
						mostrar_baralho_disponivel(carta, 81, aposta_1, 5, player_1, bot, mesa, bicho);
						if (contar__mesa == 2) {
							break;
						}

						turno_bot(&bot, &player_1, &mesa, nova_aposta);
						ver_mesa(&mesa);
						contar__mesa = contar_mesa(&mesa);
						mostrar_cartas_d(&player_1, &mesa);
						mostrar_pista(pista, 13);
						mostrar_bichos(bicho, 5);
						mostrar_baralho_disponivel(carta, 81, aposta_1, 5, player_1, bot, mesa, bicho);
						if (contar__mesa == 2) {
							break;
						}
					} while (contar__mesa != 2);

				}
				else {
					do {
						turno_bot(&bot, &player_1, &mesa, nova_aposta);

						ver_mesa(&mesa);
						contar__mesa = contar_mesa(&mesa);
						system("cls");
						mostrar_cartas_d(&player_1, &mesa);
						mostrar_pista(pista, 13);
						mostrar_bichos(bicho, 5);
						mostrar_baralho_disponivel(carta, 81, aposta_1, 5, player_1, bot, mesa, bicho);
						if (contar__mesa == 2) {
							break;
						}

						turno(&player_1, &mesa);

						ver_mesa(&mesa);
						contar__mesa = contar_mesa(&mesa);
						mostrar_cartas_d(&player_1, &mesa);
						mostrar_pista(pista, 13);
						mostrar_bichos(bicho, 5);
						mostrar_baralho_disponivel(carta, 81, aposta_1, 5, player_1, bot, mesa, bicho);
						if (contar__mesa == 2) {
							break;
						}
					} while (contar__mesa != 2);
				}
				ver_mesa(&mesa);

				mesa.soma = player_1.jogada.num_cart + bot.jogada.num_cart;
				mesa_descarte(&mesa);
				mover_bichos(&mesa, bicho, 5,pista,13);
				mostrar_baralho_disponivel(carta, 81, aposta_1, 5, player_1, bot, mesa, bicho);


				mesa.vez++;


				//prencher_mao(mesa.cartas_mesa, player_1.jogada.num_cart + bot.jogada.num_cart, mesa.descarte, 81, &mesa);
				prencher_mao(carta, 81, bot.mao, 7, &mesa);
				prencher_mao(carta, 81, player_1.mao, 7, &mesa);
				mostrar_pista(pista, 13);
				mostrar_bichos(bicho, 5);
			} while (mesa.podio[2] == 'x');


			contar_pontos(&mesa, &player_1);

			contar_pontos(&mesa, &bot);

			system("cls");

			gotoxy(10, 10);
			if(player_1.pontos>bot.pontos)
				printf("o jogador vencedor é: %s",player_1.nome);
			else if(player_1.pontos == bot.pontos)
				printf("houve um empate");
			else
				printf("o bot venceu");





			system("pause");

			return 0;
			break;
		case 'b':
			mostrarRegras();
			return 0;
			break;
		case 'c':
			system("cls");
			FILE* regras;
			regras = fopen("regras", "r");

			fclose(regras);
		case 'd':
			setColor(9, 0);
			system("cls");
			printf("saving...."); system("pause"); printf("\nsaving...."); system("pause");
			printf("ate a proxima");
			break;
		}
	} while (opc != 'd');
	//o lobo nao estava a ser contado no podio......
	//ao contar as cartas disponiveis tnho de contar o lobo e o obo a uivar juntos
}