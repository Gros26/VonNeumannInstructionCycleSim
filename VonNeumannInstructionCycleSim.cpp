/*
Liseth ....
Juan Manuel Moreno - 2417575
Juan David Lopez Vanegas - 2243077
Grosman Klein Garcia Valencia - 2340247

*/

#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

    

int accumulator = 0;
int MAR = 0;
int PC = 0;
int ALU = 0;
int theMostGreatest = 0;
string MDR = "";
string ICR = "";
string UC = "";

/*
void setTheMostGreatest(int newValue) {
    theMostGreatest = newValue;
}
*/

void printMainMemory(string* mainMemory, int size) {
    for(int i = 0; i < (theMostGreatest+size); i++) {
        cout << "Linea " << i << ":" << mainMemory[i] <<endl;
    }
}


int getAddress(string lineToAnalize) {
    int address;
    if(lineToAnalize[6] == ' ') {
        /*
        En el codigo ascii los numeros no son representados con su valor. Ejemplo : el '1' en ascci es 49
        */
        address = lineToAnalize[5] - '0';
    }
    else {
        address = stoi(lineToAnalize.substr(5,2));
    }
    return address;
}


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

int getValue3(string lineToAnalize) {
    int value3;
    
    // Determinar el punto de inicio para extraer la tercera dirección
    int startPos = lineToAnalize.find(' ', lineToAnalize.find(' ', lineToAnalize.find(' ') + 1) + 1) + 1;
    
    // Caso 1: Si la dirección es de un solo dígito, extraer directamente
    if (lineToAnalize[startPos + 1] == ' ' || lineToAnalize[startPos + 2] == ' ') {
        value3 = lineToAnalize[startPos] - '0';
    }
    // Caso 2: Si la dirección tiene dos dígitos
    else {
        value3 = stoi(lineToAnalize.substr(startPos, 2));
    }
    
    return value3;
}

void theBigger(string lineToAnalize) {
    int newBigger = getAddress(lineToAnalize);
    if(newBigger > theMostGreatest) {
        theMostGreatest = newBigger;
    }
}

void SET(string* mainMemory,string lineToAnalize) {
    int address = getAddress(lineToAnalize);
    int value = getValue2(lineToAnalize);
    mainMemory[address] = to_string(value);
}

void LDR(string* mainMemory,string lineToAnalize) {
    int address = getAddress(lineToAnalize);
    MAR = address;
    MDR = mainMemory[address];
    accumulator = stoi(MDR);
}


void ADD(string* mainMemory, string lineToAnalize) {
    int address = getAddress(lineToAnalize);
    // Case 1: ADD D1 NULL NULL
    ALU = accumulator;
    accumulator = 0;

    //ADD D3 NULL NULL
    MAR = address;           // El MAR toma la dirección de la siguiente instrucción (PC).
    MDR = mainMemory[MAR];  // La instrucción en la memoria se carga en el MDR.
    accumulator = stoi(MDR);
    ALU += accumulator;
    accumulator = ALU;
    ALU = 0;

}


//INC D3 NULL NULL
/*
Supongamos que estamos ya con la instruccion en el Unit Control, 
entocnes de ahi si dice que carga, entonces esa direccion, 
3 en este caso se la psaria al MAR, despues el MDR iria a esa direccion y 
buscaria el valor que hay guardado ahi y lo cargaria en el acumulador y despues se
lo pasaria a la ALU y despues ahi quien le incrementa uno o que. 
*/
void INC(string* mainMemory, string lineToAnalize) {
    int address = getAddress(lineToAnalize);
    MAR = address;
    MDR = mainMemory[address];
    accumulator = stoi(MDR);
    ALU = accumulator + 1;
    MDR = ALU;
    ALU = 0;
    mainMemory[address] = MDR;
}

void DEC(string* mainMemory, string lineToAnalize) {
    int address = getAddress(lineToAnalize);  // Obtener la dirección de memoria
    MAR = address;                            // Cargar la dirección en MAR
    MDR = mainMemory[address];                // Cargar el valor de la memoria en MDR
    accumulator = stoi(MDR);                  // Cargar el valor de MDR en el acumulador
    ALU = accumulator - 1;                    // Restar 1 en la ALU
    MDR = to_string(ALU);                     // Convertir el resultado de ALU a string
    ALU = 0;                                  // Limpiar ALU
    mainMemory[address] = MDR;                // Almacenar el resultado en la memoria
}


//Lee el valor del acumulador y lo asigna a la dirección de memoria que se le pasa
void STR(string* mainMemory, string lineToAnalize) {
    int address = getAddress(lineToAnalize);

    MAR = address;
    MDR = to_string(accumulator);
    mainMemory[MAR] = MDR;                  
}

/*Muestra el valor dependiendo del comando que siga a SHW, por ej: 
-SHW D3 muestra el valor que esté guardado en la dirección D3
-SHW ACC muestra el valor asignado al acumulador en el momento de consulta
-SHW ICR muestra el valor del Contador de Registro de Instrucción
-SHW MAR muestra el valor en el Registro de Dirección de Memoria
*/
void SHW(string* mainMemory, string lineToAnalize) {
    string command = lineToAnalize.substr(4, 3);
    
    if(command == "ACC"){
        cout << "Accumulator: " << accumulator << endl;
    }
    else if(command == "ICR"){
        cout << "ICR: " << ICR << endl;
    }
    else if(command == "MAR"){
        cout << "MAR: " << MAR << endl;
    }
    else if(command == "MDR"){
        cout << "MDR: " << MDR << endl;
    }
    else if(command == "UC"){
        cout << "UC: " << UC << endl;
    }
    else if(command[0] == 'D'){
        int address = getAddress(lineToAnalize);
        cout << "Memory [" << address << "]: " << mainMemory[address] << endl;
    }
    else{
        cout << "Comando SHW inválido." << endl;
    } 
}


void END() {
    cout << "Se acabo" << endl;
}


void PAUSE() {
    cout << "Estado del sistema:\n";
    cout << "Acumulador: " << accumulator << endl;
    cout << "MAR: " << MAR << endl;
    cout << "MDR: " << MDR << endl;
    cout << "PC: " << PC << endl;
    cout << "ICR: " << ICR << endl;
    cout << "UC: " << UC << endl;
}


int main(int argc, char *argv[]) {
    string program[] =  {"SET D5 12 NULL NULL", "SET D2 23 NULL NULL","SET D8 3 NULL NULL","SET D3 5 NULL NULL",
                         "LDR D2 NULL NULL NULL", "ADD D5 NULL NULL NULL",
                        "ADD D8 NULL NULL NULL", "STR D3 NULL NULL NULL",
                        "LDR D3 NULL NULL NULL", "ADD D2 NULL NULL NULL", 
                        "STR D2 NULL NULL NULL", "SHW D2 NULL NULL NULL", "END NULL NULL NULL"};
    

    int size = sizeof(program) / sizeof(program[0]);
    
    /*
    El menos 1 se usa porque como la instruccion END solo tiene tres parametros y los 3 son NULL, pues eso genera mucho conflicto
    en otras funciones ya que usamos muchos el subsr en posicicones especificas y ahi se encontrarian esos valores de NULL
    */
    for(int j = 0; j < size-1; j++) {
        theBigger(program[j]);
        cout << program[j].substr(0,3) << endl;
    }

    cout << theMostGreatest;

    //8 (en 0) + 9 (instrucciones) = 17
    /*
    Aqui lo que pasa es theMostGreates me da el numero mas grande que acompaña la primera D en mi arreglo
    program, pero entonces si creo un arreglo de 8 posiciones el solo irira de 0-7, y necesito que tambien
    tenga en cuenta la direccion 8 porque ahi voy a almacenar algo por algo es el mas grande. entocnes por eso 
    le sumo 1 para tener en cuenta esa posicion. Ademas el size es la cantidad de instrucciones*/
    string *mainMemory = new string[theMostGreatest+1+size]; 
    for(int i = 0; i < theMostGreatest+1; i++) {
        mainMemory[i] = "0";
    }
    for(int i = 0; i < size; i++) {
        mainMemory[i+8] = program[i];
    }

    /*
    Esto porque cuando creamos nuestro arreglo se decidio que ibamos a buscar el D acompañado del numero más
    grande, entonces ese numero + 1 + la cantidad de instrucciones es el tamaño del mainMemory.
    Pero entonces como el numero mas grande es el total de espacio en blaco que vamos a dejar para ir almacenando
    los valores que retornen las instruccion, por eso me paro en el theMostGreatest
    */
    PC = theMostGreatest; // aqui hice un enredo grande, la cosa esque ya no le sumo uno porque me sirve para el siguiente for que no este tan adelantado
    
    //aca estoy intentando implementar la logica de la CPU
    
    for(int i=0; i < size; i++) {
        if(program[i].substr(0,3) == "SET") {
            SET(mainMemory, program[i]);
            PC++;
        }
        else {
            cout << "Entro #" << i << ": " << endl;
            MAR = PC;
            MDR = mainMemory[MAR].substr(0,3);
            ICR = MDR;
            PC++;
            UC = ICR;
            PAUSE();
            if(UC == "LDR") {
                LDR(mainMemory,program[i]);
                PAUSE();
            }
            else if(UC == "ADD") {
                ADD(mainMemory, program[i]);
            }
            else if(UC == "STR") {
                STR(mainMemory, program[i]);
                
            }
            else if(UC == "SHW") {
                SHW(mainMemory, program[i]);
            }          
            else if(UC == "DEC") {
                DEC(mainMemory, program[i]);
            }
            else if(UC == "INC") {
                INC(mainMemory, program[i]);
            }
            else if(UC == "END") {
                END();
            }
            else {
                cout <<" no paso nada" << endl;
            }
        }
    }

    printMainMemory(mainMemory,size);

    delete[] mainMemory;

}