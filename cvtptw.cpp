#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <iomanip>
using namespace std;

struct clients
{
	int client_id, demand;
	double long x_coord, y_coord, ready_time, due_time, service_time, distance;
};

struct solution
{
	int number_of_tracks;
	double long full_time;
	double long full_distance;
	vector<vector<clients>>all_vertices;
	vector<double long> all_distances;
};

struct truck
{
	int number;
	int capacity;
	int flat_capacity;
};
void swapcl(clients& a, clients& b)
{
	clients temp = a;
	a = b;
	b = temp;
}

/*
vector<clients> give_to_lap(vector<clients>all_clients, vector <clients>lap, double long time) {
	clients chosen_client;
	double long chosen_distance = -1;
	double long distance;
	int last_elem = lap.size() - 1;
	for (int i = 1; i < all_clients.size(); i++)
	{
		distance = sqrt(((all_clients[i].x_coord - lap[last_elem].x_coord) * (all_clients[i].x_coord - lap[last_elem].x_coord)) + ((all_clients[i].y_coord - lap[last_elem].y_coord) * (all_clients[i].y_coord - lap[last_elem].y_coord)));
		if (chosen_distance < 0)
		{
			chosen_distance = distance;
			chosen_client = all_clients[i];
		}
		else if (distance < chosen_distance)
		{
			chosen_distance = distance;
			chosen_client = all_clients[i];
		}
	}
	lap.push_back(chosen_client);

	return lap;
}


double long time_counter(double long time, vector<clients>lap)
{
	int last_elem = lap.size() - 1;

	time += sqrt(((lap[last_elem].x_coord - lap[last_elem - 1].x_coord) * (lap[last_elem].x_coord - lap[last_elem - 1].x_coord)) + ((lap[last_elem].y_coord - lap[last_elem - 1].y_coord) * (lap[last_elem].y_coord - lap[last_elem - 1].y_coord)));

	if (time < lap[last_elem].ready_time)
	{
		time += lap[last_elem].ready_time - time;
	}
	time += lap[last_elem].service_time;
	return time;
}
*/


//SORTOWANIE WZGLĘDEM ODLEGŁOŚCI OD PUNKTU W KTÓRYM SIĘ ZNAJDUJEMY (MINIMUM)
void Insertion_sort(vector<clients>& all_clients, int n, clients depend) {
	int i, j;
	clients key;
	for (int i = 1; i < n; i++)
	{
		key = all_clients[i];
		j = i - 1;

		while (j >= 0 && all_clients[j].distance + all_clients[j].ready_time > key.distance + key.ready_time)
		{
			all_clients[j + 1] = all_clients[j];
			--j;
		}
		all_clients[j + 1] = key;
	}
}
bool comparison(clients a, clients b)
{
	//sprawdzamy, czy pierwszy element jest większy od drugiego
	return a.distance + a.ready_time < b.distance + b.ready_time;
}
solution driving(vector<clients> all_clients, solution sol, double long time, truck trucks, clients depend)
{
	double long maybe_back;
	double long powrut;
	sol.all_vertices.push_back(vector<clients>());//inicjalizacja pierwszego wiersza wektora dwuwymiarowego
	int choise;
	clients store = all_clients[0];// przypisanie magazynu do zmiennej

	all_clients.erase(all_clients.begin());//Usuwanie "magazynu" z listy
	for (int i = 0; i < all_clients.size(); i++)// liczenie dystansów od magazynu
	{
		all_clients[i].distance = sqrt(((all_clients[i].x_coord - depend.x_coord) * (all_clients[i].x_coord - depend.x_coord)) + ((all_clients[i].y_coord - depend.y_coord) * (all_clients[i].y_coord - depend.y_coord)));
	}

	int num = all_clients.size();
	//SPRAWDZENIE DOPUSZCZALNOŚCI ROZWIĄZANIA
	for (int i = 0; i < all_clients.size(); i++)
	{

		if (store.due_time < all_clients[i].ready_time + all_clients[i].service_time + all_clients[i].distance || all_clients[i].demand > trucks.capacity || store.due_time < all_clients[i].service_time + all_clients[i].distance * 2)
		{
			sol.number_of_tracks = -1;
			return sol;
		}
	}
	sol.number_of_tracks += 1;
	for (int i = 0; i < num; i++)
	{
		choise = 0;
		for (int i = 0; i < all_clients.size(); i++)
		{
			all_clients[i].distance = sqrt(((all_clients[i].x_coord - depend.x_coord) * (all_clients[i].x_coord - depend.x_coord)) + ((all_clients[i].y_coord - depend.y_coord) * (all_clients[i].y_coord - depend.y_coord)));
		}


		sort(all_clients.begin(), all_clients.end(), comparison);


		time += all_clients[choise].distance;
		double long dis_to_zero = sqrt(((all_clients[choise].x_coord - store.x_coord) * (all_clients[choise].x_coord - store.x_coord)) + ((all_clients[choise].y_coord - store.y_coord) * (all_clients[choise].y_coord - store.y_coord)));
		powrut = sqrt(((depend.x_coord - store.x_coord) * (depend.x_coord - store.x_coord)) + ((depend.y_coord - store.y_coord) * (depend.y_coord - store.y_coord)));;

		while (time > all_clients[choise].due_time || all_clients[choise].demand > trucks.capacity || time + all_clients[choise].service_time + dis_to_zero > store.due_time)
		{
			choise++;

			time -= all_clients[choise - 1].distance;
			if (choise != all_clients.size())
			{
				time += all_clients[choise].distance;
				dis_to_zero = sqrt(((all_clients[choise].x_coord - store.x_coord) * (all_clients[choise].x_coord - store.x_coord)) + ((all_clients[choise].y_coord - store.y_coord) * (all_clients[choise].y_coord - store.y_coord)));
			}
			else
			{

				sol.full_time += powrut;
				trucks.capacity = trucks.flat_capacity;
				depend = store;
				for (int i = 0; i < all_clients.size(); i++)
				{
					all_clients[i].distance = sqrt(((all_clients[i].x_coord - store.x_coord) * (all_clients[i].x_coord - store.x_coord)) + ((all_clients[i].y_coord - store.y_coord) * (all_clients[i].y_coord - store.y_coord)));
				}

				sort(all_clients.begin(), all_clients.end(), comparison);
				sol.all_vertices.push_back(vector<clients>());
				sol.number_of_tracks += 1;
				choise = 0;
				time += powrut;
				sol.all_distances.push_back(time);
				time = 0;
				break;
			}
		}

		if (time == 0) {
			time = all_clients[choise].distance;
		}
		sol.full_time += all_clients[choise].distance;


		if (time < all_clients[choise].ready_time)
		{


			sol.full_time += all_clients[choise].ready_time - time;
			time = all_clients[choise].ready_time;

		}
		time += all_clients[choise].service_time;

		sol.full_time += all_clients[choise].service_time;
		trucks.capacity -= all_clients[choise].demand;
		maybe_back = sqrt(((all_clients[choise].x_coord - store.x_coord) * (all_clients[choise].x_coord - store.x_coord)) + ((all_clients[choise].y_coord - store.y_coord) * (all_clients[choise].y_coord - store.y_coord)));
		depend = all_clients[choise];
		sol.all_vertices[sol.number_of_tracks - 1].push_back(all_clients[choise]);
		all_clients.erase(all_clients.begin() + choise);



	}
	time += maybe_back;
	sol.all_distances.push_back(time);
	sol.full_time += maybe_back;
	return sol;
}
vector<solution> getNeighbours(solution bestcandidate, clients depend, int k)
{
	double long time = 0;
	vector<solution>Neighbourhood;
	Neighbourhood.push_back(bestcandidate);
	solution help = bestcandidate;
	for (int i = k; i <= (help.all_vertices.size() / 4) + k; i++)
	{


		if (i >= help.all_vertices.size())
		{
			break;
		}
		for (int j = 0; j < help.all_vertices[i].size(); j++)
		{
			for (int k = 0; k < help.all_vertices[i].size(); k++)
			{
				swapcl(help.all_vertices[i][j], help.all_vertices[i][k]);

				for (int l = 0; l < help.all_vertices[i].size(); l++)
				{
					if (l == 0)
					{
						time += sqrt(((help.all_vertices[i][0].x_coord - depend.x_coord) * (help.all_vertices[i][0].x_coord - depend.x_coord)) + ((help.all_vertices[i][0].y_coord - depend.y_coord) * (help.all_vertices[i][0].y_coord - depend.y_coord)));

						if (time < help.all_vertices[i][l].ready_time) {
							time = help.all_vertices[i][l].ready_time;
						}
						if (help.all_vertices[i].size() == 1) {
							time += sqrt(((help.all_vertices[i][0].x_coord - depend.x_coord) * (help.all_vertices[i][0].x_coord - depend.x_coord)) + ((help.all_vertices[i][0].y_coord - depend.y_coord) * (help.all_vertices[i][0].y_coord - depend.y_coord)));
						}
						time += help.all_vertices[i][l].service_time;
					}
					else
					{
						time += sqrt(((help.all_vertices[i][l].x_coord - help.all_vertices[i][l - 1].x_coord) * (help.all_vertices[i][l].x_coord - help.all_vertices[i][l - 1].x_coord)) + ((help.all_vertices[i][l].y_coord - help.all_vertices[i][l - 1].y_coord) * (help.all_vertices[i][l].y_coord - help.all_vertices[i][l - 1].y_coord)));
						if (time > help.all_vertices[i][l].due_time)
						{

							time = 0;
							break;
						}
						else if (time < help.all_vertices[i][l].ready_time)
						{

							time = help.all_vertices[i][l].ready_time;


						}
						time += help.all_vertices[i][l].service_time;

						if (l == help.all_vertices[i].size() - 1) {
							time += sqrt(((help.all_vertices[i][l].x_coord - depend.x_coord) * (help.all_vertices[i][l].x_coord - depend.x_coord)) + ((help.all_vertices[i][l].y_coord - depend.y_coord) * (help.all_vertices[i][l].y_coord - depend.y_coord)));
						}

					}
				}
				if (time < depend.due_time && time != 0)
				{
					if (bestcandidate.all_distances[i] < time)
					{



					}
					else
					{

						help.full_time -= abs(time - bestcandidate.all_distances[i]);
						help.all_distances[i] = time;
						Neighbourhood.push_back(help);
						help.all_distances[i] = bestcandidate.all_distances[i];
					}



				}
				time = 0;
				swapcl(help.all_vertices[i][j], help.all_vertices[i][k]);
				help.full_time = bestcandidate.full_time;

			}
		}

	}
	return Neighbourhood;
}
solution Tabu_search(solution sol, clients depend) {
	time_t endwait;
	int seconds = 270;
	endwait = time(NULL) + seconds;
	int flaga = 1;
	solution sBest = sol;
	solution bestcandidate = sol;
	vector<double long>Tabulist;
	int k = 0;

	while (time(NULL) < endwait)
	{
		
		vector<solution>neighbourhood = getNeighbours(bestcandidate, depend, k);

		k += (bestcandidate.all_vertices.size() / 4);
		if ((bestcandidate.all_vertices.size() / 4) <= 1) {
			k++;
		}
		if (k >= bestcandidate.all_vertices.size()) {
			k = 0;
		}
		bestcandidate = neighbourhood[0];

		for (int i = 0; i < neighbourhood.size(); i++)
		{
			flaga = 1;

			for (int j = 0; j < Tabulist.size(); j++)
			{
				if (neighbourhood[i].full_time == Tabulist[j]) {
					flaga = 0;
					break;
				}
			}
			if (flaga == 1 && neighbourhood[i].full_time < bestcandidate.full_time) {
				bestcandidate = neighbourhood[i];
			}
			if (bestcandidate.full_time < sBest.full_time) {
				sBest = bestcandidate;

			}
			Tabulist.push_back(bestcandidate.full_time);
			if (Tabulist.size() > 500) {
				Tabulist.erase(Tabulist.begin());
			}
		}
		neighbourhood.clear();
	}
	return sBest;
}
void file_cleaner(string input)
{
	fstream in_file;
	string line;
	string text;
	in_file.open(input, ios::in);
	while (getline(in_file, line))
	{
		if (!(line.empty() || line.find_first_not_of(' ') == string::npos)) {
			if (line.length() == 1)
			{
			}
			else
			{
				text += line + "\n";
			}
		}
	}

	in_file.close();
	fstream out_file;
	out_file.open(input, ios::out);
	out_file << text;
	out_file.close();
}
int main(int argc, char** argv)
{
	string input(argv[1]);


	truck trucks;
	solution sol;
	clients deliver;
	string data;
	vector<clients>all_clients;
	//WCZYTYWANIE DANYCH Z PLIKU
	//file_cleaner(input);

	string discard;
	ifstream Testfile;
	Testfile.open(input);
	if (!Testfile) {
		cout << "No file" << endl;
	}
	else
	{
		getline(Testfile, discard);
		getline(Testfile, discard);
		getline(Testfile, discard);
		getline(Testfile, discard);
		Testfile >> trucks.number >> trucks.capacity;
		getline(Testfile, discard);
		getline(Testfile, discard);
		getline(Testfile, discard);
		getline(Testfile, discard);
		Testfile >> deliver.client_id >> deliver.x_coord >> deliver.y_coord >> deliver.demand >> deliver.ready_time >> deliver.due_time >> deliver.service_time;
		all_clients.push_back(deliver);
		while (Testfile)
		{
			Testfile >> deliver.client_id >> deliver.x_coord >> deliver.y_coord >> deliver.demand >> deliver.ready_time >> deliver.due_time >> deliver.service_time;
			all_clients.push_back(deliver);
		}
	}
	Testfile.close();



	for (int i = all_clients.size() - 1; i < all_clients.size(); i++)
	{
		if (deliver.client_id == all_clients[i].client_id)
		{
			all_clients.erase(all_clients.begin() + i);
		}
	}
	/*for (int i = 0; i < all_clients.size(); i++)
	{
		cout << all_clients[i].client_id << endl;
	}
	*/
	all_clients.shrink_to_fit();
	//INICJALIZACJA ZMIENNYCH DO FUNKCJI;
	sol.full_distance = 0;
	sol.full_time = 0;
	sol.number_of_tracks = 0;
	clients depend = all_clients[0];
	trucks.flat_capacity = trucks.capacity;
	
	sol = driving(all_clients, sol, 0, trucks, depend);//WYWOŁANIE GŁÓWNEJ FUNKCJI
	//ODPOWIEDŹ W KONSOLI

	if (sol.number_of_tracks != -1)
	{
		sol = Tabu_search(sol, depend);
	}



	cout << setprecision(20) << "KOSZT: " << sol.full_time << "LICZBA CIEZARÓWEK: " << sol.number_of_tracks;

	cout << "\nTRASY: ";
	for (int i = 0; i < sol.all_vertices.size(); i++)
	{
		cout << "\n";
		for (int j = 0; j < sol.all_vertices[i].size(); j++)
		{
			cout << sol.all_vertices[i][j].client_id << " ";
		}
	}

	//ZAPISYWANIE ODPOWIEDZI DO PLIKU
	ofstream zapisywanko("output.txt");
	zapisywanko << setprecision(20) << sol.number_of_tracks << "    " << sol.full_time << endl;

	for (int i = 0; i < sol.all_vertices.size(); i++)
	{
		for (int j = 0; j < sol.all_vertices[i].size(); j++)
		{
			zapisywanko << sol.all_vertices[i][j].client_id << " ";
		}
		zapisywanko << "\n";
	}

	zapisywanko.close();

	return 0;
}