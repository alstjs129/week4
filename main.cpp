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
    Type arrayType;
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

void cmdList(Database &db) {
    for (int i = 0; i < db.size; ++i) {
        cout << db.entries[i] -> key << ": ";
        switch (db.entries[i] -> type) {
        case INT:
            cout << *(static_cast<int*>(db.entries[i] -> value));
            break;
        case DOUBLE:
            cout << *(static_cast<double*>(db.entries[i] -> value));
            break;
        case STRING:
            cout << "\"" << *(static_cast<string*>(db.entries[i] -> value)) << "\"";
            break;
        case ARRAY:
            Array pa = *(static_cast<Array*>(db.entries[i] -> value));
            switch (pa.type) {
                case INT:
                    int* intItems = static_cast<int*>(pa.items);
                    for (int i = 0; i < pa.size; ++i) {
                        if (i == 0) cout << "[";
                        cout << intItems[i];
                        if (i+1 != pa.size) cout << ", ";
                        if (i+1 == pa.size) cout << "]";
                    }
                    break;
            }
            break;
        }
        cout << endl;
    }
}

void cmdAdd(Database &db) {
    cout << "key: ";
    cin >> key;

    cout << "type (int, double, string, array): ";
    cin >> inputType;

    cout << "value: ";
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

        cout << "type (int, double, string, array): ";
        cin >> arrayTypeStr;
        array.type = stringToType(arrayTypeStr);

        cout << "size: ";
        cin >> arraySize;
        array.size = arraySize;

        if (arrayTypeStr == "int") {
            int* intArray = new int[arraySize];
            for (int i = 0; i < arraySize; ++i) {
                cout << "item[" << i << "]: ";
                cin >> intArray[i];
            }
            array.items = intArray;

            add(db, create(type, key, new Array(array)));
        }
    }
}

void cmdGet(Database &db) {
    cout << "key: ";
    cin >> key;

    Entry *getEntry = get(db, key);
    if (getEntry != nullptr) {
        switch (getEntry -> type) {
        case INT:
            cout << getEntry -> key << ": " << *(static_cast<int*>(getEntry->value));
            break;
        case DOUBLE:
            cout << getEntry -> key << ": " << *(static_cast<double*>(getEntry->value));
            break;
        case STRING:
            cout << getEntry -> key << ": \"" << *(static_cast<string*>(getEntry->value)) << "\"";
            break;
        case ARRAY:
            break;
        }
    }
    cout << endl;
}

void cmdDel(Database &db) {
    cout << "key: ";
    cin >> key;

    remove(db, key);
}

int main() {

    Database db;
    init(db);

    while(1) {
        
        cout << "command (list, add, get, del, exit): ";
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
            cout << "[ERROR] undefined command" << endl;
            break;
        }
        cout << endl;
    }

    destroy(db);
    cout << "[END] destroy (db)" << endl;
    return 0;
}