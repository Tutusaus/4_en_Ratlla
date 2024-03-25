#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define N 8         //Aquesta constant estableix les dimensions del tauler de forma quadrada on es jugarà
#define VOID 255    //Valor a la taula d'Ascii per tal de representar l'ESPAI ( )
#define PLAYER 79   //Valor a la taula d'Ascii per tal de representar una X, corresponent al torn del jugador
#define PC 88       //Valor a la taula d'Ascii per tal de representar una O, corresponent al torn del PC (ordinador)
#define PROFUNDITAT 6
#define INFTY 10000

typedef struct node{
    struct node **fills;    //aquesta variable contindrà l'adreça de memòria de la primera adreça de memòria dels possibles N fills del node 
    int n_fills;            //nombre de fills que pot tenir el node. Normalment hauria de ser N, a no ser que una col·lumna hagi estat completada
    char tauler[N][N];      //0, 1 o 2 en funció de si està buida la casella, ha tirat el jugador o el PC respectivament
    int valor;           //resultat de la funció heurística que rep el node
    int nivell;
} Node;

//+++++++++++++++++++++++AQUESTES FUNCIONS NOMÉS SERVEIXEN PER DEBUGGERJAR+++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void printTauler(char t[N][N]){
    printf("\n");
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%c\t",t[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void dibuixarTauler(char t[N][N]);

void imprimirNode(Node *node){
    printf("Nivell: %d\n Valor: %d\n Numero de fills: %d\n Fills:\n",node->nivell,node->valor,node->n_fills);
    for(int i=0;i<node->n_fills;i++){
        printf("  %d: %x\n",i,node->fills[i]);
    }
    printf(" Tauler: \n");
    dibuixarTauler(node->tauler);
}

void recorrerArbre(Node *pare){
    for(int fill=0;fill<pare->n_fills;fill++){
        recorrerArbre(pare->fills[(pare->n_fills)-(fill+1)]);
    }
    imprimirNode(pare);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++






//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//----------------------AQUESTES FUNCIONS SON LES QUE S'UTILITZEN EN EL PROGRAMA-------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------






///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////CHECKS PER TAL DE COMPROVAR SI UNA CERTA ALINEACIÓ ÉS GUANYADORA//////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void checkS(char t[N][N],int a,int b,int *i){
    if(t[a][b]==t[a+1][b] && a<(N-1)){
        (*i)++;
        checkS(t,a+1,b,i);
    }
}

int checkCol(char t[N][N],int a,int b,int enratlla){        //enratlla 1, 2, 3 o 4 per comprovar si hi ha 1, 2, 3 o 4 en ratlla.
    if(t[a][b]==t[a+1][b]){
        int i=0;
        checkS(t,a,b,&i);
        if(i>(enratlla-2)){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}

void checkO(char t[N][N],int a,int b,int *i){
    if(t[a][b]==t[a][b-1] && b>0){
        (*i)++;
        checkO(t,a,b-1,i);
    }
}

void checkE(char t[N][N],int a,int b,int *i){
    if(t[a][b]==t[a][b+1] && b<(N-1)){
        (*i)++;
        checkE(t,a,b+1,i);
    }
}

int checkFila(char t[N][N],int a,int b,int enratlla){
    if(t[a][b]==t[a][b+1] || t[a][b]==t[a][b-1]){
        int i=0;
        checkO(t,a,b,&i);
        checkE(t,a,b,&i);
        if(i>(enratlla-2)){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}

void checkNO(char t[N][N],int a, int b,int *i){
    if(t[a][b]==t[a-1][b-1] && a>0 && b>0){
        (*i)++;
        checkNO(t,a-1,b-1,i);
    }
}

void checkSE(char t[N][N],int a, int b,int *i){
    if(t[a][b]==t[a+1][b+1] && a<(N-1) && b<(N-1)){
        (*i)++;
        checkSE(t,a+1,b+1,i);
    }
}

void checkNE(char t[N][N],int a, int b,int *j){
    if(t[a][b]==t[a-1][b+1] && a>0 && b<(N-1)){
        (*j)++;
        checkNE(t,a-1,b+1,j);
    }
}

void checkSO(char t[N][N],int a, int b,int *j){
    if(t[a][b]==t[a+1][b-1] && a<(N-1) && b>0){
        (*j)++;
        checkSO(t,a+1,b-1,j);
    }
}

int checkDiag(char t[N][N],int a,int b,int enratlla){
    if(t[a][b]==t[a+1][b+1] || t[a][b]==t[a+1][b-1] || t[a][b]==t[a-1][b+1] || t[a][b]==t[a-1][b-1]){
        int i=0,j=0;
        checkNO(t,a,b,&i);
        checkSE(t,a,b,&i);
        checkNE(t,a,b,&j);
        checkSO(t,a,b,&j);
        if(i>(enratlla-2) || j>(enratlla-2)){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////






int Enratlla4(Node *node){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(node->tauler[i][j]!='0'){
                if(checkFila(node->tauler,i,j,4) || checkCol(node->tauler,i,j,4) || checkDiag(node->tauler,i,j,4)){
                    return 1;
                }
            }
        }
    }
    return 0;
}

int Enratlla3lliures(Node *node){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(node->tauler[i][j]!='0'){
                if((node->tauler[i][j-1]==node->tauler[i][j] && node->tauler[i][j]==node->tauler[i][j+1]) && (j-1>0 && j+2<N)){
                    if(node->tauler[i][j+2]=='0' && node->tauler[i+1][j+2]!='0'){
                        if(node->tauler[i][j-2]=='0' && node->tauler[i+1][j-2]!='0'){
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* int Enratlla3lliures(Node *node){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(node->tauler[i][j]!='0'){
                if(checkFila(node->tauler,i,j,3) && (j-1>0 && j+2<N)){
                    if(node->tauler[i][j+2]=='0' && node->tauler[i+1][j+2]!='0' && i<N){
                        if(node->tauler[i][j-2]=='0' && node->tauler[i+1][j-2]!='0'){
                            return 1;
                        }
                    }
                }
                if(((node->tauler[i][j]==node->tauler[i+1][j+1] && node->tauler[i][j]==node->tauler[i-1][j-1]) || (node->tauler[i][j]==node->tauler[i+1][j-1] && node->tauler[i][j]==node->tauler[i-1][j+1])) && ((i+2<N && j+2<N) && (i-1>0 && j-1>0))){
                    if((node->tauler[i+2][j+2]=='0' && node->tauler[i-2][j-2]=='0' && (node->tauler[i+3][j+2]!='0' || i+3<(N+1)) && (node->tauler[i-1][j-2]!='0'))){
                        return 1;
                    }
                    else if(node->tauler[i+2][j-2]=='0' && node->tauler[i-2][j+2]=='0' && (node->tauler[i+3][j-2]!='0' || i+3<(N+1)) && (node->tauler[i-1][j+2]!='0')){
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
} */

/* int Enratlla3(Node *node){
    int jug=0,pc=0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(node->tauler[i][j]!='0'){
                if(checkCol(node->tauler,i,j,3) && node->tauler[i-1][j]=='0'){
                    if(node->tauler[i][j]=='2'){
                        pc++;
                    }
                    else{
                        jug++;
                    }
                }
                else if((node->tauler[i][j]==node->tauler[i][j-1] && node->tauler[i][j]==node->tauler[i][j+1])){
                    if(node->tauler[i][j-2]=='0' && node->tauler[i+1][j-2]!='0' && j-1>0 && i<N){
                        if(node->tauler[i][j+2]=='0' && node->tauler[i+1][j+2]!='0'){
                            if(node->tauler[i][j]=='2'){
                                pc=INFTY;
                            }
                            else{
                                jug=INFTY;
                            }
                        }
                        else if(node->tauler[i][j]=='2'){
                                pc++;
                            }
                            else{
                                jug++;
                            }
                    }
                }
                //check col
                //check fila
                //check diag
            }
        }
    }
} */

int Enratlla3(Node *node){ //Aquesta funció buscarà configuracions del tauler en les quals, hi ha tres peces alineades (vert, hortz o bé diag) les quals no estan delimitades per fitxes de l'adversari, ie estan lliures
    int jug=0,pc=0;
    for(int i=0;i<N;i++){       //CALDRIA MIRAR QUE LES DIRECCIONS ESTIGUESSIN BUIDES!!!! SINÓ, ELS TRES EN RATLLA NO VALEN TANT
        for(int j=0;j<N;j++){
            if(node->tauler[i][j]!='0'){
                if(checkFila(node->tauler,i,j,3)){
                    if(node->tauler[i][j]=='2'){
                        pc++;
                    }
                    else{
                        jug++;
                    }
                }
                else if(checkCol(node->tauler,i,j,3)){
                    if(node->tauler[i][j]=='2'){
                        pc++;
                    }
                    else{
                        jug++;
                    }
                }
                else if(checkDiag(node->tauler,i,j,3)){
                    if(node->tauler[i][j]=='2'){
                        pc++;
                    }
                    else{
                        jug++;
                    }
                }
            }
        }
    }
    return jug-pc;
}

/* int Enratlla2(Node *p){
    int jug=0,pc=0;
    for(int i=0;i<N;i++){       //CALDRIA MIRAR QUE LES DIRECCIONS ESTIGUESSIN BUIDES!!!! SINÓ, ELS TRES EN RATLLA NO VALEN RES
        for(int j=0;j<N;j++){
            if(p->tauler[i][j]!='0'){
                if(checkFila(p->tauler,i,j,3)){
                    if(p->tauler[i][j]=='2'){
                        pc++;
                    }
                    else{
                        jug++;
                    }
                }
                else if(checkCol(p->tauler,i,j,3)){
                    if(p->tauler[i][j]=='2'){
                        pc++;
                    }
                    else{
                        jug++;
                    }
                }
                else if(checkDiag(p->tauler,i,j,3)){
                    if(p->tauler[i][j]=='2'){
                        pc++;
                    }
                    else{
                        jug++;
                    }
                }
            }
        }
    }
    return jug-pc;
} */

int assignavalor(Node *node,int valor){
    if(node->nivell%2==0){
        return -valor;
    }
    else{
        return valor;
    }
}

int heuristica(Node *node,int pes_3ratlla/* ,int pes_2ratlla,int pes_1ratlla */){
    //Bàsicament, la funció heurística és una funció que avalua el valor d'un node fulla donada una taula qualsevol.
    //Aquest valor correspon a la resta entre el "potencial" del pc a fer 4 en ratlla menys el "potencial" del jugador a fer quatre en ratlla.
    //Si al tauler ja hi ha un quatre en ratlla, el valor d'aquell node fulla prendrà un valor de INFTY o -INFTY en funció de si el quatre en
    //ratlla és favorable a l'ordinador o al jugador respectivament. Si no existeixen quatres en ratlla hem de seguir un altre criteri per tal
    //de poder evaluar el tauler. PRIMERA IDEA: Per totes i cadascuna de les fitxes del tauler caldria avaluar una funció "pes", que depenent
    //de la fitxa avaluada, sumaria o restaria valor al node fulla. Aquesta funció "pes", miraria el "potencial" de la fitxa en concret i com
    //aquesta pot generar quatres en ratlla. Una primera idea seria mirar al voltant d'aquesta fitxa (a les 8 caselles al voltant d'aquesta) i
    //veure quantes n'hi ha de quin jugador i on estan col·locades. En funció de la col·locació d'aquestes fitxes adjacents calculariem el nombre
    //de quatres en ratlla que hipotèticament es podrien formar. SEGONA IDEA: El pes o valor d'una única pesa al tauler depèn del seu entorn,
    //i doncs aquest valor és una combinació lineal del nombre d'un en ratlla, de dos en ratlla i tres en ratlla del qual en forma part,
    //caldria també tenir en compte el nombre de caselles que hi ha lliures al seu voltant i doncs son les múltiples direccions d'atac a la
    //mateixa vegada. En aquesta valoració només entraran en consideració els un, dos i tres en ratlla que estiguin lliures per tots dos costats.
    //Els coeficients d'aquesta combinació lineal seran variables que es podran anar canviant per tal d'obtenir una màquina més o menys agresiva.
    //Diguem \alpha, \beta i \gamma.
    if(Enratlla4(node) || Enratlla3lliures(node)){
        return assignavalor(node,INFTY);
    }
    else{   //El tauler no conté cap quatre en ratlla i doncs hem de donar-li un valor en funció de la posició de les fitxes al tauler la funció
            //pes que dona un resultat a aquesta configuració vull que tingui la forma |a*3enratlla(p,pes_3enratlla)+b*2enratlla(p,pes_2enratlla)
            //+c*1enratlla(p,pes_1enratlla)|<INFTY (ATENCIÓ!!! Observem que aquesta funció de l'esquerra depèn nombre de fitxes al tauler i doncs
            //tot i que el tauler estigui ple, aquest valor no pot superar INFTY). Caldria afegir una funció que mirés si existeixen tres fitxes
            //al tauler alineades i que no estan envoltades per cap fitxa contraria, ja que en aquell cas és una situació win-win i se li hauria
            //d'assignar la major puntuació (INFTY)
        return assignavalor(node,pes_3ratlla*Enratlla3(node)/* +pes_2ratlla*Enratlla2(p)+pes_1ratlla*Enratlla1(p) */);
    }
}

void copiaTauler(char t1[N][N],char t2[N][N]){  //copia el tauler del node 1 al node 2
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            t2[i][j]=t1[i][j];
        }
    }
}

int calcularNumFills(char t[N][N]){
    int num_de_cols_plenes=0;
    for(int i=0;i<N;i++){
        if(t[0][i]!='0'){
            num_de_cols_plenes++;
        }
    }
    return N-num_de_cols_plenes;
}

int calculaColumna(char t[N][N],int fill_index){
    int index=0;
    for(int i=0;i<N;i++){
        if(t[0][i]=='0'){
            if(fill_index==index){
                return i;
            }
            index++;
        }
    }
}

int calculaFila(char t[N][N],int col){
    for(int i=0;i<N;i++){
        if(t[(N-1)-i][col]=='0'){
            return (N-1)-i;
        }
    }
}

void aplicarTirada(int nivell,char t[N][N],int fila,int columna){
    if(nivell%2==1){
        t[fila][columna]='2';   //PC
    }
    else{
        t[fila][columna]='1';   //Jugador
    }
}

Node *crearNode(Node *pare,int fill_index){ //aquesta funció té com a arguments l'adreça del node pare que la genera, l'índex d'aquest (fill número "tal" del node pare), i nivell on es troba el node (si és 2 llavors aquest node passa a ser un "node fulla")
    Node *p=malloc(sizeof(Node));
    copiaTauler(pare->tauler,p->tauler);
    p->nivell=(pare->nivell)+1;                                                    
    int col=calculaColumna(p->tauler,fill_index);
    int fila=calculaFila(p->tauler,col);
    aplicarTirada(p->nivell,p->tauler,fila,col);

    if(checkFila(p->tauler,fila,col,4) || checkCol(p->tauler,fila,col,4) || checkDiag(p->tauler,fila,col,4)){
        p->n_fills=0;
        p->fills=NULL;
        if(p->nivell%2==0){
            p->valor=-INFTY;
        }
        else{
            p->valor=INFTY;
        }
    }
    else{
        p->valor=0;
        p->n_fills=calcularNumFills(p->tauler);
        p->fills=malloc(p->n_fills * sizeof(Node *));
    }
    return p;
}

void crearNivell(Node *pare){
    for(int fill=0;fill<pare->n_fills;fill++){
        pare->fills[fill]=crearNode(pare,fill);
    }
}

void crearArbre(Node *pare,int profunditat){
    if(pare->nivell<profunditat){
        crearNivell(pare);
        for(int fill=0;fill<pare->n_fills;fill++){
            crearArbre(pare->fills[fill],profunditat);
        }
    }
    else{
        //Node fulla
        pare->n_fills=0;
        pare->fills=NULL;
        pare->valor=heuristica(pare,100/* ,50,25 */);
    }
}

int max(Node *pare){    //Funcions que retorna el valor màxim dels fills d'un node pare qualsevol
    int max=(pare->fills[0])->valor;
    for(int fill=0;fill<pare->n_fills;fill++){
        if(max<(pare->fills[fill])->valor){
            max=(pare->fills[fill])->valor;
        }
    }
    return max;
}

int min(Node *pare){    //Funcions que retorna el valor màxim dels fills d'un node pare qualsevol
    int min=(pare->fills[0])->valor;
    for(int fill=0;fill<pare->n_fills;fill++){
        if(min>(pare->fills[fill])->valor){
            min=(pare->fills[fill])->valor;
        }
    }
    return min;
}

void minimax(Node *pare){
    for(int fill=0;fill<pare->n_fills;fill++){
        if((pare->fills[fill])->n_fills!=0){
            minimax(pare->fills[fill]);
        }
    }
    if(pare->n_fills!=0){
        if(pare->nivell%2==0){
            pare->valor=max(pare);
        }
        else{
            pare->valor=min(pare);
        }
    }
}

void inicialitzacio(char t[N][N],int *ronda,int *run){
    *run=1;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            t[i][j]='0';  //Aquesta funció inicialitza el tauler "buidant" les caselles de "fitxes" (recordem que si a la posició [i][j] del tauler hi ha un VOID, això indica que en aquella posició no hi ha cap fitxa).
        }
    }

    char resposta;
    printf("Vols comencar tirant? (y/n)\n");
    scanf("%c",&resposta);
    if(resposta=='n'){
        *ronda=1;
    }
    else{
        *ronda=0;
    }
}

void dibuixarTauler(char t[N][N]){
    printf("\n");

    printf("%c%c%c",201,205,205);

    for(int i=0;i<(N-1);i++){
        printf("%c%c%c%c",205,203,205,205);
    }
    printf("%c%c\n",205,187);

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            switch(t[i][j]){
                case '1':
                    printf("%c%c\033[0;32m%c\033[0m%c",186,255,PLAYER,255);
                    break;

                case '2':
                    printf("%c%c\033[0;31m%c\033[0m%c",186,255,PC,255);
                    break;

                default:
                    printf("%c%c%c%c",186,255,VOID,255);
                    break;
            }
        }

        printf("%c\n",186);

        if(i==(N-1)){
            printf("%c%c%c",200,205,205);
            for(int i=0;i<(N-1);i++){
                printf("%c%c%c%c",205,202,205,205);
            }
            printf("%c%c\n",205,188);
        }else{
            printf("%c%c%c",204,205,205);
            for(int i=0;i<(N-1);i++){
                printf("%c%c%c%c",205,206,205,205);
            }
            printf("%c%c\n",205,185);
        }
    }

    printf("%c%c",255,255);
    for(int i=1;i<(N+1);i++){
        printf("%d%c%c%c",i,255,255,255);
    }
    printf("%c%c",255,255);
    printf("\n");
}

int nFora(int n){
    if(n<1 || n>8){
        printf("Cal escollir un nombre entre 1 i 8.\n");
        return 1;
    }
    else{
        return 0;
    }
}

int colPlena(int n,char t[N][N]){   //aquesta funció s'encarrega de comprovar si es vol tirar a una columna que ja està plena.
    if(t[0][n-1]!='0'){
        printf("Columna plena, tria'n una altre.\n");
        return 1;
    }
    else{
        return 0;
    }
}

int calculacol(Node *pare){
    for(int fill=0;fill<pare->n_fills;fill++){
        if((pare->fills[fill])->valor==pare->valor){
            for(int i=0;i<N;i++){
                for(int j=0;j<N;j++){
                    if(pare->tauler[i][j]!=(pare->fills[fill])->tauler[i][j]){
                        return j;
                    }
                }
            }
        }
    }
}

void esborrarArbre(Node *pare){
    for(int fill=0;fill<pare->n_fills;fill++){
        if((pare->fills[fill])->n_fills!=0){
            esborrarArbre(pare->fills[fill]);
        }
        else{
            free(pare->fills[fill]);
        }
    }
}

void torn(char t[N][N],int ronda,int *fila,int *columna){
    int n;

    if((ronda%2)+1==1){
        printf("\nA quina columna voleu tirar la fitxa? ");
        scanf("%d",&n);
        while(nFora(n) || colPlena(n,t)){
            scanf("%d",&n);
        }
    }
    else{
        double time_spent = 0;
        clock_t begin = clock();
        //creo un apuntador a un Node (adreça) que té una mida de 88 bytes
        Node *arrel=malloc(sizeof(Node));
        copiaTauler(t,arrel->tauler);   //el tauler es copia a la variable tauler de l'arrel
        arrel->nivell=0;
        arrel->valor=0;
        arrel->n_fills=calcularNumFills(arrel->tauler); //el nombre de fills correspon amb el nombre de caselles del tauler menys el nombre de columnes que son plenes
        arrel->fills=malloc(arrel->n_fills * sizeof(Node *));    //fills és una sola adreça. Aquesta és l'adreça que contindrà l'adreça del primer fill del node arrel, però com el nombre de fills no és 1 (habitualment), les altres adreces apunten als altres fills. En el cas que calcularNumFills=8, aleshores estaria reservant 8*8=64 bytes de memòria i m'estaria quedant amb la primera adreça d'aquestes que s'assignaria com a valor a arrel->fills
        //puc crear un arbre de dos nivells per sota
        crearArbre(arrel,PROFUNDITAT);
        minimax(arrel);
        //recorrerArbre(arrel);
        n=calculacol(arrel)+1;
        esborrarArbre(arrel);
        clock_t end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        printf("The elapsed time is %f seconds", time_spent);
    }
    if(ronda==1){
        n=4;
    }

    for(int i=0;i<N;i++){
        if(t[(N-1)-i][n-1]=='0'){
            if((ronda%2)+1==1){
                t[(N-1)-i][n-1]='1';    //JUGADOR
            }
            else{
                t[(N-1)-i][n-1]='2';    //PC
            }
            *fila=(N-1)-i;                  //0-7
            *columna=n-1;                   //0-7
            break;
        }
    }
}

void check(char t[N][N],int i,int j,int *run){
    if(checkFila(t,i,j,4) || checkCol(t,i,j,4) || checkDiag(t,i,j,4)){
        *run=0;
    }
}

int Guanyador(int ronda){
    if((ronda%2)+1==1){
        printf("\nHa guanyat el JUGADOR\n");
    }
    else{
        printf("\nHa guanyat el PC\n");
    }
}






//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------






int main(void){
    int run,ronda;
    int fila=0,columna=0;
    char tauler[N][N];
    inicialitzacio(tauler,&ronda,&run);

    while(run){
        torn(tauler,ronda,&fila,&columna);
        dibuixarTauler(tauler);
        check(tauler,fila,columna,&run);
        ronda++;
    }

    Guanyador(ronda-1);

    return 0;
}