#include <stdio.h>
#include <stdlib.h>

int main()
{

    FILE *fp;
    short int bytes_per_sector;
    unsigned char sector_per_cluster;
    fp= fopen("fat16_1sectorpercluster.img", "rb");
    fseek(fp, 11, SEEK_SET);
    fread(&bytes_per_sector,2,1, fp);

    fseek(fp, 13, SEEK_SET);
    fread(&sector_per_cluster, 1,1,fp);
    /****ler os demais campos do boot record*****/

    printf("Bytes per sector %hd \n", bytes_per_sector);
    printf("Sector per cluster %x \n", sector_per_cluster);
   

    return 0;
}
