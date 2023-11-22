#include <iostream>
#include <mpi.h>
#include <cmath>
#include <chrono>
#include <fstream>
#include <string>
#include <map>

using namespace std;

void load_CSV(string file_name, string *words, long long int size)
{
    // Open the file
    ifstream inputFile(file_name);

    // Check if the file is open
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening the file." << std::endl;
        perror("Error details");
    }

    // Read words from the file
    string word;
    int pos = 0;
    while (inputFile >> word)
    {
        for (int i = 0; i < word.length(); i++)
        {
            if (word[i] == ',')
            {
                pos++;
            }
            else
            {
                words[pos] += word[i];
            }
        }
    }

    // voy a imprimir las primeras 5 palabras para comprobar que se haya leído bien el archivo
    // for (int i = 1; i <= 5; i++)
    // {
    //     cout << words[i] << endl;
    // }

    // Close the file
    inputFile.close();
}

// función que guarda mi diccionario a un csv
void save_dict_to_CSV(string file_name, map<string, int> dict, long long int size, string *vocabulario)
{
    fstream fout;
    fout.open(file_name, ios::out);
    // header tienen que ser todas las palabras del vocabulario
    for (long long int i = 0; i < size; i++)
    {
        if (vocabulario[i] != "")
            fout << vocabulario[i] << ",";
    }
    fout << "\n";

    // ahora guardo los valores del diccionario

    for (long long int i = 0; i < size; i++)
    {
        if (vocabulario[i] != "")
            fout << dict[vocabulario[i]] << ",";
    }
}

// versión que no recibe un diccionario sino que lo regresa

map<string, int> ocurrence_number(string file_name, int id, int num_processes)
{
    string *words = new string[1000000];

    load_CSV(file_name, words, 1000000);

    map<string, int> resp;

    for (int i = 0; i < 1000000; i++)
    {
        resp[words[i]] += 1;
    }

    return resp;
}

// versión serial
// int main(int argc, char *argv[])
// {

//     string *vocabulario = new string[1000000];

//     // obtengo todas las palabras posibles que pueden tener los archivos
//     load_CSV("vocabulario.txt", vocabulario, 1000000);

//     string test[] = {"project", "gutenberg", "ebook", "oliver", "youth", "youthful"};

//     // cuento las ocurrencias de cada palabra en el archivo
//     map<string, int> dict = ocurrence_number("dickens_oliver_twist.txt", 0, 1);

//     // guardo el diccionario en un csv
//     save_dict_to_CSV("dickens_oliver_twist_wordbag.csv", dict, 1000000, vocabulario);
// }

// versión paralela

// versión que no recibe un diccionario sino que lo regresa, pero ahora paralela

map<string, int> ocurrence_number(int id, int num_processes)
{
    string *words = new string[1000000];

    string archivos[] = {"shakespeare_the_merchant_of_venice.txt", "shakespeare_romeo_juliet.txt",
                         "shakespeare_hamlet.txt", "dickens_a_christmas_carol.txt",
                         "dickens_oliver_twist.txt", "dickens_a_tale_of_two_cities.txt"};

    load_CSV(archivos[id], words, 1000000);

    map<string, int> resp;

    for (int i = 0; i < 1000000; i++)
    {
        resp[words[i]] += 1;
    }

    return resp;
}

int main(int argc, char *argv[])
{
    int id;
    int num_processes;

    int receiver_id = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    string *vocabulario = new string[1000000];
    load_CSV("vocabulario.txt", vocabulario, 1000000);
    map<string, int> dict = ocurrence_number(id, num_processes);

    for (int i = 0; i < 5; i++)
    {
        cout << id << " " << vocabulario[i] << " " << dict[vocabulario[i]] << endl;
    }

    MPI_Gather(&dict, 1, MPI_INT, &dict, 1, MPI_INT, receiver_id, MPI_COMM_WORLD);

    MPI_Finalize();

    // necesito juntar lo que han hecho los diferentes hilos
}