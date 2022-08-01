#include <vector>

using namespace std;

class Date
{
	public:
		char timestamp[80];

		// constructor
		Date();
};



class PaymentHistory
{
	unsigned int accountNumber;
	long double transferAmount;
	string transferContent;
	Date* transferDate;

public:
	// constructor
	PaymentHistory(const unsigned int& accNum, const long double& transAm, const string& transContent);

	// destructor
	~PaymentHistory();

	friend class PaymentAccount;
};



class PaymentAccount
{
	Date* openDate; // ngay mo tai khoan
	unsigned int accountNumber;
	long double balance;
	vector<PaymentHistory*> HistoryArray;

public:
	// constructor
	PaymentAccount();

	// destructor
	~PaymentAccount();

	long double getBalance();
	bool transferTo(const long double& amount, const string& content);
	void showHistory();

	friend class CreditCardAccount;
	friend class RewardCardAccount;
	friend class CashBackCardAccount;
};



class CreditCardAccount
{
protected:
	long double creditLimit = 30000000;
	float interestRate;
	long double minPayment;
	long double latePenalty;
	long double balance; // balance la tong so no
	string history;
	
public:
	// constructor
	CreditCardAccount();

	long double getBalance();
	void writeHistory(const long double& amount, const string& historyContent);
	void showHistory();
	bool charge(const long double& amount);
	void payment(const long double& amount);
	void payDebt(PaymentAccount& yourPA, const string& transContent);
};



class RewardCardAccount : public CreditCardAccount
{
	float rewardRate;
	int currentPoints;

public:   
	// constructor
	RewardCardAccount();

	bool charge(const long double& amount);
	void payment(const long double& amount);
	int getCurrentPoints();
	void payWithPoints(const int& pAmount);
};



class CashBackCardAccount : public CreditCardAccount
{
	float cashbackRate;
	long double currentCashBack;

public:
	// constructor
	CashBackCardAccount();

	long double getCurrentCashBack();
	bool charge(const long double& amount);
	void payment(const long double& amount);
	void redeemCashBack(PaymentAccount& yourPA);
};