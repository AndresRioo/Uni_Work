#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Sep  8 11:22:03 2022

@author: ignasi
"""
import copy
import math

import chess
import board
import numpy as np
import sys
import queue
from typing import List
import random

RawStateType = List[List[List[int]]]

from itertools import permutations


class Aichess():
    """
    A class to represent the game of chess.

    ...

    Attributes:
    -----------
    chess : Chess
        represents the chess game

    Methods:
    --------
    startGame(pos:stup) -> None
        Promotes a pawn that has reached the other side to another, or the same, piece

    """

    def __init__(self, TA, myinit=True):

        if myinit:
            self.chess = chess.Chess(TA, True)
        else:
            self.chess = chess.Chess([], False)

        self.listNextStates = []
        self.listVisitedStates = []
        self.listVisitedSituations = []
        self.pathToTarget = []
        self.currentStateW = self.chess.boardSim.currentStateW;
        self.depthMax = 8;
        self.checkMate = False

    # Methods to add in aichess.py

    def stateToString(self, whiteState):
        """
        Convert the white pieces' state to a string representation.

        Input:
        - whiteState (list): List representing the state of white pieces.

        Returns:
        - stringState (str): String representation of the white pieces' state.
        """
        wkState = self.getPieceState(whiteState, 6)
        wrState = self.getPieceState(whiteState, 2)
        stringState = str(wkState[0]) + "," + str(wkState[1]) + ","
        if wrState is not None:
            stringState += str(wrState[0]) + "," + str(wrState[1])

        return stringState



    def stringToState(self, stringWhiteState):
        """
        Convert a string representation of white pieces' state to a list.

        Input:
        - stringWhiteState (str): String representation of the white pieces' state.

        Returns:
        - whiteState (list): List representing the state of white pieces.
        """
        whiteState = []
        whiteState.append([int(stringWhiteState[0]), int(stringWhiteState[2]), 6])
        if len(stringWhiteState) > 4:
            whiteState.append([int(stringWhiteState[4]), int(stringWhiteState[6]), 2])

        return whiteState
    



    def reconstructPath(self, initialState):
        """
        Reconstruct the path of moves based on the initial state using Q-values.

        Input:
        - initialState (list): Initial state of the chessboard. eg [[7, 0, 2], [7, 4, 6]]

        Returns:
        - path (list): List of states representing the sequence of moves.
        """
        currentState = initialState
        currentString = self.stateToString(initialState)
        checkMate = False
        self.chess.board.print_board()

        # Add the initial state to the path
        path = [initialState]
        while not checkMate:
            currentDict = self.qTable[currentString]
            maxQ = -100000
            maxState = None

            # Check which is the next state with the highest Q-value
            for stateString in currentDict.keys():
                qValue = currentDict[stateString]
                if maxQ < qValue:
                    maxQ = qValue
                    maxState = stateString

            state = self.stringToState(maxState)
            # When we get it, add it to the path
            path.append(state)
            movement = self.getMovement(currentState, state)
            # Make the corresponding movement
            self.chess.move(movement[0], movement[1])
            self.chess.board.print_board()
            currentString = maxState
            currentState = state

            # When it gets to checkmate, the execution is over
            if self.isCheckMate(state):
                checkMate = True

        print("Sequence of moves: ", path)

    def copyState(self, state):
        
        copyState = []
        for piece in state:
            copyState.append(piece.copy())
        return copyState
        
    def isVisitedSituation(self, color, mystate):
        
        if (len(self.listVisitedSituations) > 0):
            perm_state = list(permutations(mystate))

            isVisited = False
            for j in range(len(perm_state)):

                for k in range(len(self.listVisitedSituations)):
                    if self.isSameState(list(perm_state[j]), self.listVisitedSituations.__getitem__(k)[1]) and color == \
                            self.listVisitedSituations.__getitem__(k)[0]:
                        isVisited = True

            return isVisited
        else:
            return False


    def getCurrentStateW(self):

        return self.myCurrentStateW

    def getListNextStatesW(self, myState):

        self.chess.boardSim.getListNextStatesW(myState)
        self.listNextStates = self.chess.boardSim.listNextStates.copy()

        return self.listNextStates

    def getListNextStatesB(self, myState):
        self.chess.boardSim.getListNextStatesB(myState)
        self.listNextStates = self.chess.boardSim.listNextStates.copy()

        return self.listNextStates

    def isSameState(self, a, b):

        isSameState1 = True
        # a and b are lists
        for k in range(len(a)):

            if a[k] not in b:
                isSameState1 = False

        isSameState2 = True
        # a and b are lists
        for k in range(len(b)):

            if b[k] not in a:
                isSameState2 = False

        isSameState = isSameState1 and isSameState2
        return isSameState

    def isVisited(self, mystate):

        if (len(self.listVisitedStates) > 0):
            perm_state = list(permutations(mystate))

            isVisited = False
            for j in range(len(perm_state)):

                for k in range(len(self.listVisitedStates)):

                    if self.isSameState(list(perm_state[j]), self.listVisitedStates[k]):
                        isVisited = True

            return isVisited
        else:
            return False

    def isWatchedBk(self, currentState):

        self.newBoardSim(currentState)

        bkPosition = self.getPieceState(currentState, 12)[0:2]
        wkState = self.getPieceState(currentState, 6)
        wrState = self.getPieceState(currentState, 2)

        # Si les negres maten el rei blanc, no és una configuració correcta
        if wkState == None:
            return False
        # Mirem les possibles posicions del rei blanc i mirem si en alguna pot "matar" al rei negre
        for wkPosition in self.getNextPositions(wkState):
            if bkPosition == wkPosition:
                # Tindríem un checkMate
                return True
        if wrState != None:
            # Mirem les possibles posicions de la torre blanca i mirem si en alguna pot "matar" al rei negre
            for wrPosition in self.getNextPositions(wrState):
                if bkPosition == wrPosition:
                    return True
        return False
        


    def isWatchedWk(self, currentState):
        self.newBoardSim(currentState)

        wkPosition = self.getPieceState(currentState, 6)[0:2]
        bkState = self.getPieceState(currentState, 12)
        brState = self.getPieceState(currentState, 8)

        # If whites kill the black king , it is not a correct configuration
        if bkState == None:
            return False
        # We check all possible positions for the black king, and chck if in any of them it may kill the white king
        for bkPosition in self.getNextPositions(bkState):
            if wkPosition == bkPosition:
                # That would be checkMate
                return True
        if brState != None:
            # We check the possible positions of the black tower, and we chck if in any o them it may killt he white king
            for brPosition in self.getNextPositions(brState):
                if wkPosition == brPosition:
                    return True

        return False



    def newBoardSim(self, listStates):
        # We create a  new boardSim
        TA = np.zeros((8, 8))
        for state in listStates:

            TA[state[0]][state[1]] = state[2]

        self.chess.newBoardSim(TA)

    def getPieceState(self, state, piece):
        pieceState = None
        for i in state:
            if i[2] == piece:
                pieceState = i
                break
        return pieceState

    def getCurrentState(self):
        listStates = []
        for i in self.chess.board.currentStateW:
            listStates.append(i)
        for j in self.chess.board.currentStateB:
            listStates.append(j)
        return listStates

    def getNextPositions(self, state):
        # Given a state, we check the next possible states
        # From these, we return a list with position, i.e., [row, column]
        if state == None:
            return None
        if state[2] > 6:
            nextStates = self.getListNextStatesB([state])
        else:
            nextStates = self.getListNextStatesW([state])
        nextPositions = []
        for i in nextStates:
            nextPositions.append(i[0][0:2])
        return nextPositions

    def getWhiteState(self, currentState):
        whiteState = []
        wkState = self.getPieceState(currentState, 6)
        whiteState.append(wkState)
        wrState = self.getPieceState(currentState, 2)
        if wrState != None:
            whiteState.append(wrState)
        return whiteState

    def getBlackState(self, currentState):
        blackState = []
        bkState = self.getPieceState(currentState, 12)
        blackState.append(bkState)
        brState = self.getPieceState(currentState, 8)
        if brState != None:
            blackState.append(brState)
        return blackState

    def getMovement(self, state, nextState):
        # Given a state and a successor state, return the postiion of the piece that has been moved in both states
        pieceState = None
        pieceNextState = None
        for piece in state:
            if piece not in nextState:
                movedPiece = piece[2]
                pieceNext = self.getPieceState(nextState, movedPiece)
                if pieceNext != None:
                    pieceState = piece
                    pieceNextState = pieceNext
                    break

        return [pieceState, pieceNextState]

    def heuristica(self, currentState, color):
        #In this method, we calculate the heuristics for both the whites and black ones
        #The value calculated here is for the whites, 
        # but finally from verything, as a function of the color parameter, we multiply the result by -1
        value = 0

        bkState = self.getPieceState(currentState, 12)
        wkState = self.getPieceState(currentState, 6)
        wrState = self.getPieceState(currentState, 2)
        brState = self.getPieceState(currentState, 8)

        filaBk = bkState[0]
        columnaBk = bkState[1]
        filaWk = wkState[0]
        columnaWk = wkState[1]

        if wrState != None:
            filaWr = wrState[0]
            columnaWr = wrState[1]
        if brState != None:
            filaBr = brState[0]
            columnaBr = brState[1]

        # We check if they killed the black tower
        if brState == None:
            value += 50
            fila = abs(filaBk - filaWk)
            columna = abs(columnaWk - columnaBk)
            distReis = min(fila, columna) + abs(fila - columna)
            if distReis >= 3 and wrState != None:
                filaR = abs(filaBk - filaWr)
                columnaR = abs(columnaWr - columnaBk)
                value += (min(filaR, columnaR) + abs(filaR - columnaR))/10
            # If we are white white, the closer our king from the oponent, the better
            # we substract 7 to the distance between the two kings, since the max distance they can be at in a board is 7 moves
            value += (7 - distReis)
            # If they black king is against a wall, we prioritize him to be at a corner, precisely to corner him
            if bkState[0] == 0 or bkState[0] == 7 or bkState[1] == 0 or bkState[1] == 7:
                value += (abs(filaBk - 3.5) + abs(columnaBk - 3.5)) * 10
            #If not, we will only prioritize that he approahces the wall, to be able to approach the check mate
            else:
                value += (max(abs(filaBk - 3.5), abs(columnaBk - 3.5))) * 10

        # They killed the black tower. Within this method, we consider the same conditions than in the previous condition
        # Within this method we consider the same conditions than in the previous section, but now with reversed values.
        if wrState == None:
            value += -50
            fila = abs(filaBk - filaWk)
            columna = abs(columnaWk - columnaBk)
            distReis = min(fila, columna) + abs(fila - columna)

            if distReis >= 3 and brState != None:
                filaR = abs(filaWk - filaBr)
                columnaR = abs(columnaBr - columnaWk)
                value -= (min(filaR, columnaR) + abs(filaR - columnaR)) / 10
            # If we are white, the close we have our king from the oponent, the better
            # If we substract 7 to the distance between both kings, as this is the max distance they can be at in a chess board
            value += (-7 + distReis)

            if wkState[0] == 0 or wkState[0] == 7 or wkState[1] == 0 or wkState[1] == 7:
                value -= (abs(filaWk - 3.5) + abs(columnaWk - 3.5)) * 10
            else:
                value -= (max(abs(filaWk - 3.5), abs(columnaWk - 3.5))) * 10

        # We are checking blacks
        if self.isWatchedBk(currentState):
            value += 20

        # We are checking whites
        if self.isWatchedWk(currentState):
            value += -20

        # If black, values are negative, otherwise positive
        if not color:
            value = (-1) * value

        return value


    def minimaxGame(self, depthWhite,depthBlack, ):
        
        currentState = self.getCurrentState()     


        print("\n \n \n \n EMPIEZA LA PARTIDA : \n")
        aichess.chess.boardSim.print_board()
        siguienteEstado = currentState

        turno = 0
        
        while True:

            turno += 1

            
            print("-------------------------")
            print(f"TURNO {turno} BLANCO")
            # mini max decision blanco
            decisionBlanca , siguienteEstado , heuristica = self.miniMaxDecision(siguienteEstado, depthWhite , 1)
            aichess.chess.boardSim.print_board()
            print(f"El movimiento blanco ha sido de {decisionBlanca[0]} a {decisionBlanca[1]} " )
            print("-------------------------")

            if heuristica == float('inf') or heuristica == float('-inf'):
                print("FIN DE LA PARTIDA TABLAS POR REPETICION  O AHOGADO")
                return


            if heuristica > 1000 or heuristica < -1000: # el blanco no tenia movimientos
                print("FIN DE LA PARTIDA GANA EL NEGRO")
                return
            

            print("-------------------------")
            print(f"TURNO {turno} NEGRO")
            # mini max decision negro
            decisionNegra , siguienteEstado , heuristica = self.miniMaxDecision(siguienteEstado, depthBlack , 0)
            aichess.chess.boardSim.print_board()
            print(f"El movimiento negro ha sido de {decisionNegra[0]} a {decisionNegra[1]} " )
            print("-------------------------")


            if heuristica == float('-inf') or heuristica == float('inf'):
                print("FIN DE LA PARTIDA TABLAS POR REPETICION O AHOGADO")

                return
            
            if heuristica > 1000 or heuristica < -1000: # # el negro no tenia movimientos
                print("FIN DE LA PARTIDA GANA EL BLANCO")
                return
            
            









        # Your code here

    def miniMaxDecision(self, estat , depth , color):

        print(f"Llamamos al max value con estat - {estat} | depth - {depth} | color - {color}")

        valorOptimo = self.getMaxValue(estat, depth , color, color)

        Heuristica = valorOptimo[0]
        SiguienteEstado = valorOptimo[1]

        MovimientoRealizado = self.getMovement(estat,valorOptimo[1])
        self.newBoardSim(SiguienteEstado)

        estadoAañadir = copy.deepcopy(SiguienteEstado)
        estadoOrdenado = sorted(estadoAañadir, key=lambda x: x[2])
        estadoOrdenado.append(color)
        self.listVisitedStates.append(copy.deepcopy(estadoOrdenado))   # Lo marcamos como visitado

        if Heuristica == float('inf') or Heuristica == float('-inf'):  # si es infinito es tablas 
            return MovimientoRealizado , SiguienteEstado , Heuristica 
        

        if MovimientoRealizado[1] == None: # si no hay movimiento pero no es infinito es victoria
            Heuristica = 8888888888888888888888

        return MovimientoRealizado , SiguienteEstado , Heuristica
       



    def getMinValue(self, estat , depth , color, jugador ):    # buscamos el sucesor con valor MINIMO


        self.newBoardSim(estat)
        colorOriginal = color


        if depth == 0:
            return ( self.heuristica(estat, jugador)  , None ) 
            # llegamos a la profundidad máxima, devolvemos la heurística ( la profundidad hace que este sea un nodo terminal )


        if color:
            color = 0
            listaNextStates = self.getListNextStatesW(estat)
        else:
            color = 1
            listaNextStates = self.getListNextStatesB(estat)

        random.shuffle(listaNextStates) # añadir factor de aleatoriedad ( Sigue siendo optimo, pero pueden haber varios )

        valor = float('inf')
        valorAnterior = valor  # variable para ver si hay algun movimiento legal

        estadoOriginal = estat

        noHayMate = False 
        posibleMate = False # variables para detectar final de partida ( jaque mate o tablas )

        for sucesor in listaNextStates:

            copiaSucesor = copy.deepcopy(sucesor)
            sucesorOrdenado = sorted(copiaSucesor, key=lambda x: x[2])
            sucesorOrdenado.append(colorOriginal)

            if sucesorOrdenado not in self.listVisitedStates:

                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original
                piezaMovida = self.getMovement(estadoOriginal,sucesor) # ver el movimiento ejecutado


                if piezaMovida[0] == None or piezaMovida[1] == None: # si no es posible pasamos
                    continue

                if self.isWatchedWk(estadoOriginal) and self.isWatchedWk(sucesor) : # si el rey estaba en jaque y sigue en jaque hay posible mate
                    posibleMate = True
                    continue

                if self.isWatchedBk(estadoOriginal) and self.isWatchedBk(sucesor): # posible mate blanco
                    posibleMate = True
                    continue

                if not color: # JUGADOR BLANCO el color esta cambiado !
                    if self.isWatchedWk(sucesor) :
                        continue # si un movimiento blanco hace que el rey blanco siga en jaque lo paso
                
                else: # mueve el negro
                    if self.isWatchedBk(sucesor):
                        continue # si un movimiento negro hace que el rey negro siga en jaque lo paso


                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original ( el iswatched lo cambia )
                self.chess.moveSim( piezaMovida[0] , piezaMovida[1] ,  verbose=False )


                estat = copy.deepcopy(estadoOriginal)
                NoRey = False

                for tablero in estadoOriginal: # bucle para tener un estado con la pieza SOLO movida ( aqui no eliminamos la comida )

                    if piezaMovida[1][2] == tablero[2]: # si tenemos la misma pieza en diferente posicion la cambiamos
                        estat.remove(tablero)   #quitamos su estado anterior
                        estat.append(piezaMovida[1])  # ponemos la pieza en el nuevo lugar 
                        sucesor = estat
                        break # deberia de haber solo 1 movimiento


                for tablero in estat:  # bucle para eliminar una pieza 

                    if piezaMovida[1][0] == tablero[0] and piezaMovida[1][1] == tablero[1] and piezaMovida[1][2] != tablero[2]: # quitamos la pieza comida
                        estat.remove(tablero) # pieza movida ya tiene el color del jugador, siendo tablero la comida

                        if tablero[2] == 6 or tablero[2] == 12: # si se come al rey esta mal, pasamos 
                            NoRey = True
                            break

                        sucesor = estat
                        break # solo se come 1 pieza por turno
                
                if NoRey: # si nos hemos comido al rey pasamos
                    continue


                valor = min(valor, self.getMaxValue( sucesor , depth - 1, color, jugador)[0] )  # cogemos el maximo de los estados sucesores 


                if valor != valorAnterior :  # si valor da menos que antes cambiamos el camino a donde va
                    sucesorDelCamino = sucesor
                    noHayMate = True # esta variable en true significa que hay un movimiento legal posible haciendo que la partida siga

                valorAnterior = valor

        if posibleMate and not noHayMate: # si esta en jaque y no tiene movimientos
            return ( 500 , estadoOriginal )  # heuristica ganadora 
        
        if not noHayMate: # si no se guarda ningun sucesor es que el negro se queda sin movimientos sin estar en jaque
            return ( float('-inf') , estadoOriginal )

        return ( valor , sucesorDelCamino )  # devolvemos heuristica y estadoSucesor
    



    def getMaxValue(self, estat , depth , color, jugador):  # buscamos el sucesor con valor MAXIMO

        self.newBoardSim(estat)  #  ponemos el tablero con el estado recibido
        colorOriginal = color

        if depth == 0:  
            return ( self.heuristica(estat, jugador) , None )  # llegamos a la profundidad máxima

        if color:
            color = 0
            listaNextStates = self.getListNextStatesW(estat)
        else:
            color = 1
            listaNextStates = self.getListNextStatesB(estat)

        random.shuffle(listaNextStates) # añadir factor de aleatoriedad ( Sigue siendo optimo, pero pueden haber varios )


        valor = float('-inf')
        valorAnterior = valor # variable para ver si hay algun movimiento legal

        estadoOriginal = estat

        noHayMate = False
        posibleMate = False # variables para detectar final de partida


        for sucesor in listaNextStates:

            copiaSucesor = copy.deepcopy(sucesor)
            sucesorOrdenado = sorted(copiaSucesor, key=lambda x: x[2])
            sucesorOrdenado.append(colorOriginal)

            if sucesorOrdenado not in self.listVisitedStates:


                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original
                piezaMovida = self.getMovement(estadoOriginal,sucesor) # ver el movimiento ejecutado

                if piezaMovida[0] == None or piezaMovida[1] == None: # si no es posible pasamos
                    continue
                
                if self.isWatchedWk(estadoOriginal) and self.isWatchedWk(sucesor) : # si el rey estaba en jaque y sigue en jaque hay posible mate
                    posibleMate = True
                    continue

                if self.isWatchedBk(estadoOriginal) and self.isWatchedBk(sucesor): # posible mate blanco
                    posibleMate = True
                    continue

                if not color: # JUGADOR BLANCO el color esta cambiado !
                    if self.isWatchedWk(sucesor) :
                        continue # si un movimiento blanco hace que el rey blanco siga en jaque lo paso
                
                else: # mueve el negro
                    if self.isWatchedBk(sucesor):
                        continue # si un movimiento negro hace que el rey negro siga en jaque lo paso

                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original ( el iswatched lo cambia )
                self.chess.moveSim( piezaMovida[0] , piezaMovida[1] , verbose=False )
                                
                estat = copy.deepcopy(estadoOriginal)
                NoRey = False

                for tablero in estadoOriginal: # bucle para tener un estado con la pieza SOLO movida ( aqui no eliminamos la comida )

                    if piezaMovida[1][2] == tablero[2]: # si tenemos la misma pieza en diferente posicion la cambiamos
                        estat.remove(tablero)   #quitamos su estado anterior
                        estat.append(piezaMovida[1])  # ponemos la pieza en el nuevo lugar 
                        sucesor = estat
                        break # deberia de haber solo 1 movimiento


                for tablero in estat:  # bucle para eliminar una pieza 

                    if piezaMovida[1][0] == tablero[0] and piezaMovida[1][1] == tablero[1] and piezaMovida[1][2] != tablero[2]: # quitamos la pieza comida
                        estat.remove(tablero) # pieza movida ya tiene el color del jugador, siendo tablero la comida

                        if tablero[2] == 6 or tablero[2] == 12: # si se come al rey esta mal, pasamos 
                            NoRey = True
                            break

                        sucesor = estat
                        break # solo se come 1 pieza por turno

                if NoRey: # si nos hemos comido al rey pasamos
                    continue

                valor = max(valor, self.getMinValue( sucesor , depth - 1, color, jugador)[0] )  # cogemos el maximo de los estados sucesores 

                if valor != valorAnterior : # si valor da menos que antes cambiamos el camino a donde va

                    sucesorDelCamino = sucesor
                    noHayMate = True # esta variable en true significa que hay un movimiento legal posible haciendo que la partida siga

                valorAnterior = valor

        if posibleMate and not noHayMate:
            return ( -500 , estadoOriginal )  
            # no hay sucesores posibles donde no sea mate ( devolvemos heuristica para dictar que el jugador esta en la peor posicion )

        if not noHayMate: # si no se guarda ningun sucesor
            return ( float('inf') , estadoOriginal ) # no hay ningun sucesor y no hay mate, saltamos
        
        return ( valor , sucesorDelCamino ) 
        











































































    def alphaBetaPoda(self, depthWhite,depthBlack, simulate=False):
        
        currentState = self.getCurrentState()     

      
        print("\n \n \n \n EMPIEZA LA PARTIDA : \n")
        aichess.chess.boardSim.print_board()
        siguienteEstado = currentState

        turno = 0
        
        while True:

            turno += 1
    
            print("-------------------------")
            print(f"TURNO {turno} BLANCO")
            # mini max decision blanco
            decisionBlanca , siguienteEstado , heuristica = self.alphaBetaDecision(siguienteEstado, depthWhite , 1)
            #decisionBlanca , siguienteEstado , heuristica = self.miniMaxDecision(siguienteEstado, depthWhite , 1) # black only alpha beta

            aichess.chess.boardSim.print_board()
            print(f"El movimiento blanco ha sido de {decisionBlanca[0]} a {decisionBlanca[1]} " )
            print("-------------------------")

            if heuristica == float('-inf') or heuristica == float('inf'): # si devuelve inf son tablas 
                print("FIN DE LA PARTIDA TABLAS POR REPETICION O AHOGADO")
                return


            if heuristica > 1000 or heuristica < -1000: # el blanco no tenia movimientos
                print("FIN DE LA PARTIDA GANA EL NEGRO")
                return
            

            print("-------------------------")
            print(f"TURNO {turno} NEGRO")
            # mini max decision negro
            decisionNegra , siguienteEstado , heuristica = self.alphaBetaDecision(siguienteEstado, depthBlack , 0)
            aichess.chess.boardSim.print_board()
            print(f"El movimiento negro ha sido de {decisionNegra[0]} a {decisionNegra[1]} " )
            print("-------------------------")


            if heuristica == float('-inf') or heuristica == float('inf'): # si devuelve inf son tablas 
                print("FIN DE LA PARTIDA TABLAS POR REPETICION O AHOGADO")
                return
            

            if heuristica > 1000 or heuristica < -1000: # # el negro no tenia movimientos
                print("FIN DE LA PARTIDA GANA EL BLANCO")
                return
            
            




    def customMinimaxAndAlphaBeta(self, depthWhite,depthBlack, simulate=False):
        
        currentState = self.getCurrentState()     

      
        print("\n \n \n \n EMPIEZA LA PARTIDA : \n")
        aichess.chess.boardSim.print_board()
        siguienteEstado = currentState

        turno = 0
        
        while True:

            turno += 1
    
            print("-------------------------")
            print(f"TURNO {turno} BLANCO")
            # mini max decision blanco
            decisionBlanca , siguienteEstado , heuristica = self.miniMaxDecision(siguienteEstado, depthWhite , 1)
            #decisionBlanca , siguienteEstado , heuristica = self.miniMaxDecision(siguienteEstado, depthWhite , 1) # black only alpha beta

            aichess.chess.boardSim.print_board()
            print(f"El movimiento blanco ha sido de {decisionBlanca[0]} a {decisionBlanca[1]} " )
            print("-------------------------")

            if heuristica == float('-inf') or heuristica == float('inf'): # si devuelve inf son tablas 
                print("FIN DE LA PARTIDA TABLAS POR REPETICION O AHOGADO")
                return


            if heuristica > 1000 or heuristica < -1000: # el blanco no tenia movimientos
                print("FIN DE LA PARTIDA GANA EL NEGRO")
                return
            

            print("-------------------------")
            print(f"TURNO {turno} NEGRO")
            # mini max decision negro
            decisionNegra , siguienteEstado , heuristica = self.alphaBetaDecision(siguienteEstado, depthBlack , 0)
            aichess.chess.boardSim.print_board()
            print(f"El movimiento negro ha sido de {decisionNegra[0]} a {decisionNegra[1]} " )
            print("-------------------------")


            if heuristica == float('-inf') or heuristica == float('inf'): # si devuelve inf son tablas 
                print("FIN DE LA PARTIDA TABLAS POR REPETICION O AHOGADO")
                return
            

            if heuristica > 1000 or heuristica < -1000: # # el negro no tenia movimientos
                print("FIN DE LA PARTIDA GANA EL BLANCO")
                return







        # Your code here

    def alphaBetaDecision(self, estat , depth , color):

        #estat = sorted(estat, key=lambda x : x[2])
        #print(f"Llamamos al alphaBeta con estat - {estat} | depth - {depth} | color - {color}")

        alpha = float("-inf") 
        beta = float("inf")

        valorOptimo = self.getMaxValueALPHA(estat, depth , color, color , alpha , beta)

        Heuristica = valorOptimo[0]
        SiguienteEstado = valorOptimo[1]
        MovimientoRealizado = self.getMovement(estat,valorOptimo[1])
        self.newBoardSim(SiguienteEstado)

        #estadoAañadir = copy.deepcopy(SiguienteEstado)
        estadoOrdenado = sorted(SiguienteEstado, key=lambda x: x[2])
        estadoOrdenado.append(color)
        self.listVisitedStates.append(estadoOrdenado)
        #self.listVisitedStates.append(copy.deepcopy(estadoOrdenado))   # Lo marcamos como visitado

        if Heuristica == float('inf') or Heuristica == float('-inf'):  # si es infinito es tablas 
            return MovimientoRealizado , SiguienteEstado , Heuristica 
        
        if MovimientoRealizado[1] == None:
            Heuristica = 8888888888888888888888

        return MovimientoRealizado , SiguienteEstado , Heuristica


    def getMinValueALPHA(self, estat , depth , color, jugador , alpha , beta):    # buscamos el sucesor con valor MINIMO


        self.newBoardSim(estat)
        colorOriginal = color


        if depth == 0:
            return ( self.heuristica(estat, jugador)  , None ) 
            # llegamos a la profundidad máxima, devolvemos la heurística ( la profundidad hace que este sea un nodo terminal )


        if color:
            color = 0
            listaNextStates = self.getListNextStatesW(estat)
        else:
            color = 1
            listaNextStates = self.getListNextStatesB(estat)

        #random.shuffle(listaNextStates) # añadir factor de aleatoriedad ( Sigue siendo optimo, pero pueden haber varios )

        valor = float('inf')
        valorAnterior = valor  # variable para ver si hay algun movimiento legal

        estadoOriginal = estat

        noHayMate = False 
        posibleMate = False # variables para detectar final de partida ( jaque mate o tablas )

        for sucesor in listaNextStates:

            copiaSucesor = copy.deepcopy(sucesor)
            sucesorOrdenado = sorted(copiaSucesor, key=lambda x: x[2])
            sucesorOrdenado.append(colorOriginal)

            if sucesorOrdenado not in self.listVisitedStates:

                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original
                piezaMovida = self.getMovement(estadoOriginal,sucesor) # ver el movimiento ejecutado


                if piezaMovida[0] == None or piezaMovida[1] == None: # si no es posible pasamos
                    continue

                if self.isWatchedWk(estadoOriginal) and self.isWatchedWk(sucesor) : # si el rey estaba en jaque y sigue en jaque hay posible mate
                    posibleMate = True
                    continue

                if self.isWatchedBk(estadoOriginal) and self.isWatchedBk(sucesor): # posible mate blanco
                    posibleMate = True
                    continue

                if not color: # JUGADOR BLANCO el color esta cambiado !
                    if self.isWatchedWk(sucesor) :
                        continue # si un movimiento blanco hace que el rey blanco siga en jaque lo paso
                
                else: # mueve el negro
                    if self.isWatchedBk(sucesor):
                        continue # si un movimiento negro hace que el rey negro siga en jaque lo paso


                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original ( el iswatched lo cambia )
                self.chess.moveSim( piezaMovida[0] , piezaMovida[1] ,  verbose=False )


                estat = copy.deepcopy(estadoOriginal)
                NoRey = False

                for tablero in estadoOriginal: # bucle para tener un estado con la pieza SOLO movida ( aqui no eliminamos la comida )

                    if piezaMovida[1][2] == tablero[2]: # si tenemos la misma pieza en diferente posicion la cambiamos
                        estat.remove(tablero)   #quitamos su estado anterior
                        estat.append(piezaMovida[1])  # ponemos la pieza en el nuevo lugar 
                        sucesor = estat
                        break # deberia de haber solo 1 movimiento


                for tablero in estat:  # bucle para eliminar una pieza 

                    if piezaMovida[1][0] == tablero[0] and piezaMovida[1][1] == tablero[1] and piezaMovida[1][2] != tablero[2]: # quitamos la pieza comida
                        estat.remove(tablero) # pieza movida ya tiene el color del jugador, siendo tablero la comida

                        if tablero[2] == 6 or tablero[2] == 12: # si se come al rey esta mal, pasamos 
                            NoRey = True
                            break

                        sucesor = estat
                        break # solo se come 1 pieza por turno
                
                if NoRey: # si nos hemos comido al rey pasamos
                    continue


                valor = min(valor, self.getMaxValueALPHA( sucesor , depth - 1, color, jugador , alpha , beta)[0] )  # cogemos el maximo de los estados sucesores 

                if valor <= alpha:
                    #print("PODA MIN")
                    return ( valor , sucesor )
                
                beta = min( beta , valor )


                if valor != valorAnterior :  # si valor da menos que antes cambiamos el camino a donde va
                    sucesorDelCamino = sucesor
                    noHayMate = True # esta variable en true significa que hay un movimiento legal posible haciendo que la partida siga

                valorAnterior = valor

        if posibleMate and not noHayMate: # si esta en jaque y no tiene movimientos
            return ( 500 , estadoOriginal )  # heuristica ganadora 
        
        if not noHayMate: # si no se guarda ningun sucesor es que el negro se queda sin movimientos sin estar en jaque
            return ( float('-inf') , estadoOriginal )

        return ( valor , sucesorDelCamino )  # devolvemos heuristica y estadoSucesor
    


    def getMaxValueALPHA(self, estat , depth , color, jugador , alpha , beta):  # buscamos el sucesor con valor MAXIMO

        self.newBoardSim(estat)  #  ponemos el tablero con el estado recibido
        colorOriginal = color

        if depth == 0:  
            return ( self.heuristica(estat, jugador) , None )  # llegamos a la profundidad máxima

        if color:
            color = 0
            listaNextStates = self.getListNextStatesW(estat)
        else:
            color = 1
            listaNextStates = self.getListNextStatesB(estat)

        #random.shuffle(listaNextStates) # añadir factor de aleatoriedad ( Sigue siendo optimo, pero pueden haber varios )


        valor = float('-inf')
        valorAnterior = valor # variable para ver si hay algun movimiento legal

        estadoOriginal = estat

        noHayMate = False
        posibleMate = False # variables para detectar final de partida


        for sucesor in listaNextStates:

            copiaSucesor = copy.deepcopy(sucesor)
            sucesorOrdenado = sorted(copiaSucesor, key=lambda x: x[2])
            sucesorOrdenado.append(colorOriginal)

            if sucesorOrdenado not in self.listVisitedStates:


                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original
                piezaMovida = self.getMovement(estadoOriginal,sucesor) # ver el movimiento ejecutado

                if piezaMovida[0] == None or piezaMovida[1] == None: # si no es posible pasamos
                    continue
                
                if self.isWatchedWk(estadoOriginal) and self.isWatchedWk(sucesor) : # si el rey estaba en jaque y sigue en jaque hay posible mate
                    posibleMate = True
                    continue

                if self.isWatchedBk(estadoOriginal) and self.isWatchedBk(sucesor): # posible mate blanco
                    posibleMate = True
                    continue

                if not color: # JUGADOR BLANCO el color esta cambiado !
                    if self.isWatchedWk(sucesor) :
                        continue # si un movimiento blanco hace que el rey blanco siga en jaque lo paso
                
                else: # mueve el negro
                    if self.isWatchedBk(sucesor):
                        continue # si un movimiento negro hace que el rey negro siga en jaque lo paso

                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original ( el iswatched lo cambia )
                self.chess.moveSim( piezaMovida[0] , piezaMovida[1] , verbose=False )
                                
                estat = copy.deepcopy(estadoOriginal)
                NoRey = False

                for tablero in estadoOriginal: # bucle para tener un estado con la pieza SOLO movida ( aqui no eliminamos la comida )

                    if piezaMovida[1][2] == tablero[2]: # si tenemos la misma pieza en diferente posicion la cambiamos
                        estat.remove(tablero)   #quitamos su estado anterior
                        estat.append(piezaMovida[1])  # ponemos la pieza en el nuevo lugar 
                        sucesor = estat
                        break # deberia de haber solo 1 movimiento


                for tablero in estat:  # bucle para eliminar una pieza 

                    if piezaMovida[1][0] == tablero[0] and piezaMovida[1][1] == tablero[1] and piezaMovida[1][2] != tablero[2]: # quitamos la pieza comida
                        estat.remove(tablero) # pieza movida ya tiene el color del jugador, siendo tablero la comida

                        if tablero[2] == 6 or tablero[2] == 12: # si se come al rey esta mal, pasamos 
                            NoRey = True
                            break

                        sucesor = estat
                        break # solo se come 1 pieza por turno

                if NoRey: # si nos hemos comido al rey pasamos
                    continue

                valor = max(valor, self.getMinValueALPHA( sucesor , depth - 1, color, jugador , alpha , beta)[0] )  # cogemos el maximo de los estados sucesores 

                if valor >= beta:
                    #print("PODA MAX")
                    return ( valor , sucesor )
                
                alpha = max( alpha , valor )

                if valor != valorAnterior : # si valor da menos que antes cambiamos el camino a donde va
                    sucesorDelCamino = sucesor
                    noHayMate = True # esta variable en true significa que hay un movimiento legal posible haciendo que la partida siga

                valorAnterior = valor

        if posibleMate and not noHayMate:
            return ( -500 , estadoOriginal )  
            # no hay sucesores posibles donde no sea mate ( devolvemos heuristica para dictar que el jugador esta en la peor posicion )

        if not noHayMate: # si no se guarda ningun sucesor
            return ( float('inf') , estadoOriginal ) # no hay ningun sucesor y no hay mate, saltamos
        

        return ( valor , sucesorDelCamino ) 
    























































    def expectimax(self, depthWhite, depthBlack):
        
        currentState = self.getCurrentState()     

      
        print("\n \n \n \n EMPIEZA LA PARTIDA : \n")
        aichess.chess.boardSim.print_board()
        siguienteEstado = currentState

        turno = 0
        
        while True:

            turno += 1
    
            print("-------------------------")
            print(f"TURNO {turno} BLANCO")
            # mini max decision blanco
            decisionBlanca , siguienteEstado , heuristica = self.ExpectiminimaxDecision(siguienteEstado, depthWhite , 1)
            #decisionBlanca , siguienteEstado , heuristica = self.miniMaxDecision(siguienteEstado, depthWhite , 1) # black only alpha beta

            aichess.chess.boardSim.print_board()
            print(f"El movimiento blanco ha sido de {decisionBlanca[0]} a {decisionBlanca[1]} " )
            print("-------------------------")

            if heuristica == float('-inf') or heuristica == float('inf'): # si devuelve inf son tablas 
                print("FIN DE LA PARTIDA TABLAS POR REPETICION O AHOGADO")
                return


            if heuristica > 1000 or heuristica < -1000: # el blanco no tenia movimientos
                print("FIN DE LA PARTIDA GANA EL NEGRO")
                print(f"heurisitca --> {heuristica}")
                return
            

            print("-------------------------")
            print(f"TURNO {turno} NEGRO")
            # mini max decision negro
            #decisionNegra , siguienteEstado , heuristica = self.alphaBetaDecision(siguienteEstado, depthBlack , 0)
            decisionNegra , siguienteEstado , heuristica = self.alphaBetaDecision(siguienteEstado, depthBlack , 0)
            aichess.chess.boardSim.print_board()
            print(f"El movimiento negro ha sido de {decisionNegra[0]} a {decisionNegra[1]} " )
            print("-------------------------")

            if heuristica == float('-inf') or heuristica == float('inf'): # si devuelve inf son tablas 
                print("FIN DE LA PARTIDA TABLAS POR REPETICION O AHOGADO")
                return


            if heuristica > 1000 or heuristica < -1000: # # el negro no tenia movimientos
                print("FIN DE LA PARTIDA GANA EL BLANCO")
                return
            
            








        # Your code here

    def ExpectiminimaxDecision(self, estat , depth , color):

        print(f"Llamamos al expectiminimax con estat - {estat} | depth - {depth} | color - {color}")

        valorOptimo = self.getMaxValueEXPECTIMAX(estat, depth , color, color)

        Heuristica = valorOptimo[0]
        SiguienteEstado = valorOptimo[1]
        MovimientoRealizado = self.getMovement(estat,valorOptimo[1])
        self.newBoardSim(SiguienteEstado)

        estadoAañadir = copy.deepcopy(SiguienteEstado)
        estadoOrdenado = sorted(estadoAañadir, key=lambda x: x[2])
        estadoOrdenado.append(color)
        self.listVisitedStates.append(copy.deepcopy(estadoOrdenado))   # Lo marcamos como visitado

        if Heuristica == float('inf') or Heuristica == float('-inf'):  # si es infinito es tablas 
            return MovimientoRealizado , SiguienteEstado , Heuristica 
        
        if MovimientoRealizado[1] == None:
            Heuristica = 8888888888888888888888

        return MovimientoRealizado , SiguienteEstado , Heuristica
    
        


    def getMaxValueEXPECTIMAX(self, estat , depth , color, jugador):  # buscamos el sucesor con valor MAXIMO

        self.newBoardSim(estat)  #  ponemos el tablero con el estado recibido
        colorOriginal = color

        if depth == 0:  
            return ( self.heuristica(estat, jugador) , None )  # llegamos a la profundidad máxima

        if color:
            color = 0
            listaNextStates = self.getListNextStatesW(estat)
        else:
            color = 1
            listaNextStates = self.getListNextStatesB(estat)

        random.shuffle(listaNextStates) # añadir factor de aleatoriedad ( Sigue siendo optimo, pero pueden haber varios )

        valor = float('-inf')
        valorAnterior = valor # variable para ver si hay algun movimiento legal

        estadoOriginal = estat

        noHayMate = False
        posibleMate = False # variables para detectar final de partida


        for sucesor in listaNextStates:

            copiaSucesor = copy.deepcopy(sucesor)
            sucesorOrdenado = sorted(copiaSucesor, key=lambda x: x[2])
            sucesorOrdenado.append(colorOriginal)

            if sucesorOrdenado not in self.listVisitedStates:


                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original
                piezaMovida = self.getMovement(estadoOriginal,sucesor) # ver el movimiento ejecutado

                if piezaMovida[0] == None or piezaMovida[1] == None: # si no es posible pasamos
                    continue
                
                if self.isWatchedWk(estadoOriginal) and self.isWatchedWk(sucesor) : # si el rey estaba en jaque y sigue en jaque hay posible mate
                    posibleMate = True
                    continue

                if self.isWatchedBk(estadoOriginal) and self.isWatchedBk(sucesor): # posible mate blanco
                    posibleMate = True
                    continue

                if not color: # JUGADOR BLANCO el color esta cambiado !
                    if self.isWatchedWk(sucesor) :
                        continue # si un movimiento blanco hace que el rey blanco siga en jaque lo paso
                
                else: # mueve el negro
                    if self.isWatchedBk(sucesor):
                        continue # si un movimiento negro hace que el rey negro siga en jaque lo paso

                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original ( el iswatched lo cambia )
                self.chess.moveSim( piezaMovida[0] , piezaMovida[1] , verbose=False )
                                
                estat = copy.deepcopy(estadoOriginal)
                NoRey = False

                for tablero in estadoOriginal: # bucle para tener un estado con la pieza SOLO movida ( aqui no eliminamos la comida )

                    if piezaMovida[1][2] == tablero[2]: # si tenemos la misma pieza en diferente posicion la cambiamos
                        estat.remove(tablero)   #quitamos su estado anterior
                        estat.append(piezaMovida[1])  # ponemos la pieza en el nuevo lugar 
                        sucesor = estat
                        break # deberia de haber solo 1 movimiento


                for tablero in estat:  # bucle para eliminar una pieza 

                    if piezaMovida[1][0] == tablero[0] and piezaMovida[1][1] == tablero[1] and piezaMovida[1][2] != tablero[2]: # quitamos la pieza comida
                        estat.remove(tablero) # pieza movida ya tiene el color del jugador, siendo tablero la comida

                        if tablero[2] == 6 or tablero[2] == 12: # si se come al rey esta mal, pasamos 
                            NoRey = True
                            break

                        sucesor = estat
                        break # solo se come 1 pieza por turno

                if NoRey: # si nos hemos comido al rey pasamos
                    continue

                valor = max(valor, self.getChanceValueEXPECTIMAX( sucesor , depth - 1, color, jugador)[0] )  # cogemos el maximo de los estados sucesores 


                if valor != valorAnterior : # si valor da menos que antes cambiamos el camino a donde va
                    sucesorDelCamino = sucesor
                    noHayMate = True # esta variable en true significa que hay un movimiento legal posible haciendo que la partida siga

                valorAnterior = valor

        if posibleMate and not noHayMate:
            return ( -500 , estadoOriginal )  
            # no hay sucesores posibles donde no sea mate ( devolvemos heuristica para dictar que el jugador esta en la peor posicion )

        if not noHayMate: # si no se guarda ningun sucesor
            return ( float('inf') , estadoOriginal ) # no hay ningun sucesor y no hay mate, saltamos
        
        return ( valor , sucesorDelCamino ) 


    def getChanceValueEXPECTIMAX(self, estat , depth , color, jugador ):    # buscamos el sucesor con valor MINIMO


        self.newBoardSim(estat)
        colorOriginal = color


        if depth == 0:
            return ( self.heuristica(estat, jugador)  , None ) 
            # llegamos a la profundidad máxima, devolvemos la heurística ( la profundidad hace que este sea un nodo terminal )


        if color:
            color = 0
            listaNextStates = self.getListNextStatesW(estat)
        else:
            color = 1
            listaNextStates = self.getListNextStatesB(estat)

        random.shuffle(listaNextStates) # añadir factor de aleatoriedad ( Sigue siendo optimo, pero pueden haber varios )

        valorRama = float('inf') # asignamos a infinito para detectar cuando no hay movimientos

        estadoOriginal = estat

        noHayMate = False 
        posibleMate = False # variables para detectar final de partida ( jaque mate o tablas )

        listaValues = []

        for sucesor in listaNextStates:

            copiaSucesor = copy.deepcopy(sucesor)
            sucesorOrdenado = sorted(copiaSucesor, key=lambda x: x[2])
            sucesorOrdenado.append(colorOriginal)

            if sucesorOrdenado not in self.listVisitedStates:

                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original
                piezaMovida = self.getMovement(estadoOriginal,sucesor) # ver el movimiento ejecutado


                if piezaMovida[0] == None or piezaMovida[1] == None: # si no es posible pasamos
                    continue

                if self.isWatchedWk(estadoOriginal) and self.isWatchedWk(sucesor) : # si el rey estaba en jaque y sigue en jaque hay posible mate
                    posibleMate = True
                    continue

                if self.isWatchedBk(estadoOriginal) and self.isWatchedBk(sucesor): # posible mate blanco
                    posibleMate = True
                    continue

                if not color: # JUGADOR BLANCO el color esta cambiado !
                    if self.isWatchedWk(sucesor) :
                        continue # si un movimiento blanco hace que el rey blanco siga en jaque lo paso
                
                else: # mueve el negro
                    if self.isWatchedBk(sucesor):
                        continue # si un movimiento negro hace que el rey negro siga en jaque lo paso


                self.newBoardSim(estadoOriginal)  # cada bucle que empieze con el estado original ( el iswatched lo cambia )
                self.chess.moveSim( piezaMovida[0] , piezaMovida[1] ,  verbose=False )


                estat = copy.deepcopy(estadoOriginal)
                NoRey = False

                for tablero in estadoOriginal: # bucle para tener un estado con la pieza SOLO movida ( aqui no eliminamos la comida )

                    if piezaMovida[1][2] == tablero[2]: # si tenemos la misma pieza en diferente posicion la cambiamos
                        estat.remove(tablero)   #quitamos su estado anterior
                        estat.append(piezaMovida[1])  # ponemos la pieza en el nuevo lugar 
                        sucesor = estat
                        break # deberia de haber solo 1 movimiento


                for tablero in estat:  # bucle para eliminar una pieza 

                    if piezaMovida[1][0] == tablero[0] and piezaMovida[1][1] == tablero[1] and piezaMovida[1][2] != tablero[2]: # quitamos la pieza comida
                        estat.remove(tablero) # pieza movida ya tiene el color del jugador, siendo tablero la comida

                        if tablero[2] == 6 or tablero[2] == 12: # si se come al rey esta mal, pasamos 
                            NoRey = True
                            break

                        sucesor = estat
                        break # solo se come 1 pieza por turno
                
                if NoRey: # si nos hemos comido al rey pasamos
                    continue


                #valor = min(valor, self.getMaxValue( sucesor , depth - 1, color, jugador)[0] )  # cogemos el maximo de los estados sucesores 
                
                valorRama = self.getMaxValueEXPECTIMAX( sucesor , depth - 1, color, jugador)[0]
                listaValues.append( valorRama )
        

        if len(listaValues) == 0:   # si no se ha visitado ningun estado fin de la partida
            return ( 500 , estadoOriginal )

        return ( self.calculateValue(listaValues) , estadoOriginal )  # devolvemos heuristica y estadoSucesor
    






































        

























        

    def mitjana(self, values):
        sum = 0
        N = len(values)
        for i in range(N):
            sum += values[i]

        return sum / N

    def desviacio(self, values, mitjana):
        sum = 0
        N = len(values)

        for i in range(N):
            sum += pow(values[i] - mitjana, 2)

        return pow(sum / N, 1 / 2)

    def calculateValue(self, values):
        
        if len(values) == 0:
            return 0
        mitjana = self.mitjana(values)
        desviacio = self.desviacio(values, mitjana)
        # If deviation is 0, we cannot standardize values, since they are all equal, thus probability willbe equiprobable
        if desviacio == 0:
            # We return another value
            return values[0]

        esperanca = 0
        sum = 0
        N = len(values)
        for i in range(N):
            #Normalize value, with mean and deviation - zcore
            normalizedValues = (values[i] - mitjana) / desviacio
            # make the values positive with function e^(-x), in which x is the standardized value
            positiveValue = pow(1 / math.e, normalizedValues)
            # Here we calculate the expected value, which in the end will be expected value/sum            
            # Our positiveValue/sum represent the probabilities for each value
            # The larger this value, the more likely
            esperanca += positiveValue * values[i]
            sum += positiveValue

        return esperanca / sum
     

if __name__ == "__main__":
    #   if len(sys.argv) < 2:
    #       sys.exit(usage())

    # intiialize board
    TA = np.zeros((8, 8))

    #Configuració inicial del taulell
    TA[7][0] = 2
    TA[7][4] = 6
    TA[0][7] = 8
    TA[0][4] = 12

    # initialise board
    print("stating AI chess... ")
    aichess = Aichess(TA, True)

    print("printing board")
    aichess.chess.boardSim.print_board()

    aichess.minimaxGame(2,2)  # blanco y negro minimax
    #aichess.alphaBetaPoda(1,5) # blanco y negro alpha beta 
    #aichess.customMinimaxAndAlphaBeta(4,4)

    #aichess.expectimax(3,3)  # blanco expectimax y  negro alpha beta

    if 0 : # just to travel to different codes
        
        aichess.miniMaxDecision()
        aichess.alphaBetaDecision()
        aichess.ExpectiminimaxDecision()

