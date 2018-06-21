//jisoo Heo
//B115233
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "sfs_types.h"
#include "sfs_func.h"
#include "sfs_disk.h"
#include "sfs.h"

/* BIT operation Macros */
/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

#define MAX_DENTRY (SFS_DENTRYPERBLOCK * SFS_NDIRECT)
#define MAX_NBITMAP 1000
#define INVALID_BLOCK 0
#define MAX_FILESIZE 73216
#define SIZE_BLOCKPTR	4  //(byte)
#define NENTRY_DPBLOCK (SFS_BLOCKSIZE / SIZE_BLOCKPTR)

void release_block(unsigned int);
unsigned int allocate_block();
void dump_directory();

unsigned int flag4touch2cpin;
unsigned int fileInodeBlockNo;
static struct sfs_super spb;	// superblock
static struct sfs_dir sd_cwd = { SFS_NOINO }; // current working directory

void error_message(const char *message, const char *path, int error_code)
{
	switch (error_code)
	{
	case -1://
		printf("%s: %s: No such file or directory\n",message, path); return;
	case -2://
		printf("%s: %s: Not a directory\n",message, path); return;
	case -3://
		printf("%s: %s: Directory full\n",message, path); return;
	case -4://
		printf("%s: %s: No block available\n",message, path); return;
	case -6://
		printf("%s: %s: Already exists\n",message, path); return;
	case -7://
		printf("%s: %s: Directory not empty\n",message, path); return;
	case -8://
		printf("%s: %s: Invalid argument\n",message, path); return;
	case -9://
		printf("%s: %s: Is a directory\n",message, path); return;
	case -10:
		printf("%s: %s: Is not a file\n",message, path); return;
	case -11://
		printf("%s: can't open %s input file\n",message, path); return;
	case -12://
		printf("%s: %s: Large file size\n",message, path); return;
	case -13://
		printf("%s: %s: File read error\n",message,path); return;
	default:
		printf("unknown error code\n");
		return;
	}
}

void sfs_mount(const char* path)
{
	if( sd_cwd.sfd_ino !=  SFS_NOINO )
	{
		disk_close();
		printf("%s, unmounted\n", spb.sp_volname);
		bzero(&spb, sizeof(struct sfs_super));
		sd_cwd.sfd_ino = SFS_NOINO;
	}

	printf("Disk image: %s\n", path);

	disk_open(path);
	disk_read( &spb, SFS_SB_LOCATION );

	printf("Superblock magic: %x\n", spb.sp_magic);

	assert( spb.sp_magic == SFS_MAGIC );
	
	printf("Number of blocks: %d\n", spb.sp_nblocks);
	printf("Volume name: %s\n", spb.sp_volname);
	printf("%s, mounted\n", spb.sp_volname);
	
	sd_cwd.sfd_ino = 1;		//init at root
	sd_cwd.sfd_name[0] = '/';
	sd_cwd.sfd_name[1] = '\0';
}

void sfs_umount()
{

	if( sd_cwd.sfd_ino !=  SFS_NOINO )
	{
		disk_close();
		printf("%s, unmounted\n", spb.sp_volname);
		bzero(&spb, sizeof(struct sfs_super));
		sd_cwd.sfd_ino = SFS_NOINO;
	}
}

void sfs_touch(const char* path)
{

	int i,j;
	int found =0;
	unsigned int nEntry,blockNo,entryNo,directNo; 
	struct sfs_inode currDirInode;
	struct sfs_inode newbieInode;
	struct sfs_dir currDir[SFS_DENTRYPERBLOCK];

	disk_read(&currDirInode,sd_cwd.sfd_ino);
	nEntry = currDirInode.sfi_size/sizeof(struct sfs_dir);
	if(nEntry < MAX_DENTRY)
	{
		//***STEP1: check same path existence
		for(i=0;i<SFS_NDIRECT;i++)
		{
			if(currDirInode.sfi_direct[i] == INVALID_BLOCK) break; 
	
			disk_read(currDir,currDirInode.sfi_direct[i]);
			for(j=0;j<SFS_DENTRYPERBLOCK;j++)
			{
				if(currDir[j].sfd_ino != SFS_NOINO && !strcmp(path,currDir[j].sfd_name))
				{
					flag4touch2cpin = 1;
					error_message("touch",path,-6);
					return;
				}
			}
		}
		
		//***STEP2: seek empty dir entry
		for(directNo=0; directNo < SFS_NDIRECT; directNo++)
		{
			if(currDirInode.sfi_direct[directNo] == INVALID_BLOCK)
			{
				entryNo = 0;
				break;
			}
			disk_read(currDir,currDirInode.sfi_direct[directNo]);
			for(entryNo=0;entryNo<SFS_DENTRYPERBLOCK;entryNo++)
			{
				if(currDir[entryNo].sfd_ino == SFS_NOINO)
				{
					found = 1;
					blockNo = currDirInode.sfi_direct[directNo];
					break;
				}
			}
			if(found == 1) break;
		}

		//*** STEP3-1: Update current dir Inode info. (total entry No. & direct pointer)
		//*** STEP3-2: Init dir Block
		currDirInode.sfi_size += sizeof(struct sfs_dir);	
		if(found  == 0)
		{
			if((blockNo = allocate_block()) == INVALID_BLOCK)
			{
				flag4touch2cpin = 1;
				error_message("touch",path,-4);
				return;
			}
			currDirInode.sfi_direct[directNo] = blockNo;
			bzero(currDir,SFS_BLOCKSIZE);
			for(i=0;i<SFS_DENTRYPERBLOCK;i++) currDir[i].sfd_ino = SFS_NOINO;
		}
		else disk_read(currDir,blockNo);

		//***STEP4: Update current directory block (insert newbie entry)
		if((currDir[entryNo].sfd_ino = allocate_block()) == INVALID_BLOCK)
		{
			flag4touch2cpin = 1;
			error_message("touch",path,-4);
			return;
		}
		strncpy(currDir[entryNo].sfd_name,path,SFS_NAMELEN);
		fileInodeBlockNo = currDir[entryNo].sfd_ino;
	
		//***STEP5: Init newbie Inode
		bzero(&newbieInode,sizeof(struct sfs_inode));
		newbieInode.sfi_size = 0;
		newbieInode.sfi_type = SFS_TYPE_FILE;
		
		//***STEP6: Succesfully file created -> write to disk
		disk_write(&currDirInode,sd_cwd.sfd_ino);
		disk_write(currDir,blockNo);
		disk_write(&newbieInode,currDir[entryNo].sfd_ino);
	}	
	else
	{
		flag4touch2cpin = 1;
		error_message("touch",path,-3);
	}
}

void sfs_ls(const char* path)
{
	int i,j;
	struct sfs_inode currDirInode, subInode;
	struct sfs_dir dirEntry[SFS_DENTRYPERBLOCK];
	static struct sfs_inode subDirInode;
	static int flag = 0;
	
	//To implement recursive function, use static variable (flag)
	if(flag == 0) disk_read(&currDirInode,sd_cwd.sfd_ino);
	else currDirInode = subDirInode;

	//should search whole direct pointer in this HW
	//in case with making a lot of directory, direct pointer[0] could be overflow.
	//after that, another dir block will be allocated to be used.
	//also, some directory entry can be removed,
	//but there is no mechanism whether reordering entry or not, just keep the entry.
		
	for(i=0;i<SFS_NDIRECT;i++)
	{
			if(currDirInode.sfi_direct[i] == INVALID_BLOCK) break;
			disk_read(dirEntry, currDirInode.sfi_direct[i]);

			for(j=0;j<SFS_DENTRYPERBLOCK;j++)
			{
				if(path == NULL){
					if(dirEntry[j].sfd_ino != SFS_NOINO)
					{
						disk_read(&subInode,dirEntry[j].sfd_ino);
						if(subInode.sfi_type == SFS_TYPE_FILE)
							printf("%s\t",dirEntry[j].sfd_name);
						else
							printf("%s/\t",dirEntry[j].sfd_name);
					}
				}
				else{
					if(dirEntry[j].sfd_ino != SFS_NOINO && !strcmp(path,dirEntry[j].sfd_name))
					{
						disk_read(&subInode, dirEntry[j].sfd_ino);
						if(subInode.sfi_type == SFS_TYPE_FILE)
							printf("%s\n",dirEntry[j].sfd_name);
						else
						{
							subDirInode = subInode;
							flag = 1;
							sfs_ls(NULL);
							flag = 0;
						}
						return;
					}
				}
			}
	}	
	if(path==NULL)	printf("\n");
	else error_message("ls",path,-1);
}

void sfs_cd(const char* path)
{
	int i,j;
	struct sfs_inode currDirInode, subDirInode;
	struct sfs_dir dirEntry[SFS_DENTRYPERBLOCK];

	if(path==NULL)
	{
		bzero(sd_cwd.sfd_name,sizeof(SFS_NAMELEN));
		sd_cwd.sfd_ino = SFS_ROOT_LOCATION;
		strcpy(sd_cwd.sfd_name,"/");
		return;
	}

	disk_read(&currDirInode,sd_cwd.sfd_ino);
	for(i=0;i<SFS_NDIRECT;i++)
	{
		if(currDirInode.sfi_direct[i] == INVALID_BLOCK) break;
		disk_read(dirEntry,currDirInode.sfi_direct[i]);
		for(j=0;j<SFS_DENTRYPERBLOCK;j++)
		{
			if(dirEntry[j].sfd_ino != SFS_NOINO && !strcmp(path,dirEntry[j].sfd_name))
			{
				disk_read(&subDirInode, dirEntry[j].sfd_ino);
				if(subDirInode.sfi_type == SFS_TYPE_FILE) error_message("cd",path,-2);
				else memcpy((void*)&sd_cwd, (void*)&dirEntry[j],sizeof(struct sfs_dir));
				return;
			}
		}	
	}
	error_message("cd",path,-1);
}

void release_block(unsigned int blockNo)
{
	unsigned char bitmap[SFS_BLOCKSIZE];
	int mapNo,lineNo,byteNo,bitNo;

	byteNo = blockNo / CHAR_BIT;
	bitNo = blockNo % CHAR_BIT;
	mapNo = byteNo / SFS_BLOCKSIZE;
	lineNo = byteNo % SFS_BLOCKSIZE;

	disk_read(bitmap,SFS_MAP_LOCATION + mapNo);
	BIT_CLEAR(bitmap[lineNo],bitNo);
	disk_write(bitmap,SFS_MAP_LOCATION + mapNo);
}

unsigned int allocate_block()
{
	int i,j;
	unsigned int mapNo,lineNo;
	unsigned int nBlocks = spb.sp_nblocks;
	unsigned int nLine = SFS_ROUNDUP(spb.sp_nblocks,CHAR_BIT)/CHAR_BIT;
	unsigned int nMap = SFS_BITBLOCKS(spb.sp_nblocks);
	unsigned char bitmap[MAX_NBITMAP][SFS_BLOCKSIZE];
	unsigned char sampling;

	//read bitmap blocks
	for(i=0;i<nMap;i++) disk_read(bitmap[i],SFS_MAP_LOCATION+i);
	
	//sampling with byte unit
	for(i=0; i<nLine; i++)
	{
		mapNo = i/SFS_BLOCKSIZE;
		lineNo = i%SFS_BLOCKSIZE;
		sampling = bitmap[mapNo][lineNo];
		
		//check sample each bit.
		for(j=0; j<CHAR_BIT; j++)
		{
			if(i*CHAR_BIT+j > nBlocks) return INVALID_BLOCK;

			if(!BIT_CHECK(sampling,j))
			{
				BIT_SET(bitmap[mapNo][lineNo],j);
				disk_write(bitmap[mapNo],SFS_MAP_LOCATION+mapNo);
				return i*CHAR_BIT+j;
			}
		}
	}
	return INVALID_BLOCK;
}

void sfs_mkdir(const char* path) 
{
	int i,j;
	unsigned int found = 0,vcnt=0;
	unsigned int nEntry,directNo,entryNo,blockNo;
	struct sfs_inode currDirInode;
	struct sfs_inode newbieInode;
	struct sfs_dir currDir[SFS_DENTRYPERBLOCK];
	struct sfs_dir newDir[SFS_DENTRYPERBLOCK];		


	disk_read(&currDirInode,sd_cwd.sfd_ino);
	nEntry = currDirInode.sfi_size/sizeof(struct sfs_dir);
	if(nEntry < MAX_DENTRY)
	{	
		//***STEP1: check same path existence
		for(i=0;i<SFS_NDIRECT;i++)
		{	
			if(currDirInode.sfi_direct[i] == INVALID_BLOCK) break;
			disk_read(currDir,currDirInode.sfi_direct[i]);
			for(j=0;j<SFS_DENTRYPERBLOCK;j++)	
			{
				if(currDir[j].sfd_ino != SFS_NOINO && !strcmp(path,currDir[j].sfd_name))
				{
					error_message("mkdir",path,-6);
					return;	
				}
			}
		}
	
		//***STEP2: seek empty dir entry
		for(directNo=0; directNo < SFS_NDIRECT; directNo++)
		{
			if(currDirInode.sfi_direct[directNo] == INVALID_BLOCK)
			{
				entryNo = 0;
				break;
			}
		
			disk_read(currDir,currDirInode.sfi_direct[directNo]);
			for(entryNo=0; entryNo < SFS_DENTRYPERBLOCK; entryNo++)
			{
				if(currDir[entryNo].sfd_ino == SFS_NOINO)
				{
					found = 1;
					blockNo = currDirInode.sfi_direct[directNo];
					break;
				}
			}
			if(found == 1) break;
		}	
		
		//***STEP3-1: Update current dir Inode info. (total entry No. & direct pointer)
		//***STEP3-2: Init dir block
		currDirInode.sfi_size += sizeof(struct sfs_dir);
		if(found == 0) 
		{	
			//allocate new directory entry block	
			if((blockNo = allocate_block()) == INVALID_BLOCK)
			{
				error_message("mkdir",path,-4);
				return;
			}
			//update current inode info.
			currDirInode.sfi_direct[directNo] = blockNo;

			//init directory block which is not written yet.
			bzero(currDir,SFS_BLOCKSIZE);
			for(i=0;i<SFS_DENTRYPERBLOCK;i++) currDir[i].sfd_ino = SFS_NOINO;
		}
		else disk_read(currDir,blockNo);

		//***STEP4: Update current directory block (insert newbie entry)
		if((currDir[entryNo].sfd_ino = allocate_block()) == INVALID_BLOCK)
		{
			error_message("mkdir",path,-4);
			return;
		}
		strncpy(currDir[entryNo].sfd_name,path,SFS_NAMELEN);

		//***STEP5: Init newbie Inode
		bzero(&newbieInode,sizeof(struct sfs_inode));	
		newbieInode.sfi_size = 2*sizeof(struct sfs_dir);
		newbieInode.sfi_type = SFS_TYPE_DIR;
		if((newbieInode.sfi_direct[0] = allocate_block()) == INVALID_BLOCK)
		{
			error_message("mkdir",path,-4);
			return;
		}
		
		//***STEP6: Init newbie dir block
		bzero(newDir,SFS_BLOCKSIZE);
		for(i=0; i<SFS_DENTRYPERBLOCK; i++) newDir[i].sfd_ino = SFS_NOINO;
		newDir[0].sfd_ino = currDir[entryNo].sfd_ino;
		strncpy(newDir[0].sfd_name,".",SFS_NAMELEN);
		newDir[1].sfd_ino = sd_cwd.sfd_ino;
		strncpy(newDir[1].sfd_name,"..",SFS_NAMELEN);
		
		//***STEP7: Succesfully directory created -> write to disk
		disk_write(&currDirInode,sd_cwd.sfd_ino);
		disk_write(currDir,blockNo);
		disk_write(&newbieInode,currDir[entryNo].sfd_ino);
		disk_write(newDir,newbieInode.sfi_direct[0]);	
	}
	else error_message("mkdir",path,-3);
}


void sfs_rmdir(const char* path) 
{
	int i,j;
	struct sfs_inode currDirInode, targetInode;
	struct sfs_dir currDir[SFS_DENTRYPERBLOCK];
	
	if(!strcmp(path,".")) { error_message("rmdir",path,-8); return; }

	disk_read(&currDirInode, sd_cwd.sfd_ino);
	
	for(i=0;i<SFS_NDIRECT;i++)
	{
		if(currDirInode.sfi_direct[i] == INVALID_BLOCK) break;		
		disk_read(currDir,currDirInode.sfi_direct[i]);
		for(j=0;j<SFS_DENTRYPERBLOCK;j++)
		{
			if(currDir[j].sfd_ino != SFS_NOINO && !strcmp(path,currDir[j].sfd_name))
			{
				disk_read(&targetInode, currDir[j].sfd_ino);
	
				//this is file
				if(targetInode.sfi_type == SFS_TYPE_FILE)
				{
					error_message("rmdir",path,-2);
					return;
				}
				
				//not empty directory
				if(targetInode.sfi_size > 2*sizeof(struct sfs_dir))
				{
					error_message("rmdir",path,-7);
					return;
				}
				
				//withdraw disk block (target dirBlock)
				release_block(targetInode.sfi_direct[0]);
				
				//withdraw disk block (target inode)
				release_block(currDir[j].sfd_ino);

				//update dir entry
				currDir[j].sfd_ino = SFS_NOINO;
				
				//update dir inode
				currDirInode.sfi_size -= sizeof(struct sfs_dir);

				//write to disk
				disk_write(currDir,currDirInode.sfi_direct[i]);
				disk_write(&currDirInode,sd_cwd.sfd_ino);	
				return;
			}
		}	
	}	
	error_message("rmdir",path,-1);
}


void sfs_rm(const char* path)
{
	int i,j,k;
	struct sfs_inode currDirInode, targetInode;
	struct sfs_dir currDir[SFS_DENTRYPERBLOCK];
	u_int32_t directPtrBlock[NENTRY_DPBLOCK];
	
	disk_read(&currDirInode, sd_cwd.sfd_ino);
	
	for(i=0;i<SFS_NDIRECT;i++)
	{
		if(currDirInode.sfi_direct[i] == INVALID_BLOCK) break;
		
		disk_read(currDir,currDirInode.sfi_direct[i]);
		for(j=0;j<SFS_DENTRYPERBLOCK;j++)
		{
			if(currDir[j].sfd_ino != SFS_NOINO && !strcmp(path,currDir[j].sfd_name))
			{
				disk_read(&targetInode, currDir[j].sfd_ino);
				
				//this is directory
				if(targetInode.sfi_type == SFS_TYPE_DIR)
				{
					error_message("rm",path,-9);
					return;
				}
				
				//withdraw data block indexed by direct pointer
				for(k=0;k<SFS_NDIRECT;k++)
				{
					if(targetInode.sfi_direct[k] != INVALID_BLOCK)
						release_block(targetInode.sfi_direct[k]);
					else break;
				}
				
				//withdraw data block indexed by indirect pointer
				if(targetInode.sfi_indirect != INVALID_BLOCK)
				{
					disk_read(directPtrBlock,targetInode.sfi_indirect);
					for(k=0;k<NENTRY_DPBLOCK;k++)
					{
						if(directPtrBlock[k] != INVALID_BLOCK)
							release_block(directPtrBlock[k]);
						else break;
					}	
				}


				//withdraw direct pointer block
				if(targetInode.sfi_indirect != INVALID_BLOCK)
					release_block(targetInode.sfi_indirect);

				//withdraw disk block (target inode)
				release_block(currDir[j].sfd_ino);
				
				//update dir entry
				currDir[j].sfd_ino = SFS_NOINO;
				
				//update dir inode
				currDirInode.sfi_size -= sizeof(struct sfs_dir);

				//write to disk
				disk_write(currDir,currDirInode.sfi_direct[i]);
				disk_write(&currDirInode,sd_cwd.sfd_ino);
				
				return;				
			}
		}
	}
	error_message("rm",path,-1);
}
 
void sfs_mv(const char* src_name, const char* dst_name) 
{
	int i=0,j=0;
	int foundFlag=0,found_entryNo,found_blockNo;
	int identicFlag=0;
	struct sfs_inode currDirInode;
	struct sfs_dir currDir[SFS_DENTRYPERBLOCK];

	if(!strcmp(src_name,".") || !strcmp(src_name,"..")) { error_message("mv",src_name,-8); return; }
	if(!strcmp(dst_name,".") || !strcmp(dst_name,"..")) { error_message("mv",dst_name,-8); return; }

	disk_read(&currDirInode,sd_cwd.sfd_ino);

	for(i=0;i<SFS_NDIRECT;i++)
	{
		if(currDirInode.sfi_direct[i] == INVALID_BLOCK) break; 
	
		disk_read(currDir,currDirInode.sfi_direct[i]);
		for(j=0;j<SFS_DENTRYPERBLOCK;j++)
		{
			if(currDir[j].sfd_ino != SFS_NOINO && !strcmp(src_name,currDir[j].sfd_name))
			{
				foundFlag = 1;
				found_entryNo = j;
				found_blockNo = currDirInode.sfi_direct[i];
			}
		
			if(currDir[j].sfd_ino != SFS_NOINO && !strcmp(dst_name,currDir[j].sfd_name))
				identicFlag = 1;
		}
	}

	if(foundFlag == 0) error_message("mv",src_name,-1);
	else if(foundFlag == 1 && identicFlag == 1) error_message("mv",dst_name,-6);
	else
	{
		disk_read(currDir,found_blockNo);
		strncpy(currDir[found_entryNo].sfd_name, dst_name, strlen(dst_name)+1);
		disk_write(currDir,found_blockNo);	
	}
}

void sfs_cpin(const char* local_path, const char* path) 
{
	int i, n;
	FILE *fp = fopen(path, "rb");
	unsigned int fileSize;
	int found = 0, directCnt=0, indirectCnt=0;
	unsigned int directPtrBlockNo, dataBlockNo;
	struct sfs_inode newbieInode;
	unsigned char buf[SFS_BLOCKSIZE];
	unsigned int directPtrBlock[NENTRY_DPBLOCK];

	//***STEP1: file processing
	//file existence check
	if(fp == NULL) { error_message("cpin",path,-11); return; }

	//path file size check
	fseek(fp,0,SEEK_END);
	fileSize = ftell(fp);
	rewind(fp);	
	if(fileSize > MAX_FILESIZE) { error_message("cpin",path,-12); fclose(fp); return; }

	//***STEP2: make empty file
	flag4touch2cpin = 0;
	sfs_touch(local_path);
	if(flag4touch2cpin == 1) return;
	disk_read(&newbieInode,fileInodeBlockNo);
	
	//***STEP3: update newbie inode
	while(1)
	{
		n=fread(buf,SFS_BLOCKSIZE,1,fp);

		if(directCnt < SFS_NDIRECT)
		{
			if((dataBlockNo = allocate_block()) == INVALID_BLOCK)
			{
				disk_write(&newbieInode,fileInodeBlockNo);	
				error_message("cpin",local_path,-4);
				fclose(fp);
				return;	
			}
			if(fileSize < SFS_BLOCKSIZE) newbieInode.sfi_size += fileSize;
			else newbieInode.sfi_size += SFS_BLOCKSIZE;
			newbieInode.sfi_direct[directCnt++] = dataBlockNo;
			disk_write(buf,dataBlockNo);
			fileSize -= SFS_BLOCKSIZE;
		}
		else
		{
			if(indirectCnt == 0)
			{			
				if((directPtrBlockNo = allocate_block()) == INVALID_BLOCK)
				{
					disk_write(&newbieInode,fileInodeBlockNo);
					error_message("cpin",local_path,-4);
					fclose(fp);
					return;
				}
				bzero(directPtrBlock,SFS_BLOCKSIZE);
				newbieInode.sfi_indirect = directPtrBlockNo;
			}

			if((dataBlockNo = allocate_block()) == INVALID_BLOCK)
			{
				disk_write(directPtrBlock,directPtrBlockNo);
				disk_write(&newbieInode,fileInodeBlockNo);
				error_message("cpin",local_path,-4);
				fclose(fp);
				return;
			}
			if(fileSize < SFS_BLOCKSIZE) newbieInode.sfi_size += fileSize;
			else newbieInode.sfi_size += SFS_BLOCKSIZE;
			directPtrBlock[indirectCnt++] = dataBlockNo;
			disk_write(buf,dataBlockNo);
			fileSize -= SFS_BLOCKSIZE;
		}
		
		if(n==0) break;
	}	
	if(feof(fp) == 0) { error_message("cpin",path,-13); fclose(fp); return; }

	//***STEP4: write to disk	
	disk_write(directPtrBlock,directPtrBlockNo);
	disk_write(&newbieInode, fileInodeBlockNo);
	fclose(fp);
}

void sfs_cpout(const char* local_path, const char* path) 
{
	int i,j,k;
	FILE* fp;
	unsigned int fileSize;
	unsigned char buf[SFS_BLOCKSIZE];
	unsigned int directPtrBlock[NENTRY_DPBLOCK];
	struct sfs_dir currDir[SFS_DENTRYPERBLOCK];
	struct sfs_inode fileInode, currDirInode;

	disk_read(&currDirInode,sd_cwd.sfd_ino);
	
	for(i=0;i<SFS_NDIRECT;i++)
	{
		if(currDirInode.sfi_direct[i] == INVALID_BLOCK) break;
		
		disk_read(currDir,currDirInode.sfi_direct[i]);
		for(j=0;j<SFS_DENTRYPERBLOCK;j++)
		{
			if(currDir[j].sfd_ino != SFS_NOINO && !strcmp(local_path,currDir[j].sfd_name))
			{
				fp = fopen(path,"wb");
				disk_read(&fileInode, currDir[j].sfd_ino);
				fileSize = fileInode.sfi_size;
				if(fileInode.sfi_type == SFS_TYPE_DIR)
				{
					error_message("cpout",local_path,-1);
					return;
				}
			
				for(k=0;k<SFS_NDIRECT;k++)
				{
					if(fileInode.sfi_direct[k] != INVALID_BLOCK)
					{
						disk_read(buf,fileInode.sfi_direct[k]);
						if(fileSize < SFS_BLOCKSIZE) fwrite(buf,fileSize,1,fp);
						else fwrite(buf,SFS_BLOCKSIZE,1,fp);
						fileSize-=SFS_BLOCKSIZE;	
					}
					else break;
				}
				
				if(fileInode.sfi_indirect != INVALID_BLOCK)
				{
					disk_read(directPtrBlock,fileInode.sfi_indirect);
					for(k=0;k<NENTRY_DPBLOCK;k++)
					{
						if(directPtrBlock[k] != INVALID_BLOCK)
						{
							disk_read(buf,directPtrBlock[k]);
							if(fileSize < SFS_BLOCKSIZE) fwrite(buf,fileSize,1,fp);
							else fwrite(buf,SFS_BLOCKSIZE,1,fp);
							fileSize-=SFS_BLOCKSIZE;
						}
						else break;
					}
				}
				return;
			}
		}
	}
	error_message("cp_out",local_path,-1);
}

void dump_inode(struct sfs_inode inode)
{
	int i;
	struct sfs_dir dir_entry[SFS_DENTRYPERBLOCK];

	printf("size %d type %d direct ", inode.sfi_size, inode.sfi_type);
	for(i=0; i < SFS_NDIRECT; i++) printf(" %d ", inode.sfi_direct[i]);
	
	printf(" indirect %d",inode.sfi_indirect);
	printf("\n");

	if (inode.sfi_type == SFS_TYPE_DIR)
	{
		for(i=0; i < SFS_NDIRECT; i++)
		{
			if (inode.sfi_direct[i] == 0) break;
			disk_read(dir_entry, inode.sfi_direct[i]);
			dump_directory(dir_entry);
		}
	}
}

void dump_directory(struct sfs_dir * dir_entry)
{
	int i;
	struct sfs_inode inode;
	for(i=0; i < SFS_DENTRYPERBLOCK;i++)
	{
		printf("%d %s\n",dir_entry[i].sfd_ino, dir_entry[i].sfd_name);
		
		disk_read(&inode,dir_entry[i].sfd_ino);
		if (inode.sfi_type == SFS_TYPE_FILE)
		{
			printf("\t");
			dump_inode(inode);
		}
	}
}

void sfs_dump()
{
	// dump the current directory structure
	struct sfs_inode c_inode;

	disk_read(&c_inode, sd_cwd.sfd_ino);
	printf("cwd inode %d name %s\n",sd_cwd.sfd_ino,sd_cwd.sfd_name);
	dump_inode(c_inode);
	printf("\n");

}
