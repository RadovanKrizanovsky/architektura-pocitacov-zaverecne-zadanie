#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#define MESSAGES_LEN 4096

#pragma comment(lib, "Ws2_32.lib")

struct addrinfo *res = NULL, *ptr = NULL;
struct addrinfo sugg;

int num_mess = 1;
int answer;
int WSAStartup_fail = 0;
SOCKET ConnectSocket = INVALID_SOCKET;

char message[MESSAGES_LEN];
int received_len = MESSAGES_LEN;
char receive[MESSAGES_LEN];
char temp[50];

int num_o_spaces = 60;
int num_o_eqa = 120;
int line_len = 59;


void eqa(int num)
{
    for (int j = 0; j < num; j++)
    {
        putchar('=');
    }
}

void spaces(int num)
{
    for (int j = 0; j < num; j++)
    {
        putchar(' ');
    }
} //<------------------------------------------------------------------------------------------ (4.) STVRTA ULOHA (left/right)

void print_message(char mess[], int spaces_on)
{

    int i = 0;
    int j = 0;
    int word_len = 0;
    int pos = 0;


    while (mess[i] != '\0')
    {
        j = i;
        while (mess[j] != (char)32 && mess[j] != '\n' && mess[j] != '\0')
        {
            word_len++;
            j++;
        }

        if ((pos%line_len) == 0 || (pos + word_len) > line_len)
        {
            printf("\n");
            pos = 0;

            if (mess[i] == (char)32)
            {
                i++;
            }

            if (spaces_on == 1)
            {
                spaces(num_o_spaces);
            }
        }
        printf("%c", mess[i]);
        pos++;
        i++;
        j = 0;
        word_len = 0;
    }

    pos = 0;
    i = 0;

    printf("\n");

} //<---------------------------------------------------------------- (4.) STVRTA ULOHA (left/right)

void setup(char *ip, char *port)
{

    //Uvodne nastavenia
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    eqa(num_o_eqa);

    WSADATA Wdata;

    answer = WSAStartup(MAKEWORD(2, 2), &Wdata);

    ZeroMemory(&sugg, sizeof(sugg));
    sugg.ai_family = AF_UNSPEC;
    sugg.ai_socktype = SOCK_STREAM;
    sugg.ai_protocol = IPPROTO_TCP;

    answer = getaddrinfo(ip, port, &sugg, &res);
    if (answer != 0)
    {
        printf("\nGetaddrinfo fail: %d", answer);
        WSACleanup();
        WSAStartup_fail = 1;
    }
    else
    {
        printf("\nGetaddrinfo successful!");
    }
}

void getIp(char *temp)
{
    eqa(num_o_eqa);
    printf("\n");

    const char *preset_ip = "147.175.115.34";

    printf("IP:");
    scanf("%s", temp);
    if (strcmp(temp, "ip") == 0)
    {
        strcpy(temp, preset_ip);
    }
}

char getPort(char *temp)
{
    const char *preset_port = "777";

    printf("Port:");
    scanf("%s", temp);

    if (strcmp(temp, "port") == 0)
    {
        strcpy(temp, preset_port);
    }
}

void _connect()
{
    //Pripojenie sa
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ptr = res;

    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Error (socket: %ld)\n", WSAGetLastError());
        freeaddrinfo(res);
        WSACleanup();
    }
    else
    {
        printf("No error at socket");
    }

    getchar();
    answer = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

    if (answer == SOCKET_ERROR)
    {
        printf("Connection to server failed\n");
    }
    else
    {
        printf("Successful connection to server!\n");
    }

    getchar();

    if (answer == SOCKET_ERROR)
    {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        WSACleanup();
    }

    eqa(num_o_eqa);
    printf("BEGINING OF COMUNICATION\n");
    eqa(num_o_eqa);
    Sleep(250);
}

void send_to_server(char what_to_send[MESSAGES_LEN])
{
    //Posielanie dat
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    answer = send(ConnectSocket, what_to_send, (int)strlen(what_to_send), 0);

    if (answer == SOCKET_ERROR)
    {
        printf("Sending fail: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
    }

    printf("Total num of bytes sent: %ld\n", answer);

    eqa(num_o_eqa);
    printf("\n");
}

void receive_n_print()
{
    //Prijmanie dat
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    answer = recv(ConnectSocket, receive, received_len, 0);
    spaces(num_o_spaces);

    if (answer > 0 )
    {
        printf("Total num of bytes received: %d\n", answer);
    }
    else if (answer == 0 )
    {
        printf("Connection fail\n");
    }
    else
    {
        printf("recv fail (error: %d)\n", WSAGetLastError());
    }
    //Vypis a zavretie socketu
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    spaces(num_o_spaces);
    printf("Server message:\n");
    spaces(num_o_spaces);

    print_message(receive, 1);
    eqa(num_o_eqa);
    printf("\n");
}

void five_digit(int num)
{
    char temp_mess[MESSAGES_LEN];
    char digits[10];

    for (int i = 0; i < 6; i++)
    {
        int cis = num % 10;
        num = num / 10;

        digits[i] = cis;
    }

    num = 0;

    for (int i = 0; i < 5; i++)
    {
        num = num + digits[i];
    }

    if (digits[4] == 0)
    {
        num = num % 9;
    }
    else
    {
        num = num % digits[4];
    }



    sprintf(temp_mess,"%d", num);
    strcpy(message, temp_mess);
} //<-------------------------------------------------------------------------------------- (3.) TRETIA ULOHA (five digits)

void xor()
{
    for (int i = 0; i < 132; i++)
    {
        receive[i] = receive[i] ^ (char)55;
    }

    receive[132] = '\0';
    print_message(receive, 0);
} //<---------------------------------------------------------------------------------------------------- (5.) PIATA ULOHA (xor)

int is_prime(int c)
{
    for(int i = 2; i <= c / 2; i++)
    {
        if(c % i != 0)
        {
            continue;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

void prime()
{
    char temp[100];
    int i = 2;
    int j = 0;

    while(receive[i] != '\0')
    {
        if (is_prime(i) == 1)
        {
            temp[j] = receive[i - 1];
            j++;
        }
        i++;
    }
    temp[j] = '\0';
    strcpy(message, temp);
    message[j] = '\0';
    printf("prime number message: %s\n", temp);
} //<-------------------------------------------------------------------------------------------------- (6.) SIESTA ULOHA (prime)

void logfile()
{
    FILE * filePtr;
    filePtr = fopen("C:\\Users\\rakri\\Desktop\\Stu\\Predmety\\2.semester\\API\\Api ulohy\\BLOK 3\\posledne_zadanie\\communication.txt", "a");

    fprintf(filePtr, "(%d.)-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n", num_mess);
    fprintf(filePtr, "You:\n");
    fprintf(filePtr, message);
    fprintf(filePtr, "\n");
    fprintf(filePtr, "\n");
    fprintf(filePtr, "Server:\n");
    fprintf(filePtr, receive);
    fprintf(filePtr, "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(filePtr, "\n");
    fprintf(filePtr, "\n");

    num_mess++;

    fclose(filePtr);
} //<------------------------------------------------------------------------------------------------ (7.) SIEDMA ULOHA (file)

void cesar()
{

    char cipher[200];
    char direction[10];
    int rotation;

    printf("cipher:");
    scanf("%s", cipher);
    printf("direction:");
    scanf("%s", direction);
    printf("number of rotations:");
    scanf("%i", &rotation);

    int i = 0;
    int temp = 0;

    if (strcmp(direction, "right") == 0)
    {
        while (cipher[i] != '\0')
        {
            if ((int)cipher[i] + rotation > 90)
            {
                temp = (int)cipher[i] + rotation - 90;
                temp = 64 + temp;
                message[i] = (char)temp;
            }
            else
            {
                message[i] = cipher[i] + (char)rotation;
            }


            i++;
        }
    }

    if (strcmp(direction, "left") == 0)
    {
        while (cipher[i] != '\0')
        {
            if ((int)cipher[i] - rotation < 65)
            {
                temp = (int)cipher[i] - rotation;
                temp = 65 - temp;
                temp = 91 - temp;
                message[i] = (char)temp;
            }
            else
            {
                message[i] = cipher[i] + (char)rotation;
            }

            i++;
        }
    }

    message[i] = '\0';
    printf("deciphered message: %s\n", message);


}

void function_picker(char command[])
{
    if (strcmp(command, "five_digits") == 0)
    {
        five_digit(111328);
    }

    if (strcmp(command, "xor") == 0)
    {
        xor();
    }

    if (strcmp(command, "prime") == 0)
    {
        prime();
    }
    if (strcmp(command, "cesar") == 0)
    {
        cesar();
    }


}

int main()
{
    HANDLE cmd;
    cmd = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(cmd, 2); //<---------------------------------------------------------------------- (2.) DRUHA ULOHA (farba)

    char ip[50];
    char port[50];

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    getIp(temp);
    strcpy(ip, temp);
    printf("IP is: %s\n", ip);
    ////////////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    getPort(temp);
    strcpy(port, temp);
    printf("Port is: %s\n", port);
    getchar();
    printf("\n");
    ////////////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    setup(ip, port);

    if(WSAStartup_fail == 1)
    {
        return 1;
    }
    getchar();
    ////////////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    _connect();

    if (answer == SOCKET_ERROR)
    {
        return 1;
    }

    if (ConnectSocket == INVALID_SOCKET)
    {
        return 1;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////


    /////////////////////////////////////BEGINING OF COMMUNICATION//////////////////////////////////////
    int communication = 1;

    SetConsoleOutputCP(CP_UTF8); //<------------------------------------------------------------------------------------ (1.) PRVA ULOHA (diakritika)

    while (communication == 1)
    {
        printf("Your message:");
        scanf("%s", message);
        function_picker(message);

        if (strcmp(message, "stop") == 0)
        {
            communication = 0;
            closesocket(ConnectSocket);
            WSACleanup();
        }

        if (communication == 1)
        {
            send_to_server(message);

            if (answer == SOCKET_ERROR)
            {
                return 1;
            }
        }

        if (answer == SOCKET_ERROR)
        {
            return 1;
        }

        receive_n_print();
        logfile();
    }

    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
