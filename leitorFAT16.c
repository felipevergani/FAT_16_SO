#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct fat_BS
{
	unsigned char 		bootjmp[3]; 
	unsigned char 		oem_name[8];
	unsigned short  	bytes_per_sector;
	unsigned char		sectors_per_cluster;
	unsigned short 	    reserved_sector_count;
	unsigned char		table_count; //quant. de FAT
	unsigned short  	root_entry_count;
	unsigned short  	total_sectors_per_fat;
    unsigned int        total_sectors_16;
	unsigned int      table_size_16; //(int)(BS.total_sectors_per_fat * BS.reserved_sector_count) TAMANHO DA FAT
	unsigned short      total_sector;
    unsigned short      init_fat;
    unsigned int        init_root_dir;
    unsigned int        init_data;
	
 
 
}__attribute__((packed)) fat_BS_t;

typedef struct format8_3
{
    char            name[8];
    char            ext[3];
    unsigned short  type;
    unsigned int    first_cluster;
    unsigned long   size;
    char            nameExt;

}__attribute__((packed)) f8_3;


int main(int argc, char *argv[]){
    FILE *arquivo;
    char *nome_arquivo = argv[1];

    arquivo = fopen(nome_arquivo, "rb");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    fat_BS_t BS;

    fseek(arquivo, 11, SEEK_SET);
    fread(&BS.bytes_per_sector, 2, 1, arquivo);

    fseek(arquivo, 13, SEEK_SET);
    fread(&BS.sectors_per_cluster, 1, 1, arquivo);

    fseek(arquivo, 14, SEEK_SET);
    fread(&BS.reserved_sector_count, 2, 1, arquivo);
    
    fseek(arquivo, 16, SEEK_SET);
    fread(&BS.table_count, 1, 1, arquivo);

    fseek(arquivo, 17, SEEK_SET);
    fread(&BS.root_entry_count, 2, 1, arquivo);

    fseek(arquivo, 19, SEEK_SET);
    fread(&BS.total_sector, 2, 1, arquivo);

    fseek(arquivo, 22, SEEK_SET);
    fread(&BS.total_sectors_per_fat, 2, 1, arquivo);



    BS.reserved_sector_count *= BS.bytes_per_sector;
    BS.init_fat = (BS.sectors_per_cluster);
    BS.table_size_16 = (int)(BS.total_sectors_per_fat * BS.reserved_sector_count);
    BS.init_root_dir = (BS.reserved_sector_count + (BS.table_count * BS.table_size_16));
    BS.init_data = (BS.init_root_dir + (BS.bytes_per_sector * 32));



    //DEBUG
    /*
    printf("Bytes per sector\t = %d \n", BS.bytes_per_sector);
    printf("Sector per cluster\t = %x \n", BS.sectors_per_cluster);
    printf("Reserved Sector Count\t = %d \n", BS.reserved_sector_count);
    printf("Table Count\t\t = %x \n", BS.table_count);
    printf("Root Entry Count\t = %d \n", BS.root_entry_count);
    printf("Total Sector\t\t = %d \n", BS.total_sector);
    printf("Total Sector per FAT\t = %d \n", BS.total_sectors_per_fat);
    printf("Init of Fat\t\t = %d \n", BS.init_fat);
    printf("Size of Fat\t\t = %d\n", BS.table_size_16);
    printf("Init ROOT dir\t\t = %d \n", BS.init_root_dir);
    printf("Init DATA\t\t = %d \n", BS.init_data);
    */
    fclose(arquivo);
    return 0;

}