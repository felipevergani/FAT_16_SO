#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

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
	unsigned int        table_size_16; //(int)(BS.total_sectors_per_fat * BS.bytes_per_sector) TAMANHO DA FAT
	unsigned short      total_sector;
    unsigned short      init_fat;
    unsigned int        init_root_dir;
    unsigned int        init_data;
	
 
 
}__attribute__((packed)) fat_BS_t;

typedef struct format8_3
{
    unsigned char name[9];  //0
    unsigned char ext[4];   //8 0x08
    unsigned char type;     //11 0x0B
    unsigned short first_cluster; //26 0x1A
    unsigned int size;     // 28 0x1C

    
}f8_3;


int fat16(char* image){
    FILE *arquivo;
    arquivo = fopen(image, "rb");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 0;
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
    BS.table_size_16 = (int)(BS.total_sectors_per_fat * BS.bytes_per_sector);
    BS.init_root_dir = (BS.reserved_sector_count + (BS.table_count * BS.table_size_16));
    BS.init_data = (BS.init_root_dir + (BS.bytes_per_sector * 32));
    unsigned int initROOTdir = BS.init_root_dir;
    unsigned int initDATA = BS.init_data;

    f8_3 aux;
    unsigned char firstvalue; 
    int helper = 0;

    fseek(arquivo, initROOTdir, SEEK_SET);
    
    for(int i = initROOTdir; i < initDATA; i+=32){
    
        fseek(arquivo, i, SEEK_SET);
        fread(&firstvalue, 1, 1, arquivo);
        fseek(arquivo, i+11, SEEK_SET);
        fread(&aux.type, 1, 1, arquivo);
        
        
            if(firstvalue == 0x00){
                break;
            } else if(firstvalue != 0xE5 && aux.type != 0x0F){
            helper++;
            }
    }
    //printf("%d", helper);
    f8_3 entry[helper+1];
    
    fseek(arquivo, initROOTdir, SEEK_SET);
    
    for(int i = initROOTdir, k = 0; i < initDATA; i+=32){

        fseek(arquivo, i, SEEK_SET);
        fread(&firstvalue, 1, 1, arquivo);
        fseek(arquivo, i+11, SEEK_SET);
        fread(&entry[k].type, 1, 1, arquivo);
        
        
            if(firstvalue == 0x00){
                break;
            } else if(firstvalue != 0xE5 && entry[k].type != 0x0F){
                fseek(arquivo, i, SEEK_SET);
                fread(&entry[k].name, 8, 1, arquivo);
                
                fseek(arquivo, i+8, SEEK_SET);
                fread(&entry[k].ext, 3, 1, arquivo);
                   
                fseek(arquivo, i+11, SEEK_SET);
                fread(&entry[k].type, 1, 1, arquivo);
                fseek(arquivo, i+26, SEEK_SET);
                fread(&entry[k].first_cluster, 2, 1, arquivo);
                fseek(arquivo, i+28, SEEK_SET);
                fread(&entry[k].size, 4, 1, arquivo);
                k++;
                
            } 
    }
    //printf("%d", helper);
    for(int i = 0; i<helper; i++){
        printf("NAME:");
        for(int j = 0; j<8; j++){
            if(entry[i].name[j] != ' ')
            printf("%c", entry[i].name[j]);
        }puts("");
        printf("EXT:");
        for(int l = 0; l<3; l++){
            if(entry[i].ext[l] != ' ')
            printf("%c", entry[i].ext[l]);
        }puts("");
        printf("Type: %x\nFirstCluster: %d\nSize: %u\n", entry[i].type, entry[i].first_cluster, entry[i].size);
        puts("");
        
    }


    char receive[18];
    char totalname[18];
    fgets(receive, 18, stdin);
    char aux2[8];
    char aux3[14];
    

    for(int i = 0; i < (sizeof(receive)-4); i++){
        aux3[i] = receive[i+4]; 
    }
    


    for(int i = 0; i < helper; i++){
        int K = 0;
        strcat(totalname, entry[i].name);
        //strcat(totalname, ".");
        //strcat(totalname, entry[i].ext);
        printf("%s", totalname);
        /*
       for(int j = 0; j<8; j++){
            if(entry[i].name[j] != ' '){
                totalname[j] = entry[i].name[j]; 
                //printf("%c\n",totalname[j]);

                K++;
            }
        }
        totalname[K+1] = ".";
        
        for(int l = 0; l<3; l++){
            if(entry[i].ext[l] != ' '){
                totalname[l+K+2] = entry[i].ext[l];
                //printf("%c\n",totalname[i]);
            }

        }
        if(strncmp(aux3, totalname, sizeof(totalname)) == 0){
            printf("%s", totalname);
        }
        totalname[0] = '\0';
        //printf("FINAL");
    
        */
    }
    
    //printf("%s", receive);
    //if (strncmp(entry.filename, "filename", 8) == 0 && strncmp(entry.ext, "ext", 3) == 0) 
    //if(strncmp(entry[i].name, receive, sizeof(entry[i].name)))

    
   






    
    /*
    //DEBUG
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

    
}

int main(int argc, char *argv[]){
    
    fat16(argv[1]);

    return 0;

}