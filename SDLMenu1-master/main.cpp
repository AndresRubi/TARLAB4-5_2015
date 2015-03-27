
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<iostream>
#include<vector>
#include<list>
#include "Sho.h"
#include "EnemigoAzul.h"
#include "EnemigoVerde.h"
#include "EnemigoRojo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background;
SDL_Rect rect_background;

TTF_Font *font;
SDL_Surface *message,*message2;
SDL_Texture *text,*text2;
SDL_Rect textRect,textRect2;
SDL_Color textColor;

string data;
bool typing;

int readFrame(){
    ifstream in("puntuacion.dat");
    in.seekg(0);
    int score;
    in.read((char*)&score,4);
    in.close();
//    cout<<"el puntaje actual "<<score<<endl;
    return score;
}

void writeFrame(int frame){
    ofstream out("puntuacion.dat");
    out.write((char*)&frame,4);
    cout<<frame<<endl;
    out.close();
    cout<<"se reemplazo el puntaje viejo "<<frame<<" es el nuevo puntaje"<<endl;
}
//template <typename T>
//  string NumberToString ( T Number )
//  {
//     ostringstream ss;
//     ss << Number;
//     return ss.str();
//  }

void loopJuego()
{
    //Init textures
int total_frame=0;
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondo.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;


    font = TTF_OpenFont("tahoma.ttf",20);
    if(font == NULL)
    {
        return;
    }


//    stringstream ss;
//    ss << total_frame;
//    string str = ss.str();
//    string hola=str;

//    char *intStr = itoa(total_frame);
//    string str = string(intStr);


//string s = lexical_cast <string> (total_frame);


    stringstream strs;
    strs << readFrame();
    string temp_str = strs.str();
    char* char_type = (char*) temp_str.c_str();

    message2 = TTF_RenderText_Solid(font,"Frames Anteriores: ",textColor);
    text2 = SDL_CreateTextureFromSurface(renderer,message2);
    SDL_QueryTexture(text2,NULL,NULL,&w,&h);
    textRect2.x=0;
    textRect2.y=0;
    textRect2.w=w;
    textRect2.h=h;

    message = TTF_RenderText_Solid(font,char_type,textColor);
    text = SDL_CreateTextureFromSurface(renderer,message);
    SDL_QueryTexture(text,NULL,NULL,&w,&h);
    textRect.x=175;
    textRect.y=0;
    textRect.w=w;
    textRect.h=h;





    list<Personaje*> personajes;
    personajes.push_back(new Sho(renderer,&personajes));
    personajes.push_back(new EnemigoVerde(renderer,&personajes));
    personajes.push_back(new EnemigoRojo(renderer,&personajes));

    //Main Loop
    int frame=0;
    cout<<"frames: "<<frame<<endl;
    int animacion_sho = 0;
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);

            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    cout<<"ultima frame: "<<frame<<endl;
                    if (readFrame()<frame)
                    {
                        writeFrame(frame);
                    }
                    else
                    {
                        cout<<"no supero los frames mala suerte trata otra vez guey "<<endl;
                    }

                    return;
                }
            }
        }

        if(frame%1000==0)
        {
            personajes.push_back(new EnemigoAzul(renderer,&personajes));
        }

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            (*p)->act();

        SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);

        // Clear the entire screen to our selected color.
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, background, NULL, &rect_background);
        SDL_RenderCopy(renderer, text2, NULL, &textRect2);



        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            (*p)->draw(renderer);

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            if((*p)->muerto)
            {
                personajes.erase(p);
                break;
            }
        SDL_RenderCopy(renderer, text, NULL, &textRect);
        SDL_RenderPresent(renderer);
        frame++;
        cout<<"frames: "<<frame<<endl;
        total_frame=frame;

    }


}

void instrucciones()
{
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
            }
        }

        int w=0,h=0;
        background = IMG_LoadTexture(renderer,"instrucciones.png");
        SDL_QueryTexture(background, NULL, NULL, &w, &h);
        rect_background.x = 0;
        rect_background.y = 0;
        rect_background.w = w;
        rect_background.h = h;
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,background,NULL,&rect_background);
        SDL_RenderPresent(renderer);
    }
}

void mainMenu()
{
    int opcion = 1;
    SDL_Texture *menu_fondo[3];
    menu_fondo[0] = IMG_LoadTexture(renderer,"menu_fondo1.png");
    menu_fondo[1] = IMG_LoadTexture(renderer,"menu_fondo2.png");
    menu_fondo[2] = IMG_LoadTexture(renderer,"menu_fondo3.png");
    SDL_Rect menu_rect;
    SDL_QueryTexture(menu_fondo[0], NULL, NULL, &menu_rect.w, &menu_rect.h);
    menu_rect.x = 0;
    menu_rect.y = 0;

    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_2)
                {
                    exit(0);
                }
                if(Event.key.keysym.sym == SDLK_1)
                {
                    loopJuego();
                }
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
                if(Event.key.keysym.sym == SDLK_DOWN)
                {
                    opcion++;
                    if(opcion > 3)
                        opcion = 3;
                }
                if(Event.key.keysym.sym == SDLK_UP)
                {
                    opcion--;
                    if(opcion < 1)
                        opcion = 1;
                }
                if(Event.key.keysym.sym == SDLK_RETURN)
                {
                    switch(opcion)
                    {
                        case 1:
                            loopJuego();
                        break;
                        case 2:
                            instrucciones();
                        break;
                        case 3:
                            exit(0);
                        break;
                    }
                }
            }
        }

        SDL_RenderCopy(renderer,menu_fondo[opcion-1],NULL,&menu_rect);

        SDL_RenderPresent(renderer);
    }
}

int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 1024/*WIDTH*/, 768/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    if( TTF_Init() == -1)
    {
        return false;
    }

    mainMenu();

	return 0;
}
