#include <eliminator.h>

// Variables //
static int exit;
static int players;
static player player1;
static player player2;
static int matrix[X_SQUARES][Y_SQUARES] = {{0}};

void eliminator(){
    exit = 0;
  
    call_sys_commandEnter();
    printf("Ingrese la cantidad de jugadores (1 o 2): ");
    call_sys_commandEnter();
    char c;
    while ( (c = getChar()) != '1' && c != '2'){
    }
    int d = c - '0';
    players = d;
 

    printMessage();

    startGame();
    return;
}

void printMessage() {
    call_sys_zoomIn();
    call_sys_sleep(1000); 
    call_sys_clear();

    for (int i = 0; i < 3; i++) call_sys_commandEnter();

    call_sys_drawWithColor("                ELIMINATOR ", RED);
    printf("PRESIONAR 'V' PARA SALIR ");
    call_sys_sleep(1000); 
    call_sys_clear();
    call_sys_sleep(500);

    for (int i = 0; i < 3; i++) call_sys_commandEnter();
    
    call_sys_drawWithColor("               ELIMINATOR ", GREEN);
    printf("PRESIONAR 'V' PARA SALIR ");
    call_sys_sleep(1000);
    call_sys_clear();
    call_sys_zoomOut();
}

void drawBorders(){
    int i, j;

    // Dibuja la fila superior
    for(j = 1; j < X_SQUARES - 1; j++){
        call_sys_draw(j * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, RED);
        matrix[j][1] = 1;
    }

    // Dibuja la fila inferior
    for(j = 1; j < X_SQUARES - 1; j++){
        call_sys_draw(j * SQUARE_SIZE, (Y_SQUARES - 2) * SQUARE_SIZE, SQUARE_SIZE, RED);
        matrix[j][Y_SQUARES - 2] = 1;
    }

    // Dibuja la columna izquierda
    for(i = 1; i < Y_SQUARES - 1; i++){
        call_sys_draw(SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, RED);
        matrix[1][i] = 1;
    }

    // Dibuja la columna derecha
    for(i = 1; i < Y_SQUARES - 1; i++){
        call_sys_draw((X_SQUARES - 2) * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, RED);
        matrix[X_SQUARES - 2][i] = 1;
    }
}

void startGame(){
    
    player1.score = 0;
    player2.score = 0;

    char c;
    
    while(exit != 1) {
        startPlayer();
        displayScore(&player1, &player2);
        char flag = 1;
        c = 0;
        while(flag){
            call_sys_sleep(100);
            uint32_t size = 0;
            call_sys_read(&c, 1, &size);
            switch(c) {
                case 'v':
                    exit = 1;
                    flag = 0;
                    break;
                case 'w':
                    if (player1.dir != DOWN) {
                        player1.dir = UP;
                    }
                    break;
                case 'a':
                    if (player1.dir != RIGHT) {
                        player1.dir = LEFT;
                    }
                    break;
                case 's':
                    if (player1.dir != UP) {
                        player1.dir = DOWN;
                    }
                    break;
                case 'd':
                    if (player1.dir != LEFT) {
                        player1.dir = RIGHT;
                    }
                    break;
                case 'i':
                    if (player2.dir != DOWN && players == 2) {
                        player2.dir = UP;
                    }
                    break;
                case 'j':
                    if (player2.dir != RIGHT && players == 2) {
                        player2.dir = LEFT;
                    }
                    break;
                case 'k':
                    if (player2.dir != UP && players == 2) {
                        player2.dir = DOWN;
                    }
                    break;
                case 'l':
                    if (player2.dir != LEFT && players == 2) {
                        player2.dir = RIGHT;
                    }
                    break;
                default:
                    break;
            }
            movePlayer(&player1, &player2, &flag);
            if(flag == 1 && players != 2){
                chatGPT(&player2);
            }
            movePlayer(&player2, &player1, &flag);
        }
    }
    call_sys_clear();
}

void startPlayer(){
    call_sys_clear();
    clearMatrix();
    drawBorders();
    player1.dir = DOWN;
    player1.x = (X_SQUARES/2);
    player1.y = 6;
    player1.color = GREEN;
    
    call_sys_draw(player1.x * SQUARE_SIZE, player1.y * SQUARE_SIZE, SQUARE_SIZE, player1.color);

    player2.dir = UP;
    player2.x = (X_SQUARES/2);
    player2.y = Y_SQUARES - 6;
    player2.color = RED;
    
    call_sys_draw(player2.x * SQUARE_SIZE, player2.y * SQUARE_SIZE, SQUARE_SIZE, player2.color);

    // Inicializa las posiciones de los jugadores en la matriz
    matrix[X_SQUARES/2][6] = 1;
    matrix[X_SQUARES/2][Y_SQUARES - 6] = 1;

}


void movePlayer(player * p, player * otherPlayer, char * flag){
    if(*flag == 0){
        return;
    }
    int x = p->x;
    int y = p->y;
    int dir = p->dir;
    int next_x = x;
    int next_y = y;
    switch(dir){
        case UP:
            next_y = y - 1;
            break;
        case DOWN:
            next_y = y + 1;
            break;
        case LEFT:
            next_x = x - 1;
            break;
        case RIGHT:
            next_x = x + 1;
            break;
    }
    if(matrix[next_x][next_y] == 0){
        call_sys_draw(next_x * SQUARE_SIZE, next_y * SQUARE_SIZE, SQUARE_SIZE, p->color);
        matrix[next_x][next_y] = 1;
        p->x = next_x;
        p->y = next_y;
    }
    else{
        call_sys_sound(1000, 50);
        
        otherPlayer->score++;
        *flag = 0;
        call_sys_clear();
        matrix[next_x][next_y] = 0;
        displayScore(&player1, &player2);
        call_sys_sleep(1000);
    }
}

void displayScore(player * p1, player * p2){
    call_sys_commandEnter();
    
    printf("                                            PLAYER1: %d         PLAYER2: %d", p1->score, p2->score);
}

void clearMatrix(){
    int i, j;
    for(i = 0; i < X_SQUARES; i++){
        for(j = 0; j < Y_SQUARES; j++){
            matrix[i][j] = 0;
        }
    }
}

void chatGPT(player * p) {
    if( (p->dir == 0 || p->dir == 1) && matrix[p->x+1][p->y] == 0){
        p->dir = RIGHT;
    }
    if( matrix[p->x][(p->y) - 1] == 0 ){ // mover hacia arriba debería decrementar y
        p->dir = UP;
        return;
    }
    if( matrix[(p->x) - 1][p->y] == 0 ){ // mover hacia la izquierda debería decrementar x
        p->dir = LEFT;
        return;
    }
    if( matrix[(p->x) + 1][p->y] == 0 ){ // mover hacia la derecha debería incrementar x
        p->dir = RIGHT;
        return;
    }
    if( matrix[p->x][(p->y) + 1] == 0 ){ // mover hacia abajo debería incrementar y
        p->dir = DOWN;
        return;
    }

}

