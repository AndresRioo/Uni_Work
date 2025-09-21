


# TESTING DEL NOSTRE CODI

Primer la part individual i els JUNITS, al fons la part de test creuat.


## Testing individual de la pràctica

Aquesta part la dividim en dues parts, testing executant el codi entre nosaltres provant el codi i casos extrems i
tests JUNIT per verificar mètodes i classes individualment.

### Testing entre nosaltres

Al posar casos extrems vam trobar errors en el codi que vam anar solucionant. Els errors més comuns eren de desincronització
i problemes de desconexió del servidor, que vam arreglar amb un try catch abans de tot el codi del servidor i assegurar tancar
correctament els sockets un cop acabi la partida.

Un altre error que estava per tot el codi eran els inputs del client, on vam afegir un mètode per a totes les lectures que evites
inputs no desitjats.

Altres errors més extrems van ser que pasaria si fesim una partida amb caracteristiques molt extremes, com un tauler molt petit on 
els vaixells no caben. Vam implementar un check per a que això no passi (él mètode es molt naive, pot ser hi ha cassos extrems on 
si els vaixells es posan de manera que deixin forats, el check no saltaria).

Per acabar vam veure amb un altre grup que les instancies negatives no es gestionaven correctament, ja que al sumar la part de
unitats, al ser valors negatius es perdia el tipus de vaixell causant problemes en la partida. Com treballem en bytes i el número
màxim de vaixells és 25, tots els valors de -12 a 12 poden ser una instancia. La nostra forma de asignar el valor a la casella era 
sumant centenes, desenes i unitats, però al ser negatiu, la unitat (el tipus) no concordaba amb la instancia (negativa), causant
tipus no existents. Vam tenir que arrelgar el codi per afegir els valors negatius correctament i la comprobació de hit i de sunk. 


### JUNIT

Els tests JUNIT van ser molt útils per a comprovar que els mètodes funcionaven correctament. 
Vam fer tests per les parts claus del nostre mètode

#### Test per a cada missatge

Simplement simulem un missatge mitjançant fitxers i comprovem que el missatge sigui el mateix que el que esperem.

#### Test per addvessel

Vam crear 2 tests, un per el cas on si que s'afig un vaixell i un altre per el cas que no es pot a causa d'un error. 
En el cas d'error, vam probar tots els possibles casos d'error, com afegir un vaixell fora del tauler, en una posició ja ocupada,
un vaixell no existent (de tipus o tamany) i que el estat sigui el correcte

#### Test per Shot

Aqui vam crear 3 tests, un per el cas que el shot sigui correcte, un altre per el cas que no sigui correcte i un altre per el cas
on el shot el fa el bot.

En el cas de shot per el jugador, creem un tauler predeterminat i provem a fer fail, hit i sunk, per veure com queda posteriorment
el tauler

En el cas d'error vam comprovar que estigues dins de les coordenades del tauler i que no es repetis, que el ID del torn sigui
el correcte i que el tipus de vaixell sigui el correcte.

En el cas del bot simplement fem un shot i comparem el tauler original amb el tauler després del shot, comprovant que hi hagi
un canvi als valors. 

#### Test per la creació d'un tauler random per al bot

En aquest cas no sol comprovem que el bot tingui un tauler vàlid en diferents escenaris. Com aquest valor no el pot donar
l'usuari i ve donat per el server, simplement l'hem utilitzat per veure taulers amb molts vaixells o amb molt pocs.

Hem fet un test per 

- Tauler bàsic 10*10 amb 2 vaixells de cada
- Tauler al límit 5*5 amb 5 vaixells de mida 5
- Tauler 10*3 amb 5 vaixells de mida 3
- Tauler amb el màxim número de vaixells 125 en un tauler molt gran
- Tauler prohibit (26 vaixells de un tipus, o massa vaixells per a un tauler)

Amb aquest mètode vam poder comprovar els errors de instancies negatives i que el tauler sigui correcte.














# Sessió de proves creuades
| Grup | Components            | Usuari GitHub |
|------|-----------------------|---------------|
| c12  | Rio Nogues, Andres    | AndresRioo    |
|      | Falgosa Campoy, Aleix | Aleix15       |


## Sessió de proves creuades

### La vostra pràctica
En aquest apartat cal explicar l'estat inicial de la vostra pràctica:

- __Servidor__
- [x] El meu __Servidor__ arranca i permet que es connectin __Clients__, assignant-los a una partida.
- [x] El meu __Servidor__ té implementada la fase de configuració en que els __Clients__ afegeixen els vaixells al tauler.
- [x] El meu __Servidor__ implementa la dinàmica de joc, en la qual els __Clients__ van disparant en posicions per enfonsar els vaixells.
- [] El meu __Servidor__ implementa el joc **multi-jugador**.
- __Client__
- [x] El meu __Client__ es connecta correctament al servidor, afegint-se a una partida.
- [x] El meu __Client__ té implementada la fase de configuració en que l'usuari pot anar afegint els vaixells al tauler.
- [x] El meu __Client__ implementa la dinàmica de joc, en la qual l'usuari va indicant les posicions on disparar per enfonsar els vaixells de l'oponent.


A banda de les proves realitzades durant la sessió de proves creuades, en aquest apartat caldrà incloure les proves que heu realitzat en el vostre pròpi codi, tant d'usuari com amb proves unitàries JUnit.


### Proves realitzades

Per cada grup que hagueu provat, caldra informar del nom del Grup que s'ha avaluat i la informació bàsica equivalent a la anterior:

### Client

##### C01

- __Servidor__
- [x] El seu __Servidor__ arranca i permet que es connectin __Clients__, assignant-los a una partida.
- [x] El seu __Servidor__ té implementada la fase de configuració en que els __Clients__ afegeixen els vaixells al tauler.
- [x] El seu __Servidor__ implementa la dinàmica de joc, en la qual els __Clients__ van disparant en posicions per enfonsar els vaixells.
- [] El seu __Servidor__ implementa el joc **multi-jugador**.
- __Client__
- [x] El seu __Client__ es connecta correctament al servidor, afegint-se a una partida.
- [x] El seu __Client__ té implementada la fase de configuració en que l'usuari pot anar afegint els vaixells al tauler.
- [x] El seu __Client__ implementa la dinàmica de joc, en la qual l'usuari va indicant les posicions on disparar per enfonsar els vaixells de l'oponent.

Al començar el testeig, provem d’afegir vaixells al taulell, en el moment que afegíem el segon vaixell a la posició 2,2 en vertical, després de posar un vaixell de 5 a la posició 1,1, es produïa un error inesperat de "Connection reset by peer". Provem de repetir la mateixa partida i torna a aparèixer l’error.

Fem més proves per veure si el problema era que el vaixell s’afegia incorrectament, però veiem que l'error no prové d'aqui. Per tant, ens fiquem a indagar millor, comprovem que l’error es deu a que el servidor tancava la connexió de manera inesperada. Per tant, demanem que es reiniciï el servidor.

Un cop solucionat el problema, veiem que el codi funciona correctament i es connecta bé a la partida. Seguidament, comprovem si els vaixells s’afegeixen correctament, revisant possibles errors bàsics que potser no havíem contemplat, com ara què passa si intento posar un vaixell fora del taulell o en una posició ja ocupada. Aquests errors es controlen adequadament.

Quan comença la partida, veiem que el hit funciona correctament i que la configuració de la partida es mostra de manera adequada. Per tant, després d’una última prova, confirmem una vegada més que el codi del grup C01 no presenta cap error visible.

##### C02

- __Servidor__
- [x] El seu __Servidor__ arranca i permet que es connectin __Clients__, assignant-los a una partida.
- [x] El seu __Servidor__ té implementada la fase de configuració en que els __Clients__ afegeixen els vaixells al tauler.
- [x] El seu __Servidor__ implementa la dinàmica de joc, en la qual els __Clients__ van disparant en posicions per enfonsar els vaixells.
- [] El seu __Servidor__ implementa el joc **multi-jugador**.
- __Client__
- [x] El seu __Client__ es connecta correctament al servidor, afegint-se a una partida.
- [x] El seu __Client__ té implementada la fase de configuració en que l'usuari pot anar afegint els vaixells al tauler.
- [x] El seu __Client__ implementa la dinàmica de joc, en la qual l'usuari va indicant les posicions on disparar per enfonsar els vaixells de l'oponent.

En aquest cas, al fer el testeig vam tenir un error a l'hora d'unir-se a la partida que provocaba un tancament correcte del servidor, després de veure com solucionar-ho, vam veure que aquest error es deu a una mala interpretació del gameId, que s’està gestionant com un uint8 en comptes de uint32. Això fa que, en rebre un id de partida massa gran, es transformi en un valor incorrecte, provocant que el servidor no reconegui la partida i tanqui la connexió amb un MessageError no especificat.

#### C08

- __Servidor__
- [x] El seu __Servidor__ arranca i permet que es connectin __Clients__, assignant-los a una partida.
- [x] El seu __Servidor__ té implementada la fase de configuració en que els __Clients__ afegeixen els vaixells al tauler.
- [x] El seu __Servidor__ implementa la dinàmica de joc, en la qual els __Clients__ van disparant en posicions per enfonsar els vaixells.
- [] El seu __Servidor__ implementa el joc **multi-jugador**.
- __Client__
- [x] El seu __Client__ es connecta correctament al servidor, afegint-se a una partida.
- [x] El seu __Client__ té implementada la fase de configuració en que l'usuari pot anar afegint els vaixells al tauler.
- [x] El seu __Client__ implementa la dinàmica de joc, en la qual l'usuari va indicant les posicions on disparar per enfonsar els vaixells de l'oponent.

#### C10

- __Servidor__
- [x] El seu __Servidor__ arranca i permet que es connectin __Clients__, assignant-los a una partida.
- [] El seu __Servidor__ té implementada la fase de configuració en que els __Clients__ afegeixen els vaixells al tauler.
- [] El seu __Servidor__ implementa la dinàmica de joc, en la qual els __Clients__ van disparant en posicions per enfonsar els vaixells.
- [] El seu __Servidor__ implementa el joc **multi-jugador**.
- __Client__
- [x] El seu __Client__ es connecta correctament al servidor, afegint-se a una partida.
- [] El seu __Client__ té implementada la fase de configuració en que l'usuari pot anar afegint els vaixells al tauler.
- [] El seu __Client__ implementa la dinàmica de joc, en la qual l'usuari va indicant les posicions on disparar per enfonsar els vaixells de l'oponent.

### Proves extres

#### F08

- __Servidor__
- [x] El seu __Servidor__ arranca i permet que es connectin __Clients__, assignant-los a una partida.
- [x] El seu __Servidor__ té implementada la fase de configuració en que els __Clients__ afegeixen els vaixells al tauler.
- [x] El seu __Servidor__ implementa la dinàmica de joc, en la qual els __Clients__ van disparant en posicions per enfonsar els vaixells.
- [] El seu __Servidor__ implementa el joc **multi-jugador**.
- __Client__
- [x] El seu __Client__ es connecta correctament al servidor, afegint-se a una partida.
- [x] El seu __Client__ té implementada la fase de configuració en que l'usuari pot anar afegint els vaixells al tauler.
- [x] El seu __Client__ implementa la dinàmica de joc, en la qual l'usuari va indicant les posicions on disparar per enfonsar els vaixells de l'oponent.

Vam probar a jugar partides amb el create i vam trobar un error en el qual el servidor no acceptava el create. 
Això pasava per la confusió d'enviar el paràmetre IA com a false, donant una situació que no es gestionava correctament. 

La resta de la partida va funcionar correctament.


### Servidor

#### C08

- __Servidor__
- [x] El seu __Servidor__ arranca i permet que es connectin __Clients__, assignant-los a una partida.
- [x] El seu __Servidor__ té implementada la fase de configuració en que els __Clients__ afegeixen els vaixells al tauler.
- [x] El seu __Servidor__ implementa la dinàmica de joc, en la qual els __Clients__ van disparant en posicions per enfonsar els vaixells.
- [] El seu __Servidor__ implementa el joc **multi-jugador**.
- __Client__
- [x] El seu __Client__ es connecta correctament al servidor, afegint-se a una partida.
- [x] El seu __Client__ té implementada la fase de configuració en que l'usuari pot anar afegint els vaixells al tauler.
- [x] El seu __Client__ implementa la dinàmica de joc, en la qual l'usuari va indicant les posicions on disparar per enfonsar els vaixells de l'oponent.

Teniem problemes amb el sistema de coordenades, que estavèn en 0-based al server i en 1-based al client. A més que el missatge de GameStatus a la hora de fer Shot per l'altre grup era 
opcional. O el demanaven manualment cada cop o es quedava penjat esperant un missatge que no era. No vam poder 
arribar al final de la partida per causa del GameStatus. 

#### C10

- __Servidor__
- [x] El seu __Servidor__ arranca i permet que es connectin __Clients__, assignant-los a una partida.
- [] El seu __Servidor__ té implementada la fase de configuració en que els __Clients__ afegeixen els vaixells al tauler.
- [] El seu __Servidor__ implementa la dinàmica de joc, en la qual els __Clients__ van disparant en posicions per enfonsar els vaixells.
- [] El seu __Servidor__ implementa el joc **multi-jugador**.
- __Client__
- [x] El seu __Client__ es connecta correctament al servidor, afegint-se a una partida.
- [] El seu __Client__ té implementada la fase de configuració en que l'usuari pot anar afegint els vaixells al tauler.
- [] El seu __Client__ implementa la dinàmica de joc, en la qual l'usuari va indicant les posicions on disparar per enfonsar els vaixells de l'oponent.

No va probar el meu server, no sabia qui era a classe (nose perque el meu grup tenia 4 persones assignades i no 2, massa gent probant el codi alhora).


#### C11 o B01 ( no recordo , només vam preguntar noms )

#### Grup de NACHO 

- __Servidor__
- [x] El seu __Servidor__ arranca i permet que es connectin __Clients__, assignant-los a una partida.
- [x] El seu __Servidor__ té implementada la fase de configuració en que els __Clients__ afegeixen els vaixells al tauler.
- [x] El seu __Servidor__ implementa la dinàmica de joc, en la qual els __Clients__ van disparant en posicions per enfonsar els vaixells.
- [] El seu __Servidor__ implementa el joc **multi-jugador**.
- __Client__
- [x] El seu __Client__ es connecta correctament al servidor, afegint-se a una partida.
- [x] El seu __Client__ té implementada la fase de configuració en que l'usuari pot anar afegint els vaixells al tauler.
- [x] El seu __Client__ implementa la dinàmica de joc, en la qual l'usuari va indicant les posicions on disparar per enfonsar els vaixells de l'oponent.

Teniem problemes amb el sistema de coordenades, que estavèn en 0-based al server i en 1-based al client. 
La info que le pasava el GameStatus de board del player 2 no li arribava bé, però al grup 08 si que li arribava
correctament. La resta funcionava i podiem arribar al final de la partida, guanyant i perdent.

#### L'altre grup

El meu gameStatus per defecte enviaba un tauler 3*3 amb 2 vaixells per fer un testing més ràpid, però el seu codi
esperava sempre 5 vaixells. No vam poder testar més enllà de la fase de configuració.


### Proves extres

#### C05

- __Servidor__
- [x] El seu __Servidor__ arranca i permet que es connectin __Clients__, assignant-los a una partida.
- [x] El seu __Servidor__ té implementada la fase de configuració en que els __Clients__ afegeixen els vaixells al tauler.
- [x] El seu __Servidor__ implementa la dinàmica de joc, en la qual els __Clients__ van disparant en posicions per enfonsar els vaixells.
- [] El seu __Servidor__ implementa el joc **multi-jugador**.
- __Client__
- [x] El seu __Client__ es connecta correctament al servidor, afegint-se a una partida.
- [x] El seu __Client__ té implementada la fase de configuració en que l'usuari pot anar afegint els vaixells al tauler.
- [x] El seu __Client__ implementa la dinàmica de joc, en la qual l'usuari va indicant les posicions on disparar per enfonsar els vaixells de l'oponent.

Abans de començar la classe, vam provar el codi i vam trobar un error per una mala interpretació de l'enunciat.
Quan el client introduïa les coordenades, nosaltres li restàvem una posició perquè el tauler comencés en 0,0 en lloc de 1,1.
Això va provocar un error de desincronització, ja que el client del grup C05 feia servir el sistema de coordenades correcte,
causant incompatibilitats en la comunicació .

També vam probar una partida de 25 vaixells i vam veure que les instancies negatives no es gestionaven correctament.
Al sumar la part de unitats, al ser valors negatius es perdia el tipus de vaixell causant problemes en la partida.

