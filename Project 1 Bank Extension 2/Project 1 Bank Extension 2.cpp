#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cctype>


using namespace std;

const string ClientsFileName = "client_file.txt";
const string UsersFileName = "Users.txt";

enum enMainMenueOptions { enShowClient = 1, enAddClient = 2, enDeleteClient = 3, enUpdateClient = 4, enFindClient = 5, enTransactionClient = 6, enManageUsersMenue = 7, enExit = 8, };

enum enTransactionsMenueOption { enDepositClient = 1, enWithdrawClient = 2, enTotalBalancesClients = 3, enMainMenue = 4 };

enum enManageUsersMenueOptions { enListUsers = 1, enAddNewUser = 2, enDeleteUser = 3, enUpdateUser = 4, enFindUser = 5, enMainMenueOp = 6 };

enum enPermissions { enAll = -1, 
    enShowClientListPerr = 1, enAddClientPerr = 2, enDeleteClientPerr = 4, enUpdateClientPerr = 8, 
    enFindClientPerr = 16, enTransactionClientPerr = 32, enManageUsersMenuePerr = 64 };

struct sClient
{
    string AccountNumber;
    string Name;
    string Phone;
    double AccountBalance;
    int PinCode;
    bool MarkClientDelete = false;
};

struct stUser
{
    string Username;
    string Password;
    short Permissions;
    bool MarkUserDelete = false;
};

stUser CurrentUser;

void showMainMenueScreen();

bool ShowTransactionScreen();

bool ShowManageUsersScreen();

void ShowAccsessDeniedSceen()
{
    system("cls");
    cout << "-----------------------------------------";
    cout << "\nAccsess Denied,";
    cout << "\nYou Don\'t have perrmission to do this,";
    cout << "\nPlease Contact Your Admin,\n";
    cout << "-----------------------------------------\n\n";

    cout << "Press any key to  go back to main menue....";
    system("pause>0");
    
}

void  Login();

vector<string> SplitString(string S1, string seperator = "#//#")
{
    short pos = 0;
    string sword = "";
    vector<string> vsword;

    while ((pos = S1.find(seperator)) != string::npos)
    {
        sword = S1.substr(0, pos);

        if (sword != "")
        {
            vsword.push_back(sword);
        }
        S1.erase(0, (pos + seperator.length()));
    }

    if (S1 != "")
    {
        vsword.push_back(S1);
    }
    return vsword;
}

sClient ConvertLineToRecord(string Line)
{
    sClient Client;

    vector<string> vS1;

    vS1 = SplitString(Line);

    Client.AccountNumber = vS1[0];
    Client.PinCode = stoi(vS1[1]);
    Client.Name = vS1[2];
    Client.Phone = vS1[3];
    Client.AccountBalance = stod(vS1[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string seperator = "#//#")
{
    string S1;

    S1 = Client.AccountNumber + seperator
        + to_string(Client.PinCode) + seperator
        + Client.Name + seperator
        + Client.Phone + seperator
        + to_string(Client.AccountBalance);
    return S1;
}

stUser ConvertUserLineToRecord(string Line)
{
    vector<string> vS1 = SplitString(Line);

    stUser User;
    User.Username = vS1[0];
    User.Password = vS1[1];
    User.Permissions = stoi(vS1[2]);

    return User;
}

string ConvertUserRecordToLine(stUser User, string seperator = "#//#")
{
    string S1;
    S1 = User.Username + seperator;
    S1 += User.Password + seperator;
    S1 += to_string(User.Permissions);
    return S1;
}

vector<sClient> LoadClientsDataFromFile(string FileName)
{
    fstream Fstream;
    vector<sClient> vsClients;
    sClient Client;

    Fstream.open(FileName, ios::in);

    if (Fstream.is_open())
    {
        string Line;
        while (getline(Fstream, Line))
        {
            Client = ConvertLineToRecord(Line);
            vsClients.push_back(Client);
        }
        Fstream.close();
    }
    return vsClients;
}

vector<stUser> LoadusersDataFromFile(string FileName)
{
    fstream UserFile;
    vector<stUser> vUsers;

    UserFile.open(FileName, ios::in);

    if (UserFile.is_open())
    {
        string Line = "";
        stUser User;

        while (getline(UserFile, Line))
        {
            User = ConvertUserLineToRecord(Line);
            vUsers.push_back(User);
        }
        UserFile.close();
    }
    UserFile.close();
    return vUsers;
}

stUser GetUserByUsernameAndPassword(string Username, string Password)
{
    vector<stUser> vusers = LoadusersDataFromFile(UsersFileName);
    stUser User;

    for (stUser& U : vusers)
    {
        if (U.Username == Username && U.Password == Password)
        {
            User = U;
            return User;
        }
    }
    return User;
}
short ReadPermissionsToSet()
{
    char Answer = ' ';

    cout << "\nDo You want to give Full Access ? y/n: ";
    cin >> Answer;

    if (tolower(Answer) == 'y')
    {
        return -1;
    }

    short CountPerrmissions = 0;

    cout << "\nDo You want to give access to:\n\n";

    cout << "\nshow Client List y/n ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        CountPerrmissions += (enPermissions::enShowClientListPerr | 0);
        Answer = false;
    }

    cout << "\nAdd new Client y/n ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        CountPerrmissions += (enPermissions::enAddClientPerr | 0);
    }
    cout << "\nDelete Client y/n ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        CountPerrmissions += (enPermissions::enDeleteClientPerr | 0);
    }
    cout << "\nUpdate Client y/n ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        CountPerrmissions += (enPermissions::enUpdateClientPerr | 0);
    }
    cout << "\nFind Client y/n ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        CountPerrmissions += (enPermissions::enFindClientPerr | 0);
    }
    cout << "\nTransaction Client y/n ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        CountPerrmissions += (enPermissions::enTransactionClientPerr | 0);
    }
    cout << "\nManage Users y/n ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        CountPerrmissions += (enPermissions::enManageUsersMenuePerr | 0);
    }
    return CountPerrmissions;

}

bool CheckPerrmission(enPermissions Permission)
{
    if (CurrentUser.Permissions == enPermissions::enAll)
        return true;

    //return (Number & (short)Permission);
    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintClientRecord(sClient Client)
{

    cout << "\nThe following are the client details:";
    cout << "\n-----------------------------------\n";
    cout << "\AccountNumber: " << Client.AccountNumber;
    cout << "\nPinCode: " << Client.PinCode;
    cout << "\nName: " << Client.Name;
    cout << "\nPhone: " << Client.Phone;
    cout << "\nAccountBalance: " << Client.AccountBalance;
    cout << "\n-----------------------------------";
}

void PrintUserRecordLine(stUser User)
{
    cout << "| " << setw(20) << left << User.Username;
    cout << "| " << setw(15) << left << User.Password;
    cout << "| " << setw(15) << left << User.Permissions;  
}

void PrintUserRecord(stUser User)
{
    cout << "\nThe following are the User details:";
    cout << "\n-----------------------------------\n";
    cout << "\nUsername: " << User.Username;
    cout << "\nPassword: " << User.Password;
    cout << "\nPermissions: " << User.Permissions;
    cout << "\n-----------------------------------";
}

void PrintUserRecordByUserName(vector<stUser> vUsers, string Username)
{
    for (stUser& U : vUsers)
    {
        if (U.Username == Username)
        {
            PrintUserRecord(U);
            break;
        }
    }
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line = "";
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            if (AccountNumber == Client.AccountNumber)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

bool FindClientByAccountNumber(sClient& Client, string AccountNumber, vector<sClient> vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool ShowAllClientsScreen()
{
    if (!CheckPerrmission(enPermissions::enShowClientListPerr))
    {
        ShowAccsessDeniedSceen();
        return false;
    }
    vector<sClient>  vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Clients.";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (sClient& C : vClients)
        {

            PrintClientRecordLine(C);
            cout << endl;
        }
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    return true;
}

bool FindUserByUsernameAndPassword(string Username, string password, stUser& User)
{
    vector<stUser> vUsers = LoadusersDataFromFile(UsersFileName);

    for (stUser& U: vUsers)
    {
        if (U.Username == User.Username && U.Password == User.Password)
        {
            User = U;
            return true;
        }
    }
    return false;
}

bool UserExistByUsername(string Username)
{
    fstream UserFile; 

    UserFile.open(UsersFileName, ios::in);
    
    if (UserFile.is_open())
    {
        string Line;
        stUser User;
        while (getline(UserFile, Line))
        {
            User = ConvertUserLineToRecord(Line);

            if (User.Username == Username)
            {
                UserFile.close();
                return true;

            }
        }
        UserFile.close();
        return false;
    }
}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient Wiht [ " << Client.AccountNumber << " ] is already exists, Enter Another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    cin >> Client.PinCode;

    cout << "Enter Name? ";
    getline(cin >> ws, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Enter PinCode? ";
    cin >> Client.PinCode;

    cout << "Enter Name? ";
    getline(cin >> ws, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

void AddClientDatatoFile(sClient Client, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::app);

    if (MyFile.is_open())
    {
        string S1 = "";

        S1 = ConvertRecordToLine(Client);
        MyFile << S1 << endl;

        MyFile.close();
    }
}

void AddUserDataToFile(string FileName, string UserLine)
{
    fstream MyFile;

    MyFile.open(FileName, ios::app);

    if (MyFile.is_open())
    {
        MyFile << UserLine << endl;
        MyFile.close();
    }

}

void SaveUserDateToFile(vector<stUser> vUsers, string FileName)
{
    fstream UserFile;

    UserFile.open(FileName, ios::out);

    if (UserFile.is_open())
    {
        string Line = "";

        for (stUser& U : vUsers)
        {
            if (!U.MarkUserDelete )
            {
                UserFile << ConvertUserRecordToLine(U) << endl;
            }
        }  
        UserFile.close();
    }
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddClientDatatoFile(Client, ClientsFileName);
}

bool AddNewClients()
{
    if (!CheckPerrmission(enPermissions::enAddClientPerr))
    {
        ShowAccsessDeniedSceen();
        return false;
    }
    cout << "===========================================\n";
    cout << "\t\Add New Clients Screen\n";
    cout << "===========================================\n";

    char AddMore = ' ';

    do
    {
        cout << "\nAdd new Client:\n\n";

        AddNewClient();

        cout << "\n Client Added Successfully, do you want to add more client? Y/N? ";
        cin >> AddMore;

    } while (tolower(AddMore) == 'y');
    return true;
}

stUser ReadNewUser()
{
    stUser User;

    cout << "Enter username? ";
    getline(cin >> ws, User.Username);

    while (UserExistByUsername(User.Username))
    {
        cout<< "User with ["<< User.Username << "] Allrady exists ,Enter another username? ";
        getline(cin >> ws, User.Username);
        
    }
    
    cout << "Enter password? ";
    getline(cin, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

stUser ChangeUserRecord(string Username)
{
    stUser User;
    //
    cout << "Enter username? ";
    getline(cin >> ws, User.Username);

    while (UserExistByUsername(User.Username))
    {
        cout << "User with [" << User.Username << "] Allrady exists ,Enter another username? ";
        getline(cin >> ws, User.Username);

    }
    //User.Username = Username;

    cout << "Enter password? ";
    getline(cin, User.Password);

    User.Permissions = ReadPermissionsToSet();
    return User;
}

string ReadUsername()
{
    string Username = "";
    cout << "\nEnter Username? ";
    getline(cin >> ws, Username);

    return Username;
}

void AddNewUser()
{
    stUser User;
    User = ReadNewUser();
    AddUserDataToFile(UsersFileName, ConvertUserRecordToLine(User));
}
void AddNewUsers()
{
    char AddMore = ' ';

    do
    {
        cout << "\nAdd new User:\n\n";
        AddNewUser();

        cout << "\n User Added Successfully, do you want to add more User? Y/N? ";
        cin >> AddMore;

    } while (tolower(AddMore) == 'y');
}
bool MarkUserDeleteByUsernameAndPassword(vector<stUser>& vUsers, string Username)
{
    for (stUser& element : vUsers)
    {
        if (element.Username == Username)
        {
            element.MarkUserDelete = true;
            return true;
        }
    }
    return false;
}

bool DeleteUserByUsernameAndPassword(vector<stUser>& vUsers, string Username)
{
    char Answer = ' ';
    if (UserExistByUsername(Username))
    {      
        if (Username == "Admin")
        {
            cout << "\n\nYou cannot Delete this user.\n";
            return true;
        }
        else
        {
            PrintUserRecordByUserName(vUsers, Username);

            cout << "\n\nAre you sure you want delete this user? y/n? ";
            cin >> Answer;

            if (tolower(Answer) == 'y')
            {
                MarkUserDeleteByUsernameAndPassword(vUsers, Username);
                SaveUserDateToFile(vUsers, UsersFileName);

                vUsers = LoadusersDataFromFile(UsersFileName);

                cout << "\n\nUser Deleted Successfully,";
                return true;
            }
        }
    }
    else
    {
        cout << "\User with Username (" << Username << ") is Not Found!";
        return false;
    }
}
bool UpdateUserByUserName(vector<stUser>& vUsers, string Username)
{
    char Answer = 'n';

    if (UserExistByUsername(Username))
    {
        PrintUserRecordByUserName(vUsers, Username);

        cout << "\n\nAre you sure you want Update this user? y/n? ";
        cin >> Answer;
                
        if (tolower(Answer) == 'y')
        {
            ChangeUserRecord(Username);

            SaveUserDateToFile(vUsers, UsersFileName);

            vUsers = LoadusersDataFromFile(UsersFileName);

            cout << "\n\nUser Updated Successfully,";
            return true;
        }
    }
    else
    {
        cout << "\User with Username (" << Username << ") is Not Found!";
        return false;
    }
}

bool FindUserByUserName(vector<stUser>& vUsers, string Username)
{
    char Answer = 'n';

    if (UserExistByUsername(Username))
    {
        PrintUserRecordByUserName(vUsers, Username);
        return true;
     }
    else
    {
        cout << "\User with Username (" << Username << ") is Not Found!";
        return false;
    }

}

void ShowAllUsersScreen()
{
    vector<stUser> vUsers = LoadusersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tClient List (" << vUsers.size() << ") Clients.";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "|  " << setw(20) << left << "User Name";
    cout << "|  " << setw(15) << left << "Password";
    cout << "|  " << setw(15) << left << "Permission";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    if (vUsers.size() == 0)
    {
        cout << "\t\t\t\tNo Users Available In the System!";
    }
    else
    {
        for (stUser& Us : vUsers)
        {
            PrintUserRecordLine(Us);
            cout << endl;
        }
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void ShowAddNewUsersScreen()
{
    cout << "===========================================\n";
    cout << "\tAdd New user Screen\n";
    cout << "===========================================\n";

    AddNewUsers();

}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Users Screen\n";
    cout << "\n-----------------------------------\n";
     
    string UserName = ReadUsername();
    vector<stUser> vUsers = LoadusersDataFromFile(UsersFileName);
    DeleteUserByUsernameAndPassword(vUsers, UserName);
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Screen\n";
    cout << "\n-----------------------------------\n";

    string UserName = ReadUsername();
    vector<stUser> vUsers = LoadusersDataFromFile(UsersFileName);
    UpdateUserByUserName(vUsers, UserName);
}
void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\Find User Screen\n";
    cout << "\n-----------------------------------\n";

    string UserName = ReadUsername();
    vector<stUser> vUsers = LoadusersDataFromFile(UsersFileName);
    FindUserByUserName(vUsers, UserName);
}

string ReadAccountNumber()
{
    string AccountNumber = "";

    cout << "Enter Account Number? ";
    getline(cin >> ws, AccountNumber);

    return AccountNumber;
}

void SaveClientDataToFile(vector<sClient>& vClients)
{
    fstream MyFile;

    MyFile.open(ClientsFileName, ios::out);

    if (MyFile.is_open())
    {
        string Line = "";

        for (sClient& C : vClients)
        {
            if (C.MarkClientDelete == false)
            {
                Line = ConvertRecordToLine(C);

                MyFile << Line << endl;
            }
        }
        MyFile.close();
    }
}

bool MarkClientDeleteByAccountNumber(vector<sClient>& vClients, string AccountNumber)
{
    for (sClient& C : vClients)
    {
        if (AccountNumber == C.AccountNumber)
        {
            C.MarkClientDelete = true;
            return C.MarkClientDelete;
        }
    }
    return false;
}

bool DeleteClientByAccountNumber(vector<sClient>& vClients, string AccountNumber)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(Client, AccountNumber, vClients))
    {
        PrintClientRecord(Client);

        cout << "\nAre you sure you want delete this client? y/n? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            MarkClientDeleteByAccountNumber(vClients, AccountNumber);
            SaveClientDataToFile(vClients);
            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\n Client Deleted successfully,";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber
            << ") is Not Found!";
        return false;
    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(Client, AccountNumber, vClients))
    {
        PrintClientRecord(Client);

        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            for (sClient& C : vClients)
            {
                if (Client.AccountNumber == C.AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientDataToFile(vClients);
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

double ReadAmountNumber(string message)
{
    double Amount = 0;

    cout << message;
    cin >> Amount;

    return Amount;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<sClient> vClient)
{
    char answer = 'n';

    cout << "\nAre yoy sure you want perfrom this transaction? Y/N ? ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
    {
        for (sClient& C : vClient)
        {
            if (AccountNumber == C.AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientDataToFile(vClient);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
        return false;
    }
}

bool ShowDeleteClientScreen()
{
    if (!CheckPerrmission(enPermissions::enDeleteClientPerr))
    {
        ShowAccsessDeniedSceen();
        return false;
    }
    cout << "\n-----------------------------------\n";
    cout << "\t\Delete Clients Screen\n";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    DeleteClientByAccountNumber(vClients, AccountNumber);

    return true;
}

bool ShowUpdateClientScreen()
{
    if (!CheckPerrmission(enPermissions::enUpdateClientPerr))
    {
        ShowAccsessDeniedSceen();
        return false;
    }
    cout << "\n-----------------------------------\n";
    cout << "\t\tUpdate Client info Screen\n";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

    return true;
}

bool ShowFindClientScreen()
{
    if (!CheckPerrmission(enPermissions::enFindClientPerr))
    {
        ShowAccsessDeniedSceen();
        return false;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    sClient Client;

    if (FindClientByAccountNumber(Client, AccountNumber, vClients))
        PrintClientRecord(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

    return true;
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadAccountNumber();


    //IsClientExist = TransactionClientByAccountNumber(AccountNumber, vClients, true);
    while (!FindClientByAccountNumber(Client, AccountNumber, vClients))
    {
        cout << "\nClient with [" << AccountNumber << "] does nont exist.\n";
        AccountNumber = ReadAccountNumber();
    }

    PrintClientRecord(Client);

    double amount = 0;

    cout << "\nPlease enter deposit amount? ";
    cin >> amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, amount, vClients);



}

void ShowWithdrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadAccountNumber();

    while (!FindClientByAccountNumber(Client, AccountNumber, vClients))
    {
        cout << "\nClient with [" << AccountNumber << "] does nont exist.\n";
        AccountNumber = ReadAccountNumber();
    }

    PrintClientRecord(Client);

    double Amount = 0;

    cout << "\nPlease enter Withdraw amount ?";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * (-1), vClients);

}

double CalculateTotalOfAllBalancesAmount(vector<sClient> vClients)
{
    double SumBalances = 0;

    for (sClient& C : vClients)
    {
        SumBalances += C.AccountBalance;
    }
    return SumBalances;
}

void ShowTotalBalancesScreen()
{
    vector<sClient>  vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Clients.";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (sClient& Client : vClients)
        {

            cout << "| " << setw(15) << left << Client.AccountNumber;
            cout << "| " << setw(40) << left << Client.Name;
            cout << "| " << setw(12) << left << Client.AccountBalance;
            cout << endl;
        }
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "\t\t\t Total Balances = " << CalculateTotalOfAllBalancesAmount(vClients) << endl;
}

void GoBackToTransactionScreen()
{
    cout << "\n\nPress any key to go back to Transaction Menue...";
    system("pause>0");
    ShowTransactionScreen();
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    showMainMenueScreen();
}

void GoBackToManageUsersMenue()
{
    cout << "\n\nPress any key to go back to Manage Users Menue...\n";
    system("pause>0");
    ShowManageUsersScreen();
}

short ReadMainMenueOptions()
{
    cout << "Choose what do you want to do? [1 to 8]? ";

    short choice = 0;
    cin >> choice;
    return choice;

}

short ReadTransactionMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";

    short choice = 0;
    cin >> choice;
    return choice;

}

short ReadManageUsersMenueOptions()
{
    cout << "Choose what do you want to do? [1 to 6]? ";

    short choice = 0;
    cin >> choice;
    return choice;

}

void PerformTransactionMenueOption(enTransactionsMenueOption  TransactionsMenueOption)

{
    switch (TransactionsMenueOption)
    {
    case enTransactionsMenueOption::enDepositClient:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionScreen();
        break;
    }
    case enTransactionsMenueOption::enWithdrawClient:
    {
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionScreen();
        break;
    }
    case enTransactionsMenueOption::enTotalBalancesClients:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionScreen();
        break;
    }
    case enTransactionsMenueOption::enMainMenue:
    {
        showMainMenueScreen();
    }
    }
}

void PerformManageUsersMenueOptions(enManageUsersMenueOptions ManageUsersMenueOptions)
{
    switch (ManageUsersMenueOptions)
    {
    case enManageUsersMenueOptions::enListUsers:
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::enAddNewUser:
        system("cls");
        ShowAddNewUsersScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::enDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::enUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::enFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::enMainMenueOp:
        system("cls");
        showMainMenueScreen();
        break;

    }
}

bool ShowTransactionScreen()
{
    if (!CheckPerrmission(enPermissions::enTransactionClientPerr))
    {
        ShowAccsessDeniedSceen();
        return false;
    }
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransaction Menus Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";

    PerformTransactionMenueOption((enTransactionsMenueOption)ReadTransactionMenueOption());
    //return true;
}

bool ShowManageUsersScreen()
{
    if (!CheckPerrmission(enPermissions::enManageUsersMenuePerr))
    {
        ShowAccsessDeniedSceen();
        return false;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\Manage User Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New users.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";

    PerformManageUsersMenueOptions((enManageUsersMenueOptions)ReadManageUsersMenueOptions());
    return true;

}

void PerformMainMenueOption(enMainMenueOptions MainMenueOptions)
{
    stUser User = GetUserByUsernameAndPassword(CurrentUser.Username, CurrentUser.Password);
    switch (MainMenueOptions)
    {
    case enMainMenueOptions::enShowClient:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::enAddClient:
    {
        system("cls");
        AddNewClients();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::enDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::enUpdateClient:
    {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::enFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::enTransactionClient:
    {
        system("cls");
        ShowTransactionScreen();
        break;
    }
    case enMainMenueOptions::enManageUsersMenue:
    {
        system("cls");
        ShowManageUsersScreen();
        break;
    }
    case enMainMenueOptions::enExit:
    {
        system("cls");
        //ShowEndScreen();
        Login();
        break;
    }
    }
}

bool LoadUserInfo(string Username, string Password)
{
    if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
        return true;
    
    else
        return false;
}

void showMainMenueScreen()
{    
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transaction Client.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";

    PerformMainMenueOption((enMainMenueOptions)ReadMainMenueOptions());
}

void Login()
{
   
    bool LoginFaild = false;

    string Username, Password;

    do
    {
        system("cls");
    
        cout << "===========================================\n";
        cout << "\tLogin Screen\n";
        cout << "===========================================\n";

        if (LoginFaild)
        {
            cout << "Invlaid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> Username;
        cout << "Enter Password? ";
        cin >> Password;
         
        LoginFaild = !LoadUserInfo(Username, Password);

    } while (LoginFaild);

    showMainMenueScreen();
}

int main()
{
    //SplitString("A150#//#1234#//#mohamed ashraf#//#+201111111111#//#1500.000000", "#//#");
    Login();
   
    system("pause>0");
    return 0;
}

