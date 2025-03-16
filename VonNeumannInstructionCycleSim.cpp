 /*
Liseth Natalia Rivera Córdoba - 2223510
Juan Manuel Moreno - 2417575
Juan David Lopez Vanegas - 2243077
Grosman Klein Garcia Valencia - 2340247
*/

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>


using namespace std;

// Definición de los registros
int accumulator = 0;
int MAR = 0;
int PC = 0;
int ALU = 0;
int theMostGreatest = 0;
string MDR = "";
string ICR = "";
string UC = "";

// Función para imprimir el estado de la memoria
/*
   Esta función recorre la memoria y muestra su contenido línea por línea,
   permitiendo visualizar el estado actual de la memoria principal.
*/
void printMainMemory(string* mainMemory, int size) {
    cout << "+--------+--------------------------------------+" << endl;
    cout << "| Línea  | Valor                                |" << endl;
    cout << "+--------+--------------------------------------+" << endl;
    for (int i = 0; i < (theMostGreatest+size); i++) {
        cout << "| " << setw(6) << i << " | " << setw(36) << left << mainMemory[i] << " |" << endl;
    }
    cout << "+--------+--------------------------------------+" << endl;

    /*
    for(int i = 0; i < (theMostGreatest+size); i++) {
        cout << "Linea " << i << ":" << mainMemory[i] <<endl;
    }
    */
}

//Función para obtener la dirección de memoria desde una línea de instrucción recibida como parámetro
int getAddress(string lineToAnalize) {
    int address;
    // Si la dirección tiene un solo dígito (D5), se obtiene restando '0' en ASCII
    if(lineToAnalize[6] == ' ') {
        /*
        En el codigo ascii los numeros no son representados con su valor. Ejemplo : el '1' en ascci es 49
        */
        address = lineToAnalize[5] - '0';
    }
    // Si tiene dos dígitos (D23), extrae la subcadena correspondiente y la convierte en entero
    else {
        address = stoi(lineToAnalize.substr(5,2));
    }
    return address;
}

// Función que extrae el segundo valor de la instrucción (En "SET D5 D12", obtiene el 12).
int getValue2(string lineToAnalize) {
    int value2;

    // Encontramos el inicio de la segunda dirección en la instrucción
    size_t startPos = lineToAnalize.find(' ', lineToAnalize.find(' ') + 1) + 1;

    // Buscamos el siguiente espacio para encontrar el final de la segunda dirección
    size_t endPos = lineToAnalize.find(' ', startPos);

    // Extraemos la subcadena que corresponde a la segunda dirección
    string valueStr = lineToAnalize.substr(startPos, endPos - startPos);

    // Convertimos la subcadena a un valor numérico
    value2 = stoi(valueStr);

    return value2;
}

// Función para obtener el tercer valor de una instrucción (En "ADD D31 D22 D4", obtiene 4)
int getValue3(string lineToAnalize) {
    int value3;
    
    // Determinar el punto de inicio para extraer la tercera dirección
    int startPos = lineToAnalize.find(' ', lineToAnalize.find(' ', lineToAnalize.find(' ') + 1) + 1) + 1;
    
    // Caso 1: Si la dirección es de un solo dígito, extraer directamente
    if (lineToAnalize[startPos + 1] == ' ' || lineToAnalize[startPos + 2] == ' ') {
        value3 = lineToAnalize[startPos] - '0';
    }
    // Caso 2: Si la dirección tiene dos dígitos se usa substr() para extraer ambos caracteres 
    else {
        value3 = stoi(lineToAnalize.substr(startPos, 2));
    }
    
    return value3;
}

// Función para encontrar la dirección de memoria más grande utilizada
void theBigger(string lineToAnalize) {
    int newBigger = getAddress(lineToAnalize);  // Recorre las instrucciones
    if(newBigger > theMostGreatest) {
        theMostGreatest = newBigger;            // Actualiza con la dirección más grande encontrada
    }
}

// Instrucción SET: Guarda un valor en una dirección de memoria
void SET(string* mainMemory,string lineToAnalize) {
    int address = getAddress(lineToAnalize);    // Extrae la dirección de memoria de la instrucción
    int value = getValue2(lineToAnalize);       // Obtiene el valor que debe almacenarse en esa dirección
    mainMemory[address] = to_string(value);     // Convierte el valor en string y lo almacena en mainMemory
}

// Instrucción LDR: Carga el valor de una dirección de memoria en el acumulador
void LDR(string* mainMemory,string lineToAnalize) {
    int address = getAddress(lineToAnalize);    // Extrae la dirección desde la instrucción
    MAR = address;                              // Guarda la dirección (por ejemplo, D3) en MAR 
    MDR = mainMemory[address];                  // Accede a la memoria en esa dirección y guarda el dato en MDR
    accumulator = stoi(MDR);                    // Convierte el valor a entero y lo almacena en el acumulador
}

// Función para contar cuántos operandos con prefijo 'D' hay en una instrucción
int contarDs(const string& lineToAnalize) {
    stringstream ss(lineToAnalize);            // Crea un stream para analizar la línea de instrucción
    string palabra;
    int contadorD = 0;

    // Leer la primera palabra "ADD" (se ignora)
    ss >> palabra;

    // Leer las siguientes palabras
    while (ss >> palabra) {
        // Verificar si la palabra empieza con 'D' y tiene al menos un dígito después
        if (palabra[0] == 'D' && palabra.length() > 1) {
            bool esNumero = true;
            // Recorre los caracteres después de 'D'
            for (size_t i = 1; i < palabra.length(); i++) {
                // Si no es un número, se descarta
                if (!isdigit(palabra[i])) {     
                    esNumero = false;
                    break;
                }
            }
            if (esNumero) {
                // Si la palabra es válida (D + número), aumenta el contador
                contadorD++;
            }
        }
        // Si es NULL, simplemente lo ignoramos
    }

    return contadorD;
}

// Instrucción ADD: Suma valores almacenados en memoria y guarda el resultado en el acumulador
void ADD(string* mainMemory, string lineToAnalize) {
    
    
    int cantidadDs = contarDs(lineToAnalize);       // Cuenta cuántas direcciones 'D' hay en la instrucción
    switch (cantidadDs)
    {
    case 1:
        // Case 1: ADD D1 NULL NULL
        ALU = accumulator;                           // Guarda el valor actual del acumulador en la ALU
        accumulator = 0;                             // Resetea el acumulador                        

        //ADD D3 NULL NULL
        MAR = getAddress(lineToAnalize);             // Obtener la dirección de memoria 
        MDR = mainMemory[MAR];                       // La instrucción en la memoria se carga en el MDR
        accumulator = stoi(MDR);                     // Convierte el valor del MDR a entero y lo almacena en el acumulador
        ALU += accumulator;                          // Suma el valor de la ALU y el acumulador
        accumulator = ALU;                           // Guarda el resultado en el acumulador
        ALU = 0;                                     // Limpia la ALU
        break;
    case 2:
        //Case 2: ADD D2 D3 NULL
        MAR = getAddress(lineToAnalize);             // Obtiene la dirección del primer operando
        MDR = mainMemory[MAR];                       // Carga el valor de memoria en el MDR
        accumulator = stoi(MDR);                     // Convierte el valor del MDR a entero y lo almacena en el acumulador
        ALU = accumulator;                           // Guarda el valor en la ALU
        accumulator = 0;                             // Resetea el acumulador

        MAR = getValue2(lineToAnalize);              // Obtiene la dirección del segundo operando
        MDR = mainMemory[MAR];                       // Carga el valor en el MDR
        accumulator = stoi(MDR);                     // Almacena el valor MDR (entero) al acumulador
        ALU += accumulator;                          // Suma el valor de la ALU y el acumulador
        accumulator = ALU;                           // Guarda el resultado en el acumulador
        ALU = 0;                                     // Limpia la ALU
        break;
    case 3:
        //Case 3: ADD D3 D22 D4
        MAR = getAddress(lineToAnalize);             // Obtener la dirección del primer operando
        MDR = mainMemory[MAR];                       // Carga el valor en el MDR
        accumulator = stoi(MDR);                     // Almacena el valor MDR (entero) al acumulador
        ALU = accumulator;                           // Guarda el valor en la ALU
        accumulator = 0;                             // Resetea el acumulador

        MAR = getValue2(lineToAnalize);              // Obtiene la dirección del segundo operando
        MDR = mainMemory[MAR];                       // Carga el valor en el MDR
        accumulator = stoi(MDR);                     // Almacena el valor MDR (entero) al acumulador
        ALU += accumulator;                          // Suma el valor de la ALU y el acumulador
        accumulator = ALU;                           // Guarda el resultado en el acumulador
        ALU = 0;                                     // Limpia la ALU
        MAR = getValue3(lineToAnalize);              // Obtiene la dirección del tercer operando (destino)
        MDR = to_string(accumulator);                // Convierte el resultado en string
        mainMemory[MAR] = MDR;                       // Guarda el resultado en memoria
        break;
    }
    
}

// Instrucción INC: Incrementa un valor en memoria
void INC(string* mainMemory, string lineToAnalize) {
    int address = getAddress(lineToAnalize);      // Obtiene la dirección de memoria
    MAR = address;                                // Carga la dirección en MAR
    MDR = mainMemory[address];                    // Carga el valor de la memoria en MDR
    accumulator = stoi(MDR);                      // Almacena el valor (entero) en el acumulador
    ALU = accumulator + 1;                        // Incrementa 1 en la ALU
    MDR = to_string(ALU);                         // Convierte el resultado de ALU a string
    ALU = 0;                                      // Limpia ALU
    mainMemory[address] = MDR;                    // Almacena el nuevo valor en memoria
}

// Instrucción DEC: Decrementa un valor en memoria
void DEC(string* mainMemory, string lineToAnalize) {
    int address = getAddress(lineToAnalize);      // Obtiene la dirección de memoria
    MAR = address;                                // Carga la dirección en MAR
    MDR = mainMemory[address];                    // Carga el valor de la memoria en MDR
    accumulator = stoi(MDR);                      // Carga el valor de MDR en el acumulador
    ALU = accumulator - 1;                        // Restar 1 en la ALU
    MDR = to_string(ALU);                         // Convierte el resultado de ALU a string
    ALU = 0;                                      // Limpia ALU
    mainMemory[address] = MDR;                    // Almacena el resultado en la memoria
}


// Instrucción STR: Guarda el valor del acumulador en una dirección de memoria asignada
void STR(string* mainMemory, string lineToAnalize) {
    int address = getAddress(lineToAnalize);      // Obtiene la dirección de memoria
    MAR = address;                                // Carga la dirección en MAR
    MDR = to_string(accumulator);                 // Convierte el acumulador a string y lo pasa al MDR
    mainMemory[MAR] = MDR;                        // Almacena el valor del acumulador en la memoria
}

// Instrucción SHW: Muestra el valor dependiendo del comando que siga a SHW
void SHW(string* mainMemory, string lineToAnalize) {
    
     // Extrae el comando específico (ACC, ICR, MAR, etc.)
     string command = lineToAnalize.substr(4, 3);

     //-SHW ACC: imprime el valor asignado al acumulador en el momento de consulta
     if(command == "ACC"){
         cout << "Accumulator: " << accumulator << endl;
     }
     //-SHW ICR: imprime el valor del Contador de Registro de Instrucción
     else if(command == "ICR"){
         cout << "ICR: " << ICR << endl;
     }
     //-SHW MAR: imprime el valor en el Registro de Dirección de Memoria
     else if(command == "MAR"){
         cout << "MAR: " << MAR << endl;
     }
     //-SHW MDR: imprime el valor almacenado en la Memoria de Registro de Datos
     else if(command == "MDR"){
         cout << "MDR: " << MDR << endl;
     }
     //-SHW UC: imprime el estado de la Unidad de Control
     else if(command == "UC"){
         cout << "UC: " << UC << endl;
     }
     //-SHW D3: muestra el valor que esté guardado en la dirección D3
     else if(command[0] == 'D'){
         int address = getAddress(lineToAnalize);
         cout << "Memory [" << address << "]: " << mainMemory[address] << endl;
     }
     else{
         cout << "Comando SHW inválido." << endl;
     } 

}

// Instrucción END: Termina la ejecución del programa (lectura de instrucciones)
void END() {
    cout << "Ya se ejecutaron todas las instrucciones, el programa finalizo" << endl;
}

void printLine(int length) {
    for (int i = 0; i < length; i++) {
        cout << '-';
    }
}

void printDoubleLine(int length) {
    printLine(length);
    cout << "   "; // Espacio entre cajas
    printLine(length);
    cout << endl;
}

void printBoxRow(const string& title1, const string& value1, const string& title2, const string& value2) {
    int width = 25;

    // Línea superior de las dos cajas
    printDoubleLine(width);

    // Primera fila (títulos)
    cout << "| " << title1;
    for (int i = 0; i < width - title1.length() - 3; i++) cout << ' ';
    cout << "|   ";
    cout << "| " << title2;
    for (int i = 0; i < width - title2.length() - 3; i++) cout << ' ';
    cout << "|" << endl;

    // Línea media de las dos cajas
    printDoubleLine(width);

    // Segunda fila (valores)
    cout << "| " << value1;
    for (int i = 0; i < width - value1.length() - 3; i++) cout << ' ';
    cout << "|   ";
    cout << "| " << value2;
    for (int i = 0; i < width - value2.length() - 3; i++) cout << ' ';
    cout << "|" << endl;

    // Línea inferior de las dos cajas
    printDoubleLine(width);
}


// Instrucción PAUSE: Detiene el ciclo de instrucción y muestra el estado actual de los registros
void PAUSE() {

    //vector<Rectangle> rectangles;

    cout << "Estado del sistema:\n";
    
    cout << "\n";
    // Primera fila: UC y PC
    printBoxRow("UNIDAD DE CONTROL", UC , "PC", to_string(PC));

    cout << "\n";
    // Segunda fila: ACCUMULADOR y ICR
    printBoxRow("ACUMULADOR", to_string(accumulator), "ICR", ICR);

    cout << "\n";
    // Tercera fila: ALU y MAR
    printBoxRow("ALU", to_string(ALU), "MAR", to_string(MAR));

    cout << "\n";
    // Cuarta fila: MDR solo
    int width = 25;
    printLine(width);
    cout << endl;
    cout << "| MDR";
    for (int i = 0; i < width - 5; i++) cout << ' ';
    cout << "|" << endl;
    printLine(width);
    cout << endl;
    cout << "| " << MDR;
    for (int i = 0; i < width - 10; i++) cout << ' ';
    cout << "|" << endl;
    printLine(width);
    cout << "\n";

    /*
    rectangles.push_back(createRectangle("Acumulador: " + accumulator));
    rectangles.push_back(createRectangle("MAR: " + MAR));
    rectangles.push_back(createRectangle("MDR: " + MDR));
    rectangles.push_back(createRectangle("PC: " + PC));
    rectangles.push_back(createRectangle("ICR: " + ICR));
    rectangles.push_back(createRectangle("UC: " + UC));

    printRectanglesInRow(rectangles);
    */
}


// Función principal que ejecuta el programa
int main(int argc, char *argv[]) {
    
    // Declarando arreglo que reciba las instrucciones
    string* programInput;
    int numLineas = 0;
    string linea;
    string fileToRead;
    
    cout << "Introduzca el nombre del archivo que quiere leer\nNOTA: Debe estar en esta misma carpeta y solo el nombre sin extensión: ";
    getline(cin,fileToRead);

    // Busca el archivo en esta misma ".txt" carpeta
    ifstream archivo("./"+fileToRead+".txt"); //"./"+fileToRead+".txt"

    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo o el nombre es incorrecto" << std::endl;
        return 1;
    }

    // Cuenta la cantidad de lineas que tiene el archivo
    while (getline(archivo, linea)) {
        numLineas++;
    }

    // Le pasamos el valor total de lineas al arreglo para inicializarlo
    programInput = new string[numLineas];

    // Reiniciar el archivo para leerlo de nuevo
    archivo.clear();  // Limpiar los flags de error
    archivo.seekg(0); // Volver al inicio

    int g = 0;
    while (getline(archivo, linea)) {
        programInput[g] = linea;
        g++;
    }
    

    int size = numLineas;
    
    /*
    El menos 1 se usa porque como la instruccion END solo tiene tres parametros y los 3 son NULL, pues eso genera mucho conflicto
    en otras funciones ya que usamos muchos el subsr en posicicones especificas y ahi se encontrarian esos valores de NULL
    */
    for(int j = 0; j < size-1; j++) {
        theBigger(programInput[j]);
    }

    //8 (en 0) + 9 (instrucciones) = 17
    /*
    Aqui lo que pasa es theMostGreates me da el numero mas grande que acompaña la primera D en mi arreglo
    programInput, pero entonces si creo un arreglo de 8 posiciones el solo irira de 0-7, y necesito que tambien
    tenga en cuenta la direccion 8 porque ahi voy a almacenar algo por algo es el mas grande. entocnes por eso 
    le sumo 1 para tener en cuenta esa posicion. Ademas el size es la cantidad de instrucciones*/
    string *mainMemory = new string[theMostGreatest+1+size]; 
    for(int i = 0; i < theMostGreatest+1; i++) {
        mainMemory[i] = "0";
    }
    for(int i = 0; i < size; i++) {
        mainMemory[i+theMostGreatest] = programInput[i];
    }


    // Ajuste del contador de programa (PC) al inicio de las instrucciones
    /*
    Esto porque cuando creamos nuestro arreglo se decidió que ibamos a buscar el D acompaaado del numero más
    grande, entonces ese número + la cantidad de instrucciones es el tamaño del mainMemory.
    Pero entonces como el número mas grande es el total de espacio en blaco que vamos a dejar para ir almacenando
    los valores que retornen las instruccion, por eso me paro en el theMostGreatest
    */
    PC = theMostGreatest; 
    
    cout << "Se muestra la memoria principal al iniciar el programa. Aún no se han ejecutado las instrucciones." << endl;
    printMainMemory(mainMemory,size);
    
    // Implementación de la lógica de la CPU para la ejecución del programa
    for(int i=0; i < size; i++) {
        if(programInput[i].substr(0,3) == "SET") {
            SET(mainMemory, programInput[i]);
            PC++;
            PAUSE();
        }
        else {
            MAR = PC;
            MDR = mainMemory[MAR].substr(0,3);
            ICR = MDR;
            PC++;
            UC = ICR;
            PAUSE();
            if(UC == "LDR") {
                LDR(mainMemory,programInput[i]);
                PAUSE();
            }
            else if(UC == "ADD") {
                ADD(mainMemory, programInput[i]);
            }
            else if(UC == "STR") {
                STR(mainMemory, programInput[i]);
                
            }
            else if(UC == "SHW") {
                SHW(mainMemory, programInput[i]);
            }          
            else if(UC == "DEC") {
                DEC(mainMemory, programInput[i]);
            }
            else if(UC == "INC") {
                INC(mainMemory, programInput[i]);
            }
            else if(UC == "END") {
                END();
            }
            else {
                cout <<"Instrucción no válida" << endl;
            }
        }
    }
    cout << "El valor final del simulador es: " << accumulator << endl;
    cout << "Se muestra la memoria principal al finalizar el programa, ya después de haberse ejecutado las instrucciones." << endl;
    printMainMemory(mainMemory,size);

    delete[] mainMemory;    // Libera la memoria dinámica
    delete [] programInput; // // Libera la memoria dinámica
    return 0;
}