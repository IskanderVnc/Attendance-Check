#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>

typedef struct data s_data;
struct data{
    char *surname;
    char *name;
    s_data *next;
};

// ORDERING PHASE //
s_data *createList(char *fileName);
void orderedInsertion(s_data *new, s_data *head);
char *getSurname(char *buf);
char *getName(char *buf);
void freeMem(s_data *head);
void updateList(char *fileName, s_data *head);
// -------------- //

// CHECK PHASE //
int checkList(s_data *list, char *csvDoc, int rows);
// -------------- //

// PRESENCES COLUMN CREATION //
void modifyString(char *string);
void addDate(char *string, char *newDate);
int searchStudent(char *buffer,s_data *list);
int checkSpacePresence(char *string);
void tolowerString(char *string);
char *copyString(char *string);
int computeNrows(char *fileName);
int superStringCompare(char **words1, char **words2, int size);
// -------------- //

void checkList3(s_data *list1, s_data*list2);
void setColor(int ForgC);

int main()
{
    char *fileName = "Actual_Attendees.txt";
    s_data *dataList, *tmp, *list2;
    FILE *fp , *output;
    char buffer[1024], newDate[32], newFile[80];
    int i=0 , rows = 244, found = 0, ok=0, ok2=0, nNames;
    char choice[1024];
    int okChoice = 0;

    // PHASE 1 : FORMATTING THE TXT DOCUMENT "elenco" //
    dataList = createList(fileName);
    tmp = dataList;
    /*while(tmp != NULL){
        printf("%d) %s %s \n", i++, tmp->surname, tmp->name);
        tmp = tmp->next;
    }*/
    printf("�����������������������������������������������������������������������������������������������������������������������������������\n");
    printf("\n");
    setColor(12);
    printf("(!) ISTRUZIONI (!) \n");
    printf("\n");
    printf("1) Copiare 'Actual_Attendees.txt' nella cartella contenente l'eseguibile\n");
    printf("2) Controllare che dopo l'ultima parola del file 'Actual_Attendees.txt' sia presente un 'a capo' \n");
    printf("3) Controllare che non ci siano due 'a capo' di fila a fine documento ( solo uno e' necessario) \n");
    printf("4) Eseguire il programma, se non parte continuare a provare fino a quando non parte \n");
    printf("5) Una volta concluso il processo copiare ed incollare la colonna presente nel nuovo documento .csv nel documento originiale \n");
    setColor(15);

    printf("\n");
    printf("Enter number of rows of the Excel document : ");
    scanf("%d", &rows);
    printf("\n");

    // UPDATING THE DOCUMENT WITH THE ORDERED LIST //
    printf("�����������������������������������������������������������������������������������������������������������������������������������\n");
    printf("Updating the ordered list to the new file ( OrderedList.txt) ...\n");
    updateList(fileName, dataList);
    printf("\n");
    setColor(10);
    printf("-- > OPERATION SUCCESFULLY COMPLETED \n");
    setColor(15);
    printf("\n");
    printf("�����������������������������������������������������������������������������������������������������������������������������������\n");
    //----------------------------------------//

    // PHASE 2 : CHECK OF PRESENCE OF NAMES FROM TXT 'OrderedList.txt' TO CSV DOCUMENT //
    printf("\n");
    printf(">> Checking if any mismatches between names of the students in 'Actual_Attendees.txt' and 'Partecipants_List.csv' are found ...\n");
    printf("\n");
    ok2 = checkList(dataList,"Partecipants_List.csv",rows);
    printf("\n");
    printf("\n");
    nNames = computeNrows("Actual_Attendees.txt");
    if(nNames != ok2){
        printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
        setColor(10);
        printf("-- > CHECK COMPLETED : you should give a look at these names in both the 'Actual_Attendees.txt' and 'presenze CLMD 2020-canipari' files : \n");
        printf(">    # OK : %d/%d ( %d names successfully matched over %d ) <\n", ok2,nNames, ok2, nNames);
        setColor(15);
        printf("\n");
        printf("�����������������������������������������������������������������������������������������������������������������������������������\n");
        printf("\n");
        printf("\n");
        printf(">> Would you like to continue the process or quit ? \n");
        printf("\n");
        printf("| [0] - Quit \n");
        printf("| [1] - Continue \n");
        printf("\n");
        printf(">> Waiting for input : ");
        fgets(choice, 1024, stdin);
        while(okChoice == 0){
            if(strlen(choice) == 2 && choice[0] == '1'){
                okChoice = 1;
                // DO NOTHING //
            }
            if(strlen(choice) == 2 && choice[0] == '0'){
                return 1;
            }
            if(okChoice == 0){
                printf(">> Waiting for input : ");
                fgets(choice,1024,stdin);
            }
        }
    } else{
        setColor(10);
        printf("-- > CHECK COMPLETED : SUCCESSFULL - NO MISMATCHES FOUND \n");
        printf(">    # OK : %d/%d ( %d names successfully matched over %d ) <\n", ok2,nNames, ok2, nNames);
        setColor(15);
        printf("�����������������������������������������������������������������������������������������������������������������������������������\n");
    }
    // ------------------------------------ //
    printf("\n");
    printf("\n");

    fp = fopen("Partecipants_List.csv","r");

    if(fp == NULL){
        setColor(12);
        printf("Error while opening file\n");
        setColor(15);
    }

    printf(">> Enter the desired date to add (in a format like this : '5-aprile') : ");
    scanf("%s", newDate);
    printf("\n");
    printf("\n");

    i = 0;
    while(i < rows){
        fgets(buffer,1024,fp);
        if(i==0){
            addDate(buffer,newDate);
            sprintf(newFile,"%s (%s).csv", "presenze CLMD 2020-canipari", newDate);
            output = fopen(newFile,"w");
            fprintf(output,"%s", buffer);
        } else{
            if(buffer[0] != ';' && buffer[0] != '\n'){
                found = searchStudent(buffer,dataList);
            }
            if(found==1){
                ok++;
                modifyString(buffer);
            } else {
                //printf("NF : %s \n", buffer);
            }
            fprintf(output,"%s", buffer);
            found = 0;
        }
        i++;
    }

    list2 = createList("found.txt");
    checkList3(dataList,list2);
    fclose(fp);
    fclose(output);

    if(remove("OrderedList.txt") == 0){
        // printf("OrderedList.txt deleted\n");
    } else {
        printf("Unable to delete the file\n");
    }
    if(remove("found.txt") == 0){
        //printf("found.txt deleted\n");
    } else {
        printf("Unable to delete the file\n");
    }

    // FREE LIST MEMORY //
    freeMem(dataList);
    freeMem(list2);
    system("PAUSE");
    return 0;
}

void modifyString(char *string){
    char tmp[268];
    string[strlen(string)-1] = ';';
    sprintf(tmp,"%s;\n","1");
    strcat(string,tmp);
}

void addDate(char *string,char *newDate){
    char tmp[80];
    string[strlen(string)-1] = ';';
    sprintf(tmp,"%s;\n", newDate);
    strcat(string,tmp);
}

int searchStudent(char *buffer,s_data *list){
    int nFound = 0, nWordsF=0, nWordsL=0, j=0, j2,k=0, nWtxt = 0, matchCount=0, found=0;
    s_data *tmpH, *tmp;
    char  *tmpString, *tmpString2, **words, **wordstxt;
    char *first, *last , *cp, *cp1;
    FILE *foundList = fopen("found.txt","a");

    tmpH = list;

    tmp = tmpH;
    tmpString = copyString(buffer);
    tmpString2 = copyString(buffer);
    nWordsF = 0;
    nWordsL = 0;
    matchCount = 0;
    found=0;
    j = 0;
    k = 0;
    last = strrchr(tmpString2,';');
    first = strtok(tmpString,";\0");
    // CHECK THE NUMBER OF WORDS IN SURNAME-NAME //
    cp = copyString(first);
    cp1 = strtok(cp," ");
    while(cp1 != NULL){
        nWordsF++;
        cp1 = strtok(NULL," \0");
    }
    free(cp);
    cp = copyString(last);
    cp1 = strtok(cp," ");
    while(cp1 != NULL){
        nWordsL++;
        cp1 = strtok(NULL," \0");
    }
    free(cp);
    //--------------//
    // SAVE WORDS IN A *CHAR ARRAY //
    words = (char**)malloc((nWordsF+nWordsL)*sizeof(char*));
    cp = copyString(first);
    cp1 = strtok(cp," ");
    while(cp1 != NULL){
        words[j] = copyString(cp1);
        j++;
        cp1 = strtok(NULL," \0");
    }
    free(cp);
    cp = copyString(last+1);
    cp1 = strtok(cp," ");
    while(cp1 != NULL){
        words[j] = copyString(cp1);
        j++;
        cp1 = strtok(NULL," \0");
    }
    free(cp);
    words[j-1][strlen(words[j-1])-1] = '\0';
    //--------------//
    j2 = j;
    // ITERATING OVER THE ORDERED LIST TO SEARCH FOR A MATCH //
    while(tmp != NULL){
        nWtxt = 0;
        k=0;
        j=0;
        matchCount = 0;
        cp = copyString(tmp->surname);
        cp1 = strtok(cp," ");
        while(cp1 != NULL){
            nWtxt++;
            cp1 = strtok(NULL," \0");
        }
        free(cp);
        cp = copyString(tmp->name);
        cp1 = strtok(cp," ");
        while(cp1 != NULL){
            nWtxt++;
            cp1 = strtok(NULL," \0");
        }
        free(cp);
        if(nWtxt == (nWordsF+nWordsL)){
            wordstxt = (char**)malloc((nWordsF+nWordsL)*sizeof(char*));
            cp = copyString(tmp->surname);
            cp1 = strtok(cp," ");
            while(cp1 != NULL){
                wordstxt[j] = copyString(cp1);
                j++;
                cp1 = strtok(NULL," \0");
            }
            free(cp);
            cp = copyString(tmp->name);
            cp1 = strtok(cp," ");
            while(cp1 != NULL){
                wordstxt[j] = copyString(cp1);
                j++;
                cp1 = strtok(NULL," \0");
            }
            free(cp);
            // COMPARE EACH STRING FROM WORDS AND EACH STRING FROM WORDSTXT //
            matchCount = superStringCompare(words,wordstxt,(nWordsF+nWordsL));
            //--------------//
            if(matchCount == (nWordsF+nWordsL)){
                found = 1;
                k = 0;
                while(k < j){
                    free(wordstxt[k]);
                    k++;
                }
                break;
            } else {
                while(k < j){
                    free(wordstxt[k]);
                    k++;
                }
            }
        }
        tmp = tmp->next;
    }
    k=0;
    //------------//

    if(found == 0){
        // printf("NF : %s \n", buffer);
    } else {
        last[0] = ' ';
        last[strlen(last)-1] = '\0';
        //printf("F : %s %s\n", first, last);
        fprintf(foundList,"%s %s\n", first, last);
        nFound++;
    }
    // FREE ALL ALLOCATED VARIABLES //

    k=0;
    while(k < j2){
        free(words[k]);
        k++;
    }
    free(wordstxt);
    free(words);
    free(tmpString);
    free(tmpString2);
    fclose(foundList);
    return nFound;
}

int checkSpacePresence(char *string){
    int i=0, found=0;
    for(i=0; i<strlen(string); i++){
        if(string[i] == ' '){
            found=1;
        }
    }
    return found;
}

void tolowerString(char *string){
    int i;

    for(i=0; i<strlen(string); i++){
        string[i] = tolower(string[i]);
    }
}

char *copyString(char *string){
    char *newString;
    newString = (char*)malloc(strlen(string)+1);
    strcpy(newString,string);
    return newString;
}

s_data *createList(char *nomeFile){
    FILE *file;
    int i = 0;
    char buffer[1024];
    char *surname;
    s_data *list, *head;

    file = fopen(nomeFile,"r");
    if(file == NULL){
        setColor(12);
        printf("�������������������������������������������������������������������������������\n");
        printf("WRONG FILE NAME OR MISSING FILE : REMEMBER TO NAME YOUR .txt FILE as 'elenco'\n");
        printf("�������������������������������������������������������������������������������\n");
        setColor(15);
        exit(EXIT_FAILURE);
    }
    while(fgets(buffer,1024,file) != NULL){
        surname = strdup(getSurname(buffer));
        if(i==0){
            list = (s_data*)malloc(sizeof(s_data));
            list->surname = strdup(surname);
            list->name = getName(buffer);
            list->next = NULL;
            head = list;
            i++;
        } else {
            list = (s_data*)malloc(sizeof(s_data));
            list->surname = strdup(surname);
            list->name = getName(buffer);
            if(i==1){
                if(strcmp(list->surname,head->surname) <= 0){
                    list->next = head;
                    head = list;
                    i++;
                } else {
                    list->next = head->next;
                    head->next = list;
                    i++;
                }
            } else{
                if(strcmp(list->surname,head->surname) <= 0){
                    list->next = head;
                    head = list;
                } else {
                    orderedInsertion(list,head);
                }
            }
        }

    }
    fclose(file);
    return head;
}

void orderedInsertion(s_data *new, s_data *head){
    s_data *tmp = head;

    while(tmp != NULL){

        if(tmp->next != NULL){
            if(strcmp(new->surname,tmp->surname) >= 0 && strcmp(new->surname,tmp->next->surname) <= 0){
                new->next = tmp->next;
                tmp->next = new;
                break;
            }}  else {
            new->next = tmp->next;
            tmp->next = new;
            break;
        }
        tmp = tmp->next;

    }
}

char *getSurname(char *buf){
    char surname[30];
    int i=0, j=0 , k=0;
    char *token = strtok(buf, " ");
    char *s;

    while(token != NULL){
        if(i==0){
            // DO NOTHING BECAUSE IT IS THE NAME //
        } else {

            if(j != 0){
                surname[j] = ' ';
                j++;
            }
            for(k=0; k<strlen(token); k++){
                surname[j] = token[k];
                j++;
            }

        }
        token = strtok(NULL, " ");
        i++;
    }
    surname[j-1] = '\0';
    surname[0] = toupper(surname[0]);
    s = strdup(surname);
    return s;
}

char *getName(char *buf){
    char *name;
    char *token = strtok(buf," ");
    name = strdup(token);
    return name;
}
void freeMem(s_data *head){
    s_data *tmp = head;
    if(tmp->next != NULL){
        freeMem(tmp->next);
    }
    free(tmp);
}

void updateList(char *fileName, s_data *head){
    FILE *file;
    s_data *tmp = head;
    file = fopen("OrderedList.txt","w");

    while(tmp != NULL){
        fprintf(file, "%s %s\n", tmp->surname, tmp->name);
        tmp = tmp->next;
    }
    fclose(file);
}

int checkList(s_data *list, char *csvDoc, int rows){
    int nFound = 0, i=0, nWordsF=0, nWordsL=0, j=0, j2,k=0, nWtxt = 0, matchCount=0, found=0;
    s_data *tmp;
    FILE *csv;
    char buffer[1024], *tmpString, *tmpString2, **words, **wordstxt;
    char *first, *last , *cp, *cp1;

    tmp = list;
    while(tmp != NULL){
        csv = fopen(csvDoc,"r");
        if(csv == NULL){
            setColor(12);
            printf("ERROR : missing .csv file - check the name or its presence in the folder \n");
            setColor(15);
            exit(EXIT_FAILURE);
        }
        found = 0;
        nWtxt = 0;
        k=0;
        j=0;
        i=0;
        matchCount = 0;
        cp = copyString(tmp->surname);
        cp1 = strtok(cp," ");
        while(cp1 != NULL){
            nWtxt++;
            cp1 = strtok(NULL," \0");
        }
        free(cp);
        cp = copyString(tmp->name);
        cp1 = strtok(cp," ");
        while(cp1 != NULL){
            nWtxt++;
            cp1 = strtok(NULL," \0");
        }
        free(cp);
        // SAVE WORDS INTO A *CHAR ARRAY //
        wordstxt = (char**)malloc((nWordsF+nWordsL)*sizeof(char*));
        cp = copyString(tmp->surname);
        cp1 = strtok(cp," ");
        while(cp1 != NULL){
            wordstxt[j] = copyString(cp1);
            j++;
            cp1 = strtok(NULL," \0");
        }
        free(cp);
        cp = copyString(tmp->name);
        cp1 = strtok(cp," ");
        while(cp1 != NULL){
            wordstxt[j] = copyString(cp1);
            j++;
            cp1 = strtok(NULL," \0");
        }
        free(cp);
        j2 = j;
        while(i < rows){
            fgets(buffer,1024,csv);
            if(i > 1){
                tmpString = copyString(buffer);
                tmpString2 = copyString(buffer);
                nWordsF = 0;
                nWordsL = 0;
                matchCount = 0;
                j = 0;
                k = 0;
                last = strrchr(tmpString2,';');
                first = strtok(tmpString,";\0");
                // CHECK THE NUMBER OF WORDS IN SURNAME-NAME //
                cp = copyString(first);
                cp1 = strtok(cp," ");
                while(cp1 != NULL){
                    nWordsF++;
                    cp1 = strtok(NULL," \0");
                }
                free(cp);
                cp = copyString(last);
                cp1 = strtok(cp," ");
                while(cp1 != NULL){
                    nWordsL++;
                    cp1 = strtok(NULL," \0");
                }
                free(cp);
                if(nWtxt == (nWordsF+nWordsL)){
                    // SAVE WORDS IN A *CHAR ARRAY //
                    words = (char**)malloc((nWordsF+nWordsL)*sizeof(char*));
                    cp = copyString(first);
                    cp1 = strtok(cp," ");
                    while(cp1 != NULL){
                        words[j] = copyString(cp1);
                        j++;
                        cp1 = strtok(NULL," \0");
                    }
                    free(cp);
                    cp = copyString(last+1);
                    cp1 = strtok(cp," ");
                    while(cp1 != NULL){
                        words[j] = copyString(cp1);
                        j++;
                        cp1 = strtok(NULL," \0");
                    }
                    free(cp);
                    words[j-1][strlen(words[j-1])-1] = '\0';
                    //--------------//

                    // COMPARE EACH STRING FROM WORDS AND EACH STRING FROM WORDSTXT //
                    matchCount = superStringCompare(words,wordstxt,(nWordsF+nWordsL));
                    //--------------//
                }

                if(matchCount == (nWordsF+nWordsL)){
                    while(k < j){
                        free(words[k]);
                        k++;
                    }
                    free(tmpString);
                    free(tmpString2);
                    found = 1;
                    nFound++;
                    break;
                } else {
                    while(k < j){
                        free(words[k]);
                        k++;
                    }

                    free(tmpString);
                    free(tmpString2);
                }
            }
            i++;
        }
        k=0;
        if(found == 0){
            printf("|");
            setColor(12);
            printf(" (X) ");
            while(k<j2){
                printf("%s ", wordstxt[k]);
                k++;
            }
            printf("\n");
            setColor(15);
        }

        k=0;
        while(k < j2){
            free(wordstxt[k]);
            k++;
        }

        fclose(csv);
        tmp = tmp->next;
    }
    //------------//
    return nFound;
}

int computeNrows(char *fileName){
    FILE *input = fopen(fileName,"r");
    int i = 0;
    char buffer[1024];
    if(input == NULL){
        setColor(12);
        printf(" ERROR : COULD NOT FIND FILE 'Actual_Attendees.txt'\n");
        setColor(15);
        exit(EXIT_FAILURE);
    }
    while(fgets(buffer,1024,input) != NULL){
        i++;
    }
    fclose(input);
    return i;
}

int superStringCompare(char **words1, char **words2, int size){
    int matchCount = 0, i=0, j=0;
    char *string1, *string2;
    for(i=0; i<size; i++){
        string1 = copyString(words1[i]);
        tolowerString(string1);
        for(j=0; j<size; j++){
            string2 = copyString(words2[j]);
            tolowerString(string2);
            if(strcmp(string1,string2) == 0){
                matchCount++;
                break;
            }
        }
    }
    return matchCount;
}

void checkList3(s_data *list1, s_data*list2){
    int nFound = 0, nWordsF=0, nWordsL=0, j=0, j2,k=0, nWtxt = 0, matchCount=0, found=0, notAss=0, i=0;
    s_data *tmp, *tmp1;
    char **words, **wordstxt, *notAssigned[200], tmpString[1024];
    char *first, *last , *cp, *cp1;

    tmp = list1;
    while(tmp != NULL){
        found = 0;
        nWtxt = 0;
        tmp1 = list2;
        k=0;
        j=0;
        matchCount = 0;
        cp = copyString(tmp->surname);
        cp1 = strtok(cp," ");
        while(cp1 != NULL){
            nWtxt++;
            cp1 = strtok(NULL," \0");
        }
        free(cp);
        cp = copyString(tmp->name);
        cp1 = strtok(cp," ");
        while(cp1 != NULL){
            nWtxt++;
            cp1 = strtok(NULL," \0");
        }
        free(cp);
        // SAVE WORDS INTO A *CHAR ARRAY //
        wordstxt = (char**)malloc((nWordsF+nWordsL)*sizeof(char*));
        cp = copyString(tmp->surname);
        cp1 = strtok(cp," ");
        while(cp1 != NULL){
            wordstxt[j] = copyString(cp1);
            j++;
            cp1 = strtok(NULL," \0");
        }
        free(cp);
        cp = copyString(tmp->name);
        cp1 = strtok(cp," ");
        while(cp1 != NULL){
            wordstxt[j] = copyString(cp1);
            j++;
            cp1 = strtok(NULL," \0");
        }
        free(cp);
        j2 = j;
        while(tmp1 != NULL){

            nWordsF = 0;
            nWordsL = 0;
            matchCount = 0;
            j = 0;
            k = 0;
            last = copyString(tmp1->name);
            first = copyString(tmp1->surname);
            // CHECK THE NUMBER OF WORDS IN SURNAME-NAME //
            cp = copyString(first);
            cp1 = strtok(cp," ");
            while(cp1 != NULL){
                nWordsF++;
                cp1 = strtok(NULL," \0");
            }
            free(cp);
            cp = copyString(last);
            cp1 = strtok(cp," ");
            while(cp1 != NULL){
                nWordsL++;
                cp1 = strtok(NULL," \0");
            }
            free(cp);
            if(nWtxt == (nWordsF+nWordsL)){
                // SAVE WORDS IN A *CHAR ARRAY //
                words = (char**)malloc((nWordsF+nWordsL)*sizeof(char*));
                cp = copyString(first);
                cp1 = strtok(cp," ");
                while(cp1 != NULL){
                    words[j] = copyString(cp1);
                    j++;
                    cp1 = strtok(NULL," \0");
                }
                free(cp);
                cp = copyString(last);
                cp1 = strtok(cp," ");
                while(cp1 != NULL){
                    words[j] = copyString(cp1);
                    j++;
                    cp1 = strtok(NULL," \0");
                }
                free(cp);
                //--------------//

                // COMPARE EACH STRING FROM WORDS AND EACH STRING FROM WORDSTXT //
                matchCount = superStringCompare(words,wordstxt,(nWordsF+nWordsL));
                //--------------//
            }

            if(matchCount == (nWordsF+nWordsL)){
                while(k < j){
                    free(words[k]);
                    k++;
                }
                free(first);
                free(last);
                found = 1;
                nFound++;
                break;
            } else {
                while(k < j){
                    free(words[k]);
                    k++;
                }

                free(first);
                free(last);
            }

            tmp1 = tmp1->next;
        }
        k=0;
        if(found == 0){
            notAss++;
            memset(tmpString,0,strlen(tmpString));
            while(k < j2){
                sprintf(tmpString,"%s %s", tmpString, wordstxt[k]);
                k++;
            }
            notAssigned[notAss-1] = copyString(tmpString);
            memset(tmpString,0,strlen(tmpString));
            /*k = 0;
                printf(" (X) NF : ");
                while(k<j2){
                    printf("%s ", wordstxt[k]);
                    k++;
                }*/
        }

        k=0;
        while(k < j2){
            free(wordstxt[k]);
            k++;
        }

        tmp = tmp->next;
    }
    printf("�����������������������������������������������������������������������������������������������������������������������������������\n");
    printf("\n");
    if(notAss != 0){
        printf(">> Ooops! The program could not execute the proper operations for these entries : \n");
        printf("\n");
        for(i = 0; i<notAss; i++){
            printf("|");
            setColor(12);
            printf(" (X) %s\n", notAssigned[i]);
            setColor(15);
        }
        printf("\n");
        printf("\n");
        setColor(10);
        printf("-- > OPERATION COMPLETED : NUMBER OF UNSOLVED ENTRIES : {%d} \n", notAss);
        setColor(15);
        printf("\n");
        printf("�����������������������������������������������������������������������������������������������������������������������������������\n");
    } else {
        printf("\n");
        setColor(10);
        printf("-- > OPERATION SUCCESSFULL :  100%% OF THE ENTRIES HAVE BEEN PROCESSED CORRECTLY \n");
        setColor(15);
        printf("�����������������������������������������������������������������������������������������������������������������������������������\n");
        printf("\n");
    }
    //------------//
}

void setColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}
