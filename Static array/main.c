#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define mwords 67221
#define mchars 20

/* Needs increase of stack size to run, otherwise it crashes. This is accomplished with the command: gcc main.c -o main.exe "-Wl,--stack=8388608" ./main !! */

int getchoice(void);
void readtext(FILE * f_ptr,char *file_string);
void text_to_words(char * filestring,char words[mwords][mchars]);

void copy(char * destination ,char * source, int size );
FILE * save_statistics(void);


void calculate_statistics(char text[mwords][mchars], char * raw_string,int choice);
void print_statistics(FILE * f_ptr,int words,int total_chars,int spaces,int different_words,int max_chars,char text[mwords][mchars]);



int calculate_words(char text[mwords][mchars]);
int calculate_characters(char word[mchars]);
int calculate_total_characters(char text[mwords][mchars],int number_of_words);
int calculate_different_words(char text[mwords][mchars],int number_of_words);
int max_letters(char text[mwords][mchars],int number_of_words);
int words_with_n_chars(char text[mwords][mchars],int number_of_words,int chars);
int calculate_spaces(char * raw_string);



void get_file_name(char[50]);
FILE * fileopen(char * name,char * mode);
void fileclose(FILE * f_ptr);


void correcttext(char text[mwords][mchars],char * text_string,char *lejilogio_string);
char * find_occurence_of_word(char * word,char * lejilogio_string);
void change_word(char * oldword,char *newword,char * text);
int word_choice(void);
void lowerstring(char *);
void updatefile(FILE * f_ptr,char * filename,char * text_string);
void addword(char * text_string, char *word);


/* void printwords(char text[mwords][mchars]); */


int main(){

    FILE * text_ptr=NULL;
    FILE * lejilogio_ptr=NULL;

    int choice;


    char text_string[mwords*mchars];
    char copy_text_string[mwords*mchars];

    char lejilogio_string[mwords*mchars];

    char text[mwords][mchars];

    char text_name[50];
    char lejilogio_name[50];



    printf("Welcome to Word Analyzer!\n\n");

   while(choice=getchoice())
    {



        if (choice==1){
                get_file_name(text_name);
                text_ptr=fileopen(text_name,"rb");
                readtext(text_ptr,text_string);
                copy(copy_text_string,text_string,mwords*mchars);
                text_to_words(copy_text_string,text);

        }


        if (choice==2){
                get_file_name(lejilogio_name);
                lejilogio_ptr=fileopen(lejilogio_name,"rb");
                readtext(lejilogio_ptr,lejilogio_string);



        }

        if (choice==3){
              correcttext(text,text_string,lejilogio_string);
              updatefile(text_ptr,text_name,text_string);
              if(lejilogio_ptr!=NULL) updatefile(lejilogio_ptr,lejilogio_name,lejilogio_string);  /* Gia tin periptosi pou o xristis den isagei lejilogio */

        }


        if (choice==4)  calculate_statistics(text,text_string,0);

        if (choice==5)  calculate_statistics(text,text_string,1);

   }


return 0;
}

int getchoice(){

    int choice;
    printf("Choose 0 to exit the program\n\n"

        "Choose 1 to enter your text file\n\n"

        "Choose 2 to load a dictionary\n\n"

        "Choose 3 to correct the text\n\n"

        "Choose 4 to save the statistics\n\n"

        "Choose 5 to display statistics from the text\n");

    scanf("%d",&choice);



    return choice;
}

void get_file_name(char filename[50]){

    printf("Enter the name of the file:\n");
    scanf("%49s",filename);

    return;

}

FILE * fileopen(char * filename,char * mode){

    FILE * file_ptr;


    if((file_ptr=fopen(filename,mode))==NULL){
        printf("Can not open file.\n");
        exit(EXIT_FAILURE);
    }

    return file_ptr;


}

void readtext(FILE * f_ptr,char *filestring){


    int size=mchars*mwords;

    memset(filestring,0,size);

    fread(filestring,1,size-1,f_ptr);



    return;
}

void text_to_words(char * filestring,char words[mwords][mchars]){


    char * ch_ptr;
    int i=0;
    char del[]="!(),-:*.?;`' \n";

    memset(words,0,mwords*mchars);

    for(ch_ptr=strtok(filestring,del);ch_ptr!=NULL;ch_ptr=strtok(NULL,del))
    {
        if (*ch_ptr !=13)  /* Se periptosi pou epistrefetai mono carriage return agnoite i leji */
        {
            strncpy(words[i],ch_ptr,mchars);
            if(words[i][strlen(words[i])-1]==13)
            {
                    words[i][strlen(words[i])-1]='\0';   /* Gia na min pernane stis teleutaies lejeis ta carriage return */
            }
                i++;
        }
    }




    return;
}

void copy(char * destination ,char * source , int size){

    memmove(destination,source,size);
    return;
}

void calculate_statistics(char text[mwords][mchars],char * raw_text,int choice){

    int words;
    int different_words;
    int max_chars;
    int total_chars;  /* Den perilambanontai ta kena */
    int spaces;


    words=calculate_words(text);
    different_words=calculate_different_words(text,words);
    max_chars=max_letters(text,words);
    total_chars=calculate_total_characters(text,words);
    spaces= calculate_spaces(raw_text);

    if(choice==0)
        {

        FILE * f_ptr;
        f_ptr=save_statistics();
        print_statistics(f_ptr,words,total_chars,spaces,different_words,max_chars,text);
        }
    if(choice==1)  {
        print_statistics(stdout,words,total_chars,spaces,different_words,max_chars,text);
    }


    return;
    }

int calculate_words(char text[mwords][mchars]){
    int i=0;

    while(i<mwords&&strlen(text[i])){

        i++;
    }
    return i;


}

int calculate_characters(char word[mchars]){


    return strlen(word);


}

int calculate_total_characters(char text[mwords][mchars],int number_of_words){

    int total_chars=0;
    int i;

    for(i=0;i<number_of_words;i++) total_chars += calculate_characters(text[i]);

return total_chars;
}

int calculate_different_words(char text[mwords][mchars],int number_of_words){


    int num=0;
    int i,j,k;
    char copy1[mchars];
    char copy2[mchars];

    for(i=0;i<number_of_words;i++){
        k=1;
        copy(copy1,text[i],mchars);
        lowerstring(copy1);

        for(j=0;j<i && k!=0;j++){

                copy(copy2,text[j],mchars);
                lowerstring(copy2);

                if (strcmp(copy1,copy2)==0)k=0;      /* Oi lejeis Word kai word einai idies kai den tropopoiountai oi arxikes*/
        }
        num += k;
    }

return num;

}

int max_letters(char text[mwords][mchars],int number_of_words){

int i;
int max_chars=0;
int chars;

    for(i=0;i<number_of_words;i++){
        chars=calculate_characters(text[i]);
        if(chars>max_chars) max_chars=chars;
    }

return max_chars;


}

int words_with_n_chars(char text[mwords][mchars],int number_of_words,int chars){

int i,number;

number=0;

for(i=0;i<number_of_words;i++)
    if(calculate_characters(text[i])==chars) {
            number +=1 ;
    }

return number;
}


int calculate_spaces(char * text_string){

    int i;
    int count=0;
    for(i=0;i<strlen(text_string);i++)
        if(text_string[i]==' ') count +=1 ;

    return count;


}

FILE * save_statistics(void){

        char filename[50];
        FILE * f_ptr;

        get_file_name(filename);
        f_ptr=fileopen(filename,"wb");


    return f_ptr;





}

void print_statistics(FILE * f_ptr,int words,int total_chars,int spaces,int different_words,int max_chars,char text[mwords][mchars]){

        int i,j;

        fprintf(f_ptr,"Text has %d words \n", words);
        fprintf(f_ptr,"There are %d characters without spaces and with spaces there are %d characters\n", total_chars, total_chars+spaces);
        fprintf(f_ptr,"There are %d different words\n", different_words);

        for(i=1;i<max_chars+1;i++)
            {
                int number;
                number=words_with_n_chars(text,words,i);
                fprintf(f_ptr,"%d: ",i);
                for(j=0;j<number;j++)fprintf(f_ptr,"#");

                fprintf(f_ptr,"(%d)",number);

                fprintf(f_ptr,"\n");
            }
        if(f_ptr != stdout) fclose(f_ptr);

    return;
}





void correcttext(char text[mwords][mchars],char * text_string,char *lejilogio_string){


    int i;
    int number_of_words=calculate_words(text);
    int choice=0;
    char newword[mchars];
    char words_found[10000]="";      /* Thimatai tis lejeis pou exoun idi antikatastathi */


    for(i=0;i<number_of_words && choice !=4;i++)
        {

            if(find_occurence_of_word(text[i],lejilogio_string)==NULL && strstr(words_found,text[i])==NULL)
            {

                printf("Word %s does not exist in the dictionary\n",text[i]);
                choice=word_choice();

                if(choice==1)
                {
                    printf("Enter a new word: ");
                    scanf("%19s",newword);


                    if(find_occurence_of_word(newword,lejilogio_string)==NULL){
                        printf("Word  %s does not exist in the dictionary.\n",newword);
                        printf("If you want the word to be added in the dictionary, enter 0 \n"
                               "If you want to continue with the change, enter 1\n");
                        scanf("%d",&choice);
                    }
                        if(choice==0)

                            addword(lejilogio_string,newword);

                        if(choice==1){

                            change_word(text[i],newword,text_string);

                        }
                }

                if(choice==2)
                    {
                        addword(lejilogio_string,text[i]);

                    }
                strcat(words_found,text[i]);
            }

        }
        printf("The correction process has finished.\n");

    return;
}


char * find_occurence_of_word(char * word,char * lejilogio_string){
    /*
        An i leji yparxi epistefi deikti se auti
        An i leji den yparxi epistefi NULL */


        /*
        Gia na yparxei i leji 'word' sto keimeno prepei

        1) Na yparxei san substring

        2) O xaraktiras prin kai o xaraktiras meta tin leji na min einai gramata
        oste na apoteli autonomi leji. (p.x. sit--sitting)

                                                                 */


    char * start;
    char * ch_ptr;


    int size;
    int thesi;
    int innercount=0;

    char copy_word[mchars];                             /* Dimiourgoume antigrafa gia na diatirountai ta kefalaia sto keimeno */
    char copy_lejilogio_string[mchars*mwords];          /* Briskoume sta antigrafa ti sxetiki thesi tis lejis apo tin arxi tou keimenou
                                                           kai epistrefoume ti thesi sto kanoniko keimeno opou emfanizetai i leji */

    copy(copy_word,word,mchars);
    copy(copy_lejilogio_string,lejilogio_string,mwords*mchars);

    start=copy_lejilogio_string;
    ch_ptr=copy_lejilogio_string;
    size=strlen(copy_word);

    lowerstring(copy_word);                        /* Oi lejeis Word kai WORD einai idies. */
    lowerstring(copy_lejilogio_string);


    while((ch_ptr=strstr(ch_ptr,copy_word))!=NULL && ch_ptr-start<mchars*mwords ){


        char boundary1;
        char boundary2;

        boundary1=*(ch_ptr-1);
        boundary2=*(ch_ptr+size);
        innercount++;


        if(!(isalpha(boundary1)) && !(isalpha(boundary2)))

        {
            thesi=ch_ptr-start;


            return lejilogio_string+thesi;

        }

        ch_ptr += size;

    }

    return NULL;

}



int word_choice(void){

    int choice=0;

    printf("If you want to replace the word with a new one, enter 1\n\n"

           "If you want to add the word in the dictionary, enter 2\n\n"

           "If you want to continue the correction process, enter 3\n\n"

           "If you want to return to the start menu, enter 4\n\n"
           );
    scanf("%d",&choice);

    return choice;

}

void change_word(char * oldword,char *newword,char * text){

    int oldsize=strlen(oldword);
    int newsize=strlen(newword);
    int metatopisi=newsize-oldsize;
    char *ch_ptr;

    while((ch_ptr=find_occurence_of_word(oldword,text))!=NULL){

        memmove(ch_ptr+strlen(oldword)+metatopisi,ch_ptr+strlen(oldword),strlen(ch_ptr)-strlen(oldword)+1);
        strncpy(ch_ptr,newword,strlen(newword));
    }



    return;
}

void lowerstring(char *str){

    int i;

    for( i=0;str[i];i++)
        str[i]=tolower(str[i]);
return;

}

void updatefile(FILE * f_ptr,char * filename,char * text_string){

    int size=strlen(text_string);
    FILE * new_f_ptr;

    fileclose(f_ptr);
    new_f_ptr=fileopen(filename,"wb");
    fwrite(text_string,1,size,new_f_ptr);
    fileclose(new_f_ptr);
    return;


}

void fileclose(FILE * f_ptr){

    fclose(f_ptr);
    return;
}

void addword(char * text_string, char *word){

    strcat(text_string,"\n");
    strcat(text_string,word);
    return;
    }

    /*
void printwords(char text[mwords][mchars]){

    int i;
    int k=calculate_words(text);
    printf("number of words %d\n",k);
    for(i=0;i<k;i++){
            printf("i: %d word:%s size: %d \n",i,text[i],strlen(text[i]));

    }

    return ;

}
*/
