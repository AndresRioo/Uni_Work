# Beta Testing

## Testing scenarios

- Case A: The game is fully functional - i.e., frontend and backend are implemented and communicate correctly. In this case, the testing is performed on the frontend by playing the game.
- Case B: The game is partially functional - i.e., frontend is not fully connected to the backend. In this case, the testing is performed on the backend by sending requests to the API endpoints using the `api/v1/*/` endpoints or `docs/` url.
- Case C: The backend is partially functional - i.e., the backend is not fully implemented. In this case, the testers will interview the developers about what is working and what is not, and about the main issues they encountered and discuss/advise on how to fix them.

## Group Information

- Your group and team members:
  - Group: [c12]
  - Team members: [Andres Rio, Aleix Falgosa]

## Tested Group Information

### Test group 1

- Test group 1:
  - Group: [B1]
  - Team members: [ Francesc Curto, Javier Blanco]

### Case A checklist

- Initialization:
  - [x] authentication works correctly
  - [x] (**OPT**) registration is implemented   

Funciona correctament, però l'usuari no té feedback de que s'ha registrat correctament, només es pot veure al backend.

  - [x] game can be created

El joc es pot crear correctament, però la informació no s'acaba d'afegir correctament en el backend, un error que vam veure es que no es pot veure qui és el user de la partida.

- Gameplay:
  - [x] can place ships
  - [] can fire shots

Es coloquen correctament els vaixells al Frontend i en el Backend en un endpoint de BoardVessels. L'error ve quan al fer Sunk a un vaixell, si que es veu reflexat en el Frontend, però no en el Backend, ja que hauria de cambiar alive de True a False i no ha estat el cas.

  - [x] can receive hits and misses
  - [x] can play against a bot
  - [] game ends correctly (win/loss)

La partida permet recivir hits i misses correctament, també funciona correctament el Bot, però no té la lògica per acabar la partida.

  - [] (**OPT**) multiplayer is implemented

Funcionalitat de multijugador no implementada, només es pot jugar contra el bot.

  - [] multiplayer works correctly

Funcionalitat no implementada.

- Stress Testing:
  - [x] can handle multiple concurrent games
  - [x] can handle multiple concurrent players
  - [] game can be restarted (disconnected players can rejoin)

Pot suportar múltiples partides al mateix temps i amb multiples jugador, però no es pot reiniciar una partida, ja que no hi ha lògica per fer-ho, actualment hauries de saber l'URL per reiniciar la partida.

  - [x] behaviour when cookies are disabled

Funciona correctament.

- Post game:

  - [] (**OPT**) leaderboard is implemented

- Additional tests (please specify):

Buscant errors aillats, hem trobat que hi ha una casella (A1), que sempre es detecta com un vaixell tot i que no tingui cap vaixell aprop, per tant es podia fer hit a una casella sense cap vaixell.

- Summarize the interview

En resum, aquest grup tenien la pràctica a mitjes, on hi havia persistència al backend amb errades a la hora de guardar informació i no tenen en compte el diseny del protocol, ja que tots els objectes del Game no el tenen a Game en si mateix, els tenen com objectes independents. Tot una mica confós, però  parcialment funcional, ja que es podia jugar una partida normal amb el frontend, encara que no tenia la lògica per acabar la partida i hi havien errors visuals com un hit a una casella sense cap vessel. 

### Test group 2

## Group Information

- Your group and team members:
  - Group: [c12]
  - Team members: [Andres Rio, Aleix Falgosa]

## Tested Group Information

### Test group 1

- Test group 1:
  - Group: [B11]
  - Team members: [ Max Semenchuk, Arnau Gelonch]

### Case B checklist

- Initialization:
  - [x] you can get a token pair
  - [x] (**OPT**) registration is implemented
  - [x] authorization is set up correctly for the Users API
  - [x] game can be created
- Gameplay:
  - [] can place ships
  - [] can fire shots
  - [] can receive hits and misses
  - [] can play against a bot
  - [] game ends correctly (win/loss)
  - [] (**OPT**) multiplayer is implemented
  - [] multiplayer works correctly
- Post game:

  - [] (**OPT**) leaderboard is implemented

- Additional tests (please specify):
  - [] ...
    - [] ...
    - [] ...

### Case C checklist

- Summarize the interview

No s'ha pogut fer correctament l'entrevista, ja que l'API no funcionava de cap manera i el registre del Frontend directament no ens permetia accedir a la partida. L'únic testeig, ha estat a l'hora d'inicialitzar el user i game, però desafortunadament no hem pogut anar més enllà.