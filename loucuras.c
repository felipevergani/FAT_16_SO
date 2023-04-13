#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[8];
    char ext[3];
    char attributes;
    char reserved[10];
    unsigned short time;
    unsigned short date;
    unsigned short start_cluster;
    unsigned int size;
} __attribute__((packed)) dir_entry_t;

void ls_fat16(char* image_path) {
    FILE* fp = fopen(image_path, "rb");
    if (!fp) {
        printf("Erro ao abrir imagem\n");
        return;
    }

    fseek(fp, 11, SEEK_SET);
    unsigned short bytes_per_sector;
    fread(&bytes_per_sector, sizeof(bytes_per_sector), 1, fp);

    fseek(fp, 13, SEEK_SET);
    unsigned char sectors_per_cluster;
    fread(&sectors_per_cluster, sizeof(sectors_per_cluster), 1, fp);

    fseek(fp, 14, SEEK_SET);
    unsigned short reserved_sectors;
    fread(&reserved_sectors, sizeof(reserved_sectors), 1, fp);

    fseek(fp, 16, SEEK_SET);
    unsigned char fat_count;
    fread(&fat_count, sizeof(fat_count), 1, fp);

    fseek(fp, 22, SEEK_SET);
    unsigned short root_entries;
    fread(&root_entries, sizeof(root_entries), 1, fp);

    unsigned int root_dir_sectors = ((root_entries * 32) + (bytes_per_sector - 1)) / bytes_per_sector;
    unsigned int first_data_sector = reserved_sectors + (fat_count * 32) + root_dir_sectors;
    unsigned int cluster_size = bytes_per_sector * sectors_per_cluster;
    unsigned int first_root_dir_sector = reserved_sectors + (fat_count * 32);

    fseek(fp, first_root_dir_sector * bytes_per_sector, SEEK_SET);

    for (int i = 0; i < root_entries; i++) {
        dir_entry_t entry;
        fread(&entry, sizeof(entry), 1, fp);

        if (entry.name[0] == 0x00) {
            break;
        }

        if (entry.name[0] == 0xE5) {
            continue;
        }

        if (entry.attributes & 0x10) {
            printf("%s\t<DIR>\n", entry.name);
        } else {
            printf("%s.%s\t%d bytes\n", entry.name, entry.ext, entry.size);
        }
    }

    fclose(fp);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: ls_fat16 <imagem>\n");
        return 1;
    }

    ls_fat16(argv[1]);

    return 0;
}
``
