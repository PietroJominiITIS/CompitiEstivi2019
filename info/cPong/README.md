# cPong
>Gioco 'pong', scritto in c, con ausilio grafico della libreria SDL2

## Librerie utilizzate:
* [SDL2](https://www.libsdl.org/index.php)
* [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
* [json-c](https://github.com/json-c/json-c/wiki)
Per l'installazione rifarsi alle guide ufficiali.
### Gcc flags necessarie:
``` bash 
... -lSDL2 -lSDL2_ttf -ljson-c ...
```

## Setting
>Il gioco prende le impostazioni in input da un file .json, il cui path è specificato nel main()

### Struttura del file .json:
``` json
{
    "gui": {
        "width": unsigned int,
        "height": unsigned int,
        "playeWidth": unsigned int,
        "playerHeight": unsigned int,
        "ballRadius": unsigned int,
        "servgingPadding": unsigned int,
        "bgColor": {
            "r": unsigned int [0..255],
            "g": unsigned int [0..255],
            "b": unsigned int [0..255]
        },
        "font": {
            "pixelToPointRatio": double,
            "path": char *,
            "size": {
                "small": unsigned int,
                "big": unsigned int
            },
            "fontColor": {
                "r": unsigned int [0..255],
                "g": unsigned int [0..255],
                "b": unsigned int [0..255]
            }
        }
    },
    "physics": {
        "speed": {
            "player": double,
            "ball": double
        },
        "desiredFPS": unsigned int,
        "winningScore": unsigned int
    }
}
```
> **La struttura del file non deve essere modificata!** <br>
Si ricorda inoltre che le velocità sono espresse in pixel/frame, quindi devono essere relative ai desiredFPS inseriti.<br>
Nel file  `setting.json` sono inseriti dei valori di esempio. 

## Autore
``` json
{
    "name": "Pietro Jomini",
    "type": "School assignement"
}
```
>Per ulteriori informazioni sul codise, *visualizzare il codice!*