#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
using namespace std;

//VIDEO ADT
class Video {
public:
    static int nextID;
    int Video_ID;
    string Movie_Title;
    string Genre;
    string Production;
    int Number_of_Copies;

    Video() {
        Video_ID = nextID++;
    }

    void input() {
        cout << "***********************************************\n";
        cout << "\tVideo Store Management System\n";
        cout << "***********************************************\n";
        cout << "Movie Title: "; cin.ignore(); getline(cin, Movie_Title);
        cout << "Genre: "; getline(cin, Genre);
        cout << "Production: "; getline(cin, Production);
        cout << "Number of Copies: "; cin >> Number_of_Copies;
    }

    void display() {
        cout << "***********************************************\n";
        cout << "\tVideo Store Management System\n";
        cout << "***********************************************\n";
        cout << "Video ID: " << Video_ID << endl;
        cout << "Movie Title: " << Movie_Title << endl;
        cout << "Genre: " << Genre << endl;
        cout << "Production: " << Production << endl;
        cout << "Number of Copies: " << Number_of_Copies << endl;
    }

    string serialize() {
        return to_string(Video_ID) + "," + Movie_Title + "," + Genre + "," + Production + "," + to_string(Number_of_Copies) +  "\n";
    }

    void deserialize(string line) {
        stringstream ss(line);
        string token;
        getline(ss, token, ','); Video_ID = stoi(token);
        getline(ss, Movie_Title, ',');
        getline(ss, Genre, ',');
        getline(ss, Production, ',');
        getline(ss, token, ','); Number_of_Copies = stoi(token);
    }
};

int Video::nextID = 1;

vector<Video> videos;

//CUSTOMER PARENT ADT
class Customer {
public:
    static int nextID;
    int Customer_ID;
    string Name;
    string Address;

    Customer() {
        Customer_ID = nextID++;
    }

    void input() {
        cout << "Name: "; cin.ignore(); getline(cin, Name);
        cout << "Address: "; getline(cin, Address);
    }

    void display() {
        cout << "Customer ID: " << Customer_ID << endl;
        cout << "Name: " << Name << endl;
        cout << "Address: " << Address << endl;
    }

    string serialize() {
        return to_string(Customer_ID) + "," + Name + "," + Address + "\n";
    }

    void deserialize(string line) {
        stringstream ss(line);
        string token;
        getline(ss, token, ','); Customer_ID = stoi(token);
        getline(ss, Name, ',');
        getline(ss, Address, ',');
    }
};

int Customer::nextID = 1;

vector<Customer> customers;

//CUSTOMER-RENT CHILD ADT
class CustomerRent : public Customer {
public:
    vector<int> rentedVideos;

    void rentVideo(int videoID) {
        rentedVideos.push_back(videoID);
    }

    void returnVideo(int videoID) {
        rentedVideos.erase(remove(rentedVideos.begin(), rentedVideos.end(), videoID), rentedVideos.end());
    }

    void displayRentedVideos() {
        cout << "Customer ID: " << Customer_ID << " has rented videos with IDs: ";
        for (int id : rentedVideos) {
            cout << id << " ";
        }
        cout << endl;
    }

    string serialize() {
        string result = to_string(Customer_ID);
        for (int id : rentedVideos) {
            result += "," + to_string(id);
        }
        return result + "\n";
    }

    void deserialize(string line) {
        stringstream ss(line);
        string token;
        getline(ss, token, ','); Customer_ID = stoi(token);
        while (getline(ss, token, ',')) {
            rentedVideos.push_back(stoi(token));
        }
    }
};

vector<CustomerRent> customerRents;

//FUNCTIONS
void loadVideos() {
    ifstream inFile("videos.txt");
    string line;
    while (getline(inFile, line)) {
        Video video;
        video.deserialize(line);
        videos.push_back(video);
    }
    inFile.close();
}

void saveVideos() {
    ofstream outFile("videos.txt");
    for (Video& video : videos) {
        outFile << video.serialize();
    }
    outFile.close();
}

void loadCustomers() {
    ifstream inFile("customers.txt");
    string line;
    while (getline(inFile, line)) {
        Customer customer;
        customer.deserialize(line);
        customers.push_back(customer);
    }
    inFile.close();
}

void saveCustomers() {
    ofstream outFile("customers.txt");
    for (Customer& customer : customers) {
        outFile << customer.serialize();
    }
    outFile.close();
}

void loadCustomerRents() {
    ifstream inFile("customerRents.txt");
    string line;
    while (getline(inFile, line)) {
        CustomerRent customerRent;
        customerRent.deserialize(line);
        customerRents.push_back(customerRent);
    }
    inFile.close();
}

void saveCustomerRents() {
    ofstream outFile("customerRents.txt");
    for (CustomerRent& customerRent : customerRents) {
        outFile << customerRent.serialize();
    }
    outFile.close();
}
void insertNewVideo() {
    Video video;
    video.input();
    videos.push_back(video);
    saveVideos();
    cout << "Video added successfully.\n";
}

void rentVideo() {
    int customerID, videoID;
    cout << "***********************************************\n";
    cout << "\tVideo Store Management System\n";
    cout << "***********************************************\n";
    cout << "Enter Customer ID: "; cin >> customerID;
    cout << "Enter Video ID: "; cin >> videoID;

    auto customer = find_if(customers.begin(), customers.end(), [&](Customer& c) { return c.Customer_ID == customerID; });
    if (customer == customers.end()) {
        cout << "Customer not found.\n";
        return;
    }

    auto video = find_if(videos.begin(), videos.end(), [&](Video& v) { return v.Video_ID == videoID; });
    if (video == videos.end()) {
        cout << "Video not found.\n";
        return;
    }

    if (video->Number_of_Copies > 0) {
        video->Number_of_Copies--;

        auto rent = find_if(customerRents.begin(), customerRents.end(), [&](CustomerRent& cr) { return cr.Customer_ID == customerID; });
        if (rent == customerRents.end()) {
            CustomerRent newRent;
            newRent.Customer_ID = customerID;
            newRent.rentVideo(videoID);
            customerRents.push_back(newRent);
        }
        else {
            rent->rentVideo(videoID);
        }

        saveVideos();
        saveCustomerRents();
        cout << "Video rented successfully.\n";
    }
    else {
        cout << "No copies available.\n";
    }
}

void returnVideo() {
    int customerID, videoID;
    cout << "***********************************************\n";
    cout << "\tVideo Store Management System\n";
    cout << "***********************************************\n";
    cout << "Enter Customer ID: "; cin >> customerID;
    cout << "Enter Video ID: "; cin >> videoID;

    auto customer = find_if(customers.begin(), customers.end(), [&](Customer& c) { return c.Customer_ID == customerID; });
    if (customer == customers.end()) {
        cout << "Customer not found.\n";
        return;
    }

    auto video = find_if(videos.begin(), videos.end(), [&](Video& v) { return v.Video_ID == videoID; });
    if (video == videos.end()) {
        cout << "Video not found.\n";
        return;
    }

    auto rent = find_if(customerRents.begin(), customerRents.end(), [&](CustomerRent& cr) { return cr.Customer_ID == customerID; });
    if (rent == customerRents.end() || find(rent->rentedVideos.begin(), rent->rentedVideos.end(), videoID) == rent->rentedVideos.end()) {
        cout << "This customer did not rent this video.\n";
        return;
    }

    video->Number_of_Copies++;
    rent->returnVideo(videoID);
    if (rent->rentedVideos.empty()) {
        customerRents.erase(rent);
    }

    saveVideos();
    saveCustomerRents();
    cout << "Video returned successfully.\n";
}

void showVideoDetails() {
    int videoID;
    cout << "***********************************************\n";
    cout << "\tVideo Store Management System\n";
    cout << "***********************************************\n";
    cout << "Enter Video ID: "; cin >> videoID;

    auto video = find_if(videos.begin(), videos.end(), [&](Video& v) { return v.Video_ID == videoID; });
    if (video == videos.end()) {
        cout << "Video not found.\n";
        return;
    }

    video->display();
}

void displayAllVideos() {
    for (Video& video : videos) {
        video.display();
        cout << "------------------\n";
    }
}

void checkVideoAvailability() {
    int videoID;
    cout << "***********************************************\n";
    cout << "\tVideo Store Management System\n";
    cout << "***********************************************\n";
    cout << "Enter Video ID: "; cin >> videoID;

    auto video = find_if(videos.begin(), videos.end(), [&](Video& v) { return v.Video_ID == videoID; });
    if (video == videos.end()) {
        cout << "Video not found.\n";
    }
    else {
        cout << "Video is " << (video->Number_of_Copies > 0 ? "available.\n" : "not available.\n");
    }
}

void addCustomer() {
    Customer customer;
    customer.input();
    customers.push_back(customer);
    saveCustomers();
    cout << "Customer added successfully.\n";
}

void showCustomerDetails() {
    int customerID;
    cout << "***********************************************\n";
    cout << "\tVideo Store Management System\n";
    cout << "***********************************************\n";
    cout << "Enter Customer ID: "; cin >> customerID;

    auto customer = find_if(customers.begin(), customers.end(), [&](Customer& c) { return c.Customer_ID == customerID; });
    if (customer == customers.end()) {
        cout << "Customer not found.\n";
    }
    else {
        customer->display();
    }
}

void listVideosRentedByCustomer() {
    int customerID;
    cout << "***********************************************\n";
    cout << "\tVideo Store Management System\n";
    cout << "***********************************************\n";
    cout << "Enter Customer ID: "; cin >> customerID;

    auto rent = find_if(customerRents.begin(), customerRents.end(), [&](CustomerRent& cr) { return cr.Customer_ID == customerID; });
    if (rent == customerRents.end()) {
        cout << "No videos rented by this customer.\n";
    }
    else {
        rent->displayRentedVideos();
    }
}
void displayMenu() {
    cout << "***********************************************\n";
    cout << "\tVideo Store Management System\n";
    cout << "***********************************************\n";
    cout << "1. New Video\n";
    cout << "2. Rent a Video\n";
    cout << "3. Return a Video\n";
    cout << "4. Show Video Details\n";
    cout << "5. Display all Videos\n";
    cout << "6. Check Video Availability\n";
    cout << "7. Customer Maintenance\n";
    cout << "8. Exit Program\n";
    cout << "\n";
    cout << "Enter your choice: ";
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


//PROGRAM
int main() {
    loadVideos();
    loadCustomers();
    loadCustomerRents();

    int choice;
    while (true) {
        clearScreen();
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            clearScreen();
            insertNewVideo();
            break;
        case 2:
            clearScreen();
            rentVideo();
            break;
        case 3:
            clearScreen();
            returnVideo();
            break;
        case 4:
            clearScreen();
            showVideoDetails();
            break;
        case 5:
            clearScreen();
            displayAllVideos();
            break;
        case 6:
            clearScreen();
            checkVideoAvailability();
            break;
        case 7:
            clearScreen();
            cout << "***********************************************\n";
            cout << "\tVideo Store Management System\n";
            cout << "***********************************************\n";
            cout << "1. Add New Customer\n";
            cout << "2. Show Customer Details\n";
            cout << "3. List of Videos Rented by a Customer\n";
            cout << "Enter your choice: ";
            int subchoice;
            cin >> subchoice;
            clearScreen();
            switch (subchoice) {
            case 1:
                addCustomer();
                break;
            case 2:
                showCustomerDetails();
                break;
            case 3:
                listVideosRentedByCustomer();
                break;
            default:
                cout << "Invalid choice.\n";
            }
            break;
        case 8:
            clearScreen();
            saveVideos();
            saveCustomers();
            saveCustomerRents();
            cout << "Exiting program.\n";
            return 0;
        default:
            cout << "Invalid choice.\n";
        }
        cout << "Press <Enter> to continue...";
        cin.ignore();
        cin.get();
    }
}
