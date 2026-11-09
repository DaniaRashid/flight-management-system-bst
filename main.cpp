#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

struct flight
{
	int id;
	string source;
	string destination;
	string airline;
	int departureTime;        
	int arrivalTime;       
	double flightDuration;    
	double ticketPrice;
	string status;
};

struct node
{
	flight data;
	node* left;
	node* right;
	node* p;

	node(flight f)
	{
		data = f;
		left = nullptr;
		right = nullptr;
		p = nullptr;
	}
};

void PrintTime(int t)
{
	if (t / 100 < 10)
		cout << "0";
	cout << t / 100 << ":";
	if (t % 100 < 10)
		cout << "0";
	cout << t % 100;
}

void printFlight(flight f)
{
	cout << f.id << " | " << f.source << " | " << f.destination << " | "
		<< f.airline << " | ";
	PrintTime(f.departureTime);
	cout << " | ";
	PrintTime(f.arrivalTime);
	cout << " | " << f.flightDuration << " | " << f.ticketPrice << " | "
		<< f.status << endl;
}

void printETA(flight f)
{
	int dep = f.departureTime / 100 * 60 + f.departureTime % 100;   
	int sched = f.arrivalTime / 100 * 60 + f.arrivalTime % 100;
	int dur = (int)(f.flightDuration * 60 + 0.5);                 

	int eta = (dep + dur) % 1440;

	cout << "Flight " << f.id << " ETA: ";
	PrintTime(eta / 60 * 100 + eta % 60);   

	if (eta == sched)     cout << "  : On-Time\n";
	else if (eta < sched) cout << "  : Early\n";
	else                  cout << "  : Delayed\n";
}

string TimeText(int t)
{
	string text = "";
	if (t / 100 < 10)
		text = text + "0";
	text = text + to_string(t / 100) + ":";
	if (t % 100 < 10)
		text = text + "0";
	text = text + to_string(t % 100);
	return text;
}

int TimeFromText(string text)
{
	int colon = text.find(':');
	if (colon == (int)string::npos)
		return atoi(text.c_str());
	return atoi(text.substr(0, colon).c_str()) * 100 + atoi(text.substr(colon + 1).c_str());
}

string DurationText(double hours)
{
	int totalMinutes = (int)(hours * 60 + 0.5);
	int hundredths = (int)(hours * 100 + 0.5);

	string text = to_string(totalMinutes / 60) + ":";
	if (totalMinutes % 60 < 10)
		text = text + "0";
	text = text + to_string(totalMinutes % 60) + "/" + to_string(hundredths / 100) + ".";
	if (hundredths % 100 < 10)
		text = text + "0";
	text = text + to_string(hundredths % 100);
	return text;
}

double DurationFromText(string text)
{
	int slash = text.find('/');
	if (slash != (int)string::npos)
		return atof(text.substr(slash + 1).c_str());
	return atof(text.c_str());
}

class bst
{
public:
	node* root;
	bst()
	{
		root = nullptr;
	}

	bool insert(flight f)
	{
		node* y = nullptr;
		node* x = root;

		while (x != nullptr)
		{
			y = x;
			if (f.id < x->data.id)
				x = x->left;
			else if (f.id > x->data.id)
				x = x->right;
			else
				return false;          
		}
		node* z = new node(f);
		z->p = y;
		if (y == nullptr)
			root = z;
		else if (z->data.id < y->data.id)
			y->left = z;
		else
			y->right = z;
		return true;
	}

	node* search(int id)
	{
		node* x = root;
		while (x != nullptr && x->data.id != id)
		{
			if (id < x->data.id)
				x = x->left;
			else
				x = x->right;
		}
		return x;
	}

	node* minimum(node* x)
	{
		while (x->left != nullptr)
			x = x->left;
		return x;
	}

	void transplant(node* u, node* v)
	{
		if (u->p == nullptr)
			root = v;
		else if (u == u->p->left)
			u->p->left = v;
		else
			u->p->right = v;

		if (v != nullptr)
			v->p = u->p;
	}

	bool deleteNode(int id)
	{
		node* z = search(id);
		if (z == nullptr)
			return false;

		if (z->left == nullptr)                 
			transplant(z, z->right);
		else if (z->right == nullptr)           
			transplant(z, z->left);
		else                                
		{
			node* y = minimum(z->right);        
			if (y->p != z)
			{
				transplant(y, y->right);
				y->right = z->right;
				y->right->p = y;
			}
			transplant(z, y);
			y->left = z->left;
			y->left->p = y;
		}
		delete z;
		return true;
	}

	void inorder(node* x)
	{
		if (x != nullptr)
		{
			inorder(x->left);
			printFlight(x->data);
			inorder(x->right);
		}
	}

	void display()
	{
		if (root == nullptr)
		{
			cout << "No flights!\n";
			return;
		}
		inorder(root);
	}

	void etaInorder(node* x)
	{
		if (x != nullptr)
		{
			etaInorder(x->left);
			printETA(x->data);
			etaInorder(x->right);
		}
	}

	void showETA()
	{
		if (root == nullptr)
		{
			cout << "No flights!\n";
			return;
		}
		etaInorder(root);
	}

	void savePreorder(node* x, ofstream& file)
	{
		if (x != nullptr)
		{
			file << x->data.id << ","
				<< x->data.source << ","
				<< x->data.destination << ","
				<< x->data.airline << ","
				<< TimeText(x->data.departureTime) << ","
				<< TimeText(x->data.arrivalTime) << ","
				<< DurationText(x->data.flightDuration) << ","
				<< x->data.ticketPrice << ","
				<< x->data.status << endl;

			savePreorder(x->left, file);
			savePreorder(x->right, file);
		}
	}

	void saveToFile(string filename)
	{
		ofstream file(filename);
		if (!file.is_open())
		{
			cout << "Could not open " << filename << endl;
			return;
		}
		savePreorder(root, file);
		file.close();
		cout << "Flights saved to " << filename << endl;
	}

	int loadFromFile(string filename)
	{
		ifstream file(filename);
		if (!file.is_open())
			return 0;                      

		int count = 0;
		flight f;
		char comma;
		string text;

		while (file >> f.id)
		{
			file >> comma;
			getline(file, f.source, ',');
			getline(file, f.destination, ',');
			getline(file, f.airline, ',');
			getline(file, text, ',');
			f.departureTime = TimeFromText(text);
			getline(file, text, ',');
			f.arrivalTime = TimeFromText(text);
			getline(file, text, ',');
			f.flightDuration = DurationFromText(text);
			file >> f.ticketPrice >> comma;
			getline(file, f.status);
			if (!f.status.empty() && f.status.back() == '\r')
				f.status.pop_back();

			if (insert(f))
				count++;
		}
		file.close();
		return count;
	}
};

string getStatus()
{
	int s = 0;
	while (s < 1 || s > 3)
	{
		cout << " status (1 = Scheduled, 2 = Delayed, 3 = Cancelled): ";
		cin >> s;
	}

	if (s == 1)
		return "Scheduled";
	else if (s == 2)
		return "Delayed";
	else
		return "Cancelled";
}

flight ReadFlight()
{
	flight f;
	cout << " Flight ID: ";
	cin >> f.id;
	cin.ignore();
	cout << " source: ";
	getline(cin, f.source);
	cout << " destination: ";
	getline(cin, f.destination);
	cout << " airline: ";
	getline(cin, f.airline);
	cout << " departure : ";
	cin >> f.departureTime;
	cout << " arrival : ";
	cin >> f.arrivalTime;
	cout << " duration in hours : ";
	cin >> f.flightDuration;
	cout << " ticket price: ";
	cin >> f.ticketPrice;
	cin.ignore();
	cout << " status: ";
	f.status = getStatus();

	return f;
}

int main()
{
	bst tree;
	int choice, id, updateChoice;

	int loaded = tree.loadFromFile("flight.txt");     // load when program starts
	if (loaded > 0)
		cout << loaded << " flights loaded from flight.txt\n";

	do
	{
		cout << endl;
		cout << "========== FLIGHT MANAGEMENT SYSTEM ==========\n";
		cout << "1. Add New Flight\n";
		cout << "2. Search Flight by ID\n";
		cout << "3. Delete Flight by ID\n";
		cout << "4. Display All Flights (Ascending Flight ID)\n";
		cout << "5. Calculate Flight Duration and ETA\n";
		cout << "6. Surprise Feature (Update Flight)\n";
		cout << "7. Save Flights to File\n";
		cout << "8. Load Flights from File\n";
		cout << "0. Exit\n";
		cout << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice)
		{
		case 1:
		{
			flight f = ReadFlight();
			if (tree.insert(f))
				cout << "Flight " << f.id << " added.\n";
			else
				cout << "Flight ID " << f.id << " already exists. Not inserted.\n";
			break;
		}
		case 2:
		{
			cout << "Enter Flight ID to search: ";
			cin >> id;
			node* result = tree.search(id);
			if (result != nullptr)
				printFlight(result->data);
			else
				cout << "Flight " << id << " not found.\n";
			break;
		}
		case 3:
		{
			cout << "Enter Flight ID to delete: ";
			cin >> id;
			if (tree.deleteNode(id))
				cout << "Flight " << id << " deleted.\n";
			else
				cout << "Flight " << id << " not found.\n";
			break;
		}
		case 4:
			tree.display();
			break;
		case 5:
			tree.showETA();
			break;
		case 6:
		{
			cout << "Enter Flight ID to update: ";
			cin >> id;
			node* result = tree.search(id);
			if (result == nullptr)
			{
				cout << "Flight " << id << " not found.\n";
			}
			else
			{
				cout << "\n--- UPDATE MENU ---\n";
				cout << "1. Update ticket price\n";
				cout << "2. Update departure and arrival time\n";
				cout << "3. Update status\n";
				cout << "Enter your choice: ";
				cin >> updateChoice;

				if (updateChoice == 1)
				{
					cout << "New ticket price: ";
					cin >> result->data.ticketPrice;
					cout << "Price updated.\n";
				}
				else if (updateChoice == 2)
				{
					cout << "New departure (e.g. 1400): ";
					cin >> result->data.departureTime;
					cout << "New arrival (e.g. 1545): ";
					cin >> result->data.arrivalTime;
					cout << "Times updated.\n";
				}
				else if (updateChoice == 3)
				{
					cout << "New status (Scheduled / Delayed / Cancelled): ";
					cin >> result->data.status;
					cout << "Status updated.\n";
				}
				else
				{
					cout << "Invalid choice.\n";
				}

				cout << "Updated flight: ";
				printFlight(result->data);
			}
			break;
		}
		case 7:
			tree.saveToFile("flight.txt");
			break;
		case 8:
		{
			int count = tree.loadFromFile("flight.txt");
			cout << count << " new flights loaded.\n";
			break;
		}
		case 0:
			cout << "Goodbye!\n";
			break;
		default:
			cout << "Invalid choice, try again.\n";
		}

	} while (choice != 0);

	return 0;
}