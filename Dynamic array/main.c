#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int getchoice(void);
char * copy(char * source);
char * get_file_name(void);
char * get_memory(long int size);
char * readtext(FILE * f_ptr);
long int get_file_size(FILE * f_ptr);
FILE * fileopen(char * filename,char * mode);
char ** text_to_words(char * filestring, int * words);
char * find_occurence_of_word(char * word,char * lejilogio_string);
char * change_word(char * oldword,char *newword,char * text);
char * addword(char * text_string, char *word);
void correcttext(char **text,char ** text_string,char **lejilogio_string,int number_of_words);
int word_choice(void);
void lowerstring(char *str);
void updatefile(FILE * f_ptr,char * filename,char * text_string);
void fileclose(FILE * f_ptr);
int count_instances_of_word(char * word,char * text);


FILE * save_statistics(void);
void calculate_statistics(char **text, char * raw_string,int choice,int words);
void print_statistics(FILE * f_ptr,int words,int total_chars,int spaces,int different_words,int max_chars,char ** text);
int calculate_characters(char *word);
int calculate_total_characters(char **text,int number_of_words);
int calculate_different_words(char **text,int number_of_words);
int max_letters(char **text,int number_of_words);
int words_with_n_chars(char **text,int number_of_words,int chars);
int calculate_spaces(char * raw_string);


int main(){

    FILE * text_ptr;
    FILE * lejilogio_ptr;

    int choice;
    int words=0;

    char *text_string=NULL;
    char *copy_text_string=NULL;

    char *lejilogio_string=NULL;

    char **text=NULL;

    char *text_name=NULL;
    char *lejilogio_name=NULL;

    printf("Welcome to Word Analyzer!\n\n");



   while(choice=getchoice())
    {



        if (choice==1){


                free(text_name);
                free(text);
                free(text_string);
                free(copy_text_string);


                text_name=get_file_name();
                text_ptr=fileopen(text_name,"rb");
                text_string=readtext(text_ptr);

                copy_text_string=copy(text_string);
                text=text_to_words(copy_text_string,&words);

        }


        if (choice==2){

                free(lejilogio_name);
                free(lejilogio_string);

                lejilogio_name=get_file_name();
                lejilogio_ptr=fileopen(lejilogio_name,"rb");
                lejilogio_string=readtext(lejilogio_ptr);

        }





        if (choice==3){
              correcttext(text,&text_string,&lejilogio_string,words);
              updatefile(text_ptr,text_name,text_string);
              updatefile(lejilogio_ptr,lejilogio_name,lejilogio_string);

        }


        if (choice==4)  calculate_statistics(text,text_string,0,words);

        if (choice==5)  calculate_statistics(text,text_string,1,words);

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



char * get_memory(long int size){


char * ch_ptr;
if((ch_ptr=malloc(size*sizeof(char)))==NULL)
    {
        printf("Allocation failed");
        exit(EXIT_FAILURE);

    }
memset(ch_ptr,0,size);

return ch_ptr;
}


char * copy(char * source){

    int size;
    char * ch_ptr;

    size=strlen(source)+1;

    ch_ptr= get_memory(size);

    strncpy(ch_ptr,source,size);


    return ch_ptr;
}


char * get_file_name(void){

    char buffer[50];
    int size;
    char * ch_ptr;

    printf("Enter the name of the file:\n");
    scanf("%49s",buffer);


    size=strlen(buffer)+1;

    ch_ptr=get_memory(size);

    strncpy(ch_ptr,buffer,size);

    return ch_ptr;

}

char * readtext(FILE * f_ptr){


    long int size=get_file_size(f_ptr)+1;

    char * filestring=get_memory(size);


    fread(filestring,1,size-1,f_ptr);


    filestring[size-1]='\0';

    return filestring;
}

long int get_file_size(FILE * f_ptr){

    long int size;

    fseek(f_ptr,0,SEEK_END);

    size=ftell(f_ptr);

    fseek(f_ptr,0,SEEK_SET);

    return size;



}


FILE * fileopen(char * filename,char * mode){

    FILE * file_ptr;


    if((file_ptr=fopen(filename,mode))==NULL){
        printf("Can not open file.\n");
        exit(EXIT_FAILURE);
    }

    return file_ptr;


}

char ** text_to_words(char * filestring, int * words){

    char ** mytext=NULL;
    char * word;
    int size;
    char del[]="\n\t\v\f\r *!?;:,`.'[]\"()-_ ";

    *words=0;

    for(word=strtok(filestring,del);word!=NULL;word=strtok(NULL,del))
    {
        if (*word !=13)  /* Se periptosi pou epistrefetai mono carriage return agnoite i leji */
        {

            if(word[strlen(word)-1]==13)
            {
                word[strlen(word)-1]='\0';   /* Gia na min pernane stis teleutaies lejeis ta carriage return */
            }
            (*words) ++;

            if((mytext=realloc(mytext, (*words) * sizeof(char *)))==NULL){
                printf("Reallocation failed.\n");
                exit(EXIT_FAILURE);
            }

            size=strlen(word)+1;
            mytext[*words-1]=get_memory(size);
            strncpy(mytext[*words-1],word,size);

        }
    }


    return mytext;
}

void correcttext(char **text,char ** text_string,char **lejilogio_string,int number_of_words){


    int i;
    int choice=0;
    char newword[20];
    char *words_found=NULL;      /* Thimatai tis lejeis pou exoun idi antikatastathi */


    for(i=0;i<number_of_words && choice !=4;i++)
        {

            if((find_occurence_of_word(text[i],words_found))==NULL &&  find_occurence_of_word(text[i],(*lejilogio_string))==NULL)
            {

                printf("Word %s does not exist on the dictionary\n",text[i]);
                choice=word_choice();

                if(choice==1)
                {
                    printf("Enter a new word: ");
                    scanf("%19s",newword);


                    if(find_occurence_of_word(newword,*lejilogio_string)==NULL){
                        printf("Word  %s does not exist in the dictionary.\n",newword);
                        printf("If you want to add the word in the dictionary, enter 0 \n"
                               "If you want to continue with the change, enter 1.\n");
                        scanf("%d",&choice);
                    }
                        if(choice==0)

                            *lejilogio_string=addword(*lejilogio_string,newword);

                        if(choice==1){

                            (*text_string)=change_word(text[i],newword,*text_string);
                        }
                }

                if(choice==2)
                    {
                        (*lejilogio_string)=addword(*lejilogio_string,text[i]);
                    }


                words_found=addword(words_found,strcat(text[i],"\n"));      /* Oi lejei arxizi me \n kai teleioni me \n gia na anagnorizetai os autonomi leji apo */
                                                                            /* tin find_occurence_of_word */

            }

        }
        printf("Correction process has finished.\n");
        free(words_found);

    return;
}

char * addword(char * text_string, char *word){

    int size;

    if(text_string==NULL)
    {
       size=strlen(word)+1+1;                  /* Ena epipleon byte gia tin alagi gramis */
       text_string=realloc(text_string,size);
       text_string[0]='\0';                     /* H strcat xeirizetai null terminated strings */

    }
    else
    {
        size=strlen(text_string)+strlen(word)+1+1;
        text_string=realloc(text_string,size);

    }

        strcat(text_string,"\n");
        strcat(text_string,word);

    return text_string;
    }



char * change_word(char * oldword,char *newword,char * text){

    int oldsize=strlen(oldword);
    int newsize=strlen(newword);
    int metatopisi=newsize-oldsize;
    char *ch_ptr;
    int number;
    int textsize;

    number=count_instances_of_word(oldword,text);    /* poses fores emfanizetai i leji */

    textsize=strlen(text)+number*metatopisi+1;

    if(metatopisi>=0)
    {

        if((text=realloc(text,textsize))==NULL)
        {

            printf("Allocation failed.");
            exit(EXIT_FAILURE);

        }
        while((ch_ptr=find_occurence_of_word(oldword,text))!=NULL)
        {

            memmove(ch_ptr+strlen(oldword)+metatopisi,ch_ptr+strlen(oldword),strlen(ch_ptr)-strlen(oldword)+1);   /* Prosthetoume 1 gia na metaferetai to NUll byte. */
            strncpy(ch_ptr,newword,strlen(newword));

        }

    }
    else
    {

        while((ch_ptr=find_occurence_of_word(oldword,text))!=NULL)
        {

            memmove(ch_ptr+strlen(oldword)+metatopisi,ch_ptr+strlen(oldword),strlen(ch_ptr)-strlen(oldword)+1);
            strncpy(ch_ptr,newword,strlen(newword));

        }

        if((text=realloc(text,textsize))==NULL)
        {

            printf("Allocation failed.");
            exit(EXIT_FAILURE);
        }

    }


    return text;
}

char * find_occurence_of_word(char * word,char * lejilogio_string){
    /*
        An i leji yparxi epistefi deikti se auti
        An i leji den yparxi epistefi NULL



        Gia na yparxei i leji 'word' sto keimeno prepei

        1) Na yparxei san substring

        2) O xaraktiras prin kai o xaraktiras meta tin leji na min einai gramata
        oste na apoteli autonomi leji. (p.x. sit != sitting)

                                                                 */


    char * ch_ptr;

    int size;
    int thesi;

    char *copy_word;                             /* Dimiourgoume antigrafa gia na diatirountai ta kefalaia sto keimeno */
    char *copy_lejilogio_string;                 /* Briskoume sta antigrafa ti sxetiki thesi tis lejis apo tin arxi tou keimenou
                                                           kai epistrefoume ti thesi sto kanoniko keimeno opou emfanizetai i leji */

    if(lejilogio_string==NULL) return NULL;


    copy_word=copy(word);
    copy_lejilogio_string=copy(lejilogio_string);

    ch_ptr=copy_lejilogio_string;
    size=strlen(copy_word);

    lowerstring(copy_word);                        /* Oi lejeis Word kai WORD einai idies. */
    lowerstring(copy_lejilogio_string);




    while((ch_ptr=strstr(ch_ptr,copy_word))!=NULL && ch_ptr-copy_lejilogio_string<strlen(copy_lejilogio_string)){


        char boundary1;
        char boundary2;

        boundary1=*(ch_ptr-1);
        boundary2=*(ch_ptr+size);


        if(!(isalpha(boundary1)) && !(isalpha(boundary2)))

        {
            thesi=ch_ptr-copy_lejilogio_string;

            free(copy_lejilogio_string);
            free(copy_word);

            return lejilogio_string+thesi;

        }

        ch_ptr += size;

    }

    free(copy_lejilogio_string);
    free(copy_word);

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
    new_f_ptr=fileopen(filename,"wb");     /* Anoigma gia grapsimo */
    fwrite(text_string,1,size,new_f_ptr);
    fileclose(new_f_ptr);
    return;


}

void fileclose(FILE * f_ptr){

    fclose(f_ptr);
    return;
}

int count_instances_of_word(char * word,char * text){

    int i=0;
    char * ch_ptr=text;

    while((ch_ptr=find_occurence_of_word(word,ch_ptr))!=NULL)
    {
        i++;
        ch_ptr += strlen(word);

    }
    return i;

}


void calculate_statistics(char **text,char * raw_text,int choice,int words){

    int different_words;
    int max_chars;
    int total_chars;  /* Den perilambanontai ta kena */
    int spaces;


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


int calculate_characters(char *word){


    return strlen(word);


}

int calculate_total_characters(char **text,int number_of_words){

    int total_chars=0;
    int i;

    for(i=0;i<number_of_words;i++) total_chars += calculate_characters(text[i]);

return total_chars;
}

int calculate_different_words(char **text,int number_of_words){


    int num=0;
    int i,j,k;
    char *copy1;
    char *copy2;

    for(i=0;i<number_of_words;i++){
        k=1;
        copy1=copy(text[i]);
        lowerstring(copy1);

        for(j=0;j<i && k!=0;j++){

                copy2=copy(text[j]);
                lowerstring(copy2);

                if (strcmp(copy1,copy2)==0)k=0;      /* Oi lejeis Word kai word einai idies kai den tropopoiountai oi arxikes*/

                free(copy2);
        }


        num += k;
        if(k==1)
        free(copy1);
    }

return num;

}

int max_letters(char **text,int number_of_words){

int i;
int max_chars=0;
int chars;

    for(i=0;i<number_of_words;i++){
        chars=calculate_characters(text[i]);
        if(chars>max_chars) max_chars=chars;
    }

return max_chars;


}

int words_with_n_chars(char **text,int number_of_words,int chars){

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

        char *filename;
        FILE * f_ptr;

        filename=get_file_name();
        f_ptr=fileopen(filename,"wb");
        free(filename);

    return f_ptr;

}

void print_statistics(FILE * f_ptr,int words,int total_chars,int spaces,int different_words,int max_chars,char **text){

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
