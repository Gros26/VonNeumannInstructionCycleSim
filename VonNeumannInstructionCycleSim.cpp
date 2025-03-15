#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

    

int accumulator = 0;
string MDR = "";
int MAR = 0;
string ICR = "";
int PC = 0;
string UC = "";
int ALU = 0;
int theMostGreatest = 0;

/*
void setTheMostGreatest(int newValue) {
    theMostGreatest = newValue;
}
*/

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
    if(lineToAnalize[8] == ' ') {
        /*
        En el codigo ascii los numeros no son representados con su valor. Ejemplo : el '1' en ascci es 49
        */
        value2 = lineToAnalize[7] - '0';
    }
    else {
        value2 = stoi(lineToAnalize.substr(7,2));
    }
    return value2;
}


void theBigger(string lineToAnalize) {
    
    int newBigger = getAddress(lineToAnalize);
    if(newBigger > theMostGreatest) {
        theMostGreatest = newBigger;
    }
    
    /*
    // Verificar que la línea tenga al menos 6 caracteres
    if (lineToAnalize.length() > 5 && lineToAnalize[6] == ' ' ) {
        char numChar = lineToAnalize[5];
        


        // Verificar si el carácter es un dígito
        if (isdigit(numChar)) {
            int newBigger = numChar - '0'; // Convertir el carácter a un número

            if (theMostGreatest < newBigger) {
                // Actualizar si es mayor
                theMostGreatest = newBigger;
            }
        }
    }
    else {
        int numChar = stoi(lineToAnalize.substr(5,2));
        if(numChar > theMostGreatest) {
            theMostGreatest = numChar;
        }
    }
    */
}


void SET(string* mainMemory,string lineToAnalize) {
    int address = getAddress(lineToAnalize);
    int value = getValue2(lineToAnalize);
    mainMemory[address] = value;
}

void LDR(int address, string* mainMemory) {
    accumulator = stoi(mainMemory[address]);
}


void ADD(int address1, int address2 , int address3, string* mainMemory) {
    if(mainMemory[address1].empty()) {
        cout << "Esta mal" << endl;
    }
    else if(mainMemory[address2].empty()) {
        accumulator = stoi(mainMemory[address1]);
    }
    else if(mainMemory[address3].empty()) {
        accumulator = stoi(mainMemory[address1]) + stoi(mainMemory[address2]);
    }
    else {
        mainMemory[address3] = stoi(mainMemory[address1]) + stoi(mainMemory[address2]);
    }
}


void INC(int address, string* mainMemory) {
    int var = stoi(mainMemory[address]);
    mainMemory[address] = var++;
}

void DEC(int address, string* mainMemory) {
    int var = stoi(mainMemory[address]);
    mainMemory[address] = var--;
}

void STR(int address, string* mainMemory) {
    mainMemory[address] = accumulator;
}

//esto esta comentando por un error que me sale, aun no se
/*
void SHW(int address = -1,string* mainMemory, string command = "") {
    string possibleCommands[] = {"ACC","MAR","MDR","UC","ICR"}; 
    int opc;
    if(address == -1) {
        for(int i = 0; i < 5; i++) {
            if(command == possibleCommands[i]) {
                opc = i;
            }
        }
        switch(opc) {
            case 1:
                    cout << accumulator << endl;
                    break;
            case 2:
                    cout << MAR << endl;
                    break;
            case 3:
                    cout << MDR << endl;
                    break;
            case 4:
                    cout << UC << endl;
                    break;
            case 5:
                    cout << ICR << endl;
                    break;
        }
    }
    else {
        cout << mainMemory[address] << endl;
    }
}
*/

void PAUSE() {
    cout << "Accumulator: " << accumulator << "\n" 
    << "MAR: " << MAR << "\n"
    << "MDR: "<< MDR << "\n"
    << "UC: " << UC << "\n" 
    << "ICR: " << ICR << "\n";
}

void END() {
    cout << "Se acabo" << endl;
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

    string *mainMemory = new string[theMostGreatest+size]; 
    for(int i = 0; i < size; i++) {
        mainMemory[i] = "0";
    }

    /*
    Esto porque cuando creamos nuestro arreglo se decidio que ibamos a buscar el D acompañado del numero más
    grande, entonces ese numero + la cantidad de instrucciones es el tamaño del mainMemory.
    Pero entonces como el numero mas grande es el total de espacio en blaco que vamos a dejar para ir almacenando
    los valores que retornen las instruccion, por eso me paro en el theMostGreatest
    */
    PC = theMostGreatest;
    
    //aca estoy intentando implementar la logica de la CPU
    
    for(int i=0; i < size; i++) {
        if(program[i].substr(0,3) == "SET") {
            SET(mainMemory, program[i]);
        }
        else {
            //MAR = PC;
            cout << "Lineas aparte del SET\nLogica por implementar" << endl;
        }
    }
    

}