#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <cmath>
#include "./bank.hpp"

using namespace std;

// constructor
Date::Date()
{
    char s[80];
    time_t t = time(0);
    strftime(this->timestamp, 80, "%d/%m/%Y", localtime(&t));
}



// constructor
PaymentHistory::PaymentHistory(const unsigned int& accNum, const long double& transAm, const string& transContent)
{
    this->transferDate = new Date();
    this->accountNumber = accNum;
    this->transferAmount = transAm;
    this->transferContent = transContent;
}



PaymentHistory::~PaymentHistory()
{
    delete this->transferDate;
}



// constructor
PaymentAccount::PaymentAccount()
{
    this->openDate = new Date();
    this->accountNumber = 759999999;
    this->balance = 100000000; // 100m
}



PaymentAccount::~PaymentAccount()
{
    for (auto h : HistoryArray)
    {
        delete h;
    }
    this->HistoryArray.clear();
}



long double PaymentAccount::getBalance()
{
    return this->balance;
}



bool PaymentAccount::transferTo(const long double& amount, const string& content = "")
{
    if (amount > this->balance) {return false;}

    // else
    this->balance -= amount;
    PaymentHistory* tmp = new PaymentHistory(this->accountNumber, -amount, content);
    this->HistoryArray.push_back(tmp); //
    return true;
}



void PaymentAccount::showHistory()
{
    cout << "\n---- HISTORY ----\n";
    for (auto& h : this->HistoryArray)
    {
        cout << fixed << "--> " << h->accountNumber << "; " << h->transferAmount << "; " << h->transferContent << "; " << h->transferDate->timestamp << endl;
    }
}



// constructor
CreditCardAccount::CreditCardAccount()
{
    this->balance = 0;
    this->interestRate = 0.01;
    this->creditLimit = 60000000; // 60m
    this->history = "";
};



long double CreditCardAccount::getBalance()
{
    return this->balance;
}



bool CreditCardAccount::charge(const long double& amount)
{
    return ((amount + this->balance) <= this->creditLimit) ? true : false;
}



void CreditCardAccount::writeHistory(const long double& amount, const string& historyContent = "")
{
    Date d;
    string transferDate(d.timestamp); // chuyen c-style string sang cstring 
    this->history += (transferDate + "; " + std::to_string(long(-amount)) + "; " + historyContent + '\n');
}



void CreditCardAccount::payment(const long double& amount)
{
    if (!this->charge(amount)) {cout << "Tong so tien thanh toan da vuot qua han muc the tin dung!" << endl; return;}

    // else
    this->balance += amount;
    this->writeHistory(amount, "thanh toan bang the tin dung");
}



void CreditCardAccount::showHistory()
{
    cout << this->history;
}



void CreditCardAccount::payDebt(PaymentAccount& yourPA, const string& transferContent)
{
    if (this->balance == 0) {return;} // khong no

    // else
    this->minPayment = (0.05 * this->balance);
    char option;
    cout << "\n---- Da den han tra no the tin dung ----\n";
    cout << "--> Ban muon tra tat ca no (nhap so 1) hay tra truoc 5% (nhap so 2)?\n>>> ";
    cin >> option; cin.ignore();

    if (option == '1')
    {
        /* neu 0 du tien tra no */
        if(!yourPA.transferTo(this->balance)) 
        {
            if (!yourPA.transferTo(0.05 * this->balance)) 
            {
                cout << "\n--->So du trong tai khoan chinh cua ban ko du de thanh toan no! Ban se phai chiu phat 2,000,000 VND + lai suat";
                this->balance += ((this->interestRate * this->balance) + this->latePenalty);
                return;
            }

            // else
            cout << "\n---> So du trong tai khoan chinh cua ban (" << yourPA.getBalance() << " VND) khong du de tra tat ca no! Ban se phai thanh toan truoc 5% so no!\n";
            yourPA.transferTo(this->minPayment, transferContent);
            this->balance -= (this->minPayment);
            this->balance += (this->interestRate * this->balance);
            return;
        }

        /* neu du tien tra no */
        // else
        yourPA.balance -= this->balance;
        this->balance = 0;
        return;
    }

    else
    {
        if (!yourPA.transferTo(this->minPayment)) 
        {
            cout << "\n--->So du trong tai khoan chinh cua ban ko du de thanh toan no! Ban se phai chiu phat 2,000,000 VND + lai suat";
            this->balance += ((this->interestRate * this->balance) + this->latePenalty);
            return;
        }

        // else
        yourPA.balance -= (this->minPayment);
        this->balance -= (this->minPayment);
        this->balance += (this->interestRate * this->balance);
        return;           
    }
}


// constructor
RewardCardAccount::RewardCardAccount()
{
    this->currentPoints = 0;
    this->rewardRate = 0.11;
}



int RewardCardAccount::getCurrentPoints()
{
    return this->currentPoints;
}



bool RewardCardAccount::charge(const long double& amount)
{
    bool success;
    success = ((amount + this->balance) <= this->creditLimit) ? true : false;

    if (success == true)
    {
        this->currentPoints += floor(amount * this->rewardRate);
    }

    return success;
}



void RewardCardAccount::payment(const long double& amount)
{
    if (!this->charge(amount)) {cout << "Tong so tien thanh toan da vuot qua han muc the tin dung!" << endl; return;}

    // else
    this->balance += amount;
    this->writeHistory(amount, "thanh toan bang the tin dung");
}



void RewardCardAccount::payWithPoints(const int& pAmount)
{
    if (pAmount > this->currentPoints) {return;}

    // else
    this->currentPoints -= pAmount;
    this->writeHistory(pAmount, "thanh toan bang diem thuong");
}



CashBackCardAccount::CashBackCardAccount()
{
    this->cashbackRate = 0.06;
    this->currentCashBack = 0;
}



long double CashBackCardAccount::getCurrentCashBack()
{
    return this->currentCashBack;
}



bool CashBackCardAccount::charge(const long double& amount)
{
    bool success;
    success = ((amount + this->balance) <= this->creditLimit) ? true : false;

    if (success)
    {
        this->currentCashBack += (amount * this->cashbackRate);
    }

    return success;
}



void CashBackCardAccount::payment(const long double& amount)
{
    if (!this->charge(amount)) {cout << "Tong so tien thanh toan da vuot qua han muc the tin dung!" << endl; return;}

    // else
    this->balance += amount;
    this->writeHistory(amount, "thanh toan bang the tin dung");
}



void CashBackCardAccount::redeemCashBack(PaymentAccount& yourPA)
{
    yourPA.balance += this->currentCashBack;
    this->currentCashBack = 0;
}



int main()
{
	PaymentAccount myPA;
    cout << "\n---- Tai khoan duoc mo thanh cong! ----\n";   
    cout << "\nSo du trong tai khoan chinh : " << fixed << setprecision(0) << myPA.getBalance() << " VND" << endl;
    myPA.transferTo(500000, "chuyen khoan"); // 0.5m
    myPA.transferTo(10000, "chuyen khoan"); // 0.01m
    myPA.showHistory();



    CreditCardAccount myCCA;
    cout << "\n----------------------------------------------------------------\n";
    cout << "\n---- The tin dung duoc mo thanh cong! ----\n";   
    myCCA.payment(2000000); // 2m
    myCCA.payment(100000); // 0.1m
    cout << "\nSo no trong the tin dung sau khi tieu sai: " << myCCA.getBalance() << " VND" << endl;
    cout << "\n---- HISTORY ----\n";
    myCCA.showHistory();
    /* giả sử đến hạn trả nợ thẻ tín dụng */
    myCCA.payDebt(myPA, "tra no the tin dung");
    cout << "\nSo no trong the tin dung sau khi tra no: " << myCCA.getBalance() << " VND" << endl;



    RewardCardAccount myRCA;
    cout << "\n----------------------------------------------------------------\n";
    cout << "\n---- The tin dung thuong duoc mo thanh cong! ----\n";   
    cout << "So diem ban dau : " << myRCA.getCurrentPoints() << endl;
    myRCA.payment(6000000); //6m
    myRCA.payment(6000000); //6m
    myRCA.payment(6000000); //6m
    myRCA.payment(6000000); //6m
    cout << "So diem da tich luy duoc : " << myRCA.getCurrentPoints() << endl;
    myRCA.payWithPoints(10);
    cout << "So diem con lai sau su dung : " << myRCA.getCurrentPoints() << endl;
    cout << "\n---- HISTORY ----\n";
    myRCA.showHistory();

    CashBackCardAccount myCBCA;
    cout << "\n----------------------------------------------------------------\n";
    cout << "\n---- The tin dung hoan tien duoc mo thanh cong! ----\n";   
    myCBCA.payment(10000); //   3m
    myCBCA.payment(3235235); //   3m
    myCBCA.payment(111111); //   3m
    myCBCA.payment(29225325); //   3m
    myCBCA.payment(83643462); //   3m
    cout << "Tong so tien da duoc hoan lai : " << myCBCA.getCurrentCashBack() << " VND\n";
    myCBCA.redeemCashBack(myPA);

	return 0;
}