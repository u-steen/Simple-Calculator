#include <iostream>
#include <string>
#include <stack>

using namespace std;

bool checkParantheses(string expression);

double SolvePostfix(string expression);

string postfixConvertor(string expression);				// DOAR PT TEST - A SE STERGE
string convertIntToString(int nr);

void Load();

int main() {
	string expression;
	Load();
	cout << "Introdu o expresie: ";
	getline(cin, expression);
	while(!checkParantheses(expression)) {
		cout << "Paranteze gresite! Mai incearca: ";
		getline(cin, expression);
	}
	string postfix = postfixConvertor(expression);
	cout << "Postfix: " << postfix << endl;
	double result = SolvePostfix(postfix);
	cout << result;
}

void Load() {
	cout << "---------------------------------------------------------------------\n";
	cout << "                           CALCULATOR\n";
	cout << "---------------------------------------------------------------------\n";
	cout << "  Introdu o expresie formata din numere si operatori (+, -, *, /, ^) \n";
	cout << "  si apasa ENTER!\n";
	cout << "---------------------------------------------------------------------\n";
	cout << "  1) Momentan nu merge sa introduci numere CU VIRGULA!\n";
	cout << "  2) Merge momentan doar cu numere POZITIVE!\n";
	cout << "  3) Nu merge cu litere! Daca se introduce o litera se converteste in\n";
	cout << "     ASCII!\n";
	cout << "---------------------------------------------------------------------\n";
}

bool isOpener(char a) {
	if (a == '(' || a == '[' || a == '{')
		return true;
	return false;
}

bool isCloser(char a) {
	if (a == ')' || a == ']' || a == '}')
		return true;
	return false;
}
bool isDigit(char a) {
	if (a >= '0' && a <= '9')
		return true;
	return false;
}

bool parMatch(char a, char b) {
	if (a == '(' && b == ')' ||
		a == '[' && b == ']' ||
		a == '{' && b == '}')
		return true;
	return false;
}
bool isOperator(char a) {
	if (a == '+' || a == '-' || a == '*' || a == '/' || a == '^')
		return true;
	return false;
}

bool checkParantheses(string expression) {
	stack <char> S;
	for (int i = 0; i < expression.length(); i++) {
		if (expression[i] == ' ') continue;
		if (isOpener(expression[i]))
			S.push(expression[i]);
		else if (isCloser(expression[i])) {
			if (!S.empty() && parMatch(S.top(), expression[i]))
				S.pop();
			else
				return false;
		}
	}
	if (S.empty())
		return true;
	return false;
}

short getWeight(char a) {
	short weight = -1;
	switch (a) {
	case '+':
	case '-':
		weight = 1;
		break;
	case '*':
	case '/':
		weight = 2;
		break;
	case '^':
		weight = 3;
		break;
	}
	return weight;
}

bool isHigher(char a, char b) {
	short w1 = getWeight(a);
	short w2 = getWeight(b);
	if (w1 == w2) {
		if (a == '^' && b == '^')
			return false;
		return true;

	}
	return w1 > w2 ? true : false;
}

bool isOperand(char a) {
	if (a >= 'a' && a <= 'z' ||
		a >= 'A' && a <= 'Z' ||
		a >= '0' && a <= '9')
		return true;
	return false;
}

string convertIntToString(int nr) {
	string charnr = "";
	int ogl = 0;
	while (nr) {
		ogl = ogl * 10 + nr%10;
		nr /= 10;
	}
	while (ogl) {
		charnr += (ogl % 10)+'0';
		ogl /= 10;
	}
	return charnr;
}

string postfixConvertor(string expression) {
	string postfix = "";
	stack <char> S;
	for (int i = 0; i < expression.length(); i++) {
		if (expression[i] == ' ') continue;
		if (isOperand(expression[i])){
			int nr=0;
			while (isDigit(expression[i])) {		// iau un string si il convertesc in int
				nr = nr * 10 + (expression[i] - '0');			// NU CONVERTESTE BINE FUTU I MORTII MAMII LUI
				i++;
			}
			i--;
			postfix += convertIntToString(nr);
			postfix += ' ';
		} 			
								
		// PARANTEZE
		else if (isOpener(expression[i]))		// daca este paranteza deschisa
			S.push(expression[i]);
		else if (isCloser(expression[i])) {		// daca este paranteza inchisa
			while (S.top() != '(') {
				postfix += S.top();
				S.pop();
			}
			S.pop();
		}

		else if (isOperator(expression[i])) {
			while (	!S.empty()&& isHigher(S.top(), expression[i]) && S.top() != '(') {
				postfix += S.top();			// cand vine un element cu prioritate mai mica, trecem tot ce avem in stack
				S.pop();
			}
			S.push(expression[i]);
		}
	}
	while (!S.empty()) {	// cand teminam expressia trecem tot ce a ramas in stack
		postfix += S.top();
		S.pop();
	}
	return postfix;
}

double Solve(double o1, double o2, char ex);

double SolvePostfix(string expression) {
	stack <double> S;
	double res = 0;
	for (int i = 0; i < expression.length(); i++) {
		if (expression[i] == ' ') continue;
		if (isOperand(expression[i])) {
			int nr = 0;
			while (isDigit(expression[i])) {
				nr = nr * 10 + (expression[i] - '0');			
				i++;
			}
			i--;
			S.push(nr);
			//cout << "STACK+ : " << expression[i] << endl;
		}
		else if (isOperator(expression[i])) {
			res = 0;
			double op2 = S.top();
			S.pop();
			double op1 = S.top();
			S.pop();
			res+=Solve(op1, op2, expression[i]);
			cout << "REZOLVAT : " << op1 << expression[i] << op2 << "=" << res << endl;
			S.push(res);
			cout << "Inlocuim expresia " << op1 << expression[i] << op2 << " cu rezultatul : " << res << endl << endl;
		}
	}
	return res;
}


double Solve(double o1, double o2, char ex) {
	if (ex == '+') return o1 + o2;
	else if (ex == '-') return o1 - o2;
	else if (ex == '*') return o1 * o2;
	else if (ex == '/') return o1 / o2;
	else if (ex == '^') return pow(o1, o2);
}