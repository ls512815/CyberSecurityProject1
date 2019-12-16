/* 
Computer Security and Forensics 4900
Name: Lydia Shiffler
Date: 3/6/19 
Description: Homework 2, part 2
*/

#include <iostream>
#include "ext2fs.h"
#include <fstream>
#include <cstring>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <time.h>

using namespace std;

/*
everything needed for the superblock:
magic number //should be 0xE5F3
inodes count //the total number of inodes in the file system
block count //the total number of blocks in the file system
free blocks count //the number of available (=free) blocks in the file system
free inodes count //the number of available (=unused) inodes in the file system
first data block //tells you which logical block is 'officially' the first block of the ext2 file system
block groups count //the number of block groups in the file system
block size // block size of given file
fragment size //should always be equal to the block size 
fragments per group //given default settings - the same as blocks per group
inodes per group //the number of inodes stored in the inode table in each block group. 
number of preallocated directory blocks //describes the number of plocks preallocated to a file when it is created
inode_size //inode size in bytes
*/

ext2_inode inode(int i)
{

}

class extfs
{
	public:

		void block(int block, ifstream& ins, ext2_super_block &superblock)
		{
			ins.seekg((1024 >> superblock.s_log_block_size) * block);
		}	


		void output() // outputs all the data
		{
			cout << "superblock magic number: " << hex << superblock.s_magic << dec << endl; 
			cout << endl;
			cout << "inodes count: " << superblock.s_inodes_count << endl;
			cout << endl;
			cout << "Blocks count: " << superblock.s_blocks_count << endl;
			cout << endl;
			cout << "Free Blocks count: " << superblock.s_free_blocks_count << endl;
			cout << endl;
			cout << "Free Inodes count: " << superblock.s_free_inodes_count << endl;
			cout << endl;
			cout << "First Data Block: " << superblock.s_first_data_block<< endl;
			cout << endl;

			int block_size = 1 << (10 + superblock.s_log_block_size);

			cout << "Block size: " << block_size << endl;
			cout << endl;
			cout << "Fragment size: " << superblock.s_log_frag_size << endl;
			cout << endl;
			cout << "Blocks per group: " << superblock.s_blocks_per_group << endl;
			cout << endl;
			cout << "Fragments per group: " << superblock.s_frags_per_group <<endl;
			cout << endl;
			cout << "Inodes per group: " << superblock.s_inodes_per_group <<endl;
			cout << endl;

			int num_blocks = ceil((float)superblock.s_blocks_count / (float)superblock.s_blocks_per_group);

			cout << "Number of Block Groups: " << num_blocks << endl;
			cout << endl;
			cout << "Number of Pre-Allocate Blocks: " << (int)superblock.s_prealloc_blocks << endl;
			cout << endl;
			cout << "Number of Pre-Allocate Directory Blocks: " << (int)superblock.s_prealloc_dir_blocks << endl;
			cout << endl;
			cout << "Inode size: " << max((int)superblock.s_inode_size, (int)sizeof(struct ext2_inode)) << endl;
			cout << endl;
			cout << endl;
			cout << endl;


			for(int j = 0; j < Number_of_groupdecr; j++)
			{

				cout << "Information for Block Group " << j << endl;
				cout << endl;
				cout << "___________________________________________________________________" << endl;
				cout << endl;
				cout << "group range: " << (superblock.s_blocks_per_group*j) + 1 << " to " << min(superblock.s_blocks_count - 1, (superblock.s_blocks_per_group*(j + 1))) << endl;
				cout << endl;
				cout << "Blocks bitmap block at: " << groupdecr[j].bg_block_bitmap << endl;
				cout << endl;
				cout << "Inodes Bitmap block at: " << groupdecr[j].bg_inode_bitmap << endl;
				cout << endl;
				cout << "Free Blocks Count: " << groupdecr[j].bg_free_blocks_count << endl;
				cout << endl;
				cout << "Inodes table block: " << groupdecr[j].bg_inode_table << endl;
				cout << endl;
				cout << "Free Inodes count: " << groupdecr[j].bg_free_inodes_count << endl;
				cout << endl;
				cout << "Used Directories count: " << groupdecr[j].bg_used_dirs_count << endl;
				cout << endl;
				cout << "___________________________________________________________________" << endl;

			}
			return 0;
		}

		int superblock() //gets superblock 
		{
			ins.seekg(1024); //where superblock sarts

			ins.read((char*) &superblock, sizeof(superblock)); //read superblocki n
			if(superblock.s_magic != 0xef53) //if it gives the wrong magic num, it is invalid
			{
				cout << "not valid" << endl;
				return(-1);
			}
			if(superblock.s_log_block_size == 0)
			{
				block(2, ins, superblock);
			}

			else
			{
				block(1, ins, superblock);
			}

		}

		void groupdec() //gets number of group descriptor
		{
			if(superbloack.s_log_block_size == 0
			{
				to_blokc(2);
			}
			else
			{
				{
					to_block(1);
				}
			}
			int Number_of_groupdecr = 1 + (superblock.s_blocks_count - 1) / superblock.s_blocks_per_group;
			for(int i = 0; i < Number_of_groupdecr; i++)
			{
				groupdecr.resize(Number_of_groupdecr); //resize number of group descriptor
				ins.read((char*) &groupdecr[i], sizeof(groupdecr[i]));
			}

		}

		void open(string filename)
		{
			ins.open(filename.c_str(), ios::binary); // read file in
			if(ins.fail()) // checks to see if it the read in was succesful 
			{
				cout << "could not open" << endl;
				return (-1);
			}
		}

		ext2_inode inode(int inumber) //given in assignment -- gets inodes
		{
			unsigned int i_group = ( inumber - 1 ) / superblock.s_inodes_per_group;
			unsigned int i_index = ( inumber - 1 ) % superblock.s_inodes_per_group;

			unsigned int i_offset = block_size * groupdecr[i_group].bg_inode_table +
			(i_index * inode_size);
			struct ext2_inode ext2_in;
			ins.seekg(i_offset);
			ins.read((char*)&ext2_in, sizeof(struct ext2_inode));
			return ext2_in;
		}
		//function that gets directory entries at this inode --takes inode and returns vector of directory entries (may not want to do it this way)


		vector<ext2_dir_entry_2> directory_entries(ext2_inode inode)
		{
			vector <ext2_dir_entry_2> entries;
			ext2_fir_entry1* entry;
			char block[blocksize];

			for(int i = 0; i <= 12; i++)
			{
				if(inode.i_block[j] != 0)
				{
					to_block(inode.i_block[i]);
					ext2.read(block,blocksize);
					int num = 0;
					entry = (struct ext2_dir_entry_2*) &block[0];
					string name;
					for(int j = 0; j < blocksize; num++)
					{
						if(entry -> inode != 0)
						{
							entry = (struct ext2_dir_entry_2*) &block[j];
							name = name.substr(0, entry -> name_len);
							i += entry -> rec_len;
							entries.resize(num + 1);
							entries[num].file_type = entry -> file_type;
							entries[num].inode = entry -> inode;
							strcpy(entries[num].name, filename.c_str());
							entries[num].name_len = entry -> name_len;
							entries[num].rec_len = entry -> rec_len;
						}
						else
						{
							{
								entry = (struct ext2_dir_entry_2*) &block
							}
						}
						
					}
				}
			}


		}
		//{
		//	vector <bg_used_dirs_count
		//	vecotr <ext2_gorup_desc
		//}

		
		string time(ext2_inode in_t)
		{
			time_t a_rawtime = inode.i_atime;
			char date[255]; // buffer to store time/date string
			struct tm* a_time_tm;
			atime_tm = localtime(&a_rawtime); // convert from time_t to tm format
			strftime(date, 255, "%b %d %R", atime_tm);
			string return_date(date);
			return(return_date);

		}


		void display(ext2_inode in_t)
		{
			vector <ext2_dir_entry2> entries;
			ext2_dir_entry_2* entry;
			char block[blocksize];

			for(int i = 0; i <12; i++)
			{
				if(inode.i_block[i] != 0)
				{
					to_block(inode.i_block[i]);
					ext2.read(block, blocksize);

					string name;
					for(int j = 0; j < blocksize; j++)
					{
						cout << hex << block[j];
					}
				}
			}
			if(inode.i_size > blocksize * 12)
			{
				int numbytes = blocksize*12;
				int datablock;
				int indirectblock[blocksize/4];
				to_block(inode.i_block[12]);
				ext2.read((char *) indirect, blocksize);

				for(int j = 0; j < blocksize/4 && numbytes < inode.i_size; j++)
				{
					to_block(indirectblock[j]);

					char tempblock[blocksize];
					ext2.read(tempblock, blocksize);

					for(int i = 0; i < blocksize && numbytes < inode.i_size; i++)
					{
						cout << hex << tempblock[i];
						numbytes ++;
					}
				}
			}

		}


	private:
		int blocksize;
		ifstream ins;
		vector<ext2_group_desc> groupdecr;
	


}



int main(int argc, char **argv)
{
	
	//reading file in	
	string filename;
	string path;
	
		if(argc < 3) //entering file name and path
		{
			cout << "enter a file name: " << endl;
			cin >> filename;
			cout << "enter a path: " << endl;
			cin >> path;
		}
		else
		{
			filename = argv[1];
			path = argv[2];
		}
		ext2fs ext2fss;

		ext2fss.open(filename); //open file
		ext2fss.superblock(); //get superblock
		ext2fss.groupdec(); //get group descriptors 
		extsfss.time();

		return 0;
	

			


		

	



}
