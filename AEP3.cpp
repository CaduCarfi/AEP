#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define XOR_KEY 0x5A  			// CHAVE XOR PRA CRIPTOGRAFIA

void linhaCol(int lin, int col);
void box(int lin1, int col1, int lin2, int col2);
int menu(int lin1, int col1, int qtd, char lista[5][40]);
void textColor(int letras, int fundo);
void menuLogin();
void addUser(char username[15], char password[15]);
void listUsers();
bool validSenha(char password[15]);
void ExcluUsers();
bool validUsuario(char username[15], char password[15]);
void criptoSENHA(char senha[15], char senhaCriptografada[15]);
void criptoUSUARIO(char usuario[15], char usuarioCriptografado[15]);
void salvarUsuariosEmArquivo();
void carregarUsuariosDeArquivo();
// COR DA LETRA
enum {
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY,
    DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
    LIGHTMAGENTA, YELLOW, WHITE
};

// COR DO FUNDO
enum {
    _BLACK = 0, _BLUE = 16, _GREEN = 32, _CYAN = 48,
    _RED = 64, _MAGENTA = 80, _BROWN = 96, _LIGHTGRAY = 112,
    _DARKGRAY = 128, _LIGHTBLUE = 144, _LIGHTGREEN = 160,
    _LIGHTCYAN = 176, _LIGHTRED = 192, _LIGHTMAGENTA = 208,
    _YELLOW = 224, _WHITE = 240
};

char usernames[100][15];
char passwords[100][15];
int userCount = 0;
int tp;

void clearScreen() {
    system("cls");
    textColor(WHITE, _BLACK); // Redefinindo as cores ap�s limpar a tela
}

void ordemAlfa();
void alterarUsuario();
	
int main() {
    int opc;
    
    carregarUsuariosDeArquivo();
    ordemAlfa();
    
    char lista[5][40] = { " Incluir ", " Alterar ", " Excluir ", " Listar  ", " Fim     " };

    setlocale(LC_ALL, "Portuguese");
    while (true) {
        clearScreen();
        opc = menu(10, 10, 5, lista);
        if (opc == 5 || opc == 0) {
            break;
        } else if (opc == 1) {
            menuLogin();
            salvarUsuariosEmArquivo();
        } else if (opc == 2){
        	alterarUsuario();	
		} else if (opc == 4) {
            listUsers();
        } else if (opc == 3) {
            ExcluUsers();
        }
        linhaCol(1, 1);
        textColor(WHITE, _BLACK);
        printf("\nEscolheu a op��o %d", opc);
    }
    textColor(WHITE, _BLACK);
    linhaCol(24, 1);
    printf("");

    return 0;
}

//---------------------------------------------------------------------------------------------
void textColor(int letra, int fundo) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), letra + fundo);
}
//---------------------------------------------------------------------------------------------
void linhaCol(int lin, int col) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){ col - 1, lin - 1 });
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
//---------------------------------------------------------------------------------------------
void box(int lin1, int col1, int lin2, int col2) {
    for (int i = col1; i <= col2; i++) {
        linhaCol(lin1, i);
        printf("%c", 196);
        linhaCol(lin2, i);
        printf("%c", 196);
    }

    for (int i = lin1; i <= lin2; i++) {
        linhaCol(i, col1);
        printf("%c", 179);
        linhaCol(i, col2);
        printf("%c", 179);
    }

    for (int i = lin1 + 1; i < lin2; i++) {
        for (int j = col1 + 1; j < col2; j++) {
            linhaCol(i, j);
            printf(" ");
        }
    }

    linhaCol(lin1, col1);
    printf("%c", 218);
    linhaCol(lin1, col2);
    printf("%c", 191);
    linhaCol(lin2, col1);
    printf("%c", 192);
    linhaCol(lin2, col2);
    printf("%c", 217);
}
//---------------------------------------------------------------------------------------------
int menu(int lin1, int col1, int qtd, char lista[5][40]) {
    int opc = 1, lin2, col2, linha, i, tamMaxItem, tecla = 0;

    tamMaxItem = strlen(lista[0]);
    for (i = 1; i < qtd; i++) {
        if (strlen(lista[i]) > tamMaxItem) {
            tamMaxItem = strlen(lista[i]);
        }
    }
    lin2 = lin1 + (qtd * 2 + 2);
    col2 = col1 + tamMaxItem + 4;

    textColor(WHITE, _BLUE);
    setlocale(LC_ALL, "C");
    box(lin1, col1, lin2, col2);
    setlocale(LC_ALL, "");
    while (tecla != 27 && tecla != 13) {
        linha = lin1 + 2;
        for (i = 0; i < qtd; i++) {
            if (i + 1 == opc) textColor(BLACK, _LIGHTGREEN);
            else textColor(WHITE, _BLUE);
            linhaCol(linha, col1 + 2);
            printf("%s", lista[i]);
            linha += 2;
        }

        linhaCol(1, 1);
        tecla = getch();
        linhaCol(22, 1);

        if (tecla == 224 || tecla == 0) {
            tecla = getch();
        }

        switch (tecla) {
            case 72: opc--; break; // Cima
            case 80: opc++; break; // Baixo
        }

        if (opc == 0) opc = 5; // Ciclo para cima
        if (opc == qtd + 1) opc = 1; // Ciclo para baixo
        if (tecla == 27) opc = 5;
    }
    return opc;
}

//---------------------------------------------------------------------------------------------
void addUser(char username[15], char password[15]) {
    if (userCount < 100) {
        strcpy(usernames[userCount], username);
        strcpy(passwords[userCount], password);
        userCount++;
    } else {
        printf("Limite de usu�rios alcan�ado.\n");
    }
}
//---------------------------------------------------------------------------------------------
void listUsers() {
    char mod;
    linhaCol(12, 10);
    setlocale(LC_ALL, "C");
    box(2, 45, 26, 100); 
    setlocale(LC_ALL, "");
    textColor(WHITE, _BLUE);
    linhaCol(2, 48);
    printf("Usuarios e senhas\n");

    // Mostrar op��es de visualiza��o
    linhaCol(20, 48);
    printf("Pressione 1 para ver senhas descriptografadas\n"); 
    linhaCol(22, 56);
	printf("Pressione 2 para criptografadas\n");
	linhaCol(25, 47);
	printf("ESC para voltar\n");

    // Loop para capturar a entrada do usu�rio
    while (true) {
        mod = getch();
        if (mod == 49) {
            // Mostrar senhas descriptografadas
            for (int i = 0; i < userCount; i++) {
                linhaCol(3 + i, 48);
                printf("Usuario: %s  |  Senha: %s\n", usernames[i],passwords[i]);
            }
            break;
        } else if (mod == 50) {
            // Mostrar senhas criptografadas
            for (int i = 0; i < userCount; i++) {
            	char senhaCriptografada[100][15];
            	char userCriptografada[100][15];
            	criptoUSUARIO(usernames[i],userCriptografada[i]);
                criptoSENHA(passwords[i], senhaCriptografada[i]);
                linhaCol(3 + i, 50);
                printf("Usu�rio: %s  |  Senha: %s\n",userCriptografada[i], senhaCriptografada[i]);
            }
            break;
        } else if (mod == 27) {
            // ESC para voltar
            break; 
        }
    }

    linhaCol(25, 47);
    printf("Pressione ESC para voltar");
    while (getch() != 27); // Espera at� o usu�rio pressionar ESC
}
//---------------------------------------------------------------------------------------------
void menuLogin() {
    char username[15];
    char password[15];

    linhaCol(12, 10);
    setlocale(LC_ALL, "C");
    box(12, 60, 16, 90);
    setlocale(LC_ALL, "");
    textColor(WHITE, _BLUE);
    linhaCol(13, 62);
    printf("Username: ");

    linhaCol(13, 72);
    scanf("%s", username);

    bool valid = false;
    while (!valid) {
        linhaCol(14, 60);
        textColor(WHITE, _BLUE);
        linhaCol(15, 62);
        linhaCol(14, 60);
        for (int i = 0; i < 30; i++) printf(""); // Limpa a linha
        linhaCol(14, 62);
        printf("Password: ");

        int i = 0;
        char ch;
        while ((ch = getch()) != '\r' && i < 29) { // L� at� Enter
            if (ch == 8 && i > 0) { // Backspace
                i--;
                printf("\b \b"); // Limpa o �ltimo caractere na tela
            } else {
                password[i++] = ch;
                printf("*"); // Mostra asterisco em vez da senha
            }
        }
        password[i] = '\0'; // Termina a string

        valid = validSenha(password);
        if (!valid) {
            textColor(WHITE, _RED);
            linhaCol(15, 62);
            printf("Senha inv�lida! ");
            Sleep(2000);
            linhaCol(15, 62);
            printf("A senha deve ter n�mero!");
            Sleep(2000);
            linhaCol(15, 62);
            printf("letra mai�scula!        ");
            Sleep(2000);
            linhaCol(15, 62);
            printf("letra min�scula!        ");
            Sleep(2000);
            linhaCol(15, 62);
            printf("caracter especial!      ");
            Sleep(2000);
            linhaCol(15, 62);
			textColor(WHITE, _BLUE);
        	linhaCol(15, 62);
        	for (int j = 0; j < 28; j++) printf(" ");
        	linhaCol(14, 62);
            for (int j = 0; j < 28; j++) printf(" "); // Limpa a linha da mensagem
        }
    }

    addUser(username, password);
    textColor(WHITE, _GREEN);
    linhaCol(15, 62);
    printf("Login feito com sucesso!\n");
    Sleep(2000);
}
//---------------------------------------------------------------------------------------------
bool validSenha(char password[15]) {
    int pontoM = 0, pontom = 0, pontoN = 0, pontoC = 0;
    int tam = strlen(password);

    for (int cont = 0; cont < tam; cont++) {
        char copia = password[cont];
        if (isupper(copia)) pontoM++;
        if (islower(copia)) pontom++;
        if (isdigit(copia)) pontoN++;
        if (!isalnum(copia)) pontoC++;
    }

    return (pontoM > 0 && pontom > 0 && pontoN > 0 && pontoC > 0);
}
//---------------------------------------------------------------------------------------------
void ExcluUsers() {
    char username[15];
    char password[15];

    linhaCol(12, 10);
    setlocale(LC_ALL, "C");
    box(12, 70, 20, 100);
    setlocale(LC_ALL, "");
    textColor(WHITE, _BLUE);
    linhaCol(13, 72);
    printf(" Deseja Excluir um Usu�rio ");
    textColor(WHITE, _BLUE);
    linhaCol(15, 72);
    printf("Username: ");

    linhaCol(15, 82);
    scanf("%s", username);

    bool valid = false;
    while (!valid) {
        linhaCol(16, 72);
        printf("Password: ");

        int i = 0;
        char ch;
        while ((ch = getch()) != '\r' && i < 29) { // L� at� Enter        
            if (ch == 8 && i > 0) { // Backspace
                i--;
                printf("\b \b"); // Limpa o �ltimo caractere na tela
            } else {
                password[i++] = ch;
                printf("*"); // Mostra asterisco em vez da senha
            }
        }
        password[i] = '\0'; // Termina a string

        valid = validUsuario(username, password);
        if (!valid) {
            textColor(WHITE, _RED);
            linhaCol(17, 72);
            printf("Usu�rio ou SENHA "); 
            textColor(WHITE, _RED);
            linhaCol(18, 72);
            printf("N�o identificado "); 
            Sleep(2000);
            linhaCol(17, 72);
            textColor(WHITE, _BLUE);
            linhaCol(16, 72);
			for (int j = 0; j < 28; j++) printf(" ");
            textColor(WHITE, _BLUE);
            linhaCol(17, 72);
            for (int j = 0; j < 28; j++) printf(" "); // Limpa a linha da mensagem
            textColor(WHITE, _BLUE);
            linhaCol(18, 72);
            for (int j = 0; j < 28; j++) printf(" "); // Limpa a linha da mensagem
        }
    }

    for (int i = 0; i < userCount; i++) {
        if (strcmp(usernames[i], username) == 0) {
            // Move os usu�rios para cima
            for (int j = i; j < userCount - 1; j++) {
                strcpy(usernames[j], usernames[j + 1]);
                strcpy(passwords[j], passwords[j + 1]);
            }
            userCount--; // Diminui o contador de usu�rios
            break;
        }
    }

    textColor(WHITE, _GREEN);
    linhaCol(17, 72);
    printf("USU�RIO EXCLUIDO!\n");
    Sleep(2000); // Pausa para que o usu�rio veja a mensagem
    ordemAlfa();
    salvarUsuariosEmArquivo();
}
//---------------------------------------------------------------------------------------------
bool validUsuario(char username[15], char password[15]) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(usernames[i], username) == 0 && strcmp(passwords[i], password) == 0) {
            return true; // Usu�rio e senha v�lidos
        }
    }
    return false; // Usu�rio ou senha inv�lidos
}
//---------------------------------------------------------------------------------------------
void criptoSENHA(char senha[15],char senhaCriptografada[15]) {
	
    int tamanho = strlen(senha);
    for (int i = 0; i < tamanho; i++) {
        senhaCriptografada[i] = senha[i] ^ XOR_KEY; // Criptografia simples
    }
    senhaCriptografada[tamanho] = '\0'; // Termina a string
}
//---------------------------------------------------------------------------------------------
void criptoUSUARIO(char usuario[15],char usuarioCriptografado[15]) {
	
    int tamanho = strlen(usuario);
    for (int i = 0; i < tamanho; i++) {
        usuarioCriptografado[i] = usuario[i] ^ XOR_KEY; // Criptografia simples
    }
    usuarioCriptografado[tamanho] = '\0'; // Termina a string
}
//---------------------------------------------------------------------------------------------
void ordemAlfa() {
    char tmp1[15], tmp2[15];
    int x, y;

    for (x = 0; x < userCount - 1; x++) {
        for (y = x + 1; y < userCount; y++) {            
            if (strcmp(usernames[y], usernames[x]) < 0) {                
                strcpy(tmp1, usernames[x]);
                strcpy(tmp2, passwords[x]);
                strcpy(usernames[x], usernames[y]);
                strcpy(passwords[x], passwords[y]);
                strcpy(usernames[y], tmp1);
                strcpy(passwords[y], tmp2);
            }
        }
    }
}
//---------------------------------------------------------------------------------------------
void salvarUsuariosEmArquivo() {
    FILE *file = fopen("AEP LISTA.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir arquivo para salvar usu�rios!\n");
        return;
    }
	
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s\n", usernames[i], passwords[i]);
    }
	ordemAlfa();
    fclose(file);
    printf("Usu�rios salvos com sucesso no arquivo!\n");
}
//---------------------------------------------------------------------------------------------
void carregarUsuariosDeArquivo() {
    FILE *file = fopen("AEP LISTA.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir arquivo para carregar usu�rios!\n");
        return;
    }
	ordemAlfa();
    while (fscanf(file, "%s %s", usernames[userCount], passwords[userCount]) != EOF) {
        userCount++;
    }

    fclose(file);
    printf("Usu�rios carregados com sucesso do arquivo!\n");
}
//---------------------------------------------------------------------------------------------
void alterarUsuario() {
    char username[15];
    char password[15];
    char novoUsername[15];
    char novaSenha[15];

    linhaCol(12, 10);
    setlocale(LC_ALL, "C");
    box(10, 60, 25, 120);
    setlocale(LC_ALL, "");
    textColor(WHITE, _BLUE);
    linhaCol(10, 62);
    printf(" Alterar Usu�rio ");
    textColor(WHITE, _BLUE);
    linhaCol(12, 62);
    printf("Username atual: ");
    linhaCol(12, 78);
    scanf("%s", username);

    bool valid = false;
    while (!valid) {
        linhaCol(14, 62);
        printf("Senha atual: ");
        
        int i = 0;
        char ch;
        while ((ch = getch()) != '\r' && i < 29) {         
            if (ch == 8 && i > 0) { 
                i--;
                printf("\b \b"); 
            } else {
                password[i++] = ch;
                printf("*"); 
            }
        }
        password[i] = '\0'; 

        // Valida o usu�rio e senha
        valid = validUsuario(username, password);
        if (!valid) {
            textColor(WHITE, _RED);
            linhaCol(16, 62);
            printf("Usu�rio ou senha incorretos! ");
            Sleep(2000);
            textColor(WHITE, _BLUE);
            linhaCol(16, 63);
            for (int j = 0; j < 28; j++) printf(" "); // Limpa a linha da mensagem
            textColor(WHITE, _BLUE);
            linhaCol(16, 62);
            for (int j = 0; j < 28; j++) printf(" ");
            textColor(WHITE, _BLUE);
            linhaCol(14, 62);
            for (int j = 0; j < 28; j++) printf(" ");
        }
    }

    linhaCol(18, 75);
    textColor(WHITE, _BLUE);                      //  VALIDAR A SENHA DE MANEIRA CORRETA
    printf("Novo Username: ");
    linhaCol(18, 90);
    scanf("%s", novoUsername);
    
	bool valida = false;
    while (!valida) {
        linhaCol(14, 60);
        textColor(WHITE, _BLUE);
        linhaCol(15, 62);
        linhaCol(14, 60);
        for (int i = 0; i < 30; i++) printf(""); // Limpa a linha
        linhaCol(19, 75);
        printf("Nova Senha: ");

        int i = 0;
        char ch;
        while ((ch = getch()) != '\r' && i < 29) { // L� at� Enter
            if (ch == 8 && i > 0) { // Backspace
                i--;
                printf("\b \b"); // Limpa o �ltimo caractere na tela
            } else {
                novaSenha[i++] = ch;
                printf("*"); // Mostra asterisco em vez da senha
            }
        }
        novaSenha[i] = '\0'; // Termina a string

        valida = validSenha(novaSenha);
        if (!valida) {
            textColor(WHITE, _RED);
            linhaCol(20, 75);
            printf("Senha inv�lida! ");
            Sleep(2000);
            linhaCol(20, 75);
            printf("A senha deve ter n�mero!");
            Sleep(2000);
            linhaCol(20, 75);
            printf("letra mai�scula!        ");
            Sleep(2000);
            linhaCol(20, 75);
            printf("letra min�scula!        ");
            Sleep(2000);
            linhaCol(20, 75);
            printf("caracter especial!      ");
            Sleep(2000);
            linhaCol(20, 75);
			textColor(WHITE, _BLUE);
        	linhaCol(20, 75);
        	for (int j = 0; j < 28; j++) printf(" ");
        	linhaCol(19, 75);
            for (int j = 0; j < 28; j++) printf(" "); // Limpa a linha da mensagem
        }
    }

    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(usernames[i], username) == 0 && strcmp(passwords[i], password) == 0) {
            
            if (strlen(novoUsername) > 0) {
                strcpy(usernames[i], novoUsername);
            }
            
            if (strlen(novaSenha) > 0) {
                strcpy(passwords[i], novaSenha);
            }
            break;
        }
    }

    textColor(WHITE, _GREEN);
    linhaCol(20, 72);
    printf("Usu�rio alterado com sucesso!");
    Sleep(2000);
    ordemAlfa();
    salvarUsuariosEmArquivo();
}
