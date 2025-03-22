#include <iostream>
#include <fstream>
#include <string>
#include<cstdlib> 
#include <cstring>
#include <sstream>
#include <queue>
#include <vector>
#define INF 200
//todo 
//1. read topology file and make table using algorithm
//2. send message 
//3. reflect contents in change file

using namespace std;

int **A_matrix; //100 100 짜리 sparse matrix
int *SPT;

struct Edge {
    int dest;
    int distance;
    int next;
    int parent;
    int SPT_flag;
};
Edge** linkstate;

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

int dijkstra(int);
int check_next();

int m_file_read(string path);
int t_file_read(string path);
int c_file_read(string path);


void send_message(Message *,Edge**);
ofstream fout;


int main(int argc, char** argv) {
    m_array = new Message[100]; //일단 100개로 할당
    c_array = new Change[100]; //일단 100개로 할당
    fout.open("output_ls.txt");

    if (argc !=4){
        cout<< "usage: linkstate topologyfile messagesfile changesfile";
        return 1;
    }
    string line;
    string t_file,m_file,c_file;
    t_file=argv[1];
    if (t_file_read(t_file)) return 1;
    m_file=argv[2];
    if (m_file_read(m_file)) return 1;
    c_file=argv[3];
    if (c_file_read(c_file)) return 1;


    for (int i =0;i<node_num;i++){ //for every node
        dijkstra(i); //do dijkstra algorithm
    }
    check_next();
    send_message(m_array,linkstate);

    for (int m = 0;m<c_num;m++){
        int c_src = c_array[m].src;
        int c_dest = c_array[m].dest;
        int c_cost = c_array[m].cost;
        A_matrix[c_src][c_dest] = c_cost;
        A_matrix[c_dest][c_src] = c_cost;
        for (int i =0;i<node_num;i++){ //for every node
            dijkstra(i); //do dijkstra algorithm
        }
        check_next();
        send_message(m_array,linkstate);
    }
    cout<<"Complete. Output file written to output_ls.txt"<<endl;
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
                i++;
                A_matrix = new int*[node_num];
                SPT = new int[node_num];
                linkstate = new Edge*[node_num];
                for(int j = 0; j < node_num; j++){
                    A_matrix[j] = new int[node_num];
                    linkstate[j] = new Edge[node_num];
                }          
            }
            else {
                stringstream ss(line);
                ss >> src >>dst>>distance;
                n_1 = stoi(src);
                n_2 = stoi(dst);
                cost = stoi(distance);
                A_matrix[n_1][n_2] = cost;
                A_matrix[n_2][n_1] = cost;
            }
		}
		file.close(); // 열었떤 파일을 닫는다. 
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


int dijkstra(int src){
    int tmp_cost,tmp_node;
    SPT[0] = src; //첫 시작은 본인부터
    for (int j = 0;j<node_num;j++){ //다익스트라의 첫 부분
        if (A_matrix[src][j]){
            linkstate[src][j].distance = A_matrix[src][j];
            linkstate[src][j].dest = j;
            linkstate[src][j].parent = src;
            linkstate[src][j].SPT_flag = 0;
        }
        else if (src==j) {
            linkstate[src][j].distance = 0;
            linkstate[src][j].dest = j;
            linkstate[src][j].parent = src;
            linkstate[src][j].SPT_flag = 1;
        }
        else{
            linkstate[src][j].distance = INF;
            linkstate[src][j].dest = j;
            linkstate[src][j].parent = 0;
            linkstate[src][j].SPT_flag = 0;
        }
    }

    for (int i = 1;i<node_num;i++){ // 이후 node_num -1 까지 진행
        //가장 distance가 적은애를 SPT에 추가
        int min = INF-1; //min을 199로 줘서 연결안된애는 제외하면서 가장 큰 값으로 초기화
        for (int j=0;j<node_num;j++){ //check every node
            if ((linkstate[src][j].distance<min)&&(!linkstate[src][j].SPT_flag)){
                min = linkstate[src][j].distance;
                SPT[i] = j;
            }
        }
        tmp_node = SPT[i];
        tmp_cost = min;
        //parent는 따로 안건들여도 될 것 같음
        linkstate[src][tmp_node].SPT_flag = 1; //SPT에 추가된 애의 SPT_flag를 1로
        //SPT에 추가했다면
        for (int j=0;j<node_num;j++){ //check every node
            if ((j==src)||(linkstate[src][j].SPT_flag)) continue; //src자기 자신이거나 SPT에 이미 있다면 패스
            if (A_matrix[tmp_node][j]){ //현재 SPT 노드와 연결이 되어있다면
                if (linkstate[src][j].distance > (tmp_cost + A_matrix[tmp_node][j])){ //SPT 통해서 가는게 빠르면
                    linkstate[src][j].parent = tmp_node;
                    linkstate[src][j].distance = tmp_cost + A_matrix[tmp_node][j];
                    //update distance and parent info
                }
                else if (linkstate[src][j].distance == (tmp_cost + A_matrix[tmp_node][j])){ //tie breaking rule 3
                    if (linkstate[src][j].parent > tmp_node) linkstate[src][j].parent = tmp_node;
                    //현재 부모보다 SPT가 더 값이 작으면? 부모를 바꾼다 -> tie rule 3
                }
            }
        }
    }
    return 1;
}


int check_next(){
    int tmp_parent;
    int before;
    for (int i=0;i<node_num;i++){
        for (int j=0;j<node_num;j++){
            if (i==j){
                linkstate[i][j].next = i;
                continue;
            }
            if (linkstate[i][j].distance == 200){
                linkstate[i][j].next = -1;
                continue;
            }
            // if (i==linkstate[i][j].parent){
            //     linkstate[i][j].next =j;
            // }
            tmp_parent = linkstate[i][j].parent;
            before = j;
            while (tmp_parent!=i){
                before = tmp_parent;
                tmp_parent = linkstate[i][tmp_parent].parent;
            }
            linkstate[i][j].next = before;
        }
    }
     for (int i=0;i<node_num;i++){
        for (int j=0;j<node_num;j++){
            if (linkstate[i][j].distance == 200) continue;
            fout<<linkstate[i][j].dest<<" "<<linkstate[i][j].next<<" "<<linkstate[i][j].distance<<endl;
        }
        fout<<endl;
    }
    return 1;
}


void send_message(Message* m_array,Edge** linkstate){
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
        path[path_count++] = src;
        if (linkstate[src][dest].distance ==200){
            fout<<"from "<<src<<" to "<<dest<<" cost infinite hops unreachable ";
            fout<<"message "<<message<<endl;
            continue;
        }

        cost += linkstate[src][dest].distance;
        fout<<"from "<<src<<" to "<<dest<<" cost "<<cost;
        while(linkstate[src][dest].next!=dest){
            src = linkstate[src][dest].next;
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
}