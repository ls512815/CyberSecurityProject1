/* 
Name: Lydia Shiffler
Date: 2/14/19
Description: reading in binary file information 
Class: CS4900 - special topics: Computer Security
*/




#include <fstream>
#include <iostream>
#include <cstring>

using namespace std; 


void skip(ifstream &ins) //checking for zeros
{

	char i;
	ins.get(i);
	while (ins.good())
	{
		if(i == 0)
		{
			ins.get(i);
		}
		else
		{
			break;
		}
		
	}
	ins.seekg(ins.tellg() - 1);
	cout << ins.tellg() << endl;
};


int main(int argc, char *args[])
{

struct tmufs_header_t
{
	unsigned short magic_number; // magic number (0x1234) tells us that we have indeed a TMUFS
	unsigned long number_dentries; // total number of directory entries in the TMUFS
	unsigned long data_area_start; // byte offset at which the data area of the TMUFS starts
	unsigned long data_checksum; // contains a checksum over the whole data -> error checking
	
};


struct bin_format_t
{
	int id;
	char name[16];
	float weight;
	float height;
};


const int TMUFS_FN_LENGTH = 10;
struct tmufs_dentry_t
{
 	unsigned long file_size; // file size in byte
 	unsigned char file_attributes; // attribute characterize the file (see above)
 	unsigned long data_start; // byte offset at which the file contents start
 	char filename[TMUFS_FN_LENGTH];// name of the file (max 9 characters (+null byte))
 	
};

#define TMUFS_ATTRIBUTE_ASCII 0x01
#define TMUFS_ATTRIBUTE_BINARY 0x02



bin_format_t bin_format; 
strcpy (bin_format.name, args[1]);
ifstream ins (bin_format.name, ios::in | ios::binary);


tmufs_header_t header; //declaring header

int numbytes = 32;
char headerbuff[32];

ins.read(headerbuff, numbytes);
/*
cout << "All of the contents in the file:" << endl;


for(int i = 0; i < 32 ; i++) //printing out whole file
{

	cout << hex << (int)headerbuff[i] << endl;
}
cout << "______________________________________________________________________________" << endl;
*/
cout << "TMUFS HEADER" << endl;
cout << "______________________________________________________________________________" << endl;

//magic_number
memcpy((char*)&header.magic_number, headerbuff, 2); 
cout << "magic number: " << header.magic_number << endl;

//number_dentries
header.number_dentries = headerbuff[8];
cout << "number of directory entries: " << header.number_dentries << endl;

//data_area_start
char buff1[16];

for(int i = 16; i < 32; i++) //storing the bits from 17 on into buff1
{
	buff1[i - 16] = headerbuff[i];
}
memcpy((char*)&header.data_area_start, buff1, 1); //copying the first byte header.data_area_start

cout << "Byte offset of data area in " << bin_format.name << " : " << dec << header.data_area_start << endl;

 //data check_sum
char buff2[8];
for(int i = 24; i < 32; i++)
{
	buff2[i - 24] = headerbuff[i];
}
memcpy((char*)&header.data_checksum, buff2, 1);

cout << "Checksum of the data area: " << header.data_checksum << endl;
cout << "______________________________________________________________________________" << endl;

tmufs_dentry_t entry[header.number_dentries];

ins.seekg(32);

char i;

cout <<"______________________________________________________________________________" << endl;
for(int i = 0; i < header.number_dentries; i ++)
{	
	skip(ins);
	ins.read((char*) &entry[i].file_size,sizeof(entry[i].file_size));
	skip(ins);
	ins.read((char*) &entry[i].file_attributes, sizeof(entry[i].file_attributes));
	skip(ins);
	ins.read((char*) &entry[i].data_start, sizeof(entry[i].data_start));
	skip(ins);
	ins.read((char*) &entry[i].filename, sizeof(entry[i].filename));
	skip(ins);
}

cout << "______________________________________________________________________________" << endl;
cout <<"File No.          File Name           Size         Start Offset          Attributes" << endl;
cout << "______________________________________________________________________________" << endl;
for(int i = 0; i < 4; i++)
{
	
	cout << i + 1 << "               " << entry[i].filename << "                  "  << entry[i].file_size << "                 " <<  entry[i].data_start << "       " << entry[i].file_attributes << endl;
	

}
		
	
for(int i = 0; i < header.number_dentries; i ++)
{
	cout << "______________________________________________________________________________" <<endl;
	char k;
	ins.seekg(entry[i].data_start);
	cout << "Contents of File: " << entry[i].filename << endl;
	cout << "______________________________________________________________________________" <<endl;

	if(entry[i].file_attributes & TMUFS_ATTRIBUTE_ASCII)
	{
		for(int j = 0; j < entry[i].file_size; j++)
		{
			ins.get(k);
			cout << k;
		}
	}

	if(entry[i].file_attributes & TMUFS_ATTRIBUTE_BINARY)
	{
		cout << "HEXDUMP" << endl;
		for(int j = 0; j < entry[i].file_size; j++)
		{
			
			cout << hex << ins.get();
		}
	}
	cout << endl;
	cout << endl;

}

return 0;
}