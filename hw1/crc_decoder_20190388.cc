#include <iostream>
#include <fstream>
#include <string>
#include<cstdlib> 
#include <bitset>
#include <cstring>

typedef std::bitset<256> MaxBits;
using namespace std;
// unsigned int buffer = 0; //overflow방지
// unsigned int bit_counter = 0;
// int padding_check = 0;
// int n_bit;


 //int count_nbit(unsigned char);
int decoder(MaxBits,MaxBits,int, int);
unsigned char write_buffer = 0;
unsigned int bit_counter = 0;
int count_8bit(unsigned char);


int main(int argc, char** argv) {
    int generator_size;
	ifstream input(argv[1], ios::binary); // example.txt 파일을 연다. 없으면 생성. 
    ofstream output(argv[2],ios::binary);
    ofstream result(argv[3]);
    string str_generator = argv[4];
    MaxBits generator(str_generator);
    generator_size = str_generator.length(); //generator가 몇 bit인지 계산
    int dataword_size =atoi(argv[5]); //dataword size 저장

    if (argc !=6){
        cout<< "usage: ./crc_encoder input_file output_file generator dataword_size" ;
        return 1;
    }
    if ((dataword_size!=4) && (dataword_size!=8)){
        cout<<"dataword size must be 4 or 8."<<endl;
        return 1;
    }

	if(!input.is_open()){
		cout << "input file open error.";
		return 1;
	}
    if(!output.is_open()){
		cout << "output file open error.";
		return 1;
	}
    if(!result.is_open()){
		cout << "output file open error.";
		return 1;
	}

    char read_buffer;
    int padding;
    int byte_count = 0;
    input.read(&read_buffer, 1);
    padding = read_buffer;
    int temp_bit;
    MaxBits encdoed;
    MaxBits tmp_buffer;
    MaxBits encoded,decoded;
    int index=0;
    int check;
    int cor =0;
    int wrong = 0;
    while (input.read(&read_buffer, 1)) {
        for (int i = 7; i >= 0; i--) {
            if (padding){
                padding--;
                continue; 
            }
            temp_bit = (read_buffer >> i) & 1;
            encoded = encoded<<1; // 한칸 shift하고 
            encoded[0] = temp_bit; // lsb자리에 집어넣어서 read_buffer의 bit를 encoded에 집어넣는다.
            index++;
            if (index == (dataword_size + generator_size -1)){
                index = 0;
                if (decoder(encoded,generator,generator_size,dataword_size)){
                    cor++;
                }
                else{
                    wrong++;
                }
                decoded = encoded >> (generator_size-1);
                for (int q =dataword_size;q>0;q--){
                    if (decoded[q-1]){
                        if (count_8bit(1)){
                        //cout<<"buffer : "<<bitset<8>(write_buffer)<<endl;
                        output.write((char*)&write_buffer, 1);
                        write_buffer = 0;
                        }
                    }
                    else{
                        if (count_8bit(0)){
                        //cout<<"buffer : "<<bitset<8>(write_buffer)<<endl;
                        output.write((char*)&write_buffer, 1);
                        write_buffer = 0;
                        }
                    }
                }
                encoded.reset();
            }
        }
    }

    result<<cor+wrong<<" "<<wrong;
    input.close(); // 열었떤 파일을 닫는다.
    output.close();
    result.close();
    return 1;
}


int decoder(MaxBits encoded, MaxBits generator,int generator_size,int dataword_size){
    MaxBits tmp_buffer, bit_buffer;
    bit_buffer = encoded;
    tmp_buffer = bit_buffer;
    for (int i=dataword_size;i>0;i--){
        if (bit_buffer[i+generator_size-2]){
            bit_buffer = bit_buffer^(generator<<(i-1));
        }
    } 
    if (!bit_buffer.count()){
        return 1;
    }
    else return 0;
}


int count_8bit(unsigned char bit) {
        write_buffer <<= 1;         // Make room for next bit.
        if (bit){
            write_buffer = write_buffer|1;
        }
        bit_counter++;              // Remember we have added a bit.
        if (bit_counter == 8) {
            //output.write((char*)&buffer, 1);
            //buffer = 0;
            bit_counter = 0;
            return 1;
        }
        else{
            return 0;
        }
}
