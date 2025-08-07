//BANK PROJECT
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;
string const ClientsFileName = "Clients.txt";
void ShowMainMenu();
void ShowTransctionsMenu();
enum enMainMenu { eClientsList = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTranscationMenu = 6, eExit = 7 };
enum enTranscationsMenuOptions { eDespoit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenu = 4 };
struct stClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};
vector<string>SplitString(string S1, string Delim) {
    vector<string>vString;
    string sWord;
    short pos = 0;
    while ((pos = S1.find(Delim)) != std::string::npos) {
        sWord = S1.substr(0, pos);
        if (sWord != "") {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "") {
        vString.push_back(S1);
    }
    return vString;
}
stClient ConvertLineToRecord(string DataLine, string separator = "#//#") {
    stClient Client;
    vector<string>vClientData = SplitString(DataLine, separator);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}
string ConvertRecordToLine(stClient Client, string Separtor = "#//#") {
    string stClientRecord;
    stClientRecord += Client.AccountNumber + Separtor;
    stClientRecord += Client.PinCode + Separtor;
    stClientRecord += Client.Name + Separtor;
    stClientRecord += Client.Phone + Separtor;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}
vector<stClient>LoadClientsDataFromFile(string FileName) {
    vector<stClient>vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        stClient Client;
        string DataLine;
        while (getline(MyFile, DataLine)) {
            if (DataLine != "") {
                Client = ConvertLineToRecord(DataLine);
                vClients.push_back(Client);
            }
        }
        MyFile.close();
    }
    return vClients;
}
short ReadMainMenuOption() {
    short Choice;
    cout << "\nChoose what do you want to do ? [1 to 6} ? ";
    cin >> Choice;
    return Choice;
}
void PrintClientRecordLine(stClient Client) {
    cout << "\n\n| " << setw(15) << left << Client.AccountNumber
        << "| " << setw(10) << left << Client.PinCode
        << "| " << setw(40) << left << Client.Name
        << "| " << setw(12) << left << Client.Phone
        << "| " << setw(12) << left << Client.AccountBalance;
}
void ShowAllClientsScreen() {
    vector<stClient>vClients;
    vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\t\t\t\t Client List (" << vClients.size() << ") Client(s).";
    cout << "\n_____________________________________________________________________________________________________________";
    cout << "\n\n| " << setw(15) << left << "Account Number"
        << "| " << setw(10) << left << "Pin Code"
        << "| " << setw(25) << left << "Client Name"
        << "| " << setw(15) << left << "Phone"
        << "| " << setw(10) << left << "Balance";
    cout << "\n_____________________________________________________________________________________________________________";
    for (stClient& C : vClients) {
        PrintClientRecordLine(C);
    }
    cout << "\n\n_____________________________________________________________________________________________________________";
}
void GoBackToMainMenu() {

    cout << "\nPress any key to go back main menu...";
    system("pause>0");
    
    ShowMainMenu();
}
bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        string DataLine;
        stClient Client;
        while (getline(MyFile, DataLine)) {
            Client = ConvertLineToRecord(DataLine);
            if (Client.AccountNumber == AccountNumber) {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();

    }
    return false;
}
void AddDataLineToFile(string DataLine, string FileName) {
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open()) {

        MyFile << DataLine << endl;
    }
    MyFile.close();
}
stClient ReadNewClient() {
    stClient Client;

    cout << "\nEnter Account Number ?";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName)) {
        cout << "\nClient With Account Number (" << Client.AccountNumber << ") already exists,";
        cout << "Enter another account number ?";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode : ";
    getline(cin, Client.PinCode);

    cout << "Enter Name : ";
    getline(cin, Client.Name);

    cout << "Enter Phone : ";
    getline(cin, Client.Phone);

    cout << "Enter Balance : ";
    cin >> Client.AccountBalance;

    return Client;
}
void AddNewClient() {
    stClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ConvertRecordToLine(Client), ClientsFileName);
}
void AddNewClients() {
    char AddMore = 'y';
    cout << "\nAdding New Clients : \n\n";
    do {

        AddNewClient();
        cout << "\nClient Added Successfully,";
        cout << "Do you want to add more client ? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}
void ShowAddClientsScreen() {
    cout << "--------------------------------------------------\n\n";
    cout << "\t\tAdd New Clients ";
    cout << "\n\n--------------------------------------------------";

    AddNewClients();
}
bool FindClientByAccountNumber(string AccountNumber, vector<stClient>vClients, stClient& Client) {
    for (stClient& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            Client = C;
            return true;
        }
    }
    return false;
}
string ReadClientAccountNumber() {
    string AccountNumber = "";
    cout << "\nEnter Account Number ? ";
    getline(cin >> ws, AccountNumber);
    return AccountNumber;
}
bool MarkClientForDelete(string AccountNumber, vector<stClient>& vClients) {
    for (stClient& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
void PrintClientCard(stClient Client) {
    cout << "\nThe following are the Client details  : ";
    cout << "\n______________________________________________";
    cout << "\nAccount Number : " << Client.AccountNumber;
    cout << "\nPinCode : " << Client.PinCode;
    cout << "\nName : " << Client.Name;
    cout << "\nPhone : " << Client.Phone;
    cout << "\nBalance : " << Client.AccountBalance;
    cout << "\n______________________________________________";
}
void SaveClientDataToFile(string FileName, vector<stClient>vClients) {
    fstream MyFile;
    string DataLine;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open()) {
        for (stClient& C : vClients) {
            if (C.MarkForDelete != true) {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
}
bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient>& vClients) {
    stClient Client;
    char Answer = 'y';
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintClientCard(Client);
        cout << "\nAre you sure you want delete this Client ? y/n";
        cin >> Answer;
        if (toupper(Answer) == 'Y') {
            MarkClientForDelete(AccountNumber, vClients);
            SaveClientDataToFile(ClientsFileName, vClients);

            //refresh vector
            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\n\nClient Deleted Successfully .";
            return true;
        }

    }
    else {
        cout << "Client With Account Number (" << AccountNumber << ") is Not Found";
        return false;
    }
}
void ShowDeleteClientScreen() {
    cout << "--------------------------------------------------\n\n";
    cout << "\t\tDelete Client ";
    cout << "\n\n--------------------------------------------------";
    vector<stClient>vClients;
    vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}
stClient ChangeClientRecord(string AccountNumber) {
    stClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode : ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name : ";
    getline(cin, Client.Name);

    cout << "Enter Phone : ";
    getline(cin, Client.Phone);

    cout << "Enter Balance : ";
    cin >> Client.AccountBalance;

    return Client;
}
bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& vClients) {
    char Answer = 'Y';
    stClient Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintClientCard(Client);
        cout << "\nAre you sure you want update this Client ? y/n ";
        cin >> Answer;
        if (toupper(Answer) == 'Y') {
            for (stClient& C : vClients) {
                if (C.AccountNumber == AccountNumber) {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }
            SaveClientDataToFile(ClientsFileName, vClients);
            cout << "Client Updated Successfully .";
            return true;
        }
    }
    else {
        cout << "Client With Account Number (" << AccountNumber << ") is Not Found.";
        return false;
    }

}
void ShowUpdateClientScreen() {
    cout << "--------------------------------------------------\n\n";
    cout << "\t\tDelete Client ";
    cout << "\n\n--------------------------------------------------";
    vector<stClient>vClients;
    vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}
void ShowFindClientScreen() {
    cout << "--------------------------------------------------\n\n";
    cout << "\t\tFind Client ";
    cout << "\n\n--------------------------------------------------";
    stClient Client;
    vector<stClient>vClients;
    vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "Client With Account Number (" << AccountNumber << ") is Not Found.";

}
void ShowEndScreen() {
    cout << "--------------------------------------------------\n\n";
    cout << "\t\tProgram Ends :-) ";
    cout << "\n\n--------------------------------------------------";
}
void PerformMainMenuOption(enMainMenu MainMenuOption) {
    switch (MainMenuOption) {
    case enMainMenu::eClientsList:
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    case enMainMenu::eAddNewClient:
        system("cls");
        ShowAddClientsScreen();
        GoBackToMainMenu();
        break;
    case enMainMenu::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenu::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenu::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenu::eShowTranscationMenu:
        system("cls");
        ShowTransctionsMenu();
        break;
    case enMainMenu::eExit:
        system("cls");
        ShowEndScreen();

        break;
    }
}
void ShowMainMenu() {
    system("cls");
    cout << "==================================================";
    cout << "\n\t\tMain Menu Screen ";
    cout << "\n==================================================";
    cout << "\n\t[1] show Client List.";
    cout << "\n\t[2] Add New Client.";
    cout << "\n\t[3] Delete Client.";
    cout << "\n\t[4] Update Client Info.";
    cout << "\n\t[5] Find Client.";
    cout << "\n\t[6] Transactions.";
    cout << "\n\t[7] Exit.";
    cout << "\n==================================================";
    PerformMainMenuOption((enMainMenu)ReadMainMenuOption());
}
short ReadTranscationsOption() {
    short Choice;
    cout << "\nChoose what do you want to do ? [1 to 4] ?";
    cin >> Choice;
    return Choice;
}
void GoBackToTranscationsMenu() {
    cout << "\nPress any key to go back to Transactions Menu... ";
    system("pause>0");
    ShowTransctionsMenu();
}
bool DepositBalanceToClientByAccountNumber(string AccountNumber,double Amount,vector<stClient>&vClients) {
    

    char Answer = 'n';
    cout << "Are you sure you want perform this transaction ? y/n ";
    cin >> Answer;

    if (toupper(Answer) == 'Y') {


        for (stClient& C : vClients) {


            if (C.AccountNumber == AccountNumber) {

                C.AccountBalance += Amount;
                SaveClientDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully,New Balance = " << C.AccountBalance << endl;
                
                return true;

            }
        }
        return false;
    }

   
    
}
void ShowDespoitScreen() {

    cout << "\n\n--------------------------------------------------";
    cout << "\n\t\tDeposit Screen";
    cout << "\n--------------------------------------------------";


    vector<stClient>vClients;
    vClients = LoadClientsDataFromFile(ClientsFileName);

    string AccountNumber = ReadClientAccountNumber();
    stClient Client;
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        cout << "Client with [" << AccountNumber << "] dose not exist.";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount=0;
    cout << "\n\nPlease enter deposit amount ? ";
    cin >> Amount;


    DepositBalanceToClientByAccountNumber(AccountNumber,Amount, vClients);

}
void ShowWithdrawScreen() {

    cout << "\n\n--------------------------------------------------";
    cout << "\n\t\tWithdraw Screen";
    cout << "\n--------------------------------------------------";

    vector<stClient>vClients;
    vClients = LoadClientsDataFromFile(ClientsFileName);

    string AccountNumber = ReadClientAccountNumber();
    stClient Client;
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        cout << "Client with [" << AccountNumber << "] dose not exist.";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount;
    cout << "\n\nPlease enter withdraw amount ? ";
    cin >> Amount;


    while (Client.AccountBalance < Amount) {
        cout << "\nAmount Exceeds the balance, you can withdraw up : " << Client.AccountBalance;
        cout << "\nPlease enter another amount ? ";
        cin >> Amount;
    }


    DepositBalanceToClientByAccountNumber(AccountNumber, Amount*-1, vClients);


}
void PrintClientRecordBalanceLine(stClient Client) {
    cout << "\n| " << setw(15) << left << Client.AccountNumber
        << "| " << setw(40) << left << Client.Name
        << "| " << setw(12) << left << Client.AccountBalance;
}
void ShowTotalBalanceScreen() { 

    vector<stClient>vClients;
    vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\tBalance List (" << vClients.size() << ") Clients(s).";

    cout << "\n__________________________________________________________________________________\n\n";
    cout << "| " << setw(15) << left << "Account Number "
        << "| " << setw(40) << left << "Client Name"
        << "| " << setw(12) << left << "Balance";
    cout << "\n__________________________________________________________________________________\n";

    double TotalBalance = 0;

    if (vClients.size() == 0)
        cout << "\n\t\t\tNo Clients Available in the system !";
    else

      for (stClient& C : vClients) {
        PrintClientRecordBalanceLine(C);
        TotalBalance += C.AccountBalance;
    }
    cout << "\n\n__________________________________________________________________________________";
    cout << "\n\n\t\t\tTotal Balance = " << TotalBalance << endl << endl;

}
void PerformTranscationsOptions(enTranscationsMenuOptions TranscationsOption) {
    switch (TranscationsOption) {

    case enTranscationsMenuOptions::eDespoit:
        system("cls");
        ShowDespoitScreen();
        GoBackToTranscationsMenu();
        break;

    case enTranscationsMenuOptions::eWithdraw:
        system("cls");
        ShowWithdrawScreen();
        GoBackToTranscationsMenu();
        break;
    case enTranscationsMenuOptions::eShowTotalBalance:
        system("cls");
        ShowTotalBalanceScreen();
        GoBackToTranscationsMenu();
        break;
    case enTranscationsMenuOptions::eShowMainMenu:
       
        ShowMainMenu();
        break;

    }
}
void ShowTransctionsMenu() {
    system("cls");
    cout << "=========================================================";
    cout << "\n\t\tTransactions Menu Screen ";
    cout << "\n=========================================================";
    cout << "\n\t[1] Deposit.";
    cout << "\n\t[2] Withdraw.";
    cout << "\n\t[3] Total Balance.";
    cout << "\n\t[4] Main Menu.";
    cout << "\n=========================================================";
    PerformTranscationsOptions(enTranscationsMenuOptions(ReadTranscationsOption()));
}
int main()
{
    ShowMainMenu();
  
}