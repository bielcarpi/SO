#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 100

typedef struct Serie {
    char nom_serie[MAX_STR];
    char genere[MAX_STR];
    char subgenere[MAX_STR];
    int any;
    int duracio;
    int temporades;
    int eposodis;
    int valoracio;
} Serie;

typedef struct Rating {
    char nom[MAX_STR];
    char cognom[MAX_STR];
    char nom_serie[MAX_STR];
    char valoracio[2];
} Rating;


int main(){
    Serie series[300];
    Rating ratings[300];

    //Open file & get its filedescriptor
    int fd_file = open("netflix_bin.show", O_RDONLY|O_CREAT);
    if(fd_file < 0) //If a file descriptor is not given to us, exit
        return 1;

    char c;
    char buffer[MAX_STR];
    int num_serie = 0;
    int counter = 0;
    int field = 0;
    Serie serie_buffer;
    while(read(fd_file, &c, sizeof(char)) != 0){ //While we're not at the end of the file
        if(c == '#'){
            buffer[counter] = 0;
            switch(field){
                case 0:
                    strcpy(serie_buffer.nom_serie, buffer);
                break;
                case 1:
                    strcpy(serie_buffer.genere, buffer);
                break;
                case 2:
                    strcpy(serie_buffer.subgenere, buffer);
                break;
                case 3:
                    serie_buffer.any = atoi(buffer);
                break;
                case 4:
                    serie_buffer.duracio = atoi(buffer);
                break;
                case 5:
                    serie_buffer.temporades = atoi(buffer);
                break;
                case 6:
                    serie_buffer.eposodis = atoi(buffer);
                break;
            }
            counter = 0;
            field++;
            memset(buffer, 0, MAX_STR);
        }
        else if(c == '\n'){
            serie_buffer.valoracio = atoi(buffer);
            counter = 0;
            field = 0;
            memset(buffer, 0, MAX_STR);
            series[num_serie] = serie_buffer;
            num_serie++;
        }
        else{
            buffer[counter] = c;
            counter++;
        }

    }

    close(fd_file);
    return 0;
}
