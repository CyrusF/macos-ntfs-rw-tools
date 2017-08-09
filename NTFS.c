#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define buf_size 4096
#define local_position "~/NTFS/"

void shell(const char* cmd,char* result)   
{   
	char buf_ps[buf_size];   
	char ps[buf_size]={0};   
	FILE *ptr;   
	strcpy(ps,cmd);   
	if((ptr=popen(ps,"r"))!=NULL)   
	{   
		while(fgets(buf_ps,buf_size,ptr)!=NULL)   
		{   
		   strcat(result,buf_ps);   
		   if(strlen(result)>buf_size)   
			   break;   
		}   
		pclose(ptr);   
		ptr = NULL;   
	}   
	else  
	{   
		printf("popen %s error\n", ps);   
	}   
}  

void mountNTFS(const char* raw)
{
	char substr[]=" \n";
	char *slice=NULL;
	char slice_res[512][512];
	int n=0;
	slice=strtok(raw,substr);
	while (slice!=NULL)
	{
		strcpy(slice_res[n],slice);
		n++;
		slice=strtok(NULL,substr);
	}
	char name[128][128],size[128][128],node[128][128];
	int m=0;
	for (int i=0;i<n;i++)
	{
		if (strcmp(slice_res[i],"Windows_NTFS")==0)
		{
			
			strcpy(name[m],slice_res[i+1]);
			strcat(slice_res[i+2],slice_res[i+3]);
			strcpy(size[m],slice_res[i+2]);
			strcpy(node[m],"/dev/");
			strcat(node[m],slice_res[i+4]);
			m++;
		}
	}
	printf("\nFind %d Windows_NTFS disk part:\n\n",m);
	for (int i=0;i<m;i++)
	{
		printf("%d:\n",i);
		printf("  TYPE  %s\n","Windows_NTFS");
		printf("  NAME  %s\n",name[i]);
		printf("  SIZE  %s\n",size[i]);
		printf("  NODE  %s\n",node[i]);
	}
	printf("\nWill run command but need your Mac OS password:\n\n");
	char cmd_mkdir[1024]="sudo mkdir ";
	strcat(cmd_mkdir,local_position);
	system(cmd_mkdir);
	for (int i=0;i<m;i++)
	{
		char cmd_umount[1024]="sudo umount /Volumes/";
		strcat(cmd_umount,name[i]);
		char cmd_mkdir[1024]="sudo mkdir ";
		strcat(cmd_mkdir,local_position);
		strcat(cmd_mkdir,name[i]);
		char cmd_mount[1024]="sudo mount_ntfs -o rw,nobrowse ";
		strcat(cmd_mount,node[i]);
		strcat(cmd_mount," ");
		strcat(cmd_mount,local_position);
		strcat(cmd_mount,name[i]);
		system(cmd_umount);
		system(cmd_mkdir);
		system(cmd_mount);
	}
	printf("\nOperation completed.\nYour NTFS disk parts are mounted at %s\n",local_position);
	printf("Please do unmount BEFORE REMOVE YOUR DEVICE, or will cause data disaster.\n");
}

void umountNTFS(const char* raw)
{
	char substr[]=" \n";
	char *slice=NULL;
	char slice_res[512][512];
	int n=0;
	slice=strtok(raw,substr);
	while (slice!=NULL)
	{
		strcpy(slice_res[n],slice);
		n++;
		slice=strtok(NULL,substr);
	}
	char name[128][128],size[128][128],node[128][128];
	int m=0;
	for (int i=0;i<n;i++)
	{
		if (strcmp(slice_res[i],"Windows_NTFS")==0)
		{
			
			strcpy(name[m],slice_res[i+1]);
			strcat(slice_res[i+2],slice_res[i+3]);
			strcpy(size[m],slice_res[i+2]);
			strcpy(node[m],"/dev/");
			strcat(node[m],slice_res[i+4]);
			m++;
		}
	}
	printf("\nFind %d Windows_NTFS disk part:\n\n",m);
	for (int i=0;i<m;i++)
	{
		printf("%d:\n",i);
		printf("  TYPE  %s\n","Windows_NTFS");
		printf("  NAME  %s\n",name[i]);
		printf("  SIZE  %s\n",size[i]);
		printf("  NODE  %s\n",node[i]);
	}
	printf("\nWill run command but maybe need your Mac OS password.\n\n");
	for (int i=0;i<m;i++)
	{
		char cmd_umount[1024]="sudo umount ";
		strcat(cmd_umount,local_position);
		strcat(cmd_umount,name[i]);
		system(cmd_umount);
	}
	printf("\nOperation completed.\n");
	printf("Your NTFS disk parts are umounted.\n");
	printf("AFTER REMOVE YOUR DEVICE, you can use \" sudo rm -R %s* \" command to remove temp file.\n",local_position);
}

int main(int argc,char *argv[]) 
{
	printf("=====NTFS R+W Tools=====\n");
	printf("Options:\n");
	printf("[0] : mount all NTFS disk part\n");
	printf("[1] : unmount all NTFS disk part\n");
	printf("Your choice : ");
	int op;
	scanf("%d",&op);
	if (op==0)
	{
		char list[]="diskutil list",res_list[buf_size];
		shell(list,res_list);
		mountNTFS(res_list);
	}
	else if (op==1)
	{
		char list[]="diskutil list",res_list[buf_size];
		shell(list,res_list);
		umountNTFS(res_list);
	}
	else
	{
		printf("Wrong option label.\n");
	}
	printf("=====Thanks for use=====\n");
}