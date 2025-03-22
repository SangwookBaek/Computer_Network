#include <iostream>
#include <fstream>
#include <string>
#include<cstdlib> 
#include <cstring>
#include <sstream>
#include <vector>
#include <queue>
#include <vector>
#define INF 200
//todo 
//1. read topology file and make table using algorithm
//2. send message 
//3. reflect contents in change file

using namespace std;

int **A_matrix; //100 100 짜리 sparse matrix


struct Edge {
    int dest;
    int distance;
    int next;
};
Edge** distance_vector;

struct Message{
    int src;
    int dest;
    string m;
};
Message *m_array;

struct Change{
    int src;
    int dest;
    int cost;
};
Change *c_array;

int c_num;
int m_num;
int node_num;

int compose_dist_vector(Edge**,int**);
int send_dist_vector(Edge**,int, int);

int m_file_read(string path);
int t_file_read(string path);
int c_file_read(string path);


void send_message(Message *,Edge**);
ofstream fout;


int main(int argc, char** argv) {
    m_array = new Message[100]; //일단 100개로 할당
    c_array = new Change[100]; //일단 100개로 할당
    fout.open("output_dv.txt");

    if (argc !=4){
        cout<< "usage: distvec topologyfile messagesfile changesfile";
        return 1;
    }
    string line;
    string t_file,m_file,c_file;
    t_file=argv[1];
    if (t_file_read(t_file)) return 0;
    m_file=argv[2];
    if (m_file_read(m_file)) return 0;
    c_file=argv[3];
    if (c_file_read(c_file)) return 0;
    compose_dist_vector(distance_vector,A_matrix);
    send_message(m_array,distance_vector);

    // for (int i = 0;i<node_num;i++){
    //         for (int j = 0;j<node_num;j++){
    //             cout<<A_matrix[i][j]<<" ";
    //         }
    //         cout<<endl;
    //     }
    // cout<<endl;
    for (int m = 0;m<c_num;m++){
        int c_src = c_array[m].src;
        int c_dest = c_array[m].dest;
        int c_cost = c_array[m].cost;
        A_matrix[c_src][c_dest] = c_cost;
        A_matrix[c_dest][c_src] = c_cost;
        compose_dist_vector(distance_vector,A_matrix);
        send_message(m_array,distance_vector);
        // for (int i = 0;i<node_num;i++){
        //     for (int j = 0;j<node_num;j++){
        //         cout<<A_matrix[i][j]<<" ";
        //     }
        //     cout<<endl;
        // }
        // cout<<endl;
    }
    cout<<"Complete. Output file written to output_dv.txt."<<endl;
}

int c_file_read(string path){
    string line;
    //int node_num;
    int i=0;
    string src,dst,distance;
    ifstream file(path); // example.txt 파일을 연다. 없으면 생성. 
	if(file.is_open()){
		while(getline(file, line)) {
            stringstream ss(line);
            ss >>src>>dst>>distance;
            c_array[i].src=stoi(src);
            c_array[i].dest=stoi(dst);
            if (stoi(distance)<0){
                c_array[i++].cost=0;
            }
            else{
                c_array[i++].cost=stoi(distance);
            } 
		}
        c_num = i;
		file.close(); // 열었떤 파일을 닫는다. 
        // for (int k = 0;k<c_num;k++){
        //     cout<<c_array[k].src<<" "<<c_array[k].dest<<" "<<c_array[k].cost<<endl;
        // }
	} else {
		cout << "Error: open input file."<<endl;
		return 1;
	}
    return 0;
}



int t_file_read(string path){
    string line;
    //int node_num;
    int i=0;
    string src,dst,distance;
    int n_1,n_2,cost;
    ifstream file(path); // example.txt 파일을 연다. 없으면 생성. 
	if(file.is_open()){
		while(getline(file, line)) {
            if (!i){
                node_num = stoi(line);
                //cout<<node_num<<endl;
                i++;
                A_matrix = new int*[node_num];
                distance_vector = new Edge*[node_num];
                for(int j = 0; j < node_num; j++){
                    A_matrix[j] = new int[node_num];
                    distance_vector[j] = new Edge[node_num];
                }               
            }
            else {
                stringstream ss(line);
                ss >> src >>dst>>distance;
                //cout<<src<<dst<<distance<<endl;
                n_1 = stoi(src);
                n_2 = stoi(dst);
                cost = stoi(distance);
                A_matrix[n_1][n_2] = cost;
                A_matrix[n_2][n_1] = cost;
            }
		}
		file.close(); // 열었떤 파일을 닫는다. 
        // for (int k = 0;k<node_num;k++){
        //     for (int m=0;m<node_num;m++){
        //         cout<<A_matrix[k][m]<<" ";
        //     }
        //     cout<<endl;
        // }
	} else {
		cout << "Error: open input file."<<endl;
		return 1;
	}
    return 0;
}


int m_file_read(string path){
    string line,message,remain_message;
    //int node_num;
    int i=0;
    int n_1,n_2;
    ifstream file(path); // example.txt 파일을 연다. 없으면 생성. 
	if(file.is_open()){
		while(getline(file, line)) {
            istringstream iss(line);
            iss >> n_1 >> n_2;
            getline(iss>>ws,message);
            m_array[i].src = n_1;
            m_array[i].dest = n_2;
            m_array[i].m = message;
            //cout<<m_array[i].src<<" "<<m_array[i].dest<<" "<<endl;
            //cout<<m_array[i].m<<endl;
            i++;
            m_num = i;
		}
		file.close(); // 열었떤 파일을 닫는다. 

	} else {
		cout << "Error: open input file."<<endl;
		return 1;
	}
    return 0;
}


int compose_dist_vector(Edge** distance_vector,int** A_matrix){
    for (int i = 0;i<node_num;i++){
        for (int j=0;j<node_num;j++){
            if (A_matrix[i][j]){
                distance_vector[i][j].distance = A_matrix[i][j];
                distance_vector[i][j].dest = j;
                distance_vector[i][j].next = j;
            }
            else if (i==j) {
                distance_vector[i][j].distance = 0;
                distance_vector[i][j].dest = j;
                distance_vector[i][j].next = j;
            }
            else{
                distance_vector[i][j].distance =INF;
                distance_vector[i][j].dest = j;
                distance_vector[i][j].next = -1;
            }
        }
    }
    // for (int i = 0;i<node_num;i++){
    //     for (int j=0;j<node_num;j++){
    //         cout<<distance_vector[i][j].dest<<" "<<distance_vector[i][j].next<<" "<<distance_vector[i][j].distance<<endl;
    //     }
    //     cout<<endl;
    // }
    for (int i=0;i<node_num;i++){
        for (int j=0;j<node_num;j++){
            if ((distance_vector[i][j].next!=-1)&&(i!=j)){ //if connected
                send_dist_vector(distance_vector,i,distance_vector[i][j].next);
            } 
        }
    }
    for (int i = 0;i<node_num;i++){
        for (int j=0;j<node_num;j++){
            if (distance_vector[i][j].distance ==200) continue;
            fout<<distance_vector[i][j].dest<<" "<<distance_vector[i][j].next<<" "<<distance_vector[i][j].distance<<endl;
        }
        fout<<endl;
    }
    return 1;
}


int send_dist_vector(Edge** distance_vector,int source, int dest){
    int update_flag=0;
    int temp_dist = distance_vector[source][dest].distance; //새로운 경로와의 기본적인 값
    for (int i=0;i<node_num;i++){
        if  ((distance_vector[source][i].next !=-1)&&(source!=i)){ //source 측에서 connected 되어있는 경로에 대해서
            if (distance_vector[dest][i].distance > (distance_vector[source][i].distance+temp_dist)){
                distance_vector[dest][i].next =source; //source로 다음으로 가야한다 경유하기 때문
                distance_vector[dest][i].distance = distance_vector[source][i].distance+temp_dist;
                update_flag = 1;
                //update가되었으므로 이 부분에서 다시 호출을 해야함
            }
            else if (distance_vector[dest][i].distance == (distance_vector[source][i].distance+temp_dist)){
                if (distance_vector[dest][i].next > source){ //tie break rule
                    distance_vector[dest][i].next =source; //더 적은 id쪽으로 간다.
                    update_flag =1;
                    //update 반영해야한다.
                }
            }
        }
    }
    if (update_flag){
        for (int j = 0 ; j<node_num;j++){
            if ((distance_vector[dest][j].next !=-1)&&(dest!=j)){ //if connected
                send_dist_vector(distance_vector,dest,distance_vector[dest][j].next);
            } 
        }
        update_flag = 0;
    }
    return 1;
}

void send_message(Message* m_array,Edge** distance_vector){
    int src;
    int dest;
    int cost;
    int path[100];
    string message;
    int path_count;
    for (int i=0;i<m_num;i++){
        cost = 0;
        path_count = 0;
        src = m_array[i].src;
        dest = m_array[i].dest;
        message = m_array[i].m;
        //cout<<src<<"to "<<dest<<endl;
        path[path_count++] = src;
        if (distance_vector[src][dest].distance ==200){
            fout<<"from "<<src<<" to "<<dest<<" cost infinite hops unreachable ";
            fout<<"message "<<message<<endl;
            continue;
        }

        cost += distance_vector[src][dest].distance;
        fout<<"from "<<src<<" to "<<dest<<" cost "<<cost;
        while(distance_vector[src][dest].next!=dest){
            src = distance_vector[src][dest].next;
            path[path_count++] = src;
        }
        //cost += distance_vector[src][dest].distance;
        fout<<" hops ";
        for (int j=0;j<path_count;j++){
            fout<<path[j]<<" ";
        }
        fout<<"message "<<message<<endl;
    }
    fout<<endl;
    return ;
}