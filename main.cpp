#include <iostream>
#include <string>
#include "database.h"

using namespace std;

string cmd, key, inputType;
int intValue;
double doubleValue;
string stringValue;

Type type;
Array array;

Type stringToType(string &strType) {
    if (strType == "int") {
        return INT;
    }
    else if(strType == "double") {
        return DOUBLE;
    }
    else if(strType == "string") {
        return STRING;
    }
    else {
        return ARRAY;
    }
}

void printArray(Array &arr) {
    if (arr.type == INT) {
        int* intItems = static_cast<int*>(arr.items);
        for (int i = 0; i < arr.size; ++i) {
            if (i == 0) std::cout << "[";
            std::cout << intItems[i];
            if (i+1 != arr.size) std::cout << ", ";
            if (i+1 == arr.size) std::cout << "]";
        }
    } else if (arr.type == DOUBLE) {
        double* doubleItems = static_cast<double*>(arr.items);
        for (int i = 0; i < arr.size; ++i) {
            if (i == 0) std::cout << "[";
            std::cout << doubleItems[i];
            if (i+1 != arr.size) std::cout << ", ";
            if (i+1 == arr.size) std::cout << "]";
        }
    } else if (arr.type == STRING) {
        string* stringItems = static_cast<string*>(arr.items);
        for (int i = 0; i < arr.size; ++i) {
            if (i == 0) std::cout << "[";
            std::cout << "\"" << stringItems[i] << "\"";
            if (i+1 != arr.size) std::cout << ", ";
            if (i+1 == arr.size) std::cout << "]";
        }
    } else {
        // arr.type == ARRAY
        
    }
}

void cmdList(Database &db) {
    for (int i = 0; i < db.size; ++i) {
        std::cout << db.entries[i] -> key << ": ";
        switch (db.entries[i] -> type) {
        case INT:
            std::cout << *(static_cast<int*>(db.entries[i] -> value));
            break;
        case DOUBLE:
            std::cout << *(static_cast<double*>(db.entries[i] -> value));
            break;
        case STRING:
            std::cout << "\"" << *(static_cast<string*>(db.entries[i] -> value)) << "\"";
            break;
        case ARRAY:
            Array arr = *(static_cast<Array*>(db.entries[i] -> value));
            // Array *arr = *static_cast<Array**>(db.entries[i]->value);
            // cout << "---" << arr.type << "---" << endl;
            // if (arr.type == ARRAY) {
                // cout << "I 'm in \n";
                // Array **arrayList = static_cast<Array**>(arr.items);
                // for (int i = 0; i < arr.size; ++i) {
                //     Array *arrayItems = arrayList[i];
                //     for (int j = 0; j < arrayItems->size; ++j) {
                //         if (arrayItems->type == INT) {
                //             int *intArrayItems = static_cast<int*>(arrayItems->items);
                //             std::cout << intArrayItems[j] << ", ";
                //         }
                //     }
                // }
            // }
            // else printArray(arr);
            printArray(arr);
            
        }
        std::cout << endl;
    }
}

void cmdAdd(Database &db) {
    std::cout << "key: ";
    cin >> key;
    int keyInDb = keyCheck(db, key);
    if (keyInDb) remove(db, key);

    std::cout << "type (int, double, string, array): ";
    cin >> inputType;

    std::cout << "value: ";
    if (inputType == "int") {
        type=INT;
        cin >> intValue;
        add(db, create(type, key, new int(intValue)));
    }
    else if(inputType == "double") {
        type=DOUBLE;
        cin >> doubleValue;
        add(db, create(type, key, new double(doubleValue)));
    }
    else if(inputType == "string") {
        type=STRING;
        getline(cin >> ws, stringValue);
        add(db, create(type, key, new string(stringValue)));
    }
    else {
        type=ARRAY;

        string arrayTypeStr;
        int arraySize;

        std::cout << "type (int, double, string, array): ";
        cin >> arrayTypeStr;
        array.type = stringToType(arrayTypeStr);

        if (arrayTypeStr == "int") {
            std::cout << "size: ";
            cin >> arraySize;
            array.size = arraySize;

            int* intArray = new int[arraySize];
            for (int i = 0; i < arraySize; ++i) {
                std::cout << "item[" << i << "]: ";
                cin >> intArray[i];
            }
            array.items = intArray;

            add(db, create(type, key, new Array(array)));
        }
        else if (arrayTypeStr == "double") {
            std::cout << "size: ";
            cin >> arraySize;
            array.size = arraySize;
                        
            double* doubleArray = new double[arraySize];
            for (int i = 0; i < arraySize; ++i) {
                std::cout << "item[" << i << "]: ";
                cin >> doubleArray[i];
            }
            array.items = doubleArray;

            add(db, create(type, key, new Array(array)));
        } 
        else if (arrayTypeStr == "string") {
            std::cout << "size: ";
            cin >> arraySize;
            array.size = arraySize;
                        
            string* stringArray = new string[arraySize];
            for (int i = 0; i < arraySize; ++i) {
                std::cout << "item[" << i << "]: ";
                getline(cin >> ws, stringArray[i]);
            }
            array.items = stringArray;

            add(db, create(type, key, new Array(array)));
        }
        else {
            // arrayTypeStr == "array"
            int nestedArrayTotalSize, nestedArraySize;
            string nestedArrayType;

            std::cout << "size: ";
            cin >> nestedArrayTotalSize;

            Array **arrayList = new Array*[nestedArrayTotalSize];
            for (int i = 0; i < nestedArrayTotalSize; ++i) {
                Array* nestedArray = new Array;

                std::cout << "item[" << i << "]: type (int, double, string, array): ";
                cin >> nestedArrayType;
                nestedArray->type = stringToType(nestedArrayType);

                std::cout << "size: ";
                cin >> nestedArraySize;
                nestedArray->size = nestedArraySize;

                if (nestedArrayType == "int") {
                    int* nestedInt = new int[nestedArraySize];
                    for (int j = 0; j < nestedArraySize; ++j) {
                        std::cout << "item[" << j << "]: ";
                        cin >> nestedInt[j];
                    }
                    nestedArray->items = nestedInt;
                    // int *a = static_cast<int*>(nestedArray->items);
                    // cout << "a === " << a[0] << ", " << a[1] << ", " << a[2];
                    arrayList[i] = nestedArray;
                } 
                else if(nestedArrayType == "double") {
                    double* nestedDouble = new double[nestedArraySize];
                    for (int j = 0; j < nestedArraySize; ++j) {
                        std::cout << "item[" << j << "]: ";
                        cin >> nestedDouble[j];
                    }
                    nestedArray->items = nestedDouble;
                    arrayList[i] = nestedArray;
                }
                else if(nestedArrayType == "string") {
                    string* nestedString = new string[nestedArraySize];
                    for (int j = 0; j < nestedArraySize; ++j) {
                        std::cout << "item[" << j << "]: ";
                        getline(cin >> ws, nestedString[j]);
                    }
                    nestedArray->items = nestedString;
                    arrayList[i] = nestedArray;
                }
                // std::cout << arrayList[i]->items;
                // int *a = static_cast<int*>(arrayList[i]->items);
                // std::cout << "hihi " << a[0] << " " << a[1] << " " << a[2] << endl;
            }
            // void **voidArray = reinterpret_cast<void**>(arrayList);
            add(db, create(ARRAY, key, arrayList)); 
            int *a = static_cast<int*>(arrayList[0]->items);
            std::cout << "hihi " << a[0] << " " << a[1] << " " << a[2] << endl;
        }
    }
}

void cmdGet(Database &db) {
    std::cout << "key: ";
    cin >> key;

    Entry *getEntry = get(db, key);
    if (getEntry != nullptr) {
        switch (getEntry -> type) {
            case INT:
                std::cout << getEntry -> key << ": " << *(static_cast<int*>(getEntry->value));
                break;
            case DOUBLE:
                std::cout << getEntry -> key << ": " << *(static_cast<double*>(getEntry->value));
                break;
            case STRING:
                std::cout << getEntry -> key << ": \"" << *(static_cast<string*>(getEntry->value)) << "\"";
                break;
            case ARRAY:
                std::cout << getEntry -> key << ": ";
                Array pa = *(static_cast<Array*>(getEntry -> value));
                printArray(pa);
                break;
        }
    }
    std::cout << endl;
}

void cmdDel(Database &db) {
    std::cout << "key: ";
    cin >> key;

    remove(db, key);
}

int main() {

    Database db;
    init(db);

    while(1) {
        
        std::cout << "command (list, add, get, del, exit): ";
        cin >> cmd;

        if (cmd == "list") {
            cmdList(db);
        } else if (cmd == "add") {
            cmdAdd(db);
        } else if (cmd == "get") {
            cmdGet(db);
        } else if (cmd == "del") {
            cmdDel(db);
        } else if (cmd == "exit") {
            break;
        } else {
            std::cout << "[ERROR] undefined command" << endl;
            break;
        }
        std::cout << endl;
    }

    destroy(db);
    std::cout << "[END] destroy (db)" << endl;
    return 0;
}