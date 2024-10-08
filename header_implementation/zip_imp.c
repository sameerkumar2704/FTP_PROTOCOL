#include <zip.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
void zip_files(char *destination, char *source_file, char *file_name)
{
    int err = 0;
    zip_t *zip = zip_open(destination, ZIP_CREATE, &err);
    zip_source_t *source = zip_source_file(zip, source_file, 0, 0);
    zip_file_add(zip, file_name, source, ZIP_FL_OVERWRITE);
    zip_close(zip);
}

void unzip_file(char *dir, char *file_path)
{
    int errorp = 0;
    mkdir(dir, 0775);
    char path[1024];

    zip_t *arch = zip_open(file_path, 0, &errorp);
    struct zip_stat *finfo = calloc(256, sizeof(int));
    zip_stat_init(finfo);
    zip_file_t *fd = NULL;
    int count = 0;
    char *txt = NULL;
    FILE *file = NULL;
    while (1)
    {

        int bytes_len = zip_stat_index(arch, count, 0, finfo);
        if (bytes_len != 0)
            break;
        txt = calloc(finfo->size + 1, sizeof(char));
        fd = zip_fopen_index(arch, count, 0);
        size_t size = zip_fread(fd, txt, finfo->size);
        snprintf(path, sizeof(path), "%s/%s", dir, finfo->name);
        file = fopen(path, "wb");
        fwrite(txt, sizeof(char), size, file);
        fclose(file);
        count++;

        free(txt);
    }
    zip_close(arch);
}