

import random
import numpy as np


class Ejercicio1:
    def __init__(self, states, actions, rewards):

        self.states = states
        self.actions = actions

        if rewards == 'TODO1':
            # tabla de recompensas con todo -1

            self.rewards = {(estado, accion): -1 for estado in states for accion in actions}

            #self.rewards[((2,1),'U')] = float('-inf')
            self.rewards[((4,2),'U')] = 100 # llegar a la casilla (4,3)
            self.rewards[((3,3),'R')] = 100 # llegar a la casilla (4,3)
        
        else :

            # Recompensa personalizada (APARTADO B)

            self.rewards = {(estado, accion): None for estado in states for accion in actions}

            # (X,Y)

            self.rewards[((1,1),'U')] = -4  # (1,2)
            self.rewards[((1,2),'U')] = -3  # (1,3)
            
            self.rewards[((3,1),'U')] = -2  # (3,2)
            self.rewards[((3,2),'U')] = -1  # (3,3)

            self.rewards[((4,1),'U')] = -1  # (4,2)
            self.rewards[((4,2),'U')] = 100 # (4,3)

            self.rewards[((1,1),'R')] = -4  # (2,1)
            self.rewards[((2,1),'R')] = -3  # (3,1)
            self.rewards[((3,1),'R')] = -2  # (4,1)

            self.rewards[((2,1),'L')] = -5  # (1,1)
            self.rewards[((3,1),'L')] = -4  # (2,1)
            self.rewards[((4,1),'L')] = -3  # (3,1)

            self.rewards[((3,2),'R')] = -1  # (4,2)

            self.rewards[((4,2),'L')] = -2  # (3,2)


            self.rewards[((1,3),'R')] = -2  # (2,3)
            self.rewards[((2,3),'R')] = -1  # (3,3)
            self.rewards[((3,3),'R')] = 100 # (4,3)

            self.rewards[((2,3),'L')] = -3  # (1,3)
            self.rewards[((3,3),'L')] = -2  # (2,3)



        
        #self.QTabla = {(estado, accion): np.random.uniform(low=-0.1, high=0.1) for estado in states for accion in actions} 
        # # probar con valores cercanos a 0

        self.QTabla = {(estado, accion): 0 for estado in states for accion in actions}
    

    def proximoEstado(self,estado_tupla,accion):
        """
        Metodo que recibe un estado y una accion y devuelve :

            · -1 si no es posible
            · estado sucesor sí es posible

        """

        estado = list(estado_tupla)  # Copia a una lista

        if accion == 'U':
            estado[1] += 1
        if accion == 'L':
            estado[0] -= 1
        if accion == 'R':
            estado[0] += 1

        estado = tuple(estado)

        if estado not in self.states:
            return -1

        return estado 

    
    def Actualizar_Q_tabla(self, estado , gamma , alpha , epsilon , Drunken = False , ProbDrunken = None):
        """
        Actualizas la Q_tabla con metodo epsilon greedy
        """

        ExplorarOExplotar = random.uniform(0, 1)  # Para decimales

        if ExplorarOExplotar > epsilon: # explotar
 
            # las tuplas son hasheables  ((2, 1), 'R') Coger accion con valor maximo
            listaValores = [ (self.QTabla[(estado,accion)],accion) for accion in self.actions] 

            maximo = max( listaValores , key = lambda x : x[0])
            accion = maximo[1]

        else: # explorar

            accion = random.choice(self.actions)

        
        # Drunken sailor
        if Drunken:

            probabilidad = random.uniform(0, 1)

            if probabilidad > ProbDrunken:  # 1% de las veces se toma una acción aleatoria                

                accion = random.choice(self.actions)




        proximoEstado = self.proximoEstado(estado,accion)

        if proximoEstado == -1: # nos quedamos sin hacer ninguna accion
            return estado

        recompensa = self.rewards[(estado,accion)]
        

        valorQTablaPrevio = self.QTabla[(estado,accion)]
        valorQTablaMaximo = max( self.QTabla[(proximoEstado, posiblesAcciones)] for posiblesAcciones in self.actions )
        valorQTabla =  valorQTablaPrevio + alpha * ( recompensa + gamma * valorQTablaMaximo - valorQTablaPrevio )

        self.QTabla[(estado,accion)] = valorQTabla

        return proximoEstado
    




    def sumar_valores_QTabla(self):
        """
        Suma todos los valores en la QTabla para comprobar si sigue aprendiendo o no

        """
        total = sum(self.QTabla[(estado, accion)] for (estado, accion) in self.QTabla)


        return total







    def ejecutarNveces(self, estadoInicial , gamma , alpha , epsilon , isDrunken = False , ProbDrunken = None ):
        """
        Metodo Qlearning viendo las q table 0 , 70 , 140 y final
        """

        print("\nQ-table inicial :")
        self.ImprimirQTablaBonito()
        print("\nTablero movimientos óptimos :")
        self.dibujar_tablero_terminal(self.states, self.actions, self.QTabla)

        _ = input("\nenter para continuar")

        contador = 0
        SumaQtable = 0

        proximoEstado = estadoInicial
        haConvergido = False

        N_episodio = 0
        N_moves = 0

        while not haConvergido:

            if N_moves == 70 or N_moves == 140 :
                print(f"Q-table en el movimiento {N_moves} ")
                self.ImprimirQTablaBonito()
                print("\nTablero movimientos óptimos :")
                self.dibujar_tablero_terminal(self.states, self.actions, self.QTabla)
                N_moves += 1

                _ = input("\nenter para continuar")

            estadoAnterior = proximoEstado
            proximoEstado = self.Actualizar_Q_tabla(proximoEstado,gamma,alpha,epsilon, isDrunken , ProbDrunken)

            if estadoAnterior != proximoEstado: # si no hace accion ilegal ( se mueve ) sumamos 1 movimiento
                N_moves += 1

            if proximoEstado == (4,3): # estado terminal

                SumaQtableDespues = self.sumar_valores_QTabla()
                N_episodio += 1

                if abs(SumaQtableDespues - SumaQtable) < 10e-12 : # si la tabla anterior y actual han cambiado menos que el umbral
                    contador += 1

                    if contador == 10:  # si pasa 10 veces seguidas significa que ya no hay cambios significativos
                        haConvergido = True
                
                else :
                    contador = 0


                SumaQtable = SumaQtableDespues
                proximoEstado = estadoInicial

        
        print("Q-table final ")
        self.ImprimirQTablaBonito()
        print("\nTablero movimientos óptimos :")
        self.dibujar_tablero_terminal(self.states, self.actions, self.QTabla)
        print(f"\nNumero iteraciones necesarias para aprender : {N_episodio}\n")        
        _ = input("\nenter para continuar")




        return N_episodio
    

    def ejecutarNvecesSinPrints(self, estadoInicial , gamma , alpha , epsilon , isDrunken = False , ProbDrunken = None ):
        """
        Metodo para ver el numero de episodios sin ver las qtable de por medio ( igual al anterior )
        """

        contador = 0
        SumaQtable = 0

        proximoEstado = estadoInicial
        haConvergido = False

        N_episodio = 0
        N_moves = 0

        while not haConvergido:

            estadoAnterior = proximoEstado

            #Actualizamos la tabla
            proximoEstado = self.Actualizar_Q_tabla(proximoEstado,gamma,alpha,epsilon, isDrunken , ProbDrunken)

            if estadoAnterior != proximoEstado:
                N_moves += 1


            if proximoEstado == (4,3): # llega al estado terminal

                SumaQtableDespues = self.sumar_valores_QTabla()
                N_episodio += 1

                if abs(SumaQtableDespues - SumaQtable) < 10e-12 : # si la tabla anterior y actual han cambiado menos que el umbral
                    contador += 1

                    if contador == 10: # si pasa 10 veces seguidas significa que ya no hay cambios significativos
                        haConvergido = True
                
                else :
                    contador = 0


                SumaQtable = SumaQtableDespues
                proximoEstado = estadoInicial

        return N_episodio
    

    
    def ImprimirQTabla(self):
        for key, value in self.QTabla.items():
             print(f"Estado: {key[0]}, Acción: {key[1]} -> Q: {value}")


    def ImprimirQTablaBonito(self):
        
        # Imprimir los nombres de las acciones en la cabecera de la tabla
        print("   ", end="")
        for accion in self.actions:
            print(f"   {accion:<5}", end="")
        print()  # Nueva línea al final de la cabecera

        # Imprimir línea divisoria
        print("-" * (5 * len(self.actions) + 3))

        # Recorrer cada estado y mostrar los valores Q para cada acción
        for estado in self.states:
            print(f"{estado[0]},{estado[1]} |", end="")  # Imprimir el estado actual
            for accion in self.actions:
                q_valor = self.QTabla.get((estado, accion), 0)  # Obtener valor Q o 0 si no existe
                print(f"{q_valor:<5.2f}  ", end="")  # Formato alineado
            print()  # Nueva línea para el siguiente estado

    def dibujar_tablero_terminal(self,estados, actions, QTabla):
        """
        Dibuja el tablero en la terminal mostrando el mejor movimiento en cada posición.

        estados: Lista de estados (pares (x, y)).
        actions: Lista de acciones disponibles.
        QTabla: Diccionario con valores Q para cada (estado, acción).
        """
        # Definir el tamaño del tablero
        max_x = max(x for x, y in estados)
        max_y = max(y for x, y in estados)

        # Crear un grid para representar el tablero
        tablero = [['   ' for _ in range(max_x)] for _ in range(max_y)]

        # Marcar las posiciones especiales
        posiciones_inaccesibles = [(2, 2)]

        # Para cada estado válido, determinar la mejor acción
        for estado in estados:
            if estado in posiciones_inaccesibles:
                tablero[estado[1] - 1][estado[0] - 1] = ' X '
            else:
                # Extraer las acciones y sus valores Q
                acciones_estado = {accion: QTabla.get((estado, accion), float('-inf')) for accion in actions}
                
                # Seleccionar la acción con el máximo valor Q
                mejor_accion = max(acciones_estado, key=acciones_estado.get)

                # Actualizar el tablero con la mejor acción
                tablero[estado[1] - 1][estado[0] - 1] = f' {mejor_accion} '

        # Imprimir el tablero en la terminal
        for fila in reversed(tablero):  # Invertir para que y=1 esté abajo
            print(''.join(fila))






if __name__ == "__main__":

    Actions = ['R','L','U']  # no hay D en el ejemplo de clase
    # R - right
    # L - left
    # U - up

    # Rango de valores para i y j
    i_range = range(1, 5)  # i = [1, 4]
    j_range = range(1, 4)  # j = [1, 3]

    # Generar todos los posibles estados (i, j) como pares
    estados = [(i, j) for i in i_range for j in j_range if (i != 2 or j != 2)]



    _ = input("1A. Enter para ejecutar con recompensas -1 ")

    tablero1 = Ejercicio1(estados,Actions,'TODO1')  # Recompensa -1
    estado = (1,1)  # estado inicial
    gamma = 0.9
    alpha = 0.3
    epsilon = 0.4


    tablero1.ejecutarNveces(estado 
                            ,gamma = gamma
                            ,alpha = alpha
                            ,epsilon = epsilon
                            ,isDrunken = False
                            ,ProbDrunken = 0.99     )








    _ = input("1B. Enter para ejecutar con recompensas personalizadas")

    tablero2 = Ejercicio1(estados,Actions,'RECOMPENSASCUSTOM') # Recompensa mas accurate

    tablero2.ejecutarNveces(estado 
                            ,gamma = gamma
                            ,alpha = alpha
                            ,epsilon = epsilon
                            ,isDrunken = False
                            ,ProbDrunken = 0.99     )


    testear = input("1B Apartado 2. Enter para Comprobar ambas funciones de recompensa. [no] para skipear")

    if testear != 'no':

        Sum_Episodios_Funcion1 = 0 # recompensa -1
        Sum_Episodios_Funcion2 = 0 # recompensa heuristica 
        Sum_Episodios_Funcion3 = 0 # recompensa heuristica con menos gamma

        N_iteraciones = 1000

        tablero1 = Ejercicio1(estados,Actions,'TODO1')  # reiniciar su q table
        tablero2 = Ejercicio1(estados,Actions,'RECOMPENSASCUSTOM') # reiniciar su q table
        tablero3 = Ejercicio1(estados,Actions,'RECOMPENSASCUSTOM') # reiniciar su q table


        for i in range(N_iteraciones):

            Sum_Episodios_Funcion1 += tablero1.ejecutarNvecesSinPrints(estado 
                                ,gamma = gamma
                                ,alpha = alpha
                                ,epsilon = epsilon
                                ,isDrunken = False
                                ,ProbDrunken = 0.99     )
            
            Sum_Episodios_Funcion2 += tablero2.ejecutarNvecesSinPrints(estado 
                                ,gamma = gamma
                                ,alpha = alpha
                                ,epsilon = epsilon
                                ,isDrunken = False
                                ,ProbDrunken = 0.99     )
            
            Sum_Episodios_Funcion3 += tablero3.ejecutarNvecesSinPrints(estado 
                                ,gamma = 0.6
                                ,alpha = alpha
                                ,epsilon = epsilon
                                ,isDrunken = False
                                ,ProbDrunken = 0.99     )
            
            tablero1 = Ejercicio1(estados,Actions,'TODO1')  # reiniciar su q table
            tablero2 = Ejercicio1(estados,Actions,'RECOMPENSASCUSTOM') # reiniciar su q table
            tablero3 = Ejercicio1(estados,Actions,'RECOMPENSASCUSTOM') # reiniciar su q table


        print(f"Media de episodios de usar recompensa -1 = {Sum_Episodios_Funcion1/N_iteraciones}" )
        print(f"Media de episodios de usar recompensa heuristica = {Sum_Episodios_Funcion2/N_iteraciones}" )
        print(f"Media de episodios de usar recompensa heuristica con menos gamma= {Sum_Episodios_Funcion3/N_iteraciones}" )

















    _ = input("1C. Enter para ejecutar Drunken Sailor con recompensas personalizadas")

    tablero2 = Ejercicio1(estados,Actions,'RECOMPENSASCUSTOM') # Recompensa mas accurate

    tablero2.ejecutarNveces(estado 
                            ,gamma = 0.6
                            ,alpha = alpha
                            ,epsilon = epsilon
                            ,isDrunken = True
                            ,ProbDrunken = 0.99     )


    testear = input("1C Apartado 2. Enter para comprobar número de episodios medio para converger. [no] para skipear")

    if testear != 'no':

        Sum_Episodios_Drunken = 0 # recompensa heuristica drunked

        N_iteraciones = 1000

        tableroDrunked = Ejercicio1(estados,Actions,'RECOMPENSASCUSTOM') # Recompensa mas accurate

        for i in range(N_iteraciones):

            Sum_Episodios_Drunken += tableroDrunked.ejecutarNvecesSinPrints(estado 
                                ,gamma = 0.6
                                ,alpha = alpha
                                ,epsilon = epsilon
                                ,isDrunken = True
                                ,ProbDrunken = 0.99     )
            
            
            tableroDrunked = Ejercicio1(estados,Actions,'RECOMPENSASCUSTOM')  # reiniciar su q table

        print(f"\n\nMedia de episodios de usar recompensa heuristica 'Drunked' = {Sum_Episodios_Drunken/N_iteraciones}\n\n" )




