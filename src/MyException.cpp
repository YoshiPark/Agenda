#include <exception>
#include <iostream>
using namespace std;
class No_User_Error : public exception {
    public:
    No_User_Error() : exception() {}
    void what() {
    		cout << "[Result]: No User!\n";
    }
};
class Password_Error : public exception {
public:
   Password_Error() : exception() {}
   void what() {
   	cout << "[Result]: Password is not correct!\n";
   }
};
class User_Exist_Error : public exception {
    public:
    User_Exist_Error(): exception() {}
    void what() {
    	cout << "[Result]: The User has been exist!\n";
    }
};
class Time_Error : public exception {
public:
	Time_Error() : exception() {}
	void what() {
		cout << "[Result]: Time Error!\n";
	}
};

class Title_Error : public exception {
public:
	Title_Error() : exception() {}
	void what() {
		cout << "[Result]: Title Error!\n";
	}
};

class Participator_Userin_Error : public exception {
public:
	Participator_Userin_Error() : exception() {}
	void what() {
		cout << "[Result] : Sponsor is in the Participator!\n";
	}
};

class Participator_Repeat_Error : public exception {
public:
	Participator_Repeat_Error() : exception() {}
	void what() {
		cout << "[Result] : Participator have the same name!\n";
	}
};

class Participator_Noin_Error : public exception {
public:
	Participator_Noin_Error() : exception() {}
	void what() {
		cout << "[Result] : there are not less than one person in the Participator!\n";
	}
};
class No_User_in_Error : public exception {
   public:
   No_User_in_Error() : exception() {}
   void what() {
   	cout << "[Result]: User is not exist!\n";
   }
};
class Date_Format_Error : public exception {
public:
    Date_Format_Error() : exception() {}
	void what() {
		cout << "[Result] : The format of Date Error!\n";
	}
};

class Date_SE_Error: public exception {
	public:
	Date_SE_Error() : exception() {}
	void what() {
		cout << "[Result] : The startDate is later than or equal than endDate!\n";
	}
};

class Time_Conflict_Error : public exception {
public:
	Time_Conflict_Error(): exception() {}
	void what() {
		cout << "[Result] : Time Conflict!\n";
	}
};


