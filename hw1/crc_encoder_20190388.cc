#include <iostream>
#include <fstream>
#include <string>
#include<cstdlib> 
#include <bitset>
#include <cstring>


typedef std::bitset<256> MaxBits;
using namespace std;
int bit_reader(char);
unsigned char write_buffer = 0;
unsigned int bit_counter = 0;
int count_8bit(unsigned char);
// read_buffer_to_bits(char);
MaxBits encoder(MaxBits,MaxBits,int, int);


int main(int argc, char** argv) {
    int generator_size;
	ifstream input(argv[1], ios::binary); // example.txt 파일을 연다. 없으면 생성. 
    ofstream output(argv[2],ios::binary);
    string str_generator = argv[3];
    MaxBits generator(str_generator);
    generator_size = str_generator.length(); //generator가 몇 bit인지 계산
    int dataword_size =atoi(argv[4]); //dataword size 저장

    if (argc !=5){
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

    char read_buffer;
    int temp_bit;
    string string_buffer;
    MaxBits bit_buffer;
    MaxBits a(15);
    MaxBits high;
    MaxBits low;
    MaxBits high_encoded, low_encoded, encoded;
    int count = 0;
    
    while (input.read(&read_buffer, 1)){
        count++;
    }

    int padding;
    int tmp_size = dataword_size + generator_size -1;
    int total_bit = count*8;
    if (dataword_size ==4){
        padding = total_bit*tmp_size/dataword_size%8;
        padding = 8-padding;
        if (padding == 8){
            padding = 0;
        }
    }
    else{
        padding = total_bit*tmp_size/dataword_size%8;
        padding = 8-padding;
        if (padding == 8){
            padding = 0;
        }
    }
    input.close();
    input.open(argv[1], ios::binary);
    output.write((char*)&padding, 1);
    for (int p=0;p<padding;p++){
        count_8bit(0);
    }
    while (input.read(&read_buffer, 1)) { //1byte 씩읽어온다.
        if (dataword_size == 4){
            for (int i = 7; i >= 4; i--) {//high 4개 읽어
                temp_bit = (read_buffer >> i) & 1;
                high[i-4] = temp_bit;
            }
            for (int i = 3; i >= 0; i--) {//low 4개 읽어
                temp_bit = (read_buffer >> i) & 1;
                low[i] = temp_bit;
            }
            high_encoded = encoder(high,generator,generator_size,dataword_size);
            low_encoded = encoder(low,generator,generator_size,dataword_size);
            for (int i= dataword_size + generator_size -1 ;i> 0;i--){
                if (high_encoded[i-1]){
                    if (count_8bit(1)){
                        output.write((char*)&write_buffer, 1);
                        //cout<< bitset<8> (buffer)<<" ";
                        write_buffer = 0;
                    }
                }
                else{
                    if (count_8bit(0)){
                        output.write((char*)&write_buffer, 1);
                        //cout<< bitset<8> (buffer)<<" ";
                        write_buffer = 0;
                    }
                }
            }
            for (int i= dataword_size + generator_size -1 ;i> 0;i--){
                if (low_encoded[i-1]){
                    if (count_8bit(1)){
                        output.write((char*)&write_buffer, 1);
                        write_buffer = 0;
                    }
                }
                else{
                    if (count_8bit(0)){
                        output.write((char*)&write_buffer, 1);
                        write_buffer = 0;
                    }
                }
            }
        }
        else{
            for (int i = 7; i >= 0; i--) {
                temp_bit = (read_buffer >> i) & 1;
                bit_buffer[i] = temp_bit;
            }
            encoded=encoder(bit_buffer,generator,generator_size,dataword_size);
            for (int i= dataword_size + generator_size -1 ;i> 0;i--){
                if (encoded[i-1]){
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
        }
    }
    input.close(); // 열었떤 파일을 닫는다.
    output.close();
}

// int read_buffer_to_bits(char read_buffer){
//     for (int i = 7; i >= 0; i--) {
//         int bit = (read_buffer >> i) & 1;
//         cout << bit;
//     }
// }
MaxBits encoder(MaxBits bit_buffer, MaxBits generator,int generator_size,int dataword_size){
    MaxBits encoded,tmp_buffer;
    bit_buffer = bit_buffer<<(generator_size-1); //0 붙여줌
    tmp_buffer = bit_buffer;
    for (int i=dataword_size;i>0;i--){
        if (bit_buffer[i+generator_size-2]){
            bit_buffer = bit_buffer^(generator<<(i-1));
        }
    }
    encoded = tmp_buffer|bit_buffer;
    return encoded;
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
