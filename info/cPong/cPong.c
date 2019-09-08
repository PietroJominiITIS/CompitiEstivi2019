/*
    ! cPong
    ? autore: 
        .nome: Pietro Jomini
        .classe: 4aRob
*/

/*
    ! LIBRARIES
    ? Info sulle librerie utilizzate
     
    -Librerie esterne:
        - SDL2: https://www.libsdl.org/index.php
        - SDL2_ttf: https://www.libsdl.org/projects/SDL_ttf/
        - json-c: https://github.com/json-c/json-c/wiki
    Per l'installazione rifarsi alla documentazione ufficiale

    Flag gcc necessarie: -lSDL2 -lSDL2_ttf -ljson-c 
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <json-c/json.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


/* 
    ! TYPEDEF
    ? definizioe di strutture e typo 

    -PI (const)
        .obj: approssimazione del pi greco, necessario per disegnare un cerchio
    
    -object (typedef)
        .obj: wrapper per facilità di lettura e scrittura

    -pflag (enum)
        .obj: flag per l'allineamento del testo
        .el:
            PCENTERED -> testo centrato rispetto alla coordinata fornita
            PBEFORE -> testo che termina nella coordinata fornita
            PAFTER -> testo che inizia nella acoordinata fornita

    -pbool (enum)
        .obj: wrapper per logica booleana, per facilità di lettura/scrittura
     
    -Player (enum)
        .obj: wrapper per classificare i giocatori in di destra e di sinistra
        .el:
            rp -> giocatore di destra
            lp -> giocatore di sinistra
            none -> nessun giocatore
    
    -SDL_Circle (struct)
        .obj: definire un cerchio in stile SDL2
        .el:
            x (double) -> posizione in x del cerchio
            y (double) -> posizione in y del cerchio
            r (size_t) -> raggio del cerchio

    -SDL_Rect_Float_Pos (struct)
        .obj: definire un rettangolo con posizioni non intere, in stile SDL2
        .el:
            x (double) -> posizione in x del rettangolo
            y (double) -> posizione in y del rettangolo
            w (size_t) -> larghezza del cerchio
            h (size_t) -> altezza del cerchio

    -PhysicsPack (struct)
        .obj: contenere i dati relativi alla fisica di gioco
        .el:
            playerSpeed (double) -> velocità dei giocatori, in pixel/frame
            ballAngle (double) -> angolo della palla, in radianti
            ballSpeed (double) -> velocità dei giocatori, in pixel/frame
            server (Player) -> giocatore che ha battuto o deve battere
            waitingForServe (pbool) -> definisce se la pallina è in attesa di essere battuta o meno
            winningScore (size_t) -> numero di punti necessari a vincere
            fps (size_t) -> fps ottimali, che il programma cerche di mantenere

    -FontPack (struct)
        .obj: contenere i dati relativi ai font ttf del gioco
        .el:
            small (TTF_Font *) -> oggetto ttf per il testo di dimensioni minori
            big (TTF_Font *) -> oggetto ttf per il testo di dimensioni maggiori
            path (char *) -> path per il .ttf
            fontsizeSmall (size_t) -> dimensione del testo più piccolo in pixel
            fontsizeBig (size_t) -> dimensione del testo più grande in pixel
            pixelToPointRatio (double) -> ratio per la conversione da pixel a point
            color (SDL_Color) -> colore del testo

    -guiPack (struct)
        .obj: contenere i dati relativi alla grafica del gioco
        .el:
            renderer (SDL_Renderer *) -> renderer SDL2
            window (SDL_Window *) -> window SDL2
            w (size_t) -> larghezza della finestra
            h (size_t) -> altezza della finestra
            pw (size_t) -> larghezza dei giocatori
            ph (size_t) -> altezza dei giocatori
            br (size_t) -> raggio della pallina
            sPadding (size_t) -> spazio tra la pallina e il giocatore nell'attesa della battuta
            bgColor (SDL_Color) -> colore dello sfondo
            font (FontPack) -> pacchetto per i font del gioco


*/

#define PI 3.14159265358979323846264338327950288419716939937510

typedef struct json_object object;

typedef enum {
    PCENTERED,
    PBEFORE,
    PAFTER
} pflag;

typedef enum {
    PFALSE = 0,
    PTRUE
} pbool;

typedef enum {
    rp,
    lp,
    none
} Player;

typedef struct {
    double x;
    double y;
    size_t r;
} SDL_Circle;

typedef struct {
    double x;
    double y;
    size_t w;
    size_t h;
} SDL_Rect_Float_Pos;

typedef struct {
    double playerSpeed;
    double ballAngle;
    double ballSpeed;
    Player server;
    pbool waitingForServe;
    size_t winningScore;
    size_t fps;
} PhysicsPack;

typedef struct {
    TTF_Font *small;
    TTF_Font *big;
    char *path;
    size_t fontsizeSmall;
    size_t fontsizeBig;
    double pixelToPointRatio;
    SDL_Color color;
} FontPack;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    size_t w;
    size_t h;
    size_t  pw;
    size_t ph;
    size_t br;
    size_t sPadding;
    SDL_Color bgColor;
    FontPack font;
} GuiPack;


/* 
    ! FUNCTION DECLARATIONS
    ? dichiarazione delle seguenti funzioni:

        -reverseAngleOrizzontal:
            .obj: specchiare un angolo sull' l'asse x
            .param: 
                a (double) -> angolo da specchiare
            .out: (double) angolo specchiato

        -reverseAngleVertical:
            .obj: specchiare un angolo sull' l'asse y
            .param: a (double) -> angolo da specchiare
            .out: (double) angolo specchiato
            .notWorking!

        -map:
            .obj: mappare un valore da un range a un altro
            .param:
                val (double) -> valore da mappare
                i1 (double) -> primo limite del range di partenza
                i2 (double) -> secondo limite del range di partenza
                o1 (double) -> primo limite del range di output
                o2 (double) -> secondo limite del range di output
            .out: (double) valore mappato

        -getMicrotime:
            .obj: ottenere il tempo corrente di time(), in microsecondi
            .param: none
            .out: (long) tempo in microsecondi

        -getOpponent:
            .obj: ottenere l'avversario del lpayer di input
            .param:
                pl (Player) -> player di cui ottenere l'avevrsario
            .out: (Player) avversario, o none

        -clearRenderer:
            .obj: pulire il rendere in input
            .param:
                renderer (SDL_Renderer *) -> renderer da pulire
                r, g, b (size_t) -> colore con ciu pulire il renderer
            .out: (int) 0

        -SDL_RendererDrawCircle
            .obj: disegnare un cerchio su un renderer SDL2, in determinate posizioni
            .origin: https://stackoverflow.com/a/47766167
            .param: 
                renderer (SDL_Renderer *) -> renderer su cui disegnare il cerchio
                ball (SDL_Circle) -> cerchio da disegnare
            .out: (int) 0

        -initPhysics
            .obj: inizializzare una struttura PhysicsPack, con input da stringa json
            .param: 
                pp (PhysicsPack *) -> pack da inizializzare
                setting (object *) -> stringa json
            .out: (int) 0

        -initGui
            .obj: inizializzare una struttura GuiPack, con input da stringa json
            .param: 
                pp (GuiPack *) -> pack da inizializzare
                setting (object *) -> stringa json
            .out: (int) 0

        -initPlayers
            .obj: inizializzare due SDL_Rect_Float_Pos come players
            .param:
                pl (SDL_Rect_Float_Pos *) -> player di sinistra
                pr (SDL_Rect_Float_Pos *) -> player di destra
                gp (GuiPack) -> pack contenente i dati grafici
            .out: (int) 0

        -initBall
            .obj: inizializzare un SDL_Circle come pallina del gioco
            .param:
                ball (SDL_Circle *) -> SDL_Circle da inizializzare
                gp (GuiPack) -> pack contenente i dati grafici
                pp (PhysicsPack) -> pack contenente i dati sulla fisica del gioco
                pl (SDL_Rect_Float_Pos) -> player di sinistra
                pr (SDL_Rect_Float_Pos) -> player di destra
            .out: (int) 0

        -renderText
            .obj: rendereizzare testo su un renderer SDL2, tramite SDL2_ttf
            .param:
                renderer (SDL_Renderer *) -> renderer su cui renderizzare il testo
                msg (char *) -> stringa contenente il testo da renderizzare
                fontColor (SDL_Color) -> colore del testo
                x (size_t) -> posizione in x del testo
                y (size_t) -> posizione in y del testo
                flagV (pflag) -> flag per l'allineamento verticale del testo
                flagO (pflag) -> flag per l'allineamento orizzontale del testo
            .out: (int) 0

        -renderPlAndBall
            .obj: renderizzare i players e la pallina su un renderer SDL2
            .param:
                renderer (SDL_Renderer *) -> renderer su cui renderizzare il testo
                pl (SDL_Rect_Float_Pos) -> player di sinistra
                pr (SDL_Rect_Float_Pos) -> player di destra
                ball (SDL_Circle) -> pallina
                color (SDL_Color) -> colore con cui renderizzare gli oggetti
            .out: (int) 0

        -updatePhysics
            .obj: aggiornare la fisica del gioco
            .param:
                pp (PhysicsPack *) -> pack da aggiornare
                gp (GuiPack) -> pack contenente le dimensioni della finestra
                pl (SDL_Rect_Float_Pos *) -> player di sinistra
                pr (SDL_Rect_Float_Pos *) -> player di destra
                ball (SDL_Circle *) -> pallina
            .out (Player) eventuale giocatore che ha segnato un punto, altrimenti none

*/

double reverseAngleOrizzontal(double a);
double reverseAngleVertical(double a);
double map(double val, double i1, double i2, double o1, double o2);
long getMicrotime();

Player getOpponent(Player pl);
int clearRenderer(SDL_Renderer *renderer, size_t r, size_t g, size_t b);
int SDL_RenderDrawCircle(SDL_Renderer *renderer, SDL_Circle ball);

int initPhysics(PhysicsPack *pp, object *setting);
int initGui(GuiPack *gp, object *setting);
int initPlayers(SDL_Rect_Float_Pos *pl, SDL_Rect_Float_Pos *pr, GuiPack gp);
int initBall(SDL_Circle *ball, GuiPack gp, PhysicsPack pp, SDL_Rect_Float_Pos pl, SDL_Rect_Float_Pos pr);

int renderText( SDL_Renderer *renderer, char *msg, TTF_Font *font, SDL_Color fontColor, size_t x, size_t y, pflag flagV, pflag flagO);
int renderPlAndBall(SDL_Renderer *renderer, SDL_Rect_Float_Pos pl, SDL_Rect_Float_Pos pr, SDL_Circle ball, SDL_Color color);

Player updatePhysics(PhysicsPack *pp, GuiPack gp, SDL_Rect_Float_Pos *pl, SDL_Rect_Float_Pos *pr, SDL_Circle *ball);


/*
    
*/

int main() {

    // Path del file .json contente i setting
    char *setting = "./setting.json";

    // Apertura del file e salvataggio in un buffer
    FILE *json;
    json = fopen(setting, "r");
    if (json == NULL) exit(-1);
    char buffer[1024];
    fread(buffer, 1024, 1, json);
    fclose(json);
    
    // Creazione dell'oggetto json
    object *jsonSetting;
    jsonSetting = json_tokener_parse(buffer);

    // Struttura per la gestione degli eventi SDL
    SDL_Event event;

    // Inizializzazioni
    GuiPack gp;
    initGui(&gp, jsonSetting);

    PhysicsPack pp;
    initPhysics(&pp, jsonSetting);

    SDL_Rect_Float_Pos playerLeft, playerRight;
    initPlayers(&playerLeft, &playerRight, gp);

    SDL_Circle ball;
    initBall(&ball, gp, pp, playerLeft, playerRight);

    // Punti dei giocatori, con rispettivi buffer da convertire in stringhe per l'output
    size_t plPoint = 0, prPoint = 0;
    char *plPointc, *prPointc, *pointToWin;
    asprintf(&pointToWin, "Punti necessari per vincere: %d", pp.winningScore);
    asprintf(&plPointc, "%d", plPoint);
    asprintf(&prPointc, "%d", prPoint);

    /*
        Calcoli per la gestione degli FPS.
        Il loop cercherà di adattarsi agli fps indicati nel file .json, attendendo
            che siano passati 'fpms' microsecondi prima di aggiornare le fisiche,
            in modo da rendere gestibile il movimento dei giocatori e delle palline.
    */
    size_t msps = 1000000;
    double fpms = (double)msps / pp.fps;
    char *fpsc;
    double loopStartTime = 0;

    pbool running = PTRUE;
    Player winner = none;
    char *winnerc;
    
    // Mainloop
    while(running && winner == none) {

        // Qui è dove viene fatto il controlo sugli FPS
        if (getMicrotime() - loopStartTime >= fpms) {
            
            double tfl = getMicrotime() - loopStartTime;
            loopStartTime = getMicrotime();

            Player scorer = updatePhysics(&pp, gp, &playerLeft, &playerRight, &ball);
            if (scorer != none) {
                initPlayers(&playerLeft, &playerRight, gp);
                pp.server = getOpponent(scorer);
                pp.waitingForServe = PTRUE;
                initBall(&ball, gp, pp, playerLeft, playerRight);
                if (scorer == lp) {
                    plPoint += 1;
                    asprintf(&plPointc, "%d", plPoint);
                } else {
                    prPoint += 1;
                    asprintf(&prPointc, "%d", prPoint);
                }
                if (plPoint >= pp.winningScore || prPoint >= pp.winningScore) {
                    winner = scorer;
                    winnerc = (winner == lp) ? "Left player!" : "Right player!";
                }
            }

            asprintf(&fpsc, "ACTUIAL FPS: %d", (int)round(msps / tfl));

            clearRenderer(gp.renderer, gp.bgColor.r, gp.bgColor.g, gp.bgColor.b);

            renderText(gp.renderer, fpsc, gp.font.small, gp.font.color, 5, 20, PBEFORE, PAFTER);
            renderText(gp.renderer, pointToWin, gp.font.small, gp.font.color, 5, 20, PAFTER, PAFTER);
            renderText(gp.renderer, plPointc, gp.font.big, gp.font.color, (gp.w / 2) - 50, 50, PAFTER, PBEFORE);
            renderText(gp.renderer, prPointc, gp.font.big, gp.font.color, (gp.w / 2) + 50, 50, PAFTER, PAFTER);

            renderPlAndBall(gp.renderer, playerLeft, playerRight, ball, gp.font.color);
        }
        
        // Controllo sugli eventi SDL
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = PFALSE;
        }

    }

    // Output del vincitore
    while(running && winner != none) {

        if (getMicrotime() - loopStartTime >= fpms) {
            
            double tfl = getMicrotime() - loopStartTime;
            loopStartTime = getMicrotime();

            asprintf(&fpsc, "ACTUIAL FPS: %d", (int)round(msps / tfl));

            clearRenderer(gp.renderer, gp.bgColor.r, gp.bgColor.g, gp.bgColor.b);
            
            renderText(gp.renderer, fpsc, gp.font.small, gp.font.color, 5, 5, PAFTER, PAFTER);

            renderText(gp.renderer, plPointc, gp.font.big, gp.font.color, (gp.w / 2) - 50, 50, PAFTER, PBEFORE);
            renderText(gp.renderer, prPointc, gp.font.big, gp.font.color, (gp.w / 2) + 50, 50, PAFTER, PAFTER);

            renderText(gp.renderer, "The winer is:", gp.font.big, gp.font.color, gp.w/2, gp.h/2, PBEFORE, PCENTERED);
            renderText(gp.renderer, winnerc, gp.font.big, gp.font.color, gp.w/2, gp.h/2, PAFTER, PCENTERED);

            SDL_RenderPresent(gp.renderer);

        }

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = PFALSE;
        }

    }

    return 0;
}

// ! ---------------------------------------------
// ! Function definitios

double reverseAngleOrizzontal(double a) {

    double a1 = 2 - a;
    if (a1 > 2) a1 = a1 - 2;
    return a1;

}

double reverseAngleVertical(double a) {

    double a1 = 1-a;
    if (a1 < 0) a1 = 2-a1;
    return a1;

}

double map(double val, double i1, double i2, double o1, double o2) {
    return (val - i1) * (o2 - o1) / (i2 - i1) + o1;
}

long getMicrotime(){

	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);

	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

int initPlayers(SDL_Rect_Float_Pos *pl, SDL_Rect_Float_Pos *pr, GuiPack gp) {

    pl->w = gp.pw;
    pr->w = gp.pw;

    pl->h = gp.ph;
    pr->h = gp.ph;

    pl->x = 0;
    pr->x = gp.w - gp.pw;

    pl->y = gp.h/2 - gp.ph/2;
    pr->y = gp.h/2 - gp.ph/2;

    return 0;
}

int initBall(SDL_Circle *ball, GuiPack gp, PhysicsPack pp, SDL_Rect_Float_Pos pl, SDL_Rect_Float_Pos pr) {

    ball->y = gp.h/2;
    ball->r = gp.br;

    ball->x = (pp.server == rp) ? gp.w - pr.w - gp.sPadding : pl.w +  gp.sPadding;

    return 0;
}

int initPhysics(PhysicsPack *pp, object *setting) {

    object *ph;
    json_object_object_get_ex(setting, "physics", &ph);

    object *wscore;
    json_object_object_get_ex(ph, "winningScore", &wscore);
    pp->winningScore = json_object_get_int(wscore);

    object *fps;
    json_object_object_get_ex(ph, "desiredFPS", &fps);
    pp->fps = json_object_get_int(fps);

    object *speed;
    json_object_object_get_ex(ph, "speed", &speed);

    object *pSpeed, *bSpeed;
    json_object_object_get_ex(speed, "player", &pSpeed);
    json_object_object_get_ex(speed, "ball", &bSpeed);

    pp->playerSpeed = json_object_get_double(pSpeed);
    pp->ballSpeed = json_object_get_double(bSpeed);;

    srand((unsigned)time(NULL));
    double pendulum = rand() / (double)RAND_MAX;

    pp->ballAngle = (pendulum >= 0.5) ? 1 : 0;
    pp->server = (pendulum >= 0.5) ? rp: lp;

    pp->waitingForServe = PTRUE;

    return 0;
}

int initGui(GuiPack *gp, object *setting) {

    object *gui;
    json_object_object_get_ex(setting, "gui", &gui);

    object *w, *h, *pw, *ph, *br, *sp;
    json_object_object_get_ex(gui, "width", &w);
    json_object_object_get_ex(gui, "height", &h);
    json_object_object_get_ex(gui, "playeWidth", &pw);
    json_object_object_get_ex(gui, "playerHeight", &ph);
    json_object_object_get_ex(gui, "ballRadius", &br);
    json_object_object_get_ex(gui, "servgingPadding", &sp);

    gp->w = json_object_get_int(w);
    gp->h = json_object_get_int(h);
    gp->pw = json_object_get_int(pw);
    gp->ph = json_object_get_int(ph);
    gp->br = json_object_get_int(br);
    gp->sPadding = json_object_get_int(sp);

    object *bgColor, *r, *g, *b;
    json_object_object_get_ex(gui, "bgColor", &bgColor);
    json_object_object_get_ex(bgColor, "r", &r);
    json_object_object_get_ex(bgColor, "g", &g);
    json_object_object_get_ex(bgColor, "b", &b);

    gp->bgColor.r = json_object_get_int(r);
    gp->bgColor.g = json_object_get_int(g);
    gp->bgColor.b = json_object_get_int(b);

    object *font;
    json_object_object_get_ex(gui, "font", &font);

    object *ptpratio, *path, *size, *fontcolor;
    json_object_object_get_ex(font, "pixelToPointRatio", &ptpratio);
    json_object_object_get_ex(font, "path", &path);
    json_object_object_get_ex(font, "size", &size);
    json_object_object_get_ex(font, "fontColor", &fontcolor);

    asprintf(&(gp->font.path), "%s", json_object_get_string(path));
    gp->font.pixelToPointRatio = json_object_get_double(ptpratio);

    json_object_object_get_ex(fontcolor, "r", &r);
    json_object_object_get_ex(fontcolor, "g", &g);
    json_object_object_get_ex(fontcolor, "b", &b);
    
    gp->font.color.r = json_object_get_int(r);
    gp->font.color.g = json_object_get_int(g);
    gp->font.color.b = json_object_get_int(b);

    object *sSmall, *sBig;
    json_object_object_get_ex(size, "small", &sSmall);
    json_object_object_get_ex(size, "big", &sBig);

    gp->font.fontsizeSmall = json_object_get_int(sSmall) * gp->font.pixelToPointRatio;
    gp->font.fontsizeBig = json_object_get_int(sBig) * gp->font.pixelToPointRatio;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(gp->w, gp->h, 0, &(gp->window), &(gp->renderer));
    TTF_Init();

    gp->font.small = TTF_OpenFont(gp->font.path, gp->font.fontsizeSmall);
    gp->font.big = TTF_OpenFont(gp->font.path, gp->font.fontsizeBig);

}

Player updatePhysics(PhysicsPack *pp, GuiPack gp, SDL_Rect_Float_Pos *pl, SDL_Rect_Float_Pos *pr, SDL_Circle *ball) {

    Uint8 *kState = SDL_GetKeyboardState(NULL);
    Player returnFlag = none;

    pbool plMoved = PFALSE, prMoved = PFALSE;

    if (kState[SDL_SCANCODE_DOWN]) {
        pr->y += pp->playerSpeed;
        prMoved = PTRUE;
    } else if (kState[SDL_SCANCODE_UP]) {
        pr->y -= pp->playerSpeed;
        prMoved = PTRUE;
    }
    if (kState[SDL_SCANCODE_S]) {
        pl->y += pp->playerSpeed;
        plMoved = PTRUE;
    } else if (kState[SDL_SCANCODE_W]) {
        pl->y -= pp->playerSpeed;
        plMoved = PTRUE;
    }

    if (pp->waitingForServe) {
        if (pp->server == rp && prMoved) pp->waitingForServe = PFALSE;
        if (pp->server == lp && plMoved) pp->waitingForServe = PFALSE;
    }

    if (pl->y < 0) pl->y = 0;
    else if (pl->y > gp.h - pl->h) pl->y = gp.h - pl->h;
    if (pr->y < 0) pr->y = 0;
    else if (pr->y > gp.h - pr->h) pr->y = gp.h - pr->h;

    if (!pp->waitingForServe) {
        ball->x += pp->ballSpeed*cos(pp->ballAngle*PI);
        ball->y += pp->ballSpeed*sin(pp->ballAngle*PI);
    }

    if (ball->y - ball->r <= 0 || ball->y + ball->r >= gp.h) pp->ballAngle = reverseAngleOrizzontal(pp->ballAngle);

    pbool scored = PFALSE;

    if (ball->x + ball->r >= gp.w) {
        ball->x = gp.w/2;
        pp->ballAngle = 0;
        returnFlag = lp;
        scored = PTRUE;
    } else if (ball->x - ball->r <= 0) {
        ball->x = gp.w/2;
        pp->ballAngle = 0;
        returnFlag = rp;
        scored = PTRUE;
    }

    if (!scored && (ball->x - ball->r <= pl->w || ball->x + ball->r >= gp.w - pr->w)) {

        if (ball->x <= gp.w/2 && ball->y + ball->r >= pl->y && ball->y - ball->r <= pl->y + pl->h) {
            
            double distFromPlayer = ball->y - pl->y;
            pp->ballAngle = map(distFromPlayer, 0, pl->h, 0, 0.8) - 0.4;

        } else if (ball->x >= gp.w/2 && ball->y + ball->r >= pr->y && ball->y - ball->r <= pr->y + pr->h) {
            
            double distFromPlayer = ball->y - pr->y;
            pp->ballAngle = map(distFromPlayer, 0, pr->h, 1.4, 0.6);

        }
    
    }

    return returnFlag;
}

int clearRenderer(SDL_Renderer *renderer, size_t r, size_t g, size_t b) {

    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    return 0;
}

int SDL_RenderDrawCircle(SDL_Renderer *renderer, SDL_Circle ball) {

    // ! thanks to https://stackoverflow.com/a/47766167

    double pi  = PI;
    double pih = pi / 2.0; //half of pi

    //drew  28 lines with   4x4  circle with precision of 150 0ms
    //drew 132 lines with  25x14 circle with precision of 150 0ms
    //drew 152 lines with 100x50 circle with precision of 150 3ms
    const int prec = 30; // precision value; value of 1 will draw a diamond, 27 makes pretty smooth circles.
    double theta = 0;     // angle that will be increased each loop

    int radiusX = ball.r;
    int radiusY = ball.r;

    //starting point
    int x  = (double)radiusX * cos(theta);//start point
    int y  = (double)radiusY * sin(theta);//start point
    int x1 = x;
    int y1 = y;

    int x0 = ball.x;
    int y0 = ball.y;

    //repeat until theta >= 90;
    double step = pih/(double)prec; // amount to add to theta each time (degrees)
    for(theta=step;  theta <= pih;  theta+=step)//step through only a 90 arc (1 quadrant)
    {
        //get new point location
        x1 = (double)radiusX * cosf(theta) + 0.5; //new point (+.5 is a quick rounding method)
        y1 = (double)radiusY * sinf(theta) + 0.5; //new point (+.5 is a quick rounding method)

        //draw line from previous point to new point, ONLY if point incremented
        if( (x != x1) || (y != y1) )//only draw if coordinate changed
        {
            SDL_RenderDrawLine(renderer, x0 + x, y0 - y,    x0 + x1, y0 - y1 );//quadrant TR
            SDL_RenderDrawLine(renderer, x0 - x, y0 - y,    x0 - x1, y0 - y1 );//quadrant TL
            SDL_RenderDrawLine(renderer, x0 - x, y0 + y,    x0 - x1, y0 + y1 );//quadrant BL
            SDL_RenderDrawLine(renderer, x0 + x, y0 + y,    x0 + x1, y0 + y1 );//quadrant BR
        }
        //save previous points
        x = x1;//save new previous point
        y = y1;//save new previous point
    }
    //arc did not finish because of rounding, so finish the arc
    if(x!=0)
    {
        x=0;
        SDL_RenderDrawLine(renderer, x0 + x, y0 - y,    x0 + x1, y0 - y1 );//quadrant TR
        SDL_RenderDrawLine(renderer, x0 - x, y0 - y,    x0 - x1, y0 - y1 );//quadrant TL
        SDL_RenderDrawLine(renderer, x0 - x, y0 + y,    x0 - x1, y0 + y1 );//quadrant BL
        SDL_RenderDrawLine(renderer, x0 + x, y0 + y,    x0 + x1, y0 + y1 );//quadrant BR
    }

    return 0;
}

int renderPlAndBall(SDL_Renderer *renderer, SDL_Rect_Float_Pos pl, SDL_Rect_Float_Pos pr, SDL_Circle ball, SDL_Color color) {

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);

    SDL_Rect plint, print;
    plint.x = (int)pl.x;
    plint.y = (int)pl.y;
    plint.h = pl.h;
    plint.w = pl.w;
    print.x = (int)pr.x;
    print.y = (int)pr.y;
    print.h = pr.h;
    print.w = pr.w;

    SDL_RenderDrawRect(renderer, &plint);
    SDL_RenderDrawRect(renderer, &print);

    SDL_RenderDrawCircle(renderer, ball);

    SDL_RenderPresent(renderer);

    return 0;
}

int renderText( SDL_Renderer *renderer, char *msg, TTF_Font *font, SDL_Color fontColor, size_t x, size_t y, pflag flagV, pflag flagO) {

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, msg, fontColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect;
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.y = (flagV == PAFTER) ? y : (flagV == PBEFORE) ? y - textRect.h : y - textRect.h/2;
    textRect.x = (flagO == PAFTER) ? x : (flagO == PBEFORE) ? x - textRect.w : x - textRect.w/2;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    return;

}

Player getOpponent(Player pl) {
    if (pl == rp) return lp;
    else if (pl == lp) return rp;
    else return none;
}