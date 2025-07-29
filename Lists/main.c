#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


typedef struct node {
    char * str;
    struct node * next ;
} Node;



char * get_memory( int size);
Node * readtext(FILE * f_ptr);
char * get_memory( int size);
Node * create_node(void);

char * get_file_name(void);
FILE * fileopen(char * filename,char * mode);
void fileclose(FILE * f_ptr);
int getchoice();
void node_to_word(Node * node,char * word);
int find_node(char * word, Node * list);
void lowerstring(char *str);

void correct_text(Node * first_node_text, Node * first_node_lejilogio);
int is_word(Node * node);
char * define_word(char * word);
void addword(Node * list,char * word);
Node * initialize_node();
void change_word(Node * list,char * oldword,char * newword);
int word_choice(void);
int find_word(char * string,char * word);
void updatefile(FILE * f_ptr,char * filename,Node * list);
void free_list(Node * list);
int count_different_words(Node * list);
int count_words(Node * list);

int calculate_characters(char *word);
int calculate_total_characters(Node * list);
int max_letters(Node * list);
int words_with_n_chars(Node * list, int chars);
int calculate_spaces(Node * list);
void calculate_statistics(Node * list,int choice);
void print_statistics(FILE * f_ptr,int words,int total_chars,int spaces,int different_words,int max_chars,Node * list);
FILE * save_statistics(void);

/* void print_words(Node * first_node); */

int main()
{

    FILE * text_ptr;
    FILE * lejilogio_ptr;

    Node * first_node_text=NULL;
    Node * first_node_lejilogio=NULL;


    char *text_name=NULL;
    char *lejilogio_name=NULL;

    int choice=0;

    printf("Welcome to Word Analyzer!\n\n");



   while(choice=getchoice())
    {



        if (choice==1){

                free(text_name);
                free_list(first_node_text);

                text_name=get_file_name();
                text_ptr=fileopen(text_name,"rb");
                first_node_text=readtext(text_ptr);


        }


        if (choice==2){

                free(lejilogio_name);
                free_list(first_node_lejilogio);

                lejilogio_name=get_file_name();
                lejilogio_ptr=fileopen(lejilogio_name,"rb");
                first_node_lejilogio=readtext(lejilogio_ptr);


        }





        if (choice==3){

              correct_text(first_node_text,first_node_lejilogio);
              updatefile(text_ptr,text_name,first_node_text);
              updatefile(lejilogio_ptr,lejilogio_name,first_node_lejilogio);
        }


        if (choice==4)  calculate_statistics(first_node_text,0);



        if (choice==5)  calculate_statistics(first_node_text,1);

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

Node * readtext(FILE * f_ptr){

    int ch;
    char buffer[100];
    int size=0;
    Node * first_ptr=NULL;
    Node * previous_ptr=NULL;
    int count=0;

    do{

        ch=fgetc(f_ptr);
        size ++;

        buffer[size-1]=ch;

        if(  (isspace(buffer[size-2])  && (isspace(buffer[size-1]))==0)  || ch==EOF  )   /* Ksekinai apo ti leksi kai prostheti ola ta kena pou akolouthoun */
        {                                                                                /* Epanalmbanoume sto telos gia ti teleutaia leksi. */

            if(first_ptr==NULL)
            {
                first_ptr=create_node();
                first_ptr->str=get_memory(size);
                strncpy(first_ptr->str,buffer,size-1);                                    /* Apothikeui sto buffer eos otou sinantisi to proto grama tis      */
                                                                                          /* epomenis lejis, to opoio metaferetai sti sinexia stin arxi tou   */
                (first_ptr->str)[size-1]='\0';                                            /* buffer                                                           */
                previous_ptr=first_ptr;

            }

            else

            {
                previous_ptr->next=create_node();

                previous_ptr=previous_ptr->next;


                previous_ptr->str=get_memory(size);
                strncpy(previous_ptr->str,buffer,size-1);
                (previous_ptr->str)[size-1]='\0';


            }


            count ++;


            buffer[0]=buffer[size-1];
            memset(buffer+1,0,99);
            size=1;


        }

    }  while(ch!=EOF);




    return first_ptr;
}

char * get_memory( int size){


char * ch_ptr;
if((ch_ptr=malloc(size*sizeof(char)))==NULL)
    {
        printf("Allocation failed");
        exit(EXIT_FAILURE);

    }
memset(ch_ptr,0,size);

return ch_ptr;
}

Node * create_node(void){
    Node * new_node_ptr;

    if((new_node_ptr= malloc(sizeof (Node)))==NULL){
        printf("Memory problem\n");
        exit(EXIT_FAILURE);

    }

    new_node_ptr->next=NULL;

   return new_node_ptr;
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

FILE * fileopen(char * filename,char * mode){

    FILE * file_ptr;


    if((file_ptr=fopen(filename,mode))==NULL){
        printf("Can not open file.\n");
        exit(EXIT_FAILURE);
    }

    return file_ptr;


}

void node_to_word(Node * node,char * word){

    char buffer[100];
    char dels[]="\n\t\v\f\r *!?;:,`.'[]\"()-_ ";
    char * ch_ptr;

    memset(buffer,0,100);

    strncpy(buffer,node->str,99);
    ch_ptr=strtok(buffer,dels);

    if(ch_ptr!=NULL){       /* Ean periexontai mono simbola i strtok epistrefei NULL kai den alazoun ta periexomena tis leksis */


        strncpy(word,ch_ptr,99);
        word[strlen(ch_ptr)]='\0';
    }

return;

}

int find_node(char * word, Node * list){

    /* Epistrefi ti thesi tis leksis an iparxei alios -1 */

    int thesi=0;
    char buffer[100]={0};
    char copy_word[100];
    Node * iterator;

    strncpy(copy_word,word,99);
    copy_word[strlen(copy_word)]='\0';


    for(iterator=list;iterator!=NULL;iterator=iterator->next)
    {

        node_to_word(iterator,buffer);    /* An den iparxoun xaraktires den allazei ta periexomena tou buffer */


        if(buffer[0]!='\0')
           {
            lowerstring(buffer);
            lowerstring(copy_word);

             if(strcmp(buffer,copy_word)==0){
                    return thesi;
             }
            }
        memset(buffer,0,100);
        thesi ++;
    }

  return -1;

}

int is_word(Node * node){

/*Epistrefi 1 ean o kombos periexi  leji kai oxi mono simbola p.x.(!':?)  alios 0 */

    char buffer[100]={0};

    node_to_word(node,buffer);

    if(buffer[0]=='\0') return 0;

    else return 1;

}

void lowerstring(char *str){

    int i;

    for( i=0;str[i];i++)
        str[i]=tolower(str[i]);
return;

}

void correct_text(Node * first_node_text, Node * first_node_lejilogio){

    Node * iterator;
    char buffer[100];
    char newword[20];
    int thesi;
    int choice=0;
    Node * words_found=initialize_node();           /* Thimatai tis leksis pou exei brei */



    for(iterator=first_node_text;iterator!=NULL && choice != 4;iterator=iterator->next)
    {


        if(is_word(iterator))
        {
            node_to_word(iterator,buffer);


            if(find_node(buffer,words_found)== -1)
            {

                thesi=find_node(buffer,first_node_lejilogio);


                if(thesi==-1)
                {

                    printf("Word %s does not exist on the dictionary\n",buffer);

                    choice=word_choice();

                    if(choice==1)
                    {

                        printf("Give a new word: ");
                        scanf("%19s",newword);


                        if(find_node(newword,first_node_lejilogio)==-1)
                        {

                            printf("Word  %s does not exist in the dictionary.\n",newword);
                            printf("If you want to add the word in the dictionary, enter 0.\n"
                                    "If you want to continue with the change, enter 1.\n");

                            scanf("%d",&choice);
                        }
                        if(choice==0)
                        {
                            addword(first_node_lejilogio,newword);
                        }

                        if(choice==1){

                            change_word(first_node_text,buffer,newword);

                        }

                        addword(words_found,newword);

                    }


                    if(choice==2)
                    {
                        addword(first_node_lejilogio,buffer);
                    }





                }



            }

        addword(words_found,buffer);

        }

    }

free_list(words_found);


return;

}

void addword(Node * list,char * word ){

    Node * iterator=list;
    int size;


    size=strlen(word)+1+1;


    while( iterator->next!=NULL) iterator=iterator->next;

    iterator->next=create_node();
    iterator=iterator->next;

    iterator->str=get_memory(size);
    strcat(iterator->str,"\n");

    strcat(iterator->str,word);


    return;
}

Node * initialize_node(){
    Node * words_found=create_node();
    words_found->str=malloc(2* sizeof(char));
    strncpy(words_found->str," ",2);

    return words_found;

}

void change_word(Node * list,char * oldword,char * newword){


    Node * iterator=list;
    int i;
    int node;
    int thesi;
    char buffer[100]={0};
    int new_size;

    while((node=find_node(oldword,list))>=0)

    {
        for(i=0;i<node;i++) iterator=iterator->next; /*Diatrexei ta stoixia tis listas */

        thesi=find_word(iterator->str,oldword);
        strncpy(buffer,iterator->str,thesi);
        strcat(buffer,newword);
        strcat(buffer,iterator->str+thesi+strlen(oldword));

        new_size=strlen(buffer)+1;

        if((iterator->str=realloc(iterator->str,new_size*sizeof(char)))==NULL)
        {
            printf("Memory problem.");
            exit(EXIT_FAILURE);

        }

        strncpy(iterator->str,buffer,new_size);




        memset(buffer,0,100);
        iterator=list;

    }


return;
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

int find_word(char * string,char * word){

    char copy_string[100];
    char copy_word[100];
    int thesi;
    char * ch_ptr;

    strncpy(copy_string,string,100);
    strncpy(copy_word,word,100);

    lowerstring(copy_string);
    lowerstring(copy_word);

    ch_ptr=strstr(copy_string,copy_word);

    thesi=ch_ptr-copy_string;

    return thesi;


}

void fileclose(FILE * f_ptr){

    fclose(f_ptr);
    return;
}


void updatefile(FILE * f_ptr,char * filename,Node * list){

    Node * iterator;
    FILE * new_f_ptr;

    fileclose(f_ptr);
    new_f_ptr=fileopen(filename,"wb");     /* Anoigma gia grapsimo */

    for(iterator=list;iterator!=NULL;iterator=iterator->next)
    {
        int size=strlen(iterator->str);
        fwrite(iterator->str,1,size,new_f_ptr);
    }

    fileclose(new_f_ptr);

    return;
}


void free_list(Node * list){

    Node * iterator=list;
    Node * previous;

    if(list!=NULL){

        while(iterator->next!=NULL)
        {
          previous=iterator;
          iterator=iterator->next;

          free(previous->str);
          free(previous);

        }

        free(iterator->str);
        free(iterator);
    }
}




int count_different_words(Node * list){

    Node * iterator1;
    Node * iterator2;
    char * ch_ptr1;
    char * ch_ptr2;
    char copy1[100];
    char copy2[100];
    char dels[]="\n\t\v\f\r *!?;:,`.'[]\"()-_ ";
    int i,j,result;
    int numerator=0,num=0;

    for(iterator1=list,i=0;iterator1!=NULL;iterator1=iterator1->next,i++)
    {
        strncpy(copy1,iterator1->str,100);
        lowerstring(copy1);

        for(ch_ptr1=strtok(copy1,dels);ch_ptr1!=NULL;ch_ptr1=strtok(ch_ptr1,dels))    /* H strtok xrisimopoieitai dio fores opote prepei na tis dothi ek neou */
            {                                                                         /* to string alios sinexizei apo kei pou emine apo tin pio kato klisi */
                result=1;

                for(iterator2=list,j=0; j<i && result !=0 ; iterator2=iterator2->next,j++)     /*    An i leksi iparxei result=0 */
                {

                    strncpy(copy2,iterator2->str,100);
                    lowerstring(copy2);

                    for(ch_ptr2=strtok(copy2,dels);ch_ptr2!=NULL && result !=0 ;ch_ptr2=strtok(NULL,dels))
                    {
                        if(strcmp(ch_ptr1,ch_ptr2)==0) result =0;

                    }

                }



                ch_ptr1 = ch_ptr1+strlen(ch_ptr1)+1;

                num += result;

            }
    }

    return num;
}


int count_words(Node * list){

    Node * iterator;
    int words=0;
    char buffer[100];
    char dels[]="\n\t\v\f\r *!?;:,`.'[]\"()-_ ";
    char * ch_ptr;

    for(iterator=list;iterator!=NULL;iterator=iterator->next)

        {
            if(is_word(iterator)){

                strncpy(buffer,iterator->str,100);
                for(ch_ptr=strtok(buffer,dels);ch_ptr!=NULL;ch_ptr=strtok(NULL,dels)) words ++;


            }
        }
    return words;
}

int calculate_characters(char *word){


    return strlen(word);


}

int calculate_total_characters(Node * list)
{

    Node * iterator;
    int total_chars=0;
    char buffer[100];
    char dels[]="\n\t\v\f\r *!?;:,`.'[]\"()-_ ";
    char * ch_ptr;


    for(iterator=list;iterator!=NULL;iterator=iterator->next)
    {
        strncpy(buffer,iterator->str,100);

        for(ch_ptr=strtok(buffer,dels);ch_ptr!=NULL;ch_ptr=strtok(NULL,dels)){

            total_chars += calculate_characters(ch_ptr);
        }
    }
    return total_chars;
}

int max_letters(Node * list){

    Node * iterator;
    char *ch_ptr;
    int max_chars=0;
    int chars=0;
    char dels[]="\n\t\v\f\r *!?;:,`.'[]\"()-_ ";
    char buffer[100];



    for(iterator=list;iterator!=NULL;iterator=iterator->next){

        strncpy(buffer,iterator->str,100);
        for(ch_ptr=strtok(buffer,dels);ch_ptr!=NULL;ch_ptr=strtok(NULL,dels))
        {
            chars=calculate_characters(ch_ptr);

        }
        if(chars>max_chars) max_chars=chars;
    }

return max_chars;

}

int words_with_n_chars(Node * list, int chars){

    int number=0;
    char * ch_ptr;
    Node * iterator;
    char dels[]="\n\t\v\f\r *!?;:,`.'[]\"()-_ ";
    char buffer[100];


    for(iterator=list;iterator!=NULL;iterator=iterator->next)
    {

        strncpy(buffer,iterator->str,100);

        for(ch_ptr=strtok(buffer,dels);ch_ptr!=NULL;ch_ptr=strtok(NULL,dels))
        {

            if(calculate_characters(ch_ptr)==chars) number ++;

        }
    }

return number;
}


int calculate_spaces(Node * list){

    Node * iterator;
    char buffer[100];
    int i;
    int count=0;

    for(iterator=list;iterator!=NULL;iterator=iterator->next)
    {
        strncpy(buffer,iterator->str,100);
        for(i=0;i<strlen(buffer);i++){
            if(buffer[i]==' ') count ++;

        }
    }

    return count;
}

void calculate_statistics(Node * list,int choice){


    int different_words;
    int max_chars;
    int total_chars;  /* Den perilambanontai ta kena */
    int spaces;
    int words=count_words(list);


    different_words=count_different_words(list);
    max_chars=max_letters(list);
    total_chars=calculate_total_characters(list);
    spaces=calculate_spaces(list);

    if(choice==0)
    {
        FILE * f_ptr;
        f_ptr=save_statistics();
        print_statistics(f_ptr,words,total_chars,spaces,different_words,max_chars,list);
    }
     if(choice==1)

    {
        print_statistics(stdout,words,total_chars,spaces,different_words,max_chars,list);
    }

}

void print_statistics(FILE * f_ptr,int words,int total_chars,int spaces,int different_words,int max_chars,Node * list){

        int i,j;

        fprintf(f_ptr,"Text has %d words \n", words);
        fprintf(f_ptr,"There are %d characters without spaces and with spaces there are %d characters\n", total_chars, total_chars+spaces);
        fprintf(f_ptr,"There are %d different words\n", different_words);

        for(i=1;i<max_chars+1;i++)
            {
                int number;
                number=words_with_n_chars(list,i);
                fprintf(f_ptr,"%d: ",i);
                for(j=0;j<number;j++)fprintf(f_ptr,"#");

                fprintf(f_ptr,"(%d)",number);


                fprintf(f_ptr,"\n");
            }
        if(f_ptr != stdout) fclose(f_ptr);

    return;
}


FILE * save_statistics(void){

        char *filename;
        FILE * f_ptr;

        filename=get_file_name();
        f_ptr=fileopen(filename,"wb");
        free(filename);


    return f_ptr;

}

/*

void print_words(Node * first_node){

    Node * iterator;
    int count=0;

    for(iterator=first_node;iterator!=NULL;iterator=iterator->next){

        count ++;

        printf("%d word is %s\n",count,iterator->str);
    }
    printf("\n");
    return;
}

*/




