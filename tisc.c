#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int parse(char* opcode, char* arg);
int validLabel(char* label, int l);
int validRegister(char* reg);

typedef struct Table{
    char* key;
    int val;
}Table;

int pointer = 0;
int acc = 0;
int bak = 0;
int reg[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int main(int argc, char** argv){
    if(argc != 2){
        printf("usage: tisc input_file\n");
        return 0;
    }
    FILE *fl;
    fl=fopen(argv[1],"r");
    if(fl == NULL){
        printf("tisc: can't open file '%s': No such file\n", argv[1]);
        return 0;
    }
    int lines = 0;
    while(!feof(fl)) {
        int ch = fgetc(fl);
        if(ch == '\n') {
            lines++;
        }
    }
    rewind(fl);
    char** prog = malloc(sizeof(char*) * lines);
    for(int i = 0; i < lines; i++){
        prog[i] = malloc(256);
        fscanf(fl, "%[^\n]\n", prog[i]);
        if(strstr(prog[i], "#"));
        else if(strstr(prog[i], ":")){
            char jmp[16];
            char check[256];
            sscanf(prog[i], "%[^:]:%s", jmp,check);
            if(check[0] != '\0'){
                printf("tisc: invalid lavel at line %d: label can not have instruction following label declaration\n", i);
                fclose(fl);
                for(int j = 0; j <= i; j++){
                    free(prog[j]);
                }
                free(prog);
                return 0;
            }
            if(!validLabel(jmp, i)){
                fclose(fl);
                for(int j = 0; j <= i; j++){
                    free(prog[j]);
                }
                free(prog);
                return 0;
            }

        }
    }

    
    fclose(fl);

    return 0;
}
int parse(char* opcode, char* arg){
    if(opcode[0] == '#'){
        pointer++;
        return 0;
    }
    if(strstr(opcode, ":")){
        pointer++;
        return 0;
    }
    if(!strcmp(opcode, "HALT")){
        return 1;
    }
    else if(!strcmp(opcode, "SAV")){
        if(arg[0] = '\0'){
            bak = acc;
            return 0;
        }
        else{
            printf("tisc: invalid invalid argument count at line %d: instruction takes 0 arguments\n");
            return 1;
        }
    }
    else if(!strcmp(opcode, "NEG")){
        if(arg[0] = '\0'){
            acc *= -1;
            return 0;
        }
        else{
            printf("tisc: invalid invalid argument count at line %d: instruction takes 0 arguments\n");
            return 1;
        }

    }
    else if(!strcmp(opcode, "SWP")){
        if(arg[0] = '\0'){
            int swap = acc;
            acc = bak;
            bak = acc;
            return 0;
        }
        else{
            printf("tisc: invalid invalid argument count at line %d: instruction takes 0 arguments\n");
            return 1;
        }

    }



    return 0;
}
int validLabel(char* label, int l){
    int i;
    for(i = 0; label[i] != '\0'; i++){
        if(label[i] < 65 || label[i] > 90){
            printf("tisc: invalid label at line %d: label name can only contain letters\n", l); 
            return 0;
        }
    }
    if(i > 16){
        printf("tisc: invalid label at line %d: label name can not be longer than 16 characters", l);
        return 0;
    }
    return 1;
}
int validRegister(char* reg){
    if(!strcmp(reg, "ACC")){
        return 1;
    }
    if(!strstr(reg, "REG")){
        return 0;
    }
    int regc;
    sscanf(reg, "REG%d", &regc);
    if(!strcmp(reg, "REG0")) {
        return 1;
    }
    if(reg[3] == '0'){
        return 0;
    }
    if(regc > 15 || regc < 0){
        return 0;
    }
    return 1;
}
