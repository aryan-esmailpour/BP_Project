#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#if __cplusplus >= 201103L
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif
#define int long long
#define ld long double
using namespace std;
const int numberOfCommands = 4, numberOfSalons = 100100;

struct Movie
{
	int runTime;
	string name;
	string type;
	vector <string> summary;
	vector <string> cast;
};

struct ScreenDate
{
	int startTime;
	int endTime;
	string date;
	int capacity;
	string name;
	int salonNumber;
	struct Movie movie;
};

struct CinemaSalon
{
	vector <struct ScreenDate> films;
};

struct Cinema
{
	struct CinemaSalon salons[numberOfSalons];
} rootCinema;

string commands[numberOfCommands] = {"AddScreenDate", "AddCinemaSalon", "BuyTicket", "Exit"};
string endOfFile = "yyttez";
map <string, int> movieId;
vector <struct ScreenDate> allMovies;

bool sortCmp(struct ScreenDate a, struct ScreenDate b)
{
	return (a.startTime < b.startTime);
}

void saveData()
{
	ofstream foutNames;
	ofstream foutSalon;
	ofstream foutStart;
	ofstream foutEnd;
	ofstream foutDate;
	ofstream foutCapacity;
	foutNames.open("movieNames.txt");
	foutSalon.open("movieSalons.txt");
	foutStart.open("movieStarts.txt");
	foutEnd.open("movieEnds.txt");
	foutDate.open("movieDates.txt");
	foutCapacity.open("movieCapacitys.txt");
	for(int i = 0; i < (int)(allMovies.size()); i++)
	{
		struct ScreenDate current = allMovies[i];
		foutNames << current.name << endl;
		foutSalon << current.salonNumber << endl;
		foutStart << current.startTime << endl;
		foutEnd << current.endTime << endl;
		foutDate << current.date << endl;
		foutCapacity << current.capacity << endl;
	}
	foutNames << endOfFile << endl;
	foutSalon << endOfFile << endl;
	foutStart << endOfFile << endl;
	foutEnd << endOfFile << endl;
	foutDate << endOfFile << endl;
	foutCapacity << endOfFile << endl;
	foutNames.close();
	foutSalon.close();
	foutStart.close();
	foutEnd.close();
	foutDate.close();
	foutCapacity.close();
}

void clearData()
{
	ofstream foutNames;
	ofstream foutSalon;
	ofstream foutStart;
	ofstream foutEnd;
	ofstream foutDate;
	ofstream foutCapacity;
	foutNames.open("movieNames.txt");
	foutSalon.open("movieSalons.txt");
	foutStart.open("movieStarts.txt");
	foutEnd.open("movieEnds.txt");
	foutDate.open("movieDates.txt");
	foutCapacity.open("movieCapacitys.txt");
	foutNames << endOfFile << endl;
	foutSalon << endOfFile << endl;
	foutStart << endOfFile << endl;
	foutEnd << endOfFile << endl;
	foutDate << endOfFile << endl;
	foutCapacity << endOfFile << endl;
	foutNames.close();
	foutSalon.close();
	foutStart.close();
	foutEnd.close();
	foutDate.close();
	foutCapacity.close();
}

void dataPrepare()
{
	int currentMovieId = 0;
	ifstream finNames;
	ifstream finSalon;
	ifstream finStart;
	ifstream finEnd;
	ifstream finDate;
	ifstream finCapacity;
	finNames.open("movieNames.txt");
	finSalon.open("movieSalons.txt");
	finStart.open("movieStarts.txt");
	finEnd.open("movieEnds.txt");
	finDate.open("movieDates.txt");
	finCapacity.open("movieCapacitys.txt");
	while(1)
	{
		string currentMovieName, currentDate;
		finNames >> currentMovieName;
		if(currentMovieName == endOfFile){break ;}
		movieId[currentMovieName] = currentMovieId;
		currentMovieId++;
		int currentSalon, currentStart, currentEnd, currentCapacity;
		finSalon >> currentSalon;
		finStart >> currentStart;
		finEnd >> currentEnd;
		finDate >> currentDate;
		finCapacity >> currentCapacity;
		struct ScreenDate currentFilm;
		currentFilm.name = currentMovieName;
		currentFilm.startTime = currentStart;
		currentFilm.endTime = currentEnd;
		currentFilm.date = currentDate;
		currentFilm.capacity = currentCapacity;
		currentFilm.salonNumber = currentSalon;
		rootCinema.salons[currentSalon].films.push_back(currentFilm);
		allMovies.push_back(currentFilm);
	}
	finNames.close();
	finSalon.close();
	finStart.close();
	finEnd.close();
	finDate.close();
	finCapacity.close();
}

bool overlapCheck(int l, int r, int s, int t)
{
	if(r < s || l > t){return 0;}
	return 1;
}

bool screenDateAddingCheck(int salon, int start, int end)
{
	for(int i = 0; i < (int)(rootCinema.salons[salon].films.size()); i++)
	{
		struct ScreenDate current = rootCinema.salons[salon].films[i];
		if(overlapCheck(start, end, current.startTime, current.endTime))
		{
			return 0;
		}
	}
	return 1;
}

string makeString(int month, int day, int hour, int minute)
{
	string res = to_string(day);
	res += '/';
	res += to_string(month);
	res += '-';
	res += to_string(hour);
	res += ':';
	res += to_string(minute);
	return res;
}

int convertToMinute(int screenDateMonth, int screenDateDay, int screenDateHour, int screenDateMinute)
{
	int res = ((screenDateMonth * 32 + screenDateDay) * 24 + screenDateHour) * 60 + screenDateMinute;
	return res;
}

void addScreenDate()
{
	string filmName;
	int screenDateMonth, screenDateDay, screenDateHour, screenDateMinute, salonNumber, screenTime, screenCapacity;
	cout << "Enter the salon number" << endl;
	cin >> salonNumber;
	cout << "Enter the film name" << endl;
	cin >> filmName;
	cout << "Enter the screen date month" << endl;
	cin >> screenDateMonth;
	cout << "Enter the screen date day" << endl;
	cin >> screenDateDay;
	cout << "Enter the screen date hour" << endl;
	cin >> screenDateHour;
	cout << "Enter the screen date minute" << endl;
	cin >> screenDateMinute;
	cout << "Enter the movie run time in minutes" << endl;
	cin >> screenTime;
	cout << "Enter the screen date capacity" << endl;
	cin >> screenCapacity;
	int startTimeMinutes = convertToMinute(screenDateMonth, screenDateDay, screenDateHour, screenDateMinute);
	int endTimeMinutes = startTimeMinutes + screenTime;
	if(!screenDateAddingCheck(salonNumber, startTimeMinutes, endTimeMinutes))
	{
		cout << "There is another movie running in this time and in this salon" << endl;
		return ;
	}
	string dateString = makeString(screenDateMonth, screenDateDay, screenDateHour, screenDateMinute);
	struct ScreenDate newMovie;
	newMovie.startTime = startTimeMinutes;
	newMovie.endTime = endTimeMinutes;
	newMovie.date = dateString;
	newMovie.capacity = screenCapacity;
	newMovie.name = filmName;
	newMovie.salonNumber = salonNumber;
	rootCinema.salons[salonNumber].films.push_back(newMovie);
	allMovies.push_back(newMovie);
	saveData();
	cout << "Command Successful!" << endl;
}

void addCinemaSalon()
{

}

void printAllMovies()
{
	sort(allMovies.begin(), allMovies.end(), sortCmp);
	for(int i = 0; i < (int)(allMovies.size()); i++)
	{
		struct ScreenDate current = allMovies[i];
		cout << i + 1 << ": ";
		cout << current.name << ' ' << current.date << endl;
		cout << "Available sits: " << current.capacity << endl;
	}
}

int checkCapacity(int movieNumber)
{
	return (allMovies[movieNumber].capacity > 0);
}

void buyTicket()
{
	printAllMovies();
	int movieNumber;
	cout << "Enter the movie number" << endl;
	cin >> movieNumber;
	movieNumber--;
	if(!checkCapacity(movieNumber))
	{
		cout << "No available sits, please select another movie" << endl;
		return ;
	}
	allMovies[movieNumber].capacity--;
	cout << "Command Successful!" << endl;
	cout << "Your salon number is: " << allMovies[movieNumber].salonNumber << endl;
	saveData();
}

void doCommand(string currentCommand)
{
	bool isValid = 0;
	for(int i = 0; i < numberOfCommands; i++)
	{
		if(currentCommand == commands[i])
		{
			isValid = 1;
			if(i == 0){addScreenDate();}
			if(i == 1){addCinemaSalon();}
			if(i == 2){buyTicket();}
			if(i == 3){saveData(); exit(0);}
		}
	}
	if(!isValid)
	{
		cout << "Command is not valid" << endl;
	}
}

void makeInput()
{
	while(1)
	{
		cout << "Please enter a command, The Commands are:" << endl;
		for(int i = 0; i < numberOfCommands; i++){cout << commands[i] << ' ';}
		cout << endl;
		string currentCommand;
		cin >> currentCommand;
		doCommand(currentCommand);
	}
}


int32_t main()
{
	dataPrepare();
	makeInput();
	saveData();
	return 0;
}

//ArEsma

















