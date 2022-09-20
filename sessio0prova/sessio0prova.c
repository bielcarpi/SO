//Llegir fitxer series (netflix_bin.show) -> NombreSerie#Género#Subgénero#Año#Duración_media#N_temporadas#N_episodios#Valoración\n
//Llegir fitxer ratings de series (ratings.dat) -> Nombre&Apellido&Nombre serie&Valoración\n
//Mostrar totes les series. Despres, mostrar la serie amb mes valoracio i els seus ratings

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

#define READING_SERIES "Reading Series...\n"
#define READING_RATINGS "Reading Ratings...\n"

typedef struct Serie{
    char *nom_serie;
    char *genere;
    char *subgenere;
    uint16_t any;
    uint16_t duracio;
    uint8_t temporades;
    uint8_t episodis;
    uint8_t valoracio;
}Serie;

typedef struct Rating{
    char *nom;
    char *nom_serie;
    int valoracio;
}Rating;

int main(){
    Serie *series;
    int num_series = 0;
    //Rating *ratings;
    //int num_ratings = 0;

    write(STDOUT_FILENO, READING_SERIES, strlen(READING_SERIES));
    int fd_series = open("netflix_bin.show", O_RDONLY|O_CREAT);
    if(fd_series < 0) //If we aren't given a file descriptor, exit the program
        return 1;

    Serie serieBuffer;
    char *buffer = (char *)malloc(sizeof(char)); //Buffer for saving text between #
    char nextChar;
    int field = 0;
    int count = 0;
    while(read(fd_series, &nextChar, sizeof(char)) != 0){ //While we have a char in the file, read it
        if(nextChar == '#'){
            buffer[count] = '\0'; //We just end that word, so put the \0 at the end
            printf("%s %d\n", buffer, field);
            switch(field){
                case 0:
                    serieBuffer.nom_serie = buffer;
                    field++;
                break;
                case 1:
                    serieBuffer.genere = buffer;
                    field++;
                break;
                case 2:
                    serieBuffer.subgenere = buffer;

                    //wtf
                    lseek(fd_series, 2, SEEK_CUR); //The file is not well formatted, year occupies 4 bytes (see in hex editor)
                    //wtf
                    read(fd_series, &(serieBuffer.any), 2); //Reading & saving year (2 bytes)
                    printf("%d\n", serieBuffer.any);
                    lseek(fd_series, sizeof(char), SEEK_CUR); //Move 1 char forward in the file (skip the next #)
                    read(fd_series, &(serieBuffer.duracio), 2); //Reading & saving duracio (2 bytes)
                    printf("%d\n", serieBuffer.duracio);
                    lseek(fd_series, sizeof(char), SEEK_CUR); //Move 1 char forward in the file (skip the next #)
                    read(fd_series, &(serieBuffer.temporades), 1); //Reading & saving temporades (1 byte)
                    printf("%d\n", serieBuffer.temporades);
                    lseek(fd_series, sizeof(char), SEEK_CUR); //Move 1 char forward in the file (skip the next #)
                    read(fd_series, &(serieBuffer.episodis), 1); //Reading & saving episodis (1 byte)
                    printf("%d\n", serieBuffer.episodis);
                    lseek(fd_series, sizeof(char), SEEK_CUR); //Move 1 char forward in the file (skip the next #)
                    read(fd_series, &(serieBuffer.valoracio), 1); //Reading & saving valoracio (1 byte)
                    printf("%d\n", serieBuffer.valoracio);
                    lseek(fd_series, sizeof(char), SEEK_CUR); //Move 1 char forward in the file (skip the \n)

                    //Save the new Serie to the Series array
                    if(num_series == 0)
                        series = (Serie *)malloc(sizeof(Serie));
                    else
                        series = (Serie *)realloc(series, sizeof(Serie) * (num_series+1));

                    series[num_series] = serieBuffer; //Copy the value of serieBuffer to series[num_series - 1] (the VALUE!!)
                    num_series++;

                    field = 0;
                break;
            }

            buffer = (char *)malloc(sizeof(char)); //We now want to point buffer to another char array on the heap, as the last 'buffer' is a variable of a serie. We allocate 2 chars
            count = 0;
        }
        else{
            buffer[count] = nextChar;
            count++;
            buffer = (char *)realloc(buffer, sizeof(char) * count); //Make buffer 1 char bigger (either for the next char, or \0)
        }
    }
    close(fd_series);

    //Now that we have all the Series, let's display them
    /*
    for(int i = 0; i < num_series; i++){
        char *temp;
        asprintf(&temp, "Serie %d: %s %s %s %d %d %d %d %d\n", num_series+1, series[num_series].nom_serie, series[num_series].genere, series[num_series].subgenere, series[num_series].any, series[num_series].duracio, series[num_series].temporades, series[num_series].episodis, series[num_series].valoracio);
        write(STDOUT_FILENO, temp, strlen(temp));
        free(temp);
    }

    write(STDOUT_FILENO, READING_RATINGS, strlen(READING_RATINGS));
    int fd_ratings = open("netflix_bin.show", O_RDONLY|O_CREAT);
    while(1){

    }
    close(fd_ratings);
    */

    return 0;
}