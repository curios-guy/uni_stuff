#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <sstream>
using namespace std;

int generateUniqueId() {
    ifstream inFile("personIDs.txt");
    vector<string> personIDs;
    string line;

    while (getline(inFile, line)) {
        personIDs.push_back(line);
    }
    inFile.close();

    srand(time(NULL));
    int num;
    string numStr;

    while (true) {
        num = (rand() % 999) + 100;
        numStr = to_string((num));

        bool found = false;
        for (string& id : personIDs) {
            if (id == numStr) {
                found = true;
                break;
            }
        }

        if (!found) break;
    }

    ofstream outFile("personIds.txt", ios::app);
    outFile << numStr << "\n";
    outFile.close();

    return num;
}



class Guest{
public:
    int personId;
    string firstName;
    string lastName;
    int stayingRoom;
    Guest(int personId, string fName, string lName, int roomNum) {
        this->personId = personId;
        this->firstName = fName;
        this->lastName = lName;
        this->stayingRoom = roomNum;
    }

};

class Room {
public:
    int roomNumber;
    string roomType = "Regular";
    double pricePerDay = 130;
    bool isAvailable = true;

    Room(int roomId, string roomType, double pricePerDay, bool isAvailable) {
        this->roomNumber = roomId;
        this->roomType = roomType;
        this->pricePerDay = pricePerDay;
        this->isAvailable = isAvailable;
    }
};
vector <Room> loadRooms(string filename) {
    vector <Room> rooms;
    ifstream inFile("rooms.csv");


    string line;
    getline(inFile, line);

    while (getline(inFile, line)) {
        stringstream ss(line);
        string roomNumStr, roomType, pricePerDay, isAvailable;

        getline(ss, roomNumStr, ',');
        getline(ss, roomType, ',');
        getline(ss, pricePerDay, ',');
        getline(ss, isAvailable, ',');

        Room room(stoi(roomNumStr),roomType, stod(pricePerDay), isAvailable == "true");

        rooms.push_back(room);
    }
    return rooms;
};

class Manager {
public:
    void registerGuest() {
        string firstName;
        string lastName;
        int roomNum;
        cout << "Firstname: ";
        cin >> firstName;
        cout << "Lastname: ";
        cin >> lastName;
        cout << "Room they want to stay: ";
        cin >> roomNum;
        Guest guest(generateUniqueId(), firstName, lastName, roomNum);

        vector<Room> rooms = loadRooms("rooms.csv");
        cout << "Room Number" << "\t" << "Room Type" << "\t" << "Price" << "\t\t" << "Availabilty"<< endl;
        cout << "---------------------------------------------------------------"<< endl;
        for (const Room& r : rooms) {
            cout << r.roomNumber << "\t\t" << r.roomType << "\t\t$" << r.pricePerDay << "\t\t"
                 << (r.isAvailable ? "Available" : "Occupied") << endl;
        }
        ofstream outFile("guests.csv", ios::app);

        if (!(outFile.is_open())) {
            cout << "Error opening the file"<< endl;
            return;
        }
        outFile << guest.personId << ","
        << guest.firstName << ","
        << guest.lastName << "\n";

        outFile.close();

        cout<< "Guest added successfully"<< endl;
    }

    //Room code
    void addRoom() {
        int roomNum;
        string roomType;
        double pricePerDay;
        bool isAvailable;
        char availability;
        cout << "Room number: ";
        cin >> roomNum;
        cout << "Room type: ";
        cin >> roomType;
        cout << "Price per day: ";
        cin >> pricePerDay;
        cout << "Is available? [Y/N]: ";
        cin >> availability;
        if (toupper(availability) == 'N') {
            isAvailable = false;
        }else {
            isAvailable = true;
        }


        Room room(roomNum, roomType, pricePerDay, isAvailable);


        //adding data to file
        ofstream outFile("rooms.csv", ios::app);
        if (!(outFile.is_open())) {
            cout << "Error opening the file"<< endl;
            return;
        }
        outFile << room.roomNumber << ","
        << room.roomType << ","
        << room.pricePerDay << ","
        << (room.isAvailable ? "true" : "false") << "\n";

        outFile.close();

        cout<< "Room added successfully"<< endl;
    }

    void viewRooms() {
        vector<Room> rooms = loadRooms("rooms.csv");
        cout << "Room Number" << "\t" << "Room Type" << "\t" << "Price" << "\t\t" << "Availabilty"<< endl;
        cout << "---------------------------------------------------------------"<< endl;
        for (const Room& r : rooms) {
            cout << r.roomNumber << "\t\t" << r.roomType << "\t\t$" << r.pricePerDay << "\t\t"
                 << (r.isAvailable ? "Available" : "Occupied") << endl;
        }
    }

    void modifyRoom() {
        cout << endl;
        cout << "Please input room number: ";
        int roomNum;
        cin >> roomNum;
        cout << endl;
        bool roomFound = false;

        vector<Room> rooms = loadRooms("rooms.csv");
        for (Room& r : rooms) {
            if (r.roomNumber == roomNum) {
                roomFound = true;
                cout << "Room Number" << "\t" << "Room Type" << "\t" << "Price" << "\t\t" << "Availabilty"<< endl;
                cout << "---------------------------------------------------------------"<< endl;
                cout << r.roomNumber << "\t\t" << r.roomType << "\t\t$" << r.pricePerDay << "\t\t"
                 << (r.isAvailable ? "Available" : "Occupied") << endl;
                cout << "Enter new Room Type: ";
                cin >> r.roomType;
                cout << "Enter new Price: ";
                cin >> r.pricePerDay;
                cout << "Is Available? [Y/N]: ";
                char availability;
                cin >> availability;
                r.isAvailable = (availability == 'Y' || availability == 'y') ? true : false;
                break;
            }
        }
        if (!roomFound) {
            cout << "Room not found"<< endl;
            return;
        }

        // Save modified rooms back to CSV
        ofstream outFile("rooms.csv");
        outFile << "Room Number,Room Type,Price,Availability" << endl; // CSV header
        for (const Room& r : rooms) {
            outFile << r.roomNumber << ","
                 << r.roomType << ","
                 << r.pricePerDay << ","
                 << (r.isAvailable ? "true" : "false") << endl;
        }

        cout << "Room updated successfully!" << endl;
    }

    void deleteRoom() {
        cout << "Please input room number to delete: ";
        int roomNum;
        cin >> roomNum;

        vector<Room> rooms = loadRooms("rooms.csv");
        bool roomFound = false;

        // Loop through the rooms and remove the matching room
        for (int i = 0; i < rooms.size(); i++) {
            if (rooms[i].roomNumber == roomNum) {
                rooms.erase(rooms.begin() + i);
                roomFound = true;
                cout << "Room " << roomNum << " deleted." << endl;
                break;
            }
        }

        if (!roomFound) {
            cout << endl;
            cout << "Room not found." << endl;
            return;
        }

        // Save the updated list back to the CSV
        ofstream outFile("rooms.csv");
        outFile << "Room Number,Room Type,Price,Availability" << endl;
        for (const Room& r : rooms) {
            outFile << r.roomNumber << ","
                 << r.roomType << ","
                 << r.pricePerDay << ","
                 << (r.isAvailable ? "true" : "false") << endl;
        }

        cout << "Room list updated." << endl;
    }


};



int main(){
    // cout << generateUniqueId();
    Manager manager;
    // manager.addRoom();
    manager.viewRooms();
    //manager.modifyRoom();
    manager.deleteRoom();
}

// improve the guest experience, the code design, terminal experience