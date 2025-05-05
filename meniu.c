#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct cont
{
    char nume[100],label[50],email[50];
    char nrtel[11];
    int block;
}cont;
cont telefon[200];
typedef struct numar
{
    int blocate[100];
}numere;
numere SIR;
int nblocate;
int numerecontacte;
void meniu()
{
    printf("--=Sistem de contacte=--\n");
    printf("\n");
    printf("1.Afisare contacte actuale\n");
    printf("2.Adaugarea de noi contacte\n");
    printf("3.Stergerea unui contact existent\n");
    printf("4.Blocarea numerelor nedorite\n");
    printf("5.Deblocarea numerelor blocate\n");
    printf("6.Cautare\n");
    printf("0.Exit\n");
}
int estemail(char *email)
{
    int at=-1,punct=-1;
    for (int i=0;i<strlen(email);i++)
    {
        if (email[i]==' ')
            return 0;
        if (email[i]=='@')
        {
            if (at!=-1) return 0;
            at=i;
        }
        else if (email[i] =='.' && at!=-1)
        {
            punct=i;
        }
    }
    if (at<1 || punct<at+2 || punct==strlen(email)-1)
        return 0;
    return 1;
}
void citire_fisier()
{
    FILE * contact_file=fopen("contacte.txt","r");
    if(contact_file!=NULL)
    {
        char linie[300];
        fgets(linie,300,contact_file);
        numerecontacte=atoi(linie);
        for(int i=0;i<numerecontacte;i++)
        {
            fgets(linie,300,contact_file);
            strcpy(telefon[i].nume,linie);
            telefon[i].nume[strlen(telefon[i].nume)-1]='\0';
            fgets(linie,300,contact_file);
            strcpy(telefon[i].nrtel,linie);
            telefon[i].nrtel[strlen(telefon[i].nrtel)-1]='\0';
            fgets(linie,300,contact_file);
            strcpy(telefon[i].label,linie);
            telefon[i].label[strlen(telefon[i].label)-1]='\0';
            fgets(linie,300,contact_file);
            strcpy(telefon[i].email,linie);
            telefon[i].email[strlen(telefon[i].email)-1]='\0';
            fgets(linie,300,contact_file);
            telefon[i].block=atoi(linie);
        }
    }
    fclose(contact_file); 
    FILE * blocat_file=fopen("nrblocate.txt","r");
    if(blocat_file!=NULL)
    {
        char linie[300];
        fgets(linie,300,contact_file);
        nblocate=atoi(linie);
        for(int i=0;i<nblocate;i++)
        {
            fgets(linie,300,blocat_file);
            SIR.blocate[i]=atoi(linie);
        }
    }
}
void contacte()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes; 
    for(int i=0;i<numerecontacte;i++)
        {
            printf("%d. ",i+1);
            SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("%s ",telefon[i].nume);
            SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
            printf("%s ",telefon[i].nrtel);
            SetConsoleTextAttribute(hConsole, saved_attributes);
            printf("(");
            SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_RED);
            printf("%s",telefon[i].label);
            SetConsoleTextAttribute(hConsole, saved_attributes);
            printf(") ");
            SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_BLUE);
            printf("%s ",telefon[i].email);
            if(telefon[i].block)
            {
                SetConsoleTextAttribute(hConsole, saved_attributes);
                printf("(");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                printf("blocat");
                SetConsoleTextAttribute(hConsole, saved_attributes);
                printf(")");
            }
            printf("\n");
            SetConsoleTextAttribute(hConsole, saved_attributes);
        }
    printf("0.Exit\n");
}
int unicnr(char *nr)
{
    for(int i=0;i<numerecontacte;i++)
    {
        if(atoi(nr)==atoi(telefon[i].nrtel))
        {
            return 0;
        }
    }
    return 1;
}
void adaugare_contacte()
{
    system("@cls");
    printf("--=Adaugati un contact=--\n");
    printf("1.Adauga contact\n");
    printf("0.Exit\n");
    int i;
    scanf("%d",&i);
    if(i==1)
    {
        FILE *contact_file = fopen("contacte.txt", "w");
        cont *nou=(cont *)malloc(sizeof(cont));
        printf("Introduceti numarul de telefon al contactului: ");
        scanf("%s", nou->nrtel);
        while(strlen(nou->nrtel) != 10 || strncmp(nou->nrtel, "07", 2) != 0 || !unicnr(nou->nrtel))
        {
            if(!unicnr(nou->nrtel))
                printf("Numarul acesta il aveti deja la un contact.\n");
            else
            {
                printf("Numarul de telefon trebuie sa inceapa cu 07 si sa aiba 10 cifre\n");
            }
            printf("Introduceti numarul de telefon al contactului:");
            scanf("%s", nou->nrtel);
        }
        getchar();
        printf("Introduceti numele contactului: ");
        fgets(nou->nume,100,stdin);
        printf("Introduceti labelul contactului: ");
        fgets(nou->label,50,stdin);
        printf("Introduceti emailul contactului: ");
        fgets(nou->email,50,stdin);
        nou->nume[strlen(nou->nume)-1]='\0';
        nou->label[strlen(nou->label)-1]='\0';
        nou->email[strlen(nou->email)-1]='\0';
        while(!estemail(nou->email))
        {
            printf("Emailul nu este valid. Trebuie sa fie <utilizator>@<domeniu>.<extensie>\n");
            printf("Introduceti emailul contactului: ");
            fgets(nou->email,50,stdin);
            nou->email[strlen(nou->email)-1]='\0';
        }
        numerecontacte++;
        fprintf(contact_file,"%d\n",numerecontacte);
        for(int i=0;i<numerecontacte-1;i++)
        {
            fprintf(contact_file,"%s\n%s\n%s\n%s\n%d\n",telefon[i].nume,telefon[i].nrtel,telefon[i].label,telefon[i].email,telefon[i].block);
        }
        nou->block=0;
        for(int i=0;i<nblocate;i++)
        {
            if(atoi(nou->nrtel)==SIR.blocate[i])
            {
                nou->block=1;
            }
        }
        if(nou->block==1)
        {
            FILE *blocate_file = fopen("nrblocate.txt", "w");
            fprintf(blocate_file,"%d\n",nblocate-1);
            for(int i=0;i<nblocate;i++)
            {
                if(SIR.blocate[i]!=atoi(nou->nrtel))
                {
                    fprintf(blocate_file,"%d\n",SIR.blocate[i]);
                }
            }
            fclose(blocate_file);
            nblocate--;
        }
        fprintf(contact_file,"%s\n%s\n%s\n%s\n%d",nou->nume,nou->nrtel,nou->label,nou->email,nou->block);
        free(nou);
        printf("Contactul a fost adaugat!\n");
        fclose(contact_file);
    }
    scanf("%d",&i);
}
int cautare(int a)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes; 
    int ok=0;
    if(a==1)
    {
        char b[100];
        printf("Introduceti numele contactului:\n");
        getchar();
        fgets(b,100,stdin);
        b[strlen(b)-1]='\0';
        for(int i=0;i<numerecontacte;i++)
        {
            if(strstr(telefon[i].nume,b)!=NULL)
            {
                ok=1;
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("%s ",telefon[i].nume);
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
                printf("%s ",telefon[i].nrtel);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                printf("(");
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_RED);
                printf("%s",telefon[i].label);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                printf(") ");
                SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_BLUE);
                printf("%s ",telefon[i].email);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                if(telefon[i].block)
                {
                    printf("(");
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    printf("blocat");
                    SetConsoleTextAttribute(hConsole, saved_attributes);
                    printf(") ");
                } 
                printf("\n");
            }
        }
    }
    if(a==2)
    {
        char b[100];
        printf("Introduceti numarul contactului:\n");
        scanf("%s", b);
        while(strlen(b) != 10 || strncmp(b, "07", 2) != 0)
        {
            printf("Numarul de telefon trebuie sa inceapa cu 07 si sa aiba 10 cifre\n");
            printf("Introduceti numarul de telefon:");
            scanf("%s", b);
        }
        for(int i=0;i<numerecontacte;i++)
        {
            if(atoi(b)==atoi(telefon[i].nrtel))
            {
                ok=1;
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("%s ",telefon[i].nume);
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
                printf("%s ",telefon[i].nrtel);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                printf("(");
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_RED);
                printf("%s",telefon[i].label);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                printf(") ");
                SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_BLUE);
                printf("%s ",telefon[i].email);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                if(telefon[i].block)
                {
                    printf("(");
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    printf("blocat");
                    SetConsoleTextAttribute(hConsole, saved_attributes);
                    printf(") ");
                } 
                printf("\n");
            }
        }
    }
    if(a==3)
    {
        char b[100];
        printf("Introduceti labelul contactului\n");
        scanf("%s",b);
        for(int i=0;i<numerecontacte;i++)
        {
            if(strcmp(b,telefon[i].label)==0)
            {
                ok=1;
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("%s ",telefon[i].nume);
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
                printf("%s ",telefon[i].nrtel);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                printf("(");
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_RED);
                printf("%s",telefon[i].label);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                printf(") ");
                SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_BLUE);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                printf("%s ",telefon[i].email);
                if(telefon[i].block)
                {
                    printf("(");
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    printf("blocat");
                    SetConsoleTextAttribute(hConsole, saved_attributes);
                    printf(") ");
                } 
                printf("\n");
            }
        }
    }
    if(a==4)
    {
        char b[100];
        printf("Introduceti emailul contactului\n");
        scanf("%s",b);
        for(int i=0;i<numerecontacte;i++)
        {
            if(strcmp(b,telefon[i].email)==0)
            {
                ok=1;
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("%s ",telefon[i].nume);
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
                printf("%s ",telefon[i].nrtel);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                printf("(");
                SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_RED);
                printf("%s",telefon[i].label);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                printf(") ");
                SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_BLUE);
                printf("%s ",telefon[i].email);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                if(telefon[i].block)
                {
                    printf("(");
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    printf("blocat");
                    SetConsoleTextAttribute(hConsole, saved_attributes);
                    printf(") ");
                } 
                printf("\n");
            }
        }
    }
    if(ok==0)
    {
        printf("Nu a fost gasit niciun contact.\n");
    }
    char b[100];
    scanf("%s",b);
}
void stergere()
{
    char a[100];
    system("@cls");
    printf("--=Stergeti un contact=--\n");
    printf("\n");
    printf("1.Stergeti\n");
    printf("0.Exit\n");
    scanf("%s", a);
    if(atoi(a)!=0)
    {
        printf("Introduceti numarul de telefon care doriti sa il stergeti:\n");
        scanf("%s", a);
        while(strlen(a) != 10 || strncmp(a, "07", 2) != 0)
        {
            printf("Numarul de telefon trebuie sa inceapa cu 07 si sa aiba 10 cifre\n");
            printf("Introduceti numarul de telefon:");
            scanf("%s", a);
        }
        int ok=0;
        for(int i=0;i<numerecontacte;i++)
        {
            if(atoi(telefon[i].nrtel)==atoi(a))
            {
                ok=1;
            }
        }
        if(ok)
        {
            FILE *contact_file = fopen("contacte.txt", "w");
            fprintf(contact_file,"%d\n",numerecontacte-1);
            for(int i=0;i<numerecontacte;i++)
            {
                if(atoi(telefon[i].nrtel)!=atoi(a))
                {
                    fprintf(contact_file,"%s\n%s\n%s\n%s\n%d\n",telefon[i].nume,telefon[i].nrtel,telefon[i].label,telefon[i].email,telefon[i].block);
                }
            }
            fclose(contact_file);
            numerecontacte--;
            printf("Contactul a fost sters!\n");
        }
        else
            printf("Nu a fost gasit contactu.\n");
        scanf("%s", a);
    }
}
void blocare(char nr[])
{
    int ok=0;
    FILE *contact_file = fopen("contacte.txt", "w");
    fprintf(contact_file,"%d\n",numerecontacte);
    for(int i=0;i<numerecontacte;i++)
    {
        if(strcmp(telefon[i].nrtel,nr)==0)
        {
            ok=1;
            fprintf(contact_file,"%s\n%s\n%s\n%s\n%d\n",telefon[i].nume,telefon[i].nrtel,telefon[i].label,telefon[i].email,1);
        }
        else
            fprintf(contact_file,"%s\n%s\n%s\n%s\n%d\n",telefon[i].nume,telefon[i].nrtel,telefon[i].label,telefon[i].email,telefon[i].block);
    }
    if(ok!=1)
    {
        SIR.blocate[nblocate]=atoi(nr);
        nblocate++;
        FILE *blocat_file=fopen("nrblocate.txt","w");
        fprintf(blocat_file,"%d\n",nblocate);
        for(int i=0;i<nblocate;i++)
        {
            fprintf(blocat_file,"%d\n",SIR.blocate[i]);
        }
        fclose(blocat_file);
    }
    fclose(contact_file);
    printf("Numarul a fost blocat!\n");
}
void deblocare(char nr[])
{
    int ok=0;
    FILE *contact_file = fopen("contacte.txt", "w");
    fprintf(contact_file,"%d\n",numerecontacte);
    for(int i=0;i<numerecontacte;i++)
    {
        if(strcmp(telefon[i].nrtel,nr)==0)
        {
            ok=1;
            fprintf(contact_file,"%s\n%s\n%s\n%s\n%d\n",telefon[i].nume,telefon[i].nrtel,telefon[i].label,telefon[i].email,0);
        }
        else
            fprintf(contact_file,"%s\n%s\n%s\n%s\n%d\n",telefon[i].nume,telefon[i].nrtel,telefon[i].label,telefon[i].email,telefon[i].block);
    }
    if(ok!=1)
    {
        FILE *blocat_file=fopen("nrblocate.txt","w");
        if(blocat_file!=NULL)
        {
            fprintf(blocat_file,"%d\n",nblocate-1);
            for(int i=0;i<nblocate;i++)
            {
                if(atoi(nr)!=SIR.blocate[i])
                {
                    fprintf(blocat_file,"%d\n",SIR.blocate[i]);
                }
                else
                    ok=1;
            }
        }
        fclose(blocat_file);
    }
    if(ok!=1)
    {
        printf("Nu a fost gasit un numar\n");
    }
    else
    {
        printf("Numarul a fost deblocat!\n");
    }
    fclose(contact_file);
   
}
int main()
{
    
    int ok=1;
    while(ok)
    {
        system("@cls");
        citire_fisier();
        meniu();
         int c;
        printf("Introduceti comanda:"); 
        scanf("%d",&c);
        while(c<0 || c>6)
        {
          
            if(c<1 || c>6)
            {
                printf("Introduceti un numar din interval\n");
            } 
            scanf("%d",&c);
        }
        char b[100];
        int a;
        switch (c)
        {
        case 0:
            ok=0;
            break;
        case 1:
            system("@cls");
            printf("--=Contacte actuale=--\n");
            printf("\n");
            contacte();
            scanf("%d",&a);
            break;
        case 2:
            adaugare_contacte();
            break;
        case 3:
            stergere();
            break;
        case 4:
            system("@cls");
            printf("--=Blocati numarul de telefon nedorit=--\n");
            printf("0.Exit\n");
            printf("Introduceti numarul de telefon: ");
            scanf("%s", b);
            if(atoi(b)!=0)
            {
                while(strlen(b) != 10 || strncmp(b, "07", 2) != 0)
                {
                    printf("%s\n",b);
                    printf("%d\n",strlen(b));
                    printf("Numarul de telefon trebuie sa inceapa cu 07 si sa aiba 10 cifre\n");
                    printf("Introduceti numarul de telefon:");
                    scanf("%s", b);
                }
                blocare(b);
                printf("Contactul a fost blocat\n");
            }
            scanf("%d",&a);
            break;
        case 5:
            system("@cls");
            printf("--=Deblocati un numar de telefon=--\n");
            printf("0.Exit\n");
            printf("Introduceti numarul de telefon: ");
            scanf("%s", b);
            if(atoi(b)!=0)
            {
                while(strlen(b) != 10 || strncmp(b, "07", 2) != 0)
                {
                    printf("%s\n",b);
                    printf("%d\n",strlen(b));
                    printf("Numarul de telefon trebuie sa inceapa cu 07 si sa aiba 10 cifre\n");
                    printf("Introduceti numarul de telefon:");
                    scanf("%s", b);
                }
                deblocare(b);
            }
            scanf("%d",&a);
            break;
        case 6:
            system("@cls");
            printf("--=Cautati un contact=--\n");
            printf("\n");
            printf("1.Dupa nume\n");
            printf("2.Dupa numar de telefon\n");
            printf("3.Dupa label\n");
            printf("4.Dupa email\n");
            printf("0.Exit\n");
            scanf("%d",&a);
            while(a<0 || a>4)
            {
              
                if(a<0 || a>4)
                {
                    printf("Introduceti un numar din interval\n");
                } 
                scanf("%d",&a);
            }
            if(a>0)
            {
                cautare(a);
            }
            break;
        }
    }
    return 0;
}