#include <iostream>  
#include <string>  
#include <stdlib.h>  
#include <time.h>  
#include <stdio.h>  
#include <string.h>  
#include <io.h>  
using namespace std;  
  
const int memory_size = 2000000;//每次10万条数据

int remove_files(char (*files)[30],int count)
{
	for(int i=0;i<count;i++)
	{
		if (!strcmp(files[i],"data.txt\n"))
			continue;
		char del_cmd[50]={0};
		sprintf(del_cmd,"del %s",files[i]);
		system(del_cmd);
	}
	return 0;
}
//int listFodler(char* file,char (*files)[30])
int listFodler(char (*files)[30])
{
	//strcat(file,"\\data*.txt");
	char *file=".\\data*.txt";
	long handle;
	struct _finddata_t fileinfo;
	handle=_findfirst(file,&fileinfo);
	if(-1==handle)return -1;
	int i=0;
	sprintf(files[i++],"%s\n",fileinfo.name);                             
	while(!_findnext(handle,&fileinfo))
	{
		if(strcmp(fileinfo.name,".")==0||strcmp(fileinfo.name,"..")==0||fileinfo.attrib==_A_SUBDIR)
		{
			continue;                                                       
		}
		sprintf(files[i++],"%s\n",fileinfo.name);                             
	}
	_findclose(handle);
	return i;
}

int read_data(FILE *fp, char **space)  
{  
    int index = 0;  
	char s_tmp[30];
    while (index < memory_size && fscanf(fp, "%s\n", s_tmp) != EOF)  
	{
		space[index]=new char[sizeof(s_tmp)+1];
		strcpy(space[index],s_tmp);
        index++;  
	}
    return index;  
}  
  
void write_data(FILE *fp, char **space, int num)  
{  
    int index = 0;  
    while (index < num)  
    {  
        fprintf(fp, "%s\n", space[index]);  
        index++;  
    }  
}  
  
void check_fp(FILE *fp)  
{  
    if (fp == NULL)  
    {  
        cout << "The file pointer is invalid!" << endl;  
        exit(1);  
    }  
}  
  
int compare(const void *first_num, const void *second_num)  
{  
	return strcmp(*(char **)first_num,*(char **)second_num);
}  
  
string new_file_name(int n)  
{  
    char file_name[20];  
    sprintf(file_name, "data%d.txt", n);  
    return file_name;  
}  
  
int memory_sort(const char *filename)  
{  
    FILE *fp_in_file = fopen(filename, "r");  
    check_fp(fp_in_file);  
    int counter = 0;  
    while (true)  
    {  
        char *space[memory_size]={NULL};
        int num = read_data(fp_in_file, space);  
        if (num == 0)  
            break;  

  
        qsort(space, num, sizeof(char *), compare);  

        string file_name = new_file_name(++counter);  
        FILE *fp_aux_file = fopen(file_name.c_str(), "w");  
        check_fp(fp_aux_file);  
  
        write_data(fp_aux_file, space, num);  
        fclose(fp_aux_file);  

		for(int i=0;i<num;i++)
			if (space[i] != NULL)
			{
				delete []space[i];
				space[i]=NULL;
			}

    }  
    fclose(fp_in_file);  
  
    return counter;  
}  
  
void merge_sort(int file_num,const char *filename)  
{  
    if (file_num <= 0)  
        return;  
    FILE *fp_out_file = fopen(filename, "w");  
    check_fp(fp_out_file);  
  
    FILE **fp_array = new FILE *[file_num];  
    int i;  
    for (i = 0; i < file_num; i++)  
    {  
        string file_name = new_file_name(i + 1);  
        fp_array[i] = fopen(file_name.c_str(), "r");  
        check_fp(fp_array[i]);  
    }  
  
	char first_data[1000][30];
    bool *finish = new bool[file_num];  
    memset(finish, false, sizeof(bool) * file_num);  
  
    for (i = 0; i < file_num; i++)  
        fscanf(fp_array[i], "%s\n", first_data[i]);  
    while (true)  
    {  
        int index = 0;  
        while (index < file_num && finish[index])  
            index++;  
  
        if (index >= file_num)  
            break;  
  
        char min_data[30];
		strcpy(min_data,first_data[index]);
        for (i = index + 1; i < file_num; i++)  
        {  
			
			int f=strcmp(min_data,first_data[i]);
			bool swap_f=false;
			if (f>0)
				swap_f=true;

            if (swap_f&&(!finish[i]))     
            {  
				strcpy(min_data,first_data[i]);
				index=i;
            }  
        }  
  
        fprintf(fp_out_file, "%s\n", min_data);  
        if (fscanf(fp_array[index], "%s\n", first_data[index]) == EOF)  
            finish[index] = true;  
    }  
  
    fclose(fp_out_file);  
    delete []finish;  
    for (i = 0; i < file_num; i++)  
        fclose(fp_array[i]);  
    delete [] fp_array;  
}  
  
int check_file()  
{  
    FILE *fp_in_file = fopen("sortedfile.txt", "r");  
    check_fp(fp_in_file);  
    FILE *fp_out_file = fopen("result.txt", "w");  
    check_fp(fp_out_file);  

	char *space[memory_size]={NULL};
	int num = read_data(fp_in_file, space);  
	
	char refd[30]={0};
	char moved[30];
	int count=1;
	int recyle=1;
	strcpy(refd,space[0]);
	int f=0;
	while(true)
	{
		if (recyle<num)
		{
			memset(moved,0,sizeof(moved));
			strcpy(moved,space[recyle]);
		}
		else{
			recyle=0;
			for(int i=0;i<num;i++)
				if (space[i] != NULL)
				{
					delete []space[i];
					space[i]=NULL;
				}
			num = read_data(fp_in_file, space);  
			if (num == 0)
				break;  
			else{
				memset(moved,0,sizeof(moved));
				strcpy(moved,space[recyle]);
			}
		}
		recyle++;
		if(!strcmp(refd,moved))
		{
			count++;
			f=1;
		}else{
			//printf("%s	%d\n",refd,count);
			fprintf(fp_out_file,"%d-%s\n",count,refd);
        	//fprintf(fp_out_file, "%s\n", min_data);  
			count=1;
			memset(refd,0,sizeof(refd));
			strcpy(refd,moved);
			f=0;
		}
		
	}
	if(f)
		fprintf(fp_out_file,"%d-%s\n",count,refd);
	else
		fprintf(fp_out_file,"%d-%s\n",1,moved);

    fclose(fp_in_file);  
    fclose(fp_out_file);  
  
    return 0;  
}  
int main()  
{  
	
	cout<<"1"<<endl;
    int aux_file_num = memory_sort("data.txt");  
	cout<<"2"<<endl;
    merge_sort(aux_file_num,"sortedfile.txt");  
	cout<<"3"<<endl;
	char files[1000][30]={0};
	int f_count=listFodler(files);
	remove_files(files,f_count);
	cout<<"4"<<endl;
	
	check_file();

    aux_file_num = memory_sort("result.txt");  
	cout<<"5"<<endl;
    merge_sort(aux_file_num,"SortResult.txt");  
	cout<<"6"<<endl;
	memset(files,0,sizeof(files));
	f_count=listFodler(files);
	remove_files(files,f_count);
	cout<<"7"<<endl;
	system("pause");

    
    return 0;  
}  

