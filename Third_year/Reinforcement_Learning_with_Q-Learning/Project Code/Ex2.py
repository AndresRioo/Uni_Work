

import random
import aichess as ai
import numpy as np
from itertools import permutations



class Ejercicio2:
    def __init__(self, Heuristica = False , isDrunken = False , P1config2 = False):

        # intiialize board
        TA = np.zeros((8, 8))

        #Configuració inicial del taulell
        TA[7][0] = 2 # torre blanca


        if P1config2:
            TA[7][7] = 6 # activar configuracion 2 (rey en la esquina)
        else: 
            TA[7][4] = 6 # rey blanco

        TA[0][4] = 12 # rey negro

        # initialise board
        print("stating AI chess... ")
        self.chess = ai.Aichess(TA, True)

        self.QTabla = {}
        
        self.currentIteracion = 0


        self.Heuristica = Heuristica # si es True usaremos una heuristica en vez de -1 o 100 ( apartado B )
        self.Drunken = isDrunken
    
    def isCheckMate(self, mystate):
        """
        Metodo P1
        """
        
        mystate = self.chess.getWhiteState(mystate) # sacar piezas negras

        # list of possible check mate states
        listCheckMateStates = [[[0,0,2],[2,4,6]],[[0,1,2],[2,4,6]],[[0,2,2],[2,4,6]],[[0,6,2],[2,4,6]],[[0,7,2],[2,4,6]]]

        # Check all state permuations and if they coincide with a list of CheckMates
        for permState in list(permutations(mystate)):
            if list(permState) in listCheckMateStates:
                return True

        return False

    def heuristicaPersonalizada(self,estado):
        """
        Heuristica propia que mira la posicion de la torre y del rey y va sumando recompensas negativas
        
        """

        estadoSorted = sorted(estado,key=lambda x:x[2])

        torre = estadoSorted[0]
        reyBlanco = estadoSorted[1]
        
        recompensa = 0

        if torre == [7,0,2]: # no se ha movido

            recompensa -= 50

        elif torre == [0,0,2]: # ha llegado a la fila del mate en 1 movimiento

            recompensa -= 0
        
        elif torre[1] == 0:

            recompensa -= 10 # esta en la fila del mate pero no en la buena

        else : #se ha movido donde no tocaba

            recompensa -= 200

        filaRey = reyBlanco[1]
        columnaRey = reyBlanco[0]

        # cuando este lejos ( fila 7 ) restara mas
        # cuando este cerca ( fila 2 ) no resta
        if filaRey == 7:
            recompensa -= 100
        elif filaRey == 6:
            recompensa -= 70
        elif filaRey == 5:
            recompensa -= 40
        elif filaRey == 4:
            recompensa -= 20
        elif filaRey == 3:
            recompensa -= 15

    


        if columnaRey == 4: # animar al rey a que vaya recto
            recompensa += 0
        elif columnaRey == 5 or columnaRey == 3: # sigue en columna del mate 
            recompensa -= 10 
        else : # se aleja demasiado
            recompensa -= 70

        
        
        return recompensa





    def reconstructPath(self, initialState):
        """
        Reconstruct the path of moves based on the initial state using Q-values.

        Input:
        - initialState (list): Initial state of the chessboard. eg [[7, 0, 2], [7, 4, 6]]

        Returns:
        - path (list): List of states representing the sequence of moves.
        """
        self.chess.newBoardSim(initialState)
        currentState = initialState
        currentString = self.chess.stateToString(initialState)
        checkMate = False
        self.chess.chess.board.print_board()

        contador = 0

        # Add the initial state to the path
        path = [initialState]
        while not checkMate:

            if currentString not in self.QTabla: 
                # cuando el mejor movimiento es de q-value 0 accede a un estado no visitado 
                print(f"Estado {currentString} no encontrado en la QTabla. Falta aprendizaje.")
                break

            currentDict = self.QTabla[currentString]
            maxQ = -100000
            maxState = None

            # Check which is the next state with the highest Q-value
            for stateString in currentDict.keys():
                qValue = currentDict[stateString]
                if maxQ < qValue:

                    # LINEA AÑADIDA PORQUE
                    # cuando el mejor movimiento es de q-value 0 accede a un estado no visitado sin estar inicializado
                     if stateString in self.QTabla: 
                        maxQ = qValue
                        maxState = stateString

            state = self.chess.stringToState(maxState)
            # When we get it, add it to the path
            path.append(state)
            movement = self.chess.getMovement(currentState, state)
            # Make the corresponding movement
            self.chess.chess.move(movement[0], movement[1])
            self.chess.chess.board.print_board()
            currentString = maxState
            currentState = state

            # When it gets to checkmate, the execution is over
            if self.isCheckMate(self.chess.getCurrentState()):
                checkMate = True

            
            contador += 1
            if contador == 50:
                print(f"La política no es óptima y se queda atascada en un bucle infinito. Falta aprendizaje/ ha convergido mal. (en 50 movimientos no ha hecho mate)")
                break


        print("Sequence of moves: ", path)

    

    def obtener_o_iniciar_valor(self,stringActual, estadoFuturo):
        """
        Función para obtener o iniciar un valor en QTabla

        estado y accion son listas [[]]

        Input

        String estado presente`
        String estado futuro

        """


        stringSiguiente = self.chess.stateToString(estadoFuturo)

        if stringActual not in self.QTabla:
            self.QTabla[stringActual] = {}  # Inicializa el sub-diccionario si no existe
        if stringSiguiente not in self.QTabla[stringActual]:
            self.QTabla[stringActual][stringSiguiente] = 0  # Inicializa el valor Q

        return self.QTabla[stringActual][stringSiguiente]   
    
    


    def ImprimirQTabla(self):
        # Imprimir la tabla Q de forma legible
        for estado, transiciones in self.QTabla.items():
            print(f"Estado: {estado}")
            for estado_proximo, q_value in transiciones.items():
                print(f"  -> {estado_proximo}: {q_value}")
            print()
    

    def LenQtable(self):
        print(f"Tamaño de QTabla (número de estados): {len(self.QTabla)}")
        total_transiciones = sum(len(transiciones) for transiciones in self.QTabla.values())
        print(f"Número total de transiciones en la QTabla: {total_transiciones}")



    def Qlearning(self , alpha , gamma , epsilon , currentState):

        umbral_convergencia = 0.1
        ha_convergido = False

        contador_consecutivo = 0
        listaProhibido = [[0,4,2],[0,5,6],[0,4,6],[0,3,6],[1,5,6],[1,4,6],[1,3,6]] # torre dentro de rey enemigo o rey muy cerca

        n_mate = 0

        #while max_dif > umbral_convergencia:
        while not ha_convergido:


            max_dif = 0 # reiniciar valor a 0 para no coger el del episodio anterior

            # avanzar pieza
            estado = currentState  
            self.chess.newBoardSim(currentState)

            if n_mate % 100 == 0: # imprimir cada 100 mates para ver progresion
                print("mate numero : ",n_mate)

            while not self.isCheckMate(estado):

                # cambiar tablero y coger estados futuros blancos
                self.chess.newBoardSim(estado)
                listaEstadosFuturos = self.chess.getListNextStatesW(estado)
                listaEstadosFuturos = [estado for estado in listaEstadosFuturos if all(prohibido not in estado for prohibido in listaProhibido)]
                # mirar todos los movimientos y filtrarlos 

                currentString = self.chess.stateToString(estado) # coger el estado actual como string

                ExplorarOExplotar = random.uniform(0, 1)  

                # Epsilon greedy
                if ExplorarOExplotar > epsilon: # explotar           

                    listaValores = [ (self.obtener_o_iniciar_valor(currentString, estadoFuturo),estadoFuturo) for estadoFuturo in listaEstadosFuturos]
                    maximo = max( listaValores , key = lambda x : x[0]) #coger el máximo q value
                    proximoEstado = maximo[1] # coger el estado futuro

                else: # explorar

                    proximoEstado = random.choice(listaEstadosFuturos) # coger el estado futuro random



                # Drunken sailor
                if self.Drunken:

                    probabilidad = random.uniform(0, 1)

                    if probabilidad > 0.99:  # 1% de las veces se toma una acción aleatoria
                        proximoEstado = random.choice(listaEstadosFuturos)
                
                

                proximoString = self.chess.stateToString(proximoEstado) #convertir estado futuro en string

                
                if self.isCheckMate(proximoEstado):
                    recompensa = 100  # Premiar fuertemente el mate

                else:  # Estado intermedio

                    if self.Heuristica: # Apartado B
                        recompensa = self.heuristicaPersonalizada(proximoEstado)
                    else:
                        recompensa = -1 

            

                valorQTablaPrevio = self.obtener_o_iniciar_valor(currentString,proximoEstado)

                # coger movimientos del estado futuro
                self.chess.newBoardSim(proximoEstado)
                listaEstadosFuturosProximo = self.chess.getListNextStatesW(proximoEstado)


                listaEstadosFuturosProximo = [estado for estado in listaEstadosFuturosProximo if all(prohibido not in estado for prohibido in listaProhibido)]
                # coger los estados futuros del estado futuro filtrados


                valorQTablaMaximo = max(self.obtener_o_iniciar_valor(proximoString, estadoFuturo) for estadoFuturo in listaEstadosFuturosProximo)
                # coger el q value maximo de los estados futuros del proximo

                # calcular el nuevo q value
                ValorQTablaNuevo = valorQTablaPrevio + alpha * ( recompensa + gamma * valorQTablaMaximo - valorQTablaPrevio )     
                self.QTabla[currentString][proximoString] = ValorQTablaNuevo # actualizar su valor
                
                diferencia = abs(valorQTablaPrevio - ValorQTablaNuevo)
                max_dif= max(max_dif,diferencia)

                
                self.chess.newBoardSim(proximoEstado)
                estado = proximoEstado
            
            n_mate += 1
            epsilon = max(epsilon - 0.001, 0.1) # epsilon dinamico
            #epsilon = 0.1 # epsilon para convergencia con menos mates (menos estable)

            if max_dif < umbral_convergencia:  # si ya no cambian los q values

                if contador_consecutivo == 5: # 5 veces consecutivas
                    ha_convergido = True # paramos de converger
                else:
                    contador_consecutivo += 1
            else:
                contador_consecutivo = 0 # si no son consecutivas que sigan  ( evitar casos excepcionales poco probables )

        return n_mate










if __name__ == "__main__":

    #tablero1.ImprimirQTabla()

    tablero1 = Ejercicio2()

    estadoInicial = tablero1.chess.getCurrentState()

    alpha = 0.3
    epsilon = 0.9  #  1 - explora 0 - explota 
    gamma = 0.9

    input_user = input("2A. parte 1 Ejecutar algoritmo y ver el path. Enter para continuar , [no] para saltar -> ")

    if input_user != 'no':

        episodios = tablero1.Qlearning(alpha,gamma,epsilon,tablero1.chess.getCurrentState())
        tablero1.reconstructPath(estadoInicial)
        print(f"Numero de episodios : {episodios}")

    input_user = input("2A. parte 2 Ejecutar algoritmo 5 veces y ver media de episodios. Enter para continuar , [no] para saltar -> ")


    if input_user != 'no':

        num_episodios_total = 0

        for i in range(5):

            tableroMedia = Ejercicio2() # reiniciar q tabla 

            episodios = tableroMedia.Qlearning(alpha,gamma,epsilon,tableroMedia.chess.getCurrentState())

            tableroMedia = None

            print(f"Iteracion {i} Episodios = {episodios}")

            num_episodios_total += episodios

        print(f"\nMedia de episodios en 5 iteraciones : {num_episodios_total/5}\n")


    input_user = input("2B. parte 1 Ejecutar algoritmo con heuristica y ver el path. Enter para continuar , [no] para saltar -> ")


    if input_user != 'no':

        tableroHeuristica = None
        tableroHeuristica = Ejercicio2(Heuristica=True)

        inicial2B = tableroHeuristica.chess.getCurrentState()
        
        episodios = tableroHeuristica.Qlearning(alpha,0.6,epsilon,tableroHeuristica.chess.getCurrentState())
        tableroHeuristica.reconstructPath(inicial2B)
        print(f"Numero de episodios : {episodios}")


    input_user = input("2B. parte 2 Ejecutar algoritmo 5 veces y ver media de episodios. Enter para continuar , [no] para saltar -> ")


    if input_user != 'no':

        num_episodios_total = 0
        num_episodios_total_gamma2 = 0

        for i in range(5):

            tableroMedia2 = Ejercicio2(Heuristica=True) # reiniciar q tabla 

            episodios = tableroMedia2.Qlearning(alpha,gamma,epsilon,tableroMedia2.chess.getCurrentState())

            tableroMedia2Gamma = Ejercicio2(Heuristica=True) # reiniciar q tabla 

            episodiosGamma6 = tableroMedia2Gamma.Qlearning(alpha,0.6,epsilon,tableroMedia2Gamma.chess.getCurrentState())

            tableroMedia2 = None
            tableroMedia2Gamma = None

            print(f"Iteracion {i} Episodios = {episodios}")
            print(f"Iteracion {i} Episodios (gamma mas bajo)= {episodiosGamma6}")

            num_episodios_total += episodios
            num_episodios_total_gamma2 += episodiosGamma6

        print(f"\nMedia de episodios en 5 iteraciones : {num_episodios_total/5}\n")        
        print(f"\nMedia de episodios en 5 iteraciones (gamma mas bajo): {num_episodios_total_gamma2/5}\n")


    input_user = input("2C. parte 1 Ejecutar algoritmo con recompensa -1 y drunked y ver el path. Enter para continuar , [no] para saltar -> ")

    if input_user != 'no':

        tableroDrunked = Ejercicio2(isDrunken=True)
        inicial2C = tableroDrunked.chess.getCurrentState()

        
        episodios = tableroDrunked.Qlearning(alpha,0.6,epsilon,tableroDrunked.chess.getCurrentState())
        tableroDrunked.reconstructPath(inicial2C)
        print(f"Numero de episodios : {episodios}")



    input_user = input("2C. parte 2 Ejecutar algoritmo Drunked 5 veces y ver media de episodios. Enter para continuar , [no] para saltar -> ")


    if input_user != 'no':

        num_episodios_total_drunked = 0
        num_episodios_total_drunked_difparam = 0

        for i in range(5):

            tableroDrunken = Ejercicio2(isDrunken=True) # reiniciar q tabla 
            tableroDrunken2 = Ejercicio2(isDrunken=True) # reiniciar q tabla 

            episodiosDrunked = tableroDrunken.Qlearning(alpha,gamma,epsilon,tableroDrunken.chess.getCurrentState())
            episodiosDrunkedParam2 = tableroDrunken2.Qlearning(0.15,0.7,epsilon,tableroDrunken2.chess.getCurrentState())

            tableroDrunken = None
            tableroDrunken2 = None

            print(f"Iteracion {i} Episodios Drunked = {episodiosDrunked}")
            print(f"Iteracion {i} Episodios Drunked (diferentes parametros)= {episodiosDrunkedParam2}")

            num_episodios_total_drunked += episodiosDrunked
            num_episodios_total_drunked_difparam += episodiosDrunkedParam2

        print(f"\nMedia de episodios en 5 iteraciones Drunked : {num_episodios_total_drunked/5}\n")        
        print(f"\nMedia de episodios en 5 iteraciones Drunked (diferentes parametros): {num_episodios_total_drunked_difparam/5}\n")


    input_user = input("VOLUNTARIO. Algoritmo Drunked 5 veces con 5 sets diferentes de parametros. Enter para continuar , [no] para saltar -> ")


    if input_user != 'no':

        num_episodios_total_drunked_param1 = 0
        num_episodios_total_drunked_param2 = 0
        num_episodios_total_drunked_param3 = 0
        num_episodios_total_drunked_param4 = 0
        num_episodios_total_drunked_param5 = 0

        for i in range(5):

            tableroDrunken1 = Ejercicio2(isDrunken=True,P1config2=True) # reiniciar q tabla 
            tableroDrunken2 = Ejercicio2(isDrunken=True,P1config2=True) # reiniciar q tabla 
            tableroDrunken3 = Ejercicio2(isDrunken=True,P1config2=True) # reiniciar q tabla 
            tableroDrunken4 = Ejercicio2(isDrunken=True,P1config2=True) # reiniciar q tabla 
            tableroDrunken5 = Ejercicio2(isDrunken=True,P1config2=True) # reiniciar q tabla 

            episodiosDrunkedParam1 = tableroDrunken1.Qlearning(0.3  ,0.90  ,epsilon,tableroDrunken1.chess.getCurrentState())
            episodiosDrunkedParam2 = tableroDrunken2.Qlearning(0.15 ,0.90 ,epsilon,tableroDrunken2.chess.getCurrentState())
            episodiosDrunkedParam3 = tableroDrunken3.Qlearning(0.3  ,0.70 ,epsilon,tableroDrunken3.chess.getCurrentState())
            episodiosDrunkedParam4 = tableroDrunken4.Qlearning(0.15 ,0.70 ,epsilon,tableroDrunken4.chess.getCurrentState())
            episodiosDrunkedParam5 = tableroDrunken5.Qlearning(0.1  ,0.55 ,epsilon,tableroDrunken5.chess.getCurrentState())

            tableroDrunken1 = None
            tableroDrunken2 = None
            tableroDrunken3 = None
            tableroDrunken4 = None
            tableroDrunken5 = None

            print(f"Iteracion {i} Episodios Drunked param 1 = {episodiosDrunkedParam1}")
            print(f"Iteracion {i} Episodios Drunked param 2 = {episodiosDrunkedParam2}")
            print(f"Iteracion {i} Episodios Drunked param 3 = {episodiosDrunkedParam3}")
            print(f"Iteracion {i} Episodios Drunked param 4 = {episodiosDrunkedParam4}")
            print(f"Iteracion {i} Episodios Drunked param 5 = {episodiosDrunkedParam5}")

            num_episodios_total_drunked_param1 += episodiosDrunkedParam1
            num_episodios_total_drunked_param2 += episodiosDrunkedParam2
            num_episodios_total_drunked_param3 += episodiosDrunkedParam3
            num_episodios_total_drunked_param4 += episodiosDrunkedParam4
            num_episodios_total_drunked_param5 += episodiosDrunkedParam5

        print(f"\nMedia de episodios en 5 iteraciones Drunked param 1 ( alpha = 0.30 , gamma = 0.90 ): {num_episodios_total_drunked_param1/5}\n")        
        print(f"\nMedia de episodios en 5 iteraciones Drunked param 2 ( alpha = 0.15 , gamma = 0.90 ): {num_episodios_total_drunked_param2/5}\n")
        print(f"\nMedia de episodios en 5 iteraciones Drunked param 3 ( alpha = 0.30 , gamma = 0.70 ): {num_episodios_total_drunked_param3/5}\n")
        print(f"\nMedia de episodios en 5 iteraciones Drunked param 4 ( alpha = 0.15 , gamma = 0.70 ): {num_episodios_total_drunked_param4/5}\n")
        print(f"\nMedia de episodios en 5 iteraciones Drunked param 5 ( alpha = 0.10 , gamma = 0.55 ): {num_episodios_total_drunked_param5/5}\n")
