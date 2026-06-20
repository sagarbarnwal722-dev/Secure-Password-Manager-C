#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include<wincrypt.h>
#ifndef CALG_SHA_256
#define CALG_SHA_256 0x0000800c
#endif
#include<conio.h>

# define KEY 0x54
#define MAX 50

// ---------------- STRUCT ----------------
struct password {
    char site[MAX];
    char username[MAX];
    char password[MAX];
    char created_date[20];
};

// ---------------- FUNCTION DECLARATIONS ----------------
void generate_strong_password(char password[], int length);
int check_password(char str[]);
void encrypt(char str[]);
void decrypt(char str[]);
void add_password();
void view_passwords();
void search_passwords();
void update_passwords();
void delete_passwords();
void toggle_password(char password[],const char *prompt);
int login();
int is_password_expired(char date[]);
void backup_data();
void restore_backup();
void countdown(int seconds);
void shuffle_password(char password[]);
void sha256(const char *str,char output[65]);
void log_activity(char action[]);
void check_expired_password();

// ---------------- PASSWORD VALIDATION ----------------

int check_password(char str[]) {
    int len = strlen(str), cap = 0, digit = 0, special = 0,lower=0;

    char sp[] = "!@#$%^&*()_+-=[]{}|;:'\",.<>?/`~";

    for(int i = 0; str[i]; i++) {
        if(isupper(str[i])) cap++;
        else if(isdigit(str[i])) digit++;
        else if(islower(str[i])) lower++;

        for(int j = 0; sp[j]; j++) {
            if(str[i] == sp[j]) special++;
        }
    }

    if(len >= 8 && cap >= 1 && digit >= 2 && special >= 1 && lower >=1) {
        printf("Strong Password\n");
        return 1;
    }

    printf("Weak Password\n");
    return 0;
}

// ---------------- ENCRYPTION ----------------

void encrypt(char str[]) {
    for(int i = 0; str[i]; i++) str[i] += 3;
}
// ---------------- DECRYPTION ----------------

void decrypt(char str[]) {
    for(int i = 0; str[i]; i++) str[i] -= 3;
}

// ---------------- log activity ----------------
void log_activity(char action[])
{
    FILE *fp=fopen("activity_log.txt","a");
    if(fp == NULL)
        return;
    time_t t=time(NULL);
    struct tm *tm_info=localtime(&t);
    char buffer[30];
    strftime(buffer,30,"%d-%m-%y %H:%M:%S",tm_info);
    fprintf(fp,"[%s] %s\n",buffer, action);
    fclose(fp);
}
// ---------------- SHA256 ----------------

void sha256(const char *str,char output[65])
{
    HCRYPTPROV hProv=0;
    HCRYPTHASH hHash=0;
    BYTE hash[32];
    DWORD hashLen=32;
    if(!CryptAcquireContext(&hProv,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT))
    return;
    if(!CryptCreateHash(hProv,CALG_SHA_256, 0, 0, &hHash))
    {
        CryptReleaseContext(hProv,0);
        return;
    }
    CryptHashData(hHash,(BYTE*)str,strlen(str),0);
    CryptGetHashParam(hHash,HP_HASHVAL,hash,&hashLen,0);
    for(int i=0;i<32;i++)
    {
        sprintf(output + (i*2),"%02x",hash[i]);
    }
    output[64]='\0';
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv,0);
}

// ---------------- Backup Data --------------------

void backup_data()
{
    FILE *source,*backup;
    int ch;

    source=fopen("password.dat","rb");

    if(source==NULL)
    {
        printf("No data file found!\n");
        return;
    }

    backup=fopen("backup.dat","wb");

    if(backup==NULL)
    {
        printf("Backup creation failed!\n");
        fclose(source);
        return;
    }

    while((ch=fgetc(source))!=EOF)
    {
        fputc(ch,backup);
    }

    fclose(source);
    fclose(backup);

    printf("Backup Created Successfully!\n");
}// ---------------- Restore Function ---------------
void restore_backup()
{
    FILE *backup,*mainfile;
    int ch;
    backup=fopen("backup.dat","rb");
    if(backup == NULL)
    {
        printf("No backup found!\n");
        return;
    }
    mainfile=fopen("password.dat","wb");
    while((ch=fgetc(backup))!=EOF)
    {
        fputc(ch,mainfile);
    }
    fclose(backup);
    fclose(mainfile);
    printf("Restore completed successfully!\n");
}
// ---------------- TOGGLE PASSWORD ----------------
void toggle_password(char password[], const char *prompt)
{
    int i = 0;
    char ch;
    int visible = 0;

    printf("%s", prompt);
    fflush(stdout);

    while(1)
    {
        ch = getch();

        // Enter key
        if(ch == 13)
        {
            password[i] = '\0';
            printf("\n");
            break;
        }

        // Backspace key
        else if(ch == 8)
        {
            if(i > 0)
            {
                i--;

                // Redraw entire line
                printf("\r%s", prompt);

                for(int j = 0; j < i; j++)
                {
                    printf("%c", visible ? password[j] : '*');
                }

                printf("  "); // erase leftover chars
                printf("\r%s", prompt);

                for(int j = 0; j < i; j++)
                {
                    printf("%c", visible ? password[j] : '*');
                }

                fflush(stdout);
            }
        }

        // TAB key -> Show / Hide password
        else if(ch == 9)
        {
            visible = !visible;

            // Redraw line
            printf("\r%s", prompt);

            for(int j = 0; j < i; j++)
            {
                printf("%c", visible ? password[j] : '*');
            }

            fflush(stdout);
        }

        // Normal characters
        else if(isprint(ch))
        {
            if(i < MAX - 1)
            {
                password[i++] = ch;

                if(visible)
                    printf("%c", ch);
                else
                    printf("*");

                fflush(stdout);
            }
        }
    }
}
// ---------------- MASTER PASSWORD ----------------

int master_exist() {
    FILE *fp = fopen("Master_password.dat", "rb");
    if(fp == NULL) return 0;
    fclose(fp);
    return 1;
}

void set_master_password()
{
    FILE *fp;
    char pass[MAX];
    char hash[65];

    toggle_password(pass, "Set Master Password:");

    sha256(pass, hash);

    fp = fopen("Master_password.dat", "wb");

    if(fp == NULL)
    {
        printf("Error creating master password file!\n");
        return;
    }

    fwrite(hash, sizeof(char), 65, fp);

    fclose(fp);

    printf("Master password set!\n");
}
// ---------------- COUNTDOWN ----------------

void countdown(int sec) {
    for(int i = sec; i > 0; i--) {
        //printf("Retry in: %2d sec  \r", i);
        printf("\rRetry in: %2d sec", i);
        fflush(stdout);
        Sleep(1000);
    }
    
    printf("\r                    \r");
    fflush(stdout);
}

// ---------------- LOGIN ----------------

int login() {
    FILE *fp;
    char stored[65], input[MAX];

    fp = fopen("Master_password.dat", "rb");
    if(fp == NULL) {
        printf("Error reading master password\n");
        return 0;
    }

    fread(stored,sizeof(char),65,fp);
    stored[64]='\0';
    fclose(fp);

    while(1) {
        int attempt = 3;
        char input_hash[65];

        while(attempt--) {
            toggle_password(input,"Enter Master Password:");

            sha256(input, input_hash);

            if(strcmp(input_hash, stored) == 0) {
                printf("Access Granted\n");
                log_activity("Successful login");
                check_expired_password();
                return 1;
            }

            printf("Wrong password! Attempts left: %d\n", attempt);
            log_activity("Failed Login Attempt");
        }

        printf("Locked for 10 seconds...\n");
        countdown(10);
    }
}

// ---------------- SHUFFLE PASSWORD FUNCTION ----------------

void shuffle_password(char password[])
{
    int len=strlen(password);
    for(int i=len-1;i>0;i--)
    {
        int j=rand() % (i+1);
        char temp=password[i];
        password[i]=password[j];
        password[j]=temp;
    }
}

// ---------------- Expiry checker --------------------
int is_password_expired(char date[])
{
    int day,month,year;
    sscanf(date,"%d-%d-%d",&day,&month,&year);
    struct tm created={0};
    created.tm_mday=day;
    created.tm_mon=month - 1;
    created.tm_year=year-1900;
    time_t created_time=mktime(&created);
    time_t current_time=time(NULL);
    double diff_days=difftime(current_time,created_time)/(60*60*24);
    return diff_days >=90;
}
// ---------------- Expired Password -----------------
void check_expired_password()
{
    struct password p;
    int found = 0;

    FILE *fp = fopen("password.dat","rb");

    if(fp == NULL)
        return;

    while(fread(&p,sizeof(struct password),1,fp) == 1)
    {
        if(is_password_expired(p.created_date))
        {
            if(found == 0)
            {
                printf("\n========== PASSWORD EXPIRY ALERT ==========\n");
            }

            printf("%s password expired!\n", p.site);
            found = 1;
        }
    }

    fclose(fp);
}
// ---------------- PASSWORD GENERATOR ----------------

void generate_strong_password(char password[], int length) {
    char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char lower[] = "abcdefghijklmnopqrstuvwxyz";
    char digits[] = "0123456789";
    char special[] = "!@#$%^&*";
    char all[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*";

    int i = 0;

    password[i++] = upper[rand()%26];
    password[i++] = lower[rand()%26];
    password[i++] = digits[rand()%10];
    password[i++] = digits[rand()%10];
    password[i++] = special[rand()%8];

    for(; i < length; i++) {
        password[i] = all[rand() % (sizeof(all)-1)];
    }

    password[length] = '\0';
    shuffle_password(password);
}

// ---------------- ADD PASSWORD ----------------

void add_password()
{
    struct password p;
    FILE *fp;
    int choice;

    printf("Site: ");
    scanf("%49s", p.site);

    printf("Username: ");
    scanf("%49s", p.username);

    while(1)
    {
        generate_strong_password(p.password, 10);

        printf("\nSuggested Password: %s\n", p.password);
        printf("1. Use Suggested Password\n");
        printf("2. Generate New Password\n");
        printf("3. Enter Password Manually\n");
        printf("Choice: ");

        if(scanf("%d", &choice) != 1)
        {
            while(getchar() != '\n');   // clear invalid input
            printf("Invalid input! Please enter 1, 2 or 3.\n");
            continue;
        }

        if(choice == 1)
        {
            printf("Suggested password selected.\n");
            break;
        }
        else if(choice == 2)
        {
            printf("Generating another password...\n");
            continue;
        }
        else if(choice == 3)
        {
            while(1)
            {
                toggle_password(
                    p.password,
                    "Enter Password (TAB to show/hide): "
                );

                if(check_password(p.password))
                {
                    break;
                }

                printf("Weak Password! Try Again.\n");
            }
            break;
        }
        else
        {
            printf("Please enter only 1, 2 or 3.\n");
        }
    }

    encrypt(p.password);

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    strftime(
        p.created_date,
        sizeof(p.created_date),
        "%d-%m-%Y",
        tm_info
    );

    fp = fopen("password.dat", "ab");

    if(fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fwrite(&p, sizeof(struct password), 1, fp);
    fclose(fp);

    printf("Password saved successfully!\n");

    backup_data();

    char logmsg[100];
    sprintf(logmsg, "Added Password for %s", p.site);
    log_activity(logmsg);
}
// ---------------- VIEW ----------------

void view_passwords()
{
    struct password p;

    FILE *fp = fopen("password.dat", "rb");

    if(fp == NULL)
    {
        printf("No data.\n");
        return;
    }

    while(fread(&p, sizeof(struct password), 1, fp) == 1)
    {
        decrypt(p.password);

        printf("%s | %s | %s\n",
               p.site,
               p.username,
               p.password);

        if(is_password_expired(p.created_date))
        {
            printf("WARNING: Password older than 90 days!\n");
        }
    }

    fclose(fp);
}
// ---------------- SEARCH ----------------

void search_passwords() {
    struct password p;
    FILE *fp = fopen("password.dat", "rb");
    char site[MAX];
    int found = 0;
    char logmsg[100];

    if(fp == NULL) return;

    printf("Search site: ");
    scanf("%49s", site);

    while(fread(&p, sizeof(struct password), 1, fp) == 1) {
        if(strcmp(p.site, site) == 0) {
            sprintf(logmsg,"Searched Password for %s",p.site);
            decrypt(p.password);
            printf("Found -> %s | %s\n", p.username, p.password);
            if(is_password_expired(p.created_date))
            {
                printf("Warning: Password older than 90 days!\n");
            }
            log_activity(logmsg);
            found = 1;
        }
    }

    if(!found) printf("Not found\n");

    fclose(fp);
}

// ---------------- Update ----------------
void update_passwords()
{
    FILE *fp,*temp;
    struct password p;
    char site[MAX];
    int found=0;
    char logmsg[100];
    fp=fopen("password.dat","rb");
    temp=fopen("temp.dat","wb");
    if(fp==NULL || temp==NULL)
    {
        printf("Error opening the file.\n");
        return;
    }
    printf("Enter the site to update:");
    scanf("%49s", site);

    while(fread(&p,sizeof(struct password),1,fp) == 1)
    {
        if(strcmp(p.site,site)==0)
        {
            found=1;
            printf("\nRecord Found!\n");
            printf("Username: %s\n",p.username);
            char ch;
            printf("Show old password?(y/n):");
            scanf(" %c",&ch);
            if(ch=='Y' || ch=='y')
            {
                char temp_pass[MAX];
                strcpy(temp_pass,p.password);

                decrypt(temp_pass);
                printf("Old Password: %s\n",temp_pass);

            }
            char confirm;
            printf("Update password(y/n):");
            scanf(" %c",&confirm);
            if(confirm =='n'||confirm=='N')
            {
                fwrite(&p,sizeof(struct password),1,temp);
                continue;
            }
            
            while(1)
            {
                toggle_password(p.password,"Enter New Password(TAB to show/hide):");
                if(check_password(p.password))
                {
                    break;
                }
                printf("Weak password! Try again.\n");
            }
            time_t t=time(NULL);
            struct tm *tm_info=localtime(&t);
            strftime(p.created_date,sizeof(p.created_date),"%d-%m-%Y",tm_info);
            encrypt(p.password);
            backup_data();
            printf("Password updated successfully!\n");
            sprintf(logmsg,"Updated Password for %s",p.site);
            log_activity(logmsg);
        }
        fwrite(&p,sizeof(struct password),1,temp);
    }
    fclose(fp);
    fclose(temp);
    remove("password.dat");
    rename("temp.dat","password.dat");
    
    
    if(!found)
    {
        printf("No matching record found.\n");
    }


}
// ---------------- DELETE ----------------

void delete_passwords() {
    struct password p;
    FILE *fp = fopen("password.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    char site[MAX];
    int found = 0;
    char logmsg[100];

    if(fp == NULL || temp == NULL) return;

    printf("Delete site: ");
    scanf("%49s", site);

    while(fread(&p,sizeof(struct password),1,fp) == 1) {
        if(strcmp(p.site, site) == 0) {
            found = 1;
            continue;
        }
        fwrite(&p,sizeof(struct password),1,temp);
    }

    fclose(fp);
    fclose(temp);
    backup_data();

    remove("password.dat");
    rename("temp.dat", "password.dat");

    if(found) 
    {

        printf("Deleted!\n");
        sprintf(logmsg,"Deleted Password for %s",site);
        log_activity(logmsg);
    }
    else 
    {
        printf("Not found\n");
    }
}

// ---------------- MAIN ----------------

int main() {
    srand(time(0));

    if(!master_exist())
        set_master_password();

    if(!login())
    {
        return 0;
    }


    int ch;

    while(1) {
        printf("\n1.Add 2.View 3.Search 4.Update 5.Delete 6.Restore Backup 7.Exit\nChoice: ");
        scanf("%d", &ch);

        switch(ch) {
            case 1: add_password(); break;
            case 2: view_passwords(); break;
            case 3: search_passwords(); break;
            case 4: update_passwords();break;
            case 5: delete_passwords(); break;
            case 6:restore_backup();break;
            case 7:log_activity("User Logged Out"); printf("Exited Successfully!\n"); return 0;
            default: printf("Invalid\n");
        }
    }
}