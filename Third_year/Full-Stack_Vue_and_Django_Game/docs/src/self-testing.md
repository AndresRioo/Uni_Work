# Self Testing

## Group Information

- Your group and team members:
  - Group: [ C12 ]
  - Team members: [Andres Rio, Aleix Falgosa]

### Implementation checklist

- Initialization:
  - [x] authentication works correctly
  - [x] (**OPT**) registration is implemented
  - [x] game can be created
- Gameplay:
  - [x] can place ships
  - [x] can fire shots
  - [x] can receive hits and misses
  - [x] can play against a bot
  - [x] game ends correctly (win/loss)
  - [] (**OPT**) multiplayer is implemented
  - [] multiplayer works correctly
- Stress Testing:
  - [x] can handle multiple concurrent games
  - [] can handle multiple concurrent players
  - [x] game can be restarted (disconnected players can rejoin)
  - [x] behaviour when cookies are disabled
- Post game:

  - [x] (**OPT**) leaderboard is implemented

- Additional features you implemented (please specify):
  - [] ...
    - [] ...
    - [] ...

### Encountered issues, how you solved them if you did.

En aquest test hem descobert diversos errors (`La solució explicada en el següent apartat`):


- El primer seria la rotació dels vaixells, aquesta no funciona correctament i permet posicions de vaixells que no haurien de ser posibles.

- Un altre error amb els vaixells, és que existeix la possibilitat que el bot no tingui 5 vaixells al tauler, causant una clara avantatge al user. No l'hem resolt a classe, però hem detectat que té a veure amb el codi per verificar si el vaixell es pot posar o no, que no verifica correctament les posicions horizontals.

- Un altre error que hem vist és que no poden jugar dos jugadors a la vegada en finestres diferents. Si s'inicia sessió amb altre usuari, automàticament l'altre finestra agafa el nou jugador. No hem trobat on succeeix l'error, però ho tenim pendent de solucionar per l'entrega final.

- El bot mai podia guanyar perque la partida continua, causant que la partida es quedi congelada

- Finalment, la leading table hi ha cops que espontàniament no s'actualitza correctament, no hem pogut recrear perquè passa, ni hem trobat la font.

### Post testing improvements

S'ha solucionat el problema de la rotació dels vaixells, ara es pot col·locar correctament i no permet posicions que no haurien de ser possibles.

S'ha solucionat el problema que no poden jugar dos jugadors a la vegada en finestres diferents, ara es pot iniciar sessió amb un altre usuari i no afecta a la partida que s'està jugant, això ho hem solucionat utilitzant SessionStorage en canvi de LocalStorage.

Amb el tema de la leading table hem trobat diversos errors sobretot amb la funcionalitat de un jugador que acaba una partida on no es el `owner`. El que hem implementat és que la partida conta per el user que acaba aquesta partida, donant la possibilitat de unir-se a qualsevol partida de qualsevol jugador.
L'error de que la leading table no anava bé està solucionat, ara les partides jugades i guanyades conten per al que termina la partida i també conten per al bot.

També habien errors a l'hora de tornar a començar la partida, sobretot al refrescar la pàgina en moments no adecuats, on hem intentat arreglar-ho evitant que partides en phase de waiting, placing o gameover no es tornin a carregar encara que si existeixin (només tornem a una partida antiga si estàvem a la fase de jugar, si no creem una partida nova).

Ara el bot pot guanyar i acabar la partida

