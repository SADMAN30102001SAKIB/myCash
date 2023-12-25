#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <chrono>
#include <fstream>
#include <cstdio>
using namespace std;

#define MAX_SIZE 100

int totalMember = 0;

struct trans
{
    string description;
    double amount;
    double balance;
};

struct myCashMember
{
    string mobileNo;
    string name;
    double amount;
    string pin;
    int totalTrans;
    struct trans transaction[MAX_SIZE];
} mcmembers[MAX_SIZE];

class History
{
private:
    struct Transaction
    {
        int tranID;
        string description;
        double amount;
        double balance;
    };

    Transaction transactions[MAX_SIZE];
    int lastTransactionID;
    int transactionNo;

public:
    History() : lastTransactionID(101), transactionNo(0) {}

    int getTransactionNo() const { return transactionNo; }
    void incrementTransactionNo() { transactionNo++; }

    void addTransaction(const string &desc, double amt, double bal, int no)
    {
        Transaction newTransaction;
        newTransaction.tranID = lastTransactionID++;
        newTransaction.description = desc;
        newTransaction.amount = amt;
        newTransaction.balance = bal;
        transactions[no] = newTransaction;
    }

    void displayTransactionHistory(int no)
    {
        cout << "\nTransaction History: " << endl;

        int count = 0;
        for (int i = 0; i < no; i++)
        {
            cout << "ID: " << transactions[i].tranID << " | Description: " << transactions[i].description
                 << " | Amount: " << transactions[i].amount << " | Balance: " << transactions[i].balance << endl;
            count++;
        }

        if (!count)
        {
            cout << "No History!"
                 << "\n";
        }
    }
};

int generateOTP();
int verifyOTP(int otp);

class Member
{
private:
    string mobileNo;
    string name;
    double amount;
    string pin;
    History transactionHistory;
    static int memberNo;

public:
    Member() : amount(0.0) {}
    Member(string mobile, string n, double a, string p) : mobileNo(mobile), name(n), amount(a), pin(p) {}

    int getMemberNo() const { return memberNo; }
    void incrementMemberNo() { memberNo++; }
    void decrementMemberNo() { memberNo--; }

    string getMobileNo() const { return mobileNo; }
    string getName() const { return name; }
    double getAmount() const { return amount; }
    string getPin() const { return pin; }
    History &getTransactionHistory() { return transactionHistory; }

    void setMobileNo(string mobile) { mobileNo = mobile; }
    void setName(string n) { name = n; }
    void setAmount(double a) { amount = a; }
    void setPin(string p) { pin = p; }

    void updateMember(int index)
    {
        cout << "Old Name: " << getName() << endl;

        string newName;

        cout << "New Name: ";

        cin.ignore();
        cin >> newName;

        string oldPin;
        string newPin, confirmNewPin;

        cout << "Old pin: ";

        for (int i = 0; i < 5; i++)
        {
            char c;
        ostart:
            c = getch();

            if (c >= '0' && c <= '9')
            {
                oldPin += c;
            }
            else
            {
                goto ostart;
            }

            cout << "*";
        }

        if (oldPin == getPin())
        {
            int flag = 0;

            cout << "\nNew pin: ";

            cin.ignore();

            for (int i = 0; i < 5; i++)
            {
                char c;
            nstart:
                c = getch();

                if (c >= '0' && c <= '9')
                {
                    newPin += c;
                }
                else
                {
                    goto nstart;
                }
                cout << "*";
            }

            if (!newPin.empty())
            {
                cout << "\nConfirm New pin: ";

                for (int i = 0; i < 5; i++)
                {
                    char c;
                cstart:
                    c = getch();

                    if (c >= '0' && c <= '9')
                    {
                        confirmNewPin += c;
                    }
                    else
                    {
                        goto cstart;
                    }
                    cout << "*";
                }

                if (newPin == confirmNewPin)
                {
                    int otp = generateOTP();
                    int f = verifyOTP(otp);

                    if (f == -1)
                    {
                        cout << "\n\nTime Out. Update canceled."
                             << endl;
                    }
                    else if (f)
                    {
                        setPin(newPin);
                        mcmembers[index].pin = newPin;

                        if (!newName.empty())
                        {
                            setName(newName);
                            mcmembers[index].name = newName;
                        }
                        cout << "\n\nUpdate is Successful" << endl;
                    }
                    else
                    {
                        cout << "\n\nIncorrect OTP. Update canceled."
                             << endl;
                    }
                }
                else
                {
                    cout << "\n\nPINs must be equal. PIN not change!" << endl;
                }
            }
        }
        else
        {
            cout << "\n\nInvalid PIN. Please try again."
                 << endl;
        }
    }
};

int Member::memberNo = 0;

void displayLoginMenu();
void displayMainMenu(Member &loggedInMember, Member members[], int &index);
void registerMember(Member members[]);
void removeMember(int &index, Member members[]);
Member *loginMember(Member members[], int &index);
void saveToFile();
void sendMoney(Member &loggedInMember, Member members[], int index);
void cashIn(Member &loggedInMember, int index);
void cashOut(Member &loggedInMember, int index);
void payBill(Member &loggedInMember, int index);
void checkBalance(Member &loggedInMember);
void viewTransactionHistory(History &transactionHistory);

int main()
{
    Member members[MAX_SIZE];

    ifstream inputFile("data.txt");

    if (!inputFile.is_open())
    {
        ofstream outputFile("data.txt");
        outputFile.close();
    }
    else
    {
        while (totalMember < MAX_SIZE && inputFile >> mcmembers[totalMember].name >> mcmembers[totalMember].mobileNo >> mcmembers[totalMember].amount >> mcmembers[totalMember].pin >> mcmembers[totalMember].totalTrans)
        {
            if (mcmembers[totalMember].totalTrans)
            {
                string s = "TH" + mcmembers[totalMember].mobileNo + ".txt";
                ifstream inputFile(s);

                if (!inputFile.is_open())
                {
                    mcmembers[totalMember].totalTrans = 0;
                }
                else
                {
                    int i = 0;
                    while (i < mcmembers[totalMember].totalTrans && inputFile >> mcmembers[totalMember].transaction[i].amount >> mcmembers[totalMember].transaction[i].balance >> mcmembers[totalMember].transaction[i].description)
                    {
                        i++;
                    }
                }

                inputFile.close();
            }
            totalMember++;
        }

        inputFile.close();
    }

    for (int i = 0; i < totalMember; i++)
    {
        members[i].setMobileNo(mcmembers[i].mobileNo);
        members[i].setPin(mcmembers[i].pin);
        members[i].setName(mcmembers[i].name);
        members[i].setAmount(mcmembers[i].amount);
        members[0].incrementMemberNo();

        if (mcmembers[i].totalTrans)
        {
            for (int j = 0; j < mcmembers[i].totalTrans; j++)
            {
                members[i].getTransactionHistory().addTransaction(mcmembers[i].transaction[j].description, mcmembers[i].transaction[j].amount, mcmembers[i].transaction[j].balance, members[i].getTransactionHistory().getTransactionNo());
                members[i].getTransactionHistory().incrementTransactionNo();
            }
        }
    }

    bool exitProgram = false;

    while (!exitProgram)
    {
        displayLoginMenu();

        int loginOption;
        cin >> loginOption;
        cin.ignore();

        switch (loginOption)
        {
        case 1:
        {
            int index;
            Member *loggedInMember = loginMember(members, index);
            if (loggedInMember)
            {
                displayMainMenu(*loggedInMember, members, index);
            }

            break;
        }
        case 2:
            registerMember(members);
            break;
        case 3:
            exitProgram = true;
            saveToFile();
            break;
        default:
            cout << "\nInvalid Option. Please try again.\n"
                 << endl;
        }
    }

    return 0;
}

void saveToFile()
{
    ofstream outputFile("data.txt");

    int i = 0;
    while (i < totalMember && outputFile << mcmembers[i].name << " " << mcmembers[i].mobileNo << " " << mcmembers[i].amount << " " << mcmembers[i].pin << " " << mcmembers[i].totalTrans << endl)
    {
        if (mcmembers[i].totalTrans)
        {
            string s = "TH" + mcmembers[i].mobileNo + ".txt";
            ofstream outputFile(s);

            int j = 0;

            while (j < mcmembers[i].totalTrans && outputFile << mcmembers[i].transaction[j].amount << " " << mcmembers[i].transaction[j].balance << " " << mcmembers[i].transaction[j].description << endl)
            {
                j++;
            }

            outputFile.close();
        }

        i++;
    }

    outputFile.close();
}

void displayLoginMenu()
{
    cout << "*** MyCash Login ***" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter Your Option: ";
}

void displayMainMenu(Member &loggedInMember, Member members[], int &index)
{
    bool logout = false;

    while (!logout)
    {
        cout << "********** MyCash Menu **********" << endl;
        cout << "1. Update Me" << endl;
        cout << "2. Remove Me" << endl;
        cout << "3. Send Money" << endl;
        cout << "4. Cash-in" << endl;
        cout << "5. Cash-out" << endl;
        cout << "6. Pay Bill" << endl;
        cout << "7. Check Balance" << endl;
        cout << "8. History" << endl;
        cout << "9. Logout" << endl;
        cout << "Enter Your Option (1-9): ";

        int option;
        cin >> option;

        switch (option)
        {
        case 1:
            loggedInMember.updateMember(index);
            break;
        case 2:
            removeMember(index, members);
            logout = true;
            break;
        case 3:
            sendMoney(loggedInMember, members, index);
            break;
        case 4:
            cashIn(loggedInMember, index);
            break;
        case 5:
            cashOut(loggedInMember, index);
            break;
        case 6:
            payBill(loggedInMember, index);
            break;
        case 7:
            checkBalance(loggedInMember);
            break;
        case 8:
            viewTransactionHistory(loggedInMember.getTransactionHistory());
            break;
        case 9:
            logout = true;
            cout << "\nLogged out successfully."
                 << endl;
            break;
        default:
            cout << "\nInvalid Option. Please try again."
                 << endl;
        }

        cout << "\nPress any key go to main menu....";

        cin.ignore();
        cin.get();
        cout << "\n";
    }
}

int generateOTP()
{
    srand(static_cast<unsigned>(time(0)));
    int r = rand() % 9000 + 1000;

    cout << "\nGenerated OTP: " << r << "\n";

    return r;
}

int verifyOTP(int otp)
{
    int enteredOTP;

    cout << "Enter OTP under 10 seconds: ";

    auto start_time = chrono::high_resolution_clock::now();

    cin >> enteredOTP;

    auto end_time = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end_time - start_time;
    double execution_time = duration.count();

    if (execution_time > 10)
    {
        return -1;
    }

    if (enteredOTP == otp)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void registerMember(Member members[])
{
    string mobileNo, name, pin, rePin;

    cout << "\nEnter Mobile No. (11-digit): ";

    cin >> mobileNo;

    if (mobileNo[0] != '0' || mobileNo[1] != '1' || mobileNo.length() != 11)
    {
        cout << "\nInvalid number."
             << "\n\n";
        return;
    }

    for (int i = 0; i < members[0].getMemberNo(); i++)
    {
        if (mobileNo == members[i].getMobileNo())
        {
            cout << "\nMember already exists"
                 << "\n";
            return;
        }
    }

    cout << "Enter Name: ";

    cin.ignore();
    getline(cin, name);

    cout << "Enter 5 digit PIN: ";                                               

    for (int i = 0; i < 5; i++)
    {
        char c;
    pstart:
        c = getch();
        if (c >= '0' && c <= '9')
        {
            pin += c;
        }
        else
        {
            goto pstart;
        }
        cout << "*";
    }

    cout << "\nReconfirm PIN: ";

    for (int i = 0; i < 5; i++)
    {
        char c;
    rstart:
        c = getch();
        if (c >= '0' && c <= '9')
        {
            rePin += c;
        }
        else
        {
            goto rstart;
        }
        cout << "*";
    }

    if (pin == rePin)
    {
        int otp = generateOTP();
        int f = verifyOTP(otp);

        if (f == -1)
        {
            cout << "\nTime Out. Update canceled."
                 << endl;
        }
        else if (f)
        {
            members[members[0].getMemberNo()].setMobileNo(mobileNo);
            members[members[0].getMemberNo()].setPin(pin);
            members[members[0].getMemberNo()].setName(name);

            mcmembers[totalMember].amount = 0;
            mcmembers[totalMember].mobileNo = mobileNo;
            mcmembers[totalMember].pin = pin;
            mcmembers[totalMember].name = name;

            members[0].incrementMemberNo();
            totalMember++;

            cout << "\nRegistration Successful.\n"
                 << endl;
        }
        else
        {
            cout << "\nRegestration canceled. Incorrect OTP.\n"
                 << endl;
        }
    }
    else
    {
        cout << "\nPINs must be equal. Try again!\n"
             << "\n";
        return;
    }
}

Member *loginMember(Member members[], int &index)
{
    Member *loggedInMember = nullptr;

    cout << "\nEnter Mobile No. (11-digit): ";

    string mobile;
    cin >> mobile;
    int isLogin = 0;

    for (int i = 0; i < members[0].getMemberNo(); i++)
    {
        if (mobile == members[i].getMobileNo())
        {
            isLogin = 1;
            string enteredPin;

            cout << "Enter your 5 digit PIN: ";

            for (int i = 0; i < 5; i++)
            {
                char c;
            estart:
                c = getch();
                if (c >= '0' && c <= '9')
                {
                    enteredPin += c;
                }
                else
                {
                    goto estart;
                }
                cout << "*";
            }

            if (enteredPin == members[i].getPin())
            {
                index = i;

                cout << "\nLogin Successful.\n"
                     << endl;

                loggedInMember = &members[i];
                return loggedInMember;
            }
            else
            {
                cout << "\nInvalid PIN. Login Failed. Please try again.\n"
                     << endl;

                return loggedInMember;
            }
        }
    }
    if (!isLogin)
    {
        cout << "\nMember not found. Login Failed. Please try again.\n"
             << endl;

        return loggedInMember;
    }
}

void removeMember(int &index, Member members[])
{
    int otp = generateOTP();
    int f = verifyOTP(otp);

    if (f == -1)
    {
        cout << "\nTime Out. Deletion canceled."
             << endl;
    }
    else if (f)
    {
        string mobileNo = mcmembers[index].mobileNo;
        string filename = "TH" + mobileNo + ".txt";
        const char *c_filename = filename.c_str();
        remove(c_filename);

        for (int i = index; i < members[0].getMemberNo() - 1; i++)
        {
            members[i] = members[i + 1];
            mcmembers[i] = mcmembers[i + 1];
        }

        members[0].decrementMemberNo();
        totalMember--;

        cout << "Account removed successfully."
             << endl;
    }
    else
    {
        cout << "Incorrect OTP. Removal canceled."
             << endl;
    }
}

void sendMoney(Member &loggedInMember, Member members[], int index)
{
    cout << "\nEnter recipient's Mobile No. (11-digit): ";

    string recipientMobile;
    cin >> recipientMobile;

    bool recipientFound = false;
    int rindex;
    Member *recipientMember = nullptr;

    for (int i = 0; i < members[0].getMemberNo(); i++)
    {
        if (recipientMobile == members[i].getMobileNo() && recipientMobile != loggedInMember.getMobileNo())
        {
            recipientMember = &members[i];
            rindex = i;
            recipientFound = true;
            break;
        }
    }

    if (!recipientFound)
    {
        cout << "\nRecipient not found. Please check the Mobile No."
             << endl;
        return;
    }

    cout << "Enter amount to send: ";

    double amount;
    cin >> amount;

    if (amount <= 0)
    {
        cout << "\nInvalid amount. Please enter a positive value."
             << endl;
        return;
    }

    if (loggedInMember.getAmount() < amount)
    {
        cout << "\nInsufficient balance. Transaction canceled."
             << endl;
        return;
    }

    cout << "Sending " << amount << " to " << recipientMobile << "\n";

    cout << "Want to pay(Y/N)? ";

    char c;
    cin >> c;

    if (c == 'Y')
    {
        int otp = generateOTP();
        int f = verifyOTP(otp);

        if (f == -1)
        {
            cout << "\nTime Out. Update canceled."
                 << endl;
        }
        else if (f)
        {
            loggedInMember.setAmount(loggedInMember.getAmount() - amount);
            mcmembers[index].amount = loggedInMember.getAmount() - amount;
            recipientMember->setAmount(recipientMember->getAmount() + amount);
            mcmembers[rindex].amount = loggedInMember.getAmount() + amount;

            cout << "\nTransaction Successful."
                 << endl;

            loggedInMember.getTransactionHistory().addTransaction("Sent-money-to-" + recipientMobile, amount, loggedInMember.getAmount(), loggedInMember.getTransactionHistory().getTransactionNo());

            mcmembers[index].transaction[mcmembers[index].totalTrans].description = "Sent-money-to-" + recipientMobile;
            mcmembers[index].transaction[mcmembers[index].totalTrans].balance = loggedInMember.getAmount();
            mcmembers[index].transaction[mcmembers[index].totalTrans].amount = amount;

            recipientMember->getTransactionHistory().addTransaction("Received-money-from-" + loggedInMember.getMobileNo(), amount, recipientMember->getAmount(), recipientMember->getTransactionHistory().getTransactionNo());

            mcmembers[rindex].transaction[mcmembers[rindex].totalTrans].description = "Received-money-from-" + loggedInMember.getMobileNo();
            mcmembers[rindex].transaction[mcmembers[rindex].totalTrans].balance = recipientMember->getAmount();
            mcmembers[rindex].transaction[mcmembers[rindex].totalTrans].amount = amount;

            loggedInMember.getTransactionHistory().incrementTransactionNo();
            mcmembers[index].totalTrans++;
            recipientMember->getTransactionHistory().incrementTransactionNo();
            mcmembers[rindex].totalTrans++;
        }
        else
        {
            cout << "\nTransaction canceled. Incorrect OTP."
                 << endl;
        }
    }
    else
    {
        cout << "\nBill payment canceled."
             << "\n";
    }
}

void cashIn(Member &loggedInMember, int index)
{
    cout << "\nEnter amount to cash-in: ";

    double amount;
    cin >> amount;

    if (amount <= 0)
    {
        cout << "\nInvalid amount. Please enter a positive value."
             << endl;
        return;
    }

    cout << "Want to pay(Y/N)? ";

    char c;
    cin >> c;

    if (c == 'Y')
    {
        int otp = generateOTP();
        int f = verifyOTP(otp);

        if (f == -1)
        {
            cout << "\nTime Out. Update canceled."
                 << endl;
        }
        else if (f)
        {
            loggedInMember.setAmount(loggedInMember.getAmount() + amount);
            mcmembers[index].amount = loggedInMember.getAmount() + amount;

            cout << "\nCash-in Successful. New balance: " << loggedInMember.getAmount() << endl;

            loggedInMember.getTransactionHistory().addTransaction("Cash-in", amount, loggedInMember.getAmount(), loggedInMember.getTransactionHistory().getTransactionNo());

            mcmembers[index].transaction[mcmembers[index].totalTrans].description = "Cash-in";
            mcmembers[index].transaction[mcmembers[index].totalTrans].balance = loggedInMember.getAmount();
            mcmembers[index].transaction[mcmembers[index].totalTrans].amount = amount;

            loggedInMember.getTransactionHistory().incrementTransactionNo();
            mcmembers[index].totalTrans++;
        }
        else
        {
            cout << "\nCash-in canceled. Incorrect OTP."
                 << endl;
        }
    }
    else
    {
        cout << "\nBill payment canceled."
             << "\n";
    }
}

void cashOut(Member &loggedInMember, int index)
{
    cout << "\nEnter amount to cash-out: ";

    double amount;
    cin >> amount;

    if (amount <= 0)
    {
        cout << "\nInvalid amount. Please enter a positive value."
             << endl;
        return;
    }

    if (loggedInMember.getAmount() < amount)
    {
        cout << "\nInsufficient balance. Cash-out canceled."
             << endl;
        return;
    }

    cout << "Want to pay(Y/N)? ";

    char c;
    cin >> c;

    if (c == 'Y')
    {
        int otp = generateOTP();
        int f = verifyOTP(otp);

        if (f == -1)
        {
            cout << "\nTime Out. Update canceled."
                 << endl;
        }
        else if (f)
        {
            loggedInMember.setAmount(loggedInMember.getAmount() - amount);
            mcmembers[index].amount = loggedInMember.getAmount() - amount;

            cout << "\nCash-out Successful. New balance: " << loggedInMember.getAmount() << endl;
            loggedInMember.getTransactionHistory().addTransaction("Cash-out", amount, loggedInMember.getAmount(), loggedInMember.getTransactionHistory().getTransactionNo());

            mcmembers[index].transaction[mcmembers[index].totalTrans].description = "Cash-out";
            mcmembers[index].transaction[mcmembers[index].totalTrans].balance = loggedInMember.getAmount();
            mcmembers[index].transaction[mcmembers[index].totalTrans].amount = amount;

            loggedInMember.getTransactionHistory().incrementTransactionNo();
            mcmembers[index].totalTrans++;
        }
        else
        {
            cout << "\nCash-out canceled. Incorrect OTP."
                 << endl;
        }
    }
    else
    {
        cout << "\nBill payment canceled."
             << "\n";
    }
}

void payBill(Member &loggedInMember, int index)
{
    cout << "\nEnter Bill Type (Gas/Electricity/Water/Internet-1/2/3/4): ";

    int n;
    cin >> n;
    double amount;
    string arr[4] = {"Gas", "Electricity", "Water", "Internet"};

    if (n >= 1 && n <= 4)
    {
        cout << "Enter " << arr[n - 1] << " bill: ";
        cin >> amount;
    }
    else
    {
        cout << "\nInvalid choice! Try again"
             << "\n";
        return;
    }

    if (amount <= 0)
    {
        cout << "\nInvalid amount. Please enter a positive value."
             << endl;
        return;
    }

    if (loggedInMember.getAmount() < amount)
    {
        cout << "\nInsufficient balance. Bill payment canceled."
             << endl;
        return;
    }

    cout << "Want to pay(Y/N)? ";

    char c;
    cin >> c;

    if (c == 'Y')
    {
        int otp = generateOTP();
        int f = verifyOTP(otp);

        if (f == -1)
        {
            cout << "\nTime Out. Update canceled."
                 << endl;
        }
        else if (f)
        {
            loggedInMember.setAmount(loggedInMember.getAmount() - amount);
            mcmembers[index].amount = loggedInMember.getAmount() - amount;

            cout << "\nBill payment Successful. New balance: " << loggedInMember.getAmount() << endl;
            loggedInMember.getTransactionHistory().addTransaction(arr[n - 1] + "-bill", amount, loggedInMember.getAmount(), loggedInMember.getTransactionHistory().getTransactionNo());

            mcmembers[index].transaction[mcmembers[index].totalTrans].description = arr[n - 1] + "-bill";
            mcmembers[index].transaction[mcmembers[index].totalTrans].balance = loggedInMember.getAmount();
            mcmembers[index].transaction[mcmembers[index].totalTrans].amount = amount;

            loggedInMember.getTransactionHistory().incrementTransactionNo();
            mcmembers[index].totalTrans++;
        }
        else
        {
            cout << "\nBill payment canceled. Incorrect OTP."
                 << endl;
        }
    }
    else
    {
        cout << "\nBill payment canceled."
             << "\n";
    }
}

void checkBalance(Member &loggedInMember)
{
    cout << "\nCurrent Balance: " << loggedInMember.getAmount() << endl;
}

void viewTransactionHistory(History &transactionHistory)
{
    transactionHistory.displayTransactionHistory(transactionHistory.getTransactionNo());
}