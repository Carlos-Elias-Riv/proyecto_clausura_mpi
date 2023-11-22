#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int num_processes = 0;
    int process_id = 0; 
    int name_length = 0;
    char host_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;
    /*
    Pongo partner/sender con id/rango 1 porque estoy asumiendo que 
    primero ejecutare este programa llamado receiver
    */
    
    
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Get_processor_name(host_name, &name_length);
    cout << "hola soy el proceso " << process_id << "\n";
    if(process_id == 0){ // entonces eres el receptor
        int etiqueta = 101;
        int mensaje[5];
        int partner = 1;
        cout << "Hola de receiver " << process_id << " en " << host_name << "\n";
        int numero_de_enteros_a_recibir = 5;
        MPI_Recv(&mensaje, numero_de_enteros_a_recibir, MPI_INT, partner, etiqueta, MPI_COMM_WORLD, &status);
        for(int i = 0; i<numero_de_enteros_a_recibir; i ++){
            cout << "Hola de receiver " << process_id << " recibi " << mensaje[i] << "\n";
        }
        cout << "Lo recibi de " << status.MPI_SOURCE;
        
        
        

    } else if (process_id == 1) { // si es el emisor
        int etiqueta = 101;
        int mensaje[5] = {11, 12, 13, 14, 15};
        int partner = 0;
        int num_elementos_enviar = 5;
        MPI_Send(&mensaje, num_elementos_enviar, MPI_INT, partner, etiqueta, MPI_COMM_WORLD);
        cout << "Acabo de enviar el arreglo " << "\n";
    }
    

    MPI_Finalize();
    return 0;
}
